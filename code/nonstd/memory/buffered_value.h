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
    static_assert(!IS_REFERENCE(T), "BufferedValues cannot wrap reference types.");

    Buffer * m_buf;


    inline Buffer * findOrAllocateBuffer(c_cstr name) {
        auto maybe_buffer = memory::find(name);
        return maybe_buffer
             ? *maybe_buffer
             : memory::allocate(name, sizeof(T));
    }

    constexpr BufferedValue() noexcept = default;
    constexpr BufferedValue(std::nullptr_t) noexcept
        : m_buf ( nullptr )
    { }

    constexpr explicit BufferedValue(Buffer * buf) noexcept
        : m_buf ( buf )
    { }

    explicit BufferedValue(c_cstr name)
        : m_buf ( findOrAllocateBuffer(name) )
    { }
    BufferedValue(c_cstr name, T const & val)
        : m_buf ( findOrAllocateBuffer(name) )
    {
        set(val);
    }
    BufferedValue(c_cstr name, T && val)
        : m_buf ( findOrAllocateBuffer(name) )
    {
        set(std::move(val));
    }

    BufferedValue(BufferedValue const & other) = default;
    BufferedValue& operator= (BufferedValue const & other) = default;

    BufferedValue(BufferedValue &&) = delete;
    BufferedValue& operator= (BufferedValue &&) = delete;


    inline BufferedValue&  set(T const & val) &  {
        *reinterpret_cast<T*>(m_buf->data) = val;
        return *this;
    }
    inline BufferedValue&  set(T && val) &  {
        *reinterpret_cast<T*>(m_buf->data) = std::move(val);
        return *this;
    }
    inline BufferedValue&& set(T const & val) && {
        return std::move(this->set(val));
    }
    inline BufferedValue&& set(T && val) && {
        return std::move(this->set(std::move(val)));
    }

    inline BufferedValue& operator= (T const & val) { return set(val); }
    inline BufferedValue& operator= (T && val) { return set(std::move(val)); }

    inline T& value() & {
        return *reinterpret_cast<T*>(m_buf->data);
    }
    inline T&       operator*   () & { return value(); }
    inline explicit operator T& () & { return value(); }

    inline T const& value() const& {
        return *reinterpret_cast<T*>(m_buf->data);
    }
    inline T const& operator*         () const& { return value(); }
    inline explicit operator T const& () const& { return value(); }

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

}; ENFORCE_POD(BufferedValue<int>);

} /* namespace nonstd */
