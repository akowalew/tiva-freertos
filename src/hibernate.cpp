// The Hibernation clock source is enabled by setting the CLK32EN bit of the HIBCTL register. The
// CLK32EN bit must be set before accessing any other Hibernation module register. If a crystal is
// used for the clock source, the software must leave a delay of t HIBOSC_START after writing to the
// CLK32EN bit and before any other accesses to the Hibernation module registers. The delay allows
// the crystal to power up and stabilize. If an external oscillator is used for the clock source, no delay
// is needed. When using an external clock source, the OSCBYP bit in the HIBCTL register should be
// set. When using a crystal clock source, the GNDX pin should be connected to digital ground along
// with the crystal load capacitors, as shown in Figure 7-2 on page 496. When using an external clock
// source, the GNDX pin should be connected to digital ground.

// Real-Time Clock
// The RTC module is designed to keep wall time. The RTC can operate in seconds counter mode. A
// 32.768 kHz clock source along with a 15-bit predivider reduces the clock to 1 Hz. The 1 Hz clock
// is used to increment the 32-bit counter and keep track of seconds. A match register can be configured
// to interrupt or wake the system from hibernate. In addition, a software trim register is implemented
// to allow the user to compensate for oscillator inaccuracies using software.
// 7.3.5.1
// RTC Counter - Seconds/Subseconds Mode
// The clock signal to the RTC is provided by either of the 32.768-kHz clock sources available to the
// Hibernation module. The Hibernation RTC Counter (HIBRTCC) register displays the seconds
// value. The Hibernation RTC Sub Seconds register (HIBRTCSS) is provided for additional time
// resolution of an application requiring less than one-second divisions.
// The RTC is enabled by setting the RTCEN bit of the HIBCTL register. The RTC counter and
// sub-seconds counters begin counting immediately once RTCEN is set. Both counters count up. The
// RTC continues counting as long as the RTC is enabled and a valid V BAT is present, regardless of
// whether V DD is present or if the device is in hibernation.
// The HIBRTCC register is set by writing the Hibernation RTC Load (HIBRTCLD) register. A write
// to the HIBRTCLD register clears the 15-bit sub-seconds counter field, RTCSSC, in the HIBRTCSS
// register. To ensure a valid read of the RTC value, the HIBRTCC register should be read first, followed
// by a read of the RTCSSC field in the HIBRTCSS register and then a re-read of the HIBRTCC register.
// If the two values for the HIBRTCC are equal, the read is valid. By following this procedure, errors
// in the application caused by the HIBRTCC register rolling over by a count of 1 during a read of the
// RTCSSC field are prevented. The RTC can be configured to generate an alarm by setting the RTCAL0
// bit in the HIBIM register. When an RTC match occurs, an interrupt is generated and displayed in
// the HIBRIS register. Refer to “RTC Match - Seconds/Subseconds Mode” on page 499 for more
// information.

// The Hibernation module has several different configurations. The following sections show the
// recommended programming sequence for various scenarios. Because the Hibernation module runs
// at a low frequency and is asynchronous to the rest of the microcontroller, which is run off the system
// clock, software must allow a delay of t HIB_REG_ACCESS after writes to registers (see “Register Access
// Timing” on page 495). The WC interrupt in the HIBMIS register can be used to notify the application
// when the Hibernation modules registers can be accessed.

// The Hibernation module comes out of reset with the system clock enabled to the module, but if the
// system clock to the module has been disabled, then it must be re-enabled, even if the RTC feature
// is not used. See page 343.

struct HIB_Block
{
	RO u32 RTCC; // RTC Counter;
	RW u32 RTCM0; // RTC Match;
	RO u32 _reserved1[0x1];
	RW u32 RTCLD; // RTC Load;
	RW u32 CTL; // Control;
	RW u32 IM; // Interrupt Mask;
	RO u32 RIS; // Raw Interrupt Status;
	RO u32 MIS; // Masked Interrupt Status;
	RW1C u32 IC; // Interrupt Clear;
	RW u32 RTCT; // RTC Trim;
	RW u32 RTCSS; // RTC Sub Seconds;
	RO u32 _reserved2[0x1];
	RW u32 DATA; // Data
};

static_assert(offsetof(HIB_Block, RTCC) == 0x000);
static_assert(offsetof(HIB_Block, RTCM0) == 0x004);
static_assert(offsetof(HIB_Block, RTCLD) == 0x00C);
static_assert(offsetof(HIB_Block, CTL) == 0x010);
static_assert(offsetof(HIB_Block, IM) == 0x014);
static_assert(offsetof(HIB_Block, RIS) == 0x018);
static_assert(offsetof(HIB_Block, MIS) == 0x01C);
static_assert(offsetof(HIB_Block, IC) == 0x020);
static_assert(offsetof(HIB_Block, RTCT) == 0x024);
static_assert(offsetof(HIB_Block, RTCSS) == 0x028);
static_assert(offsetof(HIB_Block, DATA) == 0x030);

#define HIB ((volatile HIB_Block*)(0x400FC000))

void hib_init()
{
	// Hibernation registers persist across resets
	// so we have to handle situation, that it is currently working

	// Check if hibernation module is arleady enabled
	if(!(HIB->CTL & HIB_CTL_CLK32EN))
	{
		// Enable module to use 32.768kHz crystal
		HIB->CTL = HIB_CTL_CLK32EN;
	}

	// Busy wait for write complete / capable
	while(!(HIB->CTL & HIB_CTL_WRC));

	// Enable RTC timer
	HIB->CTL = (HIB_CTL_CLK32EN | HIB_CTL_RTCEN);
}

u32 hib_rtc_seconds()
{
	// To ensure a valid read of the RTC value, the HIBRTCC register should be read first, followed
	// by a read of the RTCSSC field in the HIBRTCSS register and then a re-read of the HIBRTCC register.
	// If the two values for the HIBRTCC are equal, the read is valid. By following this procedure, errors
	// in the application caused by the HIBRTCC register rolling over by a count of 1 during a read of the
	// RTCSSC field are prevented.

	// Since we are not using sub-seconds now, we can just return seconds count
	return HIB->RTCC;
}