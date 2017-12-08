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


    inline Buffer * find_or_allocate_buffer(c_cstr buffer_name) {
        auto maybe_buffer = memory::find(buffer_name);
        return maybe_buffer
             ? *maybe_buffer
             : memory::allocate(buffer_name, sizeof(T));
    }

    constexpr BufferedValue() noexcept = default;
    constexpr BufferedValue(std::nullptr_t) noexcept
        : m_buf ( nullptr )
    { }

    constexpr explicit BufferedValue(Buffer * buf) noexcept
        : m_buf ( buf )
    { }
    explicit BufferedValue(c_cstr buffer_name)
        : m_buf ( find_or_allocate_buffer(buffer_name) )
    { }

    BufferedValue(BufferedValue const & other) = default;
    BufferedValue& operator= (BufferedValue const & other) = default;
    BufferedValue(BufferedValue &&) = default;
    BufferedValue& operator= (BufferedValue &&) = default;


    Buffer * const backing_buffer() { return m_buf; }

    BufferedValue& backing_buffer(std::nullptr_t) & {
        m_buf = nullptr;
        return *this;
    }
    BufferedValue& backing_buffer(Buffer *const buf) & {
        m_buf = buf;
        return *this;
    }
    BufferedValue& backing_buffer(c_cstr buffer_name) & {
        m_buf = find_or_allocate_buffer(buffer_name);
        return *this;
    }

    BufferedValue&& backing_buffer(std::nullptr_t) && {
        m_buf = nullptr;
        return std::move(*this);
    }
    BufferedValue&& backing_buffer(Buffer *const buf) && {
        m_buf = buf;
        return std::move(*this);
    }
    BufferedValue&& backing_buffer(c_cstr buffer_name) && {
        m_buf = find_or_allocate_buffer(buffer_name);
        return std::move(*this);
    }


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
    inline T& operator*   () & { return value(); }
    inline    operator T& () & { return value(); }

    inline T const& value() const& {
        return *reinterpret_cast<T*>(m_buf->data);
    }
    inline T const& operator*         () const& { return value(); }
    inline          operator T const& () const& { return value(); }

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
