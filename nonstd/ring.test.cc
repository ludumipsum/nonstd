/** Ring Tests
 *  ==========
 *  GOAL: Validate the contract of the ring buffer container, ring<T>.
 *
 *  METHODOLOGY:
 *   - Verify initialization & helpers
 *   - Test accessors over a simple builtin type
 *   - Test edge conditions around empty, just-filled, and rapidly-filled rings
 *   - Test over compound POD types
 */
#include <nonstd/ring.h>
#include <platform/testrunner/testrunner.h>

#include <nonstd/nonstd.h>
#include <platform/memory/memory.h>


namespace test_ring {

using nonstd::range;
using nonstd::buffer;
using nonstd::ring;
using memory::find_ring;
using memory::allocate_ring;

/* Compound POD datatype used to test non-builtin-type Rings */
struct CompoundType {
    union {
        struct { u16 a, b; };
        u32 word;
    };
};

/** API DEMONSTRATION
 *  =================
 *  Rings are a classic data structure useful in any scenario where you'd like
 *  to store a window-worth of data, discarding the oldest first when full.
 *  They are often used for scratch buffers, last-N-entries rolling statistics,
 *  temporary debugging histories, and so on.
 *
 *  Our rings are "containers" -- they operate on buffer pointers provided by
 *  the platform layer, giving you a typed view of the given buffer, treating it
 *  as a ring. Rings are cheap to create, since all the allocations and state
 * are stored in the underlying buffer.
 *
 *  The purpose of this test suite is to show you a couple basic usage
 *  examples, in a place that's very easy to find later if you get
 *  confused about how they are supposed to work.
 */

TEST_CASE("Ring API Demo", "[api][memory][ring]") {
    memory::init();

    /** Creating Rings
     *  --------------
     *  Rings provide a view over a buffer, so in order to get a ring, we'll
     *  first need a buffer object to provide storage. There are a few helpers
     *  which make allocating one of these easier, but take care -- this causes
     *  a memory allocation.
     *
     *  In general, if you can't decide which initializer to use, prefer the
     *  one highest up in this section (since they're ordered in terms
     *  of simplicity).
     */
    SECTION("Creating Rings") {
        // In the simplest case, we need to allocate a buffer
        auto created = allocate_ring<f32>("test_data", 5);
        // And in subsequent uses, we'll want to grab that buffer again
        auto maybe_ring = find_ring<f32>("test_data");
        // If we want convenience over precision, we can directly construct
        // Rings from from just a buffer name. If the named buffer has already
        // been allocated, it will be used. Otherwise, it will be allocated
        // using whatever defaults are necessary.
        ring<f32> simple_ring{ "test_data" };

        // You can also do the buffer initialization by hand. All views provide
        // a ::precompute_size static method to help with this, should you need
        // to do it.
        auto required_size = ring<f32>::precompute_size(5);
        auto manual_buffer = memory::allocate("manual_test", required_size);
        ring<f32>::initialize_buffer(manual_buffer);
        ring<f32> manual_creation{ manual_buffer };

        // Or the lookup by hand
        auto maybe_manual_buffer = memory::find("manual_test");
        if (!maybe_manual_buffer) { INFO("Error checking is good."); }
        ring<f32> manual_lookup{ *maybe_manual_buffer };
    }

    /** Inserting And Reading Data
     *  --------------------------
     *  ring<T> provides a pretty minimal ring-buffer interface -- you can
     *  insert elements, iterate over them oldest->newest, access by offset
     *  back into the ring, and wipe the whole thing.
     *
     *  For this demonstration, we'll be using a ring as a stats window.
     */
    SECTION("Inserting and Reading Data") {
        // We need a ring to use for these scenarios, so let's create one.
        // Suppose we're making a system to store the last minute worth of
        // data, sampled at one point per second. We'll need 60 points.
        auto stats = allocate_ring<f32>("stat_demo", 60);

        // Now let's pretend we have some external system we're sampling
        auto getSample = []() -> f32 { return 42; };

        // Now, let's sample half a minute's worth of data
        for (auto i : range(30)) {
            stats.push(getSample());
        }

        // And compute an average
        f32 avg = 0.f;
        for (auto& v : stats) {
            avg += v;
        }
        avg /= stats.count();

        // We can also look at a couple data points in particular
        auto newest = stats[59];
        // NB. the oldest, since we haven't written it, will be filled with
        //     zeroes (our default value).
        auto oldest = stats[0];
        // The edge of where we've written can be found at the midpoint
        auto frontier = stats[29];

        // We can even keep pushing data, overwriting the oldest first once
        // we get past the size of the ring.
        for (auto i : range(60)) {
            stats.push(getSample());
        }

        // We can access elements by negative offset too
        auto also_newest = stats[-1];
        // And offsets larger than the buffer
        auto also_oldest = stats[120];

        // Finally, if we decide we want to reset a ring back to empty data,
        // we can drop the contents.
        stats.drop();
    }

    /** Querying Metadata
     *  -----------------
     *  Rings expose a relatively limited amount of metadata by virtue of their
     *  simplicity. Mostly they're concerned with how many elements they hold,
     *  and how much memory that takes up.
     */
    SECTION("Querying Metadata") {
        ring<f32> ring { "metadata_demo", 10 };

        auto capacity = ring.capacity();
        auto size_in_bytes = ring.size();

        // Note in particular that element count is always capacity for rings
        auto elem_count = ring.count();
    }

}


/** FUNCTIONAL TESTS
 *  ================
 *  This section contains tests of the basic functionality.
 *
 *  All tests in this section should be written with BDD-stye sections such that
 *  they form completeish sentences when read sequentially. For example,
 *  "Optional types over nones should coerce to boolean correctly."
 */
TEST_CASE("Ring views", "[standalone][memory][ring]") {
    c_cstr test_name = "smoketest/ring";
    u64 num_test_points = 10;

    memory::init();
    auto size = ring<f64>::precompute_size(num_test_points);
    auto buffer = memory::allocate(test_name, size);
    ring<f64>::initialize_buffer(buffer);

    SECTION("should instantiate without error") {
        ring<f64> ring{ buffer };
    }
    SECTION("should construct from a string without error") {
        ring<f64> ring{ test_name };
        REQUIRE(ring.buf() == buffer);
    }
    SECTION("should correctly behave when multiple instances are created") {
        auto buffer_name = "smoketest/constructed_ring";
        auto capacity    = ring<f64>::default_capacity;

        auto maybe_buffer = memory::find(buffer_name);
        REQUIRE_FALSE(maybe_buffer);

        ring<f64> one{ buffer_name };
        ring<f64> two{ buffer_name };
        REQUIRE(one.capacity() == capacity);
        REQUIRE(one.buf() == two.buf());

        one.push(42.f);
        REQUIRE(two[-1] == 42.f);

        ring<f64> three{ buffer_name, (capacity * 2) };
        REQUIRE(three.capacity() == capacity * 2);
        REQUIRE(one.capacity() == capacity * 2);

        ring<f64> four{ buffer_name, capacity };
        REQUIRE(four.capacity() == capacity * 2);
    }

    ring<f64> ring{ buffer };

    SECTION("should fill to capacity with push() safely") {
        for(u64 i : range(num_test_points)) {
            ring.push((f64)i);
        }
        for(u64 i : range(num_test_points)) {
            REQUIRE(ring[i] == (f64)i);
        }
    }

    SECTION("should fill to capacity with the index operator safely") {
        for(u64 i : range(num_test_points)) {
            ring[i] = (f64)i;
        }
        for(u64 i : range(num_test_points)) {
            REQUIRE(ring[i] == (f64)i);
        }
    }

    SECTION("should be safely growable") {
        for(u64 i : range(num_test_points)) {
            ring.push((f64)i);
        }

        SECTION("using the (default) shift-left mode") {
            ring.resize(num_test_points * 2);
            REQUIRE(ring.capacity() == num_test_points*2);
            for(u64 i : range(num_test_points)) {
                REQUIRE(ring[i] == (f64)i);
            }
            // Empty elements should have been added to the back of the ring.
            for(u64 i : range(num_test_points)) {
                REQUIRE(ring[i+num_test_points] == 0);
            }
        }

        SECTION("using the shift-right mode") {
            ring.resize_shifting_right(num_test_points * 2);
            REQUIRE(ring.capacity() == num_test_points*2);
            // Empty elements should have been added to the front of the ring.
            for(u64 i : range(num_test_points)) {
                REQUIRE(ring[i] == 0);
            }
            for(u64 i : range(num_test_points)) {
                REQUIRE(ring[i+num_test_points] == (f64)i);
            }
        }

        SECTION("Using the after-dropping mode") {
            ring.resize_after_dropping(num_test_points * 2);
            REQUIRE(ring.capacity() == num_test_points*2);
            // All elements in the ring should now be empty.
            for(u64 i : range(num_test_points*2)) {
                REQUIRE(ring[i] == 0);
            }
        }
    }

    SECTION("should be safely shrinkable") {
        for(u64 i : range(num_test_points)) {
            ring.push((f64)i);
        }

        SECTION("using the (default) shift-left mode") {
            ring.resize(num_test_points/2);
            REQUIRE(ring.capacity() == num_test_points/2);
            // The 0th element should have remain unchanged.
            for(u64 i : range(num_test_points/2)) {
                REQUIRE(ring[i] == (f64)i);
            }
        }

        SECTION("using the shift-right mode") {
            ring.resize_shifting_right(num_test_points/2);
            REQUIRE(ring.capacity() == num_test_points/2);
            // The 0th element should have been the first to be dropped.
            for(u64 i : range(num_test_points/2)) {
                REQUIRE(ring[i] == i+num_test_points/2);
            }
        }

        SECTION("Using the after-dropping mode") {
            ring.resize_after_dropping(num_test_points/2);
            REQUIRE(ring.capacity() == num_test_points/2);
            // All elements in the ring should now be empty.
            for(u64 i : range(num_test_points/22)) {
                REQUIRE(ring[i] == 0);
            }
        }
    }

    SECTION("should truncate on shrink") {
        for(u64 i : range(num_test_points)) {
            ring.push((f64)i);
        }
        REQUIRE(ring.capacity() != num_test_points / 2);
        ring.resize(num_test_points / 2);
        REQUIRE(ring.capacity() == num_test_points / 2);
        for(u64 i : range(num_test_points / 2)) {
            REQUIRE(ring[i] == (f64)i);
        }
    }

    SECTION("should be iterable") {
        for(u64 i : range(num_test_points)) {
            ring.push((f64)i);
        }
        f64 n = 0.;
        u64 loops = 0;
        for(auto i : ring) {
            INFO("loops : " << loops);
            REQUIRE(i == n);
            n++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }

    SECTION("should be usable when partially filled") {
        for(auto i : range(num_test_points/2)) {
            ring.push((f64)i);
        }
        for(auto i : range(num_test_points/2)) {
            REQUIRE(ring[i] == 0.f);
        }
        for(auto i : range(num_test_points/2)) {
            REQUIRE(ring[i + (num_test_points/2)] == (f64)i);
        }
    }

    SECTION("should be iterable when partially filled") {
        for(auto i : range(num_test_points/2)) {
            ring.push((f64)i);
        }
        i64 n = -((i64)num_test_points/2);
        u64 loops = 0;
        for(auto i : ring ) {
            f64 expected = n2clamp(n, 0, (i64)num_test_points);
            REQUIRE(i == expected);
            n++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }

    SECTION("should be iterable with") {
        SECTION("only one object added") {
            ring.push(1.f);
            i64 n = (-(i64)num_test_points) + 1 + 1; // +1 b/c we 1-indexed our two vals.
            u64 loops = 0;
            for(auto i : ring) {
                f64 expected = n2clamp(n, 0, (i64)num_test_points);
                CHECK(i == expected);
                n++; loops++;
            }
            REQUIRE(loops == num_test_points);
        }
        SECTION("a small number of datapoints added") {
            ring.push(1.f);
            ring.push(2.f);
            i64 n = (-(i64)num_test_points) + 2 + 1; // +1 b/c we 1-indexed our two vals.
            u64 loops = 0;
            for(auto i : ring) {
                f64 expected = n2clamp(n, 0, (i64)num_test_points);
                CHECK(i == expected);
                n++; loops++;
            }
            REQUIRE(loops == num_test_points);
        }
    }

    SECTION("should safely overfill all of the data") {
        for(auto i : range(num_test_points * 2)) {
            ring.push((f64)i+10);
        }
        f64 j = 10 + num_test_points;
        u64 loops = 0;
        for(auto i : ring) {
            REQUIRE(i == j);
            j++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }

    SECTION("should safely overfill part of the data") {
        for(auto i : range(num_test_points * 1.5)) {
            ring.push((f64)i);
        }
        f64 j = num_test_points/2;
        u64 loops = 0;
        for(auto i : ring) {
            REQUIRE(i == j);
            j++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }

    SECTION("should be addressable with negative indices") {
        auto test_value = 42;
        ring[59] = test_value;
        REQUIRE(ring[-1] == test_value);
        REQUIRE(&ring[-1] == &ring[59]);
    }
}

TEST_CASE("Tiny ring views", "[platform][memory]") {
    c_cstr test_name = "smoketest/ring";

    SECTION("of size one") {
        memory::init();
        u64 num_test_points = 1;
        auto size = ring<f64>::precompute_size(num_test_points);
        auto buffer = memory::allocate(test_name, size);
        ring<f64>::initialize_buffer(buffer);
        ring<f64> ring{ buffer };

        SECTION("should insert without error") {
            ring.push(1.f);
            REQUIRE(ring[0] == 1.f);
        }

        SECTION("should overflow without error") {
            ring.push(1.f);
            REQUIRE(ring[0] == 1.f);
            ring.push(2.f);
            REQUIRE(ring[0] == 2.f);
            ring.push(3.f);
            REQUIRE(ring[0] == 3.f);
            ring.push(4.f);
            REQUIRE(ring[0] == 4.f);
        }

        SECTION("should be addressable with modular arithmetic") {
            ring.push(1.f);
            REQUIRE(ring[-2] == 1.f);
            REQUIRE(ring[-1] == 1.f);
            REQUIRE(ring[0]  == 1.f);
            REQUIRE(ring[1]  == 1.f);
            REQUIRE(ring[2]  == 1.f);
        }
    }


    SECTION("of size two") {
        memory::init();
        u64 num_test_points = 2;
        auto size = ring<f64>::precompute_size(num_test_points);
        auto buffer = memory::allocate(test_name, size);
        ring<f64>::initialize_buffer(buffer);
        ring<f64> ring{ buffer };

        SECTION("should insert without error") {
            ring.push(1.f);
            REQUIRE(ring[-1] == 1.f);
        }

        SECTION("should fill without error") {
            ring.push(1.f);
            REQUIRE(ring[-1] == 1.f);
            ring.push(2.f);
            REQUIRE(ring[-1] == 2.f);
            REQUIRE(ring[-2] == 1.f);
            REQUIRE(ring[0]  == 1.f);
            REQUIRE(ring[1]  == 2.f);
        }

        SECTION("should overflow without error") {
            ring.push(1.f);
            REQUIRE(ring[-1] == 1.f);
            ring.push(2.f);
            REQUIRE(ring[-1] == 2.f);
            REQUIRE(ring[-2] == 1.f);
            ring.push(3.f);
            REQUIRE(ring[-1] == 3.f);
            REQUIRE(ring[-2] == 2.f);
            REQUIRE(ring[-3] == 3.f);
            ring.push(4.f);
            REQUIRE(ring[-1] == 4.f);
            REQUIRE(ring[-2] == 3.f);
            REQUIRE(ring[-3] == 4.f);
            REQUIRE(ring[-4] == 3.f);
        }

        SECTION("should be addressable with modular arithmetic") {
            ring.push(1.f);
            ring.push(2.f);
            REQUIRE(ring[-3] == 2.f);
            REQUIRE(ring[-2] == 1.f);
            REQUIRE(ring[-1] == 2.f);
            REQUIRE(ring[0]  == 1.f);
            REQUIRE(ring[1]  == 2.f);
            REQUIRE(ring[2]  == 1.f);
            REQUIRE(ring[3]  == 2.f);
        }
    }
}

} /* namespace test_ring */
