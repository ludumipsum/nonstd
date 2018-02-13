/** Array View Tests
 *  ================
 *  GOAL: Validate the contract of the std::vector-like container, array<T>.
 *
 *  METHODOLOGY:
 *   - Verify initialization & helpers
 *   - Test accessors over a simple builtin type
 *   - Test edge conditions around empty, full, and over-full Arrays
 *   - Test over compound POD types
 */
#include <nonstd/array.h>
#include <testrunner/testrunner.h>

#include <nonstd/nonstd.h>
#include <memory/memory.h>


namespace test_array {

using nonstd::range;
using nonstd::buffer;
using nonstd::array;
using memory::find_array;
using memory::allocate_array;

/* Compound POD datatype used to test non-builtin-type Rings */
struct CompoundType {
    union {
        struct { u16 a, b; };
        u32 word;
    };
};


/** API DEMONSTRATION
 *  =================
 *  Typed array are... well, they're arrays that have been typed. They keep
 *  data contiguous, and resize when you `consume` or `push` more data than
 *  they can currently store, and that's about all there is to them.
 *
 *  Our arrays are "containers" -- they operate on buffer pointers provided by
 *  the platform layer, giving you a typed view of the given buffer, treating it
 *  as an array. Arrays are cheap to create since all the allocations and state
 *  are stored in the underlying buffer.
 *
 *  The purpose of this test suite is to show you a couple basic usage examples,
 *  in a place that's very easy to find later if you get confused about how
 *  they're supposed to work.
 */

TEST_CASE("Array API Demo", "[api][memory][array]") {
    memory::init();

    /** Creating Arrays
     *  --------------
     *  Arrays provide a view over a buffer, so in order to get an array, we'll
     *  first need a buffer object to provide storage. There are a few helpers
     *  which make allocating one of these easier, but take care -- anything
     *  that's named 'allocate*' will result in a memory allocation.
     *
     *  In general, if you can't decide which initializer to use, prefer the
     *  one highest up in this section (since they're ordered in terms
     *  of simplicity).
     */
    SECTION("Creating Arrays") {
        // In the simplest case, we just allocate an array directly.
        auto created_array = allocate_array<f32>("test_data", 5);
        // And in subsequent uses, we'll want to grab that array again.
        auto maybe_array = find_array<f32>("test_data");
        // If we want convenience over precision, we can directly construct
        // arrays from from just a buffer name. If the named buffer has already
        // been allocated, it will be used. Otherwise, it will be allocated
        // using whatever defaults are necessary.
        array<f32> simple_array{ "test_data" };

        // You can also do the buffer initialization by hand. All containers
        // provide a ::precompute_size static method and a ::initialize_buffer
        // static method to help with this, should you need to do it.
        auto required_size = array<f32>::precompute_size(5);
        auto manual_buffer = memory::allocate("manual_test", required_size);
        array<f32>::initialize_buffer(manual_buffer);
        array<f32> manual_creation{ manual_buffer };

        // Or the lookup by hand
        auto maybe_manual_buffer = memory::find("manual_test");
        if (!maybe_manual_buffer) { INFO("Error checking is good."); }
        array<f32> manual_lookup{ *maybe_manual_buffer };
    }

    /** Inserting And Reading Data
     *  --------------------------
     *  array<T> provides a pretty minimal array interface. They let you;
     *   * Push individual elements onto the end of the array
     *   * Consume a region and operate directly on a data pointer
     *   * Access elements by offset from the start of the array
     *   * Iterate all elements first->last
     *   * Erase a range of elements
     *   * Drop all elements from the array
     */
    SECTION("Inserting and Reading Data") {
        // We're just going to play with sequential numbers here, so let's start
        // with an array of, say, 60 u32s.
        auto numbers = allocate_array<u32>("demo", 60);

        // We can also pretend we have some external system we're pulling from.
        auto getNext = []() -> u32 {
            static u32 i = 0;
            return i++; // post-increment is intentional.
        };

        // Lets half-fill the array, one element at a time.
        for (auto i : range(30)) {
            numbers.push(getNext());
        }

        // Now let's claim space for 10 elements with only one call to the
        // numbers array, and populate that data via pointer indirection.
        {
            u32 * data = numbers.consume(10);
            for (auto i : range(10)) {
                *data = getNext();
                data++;
            }
        }

        // And then... compute the sum of all numbers. Why not?
        u32 sum = 0;
        for (auto&& n : numbers) {
            sum += n;
        }
        // The sum of (0,39) should be 780, by the way.
        REQUIRE(numbers.count() == 40);
        REQUIRE(sum == 780);

        // We can also look at a couple data points in particular
        auto& newest = numbers[39];
        auto& oldest = numbers[0];

        // NB. In production mode, there is no bounds checking on array
        // subscripting, so the below calls should be avoided.
        //
        // `last` will yield valid (if unwritten) data, but writing to it _will
        // not_ increment the array's write_index, so that data will be missed
        // in iteration, and will be overwritten by `push` or `consume`.
        //
        // `oob` will result in a segfault when the array attempts to
        // dereference an invalid pointer.

        // auto& last = numbers[59];
        // auto& oob  = numbers[590];

        // We can push or consume data past the initial size of the array, and
        // (if possible) the array will resize automatically.
        REQUIRE(numbers.capacity() == 60);
        for (auto i : range(30)) {
            numbers.push(getNext());
        }
        REQUIRE(numbers.capacity() > 60);

        // We can also erase data from the array.
        REQUIRE(numbers.count() == 70);

        // NB. This is a half-open range, so number[20] won't be erased.
        numbers.erase(10, 20);

        REQUIRE(numbers.count() == 60);
        REQUIRE(numbers[9]  == 9 );
        REQUIRE(numbers[10] == 20);

        // Finally, if we decide we want to reset a array back to empty, we can
        // drop all of the contents.
        numbers.drop();
        REQUIRE(numbers.count() == 0);
    }

}


/** FUNCTIONAL TESTS
 *  ================
 *  This section contains tests of the basic functionality.
 *
 *  All tests in this section should be written with BDD-stye sections such that
 *  they form completeish sentences when read sequentially. For example,
 *  "Optional types // over nones // should coerce to boolean correctly."
 */
TEST_CASE("Array containers", "[standalone][memory][array]") {
    c_cstr test_name = "smoketest/array";
    u64 num_test_points = 10;

    memory::init();
    auto size = array<f64>::precompute_size(num_test_points);
    auto buffer = memory::allocate(test_name, size);
    array<f64>::initialize_buffer(buffer);

    SECTION("should initialize and instantiate without error") {
        array<f64> array{ buffer };
    }
    SECTION("should construct from a string without error") {
        array<f64> array{ test_name };
        REQUIRE(array.buf() == buffer);
    }
    SECTION("should correctly behave when multiple instances are created") {
        auto buffer_name = "smoketest/constructed_array";
        auto capacity    = array<f64>::default_capacity;

        auto maybe_buffer = memory::find(buffer_name);
        REQUIRE_FALSE(maybe_buffer);

        array<f64> one{ buffer_name };
        array<f64> two{ buffer_name };
        REQUIRE(one.capacity() == capacity);
        REQUIRE(one.buf() == two.buf());

        one.push_back(42.f);
        REQUIRE(two[0] == 42.f);

        array<f64> three{ buffer_name, (capacity * 2) };
        REQUIRE(three.capacity() == capacity * 2);
        REQUIRE(one.capacity() == capacity * 2);

        array<f64> four{ buffer_name, capacity };
        REQUIRE(four.capacity() == capacity * 2);
    }

    array<f64> array{ buffer };

    SECTION("should fill to capacity with push() safely") {
        for (u64 i : range(num_test_points)) {
            array.push((f64)i);
        }
        for (u64 i : range(num_test_points)) {
            REQUIRE(array[i] == (f64)i);
        }

        SECTION("and should not automatically resize") {
            REQUIRE(array.count()    == num_test_points);
            REQUIRE(array.capacity() == num_test_points);
        }
    }

    SECTION("should be safely growable") {
        for (u64 i : range(num_test_points)) {
            array.push((f64)i);
        }
        REQUIRE(array.capacity() == num_test_points);

        array.resize(num_test_points * 2);
        REQUIRE(array.capacity() == (num_test_points * 2));

        for (u64 i : range(num_test_points)) {
            REQUIRE(array[i] == (f64)i);
        }
    }

    SECTION("should automatically grow on") {
        for (u64 i : range(num_test_points)) {
            array.push((f64)i);
        }
        REQUIRE(array.count()    == num_test_points);
        REQUIRE(array.capacity() == num_test_points);

        SECTION("push") {
            array.push(42.f);
            REQUIRE(array.count()    == num_test_points+1);
            REQUIRE(array.capacity() >  num_test_points);
            f64 last = 0.f;
            u64 loops = 0;
            for (auto&& i : array) {
                loops++;
                last = i;
            }
            REQUIRE(loops == num_test_points+1);
            REQUIRE(last  == 42.f);
        }

        SECTION("consume") {
            f64* data = array.consume(1);
            *data = 42.f;
            REQUIRE(array.count()    == num_test_points+1);
            REQUIRE(array.capacity() >  num_test_points);
            f64 last = 0.f;
            u64 loops = 0;
            for (auto&& i : array) {
                loops++;
                last = i;
            }
            REQUIRE(loops == num_test_points+1);
            REQUIRE(last  == 42.f);
        }
    }

    SECTION("should truncate on shrink") {
        for (u64 i : range(num_test_points)) {
            array.push((f64)i);
        }
        REQUIRE(array.capacity() == num_test_points);

        array.resize(num_test_points/2);
        REQUIRE(array.capacity() == num_test_points/2);

        for (u64 i : range(num_test_points/2)) {
            REQUIRE(array[i] == (f64)i);
        }
    }

    SECTION("should correctly erase data") {
        for (u64 i : range(num_test_points)) {
            array.push((f64)i);
        }

        REQUIRE(array.count() == num_test_points);
        REQUIRE(array[0]                 == 0.f);
        REQUIRE(array[num_test_points-1] == num_test_points-1);

        SECTION("one at a time") {
            // We're trying to erase the (currently) second-to-last element,
            // s.t. when we access the that element next, it will give us the
            // value of the (currently) last element.
            array.erase(num_test_points-2);

            REQUIRE(array.count() == num_test_points-1);
            REQUIRE(array[0]      == 0.f);
            REQUIRE(array[num_test_points-2] == num_test_points-1);
        }

        SECTION("in a range") {
            // We're trying to erase all but the (currently) last element, s.t.
            // when we access the first element next, it will give us the value
            // of the (currently) last element.
            // Note this range is half-open, so we're trying to erase all but
            // the last element.
            array.erase(0, num_test_points-1);

            REQUIRE(array.capacity() == num_test_points);
            REQUIRE(array.count()    == 1);
            REQUIRE(array[0]         == num_test_points-1);
        }
    }

    SECTION("should be iterable") {
        for (u64 i : range(num_test_points)) {
            array.push((f64)i);
        }
        f64 n = 0.;
        u64 loops = 0;
        for (auto&& i : array) {
            INFO("loops : " << loops);
            REQUIRE(i == n);
            n++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }

    SECTION("should be usable when partially filled") {
        for (auto i : range(num_test_points/2)) {
            array.push((f64)i);
        }
        for (auto i : range(num_test_points/2)) {
            REQUIRE(array[i] == (f64)i);
        }
    }

    SECTION("should be iterable when partially filled") {
        for (auto i : range(num_test_points/2)) {
            array.push((f64)i);
        }
        u64 loops = 0;
        for (auto&& i : array ) {
            REQUIRE(array[i] == (f64)i);
            loops++;
        }
        REQUIRE(loops == num_test_points/2);
    }

    SECTION("should be iterable with") {
        SECTION("only one object added") {
            array.push(1.f);
            u64 loops = 0;
            for (auto&& i : array) {
                REQUIRE(i == 1.f);
                loops++;
            }
            REQUIRE(loops == 1);
        }
        SECTION("a small number of datapoints added") {
            array.push(1.f);
            array.push(2.f);
            u64 loops = 0;
            for (auto&& i : array) {
                loops++;
            }
            REQUIRE(loops == 2);
        }
    }

}

} /* namespace test_array */
