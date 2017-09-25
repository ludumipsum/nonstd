/* Compare Utility Function Smoke Tests
 * =====================================
 * Smoke tests for the (typically noexcept) nonstd::compare and nonstd::equal_to
 * family of functions.
 */

#include <testrunner/testrunner.h>

#include "nonstd/std_ish/compare.h"
#include "nonstd/utility/optional.h"


namespace nonstd_test {
namespace compare {

static_assert( noexcept(nonstd::compare((u64)1,(u64)2)),
               "nonstd::compare(u64,u64) is correctly noexcept");
static_assert( noexcept(nonstd::compare((u64)1,(u32)2)),
               "nonstd::compare(u64,u32) is correctly noexcept");
static_assert(!noexcept(nonstd::compare("a","b")),
               "nonstd::compare(c_int,c_int) cannot be noexcept");

static_assert( noexcept(nonstd::equal_to((u64)1,(u64)2)),
               "nonstd::equal_to(u64,u64) is correctly noexcept");
static_assert( noexcept(nonstd::equal_to((u64)1,(u32)2)),
               "nonstd::equal_to(u64,u32) is correctly noexcept");
static_assert(!noexcept(nonstd::equal_to("a","b")),
               "nonstd::equal_to(c_int,c_int) cannot be noexcept");

} /* namespace nonstd_test */
} /* namespace compare */
