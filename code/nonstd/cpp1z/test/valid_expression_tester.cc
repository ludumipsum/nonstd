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
N2VET_PARAMS_TESTER(
    N2VET_NAMED(is_convertible_to_f32_param_tester),
    N2VET_WITH_ARGS(t),
    N2VET_TESTING_EXPRESSION((f32)t)
);
N2VET_TYPES_TESTER(
    N2VET_NAMED(is_convertible_to_f32),
    N2VET_FROM_PARAMS_TESTER(is_convertible_to_f32_param_tester),
    N2VET_WITH_TEMPLATE_ARGS(T)
);

static_assert( is_convertible_to_f32<u32>::value,
              "u32s are convertible to f32.");
static_assert(!is_convertible_to_f32<std::string>::value,
              "std::strings are not convertible to f32.");


/** Class Member Tests
 *  ------------------
 *  Some of the below types have a callable `.stringify()` member (member
 *  function, inherited member function, or callable member variable (functor)).
 *  Others do not. This set of tests demonstrated that an N2VET_TYPES_TESTER can
 *  be used to determine this kind of existence at compile time.
 */
namespace has_stringify {

    N2VET_PARAMS_TESTER(
        N2VET_NAMED(params),
        N2VET_WITH_ARGS(t),
        N2VET_TESTING_EXPRESSION(t.stringify())
    );
    N2VET_TYPES_TESTER(
        N2VET_NAMED(types),
        N2VET_FROM_PARAMS_TESTER(params),
        N2VET_WITH_TEMPLATE_ARGS(T)
    );

} /* namespace has_stringify */

// Type A
// ------
// No members, but an overload of the free to_string function.
struct A { };
std::string to_string(const A&) { return "A::to_string"; }

// Type B
// ------
// A basic serialize method.
struct B {
    std::string stringify() const { return "B::stringify"; }
};

// Type C
// ------
// A non-callable stringify member, and a to_string overload.
struct C {
    std::string stringify;
};
std::string to_string(const C&) { return "C::to_string"; }

// Type D
// ------
// A subclass of A, with both a to_string overload (from A), and
//           its own stringify method.
struct D : A {
    std::string stringify() const { return "D::stringify"; }
};

// Type E
// ------
// A callable inner class named "stringify" (a functor).
struct E {
    struct Functor {
        std::string operator() () { return "E::stringify"; }
    };
    Functor stringify;
};

/* Static Tests! */
static_assert( ! has_stringify::types<A>::value, "");
static_assert(   has_stringify::types<B>::value, "");
static_assert( ! has_stringify::types<C>::value, "");
static_assert(   has_stringify::types<D>::value, "");
static_assert(   has_stringify::types<E>::value, "");


/** SFINAE'd Template Function Specializations
 *  ------------------------------------------
 *  Like it says on the tin. This is probably the way to get run-time goodness
 *  out of N2VET_TESTERS. */
template <typename T>
ENABLE_IF_DTYPE( has_stringify::types<T>::value, std::string)
/* std::string */globalStringify(T& obj) { return obj.stringify(); }

template <typename T>
ENABLE_IF_DTYPE(!has_stringify::types<T>::value, std::string)
/* std::string */globalStringify(T& obj) { return to_string(obj); }


TEST_CASE("The N2VET_TESTER Construct", "[nonstd][sfinae]") {

    SECTION("should be able to correctly... function") {

        A a; REQUIRE(std::string{"A::to_string"} == globalStringify(a));
        B b; REQUIRE(std::string{"B::stringify"} == globalStringify(b));
        C c; REQUIRE(std::string{"C::to_string"} == globalStringify(c));
        D d; REQUIRE(std::string{"D::stringify"} == globalStringify(d));
        E e; REQUIRE(std::string{"E::stringify"} == globalStringify(e));

    }
}

} /* namespace valid_expression_tester */
} /* namespace nonstd_test */
