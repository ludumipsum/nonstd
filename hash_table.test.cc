/** HashTable Smoke Tests
 *  =====================
 *  TODO: Come up with a better name for ^.
 *  TODO: explain main concerns in testing this module.
 */
#include <nonstd/hash_table.h>
#include <testrunner/testrunner.h>

#include <nonstd/nonstd.h>
#include <platform/memory/memory.h>


namespace test_hashtable {

using nonstd::range;
using nonstd::buffer;
using nonstd::hash_table;


TEST_CASE("HashTable views", "[standalone][memory][hashtable]") {
    c_cstr test_name = "smoketest/hashtable";
    u64 num_test_points = 32;

    memory::init();

    auto size = hash_table<u32,u32>::precompute_size(num_test_points);
    auto buffer = memory::allocate(test_name, size);
    hash_table<u32,u32>::initialize_buffer(buffer);

    SECTION("should instantiate without error") {
        hash_table<u32,u32> table{ buffer };
        REQUIRE(table.capacity() == num_test_points);
    }
    SECTION("should construct from a string without error") {
        hash_table<u32,u32> hashtable{ test_name };
        REQUIRE(hashtable.buf() == buffer);
    }
    SECTION("should correctly behave when multiple instances are created") {
        auto buffer_name = "smoketest/constructed_hashtable";
        auto capacity    = hash_table<u32,u32>::default_capacity;
        // auto size        = hash_table<u32,u32>::precompute_size(capacity);

        auto maybe_buffer = memory::find(buffer_name);
        REQUIRE_FALSE(maybe_buffer);

        hash_table<u32,u32> one{ buffer_name };
        hash_table<u32,u32> two{ buffer_name };
        REQUIRE(one.capacity() == capacity);
        REQUIRE(one.buf() == two.buf());

        one.set(1, 42);
        REQUIRE(two.get(1) == 42);

        hash_table<u32,u32> three{ buffer_name, (capacity * 2) };
        REQUIRE(three.capacity() == capacity * 2);
        REQUIRE(one.capacity() == capacity * 2);

        hash_table<u32,u32> four{ buffer_name, capacity };
        REQUIRE(four.capacity() == capacity * 2);
    }

    hash_table<u32,u32> table{ buffer };

    SECTION("should successfully add the first k/v pair") {
        u64 test_key = 7,
            test_value = 42;
        REQUIRE(table.count() == 0);
        table.set(test_key, test_value);
        REQUIRE(table.count() == 1);
    }

    SECTION("should return the correct value on lookup") {
        u64 test_key = 7,
            test_value = 42;
        REQUIRE(table.count() == 0);

        table.set(test_key, test_value);
        auto maybe_value = table.get(test_key);

        REQUIRE(maybe_value);
        REQUIRE(*maybe_value == test_value);
        REQUIRE(table.count() == 1);
    }

    SECTION("should preserve data through resizes") {
        u64 test_key = 7,
            test_value = 42,
            resize_target = (num_test_points * 2);
        REQUIRE(table.count() == 0);
        REQUIRE(table.capacity() == num_test_points);

        table.set(test_key, test_value);
        REQUIRE(table.count() == 1);

        table.resize();
        REQUIRE(table.count() == 1);
        REQUIRE(table.capacity() == resize_target);

        REQUIRE(*table.get(test_key) == test_value);
        REQUIRE(table.count() == 1);
    }

    SECTION("should permit deleting contained data") {
        u64 test_key = 7,
            test_value = 42;
        REQUIRE(table.count() == 0);

        table.set(test_key, test_value);
        REQUIRE(table.count() == 1);

        table.erase(test_key);
        REQUIRE(table.count() == 0);

        auto maybe_get = table.get(test_key);
        REQUIRE((bool)maybe_get == false);
    }

    SECTION("should handle fantastically large keys") {
        for(auto i : range(num_test_points)) {
            i = (i * 0xfafafafa) % UINT32_MAX;
            table.set(i, i);
            REQUIRE(*table.get(i) == i);
        }
    }

    SECTION("should handle collisions") {
        SECTION("on repeated sets") {
            for(auto i : range(1, 1000)) {
                i = i % num_test_points;
                table.set(i, i);
                REQUIRE(*table.get(i) == i);
            }
        }
    }
}

TEST_CASE("HashTable iterators", "[standalone][memory][hashtable]") {
    c_cstr test_name = "smoketest/hashtable_iterators";
    u64 num_test_points = 32;

    memory::init();

    auto size = hash_table<u32,u32>::precompute_size(num_test_points);
    auto buffer = memory::allocate(test_name, size);
    hash_table<u32,u32>::initialize_buffer(buffer);
    hash_table<u32,u32> table{ buffer };

    SECTION("over keys") {
        SECTION("should yield all keys") {
            for (auto&& i : range(num_test_points)) {
                table.set(i, i);
            }
            int loops = 0;
            // NB. Keys are not guaranteed to be ordered, so test vs value.
            for (auto&& i : table.keys()) {
                REQUIRE(*(table.get(i)) == i);
                loops++;
            }
            REQUIRE(loops == num_test_points);
        }
    }

    SECTION("over values") {
        SECTION("should yield all values") {
            for (auto&& i : range(num_test_points)) {
                table.set(i, i);
            }
            int loops = 0;
            // NB. Values are not guaranteed to be ordered, so test vs key.
            for (auto&& i : table.values()) {
                REQUIRE(*(table.get(i)) == i);
                loops++;
            }
            REQUIRE(loops == num_test_points);
        }
        SECTION("should be transparent") {
            for (auto&& i : range(num_test_points)) {
                table.set(i, i * 2);
            }
            int j = 0;
            for (auto&& i : table.values()) {
                i = j;
                j++;
            }
            REQUIRE(j == num_test_points);
            j = 0;
            for (auto&& i : table.values()) {
                REQUIRE(i == j);
                j++;
            }
            REQUIRE(j == num_test_points);
        }
    }

    SECTION("over items") {
        SECTION("should yield all k/v pairs") {
            for (auto&& i : range(num_test_points)) {
                table.set(i, i * 2);
            }
            int loops = 0;
            for (auto&& i : table.items()) {
                REQUIRE(i.key * 2 == i.value);
                loops++;
            }
            REQUIRE(loops == num_test_points);
        }
        SECTION("should work for small tables") {
            for (auto&& i : range(4)) {
                table.set(i, i * 2);
            }
            int loops = 0;
            for (auto&& i : table.items()) {
                REQUIRE(i.key * 2 == i.value);
                loops++;
            }
            REQUIRE(loops == 4);
        }

        SECTION("should be transparent") {
            for (auto&& i : range(num_test_points)) {
                table.set(i, 0);
            }
            int j = 0;
            for (auto&& i : table.items()) {
                REQUIRE(i.value == 0);
                i.value = j;
                REQUIRE(*table.get(i.key) == j);
                j++;
            }
            REQUIRE(j == num_test_points);
        }
    }

    SECTION("over cells") {
        SECTION("should yield all cells") {
            for (auto&& i : range(num_test_points/2)) {
                table.set(i, i * 2);
            }
            u32 loops = 0;
            for (auto&& i : table.cells()) {
                loops++;
            }
            REQUIRE(loops == table.total_capacity());
        }

        SECTION("should be transparent") {
            for (auto&& i : range(num_test_points)) {
                table.set(i, 0);
            }
            u32 j = 0;
            for (auto&& cell : table.cells()) {
                REQUIRE(cell.value == 0);
                if (cell.is_in_use()) {
                    cell.value = j;
                    REQUIRE(*table.get(cell.key) == j);
                }
                j++;
            }
        }
    }
}

// TODO(bearcage): Test new_hashtable

} /* namespace test_hashtable */
