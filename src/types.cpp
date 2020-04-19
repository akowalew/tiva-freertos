///////////////////////////////////////////////////////////////////////////////
// Definitions of most used type definitions
///////////////////////////////////////////////////////////////////////////////

// Shorthands for sized-types
using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

// Helper definitions used when declaring hardware blocks
#define RW 
#define RO const
#define WO
#define W1C
#define RW1C

//! Generic buffer to store byte data
struct Buffer
{
	const u8* data;
	u8 size;
};