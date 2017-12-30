/** Lazy Initialization Tests
 *  =========================
 *
 */

#include "nonstd/utility/lazy.h"

#include <nonstd/core/primitive_types.h>
#include <nonstd/memory/nr_ptr.h>
#include <nonstd/preprocessor/homogenize.h>

#include <memory/memory.h>
#include <testrunner/testrunner.h>
#include <testrunner/helpers/construction_counter.h>

namespace nonstd_test::lazy {
using namespace Catch::Matchers;
using nonstd::make_lazy;
using nonstd::nr_ptr;
using nonstd::test::construction_counter;

constexpr i32 test_value = 42;
auto global_instance = make_lazy<i32>(test_value);
TEST_CASE("Global Lazy Wrappers", "[nonstd][lazy]") {
    SECTION("Should be not initialized until accessed") {
        REQUIRE(global_instance.initialized() == false);
    }
    SECTION("Should be initialized once accessed") {
        REQUIRE(*global_instance == test_value);
        REQUIRE(global_instance.initialized() == true);
    }
}

// There's some kind of template expansion problem in these right now -- we
// wind up trying to call the constructor via placement new with a tuple
// unpack structure instead of the actual unpacked arguments.
#if 0
auto nested_instance =
    make_lazy<nonstd::lazy_initializer<i32>>(
        make_lazy<i32>(test_value));

SECTION("Nested lazy wrappers should work") {
    REQUIRE(nested_instance.initialized() == false);
    REQUIRE((*nested_instance).initialized() == false);
    REQUIRE(nested_instance.initialized() == true);
    REQUIRE(**nested_instance == test_value);
    REQUIRE((*nested_instance).initialized() == true);
}
#endif

TEST_CASE("Lazy Wrappers", "[nonstd][lazy]") {
    SECTION("should neither copy nor move the contained") {
        // Create a lazy wrapper around a counter, initialize it, and get a
        // reference to the contained, so we can use them in subsequent tests
        auto lazy_counter = make_lazy<construction_counter>();
        REQUIRE(lazy_counter.initialized() == false);
        auto& counter = *lazy_counter;
        REQUIRE(lazy_counter.initialized() == true);

        // Check that the whole lazy initialization process hasn't resulted in
        // any copies, and no more than two moves
        CHECK(counter.copies == 0);
        CHECK(counter.moves_in == 0);
        CHECK(counter.moves_out == 0);
    }

    SECTION("should require a minimum of moves and copies of arguments") {
        struct inline_test_t {
            inline_test_t(construction_counter const& counter)
                : counter(counter) { }
            inline_test_t(construction_counter && counter)
                : counter(std::forward<construction_counter>(counter)) { }
            construction_counter counter;
        };

        SECTION("with inline argument construction") {
            // Create a lazy wrapper around a counter, initialize it, and get a
            // reference to the contained, so we can use them in subsequent tests
            auto lazy_container = make_lazy<inline_test_t>(construction_counter{});
            REQUIRE(lazy_container.initialized() == false);
            auto& counter = (*lazy_container).counter;
            REQUIRE(lazy_container.initialized() == true);

            // Confirm the correct number of moves and copies were done
            CHECK(counter.copies == 0);
            CHECK(counter.moves_in == 2);
            CHECK(counter.moves_out == 0);
        }

        SECTION("with copied arguments") {
            // Create a lazy wrapper around a counter, initialize it, and get a
            // reference to the contained, so we can use them in subsequent tests
            construction_counter initial_counter;
            auto lazy_container = make_lazy<inline_test_t>(initial_counter);
            REQUIRE(lazy_container.initialized() == false);
            auto& counter = (*lazy_container).counter;
            REQUIRE(lazy_container.initialized() == true);

            // Confirm the correct number of moves and copies were done
            CHECK(counter.copies == 1);
            CHECK(counter.moves_in == 1);
            CHECK(counter.moves_out == 0);
        }
    }
}

auto lazy_nr = make_lazy<nr_ptr<i32>>("test/lazy_nr");
TEST_CASE("Lazy Wrappers Around nr_ptr", "[nonstd][lazy]") {
    SECTION("should not initialize until accessed") {
        REQUIRE(lazy_nr.initialized() == false);
    }
    memory::init();
    SECTION("can be accessed once the memory system is initialized") {
        REQUIRE(*lazy_nr);
        **lazy_nr = test_value;
        REQUIRE(**lazy_nr == test_value);
    }
}

} /* namespace nonstd_test::lazy */
