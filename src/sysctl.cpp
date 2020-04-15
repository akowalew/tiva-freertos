///////////////////////////////////////////////////////////////////////////////
// System hardware management
///////////////////////////////////////////////////////////////////////////////

struct SYSCTL_Block
{
    RO u32 DID0; // Device Identification 0
    RO u32 DID1; // Device Identification 1
    RO u32 _reserved1[0xA];
    RW u32 PBORCTL; // Brown-Out Reset Control
    RO u32 _reserved2[0x7];
    RO u32 RIS; // Raw Interrupt Status
    RW u32 IMC; // Interrupt Mask Control
    RW1C u32 MISC; // Masked Interrupt Status and Clear
    RW u32 RESC; // Reset Cause
    RW u32 RCC; // Run-Mode Clock Configuration
    RO u32 _reserved3[0x2];
    RW u32 GPIOHBCTL; // GPIO High-Performance Bus Control
    RW u32 RCC2; // Run-Mode Clock Configuration 2
    RO u32 _reserved4[0x2];
    RW u32 MOSCCTL; // Main Oscillator Control
    RO u32 _reserved5[0x31];
    RW u32 DSLPCLKCFG; // Deep Sleep Clock Configuration
    RO u32 _reserved6[0x1];
    RO u32 SYSPROP; // System Properties
    RW u32 PIOSCCAL; // Precision Internal Oscillator Calibration
    RO u32 PIOSCSTAT; // Precision Internal Oscillator Statistics
    RO u32 _reserved7[0x2];
    RO u32 PLLFREQ0; // PLL Frequency 0
    RO u32 PLLFREQ1; // PLL Frequency 1
    RO u32 PLLSTAT; // PLL Status

    // TODO: add missing registers here

    u32 _reserved8[0x127];
    RW u32 RCGCGPIO;
    RW u32 RCGCDMA;
    u32 _reserved9[0x1];
    RW u32 RCGCHIB;
    RW u32 RCGCUART;
    RW u32 RCGCSSI;
    RW u32 RCGCI2C;
};

static_assert(offsetof(SYSCTL_Block, PBORCTL) == 0x030);
static_assert(offsetof(SYSCTL_Block, RIS) == 0x050);
static_assert(offsetof(SYSCTL_Block, IMC) == 0x054);
static_assert(offsetof(SYSCTL_Block, MISC) == 0x058);
static_assert(offsetof(SYSCTL_Block, RESC) == 0x05C);
static_assert(offsetof(SYSCTL_Block, RCC) == 0x060);
static_assert(offsetof(SYSCTL_Block, GPIOHBCTL) == 0x06C);
static_assert(offsetof(SYSCTL_Block, RCC2) == 0x070);
static_assert(offsetof(SYSCTL_Block, MOSCCTL) == 0x07C);
static_assert(offsetof(SYSCTL_Block, DSLPCLKCFG) == 0x144);
static_assert(offsetof(SYSCTL_Block, SYSPROP) == 0x14C);
static_assert(offsetof(SYSCTL_Block, PIOSCCAL) == 0x150);
static_assert(offsetof(SYSCTL_Block, PIOSCSTAT) == 0x154);
static_assert(offsetof(SYSCTL_Block, PLLFREQ0) == 0x160);
static_assert(offsetof(SYSCTL_Block, PLLFREQ1) == 0x164);
static_assert(offsetof(SYSCTL_Block, PLLSTAT) == 0x168);
// TODO: add missing registers here
static_assert(offsetof(SYSCTL_Block, RCGCGPIO) == 0x608);
static_assert(offsetof(SYSCTL_Block, RCGCDMA) == 0x60C);
static_assert(offsetof(SYSCTL_Block, RCGCHIB) == 0x614);
static_assert(offsetof(SYSCTL_Block, RCGCUART) == 0x618);
static_assert(offsetof(SYSCTL_Block, RCGCSSI) == 0x61C);
static_assert(offsetof(SYSCTL_Block, RCGCI2C) == 0x620);

#define SYSCTL ((volatile SYSCTL_Block*)(0x400FE000))

void sys_init()
{
    // Enable all GPIO modules to be accessed via AHB (Advanced High-Speed bus)
    u8 AHB_GPIOS = 0;
    AHB_GPIOS |= SYSCTL_GPIOHBCTL_PORTA;
    AHB_GPIOS |= SYSCTL_GPIOHBCTL_PORTB;
    AHB_GPIOS |= SYSCTL_GPIOHBCTL_PORTF;
    SYSCTL->GPIOHBCTL = AHB_GPIOS;

    // Enable clock for GPIOs
    u8 GPIO_PERIPHS = 0;
    GPIO_PERIPHS |= SYSCTL_RCGCGPIO_R0; // PORTA
    GPIO_PERIPHS |= SYSCTL_RCGCGPIO_R1; // PORTB
    GPIO_PERIPHS |= SYSCTL_RCGCGPIO_R5; // PORTF
    SYSCTL->RCGCGPIO = GPIO_PERIPHS;

    // Enable clocks for I2Cs
    u8 I2C_PERIPHS = 0;
    I2C_PERIPHS |= SYSCTL_RCGCI2C_R0; // I2C0
    SYSCTL->RCGCI2C = I2C_PERIPHS;

    // Enable clocks for UARTs
    u8 UART_PERIPHS = 0;
    UART_PERIPHS |= SYSCTL_RCGCUART_R0; // UART0
    SYSCTL->RCGCUART = UART_PERIPHS;

    // This delay is needed in order to properly initialize clock for peripherals
    __asm("NOP");
    __asm("NOP");
    __asm("NOP");
}