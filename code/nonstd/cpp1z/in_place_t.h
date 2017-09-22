/** In Place Constructor Tags
 *  =========================
 *  Useful for disambiguating constructors that accept initializer lists and/or
 *  variadic argument packs. ex;
 *
 *      struct Foo {
 *          template <typename T> Foo(T) { . . . }
 *          template <typename... Args> Foo(Args&&...) { . . . }
 *      };
 *      Foo { 0 }; // Which gets called?
 *
 *      struct Bar {
 *          template <typename T> Bar(T) { . . . }
 *          template <typename... Args> Bar(nonstd::in_place_t, Args&&...) { . . }
 *      };
 *      Bar { 0 };
 *      Bar { nonstd::in_place, 0 }; // Unambiguous.
 *
 *  Note that in the C++17 standard, the default (0-arg) constructors are
 *  explicitly defined with `= default` and are marked `explicit`. I have _no
 *  idea_ why this was done, and have chosen to not replicate that definition.
 *
 *  NB. We would like the constexpr instances of these types as inline variables
 *  so we external linkage is applied to them, but constexpr inline variables
 *  are a C++17 thing, so we're going to have to rely on the compiler to not
 *  make a million copies of each.
 */

#pragma once

#include <cstddef>


namespace nonstd {

struct in_place_t { };
constexpr /*inline*/ in_place_t in_place{};

template <typename T> struct in_place_type_t { };
template <typename T> constexpr /*inline*/ in_place_type_t<T> in_place_type{};

template <size_t I> struct in_place_index_t { };
template <size_t I> constexpr /*inline*/ in_place_index_t<I> in_place_index{};

} /* namespace nonstd */
