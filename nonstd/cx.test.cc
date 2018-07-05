/* Constexpr Compiler Standards-Compliance Verification
 * ====================================================
 * This file is a quick test to confirm that the compiler handles constexpr
 * conditionals correctly.
 */

#include <platform/testrunner/testrunner.h>


namespace nonstd_test {
namespace cx {

TEST_CASE("if constexpr", "[nonstd][cx]") {
    if constexpr (true)  { REQUIRE(true);  }
    if constexpr (false) { REQUIRE(false); }
}

} /* namespace cx */
} /* namespace nonstd_test */
