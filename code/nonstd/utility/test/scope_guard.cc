/** Scope Guard Smoke Tests
 *  =======================
 */

#include <testrunner/testrunner.h>

#include "nonstd/utility/scope_guard.h"

namespace nonstd_test {
namespace scope_guard {

using nonstd::make_guard;

i32 freeFunction() { return 0; }

struct Functor {
    i32 operator()() {
        return 0;
    }
};


TEST_CASE("Scope Guard API Demo", "[nonstd][api][scop guards]") {

    Functor functor;

    SECTION("Creating Scope Guards") {
        auto from_lambdas  = make_guard([&](){ return 0; });
        auto from_funtions = make_guard(freeFunction);
        auto from_functors = make_guard(functor);
    }
}

} /* namespace scope_guard */
} /* namespace nonstd_test */
