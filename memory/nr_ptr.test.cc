#include <nonstd/memory/nr_ptr.h>
#include <testrunner/testrunner.h>

#include <nonstd/nonstd.h>
#include <nonstd/memory.h>
#include <platform/memory/memory.h>


namespace test_buffered_value {

using nonstd::buffer;
using nonstd::nr_ptr;
using memory::allocate;

TEST_CASE("nr_ptr API Demo", "[api][memory][nr_ptr]") {
    memory::init();

    /** Creating Named-Register Pointers
     *  --------------------------------
     *  nr_ptrs are pretty much exactly what they claim to be; pointers backed
     *  by a platform-side named register. Creation of an nr_ptr implies a
     *  memory allocation, which will be performed in the nr_ptr's constructor
     *  as an allocateOrFind operation.
     */
    SECTION("Creating nr_ptrs") {
        // In the simplest case, we just pass a buffer name and call it good.
        auto simple_ptr = nr_ptr<f32> { "test/nr_ptr/direct" };
        // You can also use direct construction syntax.
        nr_ptr<f32> simpler_ptr { "test/nr_ptr/direct" };

        // We can also manually allocate a buffer, then construct a nr_ptr
        // using that.
        auto raw_buffer = allocate("test/nr_ptr/manual", sizeof(f32));
        nr_ptr<f32> manual_ptr { raw_buffer };

        // If you really need to, you can default construct a nr_ptr without any
        // backing buffer, and set one later.
        nr_ptr<f32> empty_ptr;
        // empty_ptr.backing_buffer() will return a junk address at this
        // point. NB. We **can not** rely on it being `== nulltpr`
        empty_ptr.backing_buffer(raw_buffer);
        REQUIRE(empty_ptr.backing_buffer() == raw_buffer);
    }

    SECTION("Creating nr_ptrs with default arguments") {
        auto nr_forty_two = nr_ptr<f32> { "test/nr_ptr/42", 42.f };
        REQUIRE(*nr_forty_two == 42.f);

        auto nr_array = nr_ptr<i32[4]> { "test/nr_ptr/packaged_array",
                                         0, 1, 2, 3 };
        REQUIRE((*nr_array)[2] == 2);

        auto nr_string = nr_ptr<std::string> { "test/nr_ptr/will_this_work?",
                                               "ABCDE" };
        REQUIRE(*nr_string == std::string("ABCDE"));
    }

    SECTION("Using Stored Values") {
        nr_ptr<f32> float_a { "test/nr_ptr/float_a" };
        nr_ptr<f32> float_b { "test/nr_ptr/float_b" };

        *float_a = 42.f;
        *float_b = 42.f;

        REQUIRE(*float_a == 42.f);
        REQUIRE(*float_a == *float_b);

        f32& val_ref = *float_a;
        REQUIRE(val_ref == 42.f);

        val_ref = 6.f;
        REQUIRE(*float_a == 6.f);
    }

    SECTION("Comparing against ptrs") {
        nr_ptr<f32> float_a1 { "test/nr_ptr/float_a" };
        nr_ptr<f32> float_a2 { "test/nr_ptr/float_a" };
        nr_ptr<f32> float_b  { "test/nr_ptr/float_b" };

        REQUIRE(float_a1.backing_buffer() == float_a2.backing_buffer());
        REQUIRE(float_a1.backing_buffer() != float_b.backing_buffer());
        REQUIRE(float_a1.backing_buffer() != nullptr);

        REQUIRE(float_a1.get() == float_a2.get());
        REQUIRE(float_a1.get() != float_b.get());
        REQUIRE(float_a1.get() != nullptr);

        nr_ptr<f32> empty_float;

        REQUIRE(float_a1 == float_a2);
        REQUIRE(float_a1 != float_b);
        REQUIRE(float_a1 != nullptr);
        REQUIRE(empty_float == nullptr);
    }
}

} /* namespace test_buffered_value */
