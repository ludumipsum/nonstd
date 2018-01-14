/** Scope Guard Smoke Tests
 *  =======================
 */

#include <nonstd/scope_guard.h>
#include <testrunner/testrunner.h>

#include <functional>
#include <nonstd/core/primitive_types.h>


namespace nonstd_test {
namespace scope_guard {

using nonstd::make_guard;

i32 freeFunction() { REQUIRE(true); return 0; }

struct CallableObject {
    i32 operator()() {
        REQUIRE(true); return 0;
    }
    i32 memberFn() {
        REQUIRE(true); return 0;
    }
};


TEST_CASE("Scope Guard API Demo", "[nonstd][api][scope guards]") {

    CallableObject obj;
    i32 (CallableObject::*member_fn)() = &CallableObject::memberFn;

    SECTION("Creating Scope Guards") {
        { auto from_lambdas          = make_guard([&](){ REQUIRE(true); });    }
        { auto from_funtions         = make_guard(freeFunction);               }
        { auto from_callable_objects = make_guard(obj);                        }
        { auto bound_member_fn_val   = make_guard(std::bind(member_fn, obj));  }
        { auto bound_member_fn_ptr   = make_guard(std::bind(member_fn, &obj)); }
        { auto bound_member_fn_ref   = make_guard(std::bind(member_fn,
                                                            std::ref(obj)));   }
    }

    SECTION("Guards with Side Effects") {
        int foo = 0; REQUIRE(foo == 0);
        { auto guard = make_guard([&](){ foo += 1; }); }
        REQUIRE(foo == 1);
    }

    SECTION("Guards that Roll-Back Actions") {
        int foo = 0;
        {
            int initial_foo = foo;
            foo += 1;
            auto g1 = make_guard([&](){ foo = initial_foo; });
            foo += 1;
            REQUIRE(foo == 2);
        }
        REQUIRE(foo == 0);
    }

    SECTION("Guards that Get Dismissed") {
        {
            auto guard = make_guard([&](){ REQUIRE(false); });
            guard.dismiss();
        }
        REQUIRE(true);
    }

    SECTION("Guards that are Triggered By Exceptions") {
        int foo = 0;
        REQUIRE_THROWS([&](){
            int initial_foo = foo;
            foo += 1;
            auto g1 = make_guard([&](){ foo = initial_foo; }); //< Set foo to 0
            REQUIRE(foo == 1);

            throw std::logic_error("an expected error");

            g1.dismiss(); //< Should not be reached
        }());
        REQUIRE(foo == 0);
    }
}

} /* namespace scope_guard */
} /* namespace nonstd_test */
