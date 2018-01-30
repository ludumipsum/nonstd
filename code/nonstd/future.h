/** Nonstandard Future Extensions
 *  =============================
 *  The core motivation for this file is a bug in MSVC;
 *  https://developercommunity.visualstudio.com/content/problem/60897/c-shared-state-futuresstate-default-constructs-the.html
 *  tl;dr -- Anything that's passed into a `std::future` in MSVC needs to have
 *  a default constructor.
 *  Kinda long; read anyway -- Due to some underlying assumptions made prior to
 *  C++11, MSVC's lifetime guarantees are not correct for many asynchronous
 *  data structures. The MSVC team does have a solution to this issue, but
 *  implementing it will result in an ABI incompatibility in compiled binaries.
 *  So they're not going to implement that change until a new major release of
 *  MSVC is organized.
 *
 *  All the code in this file is a workaround for the above.
 */
#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/optional.h>
#include <nonstd/type_traits_ext.h>
#include <chrono>
#include <future>


namespace nonstd {

using std::future_status;


/** Future -- Wrapped in an Optional
 *  ================================
 */
// TODO: The reference and void overloads of this class have not been tested.
// They probably work, but if they don't... blame me.
template <typename T>
class future : public std::future<optional<T>> {
public:
    future() noexcept = default;
    future(future && rhs) noexcept = default;
    future(std::future<optional<T>> && rhs)
        : std::future<optional<T>> ( std::move(rhs) )
    { }
    future& operator= (future && rhs) noexcept = default;
    future& operator= (std::future<optional<T>> && rhs) {
        std::future<optional<T>>::operator=(std::move(rhs));
    }

    ~future() = default;

    future(future const &) = delete;
    future& operator= (future const &) = delete;

    std::shared_future<optional<T>> share() noexcept {
        return std::move(this->share());
    }

    T get() {
        return std::move(std::future<optional<T>>::get().value());
    }

    bool valid() const noexcept {
        return std::future<optional<T>>::valid();
    }

    void wait() const {
        std::future<optional<T>>::wait();
    }

    template <typename Rep, typename Period>
    future_status wait_for(std::chrono::duration<Rep, Period> const & rel_time)
    const {
        return std::future<optional<T>>::wait_for(rel_time);
    }

    // [sic] `Duration_`; osx typedefs `Duration` for drivers.
    template <typename Clock, typename Duration_>
    future_status wait_until(
        std::chrono::time_point<Clock, Duration_>& abs_time )
    const {
        return std::future<optional<T>>::wait_until(abs_time);
    }
};


template <typename T>
class future<T&> : public std::future<T&> {
public:
    future() noexcept = default;
    future(future && rhs) noexcept = default;
    future& operator= (future && rhs) noexcept = default;

    future(future const &) = delete;
    future& operator= (future const &) = delete;

    ~future() = default;
};

template <>
class future<void> : public std::future<void> {
    future() noexcept = default;
    future(future && rhs) noexcept = default;
    future& operator= (future && rhs) noexcept = default;

    future(future const &) = delete;
    future& operator= (future const &) = delete;

    ~future() = default;
};


/** Promise -- Wrapped in an Optional
 *  =================================
 */
// TODO: The reference and void overloads of this class have not been tested.
// They probably work, but if they don't... blame me.
template <typename T>
class promise : public std::promise<optional<T>> {
public:
    promise() = default;
    promise(promise const &) = delete;
    promise(promise &&) = default;
    promise& operator= (promise const &) = delete;
    promise& operator= (promise &&) = delete;
    template <typename Allocator>
    promise(std::allocator_arg_t allocator_arg, Allocator const & allocator)
        : promise (allocator_arg, allocator)
    { }
    ~promise() = default;

    // assignment
    void swap(promise& other) noexcept {
        auto tmp = std::move(other);
        other = std::move(*this);
        *this = std::move(tmp);
    }
};

template <typename T>
class promise<T&> : public std::promise<T&> {
    promise() = default;
    promise(promise const &) = delete;
    promise(promise &&) = default;
    promise& operator= (promise const &) = delete;
    promise& operator= (promise &&) = delete;
};

template <>
class promise<void> : public std::promise<void> {
    promise() = default;
    promise(promise const &) = delete;
    promise(promise &&) = default;
    promise& operator= (promise const &) = delete;
    promise& operator= (promise &&) = delete;
};

} /* namespace nonstd */
