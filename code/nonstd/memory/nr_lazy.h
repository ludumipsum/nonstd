#pragma once

#include "nr_ptr.h"
#include <nonstd/lazy.h>


namespace nonstd {

template <typename T>
class nr_lazy {
private:
    lazy<nr_ptr<T>> m_lazy;

public:
    template <typename ... Args>
    nr_lazy(c_cstr buffer_name, Args&& ... args)
        : m_lazy ( buffer_name, std::forward<Args>(args)... )
    { }

    nr_lazy()                           = delete;
    nr_lazy(nr_lazy const& other)       = delete;
    nr_lazy(nr_lazy && other)           = delete;
    nr_lazy& operator= (nr_lazy const&) = delete;
    nr_lazy& operator= (nr_lazy &&)     = delete;


    inline explicit operator bool () { return (bool)(*m_lazy); }
    inline bool initialized() const { return m_lazy.initialized(); }

    // Return the stored pointer
    inline T* get() { return (*m_lazy).get(); }
    inline explicit operator T* () { return get(); }

    // Dereference the stored pointer via member-of-pointer
    inline T* operator -> () { return get(); }

    // Dereference the stored pointer
    inline T& value() { return **m_lazy; }
    inline T& operator* () { return value(); }
};



template<typename T, typename U>
inline bool operator== (nr_lazy<T> const & lhs, nr_lazy<U> const & rhs) noexcept {
    return (*lhs).backing_buffer() == (*rhs).backing_buffer();
}
template<typename T, typename U>
inline bool operator!= (nr_lazy<T> const & lhs, nr_lazy<U> const & rhs) noexcept {
    return !((*lhs) == (*rhs));
}
template<typename T, typename U>
inline bool operator<  (nr_lazy<T> const & lhs, nr_lazy<U> const & rhs) noexcept {
    return std::less<Buffer * const>{}((*lhs).backing_buffer(),
                                       (*rhs).backing_buffer());
}
template<typename T, typename U>
inline bool operator>  (nr_lazy<T> const & lhs, nr_lazy<U> const & rhs) noexcept {
    return (rhs < lhs);
}
template<typename T, typename U>
inline bool operator<= (nr_lazy<T> const & lhs, nr_lazy<U> const & rhs) noexcept {
    return !(rhs < lhs);
}
template<typename T, typename U>
inline bool operator>= (nr_lazy<T> const & lhs, nr_lazy<U> const & rhs) noexcept {
    return !(lhs < rhs);
}

template<typename T>
inline bool operator== (nr_lazy<T> const & lhs, std::nullptr_t) noexcept {
    return !lhs;
}
template<typename T>
inline bool operator== (std::nullptr_t, nr_lazy<T> const & rhs) noexcept {
    return !rhs;
}
template<typename T>
inline bool operator!= (nr_lazy<T> const & lhs, std::nullptr_t) noexcept {
    return static_cast<bool>(lhs);
}
template<typename T>
inline bool operator!= (std::nullptr_t, nr_lazy<T> const & rhs) noexcept {
    return static_cast<bool>(rhs);
}
template<typename T>
inline bool operator<  (nr_lazy<T> const & lhs, std::nullptr_t) noexcept {
    return std::less<Buffer * const>{}((*lhs).backing_buffer(), nullptr);
}
template<typename T>
inline bool operator<  (std::nullptr_t, nr_lazy<T> const & rhs) noexcept {
    return std::less<Buffer * const>{}(nullptr, (*rhs).backing_buffer());
}
template<typename T>
inline bool operator>  (nr_lazy<T> const & lhs, std::nullptr_t) noexcept {
    return (nullptr < (*lhs));
}
template<typename T>
inline bool operator>  (std::nullptr_t, nr_lazy<T> const & rhs) noexcept {
    return ((*rhs) < nullptr);
}
template<typename T>
inline bool operator<= (nr_lazy<T> const & lhs, std::nullptr_t) noexcept {
    return !(nullptr < (*lhs));
}
template<typename T>
inline bool operator<= (std::nullptr_t, nr_lazy<T> const & rhs) noexcept {
    return !((*rhs) < nullptr);
}
template<typename T>
inline bool operator>= (nr_lazy<T> const & lhs, std::nullptr_t) noexcept {
    return !((*lhs) < nullptr);
}
template<typename T>
inline bool operator>= (std::nullptr_t, nr_lazy<T> const & rhs) noexcept {
    return !(nullptr < (*rhs));
}

} /* namespace nonstd */
