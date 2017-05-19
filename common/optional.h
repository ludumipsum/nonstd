/** Optional (Maybe) Type
 *  =====================
 *  Utility type for representing maybe-a-value.
 *  This implementation is based heavily on the C++17 specification for the
 *  std::optional type. Nearly all of the interface is based on the
 *  implementation described in n4618. The largest divergence is that we do not
 *  use exceptions, and so never raise `bad_optional_access`. Instead, we offer
 *  the `N2_CHECKED_OPTIONALS` compile flag; when set to `true`, we call our
 *  own CRASH macro, and otherwise allow segfaults when attempting to access
 *  uninitialized data.
 *
 *  The structure of these Optionals is also worth discussing.
 *  I will do that. I promise.
 *  Just not gonna do it now.
 *
 *  For now, a good deal of the structure is described in the Forward
 *  Declarations section, and above each class / template specialization.
 *
 *
 *  TODO:
 *   * Implement LValue-Reference-Wrapping Optionals
 *   * Consider if we want to (and how we would?) implement
 *     RValue-Reference-Wrapping Optionals
 *   * Implement a specialization on std::swap
 *   * Implement a specialization on std::hash
 */

#pragma once
#include "batteries_included.h"
#include "primitive_types.h"

#include "crash.h"


/** Utilities
 *  ============================================================================
 */

/** Checked Optionals
 *  -----------------
 *  Because we don't use exceptions, we optionally force a validity check per
 *  value access. If we ever attempt to access a non-containing Optional, while
 *  `N2_CHECKED_OPTIONALS` is explicitly set to `true`, a proper CRASH will
 *  occur. If we're confident that we never misuse Optionals, allowing this
 *  default-to-false to occur will let us skip that branch. */
#if !defined(N2_CHECKED_OPTIONALS)
#define N2_CHECKED_OPTIONALS false
#endif

/** Null Optional Type
 *  ------------------
 *  Tag Type to explicitly create non-containing Optionals (and friends).
 *  Namespace'd to `n2_` to match the C++17 `std::nullopt_t`.
 */
namespace n2_ {

struct nullopt_t {
    /* Explicitly delete the default constructor, and define an explicit
     * constexpr constructor that will be hard to accidentally invoke.
     * Let's use the `n2_::in_place_t` tag for that. */
    nullopt_t() = delete;
    explicit constexpr nullopt_t(n2_::in_place_t) { }
};

constexpr static nullopt_t nullopt { n2_::in_place };

} /* namespace n2 */


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
    typename ENABLE_IF_TYPE(IS_CONVERTIBLE(U,T))* = nullptr



/** Forward Declarations
 *  ============================================================================
 *  Because we have a _lot_ of classes to define and specialize in this file.
 */

/** Complete Optional Type
 *  ----------------------
 *  Will inherit from one of the `_Optional_*Base` classes, using SFINAE to
 *  expand only the salient specialization based on the value_type of `T`.
 *  Will also (maybe?) inherit from the `_Enable[Copy|Move][Ctor|Assign]<B,T>`
 *  helper classes to conditionally delete the relevant special member function
 *  based on the salient type_traits of `T`.
 */
template <typename T, typename Enable = void>
class Optional;


/** Optional Base Classes
 *  ---------------------
 *  Will contain all the necessary storage and construction logic used by the
 *  complete Optional sub-type s.t. we can build Optionals assuming all special
 *  member functions will be present. The sub-types will use Constructor
 *  Inheritance (`using _Optional_*Base::_Optional_*Base`) to pull in all Base
 *  constructors s.t. the complete Optional will be able to correctly initialize
 *  without needing any details regarding the storage of its value. */
template <typename T>
class _Optional_ValueBase;


template <typename T>
class _Optional_LvalReferenceBase;



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
 */
template < typename T
         , bool MoveAndCopyCtorsAreTrivial =
             (    IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)
               && IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T) )
         , bool DestructorIsTrivial = IS_TRIVIALLY_DESTRUCTIBLE(T) >
struct _Optional_Storage;


/**
 *  Storage for types with trivial construction and trivial destruction
 *  ------------------------------------------------------------------- */
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


    constexpr _Optional_Storage()
        : is_containing ( false )
        , empty         {       } { }

    constexpr _Optional_Storage(n2_::nullopt_t /*unused*/)
        : is_containing ( false )
        , empty         {       } { }


    constexpr _Optional_Storage(_Optional_Storage const & other) = default;
    constexpr _Optional_Storage(_Optional_Storage && other) = default;


    constexpr _Optional_Storage(T const & value)
        : is_containing ( true  )
        , value         ( value ) { }

    constexpr _Optional_Storage(T && value)
        : is_containing ( true             )
        , value         ( std::move(value) ) { }

    template < typename... Args>
    constexpr _Optional_Storage(n2_::in_place_t,
                                Args && ... args)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... ) { }

    template < typename Il, typename... Args>
    constexpr _Optional_Storage(n2_::in_place_t,
                                std::initializer_list<Il> il,
                                Args && ... args)
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... ) { }
};


/** Storage for types with non-trivial construction & trivial destruction
 *  --------------------------------------------------------------------- */
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
    noexcept ( IS_NOTHROW_CONSTRUCTIBLE(T, Args...) ) {
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }


    constexpr _Optional_Storage()
        : is_containing ( false )
        , empty         {       } { }

    constexpr _Optional_Storage(n2_::nullopt_t /*unused*/)
        : is_containing ( false )
        , empty         {       } { }


    // Interestingly, we can leave the copy and move constructors `constexpr`,
    // as the compiler will be able to skip the `_construct` call if the `other`
    // is non-containing. I don't know if a compile-time, non-containing, non-
    // trivially constructible Optional will be of use, but... You can do it.
    constexpr _Optional_Storage(_Optional_Storage const & other)
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(other.value);
    }

    constexpr _Optional_Storage(_Optional_Storage && other)
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(std::move(other.value));
    }


    constexpr _Optional_Storage(T const & value)
        : is_containing ( true  )
        , value         ( value ) { }

    constexpr _Optional_Storage(T && value)
        : is_containing ( true             )
        , value         ( std::move(value) ) { }

    template < typename... Args>
    constexpr _Optional_Storage(n2_::in_place_t,
                                Args && ... args)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... ) { }

    template < typename Il, typename... Args>
    constexpr _Optional_Storage(n2_::in_place_t,
                                std::initializer_list<Il> il,
                                Args && ... args)
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... ) { }
};


/** Storage for types with non-trivial construction & non-trivial destruction
 *  ------------------------------------------------------------------------- */
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
    noexcept ( IS_NOTHROW_CONSTRUCTIBLE(T, Args...) ) {
        new ((void*)(&value)) T(std::forward<Args>(args)...);
        is_containing = true;
    }

    ~_Optional_Storage() {
        if (is_containing)
            value.~T();
    }


    constexpr _Optional_Storage()
        : is_containing ( false )
        , empty         {       } { }

    constexpr _Optional_Storage(n2_::nullopt_t /*unused*/)
        : is_containing ( false )
        , empty         {       } { }


    constexpr _Optional_Storage(_Optional_Storage const & other)
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(other.value);
    }

    constexpr _Optional_Storage(_Optional_Storage && other)
        : is_containing ( other.is_containing )
        , empty         {                     }
    {
        if (other.is_containing)
            _construct(std::move(other.value));
    }


    constexpr _Optional_Storage(T const & value)
        : is_containing ( true  )
        , value         ( value ) { }

    constexpr _Optional_Storage(T && value)
        : is_containing ( true             )
        , value         ( std::move(value) ) { }

    template < typename... Args>
    constexpr _Optional_Storage(n2_::in_place_t,
                                Args && ... args)
        : is_containing ( true                        )
        , value         ( std::forward<Args>(args)... ) { }

    template < typename Il, typename... Args>
    constexpr _Optional_Storage(n2_::in_place_t,
                                std::initializer_list<Il> il,
                                Args && ... args)
        : is_containing ( true                            )
        , value         ( il, std::forward<Args>(args)... ) { }
};



/** Optional Construction Logic and Storage for Value Types
 *  ============================================================================
 */

template < typename T >
class _Optional_ValueBase {
private:
    /* To let us use (and re-use) storage when T is cv qualified, we strip that
     * const-ness when defining our _Optional_Storage type. All public and
     * protected methods will take and return `T`s, though, so the removal of
     * the cv-qualifier should be entirely invisible. */
    using _Storage_Type = REMOVE_CONST_TYPE(T);
    _Optional_Storage<_Storage_Type> _storage;


    /* Optionals must all be friends to one another. */
    template <typename U> friend class _Optional_ValueBase;

public:

    /* Empty Ctors
     * ----------- */
    constexpr _Optional_ValueBase()
        : _storage ( ) { }
    constexpr _Optional_ValueBase(n2_::nullopt_t /*unused*/)
        : _storage ( ) { }

    /* Copy Ctor
     * ---------
     * If IS_COPY_CONSTRUCTIBLE(T) == false, this constructor will be explicitly
     * deleted in Optional<T>. */
    constexpr _Optional_ValueBase(_Optional_ValueBase const & other)
        : _storage ( other._storage ) { }

    /* Move Ctor
     * ---------
     * If IS_MOVE_CONSTRUCTIBLE(T) == false, this constructor will be explicitly
     * deleted in Optional<T>. */
    constexpr _Optional_ValueBase(_Optional_ValueBase && other)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
        : _storage ( std::move(other._storage.value) ) { }

    /* In-Place Value Ctor
     * ------------------- */
    template < typename... Args
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T, Args...), int) = 0 >
    constexpr _Optional_ValueBase(n2_::in_place_t /*unused*/, Args && ... args)
        : _storage ( n2_::in_place, std::forward<Args>(args)... ) { }

    /* In-Place Initializer List Value Ctor
     * ------------------------------------ */
    template < typename Il
             , typename... Args
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T, std::initializer_list<Il>&,
                                                Args && ...), int) = 0 >
    constexpr _Optional_ValueBase(n2_::in_place_t /*unused*/,
                                  std::initializer_list<Il> il,
                                  Args && ... args)
        : _storage ( n2_::in_place, il, std::forward<Args>(args)... ) { }

    /* Converting Move Value Ctor
     * -------------------------- */
    template < typename U = T
             , ENABLE_IF_DTYPE((   IS_CONSTRUCTIBLE(T, U&&)
                                && IS_DIFFERENT_TYPE(DECAY_TYPE(U),
                                                     n2_::in_place_t)
                                && IS_DIFFERENT_TYPE(DECAY_TYPE(U), Optional<T>)
                                && IS_CONVERTIBLE(U&&, T))
                              , int) = 0 >
    constexpr _Optional_ValueBase(U && value)
        : _storage ( std::forward<U>(value) ) { }

    template < typename U = T
             , ENABLE_IF_DTYPE((   IS_CONSTRUCTIBLE(T, U&&)
                                && IS_DIFFERENT_TYPE(DECAY_TYPE(U),
                                                     n2_::in_place_t)
                                && IS_DIFFERENT_TYPE(DECAY_TYPE(U), Optional<T>)
                                && IS_NOT_CONVERTIBLE(U&&, T))
                              , int) = 1 >
    constexpr explicit _Optional_ValueBase(U && value)
        : _storage ( std::forward<U>(value) ) { }

    /* Converting Copy Ctor
     * --------------------
     * NB. The value being copied is going to be a complete `Optional<U>` (not a
     *    `_Optional_ValueBase<U>`), so that's the parameter we accept. */
    template < typename U
             , ENABLE_IF_DTYPE((     IS_CONSTRUCTIBLE(T, U const &)
                                &&   IS_CONVERTIBLE(U const &, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 0>
    constexpr _Optional_ValueBase(Optional<U> const & other)
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase(*other)
                                : _Optional_ValueBase(n2_::nullopt) ) { }

    template < typename U
             , ENABLE_IF_DTYPE((     IS_CONSTRUCTIBLE(T, U const &)
                                &&   IS_NOT_CONVERTIBLE(U const &, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 1>
    constexpr explicit _Optional_ValueBase(Optional<U> const & other)
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase(*other)
                                : _Optional_ValueBase(n2_::nullopt) ) { }

    /* Converting Move Ctor
     * --------------------
     * NB. The value being moved is going to be a complete `Optional<U>` (not a
     *    `_Optional_ValueBase<U>`), so that's the parameter we accept. */
    template < typename U
             , ENABLE_IF_DTYPE((     IS_CONSTRUCTIBLE(T, U &&)
                                &&   IS_CONVERTIBLE(U &&, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 0>
    constexpr _Optional_ValueBase(Optional<U> && other)
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase(std::move(*other))
                                : _Optional_ValueBase(n2_::nullopt)      ) { }

    template < typename U
             , ENABLE_IF_DTYPE((     IS_CONSTRUCTIBLE(T, U &&)
                                &&   IS_NOT_CONVERTIBLE(U &&, T)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T))
                              , int) = 1>
    constexpr explicit _Optional_ValueBase(Optional<U> && other)
        : _Optional_ValueBase ( other.hasValue()
                                ? _Optional_ValueBase(std::move(*other))
                                : _Optional_ValueBase(n2_::nullopt)      ) { }

    /* Empty Assignment
     * ---------------- */
    _Optional_ValueBase<T>& operator= (n2_::nullopt_t /*unused*/) noexcept {
        if (_storage.is_containing) { _removeValue(); }
        return *this;
    }

    /* Copy Assignment
     * ---------------
     * If IS_COPY_CONSTRUCTIBLE(T) == false or IS_COPY_ASSIGNABLE(T) == false,
     * this Assignment Operator will be explicitly deleted in Optional<T>. */
    _Optional_ValueBase<T>& operator= (_Optional_ValueBase const & other) {
        if (_storage.is_containing && other._storage.is_containing) {
            // Use _getValue to assign to a `T`, rather than a `REMOVE_CONST(T)`
            _getValue() = other._getValue();
        } else {
            if (other._storage.is_containing) {
                _constructValue(other._getValue());
            } else {
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
             && IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)) {
        if (_storage.is_containing && other._storage.is_containing) {
            // Use _getValue to assign to a `T`, rather than a `REMOVE_CONST(T)`
            _getValue() = std::move(other._getValue());
        } else {
            if (other._storage.is_containing) {
                _constructValue(std::move(other._getValue()));
            } else {
                _removeValue();
            }
        }
        return *this;
    }

    /* Converting Move Value Assignment
     * -------------------------------- */
    template < typename U = T
             , ENABLE_IF_DTYPE((     IS_DIFFERENT_TYPE(DECAY_TYPE(U),
                                                       Optional<T>)
                                && ! (   IS_SCALAR(T)
                                      && IS_SAME_TYPE(DECAY_TYPE(U), T))
                                &&   IS_CONSTRUCTIBLE(T, U)
                                &&   IS_ASSIGNABLE(T&, U))
                              , int) = 0 >
    _Optional_ValueBase<T>& operator= (U&& value) {
        if (_storage.is_containing) {
            // Use _getValue to assign to a `T`, rather than a `REMOVE_CONST(T)`
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
             , ENABLE_IF_DTYPE((     IS_CONSTRUCTIBLE(T, U const &)
                                &&   IS_ASSIGNABLE(T&, U const &)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T)
                                && ! IS_ASSIGNABLE_FROM_OPTIONAL(U, T))
                              , int) = 0 >
    _Optional_ValueBase<T>& operator= (Optional<U> const & other) {
        if (_storage.is_containing && other.hasValue()) {
            // Use _getValue to assign to a `T`, rather than a `REMOVE_CONST(T)`
            _getValue() = *other;
        } else {
            if (other.hasValue()) {
                _constructValue(*other);
            } else {
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
             , ENABLE_IF_DTYPE((     IS_CONSTRUCTIBLE(T, U)
                                &&   IS_ASSIGNABLE(T&, U)
                                && ! IS_CONVERTIBLE_FROM_OPTIONAL(U, T)
                                && ! IS_ASSIGNABLE_FROM_OPTIONAL(U, T))
                              , int) = 0 >
    _Optional_ValueBase<T>& operator= (Optional<U> && other) {
        if (_storage.is_containing && other.hasValue()) {
            // Use _getValue to assign to a `T`, rather than a `REMOVE_CONST(T)`
            _getValue() = std::move(*other);
        } else {
            if (other.hasValue()) {
                _constructValue(std::move(*other));
            } else {
                _removeValue();
            }
        }
        return *this;
    }

    /* Emplacement
     * ----------- */
    template < typename... Args
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T, Args && ...), int) = 0 >
    _Optional_ValueBase<T>& emplace(Args && ... args) {
        if (_storage.is_containing) { _storage.value.~_Storage_Type(); }
        _constructValue(std::forward<Args>(args)...);
        return *this;
    }

    /* Emplacement With Initializer List
     * --------------------------------- */
    template < typename... Args
             , typename Il
             , ENABLE_IF_DTYPE(IS_CONSTRUCTIBLE(T, std::initializer_list<Il> &,
                                                Args && ...), int) = 0 >
    _Optional_ValueBase<T>& emplace(std::initializer_list<Il> il,
                                    Args && ... args) {
        if (_storage.is_containing) { _storage.value.~_Storage_Type(); }
        _constructValue(il, std::forward<Args>(args)...);
        return *this;
    }

protected:
    /* Helper Functions
     * ----------------
     * For destructing, constructing, and fetching values. */
    void _removeValue() {
        _storage.value.~_Storage_Type();
        _storage.is_containing = false;
    }

    template < typename... Args >
    void _constructValue(Args && ... args) {
        // We may be trying to switch the active member of the union in
        // _storage to a class with a user-defined constructor. Use
        // Placement New to accomplish this.
        void * val_ptr = &_storage.value;
        new (val_ptr) _Storage_Type(std::forward<Args>(args)...);
        _storage.is_containing = true;
    }

    constexpr T & _getValue() noexcept { return _storage.value; }
    constexpr T const & _getValue() const noexcept { return _storage.value; }

    constexpr bool _hasValue() const noexcept { return _storage.is_containing; }

};


/** Optional Construction Logic and Storage for LValue Reference Types
 *  ============================================================================
 */
template < typename T >
class _Optional_LvalReferenceBase {
    // TODO: Implement LValue Reference Optionals.
};


/** Value-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality is implemented in the
 *  _Optional_ValueBase parent.
 *  TODO: Write up a justification and description of this architecture.
 */
template < typename T >
class Optional < T, ENABLE_IF_TYPE(IS_NOT_REFERENCE_TYPE(T)) >
    : private _Optional_ValueBase<T>
    , private n2_::_EnableCopyCtor<IS_COPY_CONSTRUCTIBLE(T),     Optional<T>>
    , private n2_::_EnableCopyAssign<(IS_COPY_CONSTRUCTIBLE(T)
                                      && IS_COPY_ASSIGNABLE(T)), Optional<T>>
    , private n2_::_EnableMoveCtor<IS_MOVE_CONSTRUCTIBLE(T),     Optional<T>>
    , private n2_::_EnableMoveAssign<(IS_MOVE_CONSTRUCTIBLE(T)
                                      && IS_MOVE_ASSIGNABLE(T)), Optional<T>>
{
public:
    using _Optional_ValueBase<T>::_Optional_ValueBase;
    using _Optional_ValueBase<T>::operator=;
    using _Optional_ValueBase<T>::emplace;
    // TODO: Test that the second two of these `using` declarations are needed.

private:
    /* Helper function to (optionally) check the validity of this Optional. */
    constexpr void checkValue() const {
#if N2_CHECKED_OPTIONALS
        if (!this->_hasValue()) { BREAKPOINT(); }
#endif
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

    // TODO: I don't think I actually want these... Would rather standardize on
    //       the pointer-like access semantics.
    constexpr       T &  operator() ()       &  {
        checkValue(); return this->_getValue();
    }
    constexpr const T &  operator() () const &  {
        checkValue(); return this->_getValue();
    }
    constexpr       T && operator() ()       && {
        checkValue(); return std::move(this->_getValue());
    }
    constexpr const T && operator() () const && {
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
        checkValue(); return this->_getValue();
    }
    constexpr const T && value() const && {
        checkValue(); return this->_getValue();
    }

    template < typename U = T >
    constexpr T valueOr(U && value) const & {
        return this->_hasValue() ? this->_getValue()
                                 : static_cast<T>(std::forward<U>(value));
    }
    template < typename U = T >
    constexpr T valueOr(U && value) && {
        return this->_hasValue() ? this->_getValue()
                                 : static_cast<T>(std::forward<U>(value));
    }

    void reset() noexcept {
        if (this->_hasValue()) { this->_removeValue(); }
    }
};



/** LValue-Reference-Wrapping Optional
 *  ============================================================================
 *  Note that nearly all meaningful functionality is implemented in the
 *  _Optional_ValueBase parent.
 *  TODO: Write up a justification and description of this architecture.
 */

template <typename T>
class Optional < T, ENABLE_IF_TYPE(IS_LVAL_REFERENCE_TYPE(T)) >
    : _Optional_LvalReferenceBase<T>
{
    // TODO: Implement LValue Reference Optionals
};



/** Helper Creation Functions
 *  ============================================================================
 *  When you want to specify the type as part of an assignment or return.
 */

/* Create an Optional with no value */
template<typename T>
constexpr Optional<T> none() { return Optional<T>(); }

/* Create an Optional with a real value */
template<typename T>
constexpr Optional<T> just(T value) {
    return Optional<T> { value };
}
template<typename T, typename... Args>
constexpr Optional<T> just(Args && ... args) {
    return Optional<T> { n2_::in_place, std::forward<Args>(args)... };
}
template<typename T, typename Il, typename... Args>
constexpr Optional<T> just(std::initializer_list<Il> il, Args && ... args) {
    return Optional<T> { n2_::in_place, il, std::forward<Args>(args)... };
}

template<typename T>
constexpr Optional<T &> just_ref(T & value) {
    return Optional<T &> { value };
}

template<typename T>
constexpr Optional<T const &> just_cref(T const& value) {
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
    if ((bool)(lhs))                { return lhs() == rhs(); }
    return true;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator != (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return true; }
    if ((bool)(lhs))                { return lhs() != rhs(); }
    return false;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator >  (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return lhs() >  rhs(); }
    return false;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator >= (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return (bool)(lhs); }
    if ((bool)(lhs))                { return lhs() >= rhs(); }
    return true;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator <  (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return lhs() <  rhs(); }
    return false;
}

template <typename T, typename U=T, __OPT_ENABLE_IF_IS_CONVERTABLE(U,T)>
constexpr bool operator <= (Optional<T> const & lhs, Optional<U> const & rhs) {
    if ((bool)(lhs) != (bool)(rhs)) { return ! (bool)(lhs); }
    if ((bool)(lhs))                { return lhs() <= rhs(); }
    return true;
}

/** Compare an Optional to a `nullopt`
 *  ----------------------------------
 *   * A `nullopt` is always considered to be less than a containing Optional.
 *   * A `nullopt` and a non-containing Optional are considered equal.
 */
template < typename T >
constexpr bool operator == (Optional<T> const & lhs, n2_::nullopt_t) noexcept {
    return !(bool)(lhs);
}
template < typename T >
constexpr bool operator == (n2_::nullopt_t, Optional<T> const & rhs) noexcept {
    return !(bool)(rhs);
}

template < typename T >
constexpr bool operator != (Optional<T> const & lhs, n2_::nullopt_t) noexcept {
    return  (bool)(lhs);
}
template < typename T >
constexpr bool operator != (n2_::nullopt_t, Optional<T> const & rhs) noexcept {
    return  (bool)(rhs);
}

template < typename T >
constexpr bool operator <  (Optional<T> const & lhs, n2_::nullopt_t) noexcept {
    return false;
}
template < typename T >
constexpr bool operator <  (n2_::nullopt_t, Optional<T> const & rhs) noexcept {
    return (bool)(rhs);
}

template < typename T >
constexpr bool operator <= (Optional<T> const & lhs, n2_::nullopt_t) noexcept {
    return !(bool)(lhs);
}
template < typename T >
constexpr bool operator <= (n2_::nullopt_t, Optional<T> const & rhs) noexcept {
    return true;
}

template < typename T >
constexpr bool operator >  (Optional<T> const & lhs, n2_::nullopt_t) noexcept {
    return (bool)(lhs);
}
template < typename T >
constexpr bool operator >  (n2_::nullopt_t, Optional<T> const & rhs) noexcept {
    return false;
}

template < typename T >
constexpr bool operator >= (Optional<T> const & lhs, n2_::nullopt_t) noexcept {
    return true;
}
template < typename T >
constexpr bool operator >= (n2_::nullopt_t, Optional<T> const & rhs) noexcept {
    return !(bool)(rhs);
}

/** Compare an Optional and a value
 *  -------------------------------
 *   * An Empty Optional is always considered to be less than a Value.
 */
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator == (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && lhs() == rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator == (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs == rhs();
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator != (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || lhs() != rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator != (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs != rhs();
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator >  (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && lhs() > rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator >  (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs > rhs();
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator >= (Optional<T> const & lhs, Value const & rhs) {
    return   (bool)(lhs) && lhs() >= rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator >= (Value const & lhs, Optional<T> const & rhs) {
    return ! (bool)(rhs) || lhs >= rhs();
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator <  (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || lhs() < rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator <  (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs < rhs();
}

template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator <= (Optional<T> const & lhs, Value const & rhs) {
    return ! (bool)(lhs) || lhs() <= rhs;
}
template <typename T, typename Value=T, __OPT_ENABLE_IF_IS_CONVERTABLE(Value,T)>
constexpr bool operator <= (Value const & lhs, Optional<T> const & rhs) {
    return   (bool)(rhs) && lhs <= rhs();
}
