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

//! Implementation of button handling routine
static void buttons_task([[maybe_unused]] void* params)
{
	// We need some structure to represent buttons and their debouncing state
	struct Button { u8 pin; u8 timer; u8 counter; } buttons[2];

	// Initialize buttons to be in "not debouncing" state
	buttons[0].pin = BTN_LEFT_PIN;
	buttons[0].timer = 0;
	buttons[1].pin = BTN_RIGHT_PIN;
	buttons[1].timer = 0;

	// In intervals check pins state and perform debouncing if needed
	while(true)
	{
		// Idle wait for any of pins to be touched
		const auto touched_pins = gpio_wait(BTNS_PINS);

		// We exactly know, what pins are for sure to be debounced
		//  (others may come in the near future, while debouncing)
		for(auto& button : buttons)
		{
			// If that pin was touched, start debouncing
			if(touched_pins & button.pin) {
				button.counter = 0;
				button.timer = 25;
			}
		}

		// Perform debouncing as long as it is needed
		auto nextwaketime = xTaskGetTickCount();
		while(true) {
			// Wait for next interval
			vTaskDelayUntil(&nextwaketime, pdMS_TO_TICKS(4));

			// Read state of the pins with the buttons
			const auto pins_states = GPIOF->DATA[BTNS_PINS];

			// Check state of each pin and handle debouncing, if needed
			auto pressed_pins = 0x00;
			auto debounce_in_progress = false;
			for(auto& [pin, timer, counter] : buttons)
			{
				// Check if pin is debouncing or was just pressed and needs one
				const auto is_debouncing = (timer > 0);
				const auto is_not_pressed = (pins_states & pin);
				if(is_debouncing) 
				{
					// Count votes for "not_pressed" state
					if(is_not_pressed) { ++counter; }

					// Check if it needs still debouncing
					if(--timer != 0)
					{
						debounce_in_progress = true;
						continue;
					}

					// Debouncing finished. Determine final state of button
					if(counter < 25/4) {
						// Less than 25% of votes for "not pressed" => pressed
						pressed_pins |= pin;
					} 
				}
				else if(!is_not_pressed) {
					// Pin was just pressed. Start debouncing of it
					counter = 0;
					timer = 25;
					debounce_in_progress = true;
				}
			}

			// If we have any new pressed buttons, write them to the queue
			if(pressed_pins) {
				const auto tickstowait = 0U;
				CHECK(xQueueSend(buttons_queue, &pressed_pins, tickstowait));
			}

			// If none of the pins need debouncing, break loop and wait for interrupt
			if(!debounce_in_progress) {
				break;
			}
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