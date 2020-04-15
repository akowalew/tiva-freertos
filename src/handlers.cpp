///////////////////////////////////////////////////////////////////////////////
// Handlers for interrupts and their default implementation
///////////////////////////////////////////////////////////////////////////////

/*
 * Exception handlers.
 */
void xPortPendSVHandler( void ) __attribute__ (( naked ));
void xPortSysTickHandler( void );
void vPortSVCHandler( void ) __attribute__ (( naked ));

void NMI_handler()
{
    while(1);
}

void HARDFAULT_handler()
{
    while(1);
}

void BUSFAULT_handler()
{
    while(1);
}

void USAGEFAULT_handler()
{
    while(1);
}

void SVCALL_handler()
{
    vPortSVCHandler();
}

void PENDSV_handler()
{
    xPortPendSVHandler();
}

void SYSTICK_handler()
{
    xPortSysTickHandler();
}

void DUMMY_handler() 
{ 
    while(true); 
}

// Helper typedef
typedef void (*ptr_func_t)();

// Vector table for handlers
// This array will be placed in ".vectors" section defined in linker script.
__attribute__((section(".vectors"), used)) ptr_func_t __isr_vectors[] = {
    RESET_handler,
    NMI_handler, // NMI
    HARDFAULT_handler,
    DUMMY_handler, // MEMMANAGE
    BUSFAULT_handler,
    USAGEFAULT_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    SVCALL_handler, // SVCALL,
    DUMMY_handler, // DEBUGMONITOR,
    DUMMY_handler,
    PENDSV_handler, // PENDSV
    SYSTICK_handler,
    DUMMY_handler, // GPIO Port A
    DUMMY_handler, // GPIO Port B
    DUMMY_handler, // GPIO Port C
    DUMMY_handler, // GPIO Port D
    DUMMY_handler, // GPIO Port E
    DUMMY_handler, // UART0
    DUMMY_handler, // UART1
    DUMMY_handler, // SSI0
    DUMMY_handler, // I 2 C0
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
    DUMMY_handler, // GPIOF
};