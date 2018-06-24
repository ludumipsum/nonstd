/** N2VET -- Valid Expression Tester
 *  ================================
 *  Used to test the validity of arbitrary `decltype`-able expressions.
 *  Largely taken from the very informative and inspirational
 *  https://jguegant.github.io/blogs/tech/sfinae-introduction.html
 *
 *  The simplest way to leverage this construct is via the `N2VET_PARAM_TESTER`
 *  and `N2VET_TYPES_TESTER` construction macros. Long story short;
 *
 *      namespace can_do_stuff {
 *          N2VET_PARAMS_TESTER(
 *              N2VET_NAMED(params),
 *              N2VET_WITH_ARGS(t, u),
 *              N2VET_TESTING_EXPRESSION(t.doStuff(u))
 *          );
 *          N2VET_TYPES_TESTER(
 *              N2VET_NAMED(types),
 *              N2VET_FROM_PARAMS_TESTER(params),
 *              N2VET_WITH_TEMPLATE_ARGS(T, U)
 *          );
 *      }
 *
 *  The above will create two identifiers in the `can_do_stuff::` namespace;
 *  `params`, and `types` (defined by the `N2VET_NAMED(...)` parameters).The
 *  `params` identifier will accept concrete variables as arguments (two of
 *  them, `t`, and `u`) and return an object with a `.value` member that will
 *  indicate whether the given expression (`t.doStuff(u)`) is valid for the
 *  given parameters. The `types` identifier behaves similarly to `params`, but
 *  is specialized over types -- rather than called with concrete parameters --
 *  and exposes a static `::value` member rather than an instance member.
 *
 *  tl;dr,
 *
 *      struct Stuffer { void doStuff(u32) { return; } };
 *
 *      static_assert( can_do_stuff::types<Stuffer, u32>::value, "");
 *      static_assert(!can_do_stuff::types<Stuffer, f64>::value, "");
 *
 *      main() {
 *          Stuffer s { };
 *          u32 u = 0;
 *          f64 f = 0;
 *
 *          bool will_be_true  = can_do_stuff::params(s, u).value;
 *          bool will_be_false = can_do_stuff::params(s, f).value;
 *      }
 *
 *  Ideally, you shouldn't care what the above N2_PARAMS_TESTER and
 *  N2_TYPES_TESTER macros generate. But. This is the real world. This is about
 *  what the `can_do_stuff` namespace will look like after preprocessing.
 *
 *      namespace can_do_stuff {
 *          auto params = ::nonstd::valid_expression_tester(
 *              []( auto && t, auto && u ) -> decltype( t.doStuff(u) ) { }
 *          );
 *
 *          template <typename T, typename U>
 *          using types = decltype( params( ::std::declval<T>(),
 *                                          ::std::declval<U>() ) );
 *      }
 *
 *
 *  NOTES:
 *  * Because of the complexity of the macros that are used to build instances
 *    of these constructions (specifically that both the argument and expression
 *    sets in N2VET_PARAMS_TESTER must be variadic sets) the Boot Preprocessor
 *    library was brought in.
 *    #SorryNotEntirelyUnapologetic.
 *  * MSVC is, for some reason, unable to correctly apply SFINAE on these
 *    constructs when they are used in template arguments, as such:
 *
 *        These constructs **must** be used in return-type SFINAE calls
 *
 *  * A great deal of complexity here is mandated by MSVC's interpretation of
 *    SFINAE rules. In Clang and GCC, the `N2VET_TYPES_TESTER` is actually
 *    redundant, but MSVC requires that additional layer of template
 *    specialization to parse and compile. If the below ever compiles on MSVC,
 *    we could revisit and simplify this construct.
 *
 *        std::true_type  f(int) { return std::true_type();  }
 *        template <typename T>
 *        std::false_type f(T t) { return std::false_type(); }
 *
 *        template <typename T>
 *        std::enable_if_t< decltype(f(std::declval<T>()))::value>
 *        g() { printf("int\n"); }
 *
 *        template <typename T>
 *        std::enable_if_t<!decltype(f(std::declval<T>()))::value>
 *        g() { printf("not int\n"); }
 */

#pragma once

#include <type_traits>
#include <utility>

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


/* BOOST_PP_ redefinitions; For Great Readability. */
// #define SEQ                   BOOST_PP_SEQ
#define    N2VET_EXPAND          BOOST_PP_SEQ_ENUM
#define    N2VET_MAP             BOOST_PP_SEQ_FOR_EACH
#define    N2VET_VARIADIC_TO_SEQ BOOST_PP_VARIADIC_TO_SEQ

/** N2VE_PARAMS_TESTER(Name: Identifier, Args: SEQ, Expressions: SEQ)
 *  =================================================================
 *  Builds a nonstd::valid_expression_tester, with slightly-harder-to-fuck-up,
 *  and slightly-more-conceptually-clear syntax.
 *
 *  NB. Expects Args and Expressions to be SEQ "types", because both may be
 *  variadic sets. Use `N2VET_WITH_ARGS` and `N2VET_TESTING_EXPRESSION` to
 *  differentiate argument sets.
 */
#define N2VET_PARAMS_TESTER(Name, Args, Expressions)                         \
    auto Name = ::nonstd::valid_expression_tester(                           \
        []( N2VET_EXPAND(Args) ) -> decltype( N2VET_EXPAND(Expressions) ){ } \
    )

/** N2VET_NAMAED(Name: Identifier)
 *  ------------------------------
 *  Pass-through, just for readability's sake.
 */
#define N2VET_NAMED(Name) \
    Name

/** N2VET_WITH_ARGS(...)
 *  --------------------
 *  Converts a list of argument names (`x, y`) to a SEQ of auto rval-ref typed
 *  parameters (`(auto && x)(auto && y)`) suitable for expansion in the body of
 *  an N2VET_TESTER.
 */
#define N2VET_WITH_ARGS(...) \
    N2VET_MAP(N2VET_MAKE_ARG_AUTO, _, N2VET_VARIADIC_TO_SEQ(__VA_ARGS__))

// BOOST_PP FOR_EACH (N2VET_MAP) worker macro. Returns a SEQ element s.t. the
// output of the MAP is another SEQ.
#define N2VET_MAKE_ARG_AUTO(r, _, Elem) \
    (auto && Elem)


/** N2VET_TESTING_EXPRESSION(...)
 *  -----------------------------
 *  Converts a list of expressions (`x.call(), y.call()`) to a SEQ(`(x.call())
 *  (y.call())`) suitable for expansion in the body of an N2VET_TESTER.
 */
#define N2VET_TESTING_EXPRESSION(...) \
    N2VET_VARIADIC_TO_SEQ(__VA_ARGS__)


/** N2VET_TYPES_TESTER(Name: Identifier,
 *                    ParamTester: N2VET_PARAM_TESTER,
 *                    TemplateArgs: SEQ)
 *  ==================================================
 *  Builds an alias template from an N2VET_PARAM_TESTER to test with types
 *  rather than with concrete parameters.
 *
 *  N.B. Expects TemplateArgs to be a SEQ "type" because that brings the macro's
 *  use and implementation in-line with `N2VET_PARAMS_TESTER`. Use
 *  `N2VET_WITH_TEMPLATE_ARGS` to pass in template arguments.
 */
#define N2VET_TYPES_TESTER(Name, ParamTester, TemplateArgs)              \
    template <N2VET_MAP_TYPENAME(TemplateArgs)>                         \
    using Name = decltype(ParamTester(N2VET_MAP_DECLVAL(TemplateArgs)))


/** N2VET_FROM_PARAMS_TESTER(ParamTester: Identifier)
 *  -------------------------------------------------
 *  Pass-through, just for readability's sake.
 */
#define N2VET_FROM_PARAMS_TESTER(ParamTester) \
    ParamTester

/** N2VET_WITH_TEMPLATE_ARGS(...)
 *  -----------------------------
 *  Converts a variadic argument set into a SEQ. Something of a pass-through.
 */
#define N2VET_WITH_TEMPLATE_ARGS(...) \
    N2VET_VARIADIC_TO_SEQ(__VA_ARGS__)


/** N2VET_MAP_TYPENAME(TemplateArgs: SEQ)
 *  -------------------------------------
 *  Converts a SEQ of template arguments (`(T)(U)`) into an expanded set of
 *  typenames (`typename T, typename U`) suitable for use in a `template <...>`
 *  specification.
 */
#define N2VET_MAP_TYPENAME(TemplateArgs) \
    N2VET_EXPAND(N2VET_MAP(N2VET_ADD_TYPENAME, _, TemplateArgs))

// BOOST_PP FOR_EACH (N2VET_MAP) worker macro. Returns a SEQ element s.t. the
// output of the MAP is another SEQ.
#define N2VET_ADD_TYPENAME(r, _, Elem) \
    (typename Elem)


/** N2VET_MAP_DECLVAL(TemplateArgs: SEQ)
 *  ------------------------------------
 *  Converts a SEQ of template arguments (`(T)(U)`) into an expanded set of
 *  declval'd constructors (`::std::declval<T>(), ::std::declval<U>()`) suitable
 *  for use in as an argument list for an N2VET_PARAM_TESTER.
 */
#define N2VET_MAP_DECLVAL(TemplateArgs) \
    N2VET_EXPAND(N2VET_MAP(N2VET_ADD_DECLVAL, _, TemplateArgs))

// BOOST_PP FOR_EACH (N2VET_MAP) worker macro. Returns a SEQ element s.t. the
// output of the MAP is another SEQ.
#define N2VET_ADD_DECLVAL(r, _, Elem) \
    (::std::declval<Elem>())



/** ValidExpressionTester Implementation
 *  ====================================
 *  This is... pretty heavy templatize/decltype trickery. If the article linked
 *  at the top of this file doesn't answer your questions about it, feel free to
 *  ask me (Drew) what's up, and I'll see about expanding this comment.
 */
namespace nonstd {
namespace detail {

    template <typename Lambda_T>
    struct ValidExpressionTester {

        template <typename ... Params>
        constexpr auto test(int /*unused*/)
        -> decltype(std::declval<Lambda_T>()(std::declval<Params>()...),
                    std::true_type()) {
            return std::true_type{};
        }

        template <typename ... Params>
        constexpr std::false_type test(...) {
            return std::false_type{};
        }

        template <typename ... Params>
        constexpr auto operator () (Params const & ... /*unused*/) {
            return test<Params...>(int{});
        }
    };

} /* namespace detail */


template <typename Lambda_T>
constexpr auto valid_expression_tester(Lambda_T const & t) {
    return detail::ValidExpressionTester<Lambda_T>{};
}

} /* namespace nonstd */
