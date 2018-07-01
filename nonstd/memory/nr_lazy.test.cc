#include <nonstd/memory/nr_lazy.h>
#include <platform/testrunner/testrunner.h>

#include <nonstd/nonstd.h>
#include <nonstd/memory.h>
#include <platform/testrunner/helpers/construction_counter.h>
#include <platform/memory/memory.h>


namespace nonstd_test::nr_lazy {

using nonstd::nr_lazy;
using nonstd::test::construction_counter;

TEST_CASE("Lazy Named-Register Pointers", "[nonstd][nr_ptr][lazy][memory]") {
    memory::init();

    /** Creating Lazy Named-Register Pointers
     *  -------------------------------------
     *  `nr_lazy`s are pretty much exactly what they claim to be; pointers
     *  backed by a platform-side named register that is lazily initialized.
     *  Creation of an `nr_lazy` implies a memory allocation, which will be
     *  performed upon the first access of the `nr_lazy`s value as an
     *  `allocate_or_find` buffer operation.
     */
    SECTION("Creating `nr_lazy`s") {
        // constructing these is exactly like constructing an `nr_ptr`...
        nr_lazy<f32> lazy_float { "test/nr_lazy/a_float", 42.f };
        // ... but they don't start out initialized...
        REQUIRE(lazy_float.initialized() == false);
        // ... and can be dereferenced to get their value ...
        REQUIRE(*lazy_float == 42.f);
        // ... which initializes them.
        REQUIRE(lazy_float.initialized() == true);

        // Also, their stored values can be modified.
        *lazy_float = 4.2f;
        REQUIRE(*lazy_float == 4.2f);
    }


    struct inline_test_t {
        inline_test_t(construction_counter const& counter)
            : counter(counter) { }
        inline_test_t(construction_counter && counter)
            : counter(std::forward<construction_counter>(counter)) { }
        construction_counter counter;
    };
    SECTION("Copy and move checks pt. 1") {
        // Create a lazy wrapper around a counter, initialize it, and get a
        // reference to the contained
        nr_lazy<construction_counter> lazy_counter { "test/nr_lazy/counter_1" };
        REQUIRE(lazy_counter.initialized() == false);
        auto& counter = *lazy_counter;
        REQUIRE(lazy_counter.initialized() == true);

        // Check that the whole lazy initialization process hasn't resulted in
        // any copies, and no more than two moves
        CHECK(counter.copies == 0);
        CHECK(counter.moves_in == 0);
        CHECK(counter.moves_out == 0);
    }

    SECTION("Copy and move checks pt. 2") {
        // Create a lazy wrapper around a counter, initialize it, and get a
        // reference to the contained
        nr_lazy<inline_test_t> lazy_container {
            "test/nr_lazy/counter_2",
            construction_counter{}
        };
        REQUIRE(lazy_container.initialized() == false);
        auto& counter = (*lazy_container).counter;
        REQUIRE(lazy_container.initialized() == true);

        // Confirm the correct number of moves and copies were done
        CHECK(counter.copies == 0);
        CHECK(counter.moves_in == 2);
        CHECK(counter.moves_out == 0);
    }

    SECTION("Copy and move checks pt. 3") {
        // Create a lazy wrapper around a counter, initialize it, and get a
        // reference to the contained
        construction_counter initial_counter;
        nr_lazy<inline_test_t> lazy_container {
            "test/nr_lazy/counter_3",
            initial_counter
        };
        REQUIRE(lazy_container.initialized() == false);
        auto& counter = (*lazy_container).counter;
        REQUIRE(lazy_container.initialized() == true);

        // Confirm the correct number of moves and copies were done
        CHECK(counter.copies == 1);
        CHECK(counter.moves_in == 1);
        CHECK(counter.moves_out == 0);
    }
}

} /* namespace nonstd_test::nr_lazy */
