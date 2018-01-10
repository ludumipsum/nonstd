/** Optional Type
 *  =============
 *  Utility type for representing maybe-a-value.
 *
 *  This implementation should strictly adhere to the C++ specification n4713;
 *  https://github.com/cplusplus/draft/blob/master/papers/n4713.pdf
 *  For ease of navigation, the 2018-01-05 publication of eel.is was used;
 *  http://eel.is/c++draft/
 *
 *  All section references (§) refer to the above documents.
 *
 *  //TODO: Update the below.
 *
 *  The structure of these optionals is also worth discussing. They are
 *  universally composed of three parts;
 *   - The dedicated Storage component.
 *     Contains the wrapped value (or not), and the `is_containing` boolean.
 *   - The storage-aware Base class.
 *     Contains a Storage component, and understands how to interact with the
 *     value and boolean stored therein.
 *   - The complete `optional<T>`.
 *     Inherits from the Base class, but remains unaware of the Storage
 *     component. May also inherit from classes that prevent copy and move
 *     construction or assignment.
 *
 *  See the class descriptors in the Forward Declarations section below for
 *  additional details.
 *
 *  NB. No specialization on swap was added because `std::is_swappabale<T>` has
 *  not landed in Apple Clang (at the time of writing), and because the std::
 *  namespace should not be modified by user code. This is also the reason no
 *  specialization on hash:: was added.
 *
 *  Author's Note: This file really does try to maintain an 80 column maximum,
 *  but... it fails. Sometimes pretty badly. It is with great shame that I
 *  apologize for that failure, but it is honestly less painful that forcing
 *  these template monstrosities into 80 characters.
 */

#pragma once

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <type_traits>

#include <thirdparty/fmt.h>

#include <nonstd/cpp1z/in_place_t.h>
#include <nonstd/cpp1z/type_traits_ext.h>
#include <nonstd/cpp1z/special_member_filters.h>
#include <nonstd/cpp1z/valid_expression_tester.h>
#include <nonstd/c_ish/type_name.h>
#include <nonstd/std_ish/compare.h>


/** Utilities
 *  ============================================================================
 */
namespace nonstd {

/** Null Optional Type -- §23.6.4
 *  -----------------------------
 *  The struct `nullopt_­t` is an empty structure type used as a unique type to
 *  indicate the state of not containing a value for optional objects. In
 *  particular, `optional<T>` has a constructor with `nullopt_­t` as a single
 *  argument; this indicates that an optional object not containing a value
 *  shall be constructed.
 *  Namespace'd to `nonstd::` to match the C++17 `std::nullopt_t`.
 */
struct nullopt_t {
    // Explicitly delete the default constructor, and define an explicit
    // constexpr constructor that will be hard to accidentally invoke.
    // Let's use the `nonstd::in_place_t` tag for that.
    nullopt_t() = delete;
    constexpr explicit nullopt_t(nonstd::in_place_t) noexcept { }
};
inline constexpr nullopt_t nullopt { nonstd::in_place };


namespace exception {

/** Bad Optional Access Exception -- §23.6.5
 *  ----------------------------------------
 *  The class `bad_­optional_­access` defines the type of objects thrown as
 *  exceptions to report the situation where an attempt is made to access the
 *  value of an optional object that does not contain a value.
 *  NB. We inherit from `std::logic_error` -- rather than `std::exception`, as
 *  specified -- because it simplifies the construction of the exception object.
 */
class bad_optional_access : public std::logic_error {
public:
    bad_optional_access()
        : std::logic_error ( "Attempted to access the value of an optional "
                             "that does not contain a value." )
    { }
};

} /* namespace exception */


/** Forward Declarations
 *  ============================================================================
 *  Because we have a _lot_ of classes to define and specialize in this file.
 */

/** Optional Class Template -- §23.6.3
 *  ----------------------------------
 *  The end product. The Maybe-A-Value type.
 *  This class will inherit from one of the `_optional_*_base` classes; the
 *  primary template will inherit from `_optional_val_base`, a partial
 *  specialization on `optional<T&>` will inherit from `_optional_ref_base`.
 *  It will also inherit from the `Enable[Copy/Move][Ctor/Assign]If<B,T>` helper
 *  classes to conditionally "delete" the relevant special member function based
 *  on the salient type_traits of `T`.
 */
template <typename T>
class optional;


/** Optional Base-Class Templates -- nonstd
 *  ---------------------------------------
 *  The primary base-classes of the complete `optional<T>` type. These classes
 *  will contain all necessary storage -- by including an `optional_storage` as
 *  a member variable -- and construction logic that may be used by the complete
 *  `optional<T>` type.
 */
template <typename T>
class _optional_val_base;

template <typename T>
class _optional_ref_base;


/** Optional Storage Class Templates -- nonstd
 *  -------------------------------------------
 *  A simple wrapper that will directly implement the construction and reseating
 *  of stored values.
 */
//TODO: This class is a product in itself; It should be moved to its own file.
template < typename T
         , bool MoveAndCopyCtorsAreTrivial = std::is_trivially_copy_constructible_v<T>
                                          && std::is_trivially_move_constructible_v<T>
         , bool DestructorIsTrivial = std::is_trivially_destructible_v<T> >
class optional_storage;

//TODO: I don't think I need the lvalref storage class. Should be able to use
//      the optional_storage directly, and get the most trivial version.
template <typename T>
class _optional_lvalref_storage;


/** Helpers
 *  ============================================================================
 *  These are just here to save space, because holy crap these enable_if SFINAE
 *  clauses get verbose...
 */
template <typename T, typename U>
struct is_convertible_from_optional
    : public std::bool_constant<   std::is_constructible_v<T, optional<U> &>
                                || std::is_constructible_v<T, optional<U> &&>
                                || std::is_constructible_v<T, optional<U> const &>
                                || std::is_constructible_v<T, optional<U> const &&>
                                || std::is_convertible_v<optional<U> &, T>
                                || std::is_convertible_v<optional<U> &&, T>
                                || std::is_convertible_v<const optional<U> &, T>
                                || std::is_convertible_v<const optional<U> &&, T> >
{ };
template <typename T, typename U>
inline constexpr bool is_convertible_from_optional_v =
    is_convertible_from_optional<T,U>::value;

template <typename U, typename T>
struct is_assignable_from_optional
    : public std::bool_constant<   std::is_assignable_v<T&, optional<U> &>
                                || std::is_assignable_v<T&, optional<U> &&>
                                || std::is_assignable_v<T&, optional<U> const &>
                                || std::is_assignable_v<T&, optional<U> const &&> >
{ };
template <typename U, typename T>
inline constexpr bool is_assignable_from_optional_v =
    is_assignable_from_optional<T,U>::value;


/** Optional Storage Class
 *  ============================================================================
 *  //TODO: These comments are going to be wrong soon. Be ready to rewrite them.
 *
 *  Wrapper around the `is_containing` boolean, and the value-or-no-value union.
 *  We need to give optionals their own storage class s.t. SFINAE can be used to
 *  selectively define constructors and destructors based on `T`.
 *
 *  When storing an object that is trivially copy and move constructible, the
 *  implicitly defined copy/move ctors will be correct in all cases; the active
 *  member of the union will be determined (at compile time, if possible) and
 *  used to initialize the relevant member in the new _storage object. Otherwise
 *  the implicit copy/move ctors will not be defined, which forces us to
 *  explicitly define them. In these cases, we also need to wait till run-time
 *  to (optionally) construct a value based on the `rhs`.
 *
 *  When storing a type that is not trivial destructible, the _storage object's
 *  implicitly defined destructor won't be defined, and so needs to be
 *  explicitly created (and needs to explicitly call the destructor on the
 *  stored value). When storing a type that is trivially destructible, the
 *  implicitly defined destructor will be defined, will correctly clean up the
 *  stored value, and will match the constexpr-ness of the stored type.
 *
 *  The _lvalref_storage is _dramatically_ simpler than its counterpart, as it
 *  requires no in-place construction, and never requires a destructor (it will
 *  be storing raw pointers, and raw pointers don't need to be automatically
 *  cleaned up). We use it primarily to maintain architecture parity between
 *  value- and reference-wrapping optionals.
 */

/** Storage for types with trivial construction and trivial destruction
 *  -------------------------------------------------------------------
 */
template <typename T>
class optional_storage<T, /* MoveAndCopyCtorsAreTrivial */ true,
                          /* DestructorIsTrivial        */ true>
{
    static_assert(!std::is_same_v<T, in_place_t>,
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


/** Storage for types with non-trivial construction & trivial destruction
 *  ---------------------------------------------------------------------
 *  Note: Interestingly, we can leave the copy and move constructors
 *  `constexpr`, as the compiler will be able to skip the `_construct` call if
 *  `rhs` is non-containing. I don't know if a compile-time, non-containing,
 *  non-trivially constructible optional will be of use, but... You can do it!
 */
template <typename T>
class optional_storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                          /* DestructorIsTrivial        */ true>
{
    static_assert(!std::is_same_v<std::decay_t<T>, in_place_t>,
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
    // If the anonymous union was initialized to `empty`, we need to use
    // placement new to guarantee the active member is switched. Any there's
    // ambiguity in which member is active, or construction needs to be
    // deferred, use this method to construct.
    template <typename ... Args>
    void _construct(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args...>) {
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


/** Storage for types with non-trivial construction & non-trivial destruction
 *  -------------------------------------------------------------------------
 */
template <typename T>
class optional_storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                          /* DestructorIsTrivial        */ false>
{
    static_assert(!std::is_same_v<std::decay_t<T>, in_place_t>,
        "optional_storage objects cannot wrap `in_place_t`.");
    static_assert(!std::is_same_v<std::decay_t<T>, nullopt_t>,
        "optional_storage objects cannot wrap `nullopt_t`.");

private:
    struct _Empty { };

public:
    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

private:
    // If the anonymous union was initialized to `empty`, we need to use
    // placement new to guarantee the active member is switched. Any there's
    // ambiguity in which member is active, or construction needs to be
    // deferred, use this method to construct.
    template <typename ... Args>
    void _construct(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
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

    /** Non-Trivial Storage Destruction -- §23.6.3.2
     *  --------------------------------------------
     *  We only ever need to call this if `std::is_trivially_destructible_v<T>`
     *  is `false`, and if there is a contained value. In other words, only in
     *  this class, and iff `is_containing` is `true`.
     */
    ~optional_storage()
    noexcept(std::is_nothrow_destructible_v<T>) {
        if (is_containing) { value.~T(); }
    }
};


/** Storage for LValue Reference types
 *  ----------------------------------
 */
template <typename T>
class _optional_lvalref_storage {
    static_assert(std::is_pointer_v<T>,
        "_optional_lvalref_storage expects to be specialized on a pointer type");
    static_assert(!std::is_same_v<std::remove_cv_t<T>, in_place_t*>,
        "_optional_lvalref_storage objects cannot wrap `in_place_t *`.");

public:
    bool is_containing;
    T    value;

    constexpr _optional_lvalref_storage() noexcept
        : is_containing ( false   )
        , value         ( nullptr )
    { }

    constexpr _optional_lvalref_storage(_optional_lvalref_storage const & rhs)
    noexcept
        : is_containing ( rhs.is_containing )
        , value         ( rhs.value         )
    { }

    constexpr _optional_lvalref_storage(_optional_lvalref_storage && rhs)
    noexcept
        : is_containing ( rhs.is_containing    )
        , value         ( std::move(rhs.value) )
    { }

    constexpr _optional_lvalref_storage(T value) noexcept
        : is_containing ( true  )
        , value         ( value )
    { }
};



/** Optional Base-Class; Construction Logic and Storage for Value Types
 *  ===================================================================
 */

template <typename T>
class _optional_val_base {
private:
    /** A Note on Storage Types
     *  To let us use (and re-use) storage when `T` is cv qualified, we strip
     *  any const-ness when defining our `optional_storage` type. All public and
     *  protected methods will take and return `T`s, though, so the removal of
     *  the cv-qualifier should be entirely invisible. In fact, most methods in
     *  this class should call `_get_value` rather than directly accessing
     *  `_storage.value` so that they access a `T`, rather than a
     *  `std::remove_const_t<T>`.
     */
    using storage_type = std::remove_const_t<T>;
    optional_storage<storage_type> _storage;

    // `optionals` must all be friends to one another.
    template <typename U> friend class _optional_val_base;

public:
    using value_type = T;

    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     */
    constexpr _optional_val_base() noexcept
        : _storage ( )
    { }
    constexpr _optional_val_base(nonstd::nullopt_t /*unused*/) noexcept
        : _storage ( )
    { }

    /** Copy Ctor -- §23.6.3.1.3
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `*rhs`.
     *  If `std::is_copy_constructible_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableCopyCtorIf`, which `optional<T>`
     *  is an inheritor of.
     *  This constructor will only be (successfully) `constexpr` if `T` is
     *  trivially copy constructible; `optional_storage` will only have a
     *  `constexpr` copy constructor if `is_trivially_copy_constructible_v<T>`
     *  is `true`.
     */
    constexpr _optional_val_base(_optional_val_base const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : _storage ( rhs._storage )
    { }

    /** Move Ctor -- §23.6.3.1.7
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with
     *  `std::move(*rhs)`. **This will not change `bool(rhs)`**.
     *  If `std::is_move_constructible_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableMoveCtorIf`, which `optional<T>`
     *  is an inheritor of.
     *  This constructor will only be (successfully) `constexpr` if `T` is
     *  trivially move constructible; `optional_storage` will only have a
     *  `constexpr` move constructor if `is_trivially_move_constructible_v<T>`
     *  is `true`.
     */
    constexpr _optional_val_base(_optional_val_base && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : _storage ( std::move(rhs._storage) )
    { }

    /** In-Place Argument Forwarding Ctor -- §23.6.3.1.11
     *  -------------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the arguments `std::forward<Args>(args)...`.
     */
    template < typename ... Args
             , typename std::enable_if_t< std::is_constructible_v<T, Args...>
                                        , int > = 0 >
    constexpr explicit _optional_val_base(nonstd::in_place_t /*unused*/,
                                           Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : _storage ( nonstd::in_place, std::forward<Args>(args)... )
    { }

    /** In-Place Initializer List Ctor -- §23.6.3.1.15
     *  ----------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the arguments `il, std::forward<Args>(args)...`.
     */
    template < typename Il, typename ... Args
             , typename std::enable_if_t< std::is_constructible_v< T
                                                                 , std::initializer_list<Il> &
                                                                 , Args && ... >
                                        , int > = 0 >
    constexpr explicit _optional_val_base(nonstd::in_place_t /*unused*/,
                                           std::initializer_list<Il> il,
                                           Args && ... args)
    noexcept(std::is_nothrow_constructible_v< T
                                            , std::initializer_list<Il> &
                                            , Args && ...>)
        : _storage ( nonstd::in_place, il, std::forward<Args>(args)... )
    { }

    /** Converting Value Move Ctor -- §23.6.3.1.20
     *  ------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the expression `std::forward<U>(v)`.
     */
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<std::decay_t<U>, nonstd::in_place_t>
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                           &&  std::is_constructible_v<T, U &&>
                                           &&  std::is_convertible_v<U &&, T>)
                                        , int > = 0 >
    constexpr _optional_val_base(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _storage ( std::forward<U>(value) )
    { }
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<std::decay_t<U>, nonstd::in_place_t>
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                           &&  std::is_constructible_v<T, U &&>
                                           && !std::is_convertible_v<U &&, T>)
                                        , int > = 1 >
    constexpr explicit _optional_val_base(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _storage ( std::forward<U>(value) )
    { }

    /** Converting Copy Ctor -- §23.6.3.1.24
     *  ------------------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `*rhs`.
     *  [sic.] `rhs` is a `optional<U>`, _not_ an `_optional_val_base<U>`.
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           &&  std::is_convertible_v<U const &, T>)
                                        , int > = 0 >
    constexpr _optional_val_base(optional<U> const & rhs)
    noexcept(std::is_nothrow_constructible_v<T, U>)
        : _optional_val_base ( rhs.has_value()
                             ? _optional_val_base { *rhs    }
                             : _optional_val_base { nullopt } )
    { }

    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           && !std::is_convertible_v<U const &, T>)
                                        , int > = 1 >
    constexpr explicit _optional_val_base(optional<U> const & rhs)
    noexcept(std::is_nothrow_constructible_v<T, U>)
        : _optional_val_base ( rhs.has_value()
                             ? _optional_val_base { *rhs    }
                             : _optional_val_base { nullopt } )
    { }

    /** Converting Move Ctor -- §23.6.3.1.28
     *  ------------------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with
     *  `std::move(*rhs)`. **This will not change `bool(rhs)`**.
     *  [sic.] `rhs` is a `optional<U>`, _not_ an `_optional_val_base<U>`.
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U &&>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_convertible_v<U &&, T>)
                                        , int > = 0 >
    constexpr _optional_val_base(optional<U> && rhs)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _optional_val_base ( rhs.has_value()
                             ? _optional_val_base { std::move(*rhs) }
                             : _optional_val_base { nullopt         } )
    { }

    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U &&>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !std::is_convertible_v<U &&, T>)
                                        , int > = 1 >
    constexpr explicit _optional_val_base(optional<U> && rhs)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _optional_val_base ( rhs.has_value()
                             ? _optional_val_base{ std::move(*rhs) }
                             : _optional_val_base{ nullopt         } )
    { }

    /** Empty Assignment -- §23.6.3.3.1
     *  -------------------------------
     *  If `*this` contains a value, destroy it; otherwise, no effect.
     */
    _optional_val_base<T>& operator= (nonstd::nullopt_t /*unused*/)
    noexcept(std::is_nothrow_destructible_v<T>) {
        if (_has_value()) { _remove_value(); }
        return *this;
    }

    /** Copy Assignment -- §23.6.3.3.4
     *  ------------------------------
     *  Assign a value based on whether `*this` and `rhs` contain values;
     *   - If `*this` contains a value and `rhs` contains a value
     *     assign `*rhs` to the contained value.
     *   - If `*this` contains a value and `rhs` contains no value
     *     destroy the value contained in `*this`.
     *   - If `*this` contains no value and `rhs` contains a value
     *     initialize the contained value as if direct-non-list-initializing
     *     an object of type `T` with `*rhs`.
     *   - If `*this` contains no value and `rhs` contains no value
     *     no effect.
     *
     *  If `std::is_copy_constructible_v<T> == false` or if
     *  `std::is_copy_assignable_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableCopyAssignIf`, which `optional<T>`
     *  is an inheritor of.
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged.
     */
    _optional_val_base<T>& operator= (_optional_val_base const & rhs)
    noexcept(   std::is_nothrow_copy_assignable_v<T>
             && std::is_nothrow_copy_constructible_v<T>
             && std::is_nothrow_destructible_v<T>) {
        if (_has_value() && rhs._has_value()) {
            // See the "Note on Storage Types" above.
            _get_value() = rhs._get_value();
        } else {
            if (rhs._has_value()) {
                //TODO: This should maybe be wrapped in a try/catch?
                //      Maybe this needs to be another SFINAE branch, one for
                //      noexcept, one for try/catch?
                //      All `_construct_value` calls should be treated similarly
                _construct_value(rhs._get_value());
            } else if (_has_value()) {
                _remove_value();
            }
        }
        return *this;
    }

    /* Move Assignment -- §23.6.3.3.8
     * ------------------------------
     *  Assign a value based on whether `*this` and `rhs` contain values;
     *   - If `*this` contains a value and `rhs` contains a value
     *     assign `std::move(*rhs)` to the contained value.
     *   - If `*this` contains a value and `rhs` contains no value
     *     destroy the value contained in `*this`.
     *   - If `*this` contains no value and `rhs` contains a value
     *     initialize the contained value as if direct-non-list-initializing
     *     an object of type `T` with `std::move(*rhs)`.
     *   - If `*this` contains no value and `rhs` contains no value
     *     no effect.
     *
     *  If `std::is_move_constructible_v<T> == false` or if
     *  `std::is_move_assignable_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableMoveAssignIf`, which `optional<T>`
     *  is an inheritor of.
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged. The state of `*rhs.val` -- whether the exception
     *  is thrown during move construction or move assignment -- shall be
     *  defined by the safety guarantees of the relevant opeartion on `T`.
     */
    _optional_val_base<T>& operator= (_optional_val_base && rhs)
    noexcept(   std::is_nothrow_move_assignable_v<T>
             && std::is_nothrow_move_constructible_v<T>
             && std::is_nothrow_destructible_v<T>) {
        if (_has_value() && rhs._has_value()) {
            // See the "Note on Storage Types" above.
            _get_value() = std::move(rhs._get_value());
        } else {
            if (rhs._has_value()) {
                _construct_value(std::move(rhs._get_value()));
            } else if (_has_value()) {
                _remove_value();
            }
        }
        return *this;
    }

    /** Converting Value Move Assignment -- §23.6.3.3.13
     *  ------------------------------------------------
     *  If `*this` contains a value, assign `std::forward<U>(value)` to the
     *  contained value; otherwise initializes the contained value as if direct-
     *  non-list-initializing object of type `T` with `std::forward<U>(v)`.
     *
     *  NOTE: There is either a problem with this implementation, or the C++17
     *  n4618 standard. Missing from the implemented SFINAE clauses below is the
     *  requirement that "`conjunction_v<is_scalar<T>, is_same<T, decay_t<U>>>`
     *  is `false`." With that clause in place, assigning a `T` to a containing
     *  optional results in an ambiguous implicit constructor call, but assigning
     *  an implicitly-convertible-to-`T` succeeds. Observe,
     *
     *      optional<int> foo = 42; // Works; Calls into the Converting Value
     *                              // Move Ctor, *not* an assignment operator.
     *      foo = 84; // Fails; `operator= (U&& value)` does not participate in
     *                // overload resolution because `is_scalar_v<int>` is
     *                // `true`, and `is_same_v<int, decay_t<int>>` is `true`.
     *      // error: use of overloaded operator '=' is ambiguous (with operand
     *      //        types 'optional<int>' and 'int')
     *      // . . . candidate is the implicit move assignment operator
     *      // . . . candidate is the implicit copy assignment operator
     *
     *      optional<long> bar = 42; // Works; Calls into the Converting Value
     *                               // Move Ctor, *not* an assignment operator.
     *      bar = 84; // Works; `operator= (U&& value)` is used because
     *                // `is_same_v<int, decay_t<long>>` is `false`.
     *
     *  The missing clause is;
     *      && ! (   IS_SCALAR(T)
     *            && IS_SAME_TYPE(std::decay_t<U>, T))
     */
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                        // && !(std::is_scalar_v<T> && std::is_same_v<T, std::decay_t<U>>)
                                           &&  std::is_constructible_v<T, U>
                                           &&  std::is_assignable_v<T &, U>)
                                        , int > = 0 >
    _optional_val_base<T>& operator= (U&& value)
    noexcept(   std::is_nothrow_assignable_v<T, U &&>
             && std::is_nothrow_constructible_v<T, U &&>) {
        if (_has_value()) {
            // See the "Note on Storage Types" above.
            _get_value() = std::forward<U>(value);
        } else {
            _construct_value(std::forward<U>(value));
        }
        return *this;
    }

    /** Converting Copy Assignment -- §23.6.3.3.17
     *  ------------------------------------------
     *  Assign a value based on whether `*this` and `rhs` contain values;
     *   - If `*this` contains a value and `rhs` contains a value
     *     assign `*rhs` to the contained value.
     *   - If `*this` contains a value and `rhs` contains no value
     *     destroy the value contained in `*this`.
     *   - If `*this` contains no value and `rhs` contains a value
     *     initialize the contained value as if direct-non-list-initializing
     *     an object of type `T` with `*rhs`.
     *   - If `*this` contains no value and `rhs` contains no value
     *     no effect.
     *
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged.
     *  [sic.] `rhs` is a `optional<U>`, _not_ an `_optional_val_base<U>`.
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           &&  std::is_assignable_v<T &, U const &>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !is_assignable_from_optional_v<T, U>)
                                        , int > = 0 >
    _optional_val_base<T>& operator= (optional<U> const & rhs)
    noexcept(   std::is_nothrow_assignable_v<T, U>
             && std::is_nothrow_constructible_v<T, U>
             && std::is_nothrow_destructible_v<T>) {
        if (_has_value() && rhs.has_value()) {
            // See the "Note on Storage Types" above.
            _get_value() = *rhs;
        } else {
            if (rhs.has_value()) {
                _construct_value(*rhs);
            } else if (_has_value()) {
                _remove_value();
            }
        }
        return *this;
    }

    /** Converting Move Assignment -- §23.6.3.3.21
     *  ------------------------------------------
     *  Assign a value based on whether `*this` and `rhs` contain values;
     *   - If `*this` contains a value and `rhs` contains a value
     *     assign `std::move(*rhs)` to the contained value.
     *   - If `*this` contains a value and `rhs` contains no value
     *     destroy the value contained in `*this`.
     *   - If `*this` contains no value and `rhs` contains a value
     *     initialize the contained value as if direct-non-list-initializing
     *     an object of type `T` with `std::move(*rhs)`.
     *   - If `*this` contains no value and `rhs` contains no value
     *     no effect.
     *
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged. The state of `*rhs.val` -- whether the exception
     *  is thrown during move construction or move assignment -- shall be
     *  defined by the safety guarantees of the relevant opeartion on `T`.
     *
     *  [sic.] `rhs` is a `optional<U>`, _not_ an `_optional_val_base<U>`.
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U>
                                           &&  std::is_assignable_v<T &, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !is_assignable_from_optional_v<T, U>)
                                        , int > = 0 >
    _optional_val_base<T>& operator= (optional<U> && rhs)
    noexcept(   std::is_nothrow_assignable_v<T, U>
             && std::is_nothrow_constructible_v<T, U>
             && std::is_nothrow_destructible_v<T>) {
        if (_has_value() && rhs.has_value()) {
            // See the "Note on Storage Types" above.
            _get_value() = std::move(*rhs);
        } else {
            if (rhs.has_value()) {
                _construct_value(std::move(*rhs));
            } else if (_has_value()) {
                _remove_value();
            }
        }
        return *this;
    }

    /** Emplacement -- §23.6.3.3.25
     *  ---------------------------
     *  If `*this` contains a value, destroy it. Initialize the contained value
     *  as if direct-non-list-initializing an object of type `T` with the
     *  arguments `std::forward<Args>(args)...`.
     *  If an exception is thrown during the call to `T`'s constructor, `*this`
     *  shall not contain a value, and the previous `*val` (if any) will have
     *  been destroyed.
     */
    template <typename ... Args>
    T& emplace(Args && ... args)
    noexcept(   std::is_nothrow_constructible_v<T, Args && ...>
             && std::is_nothrow_destructible_v<T>) {
        if (_storage.is_containing) { _remove_value(); }
        _construct_value(std::forward<Args>(args)...);
        return _get_value();
    }

    /** Emplacement With Initializer List -- §23.6.3.3.31
     *  -------------------------------------------------
     *  If `*this` contains a value, destroy it. Initialize the contained value
     *  as if direct-non-list-initializing an object of type `T` with the
     *  arguments `il, std::forward<Args>(args)...`.
     */
    template <typename ... Args, typename Il>
    T& emplace(std::initializer_list<Il> il, Args && ... args)
    noexcept(   std::is_nothrow_constructible_v<T, std::initializer_list<Il> &, Args && ...>
             && std::is_nothrow_destructible_v<T>) {
        if (_storage.is_containing) { _remove_value(); }
        _construct_value(il, std::forward<Args>(args)...);
        return _get_value();
    }

protected:
    /* Helper Functions
     * ----------------
     * For destructing, constructing, and fetching values. */
    void _remove_value()
    noexcept(std::is_nothrow_destructible_v<T>) {
        _storage.is_containing = false;
        _storage.value.~storage_type();
    }

    template <typename ... Args>
    void _construct_value(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
        // We may be trying to switch the active member of the union in
        // _storage to a class with a user-defined constructor. Use
        // Placement New to accomplish this.
        void * val_ptr = &_storage.value;
        new (val_ptr) storage_type(std::forward<Args>(args)...);
        _storage.is_containing = true;
    }

    // NB. These methods cannot be noexcept because they may access an inactive
    // member of the `_storage` class' anonymous union. Most internal uses may
    // be considered noexcept, though, due to the checks that will need to be
    // performed prior to access.
    constexpr T       &  _get_value()       &  { return _storage.value;            }
    constexpr T const &  _get_value() const &  { return _storage.value;            }
    constexpr T       && _get_value()       && { return std::move(_storage.value); }
    constexpr T const && _get_value() const && { return std::move(_storage.value); }

    constexpr bool _has_value() const noexcept { return _storage.is_containing; }
};


/** Optional Construction Logic and Storage for LValue Reference Types
 *  ============================================================================
 */
template <typename T>
class _optional_ref_base {
    static_assert(std::is_lvalue_reference_v<T>,
        "_optional_ref_base expects to be specialized on an lval ref type");

private:
    /* Reference types can't be directly stored and can't be reseated once
     * initialized. To get around this, we strip the ref qualifier and store a
     * mutable pointer to the base type of the reference. */
    using storage_type = std::decay_t<T>*;

    _optional_lvalref_storage<storage_type> _storage;

public:
    using value_type = T;

    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     */
    constexpr _optional_ref_base() noexcept
        : _storage ( )
    { }
    constexpr _optional_ref_base(nonstd::nullopt_t /*unused*/) noexcept
        : _storage ( )
    { }

    /** Copy Ctor -- §23.6.3.1.3
     *  ------------------------
     *  Initialize `this` by copying `rhs`.
     */
    constexpr _optional_ref_base(_optional_ref_base const & rhs)
    noexcept
        : _storage ( rhs._storage )
    { }

    /** Move Ctor -- §23.6.3.1.7
     *  ------------------------
     *  Initialize `this` by copying `rhs`.
     */
    constexpr _optional_ref_base(_optional_ref_base && rhs) noexcept
        : _storage ( std::move(rhs._storage) )
    { }

    /** Value Ctor -- §23.6.3.1.20
     *  --------------------------
     *  Create an optional referencing the given value.
     */
    constexpr _optional_ref_base(T & value) noexcept
        : _storage ( const_cast<storage_type>(&value) )
    { }

    /** Empty Assignment -- §23.6.3.3.1
     *  -------------------------------
     *  If `*this` contains a value, destroy it; otherwise, no effect.
     */
    _optional_ref_base<T&>& operator= (nonstd::nullopt_t /*unused*/)
    noexcept {
        if (_has_value()) { _remove_value(); }
        return *this;
    }

    /** Copy Assignment -- §23.6.3.3.4
     *  ------------------------------
     */
    _optional_ref_base<T&>& operator= (_optional_ref_base const & rhs)
    noexcept {
        if (_has_value() && rhs._has_value()) {
            _storage.value = rhs._storage.value;
        } else {
            if (rhs._has_value()) {
                _storage.value = rhs._storage.value;
                _storage.is_containing = true;
            } else {
                _remove_value();
            }
        }
        return *this;
    }

    /* Move Assignment -- §23.6.3.3.8
     * ------------------------------
     */
    _optional_ref_base<T&>& operator= (_optional_ref_base && rhs)
    noexcept {
        if (_has_value() && rhs._has_value()) {
            _storage.value = std::move(rhs._storage.value);
        } else {
            if (rhs._has_value()) {
                _storage.value = std::move(rhs._storage.value);
                _storage.is_containing = true;
            } else {
                _remove_value();
            }
        }
        return *this;
    }

    /** Value Assignment -- nonstd
     *  --------------------------
     */
    _optional_ref_base<T>& operator= (T value) noexcept {
        _storage.value = const_cast<storage_type>(&value);
        _storage.is_containing = true;
        return *this;
    }

protected:
    /* Helper Functions
     * ---------------- */
    void _remove_value() noexcept {
        _storage.is_containing = false;
        _storage.value = nullptr;
    }

    // NB. These methods cannot be noexcept because they may access an inactive
    // member of the `_storage` class' anonymous union. Most internal uses may
    // be considered noexcept, though, due to the checks that will need to be
    // performed prior to access.
    constexpr T       &  _get_value()       { return *_storage.value; }
    constexpr T const &  _get_value() const { return *_storage.value; }

    constexpr bool _has_value() const noexcept { return _storage.is_containing; }

};



/** Value-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality related to storage is
 *  implemented in the `_optional_val_base` parent. This class only deals in
 *  user-facing observers.
 */
template <typename T>
class optional
    : private _optional_val_base<T>
    , private nonstd::EnableCopyCtorIf<std::is_copy_constructible_v<T>, optional<T>>
    , private nonstd::EnableCopyAssignIf<(   std::is_copy_constructible_v<T>
                                          && std::is_copy_assignable_v<T>), optional<T>>
    , private nonstd::EnableMoveCtorIf<std::is_move_constructible_v<T>, optional<T>>
    , private nonstd::EnableMoveAssignIf<(   std::is_move_constructible_v<T>
                                          && std::is_move_assignable_v<T>), optional<T>>
{
public:
    using _optional_val_base<T>::_optional_val_base;
    using _optional_val_base<T>::operator=;
    using _optional_val_base<T>::emplace;

private:
    /* Helper function to check the validity of indirecting through `*this`. */
    constexpr inline void _check_value() const {
        if (!this->_has_value()) {
            throw nonstd::exception::bad_optional_access{};
        }
        return;
    }

public:
    /** Observer `operator ->` -- §23.6.3.5.1
     *  -------------------------------------
     *  Return `val`.
     */
    constexpr       T *  operator -> () {
        return &this->_get_value();
    }
    constexpr const T *  operator -> () const {
        return &this->_get_value();
    }

    /** Observer `operator *` -- §23.6.3.5.5
     *  ------------------------------------
     *  Return `*val`.
     */
    constexpr       T &  operator *  ()       &  {
        return this->_get_value();
    }
    constexpr const T &  operator *  () const &  {
        return this->_get_value();
    }
    /** Observer `operator *` for  rvalue references -- §23.6.3.5.9
     *  -----------------------------------------------------------
     *  Return `std::move(*val)`.
     */
    constexpr       T && operator *  ()       && {
        return std::move(this->_get_value());
    }
    constexpr const T && operator *  () const && {
        return std::move(this->_get_value());
    }

    /** Observer `static_cast<bool>` -- §23.6.3.5.11
     *  --------------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr explicit operator bool () const noexcept {
        return this->_has_value();
    }
    /** Observer `has_value()` -- §23.6.3.5.11
     *  --------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr bool has_value() const noexcept {
        return this->_has_value();
    }

    /** Observer `value()` -- §23.6.3.5.15
     *  ----------------------------------
     *  If `*this` contains a value, return `*val`; otherwise throw
     *  an exception of type `bad_optional_access`.
     */
    constexpr       T &  value()       &  {
        _check_value(); return this->_get_value();
    }
    constexpr const T &  value() const &  {
        _check_value(); return this->_get_value();
    }
    /** Observer `value()` for rvalue references -- §23.6.3.5.16
     *  --------------------------------------------------------
     *  If `*this` contains a value, return `std::move(*val)`; otherwise throw
     *  an exception of type `bad_optional_access`.
     */
    constexpr       T && value()       && {
        _check_value(); return std::move(this->_get_value());
    }
    constexpr const T && value() const && {
        _check_value(); return std::move(this->_get_value());
    }

    /** Observer `value_or(value)` -- §23.6.3.5.17
     *  ------------------------------------------
     *  If `*this` is contains a value, return `*val`; otherwise return
     *  `static_cast<T>(std::forward<U>(value))`.
     */
    template < typename U = T >
    constexpr T valueOr(U && value) const & {
        return this->_has_value() ? this->_get_value()
                                 : static_cast<T>(std::forward<U>(value));
    }
    /** Observer `value_or(value)` for rvalue references -- §23.6.3.5.19
     *  ----------------------------------------------------------------
     *  If `*this` is contains a value, return `std::move(*val)`; otherwise
     *  return `static_cast<T>(std::forward<U>(value))`.
     */
    template < typename U = T >
    constexpr T valueOr(U && value) && {
        return this->_has_value() ? std::move(this->_get_value())
                                 : static_cast<T>(std::forward<U>(value));
    }
};



/** LValue-Reference-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality related to storage is
 *  implemented in the `_optional_val_base` parent. This class only deals in
 *  user-facing observers.
 */
template <typename T>
class optional<T&>
    : _optional_ref_base<T&>
{
public:
    using _optional_ref_base<T&>::_optional_ref_base;
    using _optional_ref_base<T&>::operator=;

    using value_type = T;
    using reference_type = T&;

private:
    /* Helper function to check the validity of indirecting through `*this`. */
    constexpr inline void _check_value() const {
        if (!this->_has_value()) {
            throw nonstd::exception::bad_optional_access();
        }
        return;
    }

public:
    /** Observer `operator ->` -- §23.6.3.5.1
     *  -------------------------------------
     *  Return `val`.
     */
    constexpr       T * operator -> ()       {
        return &this->_get_value();
    }
    constexpr const T * operator -> () const {
        return &this->_get_value();
    }

    /** Observer `operator *` -- §23.6.3.5.5
     *  ------------------------------------
     *  Return `*val`.
     */
    constexpr       T& operator * ()       {
        return this->_get_value();
    }
    constexpr const T& operator * () const {
        return this->_get_value();
    }

    /** Observer `static_cast<bool>` -- §23.6.3.5.11
     *  --------------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr explicit operator bool () const noexcept {
        return this->_has_value();
    }
    /** Observer `has_value()` -- §23.6.3.5.11
     *  --------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr bool has_value() const noexcept {
        return this->_has_value();
    }

    /** Observer `value()` -- §23.6.3.5.15
     *  ----------------------------------
     *  If `*this` contains a value, return `*val`; otherwise throw
     *  an exception of type `bad_optional_access`.
     */
    constexpr       T& value()       {
        _check_value(); return this->_get_value();
    }
    constexpr const T& value() const {
        _check_value(); return this->_get_value();
    }
};



/** Helper Creation Functions
 *  ============================================================================
 *  When you want to specify the type as part of an assignment or return.
 */

/* Create an `optional` with no value */
template <typename T>
constexpr optional<T> none() noexcept { return optional<T>(); }

/* Create an `optional` with a real value */
template <typename T>
constexpr optional<T> just(T value)
noexcept(std::is_nothrow_constructible_v<T,T>) {
    return optional<T> { value };
}
template <typename T, typename ... Args>
constexpr optional<T> just(nonstd::in_place_t /*unused*/, Args && ... args)
noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
    return optional<T> { nonstd::in_place, std::forward<Args>(args)... };
}
template <typename T, typename Il, typename ... Args>
constexpr optional<T> just(nonstd::in_place_t /*unused*/,
                           std::initializer_list<Il> il, Args && ... args)
noexcept(std::is_nothrow_constructible_v<T,
                                         std::initializer_list<Il>,
                                         Args && ...>) {
    return optional<T> { nonstd::in_place, il, std::forward<Args>(args)... };
}

template <typename T>
constexpr optional<T&> just_ref(T & value) {
    return optional<T&> { value };
}

template <typename T>
constexpr optional<T const &> just_cref(T const & value) {
    return optional<T const &> { value };
}



/** Relational Operators
 *  ============================================================================
 */

/** Compare two Optionals -- §23.6.6
 *  --------------------------------
 *   * If both optionals are containing, compare their values as normal.
 *   * A non-containing optional is always considered to be less than a
 *     containing optional.
 *   * If both optionals are non-containing, they are considered equal.
 */
template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator == (optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return false; }
    if ((bool)(lhs))                { return *lhs == *rhs; }
    return true;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator != (optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return true; }
    if ((bool)(lhs))                { return *lhs != *rhs; }
    return false;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator >  (optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs >  *rhs; }
    return false;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator >= (optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs >= *rhs; }
    return true;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator <  (optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs <  *rhs; }
    return false;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator <= (optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs <= *rhs; }
    return true;
}

/** Compare an Optional to a `nullopt` -- §23.6.7
 *  ---------------------------------------------
 *   * A `nullopt` is always considered to be less than a containing optional.
 *   * A `nullopt` and a non-containing optional are considered equal.
 */
template <typename T>
constexpr bool operator == (optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template <typename T>
constexpr bool operator == (nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

template <typename T>
constexpr bool operator != (optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return  (bool)(lhs);
}
template <typename T>
constexpr bool operator != (nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return  (bool)(rhs);
}

template <typename T>
constexpr bool operator <  (optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return false;
}
template <typename T>
constexpr bool operator <  (nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return (bool)(rhs);
}

template <typename T>
constexpr bool operator <= (optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template <typename T>
constexpr bool operator <= (nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return true;
}

template <typename T>
constexpr bool operator >  (optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return (bool)(lhs);
}
template <typename T>
constexpr bool operator >  (nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return false;
}

template <typename T>
constexpr bool operator >= (optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return true;
}
template <typename T>
constexpr bool operator >= (nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

/** Compare an Optional and a Value -- §23.6.8
 *  ------------------------------------------
 *   * If comparing with a containing optional, compare values as normal.
 *   * An Empty optional is always considered to be less than a Value.
 */
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator == (optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs == rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator == (Value const & lhs, optional<T> const & rhs) {
    return   (bool)(rhs) && lhs == *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator != (optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs != rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator != (Value const & lhs, optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs != *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >  (optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs > rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >  (Value const & lhs, optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs > *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >= (optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs >= rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >= (Value const & lhs, optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs >= *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <  (optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs < rhs;
}
template <  typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <  (Value const & lhs, optional<T> const & rhs) {
    return   (bool)(rhs) && lhs < *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <= (optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs <= rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <= (Value const & lhs, optional<T> const & rhs) {
    return   (bool)(rhs) && lhs <= *rhs;
}



/** Equality and Comparisons using `nonstd::equal_to` & `nonstd::compare`
 *  ============================================================================
 */

/** Compare two Optionals
 *  ---------------------
 *   * If both optionals are containing, compare their values as normal.
 *   * A non-containing optional is always considered to be less than a
 *     containing optional.
 *   * If both optionals are non-containing, they are considered equal.
 */
template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool equal_to(optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return false;                        }
    if ((bool)(lhs))                { return nonstd::equal_to(*lhs, *rhs); }
    return true;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr int compare(optional<T> const & lhs, optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs) ? 1 : -1;        }
    if ((bool)(lhs))                { return nonstd::compare(*lhs, *rhs); }
    return 0;
}


/** Compare an Optional to a `nullopt`
 *  ----------------------------------
 *   * A `nullopt` is always considered to be less than a containing optional.
 *   * A `nullopt` and a non-containing optional are considered equal.
 */
template <typename T>
constexpr bool equal_to(optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template <typename T>
constexpr bool equal_to(nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

template <typename T>
constexpr int compare(optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return (bool)(lhs) ? 1 : 0;
}

template <typename T>
constexpr int compare(nonstd::nullopt_t, optional<T> const & rhs)
noexcept {
    return (bool)(rhs) ? -1 : 0;
}


/** Compare an Optional and a value
 *  -------------------------------
 *   * If comparing with a containing optional, compare values as normal.
 *   * An Empty optional is always considered to be less than a Value.
 */
template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool equal_to(optional<T> const & lhs, Value const & rhs) {
    return (bool)(lhs) && nonstd::equal_to(*lhs, rhs);
}
template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool equal_to(Value const & lhs, optional<T> const & rhs) {
    return (bool)(rhs) && nonstd::equal_to(lhs, *rhs);
}

template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr int compare(optional<T> const & lhs, Value const & rhs) {
    return (bool)(lhs) ? nonstd::compare(*lhs, rhs) : -1;
}
template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr int compare(Value const & lhs, optional<T> const & rhs) {
    return (bool)(rhs) ? nonstd::compare(lhs, *rhs) : 1;
}


/** Print Specializations, both `ostream & operator<<` and {fmt}
 *  ============================================================================
 *  Allows for `std::cout << opt <<"\n"` and `fmt::print("{}\n", opt);` style
 *  formatting. Will only print the contained value if said value can be passed
 *  into an `ostream &` via `operator <<`.
 *
 *  NB. Because of {fmt}'s implementation of its `format_arg` global function
 *  (specifically how it's arguments are templated), we can't test for the
 *  existence of a global override (it exists for all types). Contained types
 *  _may_ override this function, for improved printing speeds, but in order for
 *  this pretty print to print them, they _must_ also override
 *  `ostream & operator <<`
 *
 *  Usage;
 *      fmt::print("{}\n", optional<f32>{ 4.f });
 *      std::count <<      optional<f32>{     } << "\n";
 *      fmt::print("{}\n", optional<c_cstr>{ "Hello" });
 *      std::count <<      optional<c_cstr>{         }) << "\n";
 *      std::count <<      optional<NoPrintOverload<f32>>{{ 4.f }}) << "\n";
 *      fmt::print("{}\n", optional<NoPrintOverload<f32>>{       });
 *  // Prints
 *      [optional<float>{ 4 }]
 *      [optional<float>{  }]
 *      [optional<const char *>{ Hello }]
 *      [optional<const char *>{  }]
 *      [optional<NoPrintOverload<float>>{ /unprintable/ }]
 *      [optional<NoPrintOverload<float>>{  }]
 */
namespace optional_has_insertion_operator {

    N2VET_PARAMS_TESTER(
        N2VET_NAMED(params),
        N2VET_WITH_ARGS(t),
        N2VET_TESTING_EXPRESSION(std::cout << t)
    );

    N2VET_TYPES_TESTER(
        N2VET_NAMED(types),
        N2VET_FROM_PARAMS_TESTER(params),
        N2VET_WITH_TEMPLATE_ARGS(T)
    );

} /* namespace optional_has_insertion_operator */

/** OStream Insertion Operator
 *  -------------------------- */
template <typename T>
std::enable_if_t< optional_has_insertion_operator::types<T>::value
                , std::ostream& >
/*std::ostream&*/ operator << (std::ostream & s, optional<T> const & opt) {
    return s << "optional<" << type_name<T>() << ">{ "
             << ((bool)opt ? fmt::format("{}", *opt).c_str() : "")
             << " }";
}

template <typename T>
std::enable_if_t< !optional_has_insertion_operator::types<T>::value
                , std::ostream& >
/*std::ostream&*/ operator << (std::ostream & s, optional<T> const & opt) {
    return s << "optional<" << type_name<T>() << ">{ "
             << ((bool)opt ? "/unprintable/" : "")
             << " }";
}

/** {fmt} args overload
 *  ------------------- */
template <typename T>
std::enable_if_t<optional_has_insertion_operator::types<T>::value>
/*void*/ format_arg(fmt::BasicFormatter<char> & f,
                    c_cstr                    & format_str,
                    optional<T> const         & opt) {
    f.writer().write("optional<{}>{{ {} }}",
                     type_name<T>(),
                     (bool)opt ? fmt::format("{}", *opt).c_str() : "");
}

template <typename T>
std::enable_if_t<!optional_has_insertion_operator::types<T>::value>
/*void*/ format_arg(fmt::BasicFormatter<char> & f,
                    c_cstr                    & format_str,
                    optional<T> const         & opt) {
    f.writer().write("optional<{}>{{ {} }}",
                     type_name<T>(),
                     (bool)opt ? "/unprintable/" : "");
}

} /* namespace nonstd */
