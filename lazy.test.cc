/** Lazy Initialization Tests
 *  =========================
 *
 */

#include <nonstd/lazy.h>
#include <testrunner/testrunner.h>

#include <nonstd/nonstd.h>
#include <nonstd/memory.h>
#include <platform/memory/memory.h>
#include <testrunner/helpers/construction_counter.h>


namespace nonstd_test::lazy {

using namespace Catch::Matchers;
using nonstd::lazy;
using nonstd::nr_ptr;
using nonstd::test::construction_counter;

constexpr i32 test_value = 42;
lazy<i32> global_instance { test_value };
TEST_CASE("Global Lazy Wrappers", "[nonstd][lazy]") {
    SECTION("Should be not initialized until accessed") {
        REQUIRE(global_instance.initialized() == false);
    }
    SECTION("Should be initialized once accessed") {
        REQUIRE(*global_instance == test_value);
        REQUIRE(global_instance.initialized() == true);
    }
}

TEST_CASE("Lazy Wrappers", "[nonstd][lazy]") {
    SECTION("should neither copy nor move the contained") {
        // Create a lazy wrapper around a counter, initialize it, and get a
        // reference to the contained, so we can use them in subsequent tests
        lazy<construction_counter> lazy_counter { };
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
            // reference to the contained
            lazy<inline_test_t> lazy_container { construction_counter{} };
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
            // reference to the contained
            construction_counter initial_counter;
            lazy<inline_test_t> lazy_container { initial_counter };
            REQUIRE(lazy_container.initialized() == false);
            auto& counter = (*lazy_container).counter;
            REQUIRE(lazy_container.initialized() == true);

            // Confirm the correct number of moves and copies were done
            CHECK(counter.copies == 1);
            CHECK(counter.moves_in == 1);
            CHECK(counter.moves_out == 0);
        }
    }

    SECTION("should be able to correctly nest") {
        lazy<lazy<i32>> nested_instance  { test_value };

        REQUIRE(nested_instance.initialized() == false);
        REQUIRE((*nested_instance).initialized() == false);
        REQUIRE(nested_instance.initialized() == true);
        REQUIRE(**nested_instance == test_value);
        REQUIRE((*nested_instance).initialized() == true);
    }
}

lazy<nr_ptr<i32>> lazy_nr { "test/lazy_nr" };
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
