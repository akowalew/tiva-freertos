///////////////////////////////////////////////////////////////////////////////
// Main application routine
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "types.cpp"
#include "reset.cpp"
#include "stack.cpp"

#include "FreeRTOS.h"
#include "task.h"

#include "handlers.cpp"

int main()
{
	vTaskStartScheduler();

	// We should not reach that line
	while(true);
}