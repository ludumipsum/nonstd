/** Constexpr Value Category Name Printing
 *  ======================================
 *  Like it says on the tin.
 *  Taken nearly directly from http://stackoverflow.com/questions/16637945
 *
 *  Usage;
 *      printf("The category is %s\n", VALUE_CATEGORY(std::move(4)));
 */

#pragma once


template<typename T> struct value_category {
    static constexpr auto value = "prvalue";
};

template<typename T> struct value_category<T&> {
    static constexpr auto value = "lvalue";
};

template<typename T> struct value_category<T&&> {
    static constexpr auto value = "xvalue";
};

// Double parens for ensuring we inspect an expression, not an entity
#define VALUE_CATEGORY(expr) value_category<decltype((expr))>::value
