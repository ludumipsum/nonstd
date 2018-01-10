#pragma once

#include <initializer_list>
#include <type_traits>
#include <utility>

#include <nonstd/cpp1z/in_place_t.h>


namespace nonstd {

/** Optional Storage Class Template
 *  -------------------------------
 *  This utility class is designed to provide value-semantics for possibly
 *  uninitialized data. It wraps an anonymous union that contains either an
 *  `_empty` tag type, or a member of type `T`. This allows the initialization
 *  of an instance of `T` to be deferred, or skipped entirely.
 *
 *  This class is not designed to be used directly. User-facing utility classes
 *  like `optional<T>` -- that provide maybe-a-value semantics -- and `lazy<T>`
 *  -- that allows for transparent, deferred initialization -- should include
 *  this type as a member, and provide a more complete set of operations around
 *  the given concept.
 *
 *  Complete specializations of this template are kept as trivial and constexpr
 *  as possible through partial specialization on type traits of the wrapped
 *  type. If `T`s copy or move constructor is trivial, we're able to make the
 *  relative constructor of `optional_storage<T>` `constexpr`. If `T`s destructor
 *  is trivial, we can guarantee the destructor for `optional_storage<T>` is
 *  also trivial.
 *
 *  A NOTE ON THE TRIVIALITY OF CLASSES WITH DESTRUCTORS:
 *  Due to a bug in the language definition of `is_constructible` -- and
 *  therefore `is_nothrow_constructible` and `is_trivially_constructible` -- if
 *  a type's destructor is non-trivial, `is_trivially_constructible` -- and
 *  `is_trivially_copy_constructible` and `is_trivially_move_constructible` --
 *  will resolve to `false`. There is an open bug in the C++ Library Working
 *  Group to track this defect, but there has been little (to no) activity since
 *  2011. As such, this implementation skips three otherwise valid partial
 *  specializations.
 *  http://cplusplus.github.io/LWG/lwg-active.html#2116
 */
template < typename T
         , bool TrivialCopyCtor = std::is_trivially_copy_constructible_v<T>
         , bool TrivialMoveCtor = std::is_trivially_move_constructible_v<T>
         , bool TrivialDtor     = std::is_trivially_destructible_v<T> >
class optional_storage;


/** Optional Storage -- Fully Trivial Types
 *  ---------------------------------------
 *  For types that are fully trivial, the implicitly defined special member
 *  functions will be correct in all cases; the active member of the anonymous
 *  union will be determined (at compile time, if possible) and used to
 *  initialize the relevant member in a new `optional_storage` object.
 */
template <typename T>
class optional_storage<T, /* TrivialCopyCtor */ true,
                          /* TrivialMoveCtor */ true,
                          /* TrivialDtor */     true>
{
    static_assert(!std::is_same_v<T, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

public:
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    // NB. These five constructors will be identical for all specializations.
    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

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

    // NB. These three members will vary per partial specialization.
    constexpr optional_storage(optional_storage const &) noexcept = default;
    constexpr optional_storage(optional_storage &&) noexcept = default;
    ~optional_storage() noexcept = default;
};

/** Optional Storage -- Non-Trivially Copy Ctor
 *  -------------------------------------------
 *  For types that are not trivially copy constructible or not trivially move
 *  constructible, the compiler will be unable to implicitly define the relevant
 *  special member function. In these cases we need to explicitly define the
 *  relevant member. Additionally, we may have to wait until runtime to
 *  construct a contained value, as placement new must be used to correctly
 *  change the active member of an anonymous union.
 *
 *  Interestingly, we can leave the copy and move constructors `constexpr`, as
 *  the compiler will be able to skip the `_construct` call if `rhs` is
 *  non-containing. I don't know if a compile-time, non-containing,
 *  non-trivially constructible optional will be of use, but... You can do it!
 */
template <typename T>
class optional_storage<T, /* TrivialCopyCtor */ false,
                          /* TrivialMoveCtor */ true,
                          /* TrivialDtor */     true>
{
    static_assert(!std::is_same_v<T, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

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
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    // NB. These five constructors will be identical for all specializations.
    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

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

    // NB. These three members will vary per partial specialization.
    constexpr optional_storage(optional_storage const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( false )
        , empty         (       )
    {
        if (rhs.is_containing) { _construct(rhs.value); }
    }
    constexpr optional_storage(optional_storage &&) noexcept = default;
    ~optional_storage() noexcept = default;
};

/** Optional Storage -- Non-Trivially Move Ctor
 *  -------------------------------------------
 *  See the discussion for 'Optional Storage -- Non-Trivially Copy Ctor'.
 */
template <typename T>
class optional_storage<T, /* TrivialCopyCtor */ true,
                          /* TrivialMoveCtor */ false,
                          /* TrivialDtor */     true>
{
    static_assert(!std::is_same_v<T, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

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
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    // NB. These five constructors will be identical for all specializations.
    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

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

    // NB. These three members will vary per partial specialization.
    constexpr optional_storage(optional_storage const &) noexcept = default;
    constexpr optional_storage(optional_storage && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( false )
        , empty         (       )
    {
        if (rhs.is_containing) { _construct(std::move(rhs.value)); }
    }
    ~optional_storage() noexcept = default;
};

/** Optional Storage -- Non-Trivially Copy & Move Ctors
 *  ---------------------------------------------------
 *  See the discussion for 'Optional Storage -- Non-Trivially Copy Ctor'.
 */
template <typename T>
class optional_storage<T, /* TrivialCopyCtor */ false,
                          /* TrivialMoveCtor */ false,
                          /* TrivialDtor */     true>
{
    static_assert(!std::is_same_v<T, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

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
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    // NB. These five constructors will be identical for all specializations.
    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

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


    // NB. These three members will vary per partial specialization.
    constexpr optional_storage(optional_storage const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( false )
        , empty         (       )
    {
        if (rhs.is_containing) { _construct(rhs.value); }
    }
    constexpr optional_storage(optional_storage && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( false )
        , empty         (       )
    {
        if (rhs.is_containing) { _construct(std::move(rhs.value)); }
    }
    ~optional_storage() noexcept = default;
};

/** Optional Storage -- Non-Trivially Dtor
 *  --------------------------------------
 *  For types that are not trivial destructible, the compiler will be unable to
 *  implicitly define a destructor, and so we need to explicitly define one that
 *  explicitly calls the destructor on the contained value (if there is a
 *  contained value).
 */
template <typename T>
class optional_storage<T, /* TrivialCopyCtor */ false,
                          /* TrivialMoveCtor */ false,
                          /* TrivialDtor */     false>
{
    static_assert(!std::is_same_v<T, nonstd::in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");

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
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    // NB. These five constructors will be identical for all specializations.
    constexpr optional_storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

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


    // NB. These three members will vary per partial specialization.
    constexpr optional_storage(optional_storage const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( false )
        , empty         (       )
    {
        if (rhs.is_containing) { _construct(rhs.value); }
    }
    constexpr optional_storage(optional_storage && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( false )
        , empty         (       )
    {
        if (rhs.is_containing) { _construct(std::move(rhs.value)); }
    }
    /** Non-Trivial Storage Destruction
     *  §23.6.3.2 of the c++ n4713 specification.
     */
    ~optional_storage()
    noexcept(std::is_nothrow_destructible_v<T>) {
        if (is_containing) { value.~T(); }
    }
};

} /* namespace nonstd */
