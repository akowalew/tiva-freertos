///////////////////////////////////////////////////////////////////////////////
// Main application routine
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string.h>

#include "types.cpp"
#include "reset.cpp"
#include "stack.cpp"
#include "utils.cpp"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "hw_sysctl.h"
#include "hw_gpio.h"
#include "hw_uart.h"
#include "hw_hibernate.h"

#include "nvic.cpp"
#include "sysctl.cpp"
#include "gpio.cpp"
#include "uart.cpp"
#include "hibernate.cpp"
#include "leds.cpp"
#include "buttons.cpp"

#include "cli.cpp"
#include "ui.cpp"

#include "handlers.cpp"

//
// Main application routine
//

int main()
{
	// NOTE: HardFault debug purpose only!
	// Disable write cache, IT folding and interrupts on multicycle instructions
	// *(volatile u32*)(0xE000E008) |= 0x7;

	// Hardware initialization
	sys_init();
	gpio_init();
	uart_init();
	hib_init();
	buttons_init();
	nvic_init();

	// Software initialization
	cli_init();
	ui_init();

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
