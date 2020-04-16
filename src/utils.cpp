///////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////

// Generic hook to be called, when CHECK(...) macro detects failure
[[noreturn]] void check_failed(const char* file, const char* function, int line)
{
	// You should observe in debugger values of that variables
	// and that way detect, where assertion was failed
	static_cast<void>(file);
	static_cast<void>(function);
	static_cast<void>(line);

	// Abort the program
	while(1);
}

//! Helper macro, hangs if specified condition is failed (=false)
// Especially useful, when calling functions which can really fail (e.g. memory allocation) 
#define CHECK(x) if(!(x)) check_failed(__FILE__, __FUNCTION__, __LINE__)

//! Generic hook to be called, when assert(...) macro detects failure
[[noreturn]] void assert_failed(const char* file, const char* function, int line)
{
	// You should observe in debugger values of that variables
	// and that way detect, where assertion was failed
	static_cast<void>(file);
	static_cast<void>(function);
	static_cast<void>(line);

	// Abort the program
	while(1);
}

//! Simple assertion macro, available only when NDEBUG is not present
// In other case, it evaluates to __builtin_unreachable, optimization hint for compiler
// Useful for checking pre- and post-conditions and invariants
#ifndef NDEBUG
#define assert(x) if(!(x)) assert_failed(__FILE__, __FUNCTION__, __LINE__)
#else
#define assert(x) if(!(x)) __builtin_unreachable();
#endif