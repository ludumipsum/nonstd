/* Type Trait and Type Modifying Macros
 * ====================================
 * Smoke tests for foundational type-trait checkers and modifiers.
 *
 * Author's Note: This file is breaking our 80-column line limit, sadly but
 * intentionally. I found vertical alignment to greatly aid in both authoring
 * and reading, but that came at the cost of some ~95 column lines.
 * #sorrynotsorry
 */

#include <testrunner/testrunner.h>

#include "nonstd/core/primitive_types.h"
#include "nonstd/cpp1z/type_traits.h"


namespace nonstd_test {
namespace type_macros {

// Dead simple helper structs
struct ContainerA { u8 an_int; };
struct ContainerB { u8 an_int; };

// Templatized helper struct
template<typename T> struct TemplateContainer { T member; };

// Templatized helper function -- for demonstrating function template deduction
template<typename T> T identityFunction(T x) { return x; }


TEST_CASE("Type Macros", "[nonstd][types]") {

    SECTION("that check types") {

        SECTION("should correctly compare literal types") {

            REQUIRE(IS_SAME_TYPE(u8, u8));
            REQUIRE(IS_SAME_TYPE(u8 *, u8 *));
            REQUIRE(IS_SAME_TYPE(u8 &, u8 &));
            REQUIRE(IS_SAME_TYPE(c_cstr, c_cstr));
            REQUIRE(IS_SAME_TYPE(ContainerA, ContainerA));
            REQUIRE(IS_SAME_TYPE(TemplateContainer<u8>, TemplateContainer<u8>));

            REQUIRE(IS_DIFFERENT_TYPE(u8,   u16));
            REQUIRE(IS_DIFFERENT_TYPE(u8,   i8));
            REQUIRE(IS_DIFFERENT_TYPE(u8,   u8 const));
            REQUIRE(IS_DIFFERENT_TYPE(u8 &, u8));
            REQUIRE(IS_DIFFERENT_TYPE(u8 &, u8 const &));
            REQUIRE(IS_DIFFERENT_TYPE(u8 &, u8 *));
            REQUIRE(IS_DIFFERENT_TYPE(u8 *, i8 *));
            REQUIRE(IS_DIFFERENT_TYPE(u8 *, u8 * const));
            REQUIRE(IS_DIFFERENT_TYPE(u8 *, u8 const *));
            REQUIRE(IS_DIFFERENT_TYPE(ContainerA, ContainerB));
            REQUIRE(IS_DIFFERENT_TYPE(ContainerA, TemplateContainer<u8>));
            REQUIRE(IS_DIFFERENT_TYPE(TemplateContainer<u8  >, TemplateContainer<u16>));
            REQUIRE(IS_DIFFERENT_TYPE(TemplateContainer<u8  >, TemplateContainer<u8 const>));
            REQUIRE(IS_DIFFERENT_TYPE(TemplateContainer<u8  >, TemplateContainer<u8 &>));
            REQUIRE(IS_DIFFERENT_TYPE(TemplateContainer<u8 &>, TemplateContainer<u8 const &>));
        }

        SECTION("should correctly compare types of objects") {
            u8       u8_a   = 0;
            u8       u8_b   = 255;
            u8 const u8_c_a = 0;
            i8       i8_a   = 0;
            u16      u16_a  = 0;
            ContainerA a_one = { 0 };
            ContainerA a_two = { 0 };
            ContainerB b_one = { 0 };
            auto auto_u8_a = identityFunction<u8>(0);
            auto auto_u8_b = identityFunction<u8>(255);

            REQUIRE(HAS_SAME_TYPE(u8_a, u8_b));
            REQUIRE(HAS_SAME_TYPE(a_one, a_two));
            REQUIRE(HAS_SAME_TYPE(auto_u8_a, auto_u8_b));
            REQUIRE(HAS_SAME_TYPE(auto_u8_a, u8_a));

            REQUIRE(HAS_DIFFERENT_TYPE(u8_a, i8_a));
            REQUIRE(HAS_DIFFERENT_TYPE(u8_a, u16_a));
            REQUIRE(HAS_DIFFERENT_TYPE(u8_a, u8_c_a));
            REQUIRE(HAS_DIFFERENT_TYPE(a_one, b_one));
        }

        SECTION("should correctly handle C++'s weird deduction rules") {
            //NOTE: Function template arguments and `auto` strip both cv and ref
            //      qualifiers from the top-level of deduced types. Observe.
            u8   const expl_u8_c   = identityFunction<u8 const &>(1);
            auto       auto_u8_c   = identityFunction<u8 const &>(2);
            auto const auto_c_u8_c = identityFunction<u8 const &>(3);
            REQUIRE( ! IS_SAME_TYPE(u8 const &, decltype(expl_u8_c)));
            REQUIRE(   IS_SAME_TYPE(u8 const  , decltype(expl_u8_c)));
            REQUIRE( ! IS_SAME_TYPE(u8 const &, decltype(auto_u8_c)));
            REQUIRE(   IS_SAME_TYPE(u8        , decltype(auto_u8_c)));
            REQUIRE( ! IS_SAME_TYPE(u8 const &, decltype(auto_c_u8_c)));
            REQUIRE(   IS_SAME_TYPE(u8 const  , decltype(auto_c_u8_c)));

            u8   const & expl_u8_c_ref   = identityFunction<u8 const &>(4);
            auto       & auto_u8_c_ref   = identityFunction<u8 const &>(5);
            auto const & auto_c_u8_c_ref = identityFunction<u8 const &>(6);
            REQUIRE(   IS_SAME_TYPE(u8 const &, decltype(expl_u8_c_ref)));
            REQUIRE(   IS_SAME_TYPE(u8 const &, decltype(auto_u8_c_ref)));
            REQUIRE(   IS_SAME_TYPE(u8 const &, decltype(auto_c_u8_c_ref)));


            u8 u8_ = 1;
            TemplateContainer<u8        > tmplt_mutbl     = { 0 };
            TemplateContainer<u8 const  > tmplt_const     = { 0 };
            TemplateContainer<u8       &> tmplt_mutbl_ref = { u8_ };
            TemplateContainer<u8 const &> tmplt_const_ref = { u8_ };

            REQUIRE(HAS_DIFFERENT_TYPE(tmplt_mutbl.member, tmplt_const.member));
            REQUIRE(HAS_DIFFERENT_TYPE(tmplt_mutbl_ref.member, tmplt_const_ref.member));

            auto tmplt_mutbl_m     = tmplt_mutbl.member;
            auto tmplt_const_m     = tmplt_const.member;
            auto tmplt_mutbl_ref_m = tmplt_mutbl_ref.member;
            auto tmplt_const_ref_m = tmplt_const_ref.member;
            REQUIRE(IS_SAME_TYPE(u8, decltype(tmplt_mutbl_m)));
            REQUIRE(IS_SAME_TYPE(u8, decltype(tmplt_const_m)));
            REQUIRE(IS_SAME_TYPE(u8, decltype(tmplt_mutbl_ref_m)));
            REQUIRE(IS_SAME_TYPE(u8, decltype(tmplt_const_ref_m)));

            auto & tmplt_mutbl_m_ref     = tmplt_mutbl.member;
            auto & tmplt_const_m_ref     = tmplt_const.member;
            auto & tmplt_mutbl_ref_m_ref = tmplt_mutbl_ref.member;
            auto & tmplt_const_ref_m_ref = tmplt_const_ref.member;
            REQUIRE(IS_SAME_TYPE(u8 &,       decltype(tmplt_mutbl_m_ref)));
            REQUIRE(IS_SAME_TYPE(u8 const &, decltype(tmplt_const_m_ref)));
            REQUIRE(IS_SAME_TYPE(u8 &,       decltype(tmplt_mutbl_ref_m_ref)));
            REQUIRE(IS_SAME_TYPE(u8 const &, decltype(tmplt_const_ref_m_ref)));


            auto lambda_auto  = [](auto       x) -> auto         { return x; };
            auto lambda_const = [](auto const x) -> auto const   { return x; };
            auto lambda_ref   = [](auto &     x) -> auto       & { return x; };

         /* u8 */      u8_    = 4;
            u8 const   u8_c   = 4;
            u8       & u8_r   = u8_;
            u8 const & u8_c_r = u8_;

            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_auto(u8_)) ));
            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_auto(u8_c)) ));
            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_auto(u8_r)) ));
            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_auto(u8_c_r)) ));

            /* NB. `const` is dropped from return types when they're values. */
            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_const(u8_)) ));
            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_const(u8_c)) ));
            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_const(u8_r)) ));
            REQUIRE(IS_SAME_TYPE( u8        , decltype(lambda_const(u8_c_r)) ));

            REQUIRE(IS_SAME_TYPE( u8       &, decltype(lambda_ref(u8_)) ));
            REQUIRE(IS_SAME_TYPE( u8 const &, decltype(lambda_ref(u8_c)) ));
            REQUIRE(IS_SAME_TYPE( u8       &, decltype(lambda_ref(u8_r)) ));
            REQUIRE(IS_SAME_TYPE( u8 const &, decltype(lambda_ref(u8_c_r)) ));


            ContainerA         a_    { 4 };
            ContainerA const   a_c   { 4 };
            ContainerA       & a_r   = a_;
            ContainerA const & a_c_r = a_;

            REQUIRE(IS_SAME_TYPE( ContainerA const  , decltype(lambda_const(a_)) ));
            REQUIRE(IS_SAME_TYPE( ContainerA const  , decltype(lambda_const(a_c)) ));
            REQUIRE(IS_SAME_TYPE( ContainerA const  , decltype(lambda_const(a_r)) ));
            REQUIRE(IS_SAME_TYPE( ContainerA const  , decltype(lambda_const(a_c_r)) ));

            REQUIRE(IS_SAME_TYPE( ContainerA       &, decltype(lambda_ref(a_)) ));
            REQUIRE(IS_SAME_TYPE( ContainerA const &, decltype(lambda_ref(a_c)) ));
            REQUIRE(IS_SAME_TYPE( ContainerA       &, decltype(lambda_ref(a_r)) ));
            REQUIRE(IS_SAME_TYPE( ContainerA const &, decltype(lambda_ref(a_c_r)) ));
        }
    }

    SECTION("that manipulate types") {

        SECTION("should correctly strip ref qualifiers") {
            REQUIRE(IS_SAME_TYPE(u8      , REMOVE_REFERENCE_TYPE(u8 &)));
            REQUIRE(IS_SAME_TYPE(u8 const, REMOVE_REFERENCE_TYPE(u8 const &)));
            REQUIRE(IS_SAME_TYPE(u8     *, REMOVE_REFERENCE_TYPE(u8 * &)));

            REQUIRE(IS_SAME_TYPE(u8      , REMOVE_REFERENCE_TYPE(u8 &&)));
            REQUIRE(IS_SAME_TYPE(u8 const, REMOVE_REFERENCE_TYPE(u8 const &&)));
            REQUIRE(IS_SAME_TYPE(u8     *, REMOVE_REFERENCE_TYPE(u8 * &&)));

            REQUIRE(IS_SAME_TYPE(u8      , REMOVE_REFERENCE_TYPE(u8)));
            REQUIRE(IS_SAME_TYPE(u8 const, REMOVE_REFERENCE_TYPE(u8 const)));
            REQUIRE(IS_SAME_TYPE(u8     *, REMOVE_REFERENCE_TYPE(u8 *)));
        }

        SECTION("should correctly add lvalue ref qualifiers") {
            REQUIRE(IS_SAME_TYPE(u8       &, ADD_LVAL_REFERENCE_TYPE(u8)));
            REQUIRE(IS_SAME_TYPE(u8 const &, ADD_LVAL_REFERENCE_TYPE(u8 const)));
            REQUIRE(IS_SAME_TYPE(u8 *     &, ADD_LVAL_REFERENCE_TYPE(u8 *)));

            REQUIRE(IS_SAME_TYPE(u8       &, ADD_LVAL_REFERENCE_TYPE(u8 &)));
            REQUIRE(IS_SAME_TYPE(u8 const &, ADD_LVAL_REFERENCE_TYPE(u8 const &)));
            REQUIRE(IS_SAME_TYPE(u8 *     &, ADD_LVAL_REFERENCE_TYPE(u8 * &)));

            // NB. Adding an lvalue ref qualifier to an rvalue reference doesn't
            // really makes sense, but -- as of C++11 -- reference collapsing
            // rules allows this operation to yield an lvalue reference
            // [This may change in C++17, w/ a change of add_*_ref semantics.]
            REQUIRE(IS_SAME_TYPE(u8       &, ADD_LVAL_REFERENCE_TYPE(u8 &&)));
            REQUIRE(IS_SAME_TYPE(u8 const &, ADD_LVAL_REFERENCE_TYPE(u8 const &&)));
            REQUIRE(IS_SAME_TYPE(u8 *     &, ADD_LVAL_REFERENCE_TYPE(u8 * &&)));
        }

        SECTION("should correctly add rvalue ref qualifiers") {
            REQUIRE(IS_SAME_TYPE(u8       &&, ADD_RVAL_REFERENCE_TYPE(u8)));
            REQUIRE(IS_SAME_TYPE(u8 const &&, ADD_RVAL_REFERENCE_TYPE(u8 const)));
            REQUIRE(IS_SAME_TYPE(u8 *     &&, ADD_RVAL_REFERENCE_TYPE(u8 *)));

            REQUIRE(IS_SAME_TYPE(u8       &&, ADD_RVAL_REFERENCE_TYPE(u8 &&)));
            REQUIRE(IS_SAME_TYPE(u8 const &&, ADD_RVAL_REFERENCE_TYPE(u8 const &&)));
            REQUIRE(IS_SAME_TYPE(u8 *     &&, ADD_RVAL_REFERENCE_TYPE(u8 * &&)));

            // NB. Adding an rvalue ref qualifier to an lvalue reference doesn't
            // really make sense, but -- as of C++11 -- reference collapsing
            // rules dictate that the resulting type will be an lvalue reference
            // [This may change in C++17, w/ a change of add_*_ref semantics.]
            REQUIRE(IS_DIFFERENT_TYPE(u8       &&, ADD_RVAL_REFERENCE_TYPE(u8 &)));
            REQUIRE(IS_DIFFERENT_TYPE(u8 const &&, ADD_RVAL_REFERENCE_TYPE(u8 const &)));
            REQUIRE(IS_DIFFERENT_TYPE(u8 *     &&, ADD_RVAL_REFERENCE_TYPE(u8 * &)));

            REQUIRE(IS_SAME_TYPE(u8       &, ADD_RVAL_REFERENCE_TYPE(u8 &)));
            REQUIRE(IS_SAME_TYPE(u8 const &, ADD_RVAL_REFERENCE_TYPE(u8 const &)));
            REQUIRE(IS_SAME_TYPE(u8 *     &, ADD_RVAL_REFERENCE_TYPE(u8 * &)));
        }

        SECTION("Should correctly decay top-level cv and ref qualifiers") {
            REQUIRE(IS_SAME_TYPE(u8, DECAY_TYPE(u8)));
            REQUIRE(IS_SAME_TYPE(u8, DECAY_TYPE(u8 const)));
            REQUIRE(IS_SAME_TYPE(u8, DECAY_TYPE(u8 &)));
            REQUIRE(IS_SAME_TYPE(u8, DECAY_TYPE(u8 const &)));

            REQUIRE(IS_SAME_TYPE(u8       *, DECAY_TYPE(u8       *)));
            REQUIRE(IS_SAME_TYPE(u8 const *, DECAY_TYPE(u8 const *)));
            REQUIRE(IS_SAME_TYPE(u8 const *, DECAY_TYPE(u8 const * const)));
            REQUIRE(IS_SAME_TYPE(u8 const *, DECAY_TYPE(u8 const * const &)));
        }
    }
}


using nonstd::remove_cvref_t;
using nonstd::is_swappable_v;
using std::is_same_v;

struct NotAssignable {
    u8 i;

    NotAssignable& operator= (NotAssignable const &) = delete;
};

TEST_CASE("Extended Type Traits", "[nonstd][types]") {
    SECTION("remove_cvref should work correctly") {
        REQUIRE(is_same_v<int, remove_cvref_t<int>>);
        REQUIRE(is_same_v<int, remove_cvref_t<int const>>);
        REQUIRE(is_same_v<int, remove_cvref_t<int&>>);
        REQUIRE(is_same_v<int, remove_cvref_t<int const &>>);
        REQUIRE(is_same_v<int, remove_cvref_t<int volatile &>>);
    }

    SECTION("is_swappable shoudl work correctly") {
        REQUIRE(is_swappable_v<int>);
        REQUIRE(is_swappable_v<int&>);

        REQUIRE(!is_swappable_v<int const>);
        REQUIRE(!is_swappable_v<NotAssignable>);
    }
}

} /* namespace type_macros */
} /* namespace nonstd_test */
