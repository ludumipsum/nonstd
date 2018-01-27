#pragma once

#include <type_traits>

#include <nonstd/nonstd.h>
#include <nonstd/nonstdexcept.h>
#include <nonstd/type_traits_ext.h>
#include <nonstd/utility_ext.h>

#include "buffer.h"
#include "core_functions.h"


namespace nonstd {

template <typename T>
struct nr_ptr {
    static_assert(!std::is_reference_v<T>, "nr_ptrs cannot wrap reference types.");

private:
    using storage_type = std::remove_const_t<T>;

    Buffer * m_buf;

    inline Buffer * find_or_allocate_buffer(c_cstr buffer_name) {
        auto maybe_buffer = memory::find(buffer_name);
        return maybe_buffer
             ? *maybe_buffer
             : memory::allocate(buffer_name, sizeof(storage_type));
    }

    inline bool buffer_initialization_required(Buffer * const buf) {
        if (buf->type == Buffer::type_id::single_value) { return false; }

        N2BREAK_IF(buf->type != Buffer::type_id::raw,
                   N2Error::InvalidMemory,
                   "Buffer corruption detected by type_id; Attempting to "
                   "initialize a previously initialized Buffer. type_id is "
                   "currently 0x{:X}\n"
                   "Underlying buffer is named '{}', and it is located at {}.",
                   buf->type, buf->name, buf);

        buf->type = Buffer::type_id::single_value;
        return true;
    }

public:
    constexpr nr_ptr() noexcept
        : m_buf ( nullptr )
    { }
    constexpr nr_ptr(std::nullptr_t) noexcept
        : m_buf ( nullptr )
    { }

    template <typename ... Args>
    constexpr explicit nr_ptr(Buffer * buf, Args&& ... args) noexcept
        : m_buf ( buf )
    {
        if (buffer_initialization_required(m_buf)) {
            new ((void*)m_buf->data) storage_type {
                std::forward<Args>(args)...
            };
        }
    }
    template <typename ... Args>
    explicit nr_ptr(c_cstr buffer_name, Args&& ... args)
        : m_buf ( find_or_allocate_buffer(buffer_name) )
    {
        if (buffer_initialization_required(m_buf)) {
            new ((void*)m_buf->data) storage_type {
                std::forward<Args>(args)...
            };
        }
    }

    nr_ptr(nr_ptr const & other) = default;
    nr_ptr& operator= (nr_ptr const & other) = default;
    nr_ptr(nr_ptr &&) = default;
    nr_ptr& operator= (nr_ptr &&) = default;


    // Return the backing Buffer*.
    Buffer       * const backing_buffer()       & { return m_buf; }
    Buffer const * const backing_buffer() const & { return m_buf; }

    // Unset the backing Buffer*.
    nr_ptr& backing_buffer(std::nullptr_t) & {
        m_buf = nullptr;
        return *this;
    }
    nr_ptr&& backing_buffer(std::nullptr_t) && {
        m_buf = nullptr;
        return std::move(*this);
    }
    // Set the backing Buffer* directly.
    nr_ptr& backing_buffer(Buffer *const buf) & {
        m_buf = buf;
        if (buffer_initialization_required(m_buf)) {
            new ((void*)m_buf->data) storage_type { };
        }
        return *this;
    }
    nr_ptr&& backing_buffer(Buffer *const buf) && {
        m_buf = buf;
        if (buffer_initialization_required(m_buf)) {
            new ((void*)m_buf->data) storage_type { };
        }
        return std::move(*this);
    }
    // Set the backing Buffer* by name.
    nr_ptr& backing_buffer(c_cstr buffer_name) & {
        m_buf = find_or_allocate_buffer(buffer_name);
        if (buffer_initialization_required(m_buf)) {
            new ((void*)m_buf->data) storage_type { };
        }
        return *this;
    }
    nr_ptr&& backing_buffer(c_cstr buffer_name) && {
        m_buf = find_or_allocate_buffer(buffer_name);
        if (buffer_initialization_required(m_buf)) {
            new ((void*)m_buf->data) storage_type { };
        }
        return std::move(*this);
    }

    // Check if *this is storing a nullptr.
    inline explicit operator bool () const { return m_buf != nullptr; }

    // Return the stored pointer
    inline T       * get()       & { return reinterpret_cast<T*>(m_buf->data); }
    inline T const * get() const & { return reinterpret_cast<T*>(m_buf->data); }
    inline explicit operator T       * ()       & { return get(); }
    inline explicit operator T const * () const & { return get(); }

    // Dereference the stored pointer via member-of-pointer
    inline T       * operator -> ()       & { return get(); }
    inline T const * operator -> () const & { return get(); }

    // Dereference the stored pointer
    inline T       & value()       & { return *get(); }
    inline T const & value() const & { return *get(); }
    inline T       & operator* ()       & { return value(); }
    inline T const & operator* () const & { return value(); }
};

template<typename T, typename U>
inline bool operator== (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    return lhs.backing_buffer() == rhs.backing_buffer();
}
template<typename T, typename U>
inline bool operator!= (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    return !(lhs == rhs);
}
template<typename T, typename U>
inline bool operator<  (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    return std::less<Buffer * const>{}(lhs.backing_buffer(),
                                       rhs.backing_buffer());
}
template<typename T, typename U>
inline bool operator>  (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    return (rhs < lhs);
}
template<typename T, typename U>
inline bool operator<= (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    return !(rhs < lhs);
}
template<typename T, typename U>
inline bool operator>= (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    return !(lhs < rhs);
}

template<typename T>
inline bool operator== (nr_ptr<T> const & lhs, std::nullptr_t) noexcept {
    return !lhs;
}
template<typename T>
inline bool operator== (std::nullptr_t, nr_ptr<T> const & rhs) noexcept {
    return !rhs;
}
template<typename T>
inline bool operator!= (nr_ptr<T> const & lhs, std::nullptr_t) noexcept {
    return static_cast<bool>(lhs);
}
template<typename T>
inline bool operator!= (std::nullptr_t, nr_ptr<T> const & rhs) noexcept {
    return static_cast<bool>(rhs);
}
template<typename T>
inline bool operator<  (nr_ptr<T> const & lhs, std::nullptr_t) noexcept {
    return std::less<Buffer * const>{}(lhs.backing_buffer(), nullptr);
}
template<typename T>
inline bool operator<  (std::nullptr_t, nr_ptr<T> const & rhs) noexcept {
    return std::less<Buffer * const>{}(nullptr, rhs.backing_buffer());
}
template<typename T>
inline bool operator>  (nr_ptr<T> const & lhs, std::nullptr_t) noexcept {
    return (nullptr < lhs);
}
template<typename T>
inline bool operator>  (std::nullptr_t, nr_ptr<T> const & rhs) noexcept {
    return (rhs < nullptr);
}
template<typename T>
inline bool operator<= (nr_ptr<T> const & lhs, std::nullptr_t) noexcept {
    return !(nullptr < lhs);
}
template<typename T>
inline bool operator<= (std::nullptr_t, nr_ptr<T> const & rhs) noexcept {
    return !(rhs < nullptr);
}
template<typename T>
inline bool operator>= (nr_ptr<T> const & lhs, std::nullptr_t) noexcept {
    return !(lhs < nullptr);
}
template<typename T>
inline bool operator>= (std::nullptr_t, nr_ptr<T> const & rhs) noexcept {
    return !(nullptr < rhs);
}

} /* namespace nonstd */
