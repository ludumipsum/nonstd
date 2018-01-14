/** Primitive Types
 *  ===============
 *  Defines some foundational types used across different pieces of the system.
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cinttypes>


/** Basic Types
 *  ----------- */
typedef uint8_t          u8;
typedef uint16_t        u16;
typedef uint32_t        u32;
typedef uint64_t        u64;
typedef int8_t           i8;
typedef int16_t         i16;
typedef int32_t         i32;
typedef int64_t         i64;
typedef float           f32;
typedef double          f64;
typedef long double     f_long;
typedef char *          cstr;
typedef char const *    c_cstr;
typedef ptrdiff_t       ptrdiff;
typedef uint8_t *       ptr;
typedef uint8_t const * c_ptr;
typedef void *          void_ptr;

// User Defined Literals have a reduced set of possible overloads.
using udl_raw_t   = char const *;
using udl_int_t   = unsigned long long int;
using udl_float_t = long double;

/* IDs uniquely identify game entities. We reserve the bottom few for special
   meanings in our container types (unset, deleted, etc). */
typedef u64 ID;


/* Print helpers. Used: `"My u64: " Fu64 ""` */
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


//TODO: Keep an eye on this section. If it gets big enough, we should split it
//      out to its own file.

/* Power-of-two Byte Prefixes
   NB. these implicitly cast the provided value to 64-bit int */
#define KBYTES(N) ((i64)N*1024)
#define MBYTES(N) ((i64)N*1024*1024)
#define GBYTES(N) ((i64)N*1024*1024*1024)
#define TBYTES(N) ((i64)N*1024*1024*1024*1024)


/* Human-readable temporal unit conversions */
#define NS_PER_NS  1
#define NS_PER_US  1000
#define NS_PER_MS  1000000
#define NS_PER_SEC 1000000000

#define US_PER_MS  1000
#define US_PER_SEC 1000000

#define MS_PER_SEC 1000
