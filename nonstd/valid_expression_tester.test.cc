/** nonstd::valid_expression_tester
 *  ===============================
 *  Madness. Pure madness.
 */

#include <nonstd/valid_expression_tester.h>
#include <platform/testrunner/testrunner.h>

#include <string>


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


TEST_CASE("Simple N2VET Testers", "[nonstd][sfinae]") {
    SECTION("should function at both compile time, and at run time") {
        static_assert( is_convertible_to_f32<u32>::value,         "");
        static_assert(!is_convertible_to_f32<std::string>::value, "");

        u32 u = 1;
        std::string s = "one";
        REQUIRE(is_convertible_to_f32_param_tester(u).value == true);
        REQUIRE(is_convertible_to_f32_param_tester(s).value == false);
    }
}


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

/** SFINAE'd Template Function Specializations
 *  ------------------------------------------
 *  Like it says on the tin. This is probably the way to get run-time goodness
 *  out of N2VET_TESTERS. */
template <typename T>
std::enable_if_t<has_stringify::types<T>::value, std::string>
/* std::string */globalStringify(T& obj) { return obj.stringify(); }

template <typename T>
std::enable_if_t<!has_stringify::types<T>::value, std::string>
/* std::string */globalStringify(T& obj) { return to_string(obj); }


TEST_CASE("Complex N2VET TESTER", "[nonstd][sfinae]") {
    SECTION("should function at both compile time, and at run time") {
        static_assert( ! has_stringify::types<A>::value, "");
        static_assert(   has_stringify::types<B>::value, "");
        static_assert( ! has_stringify::types<C>::value, "");
        static_assert(   has_stringify::types<D>::value, "");
        static_assert(   has_stringify::types<E>::value, "");

        A a; REQUIRE(has_stringify::params(a).value == false);
        B b; REQUIRE(has_stringify::params(b).value == true );
        C c; REQUIRE(has_stringify::params(c).value == false);
        D d; REQUIRE(has_stringify::params(d).value == true );
        E e; REQUIRE(has_stringify::params(e).value == true );
    }

    SECTION("should function in SFINAE template resolution") {
        A a; REQUIRE(std::string{"A::to_string"} == globalStringify(a));
        B b; REQUIRE(std::string{"B::stringify"} == globalStringify(b));
        C c; REQUIRE(std::string{"C::to_string"} == globalStringify(c));
        D d; REQUIRE(std::string{"D::stringify"} == globalStringify(d));
        E e; REQUIRE(std::string{"E::stringify"} == globalStringify(e));
    }
}

} /* namespace valid_expression_tester */
} /* namespace nonstd_test */
