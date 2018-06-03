/** void_t<Ts...>
 *  =============
 *  C++17 feature; its definition will be simplified with that spec.
 *  Utility metafunction that maps a sequence of any types to the type `void`.
 *  This is primarily used in SFINAE idioms, ex; to remove class template
 *  overloads based on type traits (`class` used for brevity);
 *
 *     template <class, class = std::void_t<>>
 *     struct has_member_foo : std::false_type { };
 *
 *     template <class T>
 *     struct has_member_foo<T, std::void_t<class T::foo>> : std::true_type { };
 */

#pragma once


namespace nonstd {

template <typename... Ts> struct make_void { typedef void type; };
template <typename... Ts> using void_t = typename make_void<Ts...>::type;

} /* namespace nonstd */
