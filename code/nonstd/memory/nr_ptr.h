#pragma once

#include <nonstd/core/break.h>
#include <nonstd/core/primitive_types.h>
#include <nonstd/cpp1z/type_trait_assertions.h>
#include <nonstd/std_ish/compare.h>
#include <nonstd/utility/optional.h>

#include <string>

#include "buffer.h"
#include "core_functions.h"


namespace nonstd {

template <typename T>
struct nr_ptr {
    static_assert(!IS_REFERENCE(T), "nr_ptrs cannot wrap reference types.");

private:
    Buffer * m_buf;
    nonstd::Optional<std::string> m_name;

    inline Buffer * find_or_allocate_buffer(c_cstr buffer_name) {
        auto maybe_buffer = memory::find(buffer_name);
        return maybe_buffer
             ? *maybe_buffer
             : memory::allocate(buffer_name, sizeof(T));
    }

    inline void verify_buffer_type(Buffer * const buf) {
        if (buf->type == Buffer::type_id::single_value) { return; }

        N2BREAK_IF(buf->type != Buffer::type_id::raw,
                   N2Error::InvalidMemory,
                   "Buffer corruption detected by type_id; Attempting to "
                   "initialize a previously initialized Buffer. type_id is "
                   "currently 0x{:X}\n"
                   "Underlying buffer is named '{}', and it is located at {}.",
                   buf->type, buf->name, buf);

        buf->type = Buffer::type_id::single_value;
    }

public:
    constexpr nr_ptr() noexcept
        : m_buf  ( nullptr )
        , m_name ( )
    { }
    constexpr nr_ptr(std::nullptr_t) noexcept
        : m_buf  ( nullptr )
        , m_name ( )
    { }

    constexpr explicit nr_ptr(Buffer * buf) noexcept
        : m_buf  ( buf       )
        , m_name ( buf->name )
    {
        verify_buffer_type(m_buf);
    }
    explicit nr_ptr(c_cstr buffer_name)
        : m_buf  ( nullptr )
        , m_name ( {buffer_name} )
    { }

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
        m_name = {};
        return *this;
    }
    nr_ptr&& backing_buffer(std::nullptr_t) && {
        m_buf = nullptr;
        m_name = {};
        return std::move(*this);
    }
    // Set the backing Buffer* directly.
    nr_ptr& backing_buffer(Buffer *const buf) & {
        m_buf = buf;
        m_name = buf->name;
        verify_buffer_type(m_buf);
        return *this;
    }
    nr_ptr&& backing_buffer(Buffer *const buf) && {
        m_buf = buf;
        m_name = buf->name;
        verify_buffer_type(m_buf);
        return std::move(*this);
    }
    // Set the backing Buffer* by name.
    nr_ptr& backing_buffer(c_cstr buffer_name) & {
        m_buf = find_or_allocate_buffer(buffer_name);
        m_name = buffer_name;
        verify_buffer_type(m_buf);
        return *this;
    }
    nr_ptr&& backing_buffer(c_cstr buffer_name) && {
        m_buf = find_or_allocate_buffer(buffer_name);
        m_name = buffer_name;
        verify_buffer_type(m_buf);
        return std::move(*this);
    }

    // Check if *this is valid.
    inline explicit operator bool () const noexcept {
        return (bool)m_name;
    }
    // Check if pointer has already been lazy-laoded
    inline bool is_loaded() const noexcept {
        return (bool)m_name && backing_buffer() != nullptr;
    }

    // Return the stored pointer
    inline T * get() & {
        // Handle lazy static init-by-name: if we have a valid name set, but no
        // member pointer, this indicates a lazy-load that hasn't yet been
        // run, so we should run it now.
        if ((bool)m_name && m_buf == nullptr) {
            m_buf = find_or_allocate_buffer(m_name->c_str());
        }
        return reinterpret_cast<T*>(m_buf->data);
    }
    inline explicit operator T * () { return get(); }

    // Dereference the stored pointer via member-of-pointer
    inline T * operator -> () { return get(); }

    // Dereference the stored pointer
    inline T & value() { return *get(); }
    inline T & operator* () { return value(); }

    // Get the stored name
    inline std::string const& name() const { return *m_name; }
};

// Direct equality comparisons
template<typename T, typename U>
inline bool operator== (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    // If both pointers are valid and already lazily initialized, we can do
    // the equality comparison on the pointer values instead of the names
    if (lhs.is_loaded() && rhs.is_loaded()) {
        return lhs.backing_buffer() == rhs.backing_buffer();
    }
    auto const & lh_comp = lhs ? lhs.name() : "";
    auto const & rh_comp = rhs ? rhs.name() : "";
    return lh_comp == rh_comp;
}
template<typename T, typename U>
inline bool operator!= (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    return !(lhs == rhs);
}

// Direct sorting comparators
template<typename T, typename U>
inline bool operator<  (nr_ptr<T> const & lhs, nr_ptr<U> const & rhs) noexcept {
    // Since we lazy-load the pointer value, we can't stably use it as the
    // comparison operand. Instead we have to use the name -- any time the
    // nr_ptr is valid, it will have a valid name, and when it doesn't, we
    // take the shitty shortcut of using the empty string.
    auto const & lh_comp = lhs ? lhs.name() : "";
    auto const & rh_comp = rhs ? rhs.name() : "";
    return std::less<std::string const&>{}(lh_comp, rh_comp);
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

// Nullptr equality comparisons
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

// Nullptr sorting comparators
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
