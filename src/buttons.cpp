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
	buttons[0].pin = BTN_LEFT_PIN;
	buttons[1].pin = BTN_RIGHT_PIN;

	while(true)
	{
		// Wait for any pin to be touched
		gpio_wait(BTNS_PINS);

		// Some pins have been touched. Begin debouncing
		auto nextwaketime = xTaskGetTickCount();
		auto debounce_pins = 0x00;
		while(true)
		{
			// Check, if some pins was touched recently
			const auto next_debounce_pins = gpio_get();
			if(next_debounce_pins) {
				// Initialize them for debouncing
				for(auto& button : buttons) {
					if(next_debounce_pins & button.pin) {
						button.timer = 16;
						button.counter = 0;
					}
				}

				debounce_pins |= next_debounce_pins;
			}

			// Latch state of pins
			const auto pins_states = GPIOF->DATA[debounce_pins];

			// Handle debouncing
			auto finished_pins = 0x00;
			auto pressed_pins = 0x00;
			for(auto& [pin, timer, counter] : buttons) {
				if(!(debounce_pins & pin)) {
					// Pin is not under debouncing, go to next one
					continue;
				}

				// Perform voting for "not_pressed"
				const auto is_not_pressed = (pins_states & pin);
				if(is_not_pressed) { ++counter; }

				// If debouncing has finished, detect state of the pin
				if(--timer == 0) {
					if(counter < 16/4) {
						pressed_pins |= pin;
					}

					finished_pins |= pin;
				}
			}

			// If there were detected pressed pins, send them to the queue
			if(pressed_pins) {
				CHECK(xQueueSend(buttons_queue, &pressed_pins, 0));
			}

			// Do we have some buttons, that have just end debouncing?
			if(finished_pins) {
				// Re-enable interrupts for them
				gpio_enable(finished_pins);

				// Exclude them from debouncing
				debounce_pins &= ~finished_pins;
				if(!debounce_pins)
				{
					// No more pins to debounce, we have to wait for interrupt! 
					break;
				}
			}

			// Wait for next interval
			vTaskDelayUntil(&nextwaketime, pdMS_TO_TICKS(4));
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