/* Optional Smoke Tests
 * =====================
 * GOAL: Validate the basic functionality of `optional<T>`.
 *
 * METHODOLOGY:
 *  - Test all accessors and initializers
 *  - Perform those tests over a couple different templated types
 *  - Perform those tests on both POD and non-trivial types
 *  - Test accessors and initializers over reference types
 *
 *
 * TODO:
 * - test the except guarantees of §23.6.3.3.4.
 * - test reference to constant pointer type.
 * - Mirror the static asserts present in test/optional_storage.cc (to insure
 *   triviality / constexpr-ness propagates through to `optional<T>`)
 */

#include <nonstd/optional.h>
#include <testrunner/testrunner.h>

#include <nonstd/utility_ext.h>
#include <nonstd/type_traits_ext.h>

#include <type_traits>
#include <utility>


namespace nonstd_test {
namespace optional {

using nonstd::in_place;
using nonstd::nullopt;
using nonstd::optional;
using nonstd::just;
using nonstd::just_ref;
using nonstd::just_cref;
using nonstd::none;

using std::is_pod_v;
using std::is_same_v;
using std::is_trivially_copy_constructible_v;
using std::is_trivially_move_constructible_v;
using std::is_trivially_destructible_v;


/* Simple POD datatype used to test non-builtin-type optionals */
struct PODType {
    u16 a;
    u16 b;
};
static_assert(is_pod_v<PODType>);

/* Compound POD datatype used to test non-builtin-type optionals */
struct CompoundType {
    union {
        struct { u16 a, b; };
        u32 word;
    };
};
static_assert(is_trivially_copy_constructible_v<CompoundType>);
static_assert(is_trivially_move_constructible_v<CompoundType>);
static_assert(is_trivially_destructible_v<CompoundType>);

/* Compound non-POD datatype used to test optionals over funky c++ey classes */
class NonPODType {
public:
    u16 m_a, m_b;
    NonPODType(u16 a, u16 b)
        : m_a(a)
        , m_b(b)
    { }
    u16& getA() { return m_a; }
    u16& getB() { return m_b; }
    u32& getWord() { return *((u32*) &m_a); }
};

inline bool operator == (NonPODType const & l,
                         NonPODType const & r) {
    return (l.m_a == r.m_a && l.m_b == r.m_b);
}

/* Compound non-POD, non-trivial datatype used to test optionals over complex
 * c++ey classes. */
class NonTrivialType {
public:
    u16 m_a, m_b;
    // Without this volatile qualifier, GCC will skip nulling the variable when
    // the destructor is called. As such, we test dtors with volatile members.
    volatile bool has_been_destroyed;
    NonTrivialType(u16 a, u16 b)
        : m_a(a)
        , m_b(b)
        , has_been_destroyed(false)
    { }
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


/* A class that has an optional as a member variable. */
struct OptionalContainer {
    optional<size_t> maybe;

    OptionalContainer()
        : maybe ( nonstd::nullopt )
    { }
    OptionalContainer(size_t val)
        : maybe ( val )
    { }
    OptionalContainer(optional<size_t> opt)
        : maybe ( opt )
    { }

    void assign(size_t v) {
        maybe = v;
    }
};



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

TEST_CASE("Optionals API Demo", "[nonstd][api][optionals]") {

    /** Creating Optionals
     *  ------------------
     *  There are a lot of ways to create optionals, depending on what you're
     *  trying to express, and what your focus is. In general, if you can't
     *  decide which initializer to use, prefer the one highest up in this
     *  section (since they're ordered in terms of simplicity).
     */
    SECTION("Creating Optionals") {
        // You can create non-containing optionals of any type with...
        // Assigned braced initializers,
        optional<u16> empty = { };
        // Direct braced initializers (same as the above),
        optional<i32> very_empty { };
        // The `none<>()` helper (though you still have to provide a type),
        auto          still_empty = none<c_cstr>();
        // Or the `nonstd::nullopt` tag.
        optional<u64> empty_by_tag = nullopt;

        // For containing optionals, you can do much the same...
        // Assigned with a braced initializer,
        optional<u16> definitely_16 = { 16 };
        // Assign directly from a value,
        optional<u64> definitely_64 = 64;
        // Constructing directly with a braced initializer,
        optional<i32> definitely_4 { 4 };
        // Or using the `just()` helper.
        // You can either use `just()` as a non-templatized function (if the
        // compiler knows the type of optional you're building) or as a
        // templatized function defining the type of optional.
        optional<i8> definitely_i8 = just(8);
        auto         still_def_i8  = just<i8>(8);
        auto         implicitly_i8 = just(static_cast<i8>(8));

        // You can also use both copy and move semantics to construct or assign
        // optionals from other optionals,
        auto also_empty = empty;
        auto also_very_empty { very_empty };
        auto elidingly_empty { optional<ptr> { } };

        optional<u16> also_16 { definitely_16 };
        auto          still_16  = also_16;
        optional<u16> elided_16 = optional<u16> { 16 };

        auto moved_16 { std::move(elided_16) };
        auto moved_farther = std::move(moved_16);

        // For more complex datatypes that, you have a couple more options.
        // Double Braced initialization --- For direct initialization only, plz.
        // Note that this resolves as a braced initialization of the stored
        // type, passed into a braced initialization of an optional, passed into
        // an initializing operator= (which will be elided).
        optional<PODType> simple_pod = {{ 1, 2 }};
        // Explicit value assignment,
        optional<CompoundType> explicit_compound = CompoundType{ 3, 4 };
        // In-place Construction via the `nonstd::in_place` tag.
        // NOTE: An explicit constructor of the wrapped type is required for
        // this form;
        // optional<PODType>      broken_hopes        { in_place, 5, 6 };
        // optional<CompoundType> shattered_dreams    { in_place, 5, 6 };
        optional<NonPODType>      in_place_nonpod     { in_place, 5, 6 };
        optional<NonTrivialType>  in_place_nontrivial { in_place, 5, 6 };
        // In-place via the `.emplace` member method (post initialization).
        // NOTE: The same limitation that applies to the in_place form applies
        // to the `.emplace` method.
        optional<NonTrivialType> not_empty_for_log = { };
                                 not_empty_for_log.emplace(7, 8);

        // The `just()` helpers work very well with complex types as well,
        // though explicit initialization...
        auto just_maybe_pod = just(PODType { 8, 16 });
        // ... And through in-place initialization (when the type has a matching
        // explicit constructor)
        auto just_in_place = just<NonTrivialType>(in_place, 42, 84);

        // Lastly, it's worth noting that optionals are able to leverage their
        // value-type's implicit conversions.
        u16 a_u16 = 16;
        u32 a_u32 = 32;
        u64 a_u64 = 64;

        optional<u32> optionally_u32 = a_u64;
        optional<u64> optionally_u64 = optional<u16>{ 16 };

        // And this applies even for very complicated types
        struct CoercionTest {
            CoercionTest(NonTrivialType const& ntt) { }
        };
        optional<CoercionTest> complicated = NonTrivialType { 4, 16 };
        optional<CoercionTest> very_complicated = just_in_place;
    }

    /** Getting Data Out
     *  ----------------
     *  Much like instantiation, there are a couple ways of getting data out
     *  of optionals. When you access an optional, you should first check to
     *  see if it's holding data or not. Once you've done that, you can
     *  extract whatever it's containing and use it.
     *
     *  Prefer the options higher up in this list, as always.
     */
    SECTION("Getting Data Out") {
        // You can branch on an optional as though it's a boolean
        optional<i32> has_value = { 10 };
        if (has_value) { INFO("This optional has a value!"); }
        // This applies for empty ones too
        optional<i32> no_value = { };
        if (!no_value) { INFO("This optional has no value!"); }
        // You can also explicitly cast an optional to boolean
        bool has_a_value = (bool)has_value;
        REQUIRE(has_a_value);
        // If you need to, you can use an accessor to directly check.
        REQUIRE(has_value.has_value());
        REQUIRE_FALSE(no_value.has_value());

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
            optional<u32> mutable_16 = 16;
            auto & mutable_val = *mutable_16;
            mutable_val += 1;
            REQUIRE(*mutable_16 == 17);

            optional<const u32> const_16 = 16;
            auto & const_val = *const_16;
            /* const_val += 1; */ // NOOOOPE it's a const, you can't.
        }

        // For optionals wrapping structs, you can use the -> operator to
        // directly read from (and write to) properties on the value,
        optional<CompoundType> maybe_compound = {{ 5, 10 }};
        if (maybe_compound) {
            REQUIRE(maybe_compound->a == 5);
            REQUIRE(maybe_compound->b == 10);
        }

        // You can also skip the check, and request an optional's Value, Or a
        // default. (Do you see what I did there?)
        auto maybe_val = has_value.value_or(20); // `has_value` is containing
        auto maybe_dflt = no_value.value_or(20); // `no_value` is non-containing
        REQUIRE(maybe_val  == 10);
        REQUIRE(maybe_dflt == 20);
    }

    /** Handling optional References
     *  ----------------------------
     *  optionals can also store references, if you want to be able to pass
     *  some object by reference, but still express the absence of data.
     *  This is particularly useful for container types which want to offer
     *  a lookup that accesses some data by reference, but which may
     *  not contain a value for all queries. See, for example, HashTable.
     */
    SECTION("Handling Optional References") {
        // You can create empty reference optionals with braced initializers
        optional<i32&> maybe_not_optional = { };
        // And the same is true of reference optionals with values
        i32 value = 0;
        optional<i32&> maybe_value = { value };

        // Wrapping references lets modifications made to the value stored in
        // the optional effect the referenced value, and vice versa.
        value = 10;
        REQUIRE(*maybe_value == 10);
        *maybe_value = 20;
        REQUIRE(value == 20);

        // There is also a helper function for referential optionals, just_ref
        auto maybe_ref = just_ref(value);

        // And a helper for constant references, just_cref, if you need that
        auto maybe_const_ref = just_cref(value);

        // And they can both be explicitly typed
        optional<i32&> explicit_ref = just_ref(value);
        optional<i32 const&> explicit_const_ref = just_cref(value);
    }

    /** Re-seating Optionals
     *  --------------------
     *  Optionals can always be reseated, both value- and reference-wrapping.
     */
    SECTION("Re-seating Optionals") {
        // Value-wrapping optionals are pretty easy to see, and act as you would
        // expect the values themselves to act.
        u32 value = 42;

        // This is just me showing off re-seating from non- to containing.
        optional<u32> maybe { };
        maybe = value;

        // Modifying the wrapped value does not effect the original.
        *maybe += 1;
        REQUIRE(value  == 42);
        REQUIRE(*maybe == 43);

        // Nor does re-seating the optional modify the original.
        u32 another_value = 84;
        maybe = another_value;

        REQUIRE(value  == 42);
        REQUIRE(*maybe == 84);

        // From there, the optional continues to behave as expected.
        *maybe += 1;
        REQUIRE(another_value == 84);
        REQUIRE(*maybe        == 85);

        // References are a little more complicated, as modifying either the
        // original value or the stored value will effect the other.
        u32   tmp_a     = 42;
        u32 & value_ref = tmp_a;

        optional<u32 &> maybe_ref { };
        maybe_ref = value_ref;

        // Direct modifications do go both ways;
        value_ref += 2;
        REQUIRE(value_ref  == 44);
        REQUIRE(*maybe_ref == 44);

        // Note that we're assigning _through the dereference operator_.
        *maybe_ref = 2;
        REQUIRE(value_ref  == 2);
        REQUIRE(*maybe_ref == 2);

        // Assigning _to the optional_ re-seats it.
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
        optional<u64> maybe = 42;
        REQUIRE(maybe);

        maybe = nullopt;
        REQUIRE_FALSE(maybe);
    }

    /** Non-Trivially-Destructible Objects Work
     *  ---------------------------------------
     *  When re-seating or un-seating optionals that wrap objects that have a
     *  destructor, optionals do the right thing. Not terribly complicated,
     *  but important none the less.
     */
    SECTION("Destroying wrapped non-trivially-destructible objects") {
        optional<NonTrivialType> maybe = {{4, 6}};
        NonTrivialType & m_ref = *maybe;

        REQUIRE(maybe);
        REQUIRE(maybe->m_a == 4); REQUIRE(maybe->m_b == 6);

        // Note; The destructor of the `NonTrivialType` just nulls member
        // variables. Those should be accessible until the end of this frame.
        maybe = nullopt;

        REQUIRE_FALSE(maybe);
        REQUIRE(m_ref.has_been_destroyed);
    }

    /** Swapping Optionals
     *  ------------------
     */
    SECTION("Swapping the values wrapped by optionals") {
        // NB. `using std::swap` is _required_ to guarantee out custom swap
        // functions can be found w/ ADL.
        using std::swap;

        optional<int> a { 1 };
        optional<int> b { 2 };
        optional<int> c { };
        optional<int> d { };

        swap(a, b);
        REQUIRE(*a == 2); REQUIRE(*b == 1);

        swap(a, c);
        REQUIRE((bool)a == false); REQUIRE(*c == 2);

        swap(a, d);
        REQUIRE((bool)a == false); REQUIRE((bool)d == false);

        swap(a, c);
        REQUIRE(*a == 2); REQUIRE((bool)c == false);
    }
    SECTION("Swapping the values wrapped by reference-wrapping-optionals") {
        // NB. `using std::swap` is _required_ to guarantee out custom swap
        // functions can be found w/ ADL.
        using std::swap;

        int x = 1;
        int y = 2;
        optional<int&> a { x };
        optional<int&> b { y };
        optional<int&> c { };
        optional<int&> d { };

        swap(a, b);
        REQUIRE(*a == 2); REQUIRE(*b == 1);
        REQUIRE(x == 2); REQUIRE(y == 1);

        swap(a, c);
        REQUIRE((bool)a == false); REQUIRE(*c == 2);
        REQUIRE(x == 2); REQUIRE(y == 1);

        swap(a, d);
        REQUIRE((bool)a == false); REQUIRE((bool)d == false);
        REQUIRE(x == 2); REQUIRE(y == 1);

        swap(a, c);
        REQUIRE(*a == 2); REQUIRE((bool)c == false);
        REQUIRE(x == 2); REQUIRE(y == 1);
    }
}


/** FUNCTIONAL TESTS
 *  ================
 *  This section contains tests of the basic functionality.
 *
 *  All tests in this section should be written with BDD-stye sections such that
 *  they form completeish sentences when read sequentially. For example,
 *  "optional types over nones should coerce to boolean correctly."
 */

TEST_CASE("Optional types", "[nonstd][optional]") {
    SECTION("that do not contain value") {
        optional<u64> maybe_value = { };
        optional<u64> none_value = none<u64>();
        optional<u64 &> maybe_ref = { };
        optional<u64 &> none_ref = none<u64 &>();

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

        SECTION("should correctly return default from value_or") {
            REQUIRE(maybe_value.value_or(10) == 10);
            REQUIRE(maybe_value.value_or(20) == 20);
            REQUIRE(none_value.value_or(10) == 10);
            REQUIRE(none_value.value_or(20) == 20);
        }

        SECTION("should stay non-containing through copies") {
            /* This test was added -- and should be retained as a regression
             * test -- in response to an MSVC-only memory bug. The root issue
             * was that the `_optional_val_base`'s (special) Move Constructor
             * was trying to copy uninitialized data out of an empty `other`.
             * For some reason that code path was never used by clang, and the
             * below was only able to reproduce the issue in MSVC. */
            optional<void*> no_value = { };
            REQUIRE_FALSE(no_value);

            optional<void*> still_no_value = no_value;
            REQUIRE_FALSE(no_value);

            auto nonemaker = []() -> optional<void*> {
                return optional<void*> { };
            };
            REQUIRE_FALSE(nonemaker());

            optional<void*> (*nonemaker_ptr)() = nonemaker;
            REQUIRE_FALSE(nonemaker_ptr());

            /* This was the failing test on MSVC. The theory here is that the
             * specific level of indirect met -- wrapping `nonemaker` in a
             * function pointer, calling that fptr from a wrapper function, and
             * immediately returning the results -- was preventing MSVC from
             * eliding the move on the optional constructed in `nonemaker`. In
             * Clang, only one optional was constructed in the whole chain. In
             * MSVC, a second optional would be constructed in `noneforwarder`
             * using what was returned by `nonemaker_ptr`. The (eventually)
             * lead us to find the bug in the _optional_val_base's (special)
             * Move Constructor. */
            auto noneforwarder = [&]() -> optional<void*> {
                return nonemaker_ptr();
            };
            REQUIRE_FALSE(noneforwarder());
        }

        SECTION("should correctly except on checking accessors") {
            optional<u64>        simple_none  = { };
            optional<PODType>    pod_none     = { };
            optional<NonPODType> non_pod_none = { };

            REQUIRE_THROWS_AS([=](){ auto _ = simple_none.value(); }(),
                              nonstd::exception::bad_optional_access);
            REQUIRE_THROWS_AS([=](){ auto _ = pod_none.value(); }(),
                              nonstd::exception::bad_optional_access);
            REQUIRE_THROWS_AS([=](){ auto _ = non_pod_none.value(); }(),
                              nonstd::exception::bad_optional_access);
        }
    }

    SECTION("over built-in types") {
        u64 initial_value = 42;
        auto maybe_qword = just(initial_value);
        auto maybe_not_qword = none<u64>();

        SECTION("should be constructible in all the usual ways") {
            auto value = initial_value;
            optional<u64> by_ctor { value };
            optional<u64> by_assign = value;
            auto by_just = just(value);

            REQUIRE(is_same_v<optional<u64>, decltype(by_ctor)>);
            REQUIRE(is_same_v<optional<u64>, decltype(by_assign)>);
            REQUIRE(is_same_v<optional<u64>, decltype(by_just)>);
        }

        SECTION("should be assignable from empty") {
            optional<i32> maybe = { };
            REQUIRE_FALSE(maybe);

            maybe = 42;
            REQUIRE(maybe);
            REQUIRE(*maybe == 42);

            // NB. This demonstrates that we're correctly working through a very
            // subtle type deduction issue. The below assignment could either be
            // parsed as `maybe = optional<int> { }` or `maybe = int { }`.
            // Importantly, the conversion to a zero-initialized scalar take
            // priority over the construction of a temporary. We want the zero
            // initialization, so it's good that we're constructing a temporary,
            // then performing a move assignment.
            // On the downside, the above `maybe = 42` assignment has to go
            // through the same conversion-to-temporary -> move assignment. Not
            // a bad price to pay for scalar types, though.
            maybe = { };
            REQUIRE(!maybe);
        }

        SECTION("should be sensibly coercible to boolean") {
            REQUIRE(maybe_qword);
            REQUIRE_FALSE(maybe_not_qword);
        }

        SECTION("should allow access to the contained value") {
            REQUIRE(*maybe_qword        == initial_value);
            REQUIRE(maybe_qword.value() == initial_value);
        }

        SECTION("should return their value from value_or") {
            REQUIRE(maybe_qword.value_or(0) == initial_value);
            REQUIRE(maybe_not_qword.value_or(0) == 0);
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
            optional<u64 const &> maybe = { vref };
            auto implicit = just_cref(vref);

            REQUIRE(is_same_v<optional<u64 const &>, decltype(maybe)>);
            REQUIRE(is_same_v<optional<u64 const &>, decltype(implicit)>);
        }

        optional<u64 const &> maybe = { vref };

        SECTION("should preserve constness when accessed") {
            REQUIRE(nonstd::have_same_type(*maybe, vref));

            // Remember, `auto` uses `typeof` semantics, and so does not
            // implicitly deduce when it's supposed to be a reference.
            auto & maybe_vref = *maybe;
            auto   maybe_value = *maybe;
            REQUIRE(std::is_same_v<decltype(maybe_vref), u64 const &>);
            REQUIRE(std::is_same_v<decltype(maybe_value), u64>);
        }
    }

    SECTION("over references") {
        u64 initial_value = 42;
        u64 value = initial_value;

        SECTION("should handle references gracefully in construction") {
            auto & vref = value;
            optional<u64&> by_ctor { vref };
            optional<u64&> by_assign = value;
            auto by_just = just_ref(vref);

            REQUIRE(is_same_v<optional<u64&>, decltype(by_ctor)>);
            REQUIRE(is_same_v<optional<u64&>, decltype(by_assign)>);
            REQUIRE(is_same_v<optional<u64&>, decltype(by_just)>);
        }

        SECTION("should handle constness in arguments correctly") {
            i32 number = 1;
            i32 &       val = number;
            i32 const & const_val = number;

            optional<i32&>        a { val };
            optional<i32 const &> b { val };
         // optional<i32&>        d { const_val }; // Fails; would loose const
            optional<i32 const &> c { const_val };
            auto e = just_ref(val);
            auto f = just_cref(val);
            auto g = just_ref(const_val); // Works; deduces constness
            auto h = just_cref(const_val);
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
                optional<u64 *> by_ctor { vptr };
                optional<u64 *> by_assign { &value };
                auto by_just = just(vptr);

                REQUIRE(is_same_v<optional<u64 *>, decltype(by_ctor)>);
                REQUIRE(is_same_v<optional<u64 *>, decltype(by_assign)>);
                REQUIRE(is_same_v<optional<u64 *>, decltype(by_just)>);
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

                maybe = nullopt;
                REQUIRE_FALSE(maybe);
                REQUIRE(new_value == initial_value + 12);
            }
        }

        SECTION("pointing to c_cstrs") {
            c_cstr initial_value = "42";
            c_cstr value = initial_value;
            c_cstr * value_addr = &value;

            SECTION("should be constructible as per usual") {
                auto * vptr = &value;
                optional<c_cstr *> by_ctor { vptr };
                optional<c_cstr *> by_assign = &value;
                auto by_just = just(vptr);
                auto by_copy_ctor { by_ctor };
                auto by_copy_assign = by_ctor;
                auto by_move_ctor { std::move(by_copy_ctor) };
                auto by_move_assign = std::move(by_copy_assign);

                REQUIRE(is_same_v<optional<c_cstr *>, decltype(by_ctor)>);
                REQUIRE(is_same_v<optional<c_cstr *>, decltype(by_assign)>);
                REQUIRE(is_same_v<optional<c_cstr *>, decltype(by_just)>);
                REQUIRE(is_same_v<optional<c_cstr *>, decltype(by_copy_ctor)>);
                REQUIRE(is_same_v<optional<c_cstr *>, decltype(by_copy_assign)>);
                REQUIRE(is_same_v<optional<c_cstr *>, decltype(by_move_ctor)>);
                REQUIRE(is_same_v<optional<c_cstr *>, decltype(by_move_assign)>);
            }

            auto maybe = just(&value);

            SECTION("should allow access to the referenced value") {
                REQUIRE(*maybe  == value_addr);
                REQUIRE(nonstd::equal_to(**maybe, value));
            }

            SECTION("should allow modification by address") {
                auto new_value = "54";
                **maybe = new_value;
                REQUIRE(*maybe == value_addr);
                REQUIRE(nonstd::equal_to(**maybe, new_value));
                REQUIRE(nonstd::equal_to(value, new_value));
                REQUIRE(!nonstd::equal_to(value, initial_value));
            }

            SECTION("Should un-seat and re-seat correctly") {
                auto new_value = "54";
                auto new_vptr  = &new_value;

                maybe = new_vptr;
                REQUIRE(*maybe == new_vptr);
                REQUIRE(nonstd::equal_to(**maybe, new_value));

                maybe = nullopt;
                REQUIRE_FALSE(maybe);
                REQUIRE(nonstd::equal_to(new_value, "54"));
            }
        }
    }

    SECTION("stored as members") {
        SECTION("should be constructible and coerce to boolean") {
            optional<size_t>  opt = { 42 };

            OptionalContainer oc_nul {     };
            OptionalContainer oc_val {  42 };
            OptionalContainer oc_opt { opt };

            REQUIRE_FALSE(oc_nul.maybe);
            REQUIRE(oc_val.maybe);
            REQUIRE(*(oc_val.maybe) == 42);
            REQUIRE(oc_opt.maybe);
            REQUIRE(*(oc_opt.maybe) == 42);
        }

        SECTION("should be assignable from empty") {
            OptionalContainer oc_nul { };
            REQUIRE_FALSE(oc_nul.maybe);

            oc_nul.assign(42);
            REQUIRE(oc_nul.maybe);
            REQUIRE(*(oc_nul.maybe) == 42);
        }
    }
}

#include "optional.test.disabled_special_members.inl"

} /* namespace optional */
} /* namespace nonstd_test */
