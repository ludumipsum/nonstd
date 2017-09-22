/** Special Member Enablers/Disablers
 *  =================================
 *  Beware. There be Template Magic Here.
 *  These helpers allow us selectively disable a parent classes' special member
 *  functions (Copy Constructor, Copy Assignment, Move Constructor, and Move
 *  Assignment) based on template type traits or constexpr boolean checks. The
 *  idiom to use is as follows;
 *
 *  - Let's say we have a wrapper class `<template T> class Foo`, and we want
 *    the assignment operators to not be defined unless `T` is
 *    trivially assignable.
 *  - Rename `Foo` to `_Foo_Base`
 *  - Create a new `Foo` using the below definition pattern;
 *        template <typename T>
 *        class Foo
 *              : _Foo_Base<T>
 *              , nonstd::_EnableCopyAssign<IS_TRIVIALLY_ASSIGNABLE(T), T>
 *              , nonstd::_EnableMoveAssign<IS_TRIVIALLY_ASSIGNABLE(T), T> {
 *            using _Foo_Base::_Foo_Base;
 *        };
 *    NB:
 *    - We allow `Foo`s special members to all be implicitly defined.
 *      This causes any invocation of `Foo& operator=` to call into both
 *      `_Foo_Base& operator=` and `_EnableCopyAssign& operator=`. The later
 *      does nothing, but if `IS_TRIVIALLY_ASSIGNABLE(T)` is false,
 *      `_EnableCopyAssign& operator=` will be explicitly deleted, causing a
 *      compilation error if `Foo& operator=` is ever misused.
 *    - We use `using _Foo_Base::_Foo_Base` to inherit all of `_Foo_Base`'s
 *      valid constructors, s.t. all explicit constructors defined by
 *      `_Foo_Base` will be available to `Foo`.
 *      TODO: Find out if `using` can be used to (cleanly) pull all of
 *            `_Foo_Base`s member functions in.
 */

#pragma once


namespace nonstd {

template <bool EnableCopyCtor, typename UniqueTag>
struct _EnableCopyCtor {
    constexpr _EnableCopyCtor() noexcept                              = default;
    constexpr _EnableCopyCtor(const _EnableCopyCtor&) noexcept        = default;
    _EnableCopyCtor& operator=(const _EnableCopyCtor&) noexcept       = default;
    constexpr _EnableCopyCtor(_EnableCopyCtor &&) noexcept            = default;
    _EnableCopyCtor& operator=(_EnableCopyCtor &&) noexcept           = default;
};
template <typename UniqueTag>
struct _EnableCopyCtor<false, UniqueTag> {
    constexpr _EnableCopyCtor() noexcept                              = default;
    constexpr _EnableCopyCtor(const _EnableCopyCtor&) noexcept        = delete;
    _EnableCopyCtor& operator=(const _EnableCopyCtor&) noexcept       = default;
    constexpr _EnableCopyCtor(_EnableCopyCtor &&) noexcept            = default;
    _EnableCopyCtor& operator=(_EnableCopyCtor &&) noexcept           = default;
};

template <bool EnableCopyAssign, typename UniqueTag>
struct _EnableCopyAssign {
    constexpr _EnableCopyAssign() noexcept                            = default;
    constexpr _EnableCopyAssign(const _EnableCopyAssign&) noexcept    = default;
    _EnableCopyAssign& operator=(const _EnableCopyAssign&) noexcept   = default;
    constexpr _EnableCopyAssign(_EnableCopyAssign &&) noexcept        = default;
    _EnableCopyAssign& operator=(_EnableCopyAssign &&) noexcept       = default;
};
template <typename UniqueTag>
struct _EnableCopyAssign<false, UniqueTag> {
    constexpr _EnableCopyAssign() noexcept                            = default;
    constexpr _EnableCopyAssign(const _EnableCopyAssign&) noexcept    = default;
    _EnableCopyAssign& operator=(const _EnableCopyAssign&) noexcept   = delete;
    constexpr _EnableCopyAssign(_EnableCopyAssign &&) noexcept        = default;
    _EnableCopyAssign& operator=(_EnableCopyAssign &&) noexcept       = default;
};

template <bool EnableMoveCtor, typename UniqueTag>
struct _EnableMoveCtor {
    constexpr _EnableMoveCtor() noexcept                              = default;
    constexpr _EnableMoveCtor(const _EnableMoveCtor&) noexcept        = default;
    _EnableMoveCtor& operator=(const _EnableMoveCtor&) noexcept       = default;
    constexpr _EnableMoveCtor(_EnableMoveCtor &&) noexcept            = default;
    _EnableMoveCtor& operator=(_EnableMoveCtor &&) noexcept           = default;
};
template <typename UniqueTag>
struct _EnableMoveCtor<false, UniqueTag> {
    constexpr _EnableMoveCtor() noexcept                              = default;
    constexpr _EnableMoveCtor(const _EnableMoveCtor&) noexcept        = default;
    _EnableMoveCtor& operator=(const _EnableMoveCtor&) noexcept       = default;
    constexpr _EnableMoveCtor(_EnableMoveCtor &&) noexcept            = delete;
    _EnableMoveCtor& operator=(_EnableMoveCtor &&) noexcept           = default;
};

template <bool EnableMoveAssign, typename UniqueTag>
struct _EnableMoveAssign {
    constexpr _EnableMoveAssign() noexcept                            = default;
    constexpr _EnableMoveAssign(const _EnableMoveAssign&) noexcept    = default;
    _EnableMoveAssign& operator=(const _EnableMoveAssign&) noexcept   = default;
    constexpr _EnableMoveAssign(_EnableMoveAssign &&) noexcept        = default;
    _EnableMoveAssign& operator=(_EnableMoveAssign &&) noexcept       = default;
};
template <typename UniqueTag>
struct _EnableMoveAssign<false, UniqueTag> {
    constexpr _EnableMoveAssign() noexcept                            = default;
    constexpr _EnableMoveAssign(const _EnableMoveAssign&) noexcept    = default;
    _EnableMoveAssign& operator=(const _EnableMoveAssign&) noexcept   = default;
    constexpr _EnableMoveAssign(_EnableMoveAssign &&) noexcept        = default;
    _EnableMoveAssign& operator=(_EnableMoveAssign &&) noexcept       = delete;
};

} /* namespace nonstd */
