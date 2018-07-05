/** System Time (Standalone) Tests
 *  ==============================
 *  This library is tiny, so there's not a lot to consider in testing it --
 *  basically all we can do is run it, sleep, and run it again, and confirm the
 *  numbers are different. Note, though, that even that isn't necessarily a
 *  guaranteed property of the high precision clock (the one we use). There are
 *  "safer" clocks available, for example ones enforcing monotonicity, but
 *  because we prefer greater precision moment to moment at the expense of
 *  possible occasional skips, we don't use them.
 *
 *  Our clock, therefore, reports time in nanoseconds since an arbitrary epoch.
 *  It may run forwards, backwards, or not at all. Any u64 is a viable output.
 *
 *  Given the total lack of guarantees provided by the clock we rely on, we just
 *  test a few sanity checks — stuff that isn't technically a contract violation,
 *  but that would be problematic for us if it turned out wrong.
 */

#include <nonstd/wallclock.h>
#include <platform/testrunner/testrunner.h>


namespace test {
namespace wallclock {

using nonstd::wallclock;
using namespace nonstd::literals::chrono_literals;

TEST_CASE("System timer", "[time][standalone]") {
    SECTION("returns a nonzero u64") {
        REQUIRE(wallclock::now() != std::chrono::nanoseconds::zero());
    }
    SECTION("returns different numbers at different times") {
        auto point_a = wallclock::now();
        wallclock::delay(1.5s);
        auto point_b = wallclock::now();
        REQUIRE(point_a != point_b);
    }
}

} /* namespace wallclock */
} /* namespace test */
