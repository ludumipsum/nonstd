#pragma once

#include <nonstd/cpp1z/type_trait_assertions.h>
#include <nonstd/core/break.h>
#include <nonstd/core/primitive_types.h>
#include <nonstd/std_ish/compare.h>

#include <nonstd/memory/buffer.h>
#include <nonstd/memory/core_functions.h>


namespace nonstd {

template <typename T>
struct BufferedValue {

    Buffer * m_buf = nullptr;

    constexpr BufferedValue() noexcept
    { }
    constexpr BufferedValue(std::nullptr_t) noexcept
    { }
    constexpr BufferedValue(Buffer * buf) noexcept
        : m_buf ( buf )
    { }
    BufferedValue(c_cstr name)
        : BufferedValue
            ( memory::find(name)
            ? *memory::find(name)
            : memory::allocate(name, sizeof(T))
            )
    { }


    inline BufferedValue& operator= (T val) {
        *reinterpret_cast<T*>(m_buf->data) = val;
        return *this;
    }

    inline T& value() {
        return *reinterpret_cast<T*>(m_buf->data);
    }
    inline T& operator* () { return value(); }
    inline operator T&  () { return value(); }

    inline T const & value() const {
        return *reinterpret_cast<T*>(m_buf->data);
    }
    inline T const & operator* () const { return value(); }
    inline operator T const &  () const { return value(); }

    /** What Should be the Spaceship Operator
     *  -------------------------------------
     */
    friend bool operator== (BufferedValue<T> const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::equal_to(lhs.value(), rhs.value());
    }
    friend bool operator== (T const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::equal_to(lhs, rhs.value());
    }
    friend bool operator== (BufferedValue<T> const & lhs,
                            T const & rhs) noexcept {
        return nonstd::equal_to(lhs.value(), rhs);
    }
    friend bool operator!= (BufferedValue<T> const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return !nonstd::equal_to(lhs.value(), rhs.value());
    }
    friend bool operator!= (T const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return !nonstd::equal_to(lhs, rhs.value());
    }
    friend bool operator!= (BufferedValue<T> const & lhs,
                            T const & rhs) noexcept {
        return !nonstd::equal_to(lhs.value(), rhs);
    }
    friend bool operator<  (BufferedValue<T> const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs.value()) < 0;
    }
    friend bool operator<  (T const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs, rhs.value()) < 0;
    }
    friend bool operator<  (BufferedValue<T> const & lhs,
                            T const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs) < 0;
    }
    friend bool operator> (BufferedValue<T> const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs.value()) > 0;
    }
    friend bool operator> (T const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs, rhs.value()) > 0;
    }
    friend bool operator> (BufferedValue<T> const & lhs,
                            T const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs) > 0;
    }
    friend bool operator<= (BufferedValue<T> const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs.value()) <= 0;
    }
    friend bool operator<= (T const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs, rhs.value()) <= 0;
    }
    friend bool operator<= (BufferedValue<T> const & lhs,
                            T const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs) <= 0;
    }
    friend bool operator>= (BufferedValue<T> const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs.value()) >= 0;
    }
    friend bool operator>= (T const & lhs,
                            BufferedValue<T> const & rhs) noexcept {
        return nonstd::compare(lhs, rhs.value()) >= 0;
    }
    friend bool operator>= (BufferedValue<T> const & lhs,
                            T const & rhs) noexcept {
        return nonstd::compare(lhs.value(), rhs) >= 0;
    }

};

} /* namespace nonstd */
