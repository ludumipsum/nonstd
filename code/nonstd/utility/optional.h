/** Optional (Maybe) Type
 *  =====================
 *  Utility type for representing maybe-a-value.
 *  This implementation is based heavily on the C++17 specification for the
 *  std::optional type. Nearly all of the interface is based on the
 *  implementation described in n4618. The largest divergence is that we do not
 *  use exceptions, and so never raise `bad_optional_access`. Instead, we offer
 *  the `N2_CHECKED_OPTIONALS` compile flag; when set to `true`, we fire a
 *  debugger breakpoint, and otherwise allow segfaults when attempting to access
 *  uninitialized data.
 *
 *  The structure of these Optionals is also worth discussing. They are
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
 */

#pragma once

#include "nonstd/preprocessor/homogenize.h"
#include "nonstd/cpp1z/in_place_t.h"
#include "nonstd/cpp1z/type_trait_assertions.h"
#include "nonstd/cpp1z/type_traits.h"
#include "nonstd/cpp1z/special_member_filters.h"
#include "nonstd/core/break.h"


/** Utilities
 *  ============================================================================
 */

/** Checked Optionals
 *  -----------------
 *  Because we don't use exceptions, we optionally force a validity check per
 *  value access. If we ever attempt to access a non-containing Optional, while
 *  `N2_CHECKED_OPTIONALS` is explicitly set to `true`, a forced breakpoint will
 *  occur. If we're confident that we never misuse Optionals, allowing this
 *  default-to-false to occur will let us skip that branch.
 */
#if !defined(N2_CHECKED_OPTIONALS)
#define N2_CHECKED_OPTIONALS false
#endif

namespace nonstd {

/** Null Optional Type
 *  ------------------
 *  Tag Type to explicitly create non-containing Optionals (and friends).
 *  Namespace'd to `nonstd::` to match the C++17 `std::nullopt_t`.
 */
struct nullopt_t {
    /* Explicitly delete the default constructor, and define an explicit
       constexpr constructor that will be hard to accidentally invoke.
       Let's use the `nonstd::in_place_t` tag for that. */
    nullopt_t() = delete;
    explicit constexpr nullopt_t(nonstd::in_place_t) { }
};

constexpr static nullopt_t nullopt { nonstd::in_place };


namespace exception {

/** Bad Optional Access Exception
 *  -----------------------------
 *  The exception type to be thrown when an attempt is made to access the value
 *  of a non-containing Optional.
 */
class bad_optional_access : public ::std::logic_error {
public:
    bad_optional_access()
        : ::std::logic_error("Attempted to access the value of a "
                             "non-containing Optional.")
        { }
};

} /* namespace exception */
} /* namespace nonstd */


/** Helper Defines
 *  --------------
 *  These are just here to save space, because holy crap these enable_if SFINAE
 *  clauses get verbose...
 */
#define IS_CONVERTIBLE_FROM_OPTIONAL(U, T) (     \
       IS_CONSTRUCTIBLE(T, Optional<U> &)        \
    || IS_CONSTRUCTIBLE(T, Optional<U> &&)       \
    || IS_CONSTRUCTIBLE(T, Optional<U> const &)  \
    || IS_CONSTRUCTIBLE(T, Optional<U> const &&) \
    || IS_CONVERTIBLE(Optional<U> &, T)          \
    || IS_CONVERTIBLE(Optional<U> &&, T)         \
    || IS_CONVERTIBLE(const Optional<U> &, T)    \
    || IS_CONVERTIBLE(const Optional<U> &&, T) )

#define IS_ASSIGNABLE_FROM_OPTIONAL(U, T) (     \
       IS_ASSIGNABLE(T&, Optional<U> &)         \
    || IS_ASSIGNABLE(T&, Optional<U> &&)        \
    || IS_ASSIGNABLE(T&, Optional<U> const &)   \
    || IS_ASSIGNABLE(T&, Optional<U> const &&) )

#define __OPT_ENABLE_IF_IS_CONVERTABLE(U,T) \
    ENABLE_IF_TYPE(IS_CONVERTIBLE(U,T))* = nullptr



/** Forward Declarations
 *  ============================================================================
 *  Because we have a _lot_ of classes to define and specialize in this file.
 */

/** Complete Optional Type
 *  ----------------------
 *  Will inherit from one of the `_Optional_*Base` classes, using SFINAE to
 *  expand only the salient specialization based on the value_type of `T`.
 *  Will also inherit from the `Enable[Copy|Move][Ctor|Assign]If<B,T>` helper
 *  classes to conditionally "delete" the relevant special member function based
 *  on the salient type_traits of `T`.
 */
template <typename T, typename Enable = void>
class Optional;


/** Optional Base Classes
 *  ---------------------
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



/** Optional Storage Class
 *  ============================================================================
 *  Wrapper around the `is_containing` boolean, and the value-or-no-value union.
 *  We need to give Optionals their own storage class s.t. SFINAE can be used to
 *  selectively define constructors and destructors based on `T`.
 *
 *  When storing an object that is trivially copy and move constructible, the
 *  implicitly defined copy/move ctors will be correct in all cases; the active
 *  member of the union will be determined (at compile time, if possible) and
 *  used to initialize the relevant member in the new _Storge object. Otherwise
 *  the implicit copy/move ctors will not be defined, which forces us to
 *  explicitly define them. In these cases, we also need to wait till run-time
 *  to (optionally) construct a value based on the `other`.
 *
 *  When storing a type that is not trivial destructible, the _Storage object's
 *  implicitly defined destructor won't be defined, and so needs to be
 *  explicitly created (and needs to explicitly call the destructor on the
 *  stored value). When storing a type that is trivially destructible, the
 *  implicitly defined destructor will be defined, will correctly clean up the
 *  stored value, and will match the constexpr-ness of the stored type.
 *
 *  The _LValRefStorage is _dramatically_ simpler than its counterpart, as it
 *  requires no in-place construction, and never requires a destructor (it will
 *  be storing raw pointers, and raw pointers don't need to be cleaned up). We
 *  use it primarily to maintain architecture parity between value- and
 *  reference-wrapping Optionals.
 */
template < typename T
         , bool MoveAndCopyCtorsAreTrivial =
             (    IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)
               && IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T) )
         , bool DestructorIsTrivial = IS_TRIVIALLY_DESTRUCTIBLE(T) >
struct _Optional_Storage;

template < typename T >
struct _Optional_LValRefStorage;


/**
 *  Storage for types with trivial construction and trivial destruction
 *  -------------------------------------------------------------------
 */
template < typename T >
struct _Optional_Storage<T, /* MoveAndCopyCtorsAreTrivial */ true,
                            /* DestructorIsTrivial        */ true>
{
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };


    constexpr _Optional_Storage() noexcept
        : is_containing ( false )
        , empty         {       } { }


    constexpr _Optional_Storage(_Optional_Storage const & other) = default;
    constexpr _Optional_Storage(_Optional_Storage &&      other) = default;


    constexpr _Optional_Storage(T const & value)
    noexcept(IS_NOTHROW_COPY_CONSTRUCTIBLE(T))
        : is_containing ( true  )
        , value         ( value ) { }

    constexpr _Optional_Storage(T && value)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
        : is_containing ( true             )
        , value         ( std::move(value) ) { }

    template < typename... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...))
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... ) { }

    template < typename Il, typename... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         std::initializer_list<Il> il,
                                         Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T,
                                      std::initializer_list<Il>,
                                      Args && ...))
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... ) { }
};


/** Storage for types with non-trivial construction & trivial destruction
 *  ---------------------------------------------------------------------
 */
template < typename T >
struct _Optional_Storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                            /* DestructorIsTrivial        */ true>
{
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    // The anonymous union may have be initialized to `empty`, so we use
    // placement new to guarantee the active member is switched.
    template < typename... Args >
    void _construct(Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args...)) {
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }


    constexpr _Optional_Storage() noexcept
        : is_containing ( false )
        , empty         {       } { }


    // Interestingly, we can leave the copy and move constructors `constexpr`,
    // as the compiler will be able to skip the `_construct` call if the `other`
    // is non-containing. I don't know if a compile-time, non-containing, non-
    // trivially constructible Optional will be of use, but... You can do it.
    constexpr _Optional_Storage(_Optional_Storage const & other)
    noexcept(IS_NOTHROW_COPY_CONSTRUCTIBLE(T))
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(other.value);
    }

    constexpr _Optional_Storage(_Optional_Storage && other)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(std::move(other.value));
    }


    constexpr _Optional_Storage(T const & value)
    noexcept(IS_NOTHROW_COPY_CONSTRUCTIBLE(T))
        : is_containing ( true  )
        , value         ( value ) { }

    constexpr _Optional_Storage(T && value)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
        : is_containing ( true             )
        , value         ( std::move(value) ) { }

    template < typename... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...))
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... ) { }

    template < typename Il, typename... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         std::initializer_list<Il> il,
                                         Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T,
                                      std::initializer_list<Il>,
                                      Args && ...))
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... ) { }
};


/** Storage for types with non-trivial construction & non-trivial destruction
 *  -------------------------------------------------------------------------
 */
template < typename T >
struct _Optional_Storage<T, /* MoveAndCopyCtorsAreTrivial */ false,
                            /* DestructorIsTrivial        */ false>
{
    struct _Empty { };

    bool is_containing;
    union {
        _Empty empty;
        T      value;
    };

    // The anonymous union may have be initialized to `empty`, so we use
    // placement new to guarantee the active member is switched.
    template < typename... Args >
    void _construct(Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...)) {
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }

    ~_Optional_Storage()
    noexcept(IS_NOTHROW_DESTRUCTIBLE(T)) {
        if (is_containing)
            value.~T();
    }


    constexpr _Optional_Storage() noexcept
        : is_containing ( false )
        , empty         {       } { }


    constexpr _Optional_Storage(_Optional_Storage const & other)
    noexcept(IS_NOTHROW_COPY_CONSTRUCTIBLE(T))
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(other.value);
    }

    constexpr _Optional_Storage(_Optional_Storage && other)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(std::move(other.value));
    }


    constexpr _Optional_Storage(T const & value)
    noexcept(IS_NOTHROW_COPY_CONSTRUCTIBLE(T))
        : is_containing ( true  )
        , value         ( value ) { }

    constexpr _Optional_Storage(T && value)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
        : is_containing ( true             )
        , value         ( std::move(value) ) { }

    template < typename... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...))
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... ) { }

    template < typename Il, typename... Args>
    constexpr explicit _Optional_Storage(nonstd::in_place_t,
                                         std::initializer_list<Il> il,
                                         Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T,
                                      std::initializer_list<Il>,
                                      Args && ...))
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... ) { }
};


/** Storage for LValue Reference types
 *  ----------------------------------
 */
template < typename T >
struct _Optional_LValRefStorage {
    static_assert(IS_POINTER(T),
        "_Optional_LValRefStorage expects to be specialized on a pointer type");

    bool is_containing;
    T    value;

    constexpr _Optional_LValRefStorage() noexcept
        : is_containing ( false   )
        , value         ( nullptr ) { }

    constexpr _Optional_LValRefStorage(nonstd::nullopt_t /*unused*/) noexcept
        : is_containing ( false   )
        , value         ( nullptr ) { }


    constexpr _Optional_LValRefStorage(_Optional_LValRefStorage const & other)
    noexcept
        : is_containing ( other.is_containing )
        , value         ( other.value         ) { }

    constexpr _Optional_LValRefStorage(_Optional_LValRefStorage && other)
    noexcept
        : is_containing ( other.is_containing    )
        , value         ( std::move(other.value) ) { }

    constexpr _Optional_LValRefStorage(T value) noexcept
        : is_containing ( true  )
        , value         ( value ) { }
};



/** Optional Construction Logic and Storage for Value Types
 *  =======================================================
 */

template < typename T >
class _Optional_ValueBase {
private:
    /* ### A Note on Storage Types
     * To let us use (and re-use) storage when T is cv qualified, we strip any
     * const-ness when defining our _Optional_Storage type. All public and
     * protected methods will take and return `T`s, though, so the removal of
     * the cv-qualifier should be entirely invisible. In fact, most methods in
     * this class should call `_getValue` to access `_storage.value` so that
     * they access a `T`, rather than a `REMOVE_CONST_TYPE(T)`. */
    using _Storage_Type = REMOVE_CONST_TYPE(T);
    _Optional_Storage<_Storage_Type> _storage;


    /* Optionals must all be friends to one another. */
    template <typename U> friend class _Optional_ValueBase;

public:

    /* Empty Ctors
     * ----------- */
    constexpr _Optional_ValueBase() noexcept
        : _storage ( ) { }
    constexpr _Optional_ValueBase(nonstd::nullopt_t /*unused*/) noexcept
        : _storage ( ) { }

    /* Copy Ctor
     * ---------
     * If IS_COPY_CONSTRUCTIBLE(T) == false, this constructor will be explicitly
     * deleted in Optional<T>. */
    constexpr _Optional_ValueBase(_Optional_ValueBase const & other)
    noexcept(IS_NOTHROW_COPY_CONSTRUCTIBLE(T))
        : _storage ( other._storage ) { }

    /* Move Ctor
     * ---------
     * If IS_MOVE_CONSTRUCTIBLE(T) == false, this constructor will be explicitly
     * deleted in Optional<T>. */
    constexpr _Optional_ValueBase(_Optional_ValueBase && other)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
        : _storage ( std::move(other._storage) ) { }

    /* In-Place Value Ctor
     * ------------------- */
    template < typename... Args
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T, Args...), int) = 0 >
    constexpr explicit _Optional_ValueBase(nonstd::in_place_t /*unused*/,
                                           Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...))
        : _storage ( nonstd::in_place, std::forward<Args>(args)... ) { }

    /* In-Place Initializer List Value Ctor
     * ------------------------------------ */
    template < typename Il
             , typename... Args
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T,
                                                std::initializer_list<Il> &,
                                                Args && ...), int) = 0 >
    constexpr explicit _Optional_ValueBase(nonstd::in_place_t /*unused*/,
                                           std::initializer_list<Il> il,
                                           Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T,
                                      std::initializer_list<Il> &,
                                      Args && ...))
        : _storage ( nonstd::in_place, il, std::forward<Args>(args)... ) { }

    /* Converting Value Move Ctor
     * -------------------------- */
    template < typename U = T
             , ENABLE_IF_DTYPE((   IS_DIFFERENT_TYPE(DECAY_TYPE(U),
                                                     nonstd::in_place_t)
                                && IS_DIFFERENT_TYPE(DECAY_TYPE(U), Optional<T>)
                                && IS_CONSTRUCTIBLE(T, U&&)
                                && IS_CONVERTIBLE(U&&, T))
                              , int) = 0 >
    constexpr _Optional_ValueBase(U && value)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, U &&))
        : _storage ( std::forward<U>(value) ) { }

    template < typename U = T
             , ENABLE_IF_DTYPE((   IS_DIFFERENT_TYPE(DECAY_TYPE(U),
                                                     nonstd::in_place_t)
                                && IS_DIFFERENT_TYPE(DECAY_TYPE(U), Optional<T>)
                                && IS_CONSTRUCTIBLE(T, U&&)
                                && IS_NOT_CONVERTIBLE(U&&, T))
                              , int) = 1 >
    constexpr explicit _Optional_ValueBase(U && value)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, U &&))
        : _storage ( std::forward<U>(value) ) { }

    /* Converting Copy Ctor
     * --------------------
     * NB. The value being copied is going to be a complete `Optional<U>` (not a
     *    `_Optional_ValueBase<U>`), so that's the parameter we accept. */
    template < typename U
             , ENABLE_IF_DTYPE((     IS_DIFFERENT_TYPE(T, U)
                                &&   IS_CONSTRUCTIBLE(T, U const &)
                                &&   IS_CONVERTIBLE(U const &, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 0>
    constexpr _Optional_ValueBase(Optional<U> const & other)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, U))
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase{ *other          }
                                : _Optional_ValueBase{ nonstd::nullopt } ) { }

    template < typename U
             , ENABLE_IF_DTYPE((     IS_DIFFERENT_TYPE(T, U)
                                &&   IS_CONSTRUCTIBLE(T, U const &)
                                &&   IS_NOT_CONVERTIBLE(U const &, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 1>
    constexpr explicit _Optional_ValueBase(Optional<U> const & other)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, U))
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase{ *other          }
                                : _Optional_ValueBase{ nonstd::nullopt } ) { }

    /* Converting Move Ctor
     * --------------------
     * NB. The value being moved is going to be a complete `Optional<U>` (not a
     *    `_Optional_ValueBase<U>`), so that's the parameter we accept. */
    template < typename U
             , ENABLE_IF_DTYPE((     IS_DIFFERENT_TYPE(T, U)
                                &&   IS_CONSTRUCTIBLE(T, U&&)
                                &&   IS_CONVERTIBLE(U &&, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 0>
    constexpr _Optional_ValueBase(Optional<U> && other)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, U &&))
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase{ std::move(*other) }
                                : _Optional_ValueBase{ nonstd::nullopt   } ) { }

    template < typename U
             , ENABLE_IF_DTYPE((     IS_DIFFERENT_TYPE(T, U)
                                &&   IS_CONSTRUCTIBLE(T, U&&)
                                &&   IS_NOT_CONVERTIBLE(U &&, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 1>
    constexpr explicit _Optional_ValueBase(Optional<U> && other)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, U &&))
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase{ std::move(*other) }
                                : _Optional_ValueBase{ nonstd::nullopt   } ) { }

    /* Empty Assignment
     * ---------------- */
    _Optional_ValueBase<T>& operator= (nonstd::nullopt_t /*unused*/)
    noexcept(IS_NOTHROW_DESTRUCTIBLE(T)) {
        if (_storage.is_containing) { _removeValue(); }
        return *this;
    }

    /* Copy Assignment
     * ---------------
     * If IS_COPY_CONSTRUCTIBLE(T) == false or IS_COPY_ASSIGNABLE(T) == false,
     * this Assignment Operator will be explicitly deleted in Optional<T>.
     *
     * Remarks on `noexcept`; If any exception is thrown, the result of the
     * expression bool(*this) remains unchanged. If an exception is thrown
     * during the call to T’s copy constructor, no effect. If an exception is
     * thrown during the call to T’s copy assignment, the state of its
     * contained value is as defined by the exception safety guarantee of T’s
     * copy assignment. */
    _Optional_ValueBase<T>& operator= (_Optional_ValueBase const & other)
    noexcept(   IS_NOTHROW_COPY_ASSIGNABLE(T)
             && IS_NOTHROW_COPY_CONSTRUCTIBLE(T)
             && IS_NOTHROW_DESTRUCTIBLE(T)) {
        if (_storage.is_containing && other._storage.is_containing) {
            // See the "Note on Storage Types" above.
            _getValue() = other._getValue();
        } else {
            if (other._storage.is_containing) {
                //TODO: This should maybe be wrapped in a try/catch?
                //      Maybe this needs to be another SFINAE branch, one for
                //      noexcept, one for try/catch?
                //      All _constructValue calls should be treated similarly.
                _constructValue(other._getValue());
            } else if (_storage.is_containing) {
                _removeValue();
            }
        }
        return *this;
    }

    /* Move Assignment
     * ---------------
     * If IS_MOVE_CONSTRUCTIBLE(T) == false or IS_MOVE_ASSIGNABLE(T) == false,
     * this Assignment Operator will be explicitly deleted in Optional<T>. */
    _Optional_ValueBase<T>& operator= (_Optional_ValueBase && other)
    noexcept(   IS_NOTHROW_MOVE_ASSIGNABLE(T)
             && IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)
             && IS_NOTHROW_DESTRUCTIBLE(T)) {
        if (_storage.is_containing && other._storage.is_containing) {
            // See the "Note on Storage Types" above.
            _getValue() = std::move(other._getValue());
        } else {
            if (other._storage.is_containing) {
                _constructValue(std::move(other._getValue()));
            } else if (_storage.is_containing) {
                _removeValue();
            }
        }
        return *this;
    }

    /* Converting Value Move Assignment
     * --------------------------------
     * NOTE: There is either a problem with this implementation, or the C++17
     * n4618 standard. Missing from the implemented SFINAE clauses below is the
     * requirement that "`conjunction_v<is_scalar<T>, is_same<T, decay_t<U>>>`
     * is `false`." With that clause in place, assigning a T to a containing
     * Optional results in an ambiguous implicit constructor call, but assigning
     * an implicitly-convertible-to-T succeeds. Observe,
     *     Optional<int>  foo = 42; // Works; Calls into the Converting Value
     *                              // Move Ctor, *not* an assignment operator.
     *     foo = 84; // Fails; `operator= (U&& value)` does not participate in
     *               // overload resolution because `is_scalar_v<int>` is
     *               // `true`, and `is_same_v<int, decay_t<int>>` is `true`.
     *     // error: use of overloaded operator '=' is ambiguous (with operand
     *     //         types 'Optional<int>' and 'int')
     *     // . . . candidate is the implicit move assignment operator
     *     // . . . candidate is the implicit copy assignment operator
     *
     *     Optional<long> bar = 42; // Works; Calls into the Converting Value
     *                              // Move Ctor, *not* an assignment operator.
     *     bar = 84; // Works; `operator= (U&& value)` is used because
     *               // `is_same_v<int, decay_t<long>>` is `false`.
     *
     * The missing clause is;
     *     && ! (   IS_SCALAR(T)
     *           && IS_SAME_TYPE(DECAY_TYPE(U), T))
     */
    template < typename U = T
             , ENABLE_IF_DTYPE((   IS_DIFFERENT_TYPE(DECAY_TYPE(U),
                                                     Optional<T>)
                                && IS_CONSTRUCTIBLE(T, U)
                                && IS_ASSIGNABLE(T&, U))
                              , int) = 0 >
    _Optional_ValueBase<T>& operator= (U&& value)
    noexcept(   IS_NOTHROW_ASSIGNABLE(T, U &&)
             && IS_NOTHROW_CONSTRUCTIBLE(T, U &&)) {
        if (_storage.is_containing) {
            // See the "Note on Storage Types" above.
            _getValue() = std::forward<U>(value);
        } else {
            _constructValue(std::forward<U>(value));
        }
        return *this;
    }

    /* Converting Copy Assignment
     * --------------------------
     * NB. The value being copied is going to be a complete `Optional<U>` (not a
     *    `_Optional_ValueBase<U>`), so that's the parameter we accept. */
    template < typename U
             , ENABLE_IF_DTYPE((     IS_DIFFERENT_TYPE(T, U)
                                &&   IS_CONSTRUCTIBLE(T, U const &)
                                &&   IS_ASSIGNABLE(T&, U const &)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T)
                                && ! IS_ASSIGNABLE_FROM_OPTIONAL(U, T))
                              , int) = 0 >
    _Optional_ValueBase<T>& operator= (Optional<U> const & other)
    noexcept(   IS_NOTHROW_ASSIGNABLE(T, U)
             && IS_NOTHROW_CONSTRUCTIBLE(T, U)
             && IS_NOTHROW_DESTRUCTIBLE(T)) {
        if (_storage.is_containing && other.hasValue()) {
            // See the "Note on Storage Types" above.
            _getValue() = *other;
        } else {
            if (other.hasValue()) {
                _constructValue(*other);
            } else if (_storage.is_containing) {
                _removeValue();
            }
        }
        return *this;
    }

    /* Converting Move Assignment
     * --------------------------
     * NB. The value being moved is going to be a complete `Optional<U>` (not a
     *    `_Optional_ValueBase<U>`), so that's the parameter we accept. */
    template < typename U
             , ENABLE_IF_DTYPE((     IS_DIFFERENT_TYPE(T, U)
                                &&   IS_CONSTRUCTIBLE(T, U)
                                &&   IS_ASSIGNABLE(T&, U)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T)
                                && ! IS_ASSIGNABLE_FROM_OPTIONAL(U, T))
                              , int) = 0 >
    _Optional_ValueBase<T>& operator= (Optional<U> && other)
    noexcept(   IS_NOTHROW_ASSIGNABLE(T, U &&)
             && IS_NOTHROW_CONSTRUCTIBLE(T, U &&)
             && IS_NOTHROW_DESTRUCTIBLE(T)) {
        if (_storage.is_containing && other.hasValue()) {
            // See the "Note on Storage Types" above.
            _getValue() = std::move(*other);
        } else {
            if (other.hasValue()) {
                _constructValue(std::move(*other));
            } else if (_storage.is_containing) {
                _removeValue();
            }
        }
        return *this;
    }

    /* Emplacement
     * ----------- */
    template < typename... Args
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T, Args && ...), int) = 0 >
    _Optional_ValueBase<T>& emplace(Args && ... args)
    noexcept(   IS_NOTHROW_DESTRUCTIBLE(T)
             && IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...)) {
        if (_storage.is_containing) { _storage.value.~_Storage_Type(); }
        _constructValue(std::forward<Args>(args)...);
        return *this;
    }

    /* Emplacement With Initializer List
     * --------------------------------- */
    template < typename... Args
             , typename Il
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T,
                                                std::initializer_list<Il> &,
                                                Args && ...), int) = 0 >
    _Optional_ValueBase<T>& emplace(std::initializer_list<Il> il,
                                    Args && ... args)
    noexcept (   IS_NOTHROW_DESTRUCTIBLE(T)
              && IS_NOTHROW_CONSTRUCTIBLE(T,
                                          std::initializer_list<Il> &,
                                          Args && ...)) {
        if (_storage.is_containing) { _storage.value.~_Storage_Type(); }
        _constructValue(il, std::forward<Args>(args)...);
        return *this;
    }

protected:
    /* Helper Functions
     * ----------------
     * For destructing, constructing, and fetching values. */
    void _removeValue()
    noexcept(IS_NOTHROW_DESTRUCTIBLE(T)) {
        _storage.is_containing = false;
        _storage.value.~_Storage_Type();
    }

    template < typename... Args >
    void _constructValue(Args && ... args)
    noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...)) {
        // We may be trying to switch the active member of the union in
        // _storage to a class with a user-defined constructor. Use
        // Placement New to accomplish this.
        void * val_ptr = &_storage.value;
        new (val_ptr) _Storage_Type(std::forward<Args>(args)...);
        _storage.is_containing = true;
    }

    // NB. These methods cannot be noexcept because they may access an inactive
    // member of the `_storage` class' anonymous union. Most internal uses may
    // be considered noexcept, though, due to the checks that will need to be
    // performed prior to access.
    constexpr T       & _getValue()       { return _storage.value; }
    constexpr T const & _getValue() const { return _storage.value; }

    constexpr bool _hasValue() const noexcept { return _storage.is_containing; }

};


/** Optional Construction Logic and Storage for LValue Reference Types
 *  ============================================================================
 */
template < typename T >
class _Optional_LValRefBase {
private:
    /* Reference types can't be directly stored and can't be reseated once
     * initialized. To get around this, we strip the ref qualifier and store a
     * mutable pointer to the base type of the reference. */
    using _Storage_Type = REMOVE_REFERENCE_TYPE(T) *;

    _Optional_LValRefStorage<_Storage_Type> _storage;

public:
    /* Empty Ctors
     * ----------- */
    constexpr _Optional_LValRefBase() noexcept
        : _storage ( ) { }
    constexpr _Optional_LValRefBase(nonstd::nullopt_t /*unused*/) noexcept
        : _storage ( ) { }

    /* Copy Ctor
     * --------- */
    constexpr _Optional_LValRefBase(_Optional_LValRefBase const & other)
    noexcept
        : _storage ( other._storage ) { }

    /* Move Ctor
     * --------- */
    constexpr _Optional_LValRefBase(_Optional_LValRefBase && other) noexcept
        : _storage ( std::move(other._storage) ) { }

    /* Value Ctor
     * ---------- */
    constexpr _Optional_LValRefBase(T & value) noexcept
        : _storage ( const_cast<_Storage_Type>(&value) ) { }

    /* Empty Assignment
     * ---------------- */
    _Optional_LValRefBase<T&>& operator= (nonstd::nullopt_t /*unused*/)
    noexcept {
        _removeValue();
        return *this;
    }

    /* Copy Assignment
     * --------------- */
    _Optional_LValRefBase<T&>& operator= (_Optional_LValRefBase const & other)
    noexcept {
        if (_storage.is_containing && other._storage.is_containing) {
            _storage.value = other._storage.value;
        } else {
            if (other._storage.is_containing) {
                _storage.value = other._storage.value;
                _storage.is_containing = true;
            } else {
                _removeValue();
            }
        }
        return *this;
    }

    /* Move Assignment
     * --------------- */
    _Optional_LValRefBase<T&>& operator= (_Optional_LValRefBase && other)
    noexcept {
        if (_storage.is_containing && other._storage.is_containing) {
            _storage.value = std::move(other._storage.value);
        } else {
            if (other._storage.is_containing) {
                _storage.value = std::move(other._storage.value);
                _storage.is_containing = true;
            } else {
                _removeValue();
            }
        }
        return *this;
    }

    /* Value Assignment
     * ---------------- */
    _Optional_LValRefBase<T&>& operator= (T & value) noexcept {
        _storage.value = const_cast<_Storage_Type>(&value);
        _storage.is_containing = true;
        return *this;
    }

    /* Helper Functions
     * ---------------- */
protected:
    void _removeValue() noexcept {
        _storage.is_containing = false;
        _storage.value = nullptr;
    }

    // NB. These methods cannot be noexcept because they may access an inactive
    // member of the `_storage` class' anonymous union. Most internal uses may
    // be considered noexcept, though, due to the checks that will need to be
    // performed prior to access.
    constexpr T       & _getValue()       { return *_storage.value; }
    constexpr T const & _getValue() const { return *_storage.value; }

    constexpr bool _hasValue() const noexcept { return _storage.is_containing; }

};


/** Value-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality related to storage is
 *  implemented in the _Optional_ValueBase parent. This class only deals in
 *  user-facing observers.
 */
template < typename T >
class Optional < T, ENABLE_IF_TYPE(IS_NOT_REFERENCE(T)) >
    : private _Optional_ValueBase<T>
    , private nonstd::EnableCopyCtorIf<IS_COPY_CONSTRUCTIBLE(T),    Optional<T>>
    , private nonstd::EnableCopyAssignIf<(IS_COPY_CONSTRUCTIBLE(T)
                                          && IS_COPY_ASSIGNABLE(T)),Optional<T>>
    , private nonstd::EnableMoveCtorIf<IS_MOVE_CONSTRUCTIBLE(T),    Optional<T>>
    , private nonstd::EnableMoveAssignIf<(IS_MOVE_CONSTRUCTIBLE(T)
                                          && IS_MOVE_ASSIGNABLE(T)),Optional<T>>
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
    constexpr       T *  operator-> () {
        checkValue(); return &this->_getValue();
    }
    constexpr const T *  operator-> () const {
        checkValue(); return &this->_getValue();
    }

    constexpr       T &  operator*  ()       &  {
        checkValue(); return this->_getValue();
    }
    constexpr const T &  operator*  () const &  {
        checkValue(); return this->_getValue();
    }
    constexpr       T && operator*  ()       && {
        checkValue(); return std::move(this->_getValue());
    }
    constexpr const T && operator*  () const && {
        checkValue(); return std::move(this->_getValue());
    }

    constexpr explicit operator bool () const noexcept {
        return this->_hasValue();
    }
    constexpr bool hasValue() const noexcept {
        return this->_hasValue();
    }

    constexpr       T &  value()       &  {
        checkValue(); return this->_getValue();
    }
    constexpr const T &  value() const &  {
        checkValue(); return this->_getValue();
    }
    constexpr       T && value()       && {
        checkValue(); return std::move(this->_getValue());
    }
    constexpr const T && value() const && {
        checkValue(); return std::move(this->_getValue());
    }

    template < typename U = T >
    constexpr T valueOr(U && value) const & {
        return this->_hasValue() ? this->_getValue()
                                 : static_cast<T>(std::forward<U>(value));
    }
    template < typename U = T >
    constexpr T valueOr(U && value) && {
        return this->_hasValue() ? std::move(this->_getValue())
                                 : static_cast<T>(std::forward<U>(value));
    }

    void reset()
    noexcept(IS_TRIVIALLY_DESTRUCTIBLE(T)) {
        if (this->_hasValue()) { this->_removeValue(); }
    }
};



/** LValue-Reference-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality related to storage is
 *  implemented in the _Optional_ValueBase parent. This class only deals in
 *  user-facing observers.
 */
template <typename T>
class Optional < T, ENABLE_IF_TYPE(IS_LVAL_REFERENCE(T)) >
    : _Optional_LValRefBase<T>
{
public:
    using _Optional_LValRefBase<T>::_Optional_LValRefBase;
    using _Optional_LValRefBase<T>::operator=;

    using _Base_Type = REMOVE_REFERENCE_TYPE(T);

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
    constexpr       _Base_Type *  operator-> () {
        checkValue(); return &this->_getValue();
    }
    constexpr const _Base_Type *  operator-> () const {
        checkValue(); return &this->_getValue();
    }

    constexpr       T &  operator* ()       &  {
        checkValue(); return this->_getValue();
    }
    constexpr const T &  operator* () const &  {
        checkValue(); return this->_getValue();
    }
    constexpr       T && operator* ()       && {
        checkValue(); return std::move(this->_getValue());
    }
    constexpr const T && operator* () const && {
        checkValue(); return std::move(this->_getValue());
    }

    constexpr explicit operator bool () const noexcept {
        return this->_hasValue();
    }
    constexpr bool hasValue() const noexcept {
        return this->_hasValue();
    }

    constexpr       T &  value()       &  {
        checkValue(); return this->_getValue();
    }
    constexpr const T &  value() const &  {
        checkValue(); return this->_getValue();
    }
    constexpr       T && value()       && {
        checkValue(); return std::move(this->_getValue());
    }
    constexpr const T && value() const && {
        checkValue(); return std::move(this->_getValue());
    }

    template < typename U = _Base_Type >
    constexpr _Base_Type valueOr(U && value) const & {
        return this->_hasValue()
                ? this->_getValue()
                : static_cast<_Base_Type>(std::forward<U>(value));
    }
    template < typename U = _Base_Type >
    constexpr _Base_Type valueOr(U && value) && {
        return this->_hasValue()
                ? std::move(this->_getValue())
                : static_cast<_Base_Type>(std::forward<U>(value));
    }

    void reset() noexcept {
        if (this->_hasValue()) { this->_removeValue(); }
    }
};



/** Helper Creation Functions
 *  ============================================================================
 *  When you want to specify the type as part of an assignment or return.
 */

/* Create an Optional with no value */
template<typename T>
constexpr Optional<T> none() noexcept { return Optional<T>(); }

/* Create an Optional with a real value */
template<typename T>
constexpr Optional<T> just(T value)
noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, T)) {
    return Optional<T> { value };
}
template<typename T, typename... Args>
constexpr Optional<T> just(nonstd::in_place_t /*unused*/, Args && ... args)
noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, Args && ...)) {
    return Optional<T> { nonstd::in_place, std::forward<Args>(args)... };
}
template<typename T, typename Il, typename... Args>
constexpr Optional<T> just(nonstd::in_place_t /*unused*/,
                           std::initializer_list<Il> il, Args && ... args)
noexcept(IS_NOTHROW_CONSTRUCTIBLE(T, std::initializer_list<Il>, Args && ...)) {
    return Optional<T> { nonstd::in_place, il, std::forward<Args>(args)... };
}

template<typename T>
constexpr Optional<T &> just_ref(T & value) {
    return Optional<T &> { value };
}

template<typename T>
constexpr Optional<T const &> just_cref(T const & value) {
    return Optional<T const &> { value };
}



/** Equality and Relationship Comparisons
 *  ============================================================================
 */

/** Compare two Optionals
 *  ---------------------
 *   * If both Optionals are containing, compare their values as normal.
 *   * A non-containing Optional is always considered to be less than a
 *     containing Optional.
 *   * If both Optionals are non-containing, they are considered equal.
 */
template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator == (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return false; }
    if ((bool)(lhs))                { return *lhs == *rhs; }
    return true;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator != (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return true; }
    if ((bool)(lhs))                { return *lhs != *rhs; }
    return false;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator >  (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs >  *rhs; }
    return false;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator >= (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs >= *rhs; }
    return true;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator <  (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs <  *rhs; }
    return false;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator <= (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return *lhs <= *rhs; }
    return true;
}

/** Compare an Optional to a `nullopt`
 *  ----------------------------------
 *   * A `nullopt` is always considered to be less than a containing Optional.
 *   * A `nullopt` and a non-containing Optional are considered equal.
 */
template < typename T >
constexpr inline bool operator == (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template < typename T >
constexpr inline bool operator == (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

template < typename T >
constexpr inline bool operator != (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return  (bool)(lhs);
}
template < typename T >
constexpr inline bool operator != (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return  (bool)(rhs);
}

template < typename T >
constexpr inline bool operator <  (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return false;
}
template < typename T >
constexpr inline bool operator <  (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return (bool)(rhs);
}

template < typename T >
constexpr inline bool operator <= (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template < typename T >
constexpr inline bool operator <= (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return true;
}

template < typename T >
constexpr inline bool operator >  (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return (bool)(lhs);
}
template < typename T >
constexpr inline bool operator >  (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return false;
}

template < typename T >
constexpr inline bool operator >= (Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return true;
}
template < typename T >
constexpr inline bool operator >= (nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

/** Compare an Optional and a value
 *  -------------------------------
 *   * An Empty Optional is always considered to be less than a Value.
 */
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator == (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs == rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator == (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs == *rhs;
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator != (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs != rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator != (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs != *rhs;
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator >  (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs > rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator >  (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs > *rhs;
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator >= (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && *lhs >= rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator >= (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs >= *rhs;
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator <  (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs < rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator <  (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs < *rhs;
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator <= (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || *lhs <= rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool operator <= (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs <= *rhs;
}



/** Equality and Comparisons using `nonstd::equal_to` & `nonstd::compare`
 *  ============================================================================
 */

namespace nonstd {

/** Compare two Optionals
 *  ---------------------
 *   * If both Optionals are containing, compare their values as normal.
 *   * A non-containing Optional is always considered to be less than a
 *     containing Optional.
 *   * If both Optionals are non-containing, they are considered equal.
 */
template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool equal_to(Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return false;                        }
    if ((bool)(lhs))                { return nonstd::equal_to(*lhs, *rhs); }
    return true;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr int compare (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs) ? 1 : -1;        }
    if ((bool)(lhs))                { return nonstd::compare(*lhs, *rhs); }
    return 0;
}


/** Compare an Optional to a `nullopt`
 *  ----------------------------------
 *   * A `nullopt` is always considered to be less than a containing Optional.
 *   * A `nullopt` and a non-containing Optional are considered equal.
 */
template < typename T >
constexpr inline bool equal_to(Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return !(bool)(lhs);
}
template < typename T >
constexpr inline bool equal_to(nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return !(bool)(rhs);
}

template < typename T >
constexpr inline int compare(Optional<T> const & lhs, nonstd::nullopt_t)
noexcept {
    return (bool)(lhs) ? 1 : 0;
}

template < typename T >
constexpr inline int compare(nonstd::nullopt_t, Optional<T> const & rhs)
noexcept {
    return (bool)(rhs) ? -1 : 0;
}


/** Compare an Optional and a value
 *  -------------------------------
 *   * An Empty Optional is always considered to be less than a Value.
 */
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool equal_to(Optional<T> const & lhs, Value const & rhs) {
    return (bool)(lhs) && nonstd::equal_to(*lhs, rhs);
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline bool equal_to(Value const & lhs, Optional<T> const & rhs) {
    return (bool)(rhs) && nonstd::equal_to(lhs, *rhs);
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline int compare(Optional<T> const & lhs, Value const & rhs) {
    return (bool)(lhs) ? nonstd::compare(*lhs, rhs) : -1;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr inline int compare(Value const & lhs, Optional<T> const & rhs) {
    return (bool)(rhs) ? nonstd::compare(lhs, *rhs) : 1;
}

} /* namespace nonstd */
