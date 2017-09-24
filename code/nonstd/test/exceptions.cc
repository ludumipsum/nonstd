/** Exception Smoke Tests
 *  =====================
 *  To make sure C++ works the way we think it should work.
 */

#include <testrunner/testrunner.h>


namespace nonstd_test {
namespace exceptions {

TEST_CASE("Exception handlers") {
    SECTION("should throw and catch") {
        REQUIRE_THROWS([]() {
            throw std::logic_error("Yeah, this is an error");
        }());
    }
}

} /* namespace nonstd_test */
} /* namespace exceptions */
