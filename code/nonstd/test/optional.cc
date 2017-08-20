/* Optional Smoke Tests
 * =====================
 * GOAL: Validate the basic functionality of Optional<T>.
 *
 * METHODOLOGY:
 *  - Test all accessors and initializers
 *  - Perform those tests over a couple different templated types
 *  - Perform those tests on both POD and non-trivial types
 *  - Test accessors and initializers over reference types
 */

#include "nonstd/optional.h"

#include "testing/testing.h"


namespace test {
namespace optional {

/* Simple POD datatype used to test non-builtin-type Optionals */
struct PODType {
    u16 a;
    u16 b;
};

/* Compound POD datatype used to test non-builtin-type Optionals */
struct CompoundType {
    union {
        struct { u16 a, b; };
        u32 word;
    };
};

/* Compound non-POD datatype used to test optionals over funky c++ey classes */
class NonPODType {
public:
    u16 m_a, m_b;
    NonPODType(u16 a, u16 b) : m_a(a), m_b(b) {}
    u16& getA() { return m_a; }
    u16& getB() { return m_b; }
    u32& getWord() { return *((u32*) &m_a); }
};

inline bool operator == (NonPODType const & l,
                         NonPODType const & r) {
    return (l.m_a == r.m_a && l.m_b == r.m_b);
}

/* Compound non-POD, non-trivial datatype used to test Optionals over complex
 * c++ey classes. */
class NonTrivialType {
public:
    u16 m_a, m_b;
    // Without this volatile qualifier, GCC will skip nulling the variable when
    // the destructor is called. As such, we test dtors with volatile members.
    volatile bool has_been_destroyed;
    NonTrivialType(u16 a, u16 b) : m_a(a), m_b(b), has_been_destroyed(false) {}
    ~NonTrivialType() {
        m_a = 0; m_b = 0;
        has_been_destroyed = true;
    }
    u16& getA() { return m_a; }
    u16& getB() { return m_b; }
    u32& getWord() { return *((u32*) &m_a); }
};

inline bool operator == (NonTrivialType const & l,
                         NonTrivialType const & r) {
    return (l.m_a == r.m_a && l.m_b == r.m_b);
}



/** API DEMONSTRATION
 *  =================
 *  Optional types are simple containers wrapping a value or the
 *  absence of a value. Think of them like a nullable SQL column, or a
 *  Maybe from haskell -- they either contain a value of a particular
 *  type, or no value at all.
 *
 *  The purpose of this test suite is to show you a couple basic usage
 *  examples, in a place that's very easy to find later if you get
 *  confused about how they are supposed to work.
 */

TEST_CASE("Optionals API Demo", "[api][common]") {

    /** Creating Optionals
     *  ------------------
     *  There are a lot of ways to create Optionals, depending on what you're
     *  trying to express, and what your focus is. In general, if you can't
     *  decide which initializer to use, prefer the one highest up in this
     *  section (since they're ordered in terms of simplicity).
     */
    SECTION("Creating Optionals") {
        // You can create Non-Containing Optionals of any type with...
        // Assigned braced initializers,
        Optional<u16> empty = { };
        // Direct braced initializers (same as the above),
        Optional<i32> very_empty { };
        // The `none<>()` helper (though you still have to provide a type),
        auto          still_empty = none<c_cstr>();
        // Or the `n2_::nullopt` tag.
        Optional<u64> empty_by_tag = n2_::nullopt;

        // For Containing Optionals, you can do much the same...
        // Assigned with a braced initializer,
        Optional<u16> definitely_16 = { 16 };
        // Assign directly from a value,
        Optional<u64> definitely_64 = 64;
        // Constructing directly with a braced initializer,
        Optional<i32> definitely_4 { 4 };
        // Or using the `just()` helper.
        // You can either use `just()` as a non-templatized function (if the
        // compiler knows the type of Optional you're building) or as a
        // templatized function defining the type of Optional.
        Optional<i8> definitely_i8 = just(8);
        auto         still_def_i8  = just<i8>(8);

        // You can also use both copy and move semantics to construct or assign
        // Optionals from other Optionals,
        auto also_empty = empty;
        auto also_very_empty { very_empty };
        auto movingly_empty { Optional<ptr> { } };

        Optional<u16> also_16 { definitely_16 };
        auto          still_16  = also_16;
        Optional<u16> elided_16 = Optional<u16> { 16 };

        auto moved_16 { std::move(elided_16) };
        auto moved_farther = std::move(moved_16);

        // For more complex datatypes that, you have a couple more options.
        // (Note that all of the below work as both assignment initialization
        // and direct initialization.)
        // Double Braced initialization --- For direct initialization only, plz.
        // Note that this resolves as a braced initialization of the stored
        // type, passed into a braced initialization of an Optional, passed into
        // an initializing operator= (which will be elided).
        Optional<PODType> simple_pod = {{ 1, 2 }};
        // Explicit value assignment,
        Optional<CompoundType> explicit_compound = CompoundType{ 3, 4 };
        // In-place Construction via the `n2_::in_place` tag.
        // NOTE: An explicit constructor is required for this form;
        // Optional<PODType>      broken_hopes        = { n2_::in_place, 5, 6 };
        // Optional<CompoundType> shattered_dreams    = { n2_::in_place, 5, 6 };
        Optional<NonPODType>      in_place_nonpod     = { n2_::in_place, 5, 6 };
        Optional<NonTrivialType>  in_place_nontrivial = { n2_::in_place, 5, 6 };
        // In-place via the `.emplace` member method (post initialization).
        // NOTE: The same limitation that applies to the in_place form applies
        // to the `.emplace` method.
        Optional<NonTrivialType> not_empty_for_log = { };
                                 not_empty_for_log.emplace(7, 8);

        // The `just()` helpers work very well with complex types as well,
        // though explicit initialization...
        auto just_maybe_pod = just(PODType { 8, 16 });
        // ... And through in-place initialization (when the type has a matching
        // explicit constructor)
        auto just_in_place = just<NonTrivialType>(n2_::in_place, 42, 84);

        // Lastly, it's worth noting that Optionals are able to leverage their
        // value-type's implicit conversions.
        u16 a_u16 = 16;
        u32 a_u32 = 32;
        u64 a_u64 = 64;

        Optional<u32> optionally_u32 = a_u64;
        Optional<u64> optionally_u64 = Optional<u16>{ 16 };

        // And this applies even for very complicated types
        struct CoercionTest {
            CoercionTest(NonTrivialType const& ntt) { }
        };
        Optional<CoercionTest> complicated = NonTrivialType { 4, 16 };
        Optional<CoercionTest> very_complicated = just_in_place;
    }

    /** Getting Data Out
     *  ----------------
     *  Much like instantiation, there are a couple ways of getting data out
     *  of Optionals. When you access an optional, you should first check to
     *  see if it's holding data or not. Once you've done that, you can
     *  extract whatever it's containing and use it.
     *
     *  Prefer the options higher up in this list, as always.
     */
    SECTION("Getting Data Out") {
        // You can branch on an optional as though it's a boolean
        Optional<i32> has_value = { 10 };
        if (has_value) { INFO("This optional has a value!"); }
        // This applies for empty ones too
        Optional<i32> no_value = { };
        if (!no_value) { INFO("This optional has no value!"); }
        // You can also explicitly cast an optional to boolean
        bool has_a_value = (bool)has_value;
        REQUIRE(has_a_value);
        // If you need to, you can use an accessor to directly check.
        REQUIRE(has_value.hasValue());
        REQUIRE_FALSE(no_value.hasValue());

        // Having checked that it's got a value, you have a few ways to read it.
        if (has_value) {
            // You can use the * operator,
            auto & value = *has_value;
            // Or a functional accessor.
            auto & yeah_a_value = has_value.value();
            // The -> operator also works on compound types, but we're going to
            // wait on really demo'ing that until we get farther along.

            // Note that all of the above return references, so you should
            // (probably) assign the returned value to a reference, so you can
            // maintain const correctness, and manipulate that value in-place;
            Optional<u32> mutable_16 = 16;
            auto & mutable_val = *mutable_16;
            mutable_val += 1;
            REQUIRE(*mutable_16 == 17);

            Optional<const u32> const_16 = 16;
            auto & const_val = *const_16;
            /* const_val += 1; */ // NOOOOPE it's a const, you can't.
        }

        // For Optionals wrapping structs, you can use the -> operator to
        // directly read from (and write to) properties on the value,
        Optional<CompoundType> maybe_compound = {{ 5, 10 }};
        if (maybe_compound) {
            REQUIRE(maybe_compound->a == 5);
            REQUIRE(maybe_compound->b == 10);
        }

        // You can also skip the check, and request an Optional's Value, Or a
        // default. (Do you see what I did there?)
        auto maybe_val = has_value.valueOr(20); // `has_value` is containing
        auto maybe_dflt = no_value.valueOr(20); // `no_value` is non-containing
        REQUIRE(maybe_val  == 10);
        REQUIRE(maybe_dflt == 20);
    }

    /** Handling Optional References
     *  ----------------------------
     *  Optionals can also store references, if you want to be able to pass
     *  some object by reference, but still express the absence of data.
     *  This is particularly useful for container types which want to offer
     *  a lookup that accesses some data by reference, but which may
     *  not contain a value for all queries. See, for example, HashTable.
     */
    SECTION("Handling Optional References") {
        // You can create empty reference optionals with braced initializers
        Optional<i32&> maybe_not_optional = { };
        // And the same is true of reference optionals with values
        i32 value = 0;
        Optional<i32&> maybe_value = { value };

        // Wrapping references lets modifications made to the value stored in
        // the Optional effect the referenced value, and vice versa.
        value = 10;
        REQUIRE(*maybe_value == 10);
        *maybe_value = 20;
        REQUIRE(value == 20);

        // There is also a helper function for referential optionals, just_ref
        auto maybe_ref = just_ref(value);

        // And a helper for constant references, just_cref, if you need that
        auto maybe_const_ref = just_cref(value);

        // And they can both be explicitly typed
        Optional<i32&> explicit_ref = just_ref(value);
        Optional<i32 const&> explicit_const_ref = just_cref(value);
    }

    /** Re-seating Optionals
     *  --------------------
     *  Optionals can always be reseated, both value- and reference-wrapping.
     */
    SECTION("Re-seating Optionals") {
        // Value-wrapping Optionals are pretty easy to see, and act as you would
        // expect the values themselves to act.
        u32 value = 42;

        // This is just me showing off re-seating from non- to containing.
        Optional<u32> maybe { };
        maybe = value;

        // Modifying the wrapped value does not effect the original.
        *maybe += 1;
        REQUIRE(value  == 42);
        REQUIRE(*maybe == 43);

        // Nor does re-seating the Optional modify the original.
        u32 another_value = 84;
        maybe = another_value;

        REQUIRE(value  == 42);
        REQUIRE(*maybe == 84);

        // From there, the Optional continues to behave as expected.
        *maybe += 1;
        REQUIRE(another_value == 84);
        REQUIRE(*maybe        == 85);

        // References are a little more complicated, as modifying either the
        // original value or the stored value will effect the other.
        u32   tmp_a     = 42;
        u32 & value_ref = tmp_a;

        Optional<u32 &> maybe_ref { };
        maybe_ref = value_ref;

        // Direct modifications do go both ways;
        value_ref += 2;
        REQUIRE(value_ref  == 44);
        REQUIRE(*maybe_ref == 44);

        // Note that we're assigning _through the dereference operator_.
        *maybe_ref = 2;
        REQUIRE(value_ref  == 2);
        REQUIRE(*maybe_ref == 2);

        // Assigning _to the Optional_ re-seats it.
        u32   tmp_b             = 84;
        u32 & another_value_ref = tmp_b;

        maybe_ref = another_value_ref;
        REQUIRE(value_ref == 2);
        REQUIRE(another_value_ref == 84);
        REQUIRE(*maybe_ref        == 84);

        // From there, modifying the wrapped value behaves as modifying a ref.
        *maybe_ref += 2;
        REQUIRE(another_value_ref == 86);
        REQUIRE(*maybe_ref        == 86);
    }

    /** Un-seating Optionals
     *  --------------------
     *  Optionals can always be un-seated. Though... I'm not sure why this is
     *  important... But it's there!
     */
    SECTION("Un-seating Optionals") {
        Optional<u64> maybe = 42;
        Optional<u64> other = 42;
        REQUIRE(maybe);
        REQUIRE(other);

        maybe.reset();
        other = n2_::nullopt;
        REQUIRE_FALSE(maybe);
        REQUIRE_FALSE(other);
    }

    /** Non-Trivially-Destructible Objects Work
     *  ---------------------------------------
     *  When re-seating or un-seating Optionals that wrap objects that have a
     *  destructor, Optionals do the right thing. Not terribly complicated,
     *  but important none the less.
     */
    SECTION("Destroying wrapped non-trivially-destructible objects") {
        Optional<NonTrivialType> maybe = {{4, 6}};
        NonTrivialType & m_ref = *maybe;

        REQUIRE(maybe);
        REQUIRE(maybe->m_a == 4); REQUIRE(maybe->m_b == 6);

        // Note; The destructor of the `NonTrivialType` just nulls member
        // variables. Those should be accessible until the end of this frame.
        maybe.reset();

        REQUIRE_FALSE(maybe);
        REQUIRE(m_ref.has_been_destroyed);
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

TEST_CASE("Optional types", "[optional]") {
    SECTION("over nones") {
        Optional<u64> maybe_value = { };
        Optional<u64> none_value = none<u64>();
        Optional<u64 &> maybe_ref = { };
        Optional<u64 &> none_ref = none<u64 &>();

        SECTION("should coerce to boolean correctly") {
            REQUIRE_FALSE(maybe_value);
            REQUIRE_FALSE(none_value);
            if (maybe_value && none_value) {
                REQUIRE(false);
            }
            REQUIRE_FALSE(maybe_ref);
            REQUIRE_FALSE(none_ref);
            if (maybe_ref && none_ref) {
                REQUIRE(false);
            }
        }

        SECTION("should be equivalent under bool cast and coercion") {
            bool is_value      = (bool)maybe_value;
            bool is_value_none = (bool)none_value;
            bool is_ref      = (bool)maybe_ref;
            bool is_ref_none = (bool)none_ref;
            if (maybe_value || none_value || is_value || is_value_none) {
                REQUIRE(false);
            }
            if (maybe_ref || none_ref || is_ref || is_ref_none) {
                REQUIRE(false);
            }
        }

        SECTION("should correctly return default from valueOr") {
            REQUIRE(maybe_value.valueOr(10) == 10);
            REQUIRE(maybe_value.valueOr(20) == 20);
            REQUIRE(none_value.valueOr(10) == 10);
            REQUIRE(none_value.valueOr(20) == 20);
            REQUIRE(maybe_ref.valueOr(10) == 10);
            REQUIRE(maybe_ref.valueOr(20) == 20);
            REQUIRE(none_ref.valueOr(10) == 10);
            REQUIRE(none_ref.valueOr(20) == 20);
        }

        SECTION("should stay non-containing through copies") {
            /* This test was added -- and should be retained as a regression
             * test -- in response to an MSVC-only memory bug. The root issue
             * was that the _Optional_ValueBase's (special) Move Constructor
             * was trying to copy uninitialized data out of an empty `other`.
             * For some reason that code path was never used by clang, and the
             * below was only able to reproduce the issue in MSVC. */
            Optional<void*> no_value = { };
            REQUIRE_FALSE(no_value);

            Optional<void*> still_no_value = no_value;
            REQUIRE_FALSE(no_value);

            auto nonemaker = []() -> Optional<void*> {
                return Optional<void*> { };
            };
            REQUIRE_FALSE(nonemaker());

            Optional<void*> (*nonemaker_ptr)() = nonemaker;
            REQUIRE_FALSE(nonemaker_ptr());

            /* This was the failing test on MSVC. The theory here is that the
             * specific level of indirect met -- wrapping `nonemaker` in a
             * function pointer, calling that fptr from a wrapper function, and
             * immediately returning the results -- was preventing MSVC from
             * eliding the move on the Optional constructed in `nonemaker`. In
             * Clang, only one Optional was constructed in the whole chain. In
             * MSVC, a second Optional would be constructed in `noneforwarder`
             * using what was returned by `nonemaker_ptr`. The (eventually)
             * lead us to find the bug in the _Optional_ValueBase's (special)
             * Move Constructor. */
            auto noneforwarder = [&]() -> Optional<void*> {
                return nonemaker_ptr();
            };
            REQUIRE_FALSE(noneforwarder());
        }
    }

    SECTION("over built-in types") {
        u64 initial_value = 42;
        auto maybe_qword = just(initial_value);
        auto maybe_not_qword = none<u64>();

        SECTION("should be constructible in all the usual ways") {
            auto value = initial_value;
            Optional<u64> maybe = { value };
            Optional<u64> by_value = value;
            auto implicit = just<decltype(value)>(value);
            auto lazy = just(value);

            REQUIRE(HAS_SAME_TYPE(maybe, by_value));
            REQUIRE(HAS_SAME_TYPE(maybe, implicit));
            REQUIRE(HAS_SAME_TYPE(maybe, lazy));
        }

        SECTION("should be sensibly coercible to boolean") {
            REQUIRE(maybe_qword);
            REQUIRE_FALSE(maybe_not_qword);
        }

        SECTION("should allow access to the contained value") {
            REQUIRE(*maybe_qword        == initial_value);
            REQUIRE(maybe_qword.value() == initial_value);
        }

        SECTION("should return their value from valueOr") {
            REQUIRE(maybe_qword.valueOr(0) == initial_value);
            REQUIRE(maybe_not_qword.valueOr(0) == 0);
        }

        SECTION("shouldn't do anything weird with rvalues") {
            REQUIRE(*maybe_qword + 1 == 1 + initial_value);
            REQUIRE(initial_value + 1 == 1 + *maybe_qword);
        }
    }

    SECTION("over constant references") {
        const u64    value = 42;
        const auto & vref = value;

        SECTION("should preserve constness in construction") {
            Optional<u64 const &> maybe = { vref };
            auto implicit = just<decltype(vref)>(vref);
            auto crefsafe = just_cref(vref);

            REQUIRE(HAS_SAME_TYPE(maybe, implicit));
            REQUIRE(HAS_SAME_TYPE(maybe, crefsafe));
        }

        Optional<u64 const &> maybe = { vref };

        SECTION("should preserve constness when accessed") {
            REQUIRE(HAS_SAME_TYPE(*maybe, vref));

            // Remember, `auto` uses `typeof` semantics, and so does not
            // implicitly deduce when it's supposed to be a reference.
            auto & maybe_vref = *maybe;
            auto   maybe_value = *maybe;
            REQUIRE(IS_SAME_TYPE(decltype(maybe_vref), u64 const &));
            REQUIRE(IS_SAME_TYPE(decltype(maybe_value), u64));
        }
    }

    SECTION("over references") {
        u64 initial_value = 42;
        u64 value = initial_value;

        SECTION("should handle references gracefully in construction") {
            auto & vref = value;
            Optional<u64&> maybe  = { vref };
            Optional<u64&> direct = { value };
            auto implicit = just<decltype(vref)>(vref);
            auto refsafe  = just_ref(vref);

            REQUIRE(HAS_SAME_TYPE(maybe, direct));
            REQUIRE(HAS_SAME_TYPE(maybe, implicit));
            REQUIRE(HAS_SAME_TYPE(maybe, refsafe));
        }

        auto maybe = just_ref(value);

        SECTION("should allow access to the referenced value") {
            REQUIRE(*maybe == value);
            REQUIRE(*maybe == initial_value);
        }

        SECTION("should provide address transparency") {
            REQUIRE(&*maybe == &value);
        }

        SECTION("should allow modification by reference") {
            auto new_value = initial_value + 12;
            *maybe = new_value;
            REQUIRE(*maybe == new_value);
            REQUIRE(value == new_value);
            REQUIRE(value != initial_value);
        }
    }

    SECTION("over POD compound types") {
        CompoundType value = { 1, 2 };

        SECTION("should allow construction and access by value") {
            auto maybe = just(value);
            REQUIRE(maybe->a == 1);
            REQUIRE(maybe->b == 2);
            REQUIRE(&*maybe != &value);
        }

        SECTION("should allow construction and access by reference") {
            auto maybe = just_ref(value);
            REQUIRE(maybe->a == 1);
            REQUIRE(maybe->b == 2);
            REQUIRE(&*maybe == &value);
        }
    }

    SECTION("over non-trivially-constructible compound types") {
        NonPODType value(1, 2);

        SECTION("should allow construction and access by value") {
            auto maybe = just(value);
            REQUIRE(maybe);
            REQUIRE(maybe->m_a == 1);
            REQUIRE(maybe->m_b == 2);
            REQUIRE(maybe->getA() == 1);
            REQUIRE(maybe->getB() == 2);

            REQUIRE(&*maybe != &value);
            maybe->m_a = 15;
            REQUIRE(maybe->getA() != value.m_a);
        }

        SECTION("should allow construction and access by reference") {
            auto maybe = just_ref(value);
            REQUIRE(maybe);
            REQUIRE(maybe->m_a == 1);
            REQUIRE(maybe->m_b == 2);
            REQUIRE(maybe->getA() == 1);
            REQUIRE(maybe->getB() == 2);

            REQUIRE(&*maybe == &value);
            maybe->m_a = 15;
            REQUIRE(maybe->getA() == value.m_a);
        }
    }

    SECTION("over pointers") {
        SECTION("pointing to built-in types") {
            u64 initial_value = 42;
            u64 value = initial_value;
            u64 * value_addr = &value;

            SECTION("should be constructible as per usual") {
                auto * vptr = &value;
                Optional<u64 *> maybe  = { vptr };
                Optional<u64 *> direct = { &value };
                auto implicit = just<decltype(vptr)>(vptr);
                auto lazy     = just(vptr);

                REQUIRE(HAS_SAME_TYPE(maybe, direct));
                REQUIRE(HAS_SAME_TYPE(maybe, implicit));
                REQUIRE(HAS_SAME_TYPE(maybe, lazy));
            }

            auto maybe = just(&value);

            SECTION("should allow access to the referenced value") {
                REQUIRE(*maybe  == value_addr);
                REQUIRE(**maybe == value);
            }

            SECTION("should allow modification by address") {
                auto new_value = initial_value + 12;
                **maybe = new_value;
                REQUIRE(*maybe == value_addr);
                REQUIRE(**maybe == new_value);
                REQUIRE(value == new_value);
                REQUIRE(value != initial_value);
            }

            SECTION("Should un-seat and re-seat correctly") {
                auto new_value = initial_value + 12;
                auto new_vptr  = &new_value;

                maybe = new_vptr;
                REQUIRE(*maybe == new_vptr);
                REQUIRE(**maybe == new_value);

                maybe.reset();
                REQUIRE_FALSE(maybe);
                REQUIRE(new_value == initial_value + 12);
            }
        }
    }
}

} /* namespace optional */
} /* namespace test */
