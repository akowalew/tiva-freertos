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

// 14452 14444 14312
// 7400 7388
static void buttons_task([[maybe_unused]] void* params)
{
	// We need some structure to represent buttons and theirs debouncing state
	struct Button { u8 pin; u8 timer; u8 counter; } buttons[2];

	// Initialize buttons to be in "not debouncing" state
	buttons[0].pin = BTN_LEFT_PIN;
	buttons[0].timer = 0;
	buttons[1].pin = BTN_RIGHT_PIN;
	buttons[1].timer = 0;

	// In intervals check pins state and perform debouncing if needed
	auto nextwaketime = xTaskGetTickCount();
	while(true)
	{
		// Wait for next interval
		vTaskDelayUntil(&nextwaketime, pdMS_TO_TICKS(4));

		// Read state of the pins with the buttons
		const auto pins_states = GPIOF->DATA[BTNS_PINS];

		// Check state of each pin and handle debouncing, if needed
		u8 pressed_pins = 0x00;
		for(auto& [pin, timer, counter] : buttons)
		{
			// Check if pin is debouncing or is just pressed and needs one
			const auto is_debouncing = (timer > 0);
			const auto is_pressed = (!(pins_states & pin));
			if(is_debouncing) 
			{
				// If button was pressed, it will have low-valued counter
				if(is_pressed) { --counter; }

				// Check if debouncing is completed
				if(--timer == 0)
				{
					// Perform voting to determine final state of the button
					if(counter < 25/4) {
						// Less than 25% of votes for "not pressed" => pressed
						pressed_pins |= pin;
					} 
				}
			}
			else if(is_pressed) {
				// Pin was just pressed. Start debouncing of it
				counter = 25;
				timer = 25;
			}
		}

		// If we have any new pressed buttons, write them to the queue
		if(pressed_pins) {
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