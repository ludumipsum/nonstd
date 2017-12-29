/** Lazy Initialization Wrapper Type
 *  ================================
 *  Utility class for deffering initialization of tempermental classes.
 */
#pragma once

#include "optional.h"

#include <functional>
#include <tuple>
#include <type_traits>


namespace nonstd {

/** Lazy Initializer
 *  ----------------
 *  Created with the `lazy_init<T>` free function. Stores the arguments passed
 *  to it until dereferenced. Uses a direct placement-new to construct the
 *  given `T` upon first dereference.
 */
template <typename T, typename ... Args>
class lazy_initializer {
    using storage_type = std::remove_const_t<T>;

    _Optional_Storage<storage_type>   m_storage;
    std::tuple<std::decay_t<Args>...> m_args;

    template<std::size_t ... Is>
    void initialize(std::index_sequence<Is...>) {
        void * val_ptr = &m_storage.value;
        new (val_ptr) storage_type(std::get<Is>(std::move(m_args))...);
        m_storage.is_containing = true;
    }

public:
    lazy_initializer(Args&&... args)
        : m_storage ( )
        , m_args    ( std::forward<Args>(args)... )
    { }
    lazy_initializer(lazy_initializer & other)
        : m_storage ( other.m_storage )
        , m_args    ( other.m_args    )
    { }
    lazy_initializer(lazy_initializer && other)
        : m_storage ( std::move(other.m_storage) )
        , m_args    ( std::move(other.m_args)    )
    { }

    bool initialized() { return m_storage.is_containing; }

    T& operator* () {
        if (!initialized()) { initialize(std::index_sequence_for<Args...>{}); }
        return m_storage.value;
    }
};


/** Lazily Initialize Free Function
 *  -------------------------------
 *  Takes and explicit type as a template parametr to construct, and an
 *  arbitrary set of objects as arguments to use in the construction. Returns an
 *  explicitly typed `lazy_initializer`.
 */
template <typename T, typename ... Args>
lazy_initializer<T, Args&&...> make_lazy(Args&&... args) {
    return { std::forward<Args>(args)... };
}

} /* namespace nonstd */



