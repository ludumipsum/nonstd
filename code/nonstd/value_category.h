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

template<typename T> struct value_category_t {
    static inline constexpr c_cstr value = "prvalue";
};

template<typename T> struct value_category_t<T&> {
    static inline constexpr c_cstr value = "lvalue";
};

template<typename T> struct value_category_t<T&&> {
    static inline constexpr c_cstr value = "xvalue";
};

template <typename T>
inline constexpr c_cstr value_category = value_category_t<T>::value;

// Double parens for ensuring we inspect an expression, not an entity
#define VALUE_CATEGORY(expr) ::nonstd::value_category<decltype((expr))>

}
