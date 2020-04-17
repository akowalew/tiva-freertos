///////////////////////////////////////////////////////////////////////////////
// Handlers for interrupts and their default implementation
///////////////////////////////////////////////////////////////////////////////

//
// Exception handlers from FreeRTOS
//

void xPortPendSVHandler();
void xPortSysTickHandler();
void vPortSVCHandler();

//
// Custom exception handlers
//

static void NMI_handler()
{
    while(1);
}

extern "C" __attribute__((used)) 
[[noreturn]] void get_registers_from_stack(const u32* fault_stack)
{
    const auto r0 = fault_stack[0]; static_cast<void>(r0);
    const auto r1 = fault_stack[1]; static_cast<void>(r1); 
    const auto r2 = fault_stack[2]; static_cast<void>(r2);
    const auto r3 = fault_stack[3]; static_cast<void>(r3);

    const auto r12 = fault_stack[4]; static_cast<void>(r12);
    const auto lr = fault_stack[5]; static_cast<void>(lr);
    const auto pc = fault_stack[6]; static_cast<void>(pc);
    const auto psr = fault_stack[7]; static_cast<void>(psr);

    /* When the following line is hit, the variables contain the register values. */
    while(true);
}

static void __attribute__((naked)) HARDFAULT_handler()
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word get_registers_from_stack    \n"
    );
}

static void MEMMANAGEFAULT_handler()
{
    while(1);
}

static void BUSFAULT_handler()
{
    while(1);
}

static void USAGEFAULT_handler()
{
    while(1);
}

static void DUMMY_handler() 
{ 
    while(true); 
}

// Helper typedef for interrupt handlers function
using Handler = void(*)();

// Vector table for interrupt handlers
__attribute__((section(".vectors"), used)) Handler __isr_vectors[] = {
    RESET_handler,
    NMI_handler, // NMI
    HARDFAULT_handler,
    MEMMANAGEFAULT_handler, // MEMMANAGE
    BUSFAULT_handler,
    USAGEFAULT_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    vPortSVCHandler, // SVCALL,
    DUMMY_handler, // DEBUGMONITOR,
    DUMMY_handler,
    xPortPendSVHandler, // PENDSV
    xPortSysTickHandler,
    DUMMY_handler, // GPIO Port A
    DUMMY_handler, // GPIO Port B
    DUMMY_handler, // GPIO Port C
    DUMMY_handler, // GPIO Port D
    DUMMY_handler, // GPIO Port E
    UART0_handler, // UART0
    DUMMY_handler, // UART1
    DUMMY_handler, // SSI0
    I2C0_handler, // I 2 C0
    DUMMY_handler, // PWM0 Fault
    DUMMY_handler, // PWM0 Generator 0
    DUMMY_handler, // PWM0 Generator 1
    DUMMY_handler, // PWM0 Generator 2
    DUMMY_handler, // QEI0
    DUMMY_handler, // ADC0 Sequence 0
    DUMMY_handler, // ADC0 Sequence 1
    DUMMY_handler, // ADC0 Sequence 2
    DUMMY_handler, // ADC0 Sequence 3
    DUMMY_handler, // Watchdog Timers 0 and 1
    DUMMY_handler, // 16/32-Bit Timer 0A
    DUMMY_handler, // 16/32-Bit Timer 0B
    DUMMY_handler, // 16/32-Bit Timer 1A
    DUMMY_handler, // 16/32-Bit Timer 1B
    DUMMY_handler, // 16/32-Bit Timer 2A
    DUMMY_handler, // 16/32-Bit Timer 2B
    DUMMY_handler, // Analog Comparator 0
    DUMMY_handler, // Analog Comparator 1
    DUMMY_handler, // Reserved
    DUMMY_handler, // System Control
    DUMMY_handler, // Flash
    GPIOF_handler, // GPIOF
};