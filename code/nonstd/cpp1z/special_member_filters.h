/** Selective Special Member Disablers
 *  ==================================
 *  Beware. There be Template Magic Here.
 *
 *  These helper structs allow us selectively disable a parent classes' special
 *  member functions (Copy Constructor, Copy Assignment, Move Constructor, and
 *  Move Assignment) based on template type traits or constexpr boolean checks.
 *  The idiom to use them is as follows;
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

template <bool ShouldEnableCopyCtor, typename UniqueTag>
struct EnableCopyCtorIf {
    constexpr EnableCopyCtorIf() noexcept                                       = default;
    constexpr EnableCopyCtorIf(const EnableCopyCtorIf&) noexcept                = default;
    constexpr EnableCopyCtorIf& operator=(const EnableCopyCtorIf&) noexcept     = default;
    constexpr EnableCopyCtorIf(EnableCopyCtorIf &&) noexcept                    = default;
    constexpr EnableCopyCtorIf& operator=(EnableCopyCtorIf &&) noexcept         = default;
};
template <typename UniqueTag>
struct EnableCopyCtorIf<false, UniqueTag> {
    constexpr EnableCopyCtorIf() noexcept                                       = default;
    constexpr EnableCopyCtorIf(const EnableCopyCtorIf&) noexcept                = delete;
    constexpr EnableCopyCtorIf& operator=(const EnableCopyCtorIf&) noexcept     = default;
    constexpr EnableCopyCtorIf(EnableCopyCtorIf &&) noexcept                    = default;
    constexpr EnableCopyCtorIf& operator=(EnableCopyCtorIf &&) noexcept         = default;
};

template <bool ShouldEnableCopyAssign, typename UniqueTag>
struct EnableCopyAssignIf {
    constexpr EnableCopyAssignIf() noexcept                                     = default;
    constexpr EnableCopyAssignIf(const EnableCopyAssignIf&) noexcept            = default;
    constexpr EnableCopyAssignIf& operator=(const EnableCopyAssignIf&) noexcept = default;
    constexpr EnableCopyAssignIf(EnableCopyAssignIf &&) noexcept                = default;
    constexpr EnableCopyAssignIf& operator=(EnableCopyAssignIf &&) noexcept     = default;
};
template <typename UniqueTag>
struct EnableCopyAssignIf<false, UniqueTag> {
    constexpr EnableCopyAssignIf() noexcept                                     = default;
    constexpr EnableCopyAssignIf(const EnableCopyAssignIf&) noexcept            = default;
    constexpr EnableCopyAssignIf& operator=(const EnableCopyAssignIf&) noexcept = delete;
    constexpr EnableCopyAssignIf(EnableCopyAssignIf &&) noexcept                = default;
    constexpr EnableCopyAssignIf& operator=(EnableCopyAssignIf &&) noexcept     = default;
};

template <bool ShouldEnableMoveCtor, typename UniqueTag>
struct EnableMoveCtorIf {
    constexpr EnableMoveCtorIf() noexcept                                       = default;
    constexpr EnableMoveCtorIf(const EnableMoveCtorIf&) noexcept                = default;
    constexpr EnableMoveCtorIf& operator=(const EnableMoveCtorIf&) noexcept     = default;
    constexpr EnableMoveCtorIf(EnableMoveCtorIf &&) noexcept                    = default;
    constexpr EnableMoveCtorIf& operator=(EnableMoveCtorIf &&) noexcept         = default;
};
template <typename UniqueTag>
struct EnableMoveCtorIf<false, UniqueTag> {
    constexpr EnableMoveCtorIf() noexcept                                       = default;
    constexpr EnableMoveCtorIf(const EnableMoveCtorIf&) noexcept                = default;
    constexpr EnableMoveCtorIf& operator=(const EnableMoveCtorIf&) noexcept     = default;
    constexpr EnableMoveCtorIf(EnableMoveCtorIf &&) noexcept                    = delete;
    constexpr EnableMoveCtorIf& operator=(EnableMoveCtorIf &&) noexcept         = default;
};

template <bool ShouldEnableMoveAssign, typename UniqueTag>
struct EnableMoveAssignIf {
    constexpr EnableMoveAssignIf() noexcept                                     = default;
    constexpr EnableMoveAssignIf(const EnableMoveAssignIf&) noexcept            = default;
    constexpr EnableMoveAssignIf& operator=(const EnableMoveAssignIf&) noexcept = default;
    constexpr EnableMoveAssignIf(EnableMoveAssignIf &&) noexcept                = default;
    constexpr EnableMoveAssignIf& operator=(EnableMoveAssignIf &&) noexcept     = default;
};
template <typename UniqueTag>
struct EnableMoveAssignIf<false, UniqueTag> {
    constexpr EnableMoveAssignIf() noexcept                                     = default;
    constexpr EnableMoveAssignIf(const EnableMoveAssignIf&) noexcept            = default;
    constexpr EnableMoveAssignIf& operator=(const EnableMoveAssignIf&) noexcept = default;
    constexpr EnableMoveAssignIf(EnableMoveAssignIf &&) noexcept                = default;
    constexpr EnableMoveAssignIf& operator=(EnableMoveAssignIf &&) noexcept     = delete;
};

} /* namespace nonstd */
