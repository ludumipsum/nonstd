/** nonstd::valid_expression_tester
 *  =======================
 *  Madness. Pure madness.
 */

#include <string>

#include <testrunner/testrunner.h>

#include "nonstd/core/primitive_types.h"
#include "nonstd/cpp1z/valid_expression_tester.h"

namespace nonstd_test {
namespace valid_expression_tester {

/** Simple Compile-Time Tests
 *  -------------------------
 *  This (poorly) replicates std::is_convertible<f32, FROM>::value.
 */
auto is_convertible_to_f32 = N2VET_TESTER(
        N2VET_WITH_ARGS(t),
        N2VET_TESTING_EXPRESSION((f32)t)
    );
static_assert( N2VET_IS_VALID(is_convertible_to_f32, N2VET_WITH_TYPES(u32)),
              "u32s are convertible to f32.");
static_assert(!N2VET_IS_VALID(is_convertible_to_f32, N2VET_WITH_TYPES(std::string)),
              "std::strings are not convertible to f32.");


/** Class Member Tests
 *  ------------------
 *  Some of the below types have a callable `.serialize()` member (member
 *  function, inherited member function, or callable member variable (functor)).
 *  Others do not. This set of tests demonstrated that N2VET_TESTER can be used
 *  to determine this kind of existence at compile time.
 */
auto has_serialize = N2VET_TESTER(
        N2VET_WITH_ARGS(t),
        N2VET_TESTING_EXPRESSION(t.serialize())
    );

// Type A
// ------
// No members, but an overload of the free to_string function.
struct A { };
std::string to_string(const A&) { return "A::to_string"; }

// Type B
// ------
// A basic serialize method.
struct B {
    std::string serialize() const { return "B::serialize"; }
};

// Type C
// ------
// A non-callable serialize member, and a to_string overload.
struct C {
    std::string serialize;
};
std::string to_string(const C&) { return "C::to_string"; }

// Type D
// ------
// A subclass of A, with both a to_string overload (from A), and
//           its own serialize method.
struct D : A {
    std::string serialize() const { return "D::serialize"; }
};

// Type E
// ------
// A callable inner class named "serialize" (a functor).
struct E {
    struct Functor {
        std::string operator() () { return "E::serialize"; }
    };
    Functor serialize;
};

/* Static Tests! */
static_assert(!N2VET_IS_VALID(has_serialize, N2VET_WITH_TYPES(A)), "");
static_assert( N2VET_IS_VALID(has_serialize, N2VET_WITH_TYPES(B)), "");
static_assert(!N2VET_IS_VALID(has_serialize, N2VET_WITH_TYPES(C)), "");
static_assert( N2VET_IS_VALID(has_serialize, N2VET_WITH_TYPES(D)), "");
static_assert( N2VET_IS_VALID(has_serialize, N2VET_WITH_TYPES(E)), "");


/** SFINAE'd Template Function Specializations
 *  ------------------------------------------
 *  Like it says on the tin. This is probably the way to get run-time goodness
 *  out of N2VET_TESTERS. */
template <typename T>
ENABLE_IF_DTYPE(N2VET_IS_VALID(has_serialize, N2VET_WITH_TYPES(T)),
                std::string)
globalSerialize(T& obj) { return obj.serialize(); }

template <typename T>
ENABLE_IF_DTYPE(!N2VET_IS_VALID(has_serialize, N2VET_WITH_TYPES(T)),
                std::string)
globalSerialize(T& obj) { return to_string(obj); }


TEST_CASE("The N2VET_TESTER Construct", "[nonstd][sfinae]") {

    SECTION("should be able to correctly... function") {

        A a; REQUIRE(std::string{"A::to_string"} == globalSerialize(a));
        B b; REQUIRE(std::string{"B::serialize"} == globalSerialize(b));
        C c; REQUIRE(std::string{"C::to_string"} == globalSerialize(c));
        D d; REQUIRE(std::string{"D::serialize"} == globalSerialize(d));
        E e; REQUIRE(std::string{"E::serialize"} == globalSerialize(e));

    }
}

} /* namespace valid_expression_tester */
} /* namespace nonstd_test */
