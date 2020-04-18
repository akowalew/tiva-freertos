///////////////////////////////////////////////////////////////////////////////
// GPIO management
///////////////////////////////////////////////////////////////////////////////

// Taken from TM4C123GH6PM datasheet:
// 
// All GPIO signals are 5-V tolerant when configured as inputs
// except for PD4, PD5, PB0 and PB1, which are limited to 3.6 V.

// GPIO Pins With Special Considerations
// -------------------------------------------------------------
// Pins    | Reset State | AFSEL | DEN | PDR | PUR | PCTL | CR |
// -------------------------------------------------------------
// PA[1:0] | UART0       |   0   |  0  |  0  |  0  |  0x1 |  1 |
// PA[5:2] | SSI0        |   0   |  0  |  0  |  0  |  0x2 |  1 |
// PB[3:2] | I2C0        |   0   |  0  |  0  |  0  |  0x3 |  1 |
// PC[3:0] | JTAG/SWD    |   1   |  1  |  0  |  1  |  0x1 |  0 |
// PD[7]   | GPIO        |   0   |  0  |  0  |  0  |  0x0 |  0 |
// PF[0]   | GPIO        |   0   |  0  |  0  |  0  |  0x0 |  0 |
// -------------------------------------------------------------

// To configure the GPIO pins of a particular port, follow these steps:
// 1. Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register. 
// In addition, the SCGCGPIO and DCGCGPIO registers can be programmed in the
// same manner to enable clocking in Sleep and Deep-Sleep modes.
// 2. Set the direction of the GPIO port pins by programming the DIR register. A write of a 1
// indicates output and a write of a 0 indicates input
// 3. Configure the AFSEL register to program each bit as a GPIO or alternate pin. If an alternate
// pin is chosen for a bit, then the PMCx field must be programmed in the PCTL register for
// the specific peripheral required. There are also two registers, ADCCTL and DMACTL,
// which can be used to program a GPIO pin as a ADC or μDMA trigger, respectively.
// 4. Set the drive strength for each of the pins through the DR2R, DR4R, and DR8R
// registers.
// 5. Program each pad in the port to have either pull-up, pull-down, or open drain functionality through
// the PUR, PDR, ODR register. Slew rate may also be programmed, if needed,
// through the SLR register.
// 6. To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the DEN register. To
// enable GPIO pins to their analog function (if available), set the AMSEL bit in the
// AMSEL register.
// 7. Program the IS, IBE, EV, and IM registers to configure the type, event,
// and mask of the interrupts for each port.
// Note:
// To prevent false interrupts, the following steps should be taken when re-configuring
// GPIO edge and interrupt sense registers:
// a. Mask the corresponding port by clearing the IME field in the IM register.
// b. Configure the IS field in the IS register and the IBE field in the IBE
// register.
// c. Clear the RIS register.
// d. Unmask the port by setting the IME field in the IM register.
// 8. Optionally, software can lock the configurations of the NMI and JTAG/SWD pins on the GPIO
// port pins, by setting the LOCK bits in the LOCK register.

// To prevent false interrupts, the following steps should be taken 
// when re-configuring GPIO edge and interrupt sense registers:
// a. Mask the corresponding port by clearing the IME field in the GPIOIM.
// b. Configure the IS field in the GPIOIS and the IBE field in the GPIOIBE
// c. Clear the GPIORIS register.
// d. Unmask the port by setting the IME field in the GPIOIM register.

// For a GPIO edge-detect interrupt, the RIS
// bit in the GPIORIS register is cleared by writing a ‘1’ to the 
// corresponding bit in the GPIO Interrupt Clear (GPIOICR) register

//
// Helper GPIO pins constants
//

constexpr u8 PA0 = 0x01;
constexpr u8 PA1 = 0x02;
constexpr u8 PA2 = 0x04;
constexpr u8 PA3 = 0x08;
constexpr u8 PA4 = 0x10;
constexpr u8 PA5 = 0x20;
constexpr u8 PA6 = 0x40;
constexpr u8 PA7 = 0x80;

constexpr u8 PB0 = 0x01;
constexpr u8 PB1 = 0x02;
constexpr u8 PB2 = 0x04;
constexpr u8 PB3 = 0x08;
constexpr u8 PB4 = 0x10;
constexpr u8 PB5 = 0x20;
constexpr u8 PB6 = 0x40;
constexpr u8 PB7 = 0x80;

constexpr u8 PC0 = 0x01;
constexpr u8 PC1 = 0x02;
constexpr u8 PC2 = 0x04;
constexpr u8 PC3 = 0x08;
constexpr u8 PC4 = 0x10;
constexpr u8 PC5 = 0x20;
constexpr u8 PC6 = 0x40;
constexpr u8 PC7 = 0x80;

constexpr u8 PD0 = 0x01;
constexpr u8 PD1 = 0x02;
constexpr u8 PD2 = 0x04;
constexpr u8 PD3 = 0x08;
constexpr u8 PD4 = 0x10;
constexpr u8 PD5 = 0x20;
constexpr u8 PD6 = 0x40;
constexpr u8 PD7 = 0x80;

constexpr u8 PE0 = 0x01;
constexpr u8 PE1 = 0x02;
constexpr u8 PE2 = 0x04;
constexpr u8 PE3 = 0x08;
constexpr u8 PE4 = 0x10;
constexpr u8 PE5 = 0x20;
constexpr u8 PE6 = 0x40;
constexpr u8 PE7 = 0x80;

constexpr u8 PF0 = 0x01;
constexpr u8 PF1 = 0x02;
constexpr u8 PF2 = 0x04;
constexpr u8 PF3 = 0x08;
constexpr u8 PF4 = 0x10;
constexpr u8 PF5 = 0x20;
constexpr u8 PF6 = 0x40;
constexpr u8 PF7 = 0x80;

// Structure of GPIO block as in the hardware
struct GPIO_Block
{
	//! Data
    RW  u32 DATA[0x100]; 

    //! Direction
    RW  u32 DIR;		 

    //! Interrupt Sense
    // Setting a bit in the IS register configures the corresponding pin 
    // to detect levels, while clearing a bit configures the corresponding pin
    // to detect edges. All bits are cleared by a reset.
    RW  u32 IS;          

	//! Interrupt Both Edges
	// When the corresponding bit in the IS register is set to detect edges, 
	// setting a bit in the IBE register configures the corresponding pin to 
	// detect both rising and falling edges, regardless of the corresponding 
	// bit in the IEV register. Clearing a bit configures the pin to be
	// controlled by the IEV register. All bits are cleared by a reset.
    RW  u32 IBE;         

	//! Interrupt Event
	// Setting a bit in the EV register configures the corresponding pin
	// to detect rising edges or high levels, depending on the corresponding 
	// bit in the IS register. Clearing a bit configures the pin to detect 
	// falling edges or low levels, depending on the corresponding bit value 
	// in the IS register. All bits are cleared by a reset.
    RW  u32 IEV;         

    //! Interrupt Mask
	// Setting a bit in the IM register allows interrupts that are generated 
	// by the corresponding pin to be sent to the interrupt controller on the
	// combined interrupt signal. Clearing a bit prevents an interrupt on the 
	// corresponding pin from being sent to the interrupt controller. 
	// All bits are cleared by a reset.
    RW  u32 IM;          

    //! Raw Interrupt Status
    RO  u32 RIS;         

    //! Masked Interrupt Status
    RO  u32 MIS;         

    //! Interrupt Clear
    // For edge-detect interrupts, writing a 1 to the IC bit in the ICR register
    // clears the corresponding bit in the RIS and MIS registers. If the 
    // interrupt is a level-detect, the IC bit in this register has no effect. 
    // In addition, writing a 0 to any of the bits in the ICR register has no effect.
    W1C u32 ICR;         

    //! Alternate Function Select
    RW  u32 AFSEL;       
    RO  u32 reserved[0x37];

    //! 2-mA Drive Select
    // Each GPIO signal in the port can be individually configured without 
    // affecting the other pads. When setting the DRV2 bit for a GPIO signal,
	// the corresponding DRV4 bit in the DR4R register and DRV8 bit in the DR8R
	// register are automatically cleared by hardware. 
	// By default, all GPIO pins have 2-mA drive.
    RW  u32 DR2R;       

    //! 4-mA Drive Select
    RW  u32 DR4R;       

    //! 8-mA Drive Select
    RW  u32 DR8R;       

    //! Open Drain Select
    RW  u32 ODR;         

    //! Pull-Up Select
    // When a bit is set, a weak pull-up resistor on the corresponding GPIO 
    // signal is enabled. Setting a bit in GPIOPUR automatically clears the 
    // corresponding bit in the PDR register. Write access to this register 
    // is protected with the CR register. Bits in CR that are cleared prevent 
    // writes to the equivalent bit in this register.
    RW  u32 PUR;         

    //! Pull-Down Select
    RW  u32 PDR;         

    //! Slew Rate Control Select
    RW  u32 SLR;         

    //! Digital Enable
    // By default, all GPIO signals except those listed below are configured 
    // out of reset to be undriven (tristate). Their digital function is 
    // disabled; they do not drive a logic value on the pin and they do not 
    // allow the pin voltage into the GPIO receiver. To use the pin as a digital
    // input or output (either GPIO or alternate function), the corresponding 
    // DEN bit must be set.
    RW  u32 DEN;         

    //! Lock
    RW  u32 LOCK;        

    //! Commit
    u32 CR;          

    //! Analog Mode Select
    RW  u32 AMSEL;       

    //! Port Control
    RW  u32 PCTL;        

    //! ADC Control 
    RW  u32 ADCCTL;      

    //! DMA Control
    RW  u32 DMACTL;      
};

// Concrete ports definitions
#define GPIOA ((volatile GPIO_Block*)(0x40058000))
#define GPIOB ((volatile GPIO_Block*)(0x40059000))
#define GPIOC ((volatile GPIO_Block*)(0x4005A000))
#define GPIOD ((volatile GPIO_Block*)(0x4005B000))
#define GPIOE ((volatile GPIO_Block*)(0x4005C000))
#define GPIOF ((volatile GPIO_Block*)(0x4005D000))

// Pin muxes
constexpr u32 GPIO_PMUX_PA0_U0RX = (0x1 << 4*0);
constexpr u32 GPIO_PMUX_PA1_U0TX = (0x1 << 4*1);
constexpr u32 GPIO_PMUX_PB2_I2C0SCL = (0x3 << 4*2);
constexpr u32 GPIO_PMUX_PB3_I2C0SDA = (0x3 << 4*3);

//
// Board pins configuration:
// 

constexpr u8 U0RX_PIN = PA0;
constexpr u8 U0TX_PIN = PA1;

constexpr u8 I2C0SCL_PIN = PB2;
constexpr u8 I2C0SDA_PIN = PB3;

constexpr u8 BTN_RIGHT_PIN = PF0;
constexpr u8 RED_LED_PIN   = PF1;
constexpr u8 BLUE_LED_PIN  = PF2;
constexpr u8 GREEN_LED_PIN = PF3;
constexpr u8 BTN_LEFT_PIN  = PF4;

constexpr u8 BTNS_PINS = (BTN_LEFT_PIN | BTN_RIGHT_PIN); 
constexpr u8 LEDS_PINS = (RED_LED_PIN | BLUE_LED_PIN | GREEN_LED_PIN);
constexpr u8 UART0_PINS = (U0RX_PIN | U0TX_PIN);
constexpr u8 I2C0_PINS = (I2C0SCL_PIN | I2C0SDA_PIN);

//
// Global variables
//

static QueueHandle_t wait_queue = nullptr;

//
// Public functions
//

//! Initializes GPIO peripheral and used pins to work
void gpio_init()
{
	//
	// Software initialization
	//

	const auto wait_queue_size = 1;
	CHECK(wait_queue = xQueueCreate(wait_queue_size, sizeof(u8)));

	//
	// Hardware initialization
	//

	// GPIOA->LOCK = GPIO_LOCK_KEY; Can be ommited since no pin is here locked
	// GPIOA->CR = 0; Can be ommited since no pin here is locked
	// GPIOA->LOCK = 0; Can be ommited since no pin here is locked
	GPIOA->DIR = 0;
	GPIOA->AFSEL = UART0_PINS;
	GPIOA->PCTL = GPIO_PMUX_PA0_U0RX | GPIO_PMUX_PA1_U0TX;
	GPIOA->DR2R = U0TX_PIN;
	GPIOA->DR4R = 0;
	GPIOA->DR8R = 0;
	GPIOA->SLR = 0;
	GPIOA->PUR = 0;
	GPIOA->PDR = 0;
	GPIOA->ODR = 0;
	GPIOA->DEN = UART0_PINS;
	GPIOA->AMSEL = 0;
	// GPIOF->IS = 0; Can be ommited since no pin here is for interrupts
	// GPIOF->IBE = 0; Can be ommited since no pin here is for interrupts
	// GPIOF->IEV = 0; Can be ommited since no pin here is for interrupts
	// GPIOF->ICR = 0; Can be ommited since no pin here is for interrupts
	GPIOA->IM = 0;

	// GPIOB->LOCK = GPIO_LOCK_KEY; Can be ommited since no pin here is locked
	// GPIOB->CR = 0; Can be ommited since no pin here is locked
	// GPIOB->LOCK = 0; Can be ommited since no pin here is locked
	GPIOB->DIR = 0;
	GPIOB->AFSEL = I2C0_PINS;
	GPIOB->PCTL = GPIO_PMUX_PB2_I2C0SCL | GPIO_PMUX_PB3_I2C0SDA;
	GPIOB->DR2R = I2C0_PINS;
	GPIOB->DR4R = 0;
	GPIOB->DR8R = 0;
	GPIOB->SLR = 0;
	GPIOB->PUR = I2C0_PINS;
	GPIOB->PDR = 0;
	GPIOB->ODR = I2C0SDA_PIN;
	GPIOB->DEN = I2C0_PINS;
	GPIOB->AMSEL = 0;
	// GPIOF->IS = 0; Can be ommited since no pin here is for interrupts
	// GPIOF->IBE = 0; Can be ommited since no pin here is for interrupts
	// GPIOF->IEV = 0; Can be ommited since no pin here is for interrupts
	// GPIOF->ICR = 0; Can be ommited since no pin here is for interrupts
	GPIOB->IM = 0;

	GPIOF->LOCK = GPIO_LOCK_KEY;
	GPIOF->CR = BTN_RIGHT_PIN;
	GPIOF->LOCK = 0;
	GPIOF->DIR = LEDS_PINS;
	GPIOF->AFSEL = 0;
	// GPIOF->PCTL = 0; Can be ommited since no pin here is alternate
	GPIOF->DR2R = LEDS_PINS;
	GPIOF->DR4R = 0;
	GPIOF->DR8R = 0;
	GPIOF->SLR = 0;
	GPIOF->PUR = BTNS_PINS;
	GPIOF->PDR = 0;
	GPIOF->ODR = 0;
	GPIOF->DEN = LEDS_PINS | BTNS_PINS;
	GPIOF->AMSEL = 0;
	GPIOF->IS = 0;
	GPIOF->IBE = 0;
	GPIOF->IEV = 0;
	GPIOF->ICR = BTNS_PINS;
	GPIOF->IM = 0;
}

u8 gpio_wait(u8 pins)
{
	// Just to be sure, previous wait must be done completely
	#ifndef NDEBUG
		NVIC_CRITICAL_SECTION_ENTER(INT_GPIOF);
		{
			assert(GPIOF->IM == 0x00);
		}
		NVIC_CRITICAL_SECTION_LEAVE(INT_GPIOF);
	#endif

	// Enable interrupts for specified pins
	GPIOF->IM = pins;

	// Wait for interrupt to detect pins touch and return them
	u8 touched_pins;
	CHECK(xQueueReceive(wait_queue, &touched_pins, portMAX_DELAY));
	return touched_pins;
}

void GPIOF_handler()
{
	// Interrupt should come from touched pins
	const auto pins = GPIOF->MIS;
	assert(pins != 0x00);

	// Disable further interrupts for any pins and clear spurious ones
	GPIOF->IM = 0x00;
	GPIOF->ICR = 0xFF;

	// Send the touched pins to the queue.
	// If writing to the queue unblocks a task, and the unblocked task has a
	// priority above the currently running task (the task that this interrupt
	// interrupted), then highpriotask_woken will be set to pdTRUE inside the
	// xQueueSendFromISR() function. highpriotask_woken is then passed to
	// portYIELD_FROM_ISR() at the end of this interrupt handler to request a
	// context switch so the interrupt returns directly to the (higher priority)
	// unblocked task.
	auto highpriotask_woken = pdFALSE;
	xQueueSendFromISR(wait_queue, &pins, &highpriotask_woken);

	/* portYIELD_FROM_ISR() will request a context switch if executing this
	interrupt handler caused a task to leave the blocked state, and the task
	that left the blocked state has a higher priority than the currently running
	task (the task this interrupt interrupted).  See the comment above the calls
	to xSemaphoreGiveFromISR() and xQueueSendFromISR() within this function. */
	portYIELD_FROM_ISR(highpriotask_woken);
}
