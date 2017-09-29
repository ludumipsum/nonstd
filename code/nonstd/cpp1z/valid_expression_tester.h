/** nonstd::valid_expression_tester
 *  ===============================
 *  Used to test the validity of arbitrary `decltype` able expressions.
 *  Largely taken from the very informative and inspirational
 *  https://jguegant.github.io/blogs/tech/sfinae-introduction.html
 *
 *  The simplest way to leverage these structures is via the `N2VET_TESTER` and
 *  `N2VET_IS_VALID` macro set. Because of the complexities of building argument
 *  lists and mapping a macro over a list of macro arguments, the Boost
 *  Preprocessor library was brought in. #SorryNotEntirelyUnapologetic. I am a
 *  little bit sorry...
 */

#pragma once

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "type_traits.h"


/* BOOST_PP_ redefinitions; For Great Readability. */
// #define SEQ                   BOOST_PP_SEQ
#define    N2VET_EXPAND          BOOST_PP_SEQ_ENUM
#define    N2VET_MAP             BOOST_PP_SEQ_FOR_EACH
#define    N2VET_VARIADIC_TO_SEQ BOOST_PP_VARIADIC_TO_SEQ
#define foo BOOST_PP_SEQ_ENUM_0

/** N2VET_TESTER(Args: SEQ, Expression: SEQ)
 *  ==========================================
 *  Builds a nonstd::valid_expression_tester, with slightly-harder-to-fuck-up,
 *  and slightly more conceptually-clear syntax.
 *
 *  Usage;
 *      auto is_expression_valid = N2VET_TESTER(
 *          N2VET_WITH_ARGS(t, u),
 *          N2VET_TESTING_EXPRESSION(t.doStuffWith(u), t.doMoreWith(u))
 *      );
 *
 *      auto has_global_insert_operator = N2VET_TESTER(
 *          N2VET_WITH_ARGS(t),
 *          N2VET_TESTING_EXPRESSION(std::cout << t)
 *      );
 *
 *  Because multiple arguments can be passed into multiple expressions, this
 *  macro expects to take two arguments of (Macro-)type SEQ -- the argument list
 *  generated with `N2VET_WITH_ARGS`, and the expression set generated with
 *  `N2VET_TESTING_EXPRESSION`.
 */
#define N2VET_TESTER(Args, Expression)                                      \
    ::nonstd::valid_expression_tester(                                      \
        []( N2VET_EXPAND(Args) ) -> decltype( N2VET_EXPAND(Expression) ){ } \
    )

/** N2VET_WITH_ARGS(...)
 *  --------------------
 *  Converts a list of argument names (`x, y`) to a SEQ of auto rval-ref typed
 *  parameters (`(auto && x)(auto && y)`) suitable for expansion in the body of
 *  an N2VET_TESTER.
 */
#define N2VET_WITH_ARGS(...) \
    N2VET_MAP(N2VET_AUTOIZE_ARGS, _, \
              N2VET_VARIADIC_TO_SEQ(__VA_ARGS__))

// N.B. This worker function wraps its returned element in `( )` s.t. the output
// of N2VET_MAP is another SEQ, rather than raw tokens.
#define N2VET_AUTOIZE_ARGS(r, _, Elem) \
    (auto && Elem)


/** N2VET_TESTING_EXPRESSION(...)
 *  -----------------------------
 *  Converts a list of expressions (`x.call(), y.call()`) to a SEQ(`(x.call())
 *  (y.call())`) suitable for expansion in the body of an N2VET_TESTER.
 */
#define N2VET_TESTING_EXPRESSION(...) \
    N2VET_VARIADIC_TO_SEQ(__VA_ARGS__)



/** N2VET_IS_VALID(Lambda: N2VET_TESTER, Params: SEQ)
 *  =================================================
 *  Tests if an N2VET_TESTER's expression would be valid if called with the
 *  given parameter types.
 *
 *  Usage;
 *      static_assert(N2VET_IS_VALID(has_global_insert_operator,
 *                                   N2VET_WITH_TYPES(std::string)),
 *                    "std::strings definitely respect `operator <<`.");
 */
#define N2VET_IS_VALID(Lambda, Params) \
    decltype(Lambda( N2VET_EXPAND(Params) ))::value


/** N2VET_WITH_TYPES(...)
 *  --------------------
 */
#define N2VET_WITH_TYPES(...) \
    N2VET_MAP(N2VET_DECLVALIZE_PARAMS, _, \
              N2VET_VARIADIC_TO_SEQ(__VA_ARGS__))

// N.B. This worker function wraps its returned element in `( )` s.t. the output
// of N2VET_MAP is another SEQ, rather than raw tokens.
#define N2VET_DECLVALIZE_PARAMS(r, _, Elem) \
    (::std::declval<Elem>())


/** N2VET_WITH_PARAMS(...)
 *  ---------------------
 *  Accepts a list of pre-defined parameters (ex; from the argument list of an
 *  `auto/->`d function) and returns a SEQ. */
#define N2VET_WITH_PARAMS(...) \
    N2VET_VARIADIC_TO_SEQ(__VA_ARGS__)



namespace nonstd {
namespace detail {

    template <typename Lambda_T>
    struct ValidExpressionTester {

        template <typename ... Params>
        constexpr auto test(int /*unused*/)
        -> decltype(std::declval<Lambda_T>()(std::declval<Params>()...),
                    std::true_type()) {
            return std::true_type();
        }

        template <typename ... Params>
        constexpr std::false_type test(...) {
            return std::false_type();
        }

        template <typename ... Params>
        constexpr auto operator () (Params const & ... /*unused*/) {
            return test<Params...>(int());
        }
    };

} /* namespace detail */


template <typename Lambda_T>
constexpr auto valid_expression_tester(const Lambda_T& t) {
    return detail::ValidExpressionTester<Lambda_T>();
}

} /* namespace nonstd */
