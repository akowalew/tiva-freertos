///////////////////////////////////////////////////////////////////////////////
// NVIC management
///////////////////////////////////////////////////////////////////////////////

constexpr u32 VEC_GPIOA =       16;  constexpr u32 INT_GPIOA =       VEC_GPIOA-16;
constexpr u32 VEC_GPIOB =       17;  constexpr u32 INT_GPIOB =       VEC_GPIOB-16;
constexpr u32 VEC_GPIOC =       18;  constexpr u32 INT_GPIOC =       VEC_GPIOC-16;
constexpr u32 VEC_GPIOD =       19;  constexpr u32 INT_GPIOD =       VEC_GPIOD-16;
constexpr u32 VEC_GPIOE =       20;  constexpr u32 INT_GPIOE =       VEC_GPIOE-16;
constexpr u32 VEC_UART0 =       21;  constexpr u32 INT_UART0 =       VEC_UART0-16;
constexpr u32 VEC_PWM0_FAULT =  25;  constexpr u32 INT_PWM0_FAULT =  VEC_PWM0_FAULT-16;
constexpr u32 VEC_PWM0_0 =      26;  constexpr u32 INT_PWM0_0 =      VEC_PWM0_0-16;
constexpr u32 VEC_PWM0_1 =      27;  constexpr u32 INT_PWM0_1 =      VEC_PWM0_1-16;
constexpr u32 VEC_PWM0_2 =      28;  constexpr u32 INT_PWM0_2 =      VEC_PWM0_2-16;
constexpr u32 VEC_QEI0 =        29;  constexpr u32 INT_QEI0 =        VEC_QEI0-16;
constexpr u32 VEC_ADC0SS0 =     30;  constexpr u32 INT_ADC0SS0 =     VEC_ADC0SS0-16;
constexpr u32 VEC_ADC0SS1 =     31;  constexpr u32 INT_ADC0SS1 =     VEC_ADC0SS1-16;
constexpr u32 VEC_ADC0SS2 =     32;  constexpr u32 INT_ADC0SS2 =     VEC_ADC0SS2-16;
constexpr u32 VEC_ADC0SS3 =     33;  constexpr u32 INT_ADC0SS3 =     VEC_ADC0SS3-16;
constexpr u32 VEC_WATCHDOG =    34;  constexpr u32 INT_WATCHDOG =    VEC_WATCHDOG-16;
constexpr u32 VEC_TIMER0A =     35;  constexpr u32 INT_TIMER0A =     VEC_TIMER0A-16;
constexpr u32 VEC_TIMER0B =     36;  constexpr u32 INT_TIMER0B =     VEC_TIMER0B-16;
constexpr u32 VEC_TIMER1A =     37;  constexpr u32 INT_TIMER1A =     VEC_TIMER1A-16;
constexpr u32 VEC_TIMER1B =     38;  constexpr u32 INT_TIMER1B =     VEC_TIMER1B-16;
constexpr u32 VEC_TIMER2A =     39;  constexpr u32 INT_TIMER2A =     VEC_TIMER2A-16;
constexpr u32 VEC_TIMER2B =     40;  constexpr u32 INT_TIMER2B =     VEC_TIMER2B-16;
constexpr u32 VEC_COMP0 =       41;  constexpr u32 INT_COMP0 =       VEC_COMP0-16;
constexpr u32 VEC_COMP1 =       42;  constexpr u32 INT_COMP1 =       VEC_COMP1-16;
constexpr u32 VEC_COMP2 =       43;  constexpr u32 INT_COMP2 =       VEC_COMP2-16;
constexpr u32 VEC_SYSCTL =      44;  constexpr u32 INT_SYSCTL =      VEC_SYSCTL-16;
constexpr u32 VEC_FLASH =       45;  constexpr u32 INT_FLASH =       VEC_FLASH-16;
constexpr u32 VEC_GPIOF =       46;  constexpr u32 INT_GPIOF =       VEC_GPIOF-16;
constexpr u32 VEC_GPIOG =       47;  constexpr u32 INT_GPIOG =       VEC_GPIOG-16;
constexpr u32 VEC_GPIOH =       48;  constexpr u32 INT_GPIOH =       VEC_GPIOH-16;
constexpr u32 VEC_UART2 =       49;  constexpr u32 INT_UART2 =       VEC_UART2-16;
constexpr u32 VEC_SSI1 =        50;  constexpr u32 INT_SSI1 =        VEC_SSI1-16;
constexpr u32 VEC_TIMER3A =     51;  constexpr u32 INT_TIMER3A =     VEC_TIMER3A-16;
constexpr u32 VEC_TIMER3B =     52;  constexpr u32 INT_TIMER3B =     VEC_TIMER3B-16;
constexpr u32 VEC_I2C1 =        53;  constexpr u32 INT_I2C1 =        VEC_I2C1-16;
constexpr u32 VEC_QEI1 =        54;  constexpr u32 INT_QEI1 =        VEC_QEI1-16;
constexpr u32 VEC_CAN0 =        55;  constexpr u32 INT_CAN0 =        VEC_CAN0-16;
constexpr u32 VEC_CAN1 =        56;  constexpr u32 INT_CAN1 =        VEC_CAN1-16;
constexpr u32 VEC_HIBERNATE =   59;  constexpr u32 INT_HIBERNATE =   VEC_HIBERNATE-16;
constexpr u32 VEC_USB0 =        60;  constexpr u32 INT_USB0 =        VEC_USB0-16;
constexpr u32 VEC_PWM0_3 =      61;  constexpr u32 INT_PWM0_3 =      VEC_PWM0_3-16;
constexpr u32 VEC_UDMA =        62;  constexpr u32 INT_UDMA =        VEC_UDMA-16;
constexpr u32 VEC_UDMAERR =     63;  constexpr u32 INT_UDMAERR =     VEC_UDMAERR-16;
constexpr u32 VEC_ADC1SS0 =     64;  constexpr u32 INT_ADC1SS0 =     VEC_ADC1SS0-16;
constexpr u32 VEC_ADC1SS1 =     65;  constexpr u32 INT_ADC1SS1 =     VEC_ADC1SS1-16;
constexpr u32 VEC_ADC1SS2 =     66;  constexpr u32 INT_ADC1SS2 =     VEC_ADC1SS2-16;
constexpr u32 VEC_ADC1SS3 =     67;  constexpr u32 INT_ADC1SS3 =     VEC_ADC1SS3-16;
constexpr u32 VEC_GPIOJ =       70;  constexpr u32 INT_GPIOJ =       VEC_GPIOJ-16;
constexpr u32 VEC_GPIOK =       71;  constexpr u32 INT_GPIOK =       VEC_GPIOK-16;
constexpr u32 VEC_GPIOL =       72;  constexpr u32 INT_GPIOL =       VEC_GPIOL-16;
constexpr u32 VEC_SSI2 =        73;  constexpr u32 INT_SSI2 =        VEC_SSI2-16;
constexpr u32 VEC_SSI3 =        74;  constexpr u32 INT_SSI3 =        VEC_SSI3-16;
constexpr u32 VEC_UART3 =       75;  constexpr u32 INT_UART3 =       VEC_UART3-16;
constexpr u32 VEC_UART4 =       76;  constexpr u32 INT_UART4 =       VEC_UART4-16;
constexpr u32 VEC_UART5 =       77;  constexpr u32 INT_UART5 =       VEC_UART5-16;
constexpr u32 VEC_UART6 =       78;  constexpr u32 INT_UART6 =       VEC_UART6-16;
constexpr u32 VEC_UART7 =       79;  constexpr u32 INT_UART7 =       VEC_UART7-16;
constexpr u32 VEC_I2C2 =        84;  constexpr u32 INT_I2C2 =        VEC_I2C2-16;
constexpr u32 VEC_I2C3 =        85;  constexpr u32 INT_I2C3 =        VEC_I2C3-16;
constexpr u32 VEC_TIMER4A =     86;  constexpr u32 INT_TIMER4A =     VEC_TIMER4A-16;
constexpr u32 VEC_TIMER4B =     87;  constexpr u32 INT_TIMER4B =     VEC_TIMER4B-16;
constexpr u32 VEC_TIMER5A =     108; constexpr u32 INT_TIMER5A =     VEC_TIMER5A-16;
constexpr u32 VEC_TIMER5B =     109; constexpr u32 INT_TIMER5B =     VEC_TIMER5B-16;
constexpr u32 VEC_WTIMER0A =    110; constexpr u32 INT_WTIMER0A =    VEC_WTIMER0A-16;
constexpr u32 VEC_WTIMER0B =    111; constexpr u32 INT_WTIMER0B =    VEC_WTIMER0B-16;
constexpr u32 VEC_WTIMER1A =    112; constexpr u32 INT_WTIMER1A =    VEC_WTIMER1A-16;
constexpr u32 VEC_WTIMER1B =    113; constexpr u32 INT_WTIMER1B =    VEC_WTIMER1B-16;
constexpr u32 VEC_WTIMER2A =    114; constexpr u32 INT_WTIMER2A =    VEC_WTIMER2A-16;
constexpr u32 VEC_WTIMER2B =    115; constexpr u32 INT_WTIMER2B =    VEC_WTIMER2B-16;
constexpr u32 VEC_WTIMER3A =    116; constexpr u32 INT_WTIMER3A =    VEC_WTIMER3A-16;
constexpr u32 VEC_WTIMER3B =    117; constexpr u32 INT_WTIMER3B =    VEC_WTIMER3B-16;
constexpr u32 VEC_WTIMER4A =    118; constexpr u32 INT_WTIMER4A =    VEC_WTIMER4A-16;
constexpr u32 VEC_WTIMER4B =    119; constexpr u32 INT_WTIMER4B =    VEC_WTIMER4B-16;
constexpr u32 VEC_WTIMER5A =    120; constexpr u32 INT_WTIMER5A =    VEC_WTIMER5A-16;
constexpr u32 VEC_WTIMER5B =    121; constexpr u32 INT_WTIMER5B =    VEC_WTIMER5B-16;
constexpr u32 VEC_SYSEXC =      122; constexpr u32 INT_SYSEXC =      VEC_SYSEXC-16;
constexpr u32 VEC_I2C4 =        125; constexpr u32 INT_I2C4 =        VEC_I2C4-16;
constexpr u32 VEC_I2C5 =        126; constexpr u32 INT_I2C5 =        VEC_I2C5-16;
constexpr u32 VEC_GPIOM =       127; constexpr u32 INT_GPIOM =       VEC_GPIOM-16;
constexpr u32 VEC_GPION =       128; constexpr u32 INT_GPION =       VEC_GPION-16;
constexpr u32 VEC_GPIOP0 =      132; constexpr u32 INT_GPIOP0 =      VEC_GPIOP0-16;
constexpr u32 VEC_GPIOP1 =      133; constexpr u32 INT_GPIOP1 =      VEC_GPIOP1-16;
constexpr u32 VEC_GPIOP2 =      134; constexpr u32 INT_GPIOP2 =      VEC_GPIOP2-16;
constexpr u32 VEC_GPIOP3 =      135; constexpr u32 INT_GPIOP3 =      VEC_GPIOP3-16;
constexpr u32 VEC_GPIOP4 =      136; constexpr u32 INT_GPIOP4 =      VEC_GPIOP4-16;
constexpr u32 VEC_GPIOP5 =      137; constexpr u32 INT_GPIOP5 =      VEC_GPIOP5-16;
constexpr u32 VEC_GPIOP6 =      138; constexpr u32 INT_GPIOP6 =      VEC_GPIOP6-16;
constexpr u32 VEC_GPIOP7 =      139; constexpr u32 INT_GPIOP7 =      VEC_GPIOP7-16;
constexpr u32 VEC_GPIOQ0 =      140; constexpr u32 INT_GPIOQ0 =      VEC_GPIOQ0-16;
constexpr u32 VEC_GPIOQ1 =      141; constexpr u32 INT_GPIOQ1 =      VEC_GPIOQ1-16;
constexpr u32 VEC_GPIOQ2 =      142; constexpr u32 INT_GPIOQ2 =      VEC_GPIOQ2-16;
constexpr u32 VEC_GPIOQ3 =      143; constexpr u32 INT_GPIOQ3 =      VEC_GPIOQ3-16;
constexpr u32 VEC_GPIOQ4 =      144; constexpr u32 INT_GPIOQ4 =      VEC_GPIOQ4-16;
constexpr u32 VEC_GPIOQ5 =      145; constexpr u32 INT_GPIOQ5 =      VEC_GPIOQ5-16;
constexpr u32 VEC_GPIOQ6 =      146; constexpr u32 INT_GPIOQ6 =      VEC_GPIOQ6-16;
constexpr u32 VEC_GPIOQ7 =      147; constexpr u32 INT_GPIOQ7 =      VEC_GPIOQ7-16;
constexpr u32 VEC_PWM1_0 =      150; constexpr u32 INT_PWM1_0 =      VEC_PWM1_0-16;
constexpr u32 VEC_PWM1_1 =      151; constexpr u32 INT_PWM1_1 =      VEC_PWM1_1-16;
constexpr u32 VEC_PWM1_2 =      152; constexpr u32 INT_PWM1_2 =      VEC_PWM1_2-16;
constexpr u32 VEC_PWM1_3 =      153; constexpr u32 INT_PWM1_3 =      VEC_PWM1_3-16;
constexpr u32 VEC_PWM1_FAULT =  154; constexpr u32 INT_PWM1_FAULT =  VEC_PWM1_FAULT-16;
constexpr u32 NUM_VECS =        155; constexpr u32 NUM_INTS =        NUM_VECS-16;

struct NVIC_Block
{
	RO u32 _reserved1[0x40]; 
	RW u32 EN[5];
	RO u32 _reserved2[0x1B];
	RW u32 DIS[5];
	RO u32 _reserved3[0x1B];
	RW u32 PEND[5];
	RO u32 _reserved4[0x1B];
	RW u32 UNPEND[5];
	RO u32 _reserved5[0x1B];
	RO u32 ACTIVE[5];
	RO u32 _reserved6[0x3B];
	RW u32 PRI[35];
	RO u32 _reserved7[0x29D];
	WO u32 SWTRIG;
}; 

#define NVIC ((volatile NVIC_Block*)(0xE000E000))

static_assert(offsetof(NVIC_Block, EN) == 0x100);
static_assert(offsetof(NVIC_Block, DIS) == 0x180);
static_assert(offsetof(NVIC_Block, PEND) == 0x200);
static_assert(offsetof(NVIC_Block, UNPEND) == 0x280);
static_assert(offsetof(NVIC_Block, ACTIVE) == 0x300);
static_assert(offsetof(NVIC_Block, PRI) == 0x400);
static_assert(offsetof(NVIC_Block, SWTRIG) == 0xF00);

void nvic_init()
{
	//
	// Enable particular interrupts and set their priorities
	//

	// NVIC->EN[INT_UART0 / 32] = (1 << (INT_UART0 % 32));
	NVIC->PRI[INT_UART0 / 4] = (0x5 << ((INT_UART0 % 4)*8 + 5));
	NVIC->PRI[INT_GPIOF / 4] = (0x5 << ((INT_GPIOF % 4)*8 + 5));
}

void nvic_enable_int(u32 num)
{
	// EN* registers are write-1 sensitive. It means that we can
	// without care set other bits to zero, because it does nothing with
	// 0-written bits
	NVIC->EN[num / 32] = (1 << (num % 32));
}

void nvic_disable_int(u32 num)
{
	// DIS* registers are write-1 sensitive. It means that we can
	// without care set other bits to zero, because it does nothing with
	// 0-written bits
	NVIC->DIS[num / 32] = (1 << (num % 32));
}

#define NVIC_CRITICAL_SECTION_ENTER(num) nvic_disable_int((num))

#define NVIC_CRITICAL_SECTION_LEAVE(num) nvic_enable_int((num))