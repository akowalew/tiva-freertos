///////////////////////////////////////////////////////////////////////////////
// Main application routine
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "types.cpp"
#include "reset.cpp"
#include "stack.cpp"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "hw_sysctl.h"
#include "hw_gpio.h"
#include "hw_uart.h"

#include "utils.cpp"
#include "sysctl.cpp"
#include "gpio.cpp"
#include "uart.cpp"
#include "nvic.cpp"

#include "handlers.cpp"

// //
// // Reader task
// //

// static void rx_task([[maybe_unused]] void *params)
// {
// 	while(true)
// 	{
// 		// Get message from writer
// 		u32 recv_value;
// 		CHECK(xQueueReceive(queue, &recv_value, portMAX_DELAY));
// 		CHECK(recv_value == 100UL);

// 		// Flash the receive LED for a while
// 		GPIOF->DATA[GREEN_LED_PIN] ^= GREEN_LED_PIN;
// 		vTaskDelay(pdMS_TO_TICKS(100));
// 		GPIOF->DATA[GREEN_LED_PIN] ^= GREEN_LED_PIN;
// 		vTaskDelay(pdMS_TO_TICKS(100));
// 	}

// 	while(1);
// }

//
// Writer task
//

static void writer_task([[maybe_unused]] void *params)
{
	auto nextwaketime = xTaskGetTickCount();

	while(true)
	{
		// Flash the transmit LED for a while
		GPIOF->DATA[RED_LED_PIN] ^= RED_LED_PIN;
		vTaskDelay(pdMS_TO_TICKS(100));
		GPIOF->DATA[RED_LED_PIN] ^= RED_LED_PIN;
		vTaskDelay(pdMS_TO_TICKS(100));

		// Send some string through UART
		uart_write("En taro Adun, Executor!\n");
		
		// Keep fixed update rate
		vTaskDelayUntil(&nextwaketime, pdMS_TO_TICKS(1000));
	} 
}

//
// Main application routine
//

int main()
{
	// Disable write cache, IT folding and interrupts on multicycle instructions
	*(volatile u32*)(0xE000E008) |= 0x7;

	// Hardware initialization
	sys_init();
	gpio_init();
	uart_init();
	nvic_init();

	// Writer task
	const auto writer_stacksize = configMINIMAL_STACK_SIZE;
	const auto writer_params = nullptr;
	const auto writer_priority = (tskIDLE_PRIORITY + 1);
	CHECK(xTaskCreate(writer_task, "writer", writer_stacksize, writer_params, writer_priority, nullptr));

	// // Receive task
	// const auto rx_stacksize = configMINIMAL_STACK_SIZE;
	// const auto rx_params = nullptr;
	// const auto rx_priority = (tskIDLE_PRIORITY + 2);
	// CHECK(xTaskCreate(rx_task, "rx", rx_stacksize, rx_params, rx_priority, nullptr));

	// Run kernel
	vTaskStartScheduler();

	// We should not reach that line
	while(true);
}

//
// Free RTOS hooks
//

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	/* Check pcTaskName for the name of the offending task, or pxCurrentTCB
	if pcTaskName has itself been corrupted. */
	( void ) pxTask;
	( void ) pcTaskName;
	for( ;; );
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
