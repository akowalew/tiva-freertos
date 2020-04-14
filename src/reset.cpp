///////////////////////////////////////////////////////////////////////////////
// Reset handling routines
///////////////////////////////////////////////////////////////////////////////

//
// Global declarations
//

extern int main();

///////////////////////////////////////////////////////////////////////////////
// Startup code for microcontroller
///////////////////////////////////////////////////////////////////////////////

//
// Symbols provided by linker script
//

extern unsigned __data_start;
extern unsigned __data_end;
extern unsigned __data_load;

extern unsigned __bss_start;
extern unsigned __bss_end;

//
// Private initialization routines
//

//! Copies data to initialized global variables
static void copy_init_data()
{
    auto src = &__data_load;
    auto dst = &__data_start;
    while (dst < &__data_end) {
        *(dst++) = *(src++);
    }
}

//! Erases BSS section
// BSS is for global variables storage, which has to be zeroed
// before program goes to main
static void zero_bss()
{
    auto dst = &__bss_start;
    while (dst < &__bss_end) {
        *(dst++) = 0;
    }
}

//! RESET signal handler
// Performs global initialization and runs `main` procedure
// When it has finished work, it simply hangs.
[[noreturn]] void RESET_handler() 
{
    // Run low-level initialization
    copy_init_data();
    zero_bss();

    // NOTE: We are not running here any static objects initialization
    // (e.g. constructors). We would not like to use them.

    // We have to turn `pedantic` warnings, because of message
    // "ISO C++ forbids taking address of function ::main"...
    #pragma GCC diagnostic ignored "-Wpedantic"
    main(); // Run the application
    #pragma GCC diagnostic pop 

    // When `main` has go out, we have a serious problem
    while(true);

    // We should really not reach this code
    __builtin_unreachable();
}
