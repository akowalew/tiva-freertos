///////////////////////////////////////////////////////////////////////////////
// Buttons management
///////////////////////////////////////////////////////////////////////////////

//
// Global variables
//

//! Queue to store buttons presses events
static QueueHandle_t buttons_queue = nullptr;

//
// Private functions
//

static void buttons_task([[maybe_unused]] void* params)
{
	while(true)
	{
		// Wait for any pin to be pressed
		const auto touched_pins = gpio_wait(BTNS_PINS);
		assert(touched_pins != 0x00);
		assert((touched_pins & ~BTNS_PINS) == 0x00);

		// Perform debouncing. Read pins 25 times in 4ms intervals
		u8 pins_states[25]; 
		auto nextwaketime = xTaskGetTickCount();
		for(u8 i = 0; i < 25; ++i)
		{
			pins_states[i] = GPIOF->DATA[touched_pins];
			vTaskDelayUntil(&nextwaketime, pdMS_TO_TICKS(4));
		}

		// Check buttons presses based on readed pins states 
		u8 pressed_pins = 0x00;
		for(u8 j = 0, mask = 0x01; j < 8; ++j, mask <<= 1) 
		{
			if(!(touched_pins & mask)) {
				// This is not any of touched pins, go to next 
				continue;
			}

			// Count hits for that pins
			u16 counter = 0;
			for(u8 i = 0; i < 25; ++i) {
				counter += (u16)(pins_states[i] & mask);
			}

			// Determine, if button is pressed
			counter >>= j;
			if(counter < 25/4) {
				// Buttons are active LOW, so we are counting 0's
				pressed_pins |= mask;
			}
		}

		if(pressed_pins) {
			// Send pressed pins to the queue
			const auto tickstowait = 0U;
			CHECK(xQueueSend(buttons_queue, &pressed_pins, tickstowait));
		}
	}
}

//
// Public functions
//

void buttons_init()
{
	// Create queue for button press events
	const auto queue_size = 8;
	CHECK(buttons_queue = xQueueCreate(queue_size, sizeof(u8)));

	// Create task for buttons handling
	const auto stacksize = configMINIMAL_STACK_SIZE;
	const auto params = nullptr;
	const auto priority = (tskIDLE_PRIORITY + 1);
	CHECK(xTaskCreate(buttons_task, "buttons", stacksize, params, priority, nullptr));
}

u8 buttons_read()
{
	u8 pressed_pins;
	// Read in blocking way press events from the queue
	CHECK(xQueueReceive(buttons_queue, &pressed_pins, portMAX_DELAY));
	CHECK(pressed_pins != 0x00);

	// Return what buttons was pressed
	return pressed_pins;
}