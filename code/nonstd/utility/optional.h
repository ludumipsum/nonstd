/** Optional Type
 *  =============
 *  Utility type for representing maybe-a-value.
 *
 *  An optional object is an object that contains the storage for another object
 *  and manages the lifetime of this contained object, if any. The contained
 *  object may be initialized after the optional object has been initialized,
 *  and may be destroyed before the optional object has been destroyed.
 *
 *  This implementation should strictly adhere to the C++ specification n4713;
 *  https://github.com/cplusplus/draft/blob/master/papers/n4713.pdf
 *  For ease of navigation, the 2018-01-05 publication of eel.is was used;
 *  http://eel.is/c++draft/
 *
 *  All section references (§) refer to the above documents.
 *
 *  The structure of these optionals is also worth discussing. They are
 *  composed of three parts;
 *
 *   - The `optional_storage<T>` component.
 *     Contains the wrapped value (or doesn't), and the `is_containing` boolean.
 *     The `optional_storage` understands how to construct and destruct stored
 *     values irrespective of `T`s const-ness. See optional_storage.h for the
 *     implementation details thereof.
 *
 *   - The intermediate `_optional_*_base<T>` class template.
 *     This class is required to correctly make use of the `Enable[Member]If`
 *     utility classes that will conditionally disable special member functions
 *     based on the type traits of `T`. See special_member_filters.h for
 *     implementation details thereof.
 *     Because of the necessary inheritance graph, no special member functions
 *     may be defined on `optional<T>`. As such, the `optional_storage` member
 *     will be stored on the `_optional_*_base` class. We will define as few
 *     members functions on this class as possible, so there  will be no
 *     (public) observers or value assignment operators.
 *
 *   - The complete `optional<T>` template class.
 *     Inherits from either `_optional_val_base` or `_optional_ref_base`, as
 *     well as the `Enable[Member]If` utility classes. A side effect of
 *     inheriting from a `_base` class means it can be entirely unaware of the
 *     `optional_storage` member. It will implement as many of the constructors
 *     and mutators as possible, all in terms of the `_base` class, and will be
 *     responsible for all of the observers and special algorithms.
 *
 *  Author's note: I say this class strictly adheres to the standard, but we do
 *  diverge on two major points;
 *  First, we allow optionals to wrap referential types. There are valid reasons
 *  not to do this, but I'm curious if there's value in the concept. Best way to
 *  find out is to not restrict their use. Luckily, the implementation of an
 *  `optional<T&>` is dramatically simpler than an `optional<T>` (effectively
 *  it's an `optional<T*>`) so we can cut out all conversion operators, emplace,
 *  value_or... Pretty much everything that's hard.
 *  Second, we don't specialize on std::hash because... I'm not sure what that
 *  should really look like. What should the hash of a non-containing optional
 *  be? Is it even safe to specialize a struct that's part of std::? Maybe
 *  there are answers to these questions but... I don't have them at present.
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
#include <nonstd/utility/optional_storage.h>


/** Utilities
 *  ============================================================================
 */
namespace nonstd {

/** Null Optional Type -- §23.6.4
 *  -----------------------------
 *  The struct `nullopt_t` is an empty structure type used as a unique type to
 *  indicate the state of not containing a value for optional objects. In
 *  particular, `optional<T>` has a constructor with `nullopt_t` as a single
 *  argument; this indicates that an optional object not containing a value
 *  shall be constructed.
 *
 *  This class should be difficult to accidentally construct. In fact, it
 *  _should_ be completely impossible to construct outside of this file. To get
 *  close to that second requirement, define no default constructor and require
 *  the `in_place` tag to construct at all.
 */
struct nullopt_t {
    constexpr explicit nullopt_t(nonstd::in_place_t /*unused*/) noexcept { }
};
inline constexpr nullopt_t nullopt { nonstd::in_place };


namespace exception {

/** Bad Optional Access Exception -- §23.6.5
 *  ----------------------------------------
 *  The class `bad_optional_access` defines the type of objects thrown as
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
 */

/** Optional Class Template -- §23.6.3
 *  ----------------------------------
 *  The end product. The Maybe-A-Value type.
 */
template <typename T>
class optional;


/** Optional Base-Class Templates -- nonstd
 *  ---------------------------------------
 */
template <typename T>
class _optional_val_base;

template <typename T>
class _optional_ref_base;


/** Helpers
 *  ============================================================================
 *  These are just here to save space, because holy crap these `enable_if`
 *  SFINAE clauses get verbose...
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


/** Base-Class for Value-Wrapping Optional Types
 *  ============================================================================
 */
template <typename T>
class _optional_val_base {
public:
    using value_type = T;
    using pointer_type = T*;

private:
    optional_storage<T> m_storage;

public:
    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     *
     *  NB. These are duplicated on `optional<T>`.
     */
    constexpr _optional_val_base() noexcept
        : m_storage ( )
    { }
    constexpr _optional_val_base(nonstd::nullopt_t /*unused*/) noexcept
        : m_storage ( )
    { }

    /** Copy Ctor -- §23.6.3.1.3
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `*rhs`.
     *
     *  If `std::is_copy_constructible_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableCopyCtorIf`, which `optional<T>`
     *  is an inheritor of.
     *
     *  This constructor will only be (successfully) `constexpr` if `T` is
     *  trivially copy constructible; `optional_storage` will only have a
     *  `constexpr` copy constructor if `is_trivially_copy_constructible_v<T>`
     *  is `true`.
     *
     *  NB. We copy `m_storage` here, not `_get_value()`, because the later
     *  assumes storage is containing. That is a bad assumption.
     *
     *  NB. This is _not_ duplicated on `optional<T>`.
     */
    constexpr _optional_val_base(_optional_val_base const & rhs)
    noexcept(std::is_nothrow_copy_constructible_v<T>)
        : m_storage ( rhs.m_storage )
    { }

    /** Move Ctor -- §23.6.3.1.7
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `std::move(*rhs)`.
     *  **This will not change `bool(rhs)`**.
     *
     *  If `std::is_move_constructible_v<T> == false`, this constructor will be
     *  explicitly deleted by `nonstd::EnableMoveCtorIf`, which `optional<T>`
     *  is an inheritor of.
     *
     *  This constructor will only be (successfully) `constexpr` if `T` is
     *  trivially move constructible; `optional_storage` will only have a
     *  `constexpr` move constructor if `is_trivially_move_constructible_v<T>`
     *  is `true`.
     *
     *  NB. We copy `m_storage` here, not `_get_value()`, because the later
     *  assumes storage is containing. That is a bad assumption.
     *
     *  NB. This is _not_ duplicated on `optional<T>`.
     */
    constexpr _optional_val_base(_optional_val_base && rhs)
    noexcept(std::is_nothrow_move_constructible_v<T>)
        : m_storage ( std::move(rhs.m_storage) )
    { }

    /** In-Place Argument Forwarding Ctor -- §23.6.3.1.11
     *  -------------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the arguments `std::forward<Args>(args)...`.
     *
     *  NB. This is duplicated on `optional<T>`.
     */
    template < typename ... Args
             , typename std::enable_if_t< std::is_constructible_v<T, Args...>
                                        , int > = 0 >
    constexpr explicit _optional_val_base(nonstd::in_place_t /*unused*/,
                                          Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : m_storage ( nonstd::in_place, std::forward<Args>(args)... )
    { }

    /** In-Place Initializer List Ctor -- §23.6.3.1.15
     *  ----------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the arguments `il, std::forward<Args>(args)...`.
     *
     *  NB. This is duplicated on `optional<T>`.
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
        : m_storage ( nonstd::in_place, il, std::forward<Args>(args)... )
    { }

    /** Converting Value Move Ctor -- §23.6.3.1.20
     *  ------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the expression `std::forward<U>(v)`.
     *
     *  NB. These are duplicated on `optional<T>`.
     */
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<std::decay_t<U>, nonstd::in_place_t>
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                           &&  std::is_constructible_v<T, U&&>
                                           &&  std::is_convertible_v<U &&, T>)
                                        , int > = 0 >
    constexpr _optional_val_base(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : m_storage ( std::forward<U>(value) )
    { }

    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<std::decay_t<U>, nonstd::in_place_t>
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                           &&  std::is_constructible_v<T, U&&>
                                           && !std::is_convertible_v<U &&, T>)
                                        , int > = 1 >
    constexpr explicit _optional_val_base(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : m_storage ( std::forward<U>(value) )
    { }

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
     *
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged.
     *
     *  NB. This is _not_ duplicated on `optional<T>`.
     */
    _optional_val_base& operator= (_optional_val_base const & rhs)
    noexcept(   std::is_nothrow_copy_assignable_v<T>
             && std::is_nothrow_copy_constructible_v<T>
             && std::is_nothrow_destructible_v<T>) {
        if (_has_value() && rhs._has_value()) {
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
     *
     *  If any exception is thrown, the result of the expression `bool(*this)`
     *  shall remain unchanged. The state of `*rhs.val` -- whether the exception
     *  is thrown during move construction or move assignment -- shall be
     *  defined by the safety guarantees of the relevant opeartion on `T`.
     *
     *  NB. This is _not_ duplicated on `optional<T>`.
     */
    _optional_val_base& operator= (_optional_val_base && rhs)
    noexcept(   std::is_nothrow_move_assignable_v<T>
             && std::is_nothrow_move_constructible_v<T>
             && std::is_nothrow_destructible_v<T>) {
        if (_has_value() && rhs._has_value()) {
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

protected:
    constexpr bool _has_value() const noexcept {
        return m_storage.has_value();
    }

    constexpr T       &  _get_value()       &  {
        return m_storage.get_value();
    }
    constexpr T const &  _get_value() const &  {
        return m_storage.get_value();
    }
    constexpr T       && _get_value()       && {
        return std::move(m_storage.get_value());
    }
    constexpr T const && _get_value() const && {
        return std::move(m_storage.get_value());
    }

    template <typename ... Args>
    void _construct_value(Args && ... args)
    noexcept(std::is_nothrow_constructible_v<optional_storage<T>, Args...>) {
        m_storage.construct_value(std::forward<Args>(args)...);
    }

    void _remove_value()
    noexcept(std::is_nothrow_destructible_v<optional_storage<T>>) {
        m_storage.remove_value();
    }
};


/** `optional<T>`
 *  ============================================================================
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
    using value_type = T;
    using pointer_type = T*;

    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     */
    constexpr optional() noexcept = default;
    constexpr optional(nonstd::nullopt_t /*unused*/) noexcept
        : _optional_val_base<T> ( nonstd::nullopt )
    { }

    /** In-Place Argument Forwarding Ctor -- §23.6.3.1.11
     *  -------------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the arguments `std::forward<Args>(args)...`.
     */
    template < typename ... Args
             , typename std::enable_if_t< std::is_constructible_v<T, Args...>
                                        , int > = 0 >
    constexpr explicit optional(nonstd::in_place_t /*unused*/,
                                Args && ... args)
    noexcept(std::is_nothrow_constructible_v<T, Args && ...>)
        : _optional_val_base<T> ( nonstd::in_place,
                                  std::forward<Args>(args)... )
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
    constexpr explicit optional(nonstd::in_place_t /*unused*/,
                                std::initializer_list<Il> il,
                                Args && ... args)
    noexcept(std::is_nothrow_constructible_v< T
                                            , std::initializer_list<Il> &
                                            , Args && ...>)
        : _optional_val_base<T> ( nonstd::in_place,
                                  il,
                                  std::forward<Args>(args)... )
    { }

    /** Converting Perfect Forwarding Value Ctor -- §23.6.3.1.20
     *  --------------------------------------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T` with the expression `std::forward<U>(v)`.
     */
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<std::decay_t<U>, nonstd::in_place_t>
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                           &&  std::is_constructible_v<T, U&&>
                                           &&  std::is_convertible_v<U &&, T>)
                                        , int > = 0 >
    constexpr optional(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : _optional_val_base<T> ( nonstd::in_place, std::forward<U>(value) )
    { }

    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<std::decay_t<U>, nonstd::in_place_t>
                                           && !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                           &&  std::is_constructible_v<T, U&&>
                                           && !std::is_convertible_v<U &&, T>)
                                        , int > = 1 >
    constexpr explicit optional(U && value)
    noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : _optional_val_base<T> ( nonstd::in_place, std::forward<U>(value) )
    { }

    /** Converting Copy Ctor -- §23.6.3.1.24
     *  ------------------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `*rhs`.
     *
     *  NB. We use the user-facing observers here because `optional<T>` may not
     *  be the same type as `optional<U>`
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           &&  std::is_convertible_v<U const &, T>)
                                        , int > = 0 >
    constexpr optional(optional<U> const & rhs)
    noexcept(std::is_nothrow_constructible_v<T, U>)
        : _optional_val_base<T> ( rhs.has_value()
                                ? _optional_val_base<T> { *rhs    }
                                : _optional_val_base<T> { nullopt } )
    { }

    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           && !std::is_convertible_v<U const &, T>)
                                        , int > = 1 >
    constexpr explicit optional(optional<U> const & rhs)
    noexcept(std::is_nothrow_constructible_v<T, U>)
        : _optional_val_base<T> ( rhs.has_value()
                                ? _optional_val_base<T> { *rhs    }
                                : _optional_val_base<T> { nullopt } )
    { }

    /** Converting Move Ctor -- §23.6.3.1.28
     *  ------------------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with
     *  `std::move(*rhs)`.
     *  **This will not change `bool(rhs)`**.
     *
     *  NB. We use the user-facing observers here because `optional<T>` may not
     *  be the same type as `optional<U>`
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U&&>
                                           && !is_convertible_from_optional_v<T, U>
                                           &&  std::is_convertible_v<U &&, T>)
                                        , int > = 0 >
    constexpr optional(optional<U>&& rhs)
    noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : _optional_val_base<T> ( rhs.has_value()
                                ? _optional_val_base<T> { std::move(*rhs) }
                                : _optional_val_base<T> { nullopt         } )
    { }

    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U&&>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !std::is_convertible_v<U &&, T>)
                                        , int > = 1 >
    constexpr explicit optional(optional<U>&& rhs)
    noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : _optional_val_base<T> ( rhs.has_value()
                                ? _optional_val_base<T>{ std::move(*rhs) }
                                : _optional_val_base<T>{ nullopt         } )
    { }

    /** Empty Assignment -- §23.6.3.3.1
     *  -------------------------------
     *  If `*this` contains a value, destroy it; otherwise, no effect.
     */
    optional& operator= (nonstd::nullopt_t /*unused*/)
    noexcept(std::is_nothrow_destructible_v<T>) {
        if (this->_has_value()) { this->_remove_value(); }
        return *this;
    }

    /** Converting Perfect Forwarding Value Assignment -- §23.6.3.3.13
     *  --------------------------------------------------------------
     *  If `*this` contains a value, assign `std::forward<U>(value)` to the
     *  contained value; otherwise initializes the contained value as if direct-
     *  non-list-initializing an object of type `T` with `std::forward<U>(v)`.
     */
    template < typename U = T
             , typename std::enable_if_t< (   !std::is_same_v<nonstd::remove_cvref_t<U>, optional<T>>
                                           && !(std::is_scalar_v<T> && std::is_same_v<T, std::decay_t<U>>)
                                           &&  std::is_constructible_v<T, U>
                                           &&  std::is_assignable_v<T&, U>)
                                        , int > = 0 >
    optional& operator= (U&& value)
    noexcept(   std::is_nothrow_assignable_v<T, U&&>
             && std::is_nothrow_constructible_v<T, U&&>) {
        if (this->_has_value()) {
            this->_get_value() = std::forward<U>(value);
        } else {
            this->_construct_value(std::forward<U>(value));
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
     *
     *  NB. We use the user-facing observers here because `optional<T>` may not
     *  be the same type as `optional<U>`
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U const &>
                                           &&  std::is_assignable_v<T&, U const &>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !is_assignable_from_optional_v<T, U>)
                                        , int > = 0 >
    optional& operator= (optional<U> const & rhs)
    noexcept(   std::is_nothrow_assignable_v<T, U>
             && std::is_nothrow_constructible_v<T, U>
             && std::is_nothrow_destructible_v<T>) {
        if (this->_has_value() && rhs.has_value()) {
            this->_get_value() = *rhs;
        } else {
            if (rhs.has_value()) {
                this->_construct_value(*rhs);
            } else if (this->_has_value()) {
                this->_remove_value();
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
     *  NB. We use the user-facing observers here because `optional<T>` may not
     *  be the same type as `optional<U>`
     */
    template < typename U
             , typename std::enable_if_t< (   !std::is_same_v<T, U>
                                           &&  std::is_constructible_v<T, U>
                                           &&  std::is_assignable_v<T&, U>
                                           && !is_convertible_from_optional_v<T, U>
                                           && !is_assignable_from_optional_v<T, U>)
                                        , int > = 0 >
    optional& operator= (optional<U>&& rhs)
    noexcept(   std::is_nothrow_assignable_v<T, U>
             && std::is_nothrow_constructible_v<T, U>
             && std::is_nothrow_destructible_v<T>) {
        if (this->_has_value() && rhs.has_value()) {
            this->_get_value() = std::move(*rhs);
        } else {
            if (rhs.has_value()) {
                this->_construct_value(std::move(*rhs));
            } else if (this->_has_value()) {
                this->_remove_value();
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
        if (has_value()) { this->_remove_value(); }
        this->_construct_value(std::forward<Args>(args)...);
        return this->_get_value();
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
        if (this->_has_value()) { this->_remove_value(); }
        this->_construct_value(il, std::forward<Args>(args)...);
        return this->_get_value();
    }


    /** Swap -- §23.6.3.4.1
     *  -------------------
     *  Swap the values of `*this` and `rhs`, based on whether `*this`
     *  and/or `rhs` contain or do not contain values;
     *   - If `*this` contais a value and `rhs` contains a value
     *     call `swap(*(*this), *rhs)`.
     *   - If `*this` contains a value and `rhs` contains no value
     *     initialize the contained value of `rhs` as if direct-non-list-
     *     initializing an object of type `T` with the expression
     *     `std::move(*(*this))`, then destruct the value contained in `*this`.
     *     Postcondition: `*this` shall not contain a value, and `rhs` shall.
     *   - If `*this` contains no value and `rhs` contains a value
     *     initialize the contained value of `*this` as if direct-non-list-
     *     initializing an object of type `T` with the expression
     *     `std::move(*rhs)`, then destruct the value contained in `rhs`.
     *     Postcondition: `*this` shall contain a value, and `rhs` shall not.
     *   - If `*this` contains no value and `rhs` contains no value
     *     no effect.
     *
     *  Author's Note: The specification of this function requires that "Lvalues
     *  of type `T` shall be swappable and `is_move_constructible_v<T>` is
     *  `true`." At the time of writing, I'm not sure how to enforce those
     *  requirements, but I am confident lower-level compilation errors will
     *  prevent misuse of this function.
     */
    void swap(optional& rhs)
    noexcept(   std::is_nothrow_move_constructible_v<T>
             && nonstd::is_nothrow_swappable_v<T>) {
        if (this->_has_value() && rhs._has_value()) {
            std::swap(this->_get_value(), rhs._get_value());
        } else {
            if (rhs._has_value()) {
                this->_construct_value(std::move(rhs._get_value()));
                rhs._remove_value();
            } else if (this->_has_value()) {
                rhs._construct_value(std::move(this->_get_value()));
                this->_remove_value();
            }
        }
    }


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
    constexpr T value_or(U && value) const & {
        return this->_has_value()
             ? this->_get_value()
             : static_cast<T>(std::forward<U>(value));
    }
    /** Observer `value_or(value)` for rvalue references -- §23.6.3.5.19
     *  ----------------------------------------------------------------
     *  If `*this` is contains a value, return `std::move(*val)`; otherwise
     *  return `static_cast<T>(std::forward<U>(value))`.
     */
    template < typename U = T >
    constexpr T value_or(U && value) && {
        return this->_has_value()
             ? std::move(this->_get_value())
             : static_cast<T>(std::forward<U>(value));
    }

private:
    /** Helper function; Check the validity of indirecting through `*this`. */
    constexpr inline void _check_value() const {
        if (!this->_has_value()) {
            throw nonstd::exception::bad_optional_access{};
        }
        return;
    }
};


/** Base-Class for Reference-Wrapping Optional Types
 *  ============================================================================
 */
template <typename T>
class _optional_ref_base<T&> {
public:
    using value_type = T;
    using pointer_type = T*;
    using reference_type = T&;

private:
    // Reference types can't be directly stored and can't be reseated once
    // initialized. To get around this, we store a mutable pointer to the
    // referenced type.
    optional_storage<pointer_type> m_storage;

public:
    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     *
     *  NB. These are duplicated on `optional<T&>`.
     */
    constexpr _optional_ref_base() noexcept
        : m_storage ( )
    { }
    constexpr _optional_ref_base(nonstd::nullopt_t /*unused*/) noexcept
        : m_storage ( )
    { }

    /** Copy Ctor -- §23.6.3.1.3
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `*rhs`.
     *
     *  NB. We copy `m_storage` here, not `_get_value()`, because the later
     *  assumes storage is containing. That is a bad assumption.
     *
     *  NB. This is _not_ duplicated on `optional<T&>`.
     */
    constexpr _optional_ref_base(_optional_ref_base const & rhs) noexcept
        : m_storage ( rhs.m_storage )
    { }

    /** Move Ctor -- §23.6.3.1.7
     *  ------------------------
     *  Initialize `this` by copying `rhs`. If a value is initialized, it will
     *  be as if direct-non-list-initialization occurred with `std::move(*rhs)`.
     *  **This will not change `bool(rhs)`**.
     *
     *  NB. We copy `m_storage` here, not `_get_value()`, because the later
     *  assumes storage is containing. That is a bad assumption.
     *
     *  NB. This is _not_ duplicated on `optional<T&>`.
     */
    constexpr _optional_ref_base(_optional_ref_base && rhs) noexcept
        : m_storage ( std::move(rhs.m_storage) )
    { }

    /** Value Ctor -- §23.6.3.1.20
     *  --------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T*` with the expression `&value`.
     *
     *  NB. This is duplicated on `optional<T&>`.
     */
    constexpr _optional_ref_base(T& value) noexcept
        : m_storage ( &value )
    { }

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
     *  NB. This is _not_ duplicated on `optional<T&>`.
     */
    _optional_ref_base& operator= (_optional_ref_base const & rhs) noexcept {
        if (_has_value() && rhs._has_value()) {
            _get_pointer() = rhs._get_pointer();
        } else {
            if (rhs._has_value()) {
                _contstruct_value(rhs._get_pointer());
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
     *  NB. This is _not_ duplicated on `optional<T&>`.
     */
    _optional_ref_base& operator= (_optional_ref_base && rhs) noexcept {
        if (_has_value() && rhs._has_value()) {
            _get_pointer() = std::move(rhs._get_pointer());
        } else {
            if (rhs._has_value()) {
                _construct_value(std::move(rhs._get_pointer()));
            } else if (_has_value()) {
                _remove_value();
            }
        }
        return *this;
    }

protected:
    constexpr bool _has_value() const noexcept {
        return m_storage.is_containing;
    }

    constexpr pointer_type       &  _get_pointer()       &  {
        return m_storage.get_value();
    }
    constexpr pointer_type const &  _get_pointer() const &  {
        return m_storage.get_value();
    }
    constexpr pointer_type       && _get_pointer()       && {
        return std::move(m_storage.get_value());
    }
    constexpr pointer_type const && _get_pointer() const && {
        return std::move(m_storage.get_value());
    }

    constexpr T       & _get_reference()       {
        return *(m_storage.get_value());
    }
    constexpr T const & _get_reference() const {
        return *(m_storage.get_value());
    }

    constexpr void _assign_reference(T& value) noexcept {
        m_storage.get_value() = &value;
    }

    void _construct_value(pointer_type value) noexcept {
        m_storage.construct_value(value);
    }

    void _construct_value(T& value) noexcept {
        m_storage.construct_value(&value);
    }

    void _remove_value() noexcept {
        m_storage.remove_value();
    }
};


/** `optional<T&>`
 *  ============================================================================
 */
template <typename T>
class optional<T&>
    : _optional_ref_base<T&>
{
public:
    using value_type = T;
    using pointer_type = T*;
    using reference_type = T&;

    /** Empty Ctors -- §23.6.3.1.1
     *  --------------------------
     *  Initialize a non-containing optional.
     */
    constexpr optional() noexcept = default;
    constexpr optional(nonstd::nullopt_t /*unused*/) noexcept
        : _optional_ref_base<T&> ( nonstd::nullopt )
    { }

    /** Value Ctor -- §23.6.3.1.20
     *  --------------------------
     *  Initialize the contained value as if direct-non-list-initializing an
     *  object of type `T&` with the expression `value`.
     */
    constexpr optional(T& value) noexcept
        : _optional_ref_base<T&> ( value )
    { }

    /** Empty Assignment -- §23.6.3.3.1
     *  -------------------------------
     *  If `*this` contains a value, destroy it; otherwise, no effect.
     */
    optional& operator= (nonstd::nullopt_t /*unused*/) noexcept {
        if (this->_has_value()) { this->_remove_value(); }
        return *this;
    }

    /** Value Assignment -- §23.6.3.3.13
     *  --------------------------------
     *  If `*this` contains a value, assign `value` to the contained value;
     *  otherwise initializes the contained value as if direct-non-list-
     *  initializing an object of type `T&` with `value`.
     */
    optional& operator= (T& value) noexcept {
        if (this->_has_value()) {
            this->_assign_reference(value);
        } else {
            this->_construct_value(value);
        }
        return *this;
    }


    /** Swap -- §23.6.3.4.1
     *  -------------------
     *  Swap the values of `*this` and `rhs`, based on whether `*this`
     *  and/or `rhs` contain or do not contain values;
     *   - If `*this` contais a value and `rhs` contains a value
     *     call `swap(*(*this), *rhs)`.
     *   - If `*this` contains a value and `rhs` contains no value
     *     initialize the contained value of `rhs` as if direct-non-list-
     *     initializing an object of type `T` with the expression
     *     `std::move(*(*this))`, then destruct the value contained in `*this`.
     *     Postcondition: `*this` shall not contain a value, and `rhs` shall.
     *   - If `*this` contains no value and `rhs` contains a value
     *     initialize the contained value of `*this` as if direct-non-list-
     *     initializing an object of type `T` with the expression
     *     `std::move(*rhs)`, then destruct the value contained in `rhs`.
     *     Postcondition: `*this` shall contain a value, and `rhs` shall not.
     *   - If `*this` contains no value and `rhs` contains no value
     *     no effect.
     *
     *  Author's Note: The specification of this function requires that "Lvalues
     *  of type `T` shall be swappable and `is_move_constructible_v<T>` is
     *  `true`." At the time of writing, I'm not sure how to enforce those
     *  requirements, but I am confident lower-level compilation errors will
     *  prevent misuse of this function.
     */
    void swap(optional& rhs)
    noexcept(   std::is_nothrow_move_constructible_v<T>
             && nonstd::is_nothrow_swappable_v<T>) {
        if (this->_has_value() && rhs._has_value()) {
            std::swap(this->_get_reference(), rhs._get_reference());
        } else {
            if (rhs._has_value()) {
                this->_construct_value(std::move(rhs._get_pointer()));
                rhs._remove_value();
            } else if (this->_has_value()) {
                rhs._construct_value(std::move(this->_get_pointer()));
                this->_remove_value();
            }
        }
    }


    /** Observer `operator ->` -- §23.6.3.5.1
     *  -------------------------------------
     *  Return `val`.
     */
    constexpr       T * operator -> ()       {
        return this->_get_pointer();
    }
    constexpr const T * operator -> () const {
        return this->_get_pointer();
    }

    /** Observer `operator *` -- §23.6.3.5.5
     *  ------------------------------------
     *  Return `*val`.
     */
    constexpr       T& operator * ()       {
        return this->_get_reference();
    }
    constexpr const T& operator * () const {
        return this->_get_reference();
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
        _check_value();
        return this->_get_reference();
    }
    constexpr const T& value() const {
        _check_value();
        return this->_get_reference();
    }

private:
    /** Helper function; Check the validity of indirecting through `*this`. */
    constexpr inline void _check_value() const {
        if (!this->_has_value()) {
            throw nonstd::exception::bad_optional_access();
        }
        return;
    }
};



/** Helper Creation Functions
 *  ============================================================================
 */

/** Create an `optional` with no value. */
template <typename T>
constexpr optional<T> none() noexcept { return optional<T>(); }

/** Create an `optional` containing a value, deducing the type if possible. */
template <typename T>
constexpr optional<std::decay_t<T>> just(T&& value)
noexcept(std::is_nothrow_constructible_v<T,T&&>) {
    return optional<std::decay_t<T>> { std::forward<T>(value) };
}
template <typename T, typename ... Args>
constexpr optional<T> just(nonstd::in_place_t /*unused*/, Args && ... args)
noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
    return optional<T> { nonstd::in_place, std::forward<Args>(args)... };
}
template <typename T, typename Il, typename ... Args>
constexpr optional<T> just(nonstd::in_place_t /*unused*/,
                           std::initializer_list<Il> il,
                           Args && ... args)
noexcept(std::is_nothrow_constructible_v<T,
                                         std::initializer_list<Il>,
                                         Args && ...>) {
    return optional<T> { nonstd::in_place, il, std::forward<Args>(args)... };
}

/** Create an `optional` wrapping a reference. */
template <typename T>
constexpr optional<T&> just_ref(T & value) {
    return optional<T&> { value };
}

/** Create an `optional` wrapping a reference to a const. */
template <typename T>
constexpr optional<T const &> just_cref(T const & value) {
    return optional<T const &> { value };
}

/** Make Optional -- §23.6.9.3
 *  ----------------------------
 *  Return `optional<decay_t<T>> { std::morward<T>(value) }`.
 */
template <typename T>
constexpr optional<std::decay_t<T>> make_optional(T&& value)
noexcept(std::is_nothrow_constructible_v<T,T&&>) {
    return optional<std::decay_t<T>> { std::forward<T>(value) };
}
/** Make Optional -- §23.6.9.4
 *  ----------------------------
 * Return `optional<T> { in_place, std::morward<Args>(args)... }`.
 */
template <typename T, typename ... Args>
constexpr optional<T> make_optional(nonstd::in_place_t /*unused*/,
                                    Args && ... args)
noexcept(std::is_nothrow_constructible_v<T, Args && ...>) {
    return optional<T> { nonstd::in_place, std::forward<Args>(args)... };
}
/** Make Optional -- §23.6.9.5
 *  ----------------------------
 * Return `optional<T> { in_place, il, std::morward<Args>(args)... }`.
 */
template <typename T, typename Il, typename ... Args>
constexpr optional<T> make_optional(nonstd::in_place_t /*unused*/,
                                    std::initializer_list<Il> il,
                                    Args && ... args)
noexcept(std::is_nothrow_constructible_v<T,
                                         std::initializer_list<Il>,
                                         Args && ...>) {
    return optional<T> { nonstd::in_place, il, std::forward<Args>(args)... };
}


/** Swap -- §23.6.9.1
 *  -----------------
 *  Swap the values of `lhs` and `rhs`.
 *
 *  See §23.6.3.4.1 for details.
 */
template < typename T
         , typename std::enable_if_t< (   std::is_move_constructible_v<T>
                                       && nonstd::is_swappable_v<T>)
                                    , int > = 0 >
inline void swap(optional<T>& lhs, optional<T>& rhs)
noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}
template < typename T
         , typename std::enable_if_t< !(   std::is_move_constructible_v<T>
                                        && nonstd::is_swappable_v<T>)
                                    , int > = 1 >
inline void swap(optional<T>& lhs, optional<T>& rhs)
noexcept(noexcept(lhs.swap(rhs))) = delete;



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
