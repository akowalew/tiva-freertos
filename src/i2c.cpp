///////////////////////////////////////////////////////////////////////////////
// I2C management
///////////////////////////////////////////////////////////////////////////////

struct I2C_Block
{
	//
	// I2C Master
	//

	RW u32 MSA; // Master Slave Address
	RW u32 MCS; // Master Control/Status
	RW u32 MDR; // Master Data
	RW u32 MTPR; // Master Timer Period
	RW u32 MIMR; // Master Interrupt Mask
	RO u32 MRIS; // Master Raw Interrupt Status
	RO u32 MMIS; // Master Masked Interrupt Status
	WO u32 MICR; // Master Interrupt Clear
	RW u32 MCR; // Master Configuration
	RW u32 MCLKOCNT; // Master Clock Low Timeout Count
	RO u32 _reserved1[0x1];
	RO u32 MBMON; // Master Bus Monitor
	RO u32 _reserved2[0x2];
	RW u32 MCR2; // Master Configuration 2

	//
	// I2C Slave
	//

	RO u32 _reserved3[0x1F1];
	RW u32 SOAR; // Slave Own Address
	RO u32 SCSR; // Slave Control/Status
	RW u32 SDR; // Slave Data
	RW u32 SIMR; // Slave Interrupt Mask
	RO u32 SRIS; // Slave Raw Interrupt Status
	RO u32 SMIS; // Slave Masked Interrupt Status
	WO u32 SICR; // Slave Interrupt Clear
	RW u32 SOAR2; // Slave Own Address 2
	RW u32 SACKCTL; // Slave ACK Control

	//
	// I2C Status and Control
	//

	RO u32 _reserved4[0x1E7];
	RO u32 PP; // Peripheral Properties
	RO u32 PC; // Peripheral Configuration
};

static_assert(offsetof(I2C_Block, MSA) == 0x000);
static_assert(offsetof(I2C_Block, MCS) == 0x004);
static_assert(offsetof(I2C_Block, MDR) == 0x008);
static_assert(offsetof(I2C_Block, MTPR) == 0x00C);
static_assert(offsetof(I2C_Block, MIMR) == 0x010);
static_assert(offsetof(I2C_Block, MRIS) == 0x014);
static_assert(offsetof(I2C_Block, MMIS) == 0x018);
static_assert(offsetof(I2C_Block, MICR) == 0x01C);
static_assert(offsetof(I2C_Block, MCR) == 0x020);
static_assert(offsetof(I2C_Block, MCLKOCNT) == 0x024);
static_assert(offsetof(I2C_Block, MBMON) == 0x02C);
static_assert(offsetof(I2C_Block, MCR2) == 0x038);
static_assert(offsetof(I2C_Block, SOAR) == 0x800);
static_assert(offsetof(I2C_Block, SCSR) == 0x804);
static_assert(offsetof(I2C_Block, SDR) == 0x808);
static_assert(offsetof(I2C_Block, SIMR) == 0x80C);
static_assert(offsetof(I2C_Block, SRIS) == 0x810);
static_assert(offsetof(I2C_Block, SMIS) == 0x814);
static_assert(offsetof(I2C_Block, SICR) == 0x818);
static_assert(offsetof(I2C_Block, SOAR2) == 0x81C);
static_assert(offsetof(I2C_Block, SACKCTL) == 0x820);
static_assert(offsetof(I2C_Block, PP) == 0xFC0);
static_assert(offsetof(I2C_Block, PC) == 0xFC4);

#define I2C0 ((volatile I2C_Block*)(0x40020000))
#define I2C1 ((volatile I2C_Block*)(0x40021000))
#define I2C2 ((volatile I2C_Block*)(0x40022000))
#define I2C3 ((volatile I2C_Block*)(0x40023000))

#define I2C_WRITE_TO(x) ((x) << 1)
#define I2C_READ_FROM(x) (((x) << 1) | I2C_MSA_RS)

//
// Global variables
// 

static volatile TaskHandle_t i2c_tx_task;
static const volatile u8* i2c_tx_data;
static const volatile u8* i2c_tx_dataend;

//
// Public functions
//

void i2c_init()
{
	//
	// Software initialization
	//

	i2c_tx_task = nullptr;
	i2c_tx_data = nullptr;
	i2c_tx_dataend = nullptr;

	//
	// Hardware initialization
	//

	// Initialize I2C Master
	I2C0->MCR = I2C_MCR_MFE;

	// Set the desired SCL clock speed of 100 Kbps
	// The value written to the I2CMTPR register represents the number 
	// of system clock periods in one SCL clock period:
	constexpr u32 SCL_LP = 6; // SCL Low Period - fixed at 6
	constexpr u32 SCL_HP = 4; // SCL High Period - fixed at 4
	constexpr u32 SCL_CLK = 100000; // Clock of I2C
	constexpr u32 TPR = ((configCPU_CLOCK_HZ / (2 * (SCL_LP+SCL_HP) * SCL_CLK)) - 1);
	static_assert(TPR > 0);
	I2C0->MTPR = TPR;

	// Clear any interrupt causes
	I2C0->MICR = 0xFFFFFFFF;

	// Enable master interrupts
	I2C0->MIMR = I2C_MIMR_IM;
}

bool i2c_write_one(u8 data, u8 addr)
{
	// Just to be sure, previous write must be done
	#ifndef NDEBUG
		NVIC_CRITICAL_SECTION_ENTER(INT_I2C0);
		{
			assert(i2c_tx_task == nullptr);
			assert(i2c_tx_data == i2c_tx_dataend);
		}
		NVIC_CRITICAL_SECTION_LEAVE(INT_I2C0);
	#endif

	// Master must be in idle mode
	assert(I2C0->MCS & I2C_MCS_IDLE);

	// Remember, which task will be waiting
	i2c_tx_task = xTaskGetCurrentTaskHandle();

	// Set slave address as transmit target
	I2C0->MSA = I2C_WRITE_TO(addr);

	// Set data to transmit and leave data pointer equal to each other
	//  so when interrupt is hit, it will immediately quit and notify us
	I2C0->MDR = data;
	assert(i2c_tx_data == i2c_tx_dataend);

	// Run I2C master, generate START and after send - generate STOP
	I2C0->MCS = (I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP);

	// Wait for notification from interrupt
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	// If ERROR flag is set, write failed
	return (I2C0->MCS & I2C_MCS_ERROR) ? false : true;
}

//! Multibyte write function with support for control byte
// Control bytes are used e.g. in SSD1306, where we have to specify, if coming
// bytes will contain commands or data for RAM. This function sends control byte
// first, then sleeps and waits for write of the specified buffer
bool i2c_write(const u8* data, u8 size, u8 ctrl, u8 addr)
{
	assert(data != nullptr);
	assert(size > 0);

	// Just to be sure, previous write must be done
	#ifndef NDEBUG
		NVIC_CRITICAL_SECTION_ENTER(INT_I2C0);
		{
			assert(i2c_tx_task == nullptr);
			assert(i2c_tx_data == i2c_tx_dataend);
		}
		NVIC_CRITICAL_SECTION_LEAVE(INT_I2C0);
	#endif

	// Master must be in idle mode
	assert(I2C0->MCS & I2C_MCS_IDLE);

	// Remember, which task will be waiting
	i2c_tx_task = xTaskGetCurrentTaskHandle();

	// Set slave address as transmit target
	I2C0->MSA = I2C_WRITE_TO(addr);

	// Set data to transmit
	I2C0->MDR = ctrl;
	i2c_tx_data = (data);
	i2c_tx_dataend = (data + size);

	// Begin transmission
	I2C0->MCS = (I2C_MCS_RUN | I2C_MCS_START);

	// Wait for notification from interrupt
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	// If ERROR flag is set, write failed
	return (I2C0->MCS & I2C_MCS_ERROR) ? false : true;
}

//
// Private functions
//

static void I2C0_handler()
{
	// Retrieve cause of the interrupt
	const auto masked_status = I2C0->MMIS;

	// NOTE: Sometimes there occurs interrupt with masked status = 0
	//  At the moment I have no idea, why that happens, and what to do with it

	// Clear interrupt cause
	I2C0->MICR = masked_status;

	// We would like to know, if higher priority task must be woken
	auto highpriotask_woken = pdFALSE;

	if(masked_status == I2C_MMIS_MIS) 
	{
		// Latch write context, because they are volatile variables
		auto tx_data = i2c_tx_data;
		const auto tx_dataend = i2c_tx_dataend;
		assert(tx_data <= tx_dataend);

		// Check if transmission is finished or error occured
		// Note the data pointer may be both nullptrs, so then we are handling
		// write_one case of transmission. In both cases above assertion works 
		const auto write_finished = (tx_data == tx_dataend);
		const auto error_occured = (I2C0->MCS & I2C_MCS_ERROR);
		if(write_finished || error_occured) 
		{
			// Notify sending task that write has been finished
			//  (either with success or error - it will check)
			const auto tx_task = i2c_tx_task;
			assert(tx_task != nullptr);
			vTaskNotifyGiveFromISR(tx_task, &highpriotask_woken);
			#ifndef NDEBUG
				i2c_tx_task = nullptr;
			#endif
		}
		else
		{
			// Put next data byte to the transmitter
			assert(tx_data != nullptr);
			I2C0->MDR = *(tx_data++);
			if(tx_data == tx_dataend) {
				// We are writing last data byte, after it stop transmission
				I2C0->MCS = (I2C_MCS_RUN | I2C_MCS_STOP);
			} else {
				// We are writing some middle byte, run transmission as usual
				I2C0->MCS = (I2C_MCS_RUN);
			}

			// Remember that we advanced to the next data byte
			i2c_tx_data = tx_data;
		}
	}

	/* portYIELD_FROM_ISR() will request a context switch if executing this
	interrupt handler caused a task to leave the blocked state, and the task
	that left the blocked state has a higher priority than the currently running
	task (the task this interrupt interrupted).  See the comment above the calls
	to xSemaphoreGiveFromISR() and xQueueSendFromISR() within this function. */
	portYIELD_FROM_ISR(highpriotask_woken);
}