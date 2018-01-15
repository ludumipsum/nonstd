/** Constexpr Value Category Name Printing
 *  ======================================
 *  Like it says on the tin.
 *  Taken nearly directly from http://stackoverflow.com/questions/16637945
 *
 *  Usage;
 *      printf("The category is %s\n", VALUE_CATEGORY(std::move(4)));
 *
 *  TODO: If we make serious use of these at any point, we should consider
 *  moving the whole construct into to_cstr.h; map from value type to enum, from
 *  enum to c_cstr, return the c_cstr to be printf'd.
 */

#pragma once


namespace nonstd {

template<typename T> struct value_category {
    static constexpr auto value = "prvalue";
};

template<typename T> struct value_category<T&> {
    static constexpr auto value = "lvalue";
};

template<typename T> struct value_category<T&&> {
    static constexpr auto value = "xvalue";
};

template <template T>
inline constexpr bool value_category_v = value_category<T>::value;

// Double parens for ensuring we inspect an expression, not an entity
#define VALUE_CATEGORY(expr) ::nonstd::value_category_v<decltype((expr))>

}
