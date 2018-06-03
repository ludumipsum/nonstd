/** Stream Smoke Tests
 *  ==================
 *  TODO: Come up with a better name for ^.
 *  TODO: explain main concerns in testing this module.
 */
#include <nonstd/stream.h>
#include <testrunner/testrunner.h>

#include <nonstd/nonstd.h>
#include <memory/memory.h>


namespace test_stream {

using nonstd::range;
using nonstd::buffer;
using nonstd::stream;
using memory::find_stream;
using memory::allocate_stream;

TEST_CASE("Stream views", "[standalone][memory][stream]") {
    c_cstr test_name = "smoketest/stream";
    u64 num_test_points = 10;

    memory::init();

    SECTION("should instantiate with at least one slot") {
        auto size = stream<f64>::precompute_size(0);
        auto buffer = memory::allocate("smoketest/stream/zero-sized", size);
        stream<f64>::initialize_buffer(buffer);
        stream<f64> stream { buffer };

        REQUIRE(stream.capacity() == 1);
    }

    auto size = stream<f64>::precompute_size(num_test_points);
    auto buffer = memory::allocate(test_name, size);
    stream<f64>::initialize_buffer(buffer);

    SECTION("should instantiate without error") {
        stream<f64> stream{ buffer };
    }
    SECTION("should construct from a string without error") {
        stream<f64> stream{ test_name, num_test_points };
        REQUIRE(stream.buf() == buffer);
    }
    SECTION("should correctly behave when multiple instances are created") {
        auto buffer_name = "smoketest/constructed_array";
        auto capacity    = stream<f64>::default_capacity;

        auto maybe_buffer = memory::find(buffer_name);
        REQUIRE_FALSE(maybe_buffer);

        stream<f64> one{ buffer_name };
        stream<f64> two{ buffer_name };
        REQUIRE(one.capacity() == capacity);
        REQUIRE(one.buf() == two.buf());

        one.push(42.f);
        REQUIRE(two[0] == 42.f);

        // Streams currently cannot resize themselves. The below section relies
        // on unimplemented code.
#if 0
        stream<f64> three{ buffer_name, (capacity * 2) };
        REQUIRE(three.capacity() == capacity * 2);
        REQUIRE(one.capacity() == capacity * 2);

        stream<f64> four{ buffer_name, capacity };
        REQUIRE(four.capacity() == capacity * 2);
#endif
    }

    stream<f64> stream{ buffer };

    SECTION("should fill to capacity with push() safely") {
        for(u64 i : range(num_test_points)) {
            stream.push((f64)i);
        }
        for(u64 i : range(num_test_points)) {
            REQUIRE(stream[i] == (f64)i);
        }
        REQUIRE(stream.count() == num_test_points);
    }

    SECTION("should be iterable") {
        for(u64 i : range(num_test_points)) {
            stream.push((f64)i);
        }
        f64 n = 0.;
        u64 loops = 0;
        for(auto i : stream) {
            INFO("loops : " << loops);
            REQUIRE(i == n);
            n++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }

    SECTION("should be usable when partially filled") {
        for(auto i : range(num_test_points/2)) {
            stream.push((f64)i);
        }
        u64 loops = 0;
        for(auto i : range(num_test_points/2)) {
            REQUIRE(stream[i] == (f64)i);
        }
        REQUIRE(stream.count() == num_test_points/2);
    }

    SECTION("should be indexable when partially filled") {
        for(auto i : range(num_test_points/2)) {
            stream.push((f64)i);
        }
        u64 loops = 0;
        for(auto i : range(num_test_points/2)) {
            REQUIRE(stream[i] == (f64)i);
        }
        REQUIRE(stream.count() == num_test_points/2);
    }

    SECTION("should be iterable when partially filled") {
        for(auto i : range(num_test_points/2)) {
            stream.push((f64)i);
        }
        i64 n = 0;
        u64 loops = 0;
        for(auto i : stream ) {
            REQUIRE(i == n);
            n++; loops++;
        }
        REQUIRE(loops == (num_test_points/2));
    }

    SECTION("should be iterable with") {
        SECTION("only one object contained") {
            stream.push(1.f);
            i64 n = 1; // 1 b/c we 1-indexed our two vals.
            u64 loops = 0;
            for(auto i : stream) {
                CHECK(i == n);
                n++; loops++;
            }
            REQUIRE(loops == 1);
        }
        SECTION("small numbers of objects contained") {
            stream.push(1.f);
            stream.push(2.f);
            i64 n = 1; // 1 b/c we 1-indexed our two vals.
            u64 loops = 0;
            for(auto i : stream) {
                CHECK(i == n);
                n++; loops++;
            }
            REQUIRE(loops == 2);
        }
    }

    SECTION("should safely overfill all of the data") {
        for(auto i : range(num_test_points * 2)) {
            stream.push((f64)i);
        }
        f64 j = num_test_points;
        u64 loops = 0;
        for(auto i : stream) {
            REQUIRE(i == j);
            j++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }

    SECTION("should safely overfill part of the data") {
        for(auto i : range(num_test_points * 1.5)) {
            stream.push((f64)i);
        }
        f64 j = num_test_points/2;
        u64 loops = 0;
        for(auto i : stream) {
            REQUIRE(i == j);
            j++; loops++;
        }
        REQUIRE(loops == num_test_points);
    }
}

} /* namespace test_stream */
