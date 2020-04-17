///////////////////////////////////////////////////////////////////////////////
// UART management
///////////////////////////////////////////////////////////////////////////////

struct UART_Block
{
	//! Data
	RW u32 DR;

	//! Receive Status/Error Clear
	union {
		RO u32 RSR;
		WO u32 ECR;
	};

	RO u32 _reserved1[0x4];

	//! Flag
	RO u32 FR;

	RO u32 _reserved2[0x1];

	//! IrDA Low-Power Register
	RW u32 ILPR;

	//! Integer Baud-Rate Divisor
	RW u32 IBRD;

	//! Fractional Baud-Rate Divisor
	RW u32 FBRD;

	//! Line Control
	RW u32 LCRH;

	//! Control
	RW u32 CTL;

	//! Interrupt FIFO Level Select
	RW u32 IFLS;

	//! Interrupt Mask
	RW u32 IM;

	//! Raw Interrupt Status
	RO u32 RIS;

	//! Masked Interrupt Status
	RO u32 MIS;

	//! Interrupt Clear
	W1C u32 ICR;

	//! DMA Control
	RW u32 DMACTL;

	RO u32 _reserved3[0x16];

	//! 9-Bit Self Address
	RW u32 BIT9ADDR;

	//! 9-Bit Self Address Mask
	RW u32 BIT9AMASK;

	RO u32 _reserved4[0x3C5];

	//! Peripheral Properties
	RO u32 PP;

	RO u32 _reserved5[0x1];

	//! Clock Configuration 
	RW u32 CC;
};

static_assert(offsetof(UART_Block, DR) == 0x000);
static_assert(offsetof(UART_Block, RSR) == 0x004);
static_assert(offsetof(UART_Block, ECR) == 0x004);
static_assert(offsetof(UART_Block, FR) == 0x018);
static_assert(offsetof(UART_Block, ILPR) == 0x020);
static_assert(offsetof(UART_Block, IBRD) == 0x024);
static_assert(offsetof(UART_Block, FBRD) == 0x028);
static_assert(offsetof(UART_Block, LCRH) == 0x02C);
static_assert(offsetof(UART_Block, CTL) == 0x030);
static_assert(offsetof(UART_Block, IFLS) == 0x034);
static_assert(offsetof(UART_Block, IM) == 0x038);
static_assert(offsetof(UART_Block, RIS) == 0x03C);
static_assert(offsetof(UART_Block, MIS) == 0x040);
static_assert(offsetof(UART_Block, ICR) == 0x044);
static_assert(offsetof(UART_Block, DMACTL) == 0x048);
static_assert(offsetof(UART_Block, BIT9ADDR) == 0x0A4);
static_assert(offsetof(UART_Block, BIT9AMASK) == 0x0A8);
static_assert(offsetof(UART_Block, PP) == 0xFC0);
static_assert(offsetof(UART_Block, CC) == 0xFC8);

#define UART0 ((volatile UART_Block*)(0x4000C000))
#define UART1 ((volatile UART_Block*)(0x4000D000))
#define UART2 ((volatile UART_Block*)(0x4000E000))
#define UART3 ((volatile UART_Block*)(0x4000F000))
#define UART4 ((volatile UART_Block*)(0x40010000))
#define UART5 ((volatile UART_Block*)(0x40011000))
#define UART6 ((volatile UART_Block*)(0x40012000))
#define UART7 ((volatile UART_Block*)(0x40013000))

//
// Global variables
//

static QueueHandle_t rx_queue = nullptr;

static volatile const char* tx_string;
static volatile const char* tx_string_end;
static volatile TaskHandle_t tx_task;

//
// Public functions
//

void uart_init()
{
	//
	// Global data initialization
	//

	const auto rx_queue_size = 8;
	CHECK(rx_queue = xQueueCreate(rx_queue_size, sizeof(char)));

	tx_task = nullptr;
	tx_string = nullptr;
	tx_string_end = nullptr;

	//
	// Baud-rate calculation
	//

	// The baud-rate divisor is a 22-bit number consisting of 
	//  a 16-bit integer and a 6-bit fractional part.
	// BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
	// UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
	// ClkDiv is either 8 or 16 (HSE=1 or HSE=0)
	constexpr u32 BAUDRATE = 115200;
	constexpr float BAUDRATE_DIV = configCPU_CLOCK_HZ / (8.0 * BAUDRATE);

	constexpr u16 BAUDRATE_DIVI = BAUDRATE_DIV;
	static_assert(BAUDRATE_DIVI > 0);

	constexpr u8 BAUDRATE_DIVF = ((BAUDRATE_DIV-BAUDRATE_DIVI) * 64 + 0.5); 
	static_assert(BAUDRATE_DIVF > 0);

	//
	// Initialize hardware
	//

	// Disable UART first
	UART0->CTL &= ~UART_CTL_UARTEN;

	// Wait for end of transmission or reception of the current character
	while(UART0->FR & UART_FR_BUSY);

	// Flush the transmit FIFO if it was previously used and there are any chars
	UART0->LCRH &= ~UART_LCRH_FEN;

	// Write the integer portion of the BRD to the UARTIBRD register.
	UART0->IBRD = BAUDRATE_DIVI;

	// Write the fractional portion of the BRD to the UARTFBRD register.
	UART0->FBRD = BAUDRATE_DIVF;

	// 8-bit, Parity none, 1 stop bit, do NOT use FIFOs
	UART0->LCRH = (UART_LCRH_WLEN_8);

	// Use system clock
	UART0->CC = UART_CC_CS_SYSCLK;

	// Clear any pending UART interrupts
	UART0->ICR = 0xFFFFFFFF;

	// Receive interrupts will be enabled always
	// even if nobody is reading now
	UART0->IM = (UART_IM_RXIM | UART_IM_TXIM);

	// Enable transmitter, receiver and use high speed mode
	UART0->CTL = (UART_CTL_HSE | UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN);
}

u32 uart_read_until(char* string, u32 size, char delimiter)
{
	assert(string != nullptr);
	assert(size > 0);

	// Note there is no mutual exclusion at the driver level. If more than one
	// task is using the serial port then mutual exclusion should be provided 
	// where this function is called. 

	// Perform read of the string
	u32 i;
	for(i = 0; i < size; ++i) {
		// Obtain a received character from the queue - entering the Blocked state
		// (so not consuming any processing time) to wait for a character 
		// if one is not already available.
		char c;
		CHECK(xQueueReceive(rx_queue, &c, portMAX_DELAY));
		if(c == delimiter) {
			// We have reached delimiter, we have to stop further reading
			break;
		}

		// Store read char in destination string
		string[i] = c;
	}

	// Return number of characters read
	return i;
}

void uart_write(const char* string, u32 size)
{
	assert(string != nullptr);
	assert(size > 0);

	// Note there is no mutual exclusion at the driver level. If more than one
	// task is using the serial port then mutual exclusion should be provided 
	// where this function is called. 

	// Ensure notifications are not already waiting
	ulTaskNotifyTake(pdTRUE, 0);

	// Just to be sure, previous write must be done completely
	#ifndef NDEBUG
		NVIC_CRITICAL_SECTION_ENTER(INT_UART0);
		{
			assert(tx_task == nullptr);
			assert(tx_string == tx_string_end);
		}
		NVIC_CRITICAL_SECTION_LEAVE(INT_UART0);
	#endif

	// Remember which task is sending
	tx_task = xTaskGetCurrentTaskHandle();

	// Store the rest of the string to send
	tx_string = (string);
	tx_string_end = (string + size);

	// Start string sending
	// We are doing it right before going to sleep in order to reduce 
	// amount of time between this write and further interrupt
	UART0->DR = *string;

	// Wait for string to be send
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

// Overloaded version, handling strings with compile-time known size
template<u32 N>
void uart_write(char const (&data)[N])
{
	uart_write(data, N);
}

// The transmit interrupt changes state when one of the following events occurs:
// - If the FIFOs are enabled and the transmit FIFO progresses through the programmed trigger
// level, the TXRIS bit is set. The transmit interrupt is based on a transition through level, therefore
// the FIFO must be written past the programmed trigger level otherwise no further transmit interrupts
// will be generated. The transmit interrupt is cleared by writing data to the transmit FIFO until it
// becomes greater than the trigger level, or by clearing the interrupt by writing a 1 to the TXIC bit.
// - If the FIFOs are disabled (have a depth of one location) and there is no data present in the
// transmitters single location, the TXRIS bit is set. It is cleared by performing a single write to the
// transmit FIFO, or by clearing the interrupt by writing a 1 to the TXIC bit.
// - If the EOT bit is set, the last bit of all transmitted data and flags
// has left the serializer.

void UART0_handler()
{
	// We need to know, if some of the operations done here may 
	// unblock task with higher priority.
	auto highpriotask_woken = pdFALSE;

	// Latch status of occured UART interrupts
	const auto masked_status = UART0->MIS;

	// Handle Tx interrupt if there is any
	if(masked_status & UART_MIS_TXMIS)
	{	
		const auto string = (++tx_string);
		if(string < tx_string_end) 
		{
			// There is still some string to send. 
			// Put next char and advance string pointer
			// Interrupt cause will be automatically cleared
			UART0->DR = *string;
		}
		else
		{
			// No more characters to send. Clear interrupt cause
			UART0->ICR = UART_ICR_TXIC;

			// Notify transmit task, if it is waiting
			assert(tx_task != nullptr);
			vTaskNotifyGiveFromISR(tx_task, &highpriotask_woken);
			#ifndef NDEBUG
				tx_task = nullptr;
			#endif
		}
	}

	// Handle Rx interrupt if there is any
	if(masked_status & UART_MIS_RXMIS)
	{
		// Retrieve read character thus clearing interrupt cause
		const auto c = UART0->DR;

		// Send the character to the queue.
		// If writing to the queue unblocks a task, and the unblocked task has a
		// priority above the currently running task (the task that this interrupt
		// interrupted), then highpriotask_woken will be set to pdTRUE inside the
		// xQueueSendFromISR() function. highpriotask_woken is then passed to
		// portYIELD_FROM_ISR() at the end of this interrupt handler to request a
		// context switch so the interrupt returns directly to the (higher priority)
		// unblocked task.
		xQueueSendFromISR(rx_queue, &c, &highpriotask_woken);
	}

	/* portYIELD_FROM_ISR() will request a context switch if executing this
	interrupt handler caused a task to leave the blocked state, and the task
	that left the blocked state has a higher priority than the currently running
	task (the task this interrupt interrupted).  See the comment above the calls
	to xSemaphoreGiveFromISR() and xQueueSendFromISR() within this function. */
	portYIELD_FROM_ISR(highpriotask_woken);
}
