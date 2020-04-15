///////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////

// Generic hook to be called, when CHECK(...) macro detects failure
template<typename T>
[[noreturn]] void check_failed(const char* file, const char* function, int line, T result)
{
	// You should observe in debugger values of that variables
	// and that way detect, where assertion was failed
	static_cast<void>(file);
	static_cast<void>(function);
	static_cast<void>(line);
	static_cast<void>(result);

	// Abort the program
	while(1);
}

//! Helper macro, hangs if specified condition is failed (=false)
#define CHECK(x) if(const auto result = (x); !result) check_failed(__FILE__, __FUNCTION__, __LINE__, result)
