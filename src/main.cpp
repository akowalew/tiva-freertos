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

#include "handlers.cpp"

// Generic hook to be called, when CHECK(...) macro detects failure
template<typename T>
[[noreturn]] void check_failed(const char* file, const char* function, int line, T result)
{
	static_cast<void>(file);
	static_cast<void>(function);
	static_cast<void>(line);
	static_cast<void>(result);

	while(1);
}

//! Helper macro, hangs if specified condition is failed (=false)
#define CHECK(x) if(const auto result = (x); !result) check_failed(__FILE__, __FUNCTION__, __LINE__, result)

// Queue to be used between tx_task and rx_task
static QueueHandle_t queue = NULL;

// Reader task. Just receives data from the queue
static void rx_task([[maybe_unused]] void *params)
{
	while(true)
	{
		u32 recv_value;
		CHECK(xQueueReceive(queue, &recv_value, portMAX_DELAY));
		CHECK(recv_value == 100UL);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}

	while(1);
}

// Writer task. Just sends data to the queue
static void tx_task([[maybe_unused]] void *params)
{
	auto nextwaketime = xTaskGetTickCount();

	while(true)
	{
		vTaskDelayUntil(&nextwaketime, pdMS_TO_TICKS(1000));

		const auto value = 100UL;
		const auto tickstowait = 0U;
		CHECK(xQueueSend(queue, &value, tickstowait));
	} 
}

//
// Main application routine
//

int main()
{
	// Create queue to be used by tasks
	const auto QUEUE_LENGTH = 1;
	CHECK(queue = xQueueCreate(QUEUE_LENGTH, sizeof(u32)));

	// Send task
	const auto tx_stacksize = configMINIMAL_STACK_SIZE;
	const auto tx_params = nullptr;
	const auto tx_priority = (tskIDLE_PRIORITY + 1);
	CHECK(xTaskCreate(tx_task, "tx", tx_stacksize, tx_params, tx_priority, nullptr));

	// Receive task
	const auto rx_stacksize = configMINIMAL_STACK_SIZE;
	const auto rx_params = nullptr;
	const auto rx_priority = (tskIDLE_PRIORITY + 2);
	CHECK(xTaskCreate(rx_task, "rx", rx_stacksize, rx_params, rx_priority, nullptr));

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
