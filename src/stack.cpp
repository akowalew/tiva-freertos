///////////////////////////////////////////////////////////////////////////////
// Stack initialization stubs
//
// Initialization of stack is done by MCU and everything we need is
// to set stack pointer to have correct value
///////////////////////////////////////////////////////////////////////////////

// Provided by linker script
extern unsigned __stacktop;

// Initialize stack pointer
__attribute__((section(".stack"), used)) unsigned* __stack_init = &__stacktop;
