///////////////////////////////////////////////////////////////////////////////
// User Inteface module
///////////////////////////////////////////////////////////////////////////////

//
// Private functions
//

static void ui_task([[maybe_unused]] void* params)
{
	// Read inputs
	// Write outputs

	// If left pressed, invert screen
	// If right pressed, show date/time/seconds

	while(true)
	{
		// Wait for button press
		const auto pressed_pins = buttons_read();
		assert(pressed_pins != 0x00);

		// Toggle LED to signal button press
		GPIOF->DATA[BLUE_LED_PIN] ^= 0xFF;
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