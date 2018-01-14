/** Selective Special Member Disablers
 *  ==================================
 *  Beware. There be Template Magic Here.
 *
 *  These helper structs allow us selectively disable a parent classes' special
 *  member functions (Copy Constructor, Copy Assignment, Move Constructor, and
 *  Move Assignment) based on template type traits or constexpr boolean checks.
 *  The idiom to use them is as follows;
 *
 *  Let's say we have a wrapper class `<template T> class Foo`, and we want to
 *  conditionally disable Foo's special members based on the output of
 *  `myConstexprTest<T>()`.
 *
 *  1. Define a new template class `template <typename T> _Foo_Base`.
 *  2. Move all of `Foo`'s special members into `_Foo_Base` --- reworking the
 *     signatures and member storage, of course.
 *  3. Modify the definition of `Foo` to something like the below;
 *
 *         template <typename T>
 *         class Foo
 *               : _Foo_Base<T>
 *               , nonstd::EnableCopyCtorIf<myConstexprTest<T>(), Foo<T>>
 *               , nonstd::EnableMoveCtorIf<myConstexprTest<T>(), Foo<T>>
 *               , nonstd::EnableCopyAssignIf<myConstexprTest<T>(), Foo<T>>
 *               , nonstd::EnableMoveAssignIf<myConstexprTest<T>(), Foo<T>> {
 *             . . .
 *         };
 *
 *  This works because the special members of `Foo` will be implicitly defined
 *  once they've been moved to `_Foo_Base`, and each of them will implicitly
 *  call the relative member of `_Foo_Base`, `nonstd::EnableCopyCtorIf<...>`,
 *  `nonstd::EnableMoveCtorIf<...>`, etc.. The special members functions of each
 *  of the `Enable[Member]If` types will do, at most, nothing. In the cases
 *  where they've been deleted, though, the compiler will see that `Foo`'s
 *  implicit member will always fail (by calling an explicitly deleted function)
 *  and simply not instantiate that member.
 *
 *  NB. While it may be possible to skip the `_Foo_Base` extension and
 *  explicitly call the correct `Enable[Member]If::` member function, that is...
 *  _highly_ brittle, and may in fact never be correct for assignment operators.
 *
 *  NB. If `Foo` is sufficiently simple, you may be able to move all of `Foo`s
 *  functionality into `_Foo_Base`, and use `using` declarations to pull in
 *  selected functionality, including constructors. There is a significat risk
 *  of interfering with the compiler's ability to disambiguate coplex calls,
 *  though (conssider `template <typename U> _FooBase(Foo<U> rhs)`, specifically
 *  how it doesn't really work.)
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
