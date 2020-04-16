///////////////////////////////////////////////////////////////////////////////
// Command Line Interface module
///////////////////////////////////////////////////////////////////////////////

//
// Command Line Interface task
//

static void cli_task([[maybe_unused]] void *params)
{
	// Print some welcome string
	uart_write("En taro Adun, Executor!\n");

	// We need some string buffers to read/write data
	char rx_string[32];
	char tx_string[32];

	// In loop read commands from UART, parse them and write responses
	while(true)
	{
		// Read command from UART until CarriageReturn happen
		// Minicom uses that when ENTER key is hit.
		auto rx_count = uart_read_until(rx_string, sizeof(rx_string), '\r');

		// Signal that we've received data
		leds_flash(GREEN_LED_PIN);

		// Process received data
		if((rx_count == 4) && (memcmp(rx_string, "time", 4) == 0))
		{
			// "time" command received. 
			// Since there is only one command available, handle it right here

			// Get RTC seconds, parse to string and write back
			const auto seconds = hib_rtc_seconds();

			// `to_digits_ascii` writes characters "from back" of the buffer
			// and we would like to insert NewLine character to the end, so '-1'
			const auto tx_string_end = (tx_string + sizeof(tx_string));
			const auto tx_string_begin = to_digits_ascii(seconds, tx_string_end - 1);
			*(tx_string_end - 1) = '\n';

			// Write prepared string to UART
			const auto tx_count = (tx_string_end - tx_string_begin);
			uart_write(tx_string_begin, tx_count);
		}
		else {
			uart_write("Invalid command\n");
		}

		// Signal that we've transmitted data
		leds_flash(RED_LED_PIN);
	} 
}

static void cli_init()
{
	const auto cli_stacksize = configMINIMAL_STACK_SIZE;
	const auto cli_params = nullptr;
	const auto cli_priority = (tskIDLE_PRIORITY + 1);
	CHECK(xTaskCreate(cli_task, "cli", cli_stacksize, cli_params, cli_priority, nullptr));
}