///////////////////////////////////////////////////////////////////////////////
// User Inteface module
///////////////////////////////////////////////////////////////////////////////

//
// Private functions
//

const auto BIG_DIGIT_HEIGHT = 4;
const auto BIG_DIGIT_WIDTH = 16;
const auto SMALL_DIGIT_HEIGHT = 2;
const auto SMALL_DIGIT_WIDTH = 8;

void draw_big_digit(u8 digit, u8 x, u8 y)
{
	for(u8 j = 0; j < BIG_DIGIT_HEIGHT; ++j) 
		memcpy(&ssd1306_data[y+j][x], &digits_big[digit][j][0], BIG_DIGIT_WIDTH);
}

void draw_small_digit(u8 digit, u8 x, u8 y) {
	for(u8 j = 0; j < SMALL_DIGIT_HEIGHT; ++j) 
		memcpy(&ssd1306_data[y+j][x], &digits_small[digit][j][0], SMALL_DIGIT_WIDTH);
}

void draw_colon(u8 x, u8 y) {
	for(u8 j = 0; j < BIG_DIGIT_HEIGHT; ++j) 
		memcpy(&ssd1306_data[y+j][x], &colon[j][0], BIG_DIGIT_WIDTH);
}

static void ui_task([[maybe_unused]] void* params)
{
	ssd1306_startup();
	
	// Static clock's digits display configuration
	constexpr auto big_y = 2;
	constexpr auto small_y = 4;
	constexpr auto spacing_big = 4;
	constexpr auto spacing_small = 2;
	constexpr auto hour_hi_x = 8;
	constexpr auto hour_lo_x = hour_hi_x+BIG_DIGIT_WIDTH+spacing_big;
	constexpr auto colon_x = hour_lo_x+BIG_DIGIT_WIDTH+spacing_big;
	constexpr auto minute_hi_x = colon_x+BIG_DIGIT_WIDTH+spacing_big;
	constexpr auto minute_lo_x = minute_hi_x+BIG_DIGIT_WIDTH+spacing_big;
	constexpr auto second_hi_x = minute_lo_x+BIG_DIGIT_WIDTH+spacing_big;
	constexpr auto second_lo_x = second_hi_x+SMALL_DIGIT_WIDTH+spacing_small;

	// Colon will not change, so we can draw it to the buffer only once
	draw_colon(colon_x, big_y);

	while(true)
	{
		// We would like to know, if some digit really changed, to redraw it
		// NOTE: Maybe 4-bits per digit, instead of 8? Not sure of performance
		u8 prev_hour = 255;
		u8 prev_minute = 255;
		u8 prev_hour_hi = 255;
		u8 prev_hour_lo = 255;
		u8 prev_minute_hi = 255;
		u8 prev_minute_lo = 255;
		u8 prev_second_hi = 255;

		// For some duration display current time
		auto nextwaketime = xTaskGetTickCount();
		for(u8 i = 0; i < 5; ++i)
		{
			const auto time = hib_rtc_seconds();

			// Convert number of seconds to format HH:MM:SS
			const u8 hour = ((time / 3600) % 24);
			const u8 minute = ((time / 60) % 60);
			const u8 second = (time % 60);

			// Only draw hours, when they changed
			if(hour != prev_hour) {
				const auto hour_hi = (hour / 10);
				if(hour_hi != prev_hour_hi) {
					draw_big_digit(hour_hi, hour_hi_x, big_y);
					prev_hour_hi = hour_hi;
				}

				const auto hour_lo = (hour % 10);
				if(hour_lo != prev_hour_lo) {
					draw_big_digit(hour_lo, hour_lo_x, big_y);
					prev_hour_lo = hour_lo;
				}

				prev_hour = hour;
			}

			// Only draw minutes, when they changed
			if(minute != prev_minute) {
				const auto minute_hi = (minute / 10);
				if(minute_hi != prev_minute_hi) {
					draw_big_digit(minute_hi, minute_hi_x, big_y);
					prev_minute_hi = minute_hi;
				}

				const auto minute_lo = (minute % 10);
				if(minute_lo != prev_minute_lo) {
					draw_big_digit(minute_lo, minute_lo_x, big_y);
					prev_minute_lo = minute_lo;
				}

				prev_minute = minute;
			}

			// Higher digit of seconds may change, but not often
			const auto second_hi = (second / 10);
			if(second_hi != prev_second_hi) {
				draw_small_digit(second_hi, second_hi_x, small_y);
				prev_second_hi = second_hi;
			}

			// Lower digit of seconds will for sure change...
			const auto second_lo = (second % 10);
			draw_small_digit(second_lo, second_lo_x, small_y);

			// Write changes to the display
			ssd1306_write();

			// Wait for the next second
			// NOTE: It can be done with RTC actually, but it brings some complications...
			vTaskDelayUntil(&nextwaketime, pdMS_TO_TICKS(1000));
		}

		// Let the display be black
		// TODO: The best will be simply to put display to sleep 
		//       and then wake it up.
		ssd1306_clear();
		ssd1306_write();

		// Wait for any button to be pressed
		const auto buttons = buttons_read();
		assert(buttons != 0x00);
	}
}

//
// Public functions
//

void ui_init()
{
	const auto stacksize = configMINIMAL_STACK_SIZE;
	const auto params = nullptr;
	const auto priority = (tskIDLE_PRIORITY + 1);
	CHECK(xTaskCreate(ui_task, "ui", stacksize, params, priority, nullptr));
}