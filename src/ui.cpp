///////////////////////////////////////////////////////////////////////////////
// User Inteface module
///////////////////////////////////////////////////////////////////////////////

//
// Private functions
//

static void ui_task([[maybe_unused]] void* params)
{
	ssd1306_startup();
	
	bool show_colon = false;
	while(true)
	{
		ssd1306_clear();
		const auto time = hib_rtc_seconds();

		const u8 hour = ((time / 3600) % 24);
		const u8 minute = ((time / 60) % 60);
		const u8 second = (time % 60);

		const u8 hour_hi = (hour / 10);
		const u8 hour_lo = (hour % 10);

		const u8 minute_hi = (minute / 10);
		const u8 minute_lo = (minute % 10);

		const u8 second_hi = (second / 10);
		const u8 second_lo = (second % 10);

		u8 y = 2;
		u8 x = 8;
		u8 spacing_big = 4;
		u8 spacing_small = 2;

		for(u8 j = 0; j < 4; ++j) 
			memcpy(&ssd1306_data[j+y][x], &digits_big[hour_hi][j][0], 16);
		x += (16 + spacing_big);

		for(u8 j = 0; j < 4; ++j) 
			memcpy(&ssd1306_data[j+y][x], &digits_big[hour_lo][j][0], 16);
		x += (16 + spacing_big);

		if((show_colon = !show_colon))
			for(u8 j = 0; j < 4; ++j) 
				memcpy(&ssd1306_data[j+y][x], &colon[j][0], 16);
		x += (16 + spacing_big);

		for(u8 j = 0; j < 4; ++j) 
			memcpy(&ssd1306_data[j+y][x], &digits_big[minute_hi][j][0], 16);
		x += (16 + spacing_big);

		for(u8 j = 0; j < 4; ++j) 
			memcpy(&ssd1306_data[j+y][x], &digits_big[minute_lo][j][0], 16);
		x += (16 + spacing_big);

		for(u8 j = 0; j < 2; ++j) 
			memcpy(&ssd1306_data[j+y+2][x], &digits_small[second_hi][j][0], 8);
		x += (8 + spacing_small);

		for(u8 j = 0; j < 2; ++j) 
			memcpy(&ssd1306_data[j+y+2][x], &digits_small[second_lo][j][0], 8);
		x += (8 + spacing_small);

		ssd1306_write();
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