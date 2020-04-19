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

//! Generic hook to be called, when assert(...) macro detects failure
template<typename X, typename Y>
[[noreturn]] void assert_equal_failed(const char* file, const char* function, int line, X x, Y y)
{
	// You should observe in debugger values of that variables
	// and that way detect, where assertion was failed
	static_cast<void>(file);
	static_cast<void>(function);
	static_cast<void>(line);
	static_cast<void>(x);
	static_cast<void>(y);

	// Abort the program
	while(1);
}

//! Simple assertion macro for testing equality, available only when NDEBUG is not present
// In other case, it evaluates to __builtin_unreachable, optimization hint for compiler
// Useful for checking pre- and post-conditions and invariants
#ifndef NDEBUG
#define assert_equal(x, y) if((x) != (y)) assert_equal_failed(__FILE__, __FUNCTION__, __LINE__, (x), (y))
#else
#define assert_equal(x, y) if((x) != (y)) __builtin_unreachable();
#endif

//! Converts numerical value to string
// Similar to stdlib's `itoa`, but does not add any '\0' char at the end
// and writes data "from_back", instead of re-writing it at the begin of string
char* to_digits_ascii(u32 value, char* buffer_end)
{
	// 1234567 / 10 = 123456
	// 1234567 - 123456*10 = 7
	// -------
	// 123456 / 10 = 12345
	// 123456 - 12345*10 = 6
	// -------
	// 12345 / 10 = 1234
	// 12345 - 1234*10 = 5
	// ...

	// 7654321
	//   ...
	// 1234567

	// 1000 / 10 = 100
	// 1000 - 100*10 = 0
	
	auto buffer = buffer_end;

	do {
		// TODO: May be done more performant by lookup table!
		const auto tmp = (value / 10);
		const u8 digit = (value - tmp*10);
		value = tmp;

		assert(digit < 10);
		*(--buffer) = digit + '0';
	} 
	while(value);

	return buffer;
}