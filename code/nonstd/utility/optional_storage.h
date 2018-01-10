#pragma once

#include <type_traits>

#include <nonstd/cpp1z/in_place_t.h>


namespace nonstd {

/** Optional Storage Class Template
 *  -------------------------------
 *  This utility class is designed to provide value-semantics for possibly
 *  uninitialized data.
 *  It wraps an anonymous union that contains a (default) `empty` tag, and
 *  member of type `T`. This allows the initialization of T to be deferred, or
 *  skipped entirely.
 *  This class is not designed to be used directly. User-facing utility classes
 *  like `optional<T>` -- that provides maybe-a-value semantics -- and `lazy<T>`
 *  -- that allows for transparent, deferred initialization -- should include
 *  this type as a member, and provide a more complete set of operations.
 */
template < typename T
         , bool MoveAndCopyCtorsAreTrivial = std::is_trivially_copy_constructible_v<T>
                                          && std::is_trivially_move_constructible_v<T>
         , bool DestructorIsTrivial = std::is_trivially_destructible_v<T> >
class optional_storage;


/** Storage for types with Trivial Construction and Trivial Destruction
 *  -------------------------------------------------------------------
 *  For types that are trivially copy and trivially move constructible, the
 *  default copy/move ctors will be correct in all cases; the active
 *  member of the union will be determined (at compile time, if possible) and
 *  used to initialize the relevant member in a new `optional_storage` object.
 */
template <typename T>
class optional_storage<T, /* MoveAndCopyCtorsAreTrivial */ true,
                          /* DestructorIsTrivial        */ true>
{
    static_assert(!std::is_same_v<T, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

private:
    struct _Empty { };

public:
    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

    constexpr optional_storage(optional_storage const &) = default;
    constexpr optional_storage(optional_storage &&     ) = default;

    constexpr optional_storage(T const & value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( true  )
        , value         ( value )
    { }
    constexpr optional_storage(T && value)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( true             )
        , value         ( std::move(value) )
    { }

    template <typename ... Args>
    constexpr explicit optional_storage(nonstd::in_place_t,
                                        Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... )
    { }

    template <typename Il, typename ... Args>
    constexpr explicit optional_storage(nonstd::in_place_t,
                                        std::initializer_list<Il> il,
                                        Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T,
                                             std::initializer_list<Il>,
                                             Args && ...>)
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... )
    { }
};


/** Storage for types with Non-Trivial Construction & Trivial Destruction
 *  ---------------------------------------------------------------------
 *  When storing a type that is not trivially copy constructible or not
 *  trivially move constructible, the compiler will be unable to implicitly
 *  define the relevant special member function. In these cases we need to
 *  explicitly define the relevant member. Additionally, we may have to wait
 *  till runtime to construct a contained value, as placement new is needed in
 *  order to correctly change the active member of an anonymous union.
 *
 *  Interestingly, we can leave the copy and move constructors `constexpr`, as
 *  the compiler will be able to skip the `_construct` call if `rhs` is
 *  non-containing. I don't know if a compile-time, non-containing,
 *  non-trivially constructible optional will be of use, but... You can do it!
 */
template <typename T>
class optional_storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                          /* DestructorIsTrivial        */ true>
{
    static_assert(!std::is_same_v<std::decay_t<T>, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

private:
    struct _Empty { };

public:
    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

private:
    template <typename ... Args>
    void _construct(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        // If the anonymous union was initialized to `empty`, we need to use
        // placement new to guarantee the active member is switched. If there's
        // any ambiguity in which member is active, or if construction needs to
        // be deferred, use this method to construct.
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }

public:
    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

    constexpr optional_storage(optional_storage const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(rhs.value); }
    }
    constexpr optional_storage(optional_storage && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(std::move(rhs.value)); }
    }

    constexpr optional_storage(T const & value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( true  )
        , value         ( value )
    { }
    constexpr optional_storage(T && value)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( true             )
        , value         ( std::move(value) )
    { }

    template <typename ... Args>
    constexpr explicit optional_storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... )
    { }

    template <typename Il, typename ... Args>
    constexpr explicit optional_storage(nonstd::in_place_t,
                                         std::initializer_list<Il> il,
                                         Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T,
                                             std::initializer_list<Il>,
                                             Args && ...>)
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... )
    { }
};


/** Storage for types with Non-Trivial Construction & Non Trivial-Destruction
 *  -------------------------------------------------------------------------
 *  When storing a type that is not trivial destructible, the compiler will be
 *  unable to implicitly define a destructor, and so we need to explicitly
 *  create one that explicitly calls the destructor on the contained value.
 */
template <typename T>
class optional_storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                          /* DestructorIsTrivial        */ false>
{
    static_assert(!std::is_same_v<std::decay_t<T>, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

private:
    struct _Empty { };

public:
    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

private:
    template <typename ... Args>
    void _construct(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
        // If the anonymous union was initialized to `empty`, we need to use
        // placement new to guarantee the active member is switched. If there's
        // any ambiguity in which member is active, or if construction needs to
        // be deferred, use this method to construct.
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }

public:
    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

    constexpr optional_storage(optional_storage const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(rhs.value); }
    }
    constexpr optional_storage(optional_storage && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(std::move(rhs.value)); }
    }

    constexpr optional_storage(T const & value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( true  )
        , value         ( value )
    { }
    constexpr optional_storage(T && value)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( true             )
        , value         ( std::move(value) )
    { }

    template <typename ... Args>
    constexpr explicit optional_storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... )
    { }

    template <typename Il, typename ... Args>
    constexpr explicit optional_storage(nonstd::in_place_t,
                                         std::initializer_list<Il> il,
                                         Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T,
                                             std::initializer_list<Il>,
                                             Args && ...>)
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... )
    { }

    /** Non-Trivial Storage Destruction
     *  -------------------------------
     *  §23.6.3.2 of the c++ n4713 specification.
     *  We only ever need to call this if `std::is_trivially_destructible_v<T>`
     *  is `false`, and if there is a contained value.
     */
    ~optional_storage()
    noexcept(std::is_nothrow_destructible_v<T>) {
        if (is_containing) { value.~T(); }
    }
};



} /* namespace nonstd */
