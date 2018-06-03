/** Primitive Types
 *  ===============
 *  Defines some foundational types used across different pieces of the system.
 */

#pragma once

#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>


/** Basic Types
 *  -----------
 */
using u8       = uint8_t;
using u16      = uint16_t;
using u32      = uint32_t;
using u64      = uint64_t;
using i8       = int8_t;
using i16      = int16_t;
using i32      = int32_t;
using i64      = int64_t;
using f32      = float;
using f64      = double;
using f_long   = long double;
using cstr     = char *;
using c_cstr   = char const *;
using ptrdiff  = ptrdiff_t;
using ptr      = uint8_t *;
using c_ptr    = uint8_t const *;
using void_ptr = void *;

// User Defined Literals have a reduced set of possible overloads, and those
// usually don't exactly match the type you want to operate on. These types
// allow us to make explicit the conceptual differences implied by that.
using udl_raw_t   = char const *;
using udl_int_t   = unsigned long long int;
using udl_float_t = long double;

// IDs uniquely identify game entities. We reserve the bottom few for special
// meanings in our container types (unset, deleted, etc).
using ID = u64;


/** Power-of-two Byte Prefixes
 *  --------------------------
 *  NB. these implicitly cast the provided value to 64-bit int
 *
 *  TODO: I have a feeling a user defined literal set of these would be helpful.
 */
#define KBYTES(N) ((i64)N*1024)
#define MBYTES(N) ((i64)N*1024*1024)
#define GBYTES(N) ((i64)N*1024*1024*1024)
#define TBYTES(N) ((i64)N*1024*1024*1024*1024)


/** Human-Readable Temporal Unit Conversions
 *  ----------------------------------------
 *  TODO: I _know_ a user defined literal set of these would be helpful.
 */
#define NS_PER_NS  1
#define NS_PER_US  1000
#define NS_PER_MS  1000000
#define NS_PER_SEC 1000000000

#define US_PER_MS  1000
#define US_PER_SEC 1000000

#define MS_PER_SEC 1000


/** Print Helpers
 *  -------------
 *  Usage: `fn("My u64: " Fu64 "", my_u64)`.
 *
 *  NOTE: Try to use {fmt} rather than these macros.
 */
#define  Fu8     "%"  PRIu8
#define Fu16     "%" PRIu16
#define Fu32     "%" PRIu32
#define Fu64     "%" PRIu64
#define  Fi8     "%"  PRId8
#define Fi16     "%" PRId16
#define Fi32     "%" PRId32
#define Fi64     "%" PRId64
#define Fint     "%"    "d"
#define  Fx8   "0x%"  PRIX8
#define Fx16   "0x%" PRIX16
#define Fx32   "0x%" PRIX32
#define Fx64   "0x%" PRIX64
#define Fsize_t  "%zu"
#define Fptr     "%p"
#define Fptrdiff "%td"
