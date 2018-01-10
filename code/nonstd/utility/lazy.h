/** Lazy Object Initialization Wrapper Type
 *  =======================================
 *  Utility class for deffering initialization of tempermental classes.
 */
#pragma once

#include <nonstd/utility/optional_storage.h>

#include <functional>
#include <tuple>
#include <type_traits>


namespace nonstd {

/** Lazy Object Initializer
 *  -----------------------
 *  Created with the `lazy_init<T>` free function. Stores the arguments passed
 *  to it until dereferenced. Uses a direct placement-new to construct the
 *  given `T` upon first dereference.
 */
template <typename T>
class lazy {
private:
    using storage_type = std::remove_const_t<T>;

    nonstd::optional_storage<storage_type> m_storage;
    std::function<void()>                  m_init;

    template <typename TuplePtr_T, std::size_t ... Is>
    void initialize(TuplePtr_T arg_tuple_ptr, std::index_sequence<Is...>) {
        void * val_ptr = &m_storage.value;
        new (val_ptr) storage_type(std::get<Is>(std::move(*arg_tuple_ptr))...);
        m_storage.is_containing = true;
        delete arg_tuple_ptr;
    }

public:
    template <typename ... Args>
    lazy(Args&&... args)
        : m_storage ( )
        , m_init (
            [ this,
              args_tuple_ptr = new std::tuple<std::decay_t<Args>...> {
                std::forward<Args>(args)...
              }
            ] () {
                this->initialize(args_tuple_ptr,
                                 std::index_sequence_for<Args...>{});
            }
        )
    { }
    lazy(lazy const& other)       = delete;
    lazy(lazy && other)           = delete;
    lazy& operator= (lazy const&) = delete;
    lazy& operator= (lazy &&)     = delete;

    constexpr bool initialized() { return m_storage.is_containing; }

    T& operator * () {
        if (!initialized()) { m_init(); }
        return m_storage.value;
    }

    T* operator -> () {
        if (!initialized()) { m_init(); }
        return &m_storage.value;
    }
};

} /* namespace nonstd */
