/** Optional (Maybe) Type
 *  =====================
 *  Utility type for representing maybe-a-value.
 *
 *  This implementation is based on the C++ specification n4713;
 *  https://github.com/cplusplus/draft/blob/master/papers/n4713.pdf
 *  For ease of navigation, the 2018-01-05 publication of eel.is was used;
 *  http://eel.is/c++draft/
 *
 *  All section references (§) refer to the above documents.
 *
 *  //TODO: Update the below.
 *
 *  The largest divergence is that we do not (always) use exceptions, and so
 *  never raise `bad_optional_access`. Instead, we offer the
 *  `N2_CHECKED_OPTIONALS` compile flag; when set to `true`, we fire a debugger
 *  breakpoint, and otherwise allow segfaults when attempting to access
 *  uninitialized data.
 *
 *  The structure of these optionals is also worth discussing. They are
 *  universally composed of three parts;
 *   - The dedicated Storage component.
 *     Contains the wrapped value (or not), and the `is_containing` boolean.
 *   - The storage-aware Base class.
 *     Contains a Storage component, and understands how to interact with the
 *     value and boolean stored therein.
 *   - The complete Optional.
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

#include <iostream>
#include <type_traits>
#include <initializer_list>

#include <thirdparty/fmt.h>

#include <nonstd/preprocessor/homogenize.h>
#include <nonstd/cpp1z/in_place_t.h>
#include <nonstd/cpp1z/type_traits_ext.h>
#include <nonstd/cpp1z/special_member_filters.h>
#include <nonstd/cpp1z/valid_expression_tester.h>
#include <nonstd/core/break.h>
#include <nonstd/c_ish/type_name.h>
#include <nonstd/std_ish/compare.h>


/** Utilities
 *  ============================================================================
 */

/** Checked Optionals
 *  -----------------
 *  Because we don't use exceptions, we optionally force a validity check per
 *  value access. If we ever attempt to access a non-containing Optional, while
 *  `N2_CHECKED_OPTIONALS` is explicitly set to `true`, a forced breakpoint will
 *  occur. If we're confident that we never misuse optionals, allowing this
 *  default-to-false to occur will let us skip that branch.
 */
#if !defined(N2_CHECKED_OPTIONALS)
#define N2_CHECKED_OPTIONALS false
#endif

namespace nonstd {

/** Null Optional Type -- §23.6.4
 *  -----------------------------
 *  The struct `nullopt_­t` is an empty structure type used as a unique type to
 *  indicate the state of not containing a value for `optional` objects. In
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
        : std::logic_error ( "Attempted to access the value of a "
                             "non-containing Optional." )
    { }
};

} /* namespace exception */


/** Forward Declarations
 *  ============================================================================
 *  Because we have a _lot_ of classes to define and specialize in this file.
 */

/** Optional Class Template -- §23.6.3
 *  ----------------------------------
 *  Will inherit from one of the `_Optional_*Base` classes, using SFINAE to
 *  expand only the salient specialization based on the value_type of `T`.
 *  Will also inherit from the `Enable[Copy|Move][Ctor|Assign]If<B,T>` helper
 *  classes to conditionally "delete" the relevant special member function based
 *  on the salient type_traits of `T`.
 */
template <typename T>
class Optional;


/** Optional Base Implementation Template -- nonstd
 *  -----------------------------------------------
 *  Will contain all the necessary storage and construction logic used by the
 *  complete Optional sub-type. The sub-types will use Constructor Inheritance
 *  (`using _Optional_*Base::_Optional_*Base`) to pull in all of the
 *  constructors defined by the Base s.t. the complete Optional will be able to
 *  correctly initialize without needing any details regarding the storage of
 *  its value.
 */
template <typename T>
class _Optional_ValueBase;

template <typename T>
class _Optional_LValRefBase;


/** _Optional_Storage Class Templates -- nonstd
 *  -------------------------------------------
 *  //TODO: Fixup the comments here & in the below _Optional_Storage section.
 */
template < typename T
         , bool MoveAndCopyCtorsAreTrivial = std::is_trivially_copy_constructible_v<T>
                                          && std::is_trivially_move_constructible_v<T>
         , bool DestructorIsTrivial = std::is_trivially_destructible_v<T> >
class _Optional_Storage;

template <typename T>
class _Optional_LValRefStorage;


/** Helpers
 *  ============================================================================
 *  These are just here to save space, because holy crap these enable_if SFINAE
 *  clauses get verbose...
 */
template <typename T, typename U>
struct is_convertible_from_optional
    : public std::bool_constant<   std::is_constructible_v<T, Optional<U> &>
                                || std::is_constructible_v<T, Optional<U> &&>
                                || std::is_constructible_v<T, Optional<U> const &>
                                || std::is_constructible_v<T, Optional<U> const &&>
                                || std::is_convertible_v<Optional<U> &, T>
                                || std::is_convertible_v<Optional<U> &&, T>
                                || std::is_convertible_v<const Optional<U> &, T>
                                || std::is_convertible_v<const Optional<U> &&, T> >
{ };
template <typename T, typename U>
inline constexpr bool is_convertible_from_optional_v =
    is_convertible_from_optional<T,U>::value;

template <typename U, typename T>
struct is_assignable_from_optional
    : public std::bool_constant<   std::is_assignable_v<T&, Optional<U> &>
                                || std::is_assignable_v<T&, Optional<U> &&>
                                || std::is_assignable_v<T&, Optional<U> const &>
                                || std::is_assignable_v<T&, Optional<U> const &&> >
{ };
template <typename U, typename T>
inline constexpr bool is_assignable_from_optional_v =
    is_assignable_from_optional<T,U>::value;


/** Optional Storage Class
 *  ============================================================================
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
 *  The _LValRefStorage is _dramatically_ simpler than its counterpart, as it
 *  requires no in-place construction, and never requires a destructor (it will
 *  be storing raw pointers, and raw pointers don't need to be automatically
 *  cleaned up). We use it primarily to maintain architecture parity between
 *  value- and reference-wrapping optionals.
 */

/** Storage for types with trivial construction and trivial destruction
 *  -------------------------------------------------------------------
 */
template <typename T>
class _Optional_Storage<T, /* MoveAndCopyCtorsAreTrivial */ true,
                           /* DestructorIsTrivial        */ true>
{
    static_assert(!std::is_same_v<T, in_place_t>,
        "_Optional_Storage objects cannot wrap `in_place_t`.");
    static_assert(!std::is_same_v<T, nullopt_t>,
        "_Optional_Storage objects cannot wrap `nullopt_t`.");

private:
    struct _Empty { };

public:
    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };


    constexpr _Optional_Storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }


    constexpr _Optional_Storage(_Optional_Storage const &) = default;
    constexpr _Optional_Storage(_Optional_Storage &&     ) = default;


    constexpr _Optional_Storage(T const & value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( true  )
        , value         ( value )
    { }

    constexpr _Optional_Storage(T && value)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( true             )
        , value         ( std::move(value) )
    { }

    template <typename ... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... )
    { }

    template <typename Il, typename ... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
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
 *  Interestingly, we can leave the copy and move constructors `constexpr`,
 *  as the compiler will be able to skip the `_construct` call if the `rhs`
 *  is non-containing. I don't know if a compile-time, non-containing, non-
 *  trivially constructible Optional will be of use, but... You can do it.
 */
template <typename T>
class _Optional_Storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                           /* DestructorIsTrivial        */ true>
{
    static_assert(!std::is_same_v<std::decay_t<T>, in_place_t>,
        "_Optional_Storage objects cannot wrap `in_place_t`.");
    static_assert(!std::is_same_v<std::decay_t<T>, nullopt_t>,
        "_Optional_Storage objects cannot wrap `nullopt_t`.");

private:
    struct _Empty { };

public:
    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

private:
    // The anonymous union may have be initialized to `empty`, so we use
    // placement new to guarantee the active member is switched.
    template <typename ... Args>
    void _construct(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }


public:
    constexpr _Optional_Storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }

    constexpr _Optional_Storage(_Optional_Storage const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(rhs.value); }
    }

    constexpr _Optional_Storage(_Optional_Storage && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(std::move(rhs.value)); }
    }


    constexpr _Optional_Storage(T const & value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( true  )
        , value         ( value )
    { }

    constexpr _Optional_Storage(T && value)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( true             )
        , value         ( std::move(value) )
    { }

    template <typename ... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... )
    { }

    template <typename Il, typename ... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
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
class _Optional_Storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                           /* DestructorIsTrivial        */ false>
{
    static_assert(!std::is_same_v<std::decay_t<T>, in_place_t>,
        "_Optional_Storage objects cannot wrap `in_place_t`.");
    static_assert(!std::is_same_v<std::decay_t<T>, nullopt_t>,
        "_Optional_Storage objects cannot wrap `nullopt_t`.");

private:
    struct _Empty { };

public:
    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

private:
    // The anonymous union may have be initialized to `empty`, so we use
    // placement new to guarantee the active member is switched.
    template <typename ... Args>
    void _construct(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }

public:
    constexpr _Optional_Storage() noexcept
        : is_containing ( false )
        , empty         (       )
    { }


    constexpr _Optional_Storage(_Optional_Storage const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(rhs.value); }
    }

    constexpr _Optional_Storage(_Optional_Storage && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( rhs.is_containing )
        , empty         (                   )
    {
        if (rhs.is_containing) { _construct(std::move(rhs.value)); }
    }


    constexpr _Optional_Storage(T const & value)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : is_containing ( true  )
        , value         ( value )
    { }

    constexpr _Optional_Storage(T && value)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_containing ( true             )
        , value         ( std::move(value) )
    { }

    template <typename ... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... )
    { }

    template <typename Il, typename ... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
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
    ~_Optional_Storage()
    noexcept(std::is_nothrow_destructible_v<T>) {
        if (is_containing) { value.~T(); }
    }
};


/** Storage for LValue Reference types
 *  ----------------------------------
 */
template <typename T>
class _Optional_LValRefStorage {
    static_assert(std::is_pointer_v<T>,
        "_Optional_LValRefStorage expects to be specialized on a pointer type");
    static_assert(!std::is_same_v<std::remove_cv_t<T>, in_place_t*>,
        "_Optional_Storage objects cannot wrap `in_place_t *`.");
    static_assert(!std::is_same_v<std::remove_cv_t<T>, nullopt_t*>,
        "_Optional_Storage objects cannot wrap `nullopt_t *`.");

public:
    bool is_containing;
    T    value;

    constexpr _Optional_LValRefStorage() noexcept
        : is_containing ( false   )
        , value         ( nullptr )
    { }

    constexpr _Optional_LValRefStorage(nonstd::nullopt_t /*unused*/) noexcept
        : is_containing ( false   )
        , value         ( nullptr )
    { }


    constexpr _Optional_LValRefStorage(_Optional_LValRefStorage const & rhs)
    noexcept
        : is_containing ( rhs.is_containing )
        , value         ( rhs.value         )
    { }

    constexpr _Optional_LValRefStorage(_Optional_LValRefStorage && rhs)
    noexcept
        : is_containing ( rhs.is_containing    )
        , value         ( std::move(rhs.value) )
    { }

    constexpr _Optional_LValRefStorage(T value) noexcept
        : is_containing ( true  )
        , value         ( value )
    { }
};



/** Optional Construction Logic and Storage for Value Types
 *  =======================================================
 */

template <typename T>
class _Optional_ValueBase {
private:
    /** A Note on Storage Types
     *  To let us use (and re-use) storage when `T` is cv qualified, we strip
     *  any const-ness when defining our _Optional_Storage type. All public and
     *  protected methods will take and return `T`s, though, so the removal of
     *  the cv-qualifier should be entirely invisible. In fact, most methods in
     *  this class should call `_getValue` rather than directly accessing
     *  `_storage.value` so that they access a `T`, rather than a
     *  `std::remove_const_t<T>`.
     */
    using storage_type = std::remove_const_t<T>;
    _Optional_Storage<storage_type> _storage;

    /* Optionals must all be friends to one another. */
    template <typename U> friend class _Optional_ValueBase;

public:
    using value_type = T;

    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     */
    constexpr _Optional_ValueBase() noexcept
        : _storage ( )
    { }
    constexpr _Optional_ValueBase(nonstd::nullopt_t /*unused*/) noexcept
        : _storage ( )
    { }

    /** Copy Ctor -- §23.6.3.1.3
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `*rhs`.
     *  If `std::is_copy_constructible_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableCopyCtorIf`, which `Optional<T>`
     *  is an inheritor of.
     *  This constructor will only be (successfully) `constexpr` if `T` is
     *  trivially copy constructible; `_Optional_Storage` will only have a
     *  `constexpr` copy constructor if `is_trivially_copy_constructible_v<T>`
     *  is `true`.
     */
    constexpr _Optional_ValueBase(_Optional_ValueBase const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : _storage ( rhs._storage )
    { }

    /** Move Ctor -- §23.6.3.1.7
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with
     *  `std::move(*rhs)`. **This will not change `bool(rhs)`**.
     *  If `std::is_move_constructible_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableMoveCtorIf`, which `Optional<T>`
     *  is an inheritor of.
     *  This constructor will only be (successfully) `constexpr` if `T` is
     *  trivially move constructible; `_Optional_Storage` will only have a
     *  `constexpr` move constructor if `is_trivially_move_constructible_v<T>`
     *  is `true`.
     */
    constexpr _Optional_ValueBase(_Optional_ValueBase && rhs)
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
    constexpr explicit _Optional_ValueBase(nonstd::in_place_t /*unused*/,
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
    constexpr explicit _Optional_ValueBase(nonstd::in_place_t /*unused*/,
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
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, Optional<T>>
                                           &&  std::is_constructible_v<T, U &&>
                                           &&  std::is_convertible_v<U &&, T>)
                                        , int > = 0 >
    constexpr _Optional_ValueBase(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _storage ( std::forward<U>(value) )
    { }
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<std::decay_t<U>, nonstd::in_place_t>
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, Optional<T>>
                                           &&  std::is_constructible_v<T, U &&>
                                           && !std::is_convertible_v<U &&, T>)
                                        , int > = 1 >
    constexpr explicit _Optional_ValueBase(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _storage ( std::forward<U>(value) )
    { }

    /** Converting Copy Ctor -- §23.6.3.1.24
     *  ------------------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `*rhs`.
     *  [sic.] `rhs` is a `Optional<U> const &`, _not_ an
     *  `_Optional_ValueBase<U> const &`
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           &&  std::is_convertible_v<U const &, T>)
                                        , int > = 0 >
    constexpr _Optional_ValueBase(Optional<U> const & rhs)
    noexcept(std::is_nothrow_constructible_v<T, U>)
        : _Optional_ValueBase ( rhs.hasValue()
                                ? _Optional_ValueBase{ *rhs            }
                                : _Optional_ValueBase{ nonstd::nullopt } )
    { }

    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           && !std::is_convertible_v<U const &, T>)
                                        , int > = 1 >
    constexpr explicit _Optional_ValueBase(Optional<U> const & rhs)
    noexcept(std::is_nothrow_constructible_v<T, U>)
        : _Optional_ValueBase ( rhs.hasValue()
                                ? _Optional_ValueBase{ *rhs            }
                                : _Optional_ValueBase{ nonstd::nullopt } )
    { }

    /** Converting Move Ctor -- §23.6.3.1.28
     *  ------------------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with
     *  `std::move(*rhs)`. **This will not change `bool(rhs)`**.
     *  [sic.] `rhs` is a `Optional<U> &&`, _not_ an
     *  `_Optional_ValueBase<U> &&`.
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U &&>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_convertible_v<U &&, T>)
                                        , int > = 0 >
    constexpr _Optional_ValueBase(Optional<U> && rhs)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _Optional_ValueBase ( rhs.hasValue()
                                ? _Optional_ValueBase{ std::move(*rhs) }
                                : _Optional_ValueBase{ nonstd::nullopt } )
    { }

    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U &&>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !std::is_convertible_v<U &&, T>)
                                        , int > = 1 >
    constexpr explicit _Optional_ValueBase(Optional<U> && rhs)
    noexcept(std::is_nothrow_constructible_v<T, U &&>)
        : _Optional_ValueBase ( rhs.hasValue()
                                ? _Optional_ValueBase{ std::move(*rhs) }
                                : _Optional_ValueBase{ nonstd::nullopt } )
    { }

    /** Empty Assignment -- §23.6.3.3.1
     *  -------------------------------
     *  If `*this` contains a value, destroy it; otherwise, no effect.
     */
    _Optional_ValueBase<T>& operator= (nonstd::nullopt_t /*unused*/)
    noexcept(std::is_nothrow_destructible_v<T>) {
        if (_storage.is_containing) { _removeValue(); }
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
     *  explicitly deleted by `nonstd::EnableCopyAssignIf`, which `Optional<T>`
     *  is an inheritor of.
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged.
     */
    _Optional_ValueBase<T>& operator= (_Optional_ValueBase const & rhs)
    noexcept(   std::is_nothrow_copy_assignable_v<T>
             && std::is_nothrow_copy_constructible_v<T>
             && std::is_nothrow_destructible_v<T>) {
        if (_storage.is_containing && rhs._storage.is_containing) {
            // See the "Note on Storage Types" above.
            _getValue() = rhs._getValue();
        } else {
            if (rhs._storage.is_containing) {
                //TODO: This should maybe be wrapped in a try/catch?
                //      Maybe this needs to be another SFINAE branch, one for
                //      noexcept, one for try/catch?
                //      All `_constructValue` calls should be treated similarly.
                _constructValue(rhs._getValue());
            } else if (_storage.is_containing) {
                _removeValue();
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
     *  explicitly deleted by `nonstd::EnableMoveAssignIf`, which `Optional<T>`
     *  is an inheritor of.
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged. The state of `*rhs.val` -- whether the exception
     *  is thrown during move construction or move assignment -- shall be
     *  defined by the safety guarantees of the relevant opeartion on `T`.
     */
    _Optional_ValueBase<T>& operator= (_Optional_ValueBase && rhs)
    noexcept(   std::is_nothrow_move_assignable_v<T>
             && std::is_nothrow_move_constructible_v<T>
             && std::is_nothrow_destructible_v<T>) {
        if (_storage.is_containing && rhs._storage.is_containing) {
            // See the "Note on Storage Types" above.
            _getValue() = std::move(rhs._getValue());
        } else {
            if (rhs._storage.is_containing) {
                _constructValue(std::move(rhs._getValue()));
            } else if (_storage.is_containing) {
                _removeValue();
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
     *  is `false`." With that clause in place, assigning a T to a containing
     *  Optional results in an ambiguous implicit constructor call, but assigning
     *  an implicitly-convertible-to-T succeeds. Observe,
     *      Optional<int>  foo = 42; // Works; Calls into the Converting Value
     *                               // Move Ctor, *not* an assignment operator.
     *      foo = 84; // Fails; `operator= (U&& value)` does not participate in
     *                // overload resolution because `is_scalar_v<int>` is
     *                // `true`, and `is_same_v<int, decay_t<int>>` is `true`.
     *      // error: use of overloaded operator '=' is ambiguous (with operand
     *      //         types 'Optional<int>' and 'int')
     *      // . . . candidate is the implicit move assignment operator
     *      // . . . candidate is the implicit copy assignment operator
     *
     *      Optional<long> bar = 42; // Works; Calls into the Converting Value
     *                               // Move Ctor, *not* an assignment operator.
     *      bar = 84; // Works; `operator= (U&& value)` is used because
     *                // `is_same_v<int, decay_t<long>>` is `false`.
     *
     *  The missing clause is;
     *      && ! (   IS_SCALAR(T)
     *            && IS_SAME_TYPE(std::decay_t<U>, T))
     */
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<nonstd::remove_cvref_t<U>, Optional<T>>
                                        // && !(std::is_scalar_v<T> && std::is_same_v<T, std::decay_t<U>>)
                                           &&  std::is_constructible_v<T, U>
                                           &&  std::is_assignable_v<T &, U>)
                                        , int > = 0 >
    _Optional_ValueBase<T>& operator= (U&& value)
    noexcept(   std::is_nothrow_assignable_v<T, U &&>
             && std::is_nothrow_constructible_v<T, U &&>) {
        if (_storage.is_containing) {
            // See the "Note on Storage Types" above.
            _getValue() = std::forward<U>(value);
        } else {
            _constructValue(std::forward<U>(value));
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
     *  [sic.] `rhs` is a `Optional<U> const &`, _not_ an
     *  `_Optional_ValueBase<U> const &`.
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           &&  std::is_assignable_v<T &, U const &>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !is_assignable_from_optional_v<T, U>)
                                        , int > = 0 >
    _Optional_ValueBase<T>& operator= (Optional<U> const & rhs)
    noexcept(   std::is_nothrow_assignable_v<T, U>
             && std::is_nothrow_constructible_v<T, U>
             && std::is_nothrow_destructible_v<T>) {
        if (_storage.is_containing && rhs.hasValue()) {
            // See the "Note on Storage Types" above.
            _getValue() = *rhs;
        } else {
            if (rhs.hasValue()) {
                _constructValue(*rhs);
            } else if (_storage.is_containing) {
                _removeValue();
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
     *  [sic.] `rhs` is a `Optional<U> &&`, _not_ an
     *  `_Optional_ValueBase<U> &&`.
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U>
                                           &&  std::is_assignable_v<T &, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !is_assignable_from_optional_v<T, U>)
                                        , int > = 0 >
    _Optional_ValueBase<T>& operator= (Optional<U> && rhs)
    noexcept(   std::is_nothrow_assignable_v<T, U>
             && std::is_nothrow_constructible_v<T, U>
             && std::is_nothrow_destructible_v<T>) {
        if (_storage.is_containing && rhs.hasValue()) {
            // See the "Note on Storage Types" above.
            _getValue() = std::move(*rhs);
        } else {
            if (rhs.hasValue()) {
                _constructValue(std::move(*rhs));
            } else if (_storage.is_containing) {
                _removeValue();
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
        if (_storage.is_containing) { _removeValue(); }
        _constructValue(std::forward<Args>(args)...);
        return _getValue();
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
        if (_storage.is_containing) { _removeValue(); }
        _constructValue(il, std::forward<Args>(args)...);
        return _getValue();
    }

protected:
    /* Helper Functions
     * ----------------
     * For destructing, constructing, and fetching values. */
    void _removeValue()
    noexcept(std::is_nothrow_destructible_v<T>) {
        _storage.is_containing = false;
        _storage.value.~storage_type();
    }

    template <typename ... Args>
    void _constructValue(Args && ... args)
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
    constexpr T       &  _getValue()       &  { return _storage.value;            }
    constexpr T const &  _getValue() const &  { return _storage.value;            }
    constexpr T       && _getValue()       && { return std::move(_storage.value); }
    constexpr T const && _getValue() const && { return std::move(_storage.value); }

    constexpr bool _hasValue() const noexcept { return _storage.is_containing; }
};


/** Optional Construction Logic and Storage for LValue Reference Types
 *  ============================================================================
 */
template <typename T>
class _Optional_LValRefBase {
    static_assert(std::is_lvalue_reference_v<T>,
        "_Optional_LValRefBase expects to be specialized on an lval ref type");

private:
    /* Reference types can't be directly stored and can't be reseated once
     * initialized. To get around this, we strip the ref qualifier and store a
     * mutable pointer to the base type of the reference. */
    using storage_type = std::decay_t<T>*;

    _Optional_LValRefStorage<storage_type> _storage;

public:
    using value_type = T;

    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     */
    constexpr _Optional_LValRefBase() noexcept
        : _storage ( )
    { }
    constexpr _Optional_LValRefBase(nonstd::nullopt_t /*unused*/) noexcept
        : _storage ( )
    { }

    /** Copy Ctor -- §23.6.3.1.3
     *  ------------------------
     *  Initialize `this` by copying `rhs`.
     */
    constexpr _Optional_LValRefBase(_Optional_LValRefBase const & rhs)
    noexcept
        : _storage ( rhs._storage )
    { }

    /** Move Ctor -- §23.6.3.1.7
     *  ------------------------
     *  Initialize `this` by copying `rhs`.
     */
    constexpr _Optional_LValRefBase(_Optional_LValRefBase && rhs) noexcept
        : _storage ( std::move(rhs._storage) )
    { }

    /** Value Ctor -- §23.6.3.1.20
     *  --------------------------
     *  Create an optional referencing the given value.
     */
    constexpr _Optional_LValRefBase(T & value) noexcept
        : _storage ( const_cast<storage_type>(&value) )
    { }

    /** Empty Assignment -- §23.6.3.3.1
     *  -------------------------------
     *  If `*this` contains a value, destroy it; otherwise, no effect.
     */
    _Optional_LValRefBase<T&>& operator= (nonstd::nullopt_t /*unused*/)
    noexcept {
        if (_storage.is_containing) { _removeValue(); }
        return *this;
    }

    /** Copy Assignment -- §23.6.3.3.4
     *  ------------------------------
     */
    _Optional_LValRefBase<T&>& operator= (_Optional_LValRefBase const & rhs)
    noexcept {
        if (_storage.is_containing && rhs._storage.is_containing) {
            _storage.value = rhs._storage.value;
        } else {
            if (rhs._storage.is_containing) {
                _storage.value = rhs._storage.value;
                _storage.is_containing = true;
            } else {
                _removeValue();
            }
        }
        return *this;
    }

    /* Move Assignment -- §23.6.3.3.8
     * ------------------------------
     */
    _Optional_LValRefBase<T&>& operator= (_Optional_LValRefBase && rhs)
    noexcept {
        if (_storage.is_containing && rhs._storage.is_containing) {
            _storage.value = std::move(rhs._storage.value);
        } else {
            if (rhs._storage.is_containing) {
                _storage.value = std::move(rhs._storage.value);
                _storage.is_containing = true;
            } else {
                _removeValue();
            }
        }
        return *this;
    }

    /** Value Assignment -- nonstd
     *  --------------------------
     */
    _Optional_LValRefBase<T>& operator= (T value) noexcept {
        _storage.value = const_cast<storage_type>(&value);
        _storage.is_containing = true;
        return *this;
    }

protected:
    /* Helper Functions
     * ---------------- */
    void _removeValue() noexcept {
        _storage.is_containing = false;
        _storage.value = nullptr;
    }

    // NB. These methods cannot be noexcept because they may access an inactive
    // member of the `_storage` class' anonymous union. Most internal uses may
    // be considered noexcept, though, due to the checks that will need to be
    // performed prior to access.
    constexpr T       &  _getValue()       { return *_storage.value; }
    constexpr T const &  _getValue() const { return *_storage.value; }

    constexpr bool _hasValue() const noexcept { return _storage.is_containing; }

};



/** Value-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality related to storage is
 *  implemented in the _Optional_ValueBase parent. This class only deals in
 *  user-facing observers.
 */
template <typename T>
class Optional
    : private _Optional_ValueBase<T>
    , private nonstd::EnableCopyCtorIf<std::is_copy_constructible_v<T>, Optional<T>>
    , private nonstd::EnableCopyAssignIf<(   std::is_copy_constructible_v<T>
                                          && std::is_copy_assignable_v<T>), Optional<T>>
    , private nonstd::EnableMoveCtorIf<std::is_move_constructible_v<T>, Optional<T>>
    , private nonstd::EnableMoveAssignIf<(   std::is_move_constructible_v<T>
                                          && std::is_move_assignable_v<T>), Optional<T>>
{
public:
    using _Optional_ValueBase<T>::_Optional_ValueBase;
    using _Optional_ValueBase<T>::operator=;
    using _Optional_ValueBase<T>::emplace;

private:
    /* Helper function to (optionally) check the validity of this Optional. */
    constexpr inline void checkValue() const {
#if N2_CHECKED_OPTIONALS
        if (!this->_hasValue()) {
            throw nonstd::exception::bad_optional_access{};
        }
#endif
        return;
    }

public:
    /** Observer `operator ->` -- §23.6.3.5.1
     *  -------------------------------------
     *  Return `val`.
     */
    constexpr       T *  operator -> () {
        checkValue(); return &this->_getValue();
    }
    constexpr const T *  operator -> () const {
        checkValue(); return &this->_getValue();
    }

    /** Observer `operator *` -- §23.6.3.5.5
     *  ------------------------------------
     *  Return `*val`.
     */
    constexpr       T &  operator *  ()       &  {
        checkValue(); return this->_getValue();
    }
    constexpr const T &  operator *  () const &  {
        checkValue(); return this->_getValue();
    }
    /** Observer `operator *` for  rvalue references -- §23.6.3.5.9
     *  -----------------------------------------------------------
     *  Return `std::move(*val)`.
     */
    constexpr       T && operator *  ()       && {
        checkValue(); return std::move(this->_getValue());
    }
    constexpr const T && operator *  () const && {
        checkValue(); return std::move(this->_getValue());
    }

    /** Observer `static_cast<bool>` -- §23.6.3.5.11
     *  --------------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr explicit operator bool () const noexcept {
        return this->_hasValue();
    }
    /** Observer `has_value()` -- §23.6.3.5.11
     *  --------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr bool hasValue() const noexcept {
        return this->_hasValue();
    }

    /** Observer `value()` -- §23.6.3.5.15
     *  ----------------------------------
     *  If `*this` contains a value, return `*val`; otherwise throw
     *  an exception of type `bad_optional_access`.
     */
    constexpr       T &  value()       &  {
        checkValue(); return this->_getValue();
    }
    constexpr const T &  value() const &  {
        checkValue(); return this->_getValue();
    }
    /** Observer `value()` for rvalue references -- §23.6.3.5.16
     *  --------------------------------------------------------
     *  If `*this` contains a value, return `std::move(*val)`; otherwise throw
     *  an exception of type `bad_optional_access`.
     */
    constexpr       T && value()       && {
        checkValue(); return std::move(this->_getValue());
    }
    constexpr const T && value() const && {
        checkValue(); return std::move(this->_getValue());
    }

    /** Observer `value_or(value)` -- §23.6.3.5.17
     *  ------------------------------------------
     *  If `*this` is contains a value, return `*val`; otherwise return
     *  `static_cast<T>(std::forward<U>(value))`.
     */
    template < typename U = T >
    constexpr T valueOr(U && value) const & {
        return this->_hasValue() ? this->_getValue()
                                 : static_cast<T>(std::forward<U>(value));
    }
    /** Observer `value_or(value)` for rvalue references -- §23.6.3.5.19
     *  ----------------------------------------------------------------
     *  If `*this` is contains a value, return `std::move(*val)`; otherwise
     *  return `static_cast<T>(std::forward<U>(value))`.
     */
    template < typename U = T >
    constexpr T valueOr(U && value) && {
        return this->_hasValue() ? std::move(this->_getValue())
                                 : static_cast<T>(std::forward<U>(value));
    }
};



/** LValue-Reference-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality related to storage is
 *  implemented in the _Optional_ValueBase parent. This class only deals in
 *  user-facing observers.
 */
template <typename T>
class Optional<T&>
    : _Optional_LValRefBase<T&>
{
public:
    using _Optional_LValRefBase<T&>::_Optional_LValRefBase;
    using _Optional_LValRefBase<T&>::operator=;

    using value_type = T;
    using reference_type = T&;

private:
    /* Helper function to (optionally) check the validity of this Optional. */
    constexpr inline void checkValue() const {
#if N2_CHECKED_OPTIONALS
        if (!this->_hasValue()) {
            throw nonstd::exception::bad_optional_access();
        }
#endif
        return;
    }

public:
    /** Observer `operator ->` -- §23.6.3.5.1
     *  -------------------------------------
     *  Return `val`.
     */
    constexpr       T * operator -> ()       {
        checkValue(); return &this->_getValue();
    }
    constexpr const T * operator -> () const {
        checkValue(); return &this->_getValue();
    }

    /** Observer `operator *` -- §23.6.3.5.5
     *  ------------------------------------
     *  Return `*val`.
     */
    constexpr       T& operator * ()       {
        checkValue(); return this->_getValue();
    }
    constexpr const T& operator * () const {
        checkValue(); return this->_getValue();
    }

    /** Observer `static_cast<bool>` -- §23.6.3.5.11
     *  --------------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr explicit operator bool () const noexcept {
        return this->_hasValue();
    }
    /** Observer `has_value()` -- §23.6.3.5.11
     *  --------------------------------------
     *  Return `true` if `*this` contains a value.
     */
    constexpr bool hasValue() const noexcept {
        return this->_hasValue();
    }

    /** Observer `value()` -- §23.6.3.5.15
     *  ----------------------------------
     *  If `*this` contains a value, return `*val`; otherwise throw
     *  an exception of type `bad_optional_access`.
     */
    constexpr       T& value()       {
        checkValue(); return this->_getValue();
    }
    constexpr const T& value() const {
        checkValue(); return this->_getValue();
    }
};



/** Helper Creation Functions
 *  ============================================================================
 *  When you want to specify the type as part of an assignment or return.
 */

/* Create an Optional with no value */
template <typename T>
constexpr Optional<T> none() noexcept { return Optional<T>(); }

/* Create an Optional with a real value */
template <typename T>
constexpr Optional<T> just(T value)
noexcept(std::is_nothrow_constructible_v<T,T>) {
    return Optional<T> { value };
}
template <typename T, typename ... Args>
constexpr Optional<T> just(nonstd::in_place_t /*unused*/, Args && ... args)
noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
    return Optional<T> { nonstd::in_place, std::forward<Args>(args)... };
}
template <typename T, typename Il, typename ... Args>
constexpr Optional<T> just(nonstd::in_place_t /*unused*/,
                           std::initializer_list<Il> il, Args && ... args)
noexcept(std::is_nothrow_constructible_v<T,
                                         std::initializer_list<Il>,
                                         Args && ...>) {
    return Optional<T> { nonstd::in_place, il, std::forward<Args>(args)... };
}

template <typename T>
constexpr Optional<T &> just_ref(T & value) {
    return Optional<T &> { value };
}

template <typename T>
constexpr Optional<T const &> just_cref(T const & value) {
    return Optional<T const &> { value };
}



/** Relational Operators
 *  ============================================================================
 */

/** Compare two Optionals -- §23.6.6
 *  --------------------------------
 *   * If both Optionals are containing, compare their values as normal.
 *   * A non-containing Optional is always considered to be less than a
 *     containing Optional.
 *   * If both Optionals are non-containing, they are considered equal.
 */
template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator == (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return false; }
    if ((bool)(lhs))                { return *lhs == *rhs; }
    return true;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator != (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return true; }
    if ((bool)(lhs))                { return *lhs != *rhs; }
    return false;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator >  (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs >  *rhs; }
    return false;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator >= (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs >= *rhs; }
    return true;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator <  (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs <  *rhs; }
    return false;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool operator <= (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs <= *rhs; }
    return true;
}

/** Compare an Optional to a `nullopt` -- §23.6.7
 *  ---------------------------------------------
 *   * A `nullopt` is always considered to be less than a containing Optional.
 *   * A `nullopt` and a non-containing Optional are considered equal.
 */
template <typename T>
constexpr bool operator == (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template <typename T>
constexpr bool operator == (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

template <typename T>
constexpr bool operator != (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return  (bool)(lhs);
}
template <typename T>
constexpr bool operator != (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return  (bool)(rhs);
}

template <typename T>
constexpr bool operator <  (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return false;
}
template <typename T>
constexpr bool operator <  (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return (bool)(rhs);
}

template <typename T>
constexpr bool operator <= (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template <typename T>
constexpr bool operator <= (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return true;
}

template <typename T>
constexpr bool operator >  (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return (bool)(lhs);
}
template <typename T>
constexpr bool operator >  (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return false;
}

template <typename T>
constexpr bool operator >= (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return true;
}
template <typename T>
constexpr bool operator >= (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

/** Compare an Optional and a Value -- §23.6.8
 *  ------------------------------------------
 *   * If comparing with a containing Optional, compare values as normal.
 *   * An Empty Optional is always considered to be less than a Value.
 */
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator == (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs == rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator == (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs == *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator != (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs != rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator != (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs != *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >  (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs > rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >  (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs > *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >= (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs >= rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator >= (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs >= *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <  (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs < rhs;
}
template <  typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <  (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs < *rhs;
}

template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <= (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs <= rhs;
}
template < typename T, typename Value=T
         , typename std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool operator <= (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs <= *rhs;
}



/** Equality and Comparisons using `nonstd::equal_to` & `nonstd::compare`
 *  ============================================================================
 */

/** Compare two Optionals
 *  ---------------------
 *   * If both Optionals are containing, compare their values as normal.
 *   * A non-containing Optional is always considered to be less than a
 *     containing Optional.
 *   * If both Optionals are non-containing, they are considered equal.
 */
template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr bool equal_to(Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return false;                        }
    if ((bool)(lhs))                { return nonstd::equal_to(*lhs, *rhs); }
    return true;
}

template < typename T, typename U=T
         , typename std::enable_if_t<std::is_convertible_v<U,T>, int> = 0 >
constexpr int compare(Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs) ? 1 : -1;        }
    if ((bool)(lhs))                { return nonstd::compare(*lhs, *rhs); }
    return 0;
}


/** Compare an Optional to a `nullopt`
 *  ----------------------------------
 *   * A `nullopt` is always considered to be less than a containing Optional.
 *   * A `nullopt` and a non-containing Optional are considered equal.
 */
template <typename T>
constexpr bool equal_to(Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template <typename T>
constexpr bool equal_to(nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

template <typename T>
constexpr int compare(Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return (bool)(lhs) ? 1 : 0;
}

template <typename T>
constexpr int compare(nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return (bool)(rhs) ? -1 : 0;
}


/** Compare an Optional and a value
 *  -------------------------------
 *   * If comparing with a containing Optional, compare values as normal.
 *   * An Empty Optional is always considered to be less than a Value.
 */
template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool equal_to(Optional<T> const & lhs, Value const & rhs) {
    return (bool)(lhs) && nonstd::equal_to(*lhs, rhs);
}
template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr bool equal_to(Value const & lhs, Optional<T> const & rhs) {
    return (bool)(rhs) && nonstd::equal_to(lhs, *rhs);
}

template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr int compare(Optional<T> const & lhs, Value const & rhs) {
    return (bool)(lhs) ? nonstd::compare(*lhs, rhs) : -1;
}
template < typename T, typename Value=T
         , std::enable_if_t<std::is_convertible_v<Value,T>, int> = 0 >
constexpr int compare(Value const & lhs, Optional<T> const & rhs) {
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
 *      fmt::print("{}\n", Optional<f32>{ 4.f });
 *      std::count <<      Optional<f32>{     } << "\n";
 *      fmt::print("{}\n", Optional<c_cstr>{ "Hello" });
 *      std::count <<      Optional<c_cstr>{         }) << "\n";
 *      std::count <<      Optional<NoPrintOverload<f32>>{{ 4.f }}) << "\n";
 *      fmt::print("{}\n", Optional<NoPrintOverload<f32>>{       });
 *  // Prints
 *      [Optional<float>{ 4 }]
 *      [Optional<float>{  }]
 *      [Optional<const char *>{ Hello }]
 *      [Optional<const char *>{  }]
 *      [Optional<NoPrintOverload<float>>{ /unprintable/ }]
 *      [Optional<NoPrintOverload<float>>{  }]
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
/*std::ostream&*/ operator << (std::ostream & s, Optional<T> const & opt) {
    return s << "Optional<" << type_name<T>() << ">{ "
             << ((bool)opt ? fmt::format("{}", *opt).c_str() : "")
             << " }";
}

template <typename T>
std::enable_if_t< !optional_has_insertion_operator::types<T>::value
                , std::ostream& >
/*std::ostream&*/ operator << (std::ostream & s, Optional<T> const & opt) {
    return s << "Optional<" << type_name<T>() << ">{ "
             << ((bool)opt ? "/unprintable/" : "")
             << " }";
}

/** {fmt} args overload
 *  ------------------- */
template <typename T>
std::enable_if_t<optional_has_insertion_operator::types<T>::value>
/*void*/ format_arg(fmt::BasicFormatter<char> & f,
                    c_cstr                    & format_str,
                    Optional<T> const         & opt) {
    f.writer().write("Optional<{}>{{ {} }}",
                     type_name<T>(),
                     (bool)opt ? fmt::format("{}", *opt).c_str() : "");
}

template <typename T>
std::enable_if_t<!optional_has_insertion_operator::types<T>::value>
/*void*/ format_arg(fmt::BasicFormatter<char> & f,
                    c_cstr                    & format_str,
                    Optional<T> const         & opt) {
    f.writer().write("Optional<{}>{{ {} }}",
                     type_name<T>(),
                     (bool)opt ? "/unprintable/" : "");
}

} /* namespace nonstd */
