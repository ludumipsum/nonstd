/** `enum class` Operator Overload Generators
 *  =========================================
 *  `enum class`s are great. Except that they can't be easily converted to their
 *  underlying type. And binary operators aren't defined for them. Arithmetic
 *  operators aren't defined, either. Print overloads aren't there either...
 *
 *  The free functions and free-function-generating macros defined in this file
 *  are designed to mollify these issues.
 */
#pragma once

#include <nonstd/nonstd.h>
#include <type_traits>


namespace nonstd {

template <typename Enum>
auto underlying_value(std::enable_if_t<std::is_enum_v<Enum>, Enum> e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}


#define GENERATE_PRINT_OVERLOADS_FOR_ENUM_CLASS(ENUM) \
inline void format_arg(fmt::BasicFormatter<char> & f,                          \
                       c_cstr                    & format_str,                 \
                       ENUM const                & e) {                        \
    f.writer().write("0x{:x}", static_cast<std::underlying_type_t<ENUM>>(e));  \
}                                                                              \
inline std::ostream & operator<< (std::ostream & s,                            \
                                  ENUM const & e) {                            \
    return s << "0x{:x}"_format(static_cast<std::underlying_type_t<ENUM>>(e)); \
}                                                                              \
struct ___n2_force_semicolon_on_macro


#define GENERATE_OPERATOR_OVERLOADS_FOR_ENUM_CLASS(ENUM) \
inline constexpr ENUM operator~ (ENUM const & a) noexcept {                    \
    return static_cast<ENUM>(                                                  \
        ~static_cast<std::underlying_type_t<ENUM>>(a)                          \
    );                                                                         \
}                                                                              \
inline constexpr ENUM operator& (ENUM const & lhs, ENUM const & rhs) noexcept {\
    return static_cast<ENUM>(                                                  \
        static_cast<std::underlying_type_t<ENUM>>(lhs) &                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
}                                                                              \
inline constexpr ENUM operator| (ENUM const & lhs, ENUM const & rhs) noexcept {\
    return static_cast<ENUM>(                                                  \
        static_cast<std::underlying_type_t<ENUM>>(lhs) |                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
}                                                                              \
inline constexpr ENUM operator^ (ENUM const & lhs, ENUM const & rhs) noexcept {\
    return static_cast<ENUM>(                                                  \
        static_cast<std::underlying_type_t<ENUM>>(lhs) ^                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
}                                                                              \
inline constexpr ENUM operator+ (ENUM const & lhs, ENUM const & rhs) noexcept {\
    return static_cast<ENUM>(                                                  \
        static_cast<std::underlying_type_t<ENUM>>(lhs) +                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
}                                                                              \
inline constexpr ENUM operator- (ENUM const & lhs, ENUM const & rhs) noexcept {\
    return static_cast<ENUM>(                                                  \
        static_cast<std::underlying_type_t<ENUM>>(lhs) -                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
}                                                                              \
inline constexpr ENUM& operator&= (ENUM& lhs, ENUM const & rhs) noexcept {     \
    lhs = static_cast<ENUM>(                                                   \
        static_cast<std::underlying_type_t<ENUM>>(lhs) &                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
    return lhs;                                                                \
}                                                                              \
inline constexpr ENUM& operator|= (ENUM& lhs, ENUM const & rhs) noexcept {     \
    lhs = static_cast<ENUM>(                                                   \
        static_cast<std::underlying_type_t<ENUM>>(lhs) |                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
    return lhs;                                                                \
}                                                                              \
inline constexpr ENUM& operator^= (ENUM& lhs, ENUM const & rhs) noexcept {     \
    lhs = static_cast<ENUM>(                                                   \
        static_cast<std::underlying_type_t<ENUM>>(lhs) ^                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
    return lhs;                                                                \
}                                                                              \
inline constexpr ENUM& operator+= (ENUM& lhs, ENUM const & rhs) noexcept {     \
    lhs = static_cast<ENUM>(                                                   \
        static_cast<std::underlying_type_t<ENUM>>(lhs) +                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
    return lhs;                                                                \
}                                                                              \
inline constexpr ENUM& operator-= (ENUM& lhs, ENUM const & rhs) noexcept {     \
    lhs = static_cast<ENUM>(                                                   \
        static_cast<std::underlying_type_t<ENUM>>(lhs) -                       \
        static_cast<std::underlying_type_t<ENUM>>(rhs)                         \
    );                                                                         \
    return lhs;                                                                \
}                                                                              \
inline constexpr bool is_null(ENUM const & e) noexcept {                       \
    return static_cast<std::underlying_type_t<ENUM>>(e) == 0;                  \
}                                                                              \
inline constexpr bool is_any(ENUM const & e) noexcept {                        \
    return static_cast<std::underlying_type_t<ENUM>>(e) != 0;                  \
}                                                                              \
struct ___n2_force_semicolon_on_macro

} /* namespace nonstd */
