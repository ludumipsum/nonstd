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
#include <nonstd/type_traits_ext.h>
#include <nonstd/optional_storage.h>
#include <chrono>
#include <future>


namespace nonstd {

using std::future_status;


template <typename T>
class future : std::future<optional_storage<T>> {
public:
    future() noexcept = default;
    future(future && rhs) noexcept = default;
    future(std::future<optional_storage<T>> && rhs)
        : std::future<optional_storage<T>> ( std::move(rhs) )
    { }
    future& operator= (future && rhs) noexcept = default;
    future& operator= (std::future<optional_storage<T>> && rhs) {
        std::future<optional_storage<T>>::operator=(std::move(rhs));
    }

    ~future() = default;

    future(future const &) = delete;
    future& operator= (future const &) = delete;

    std::shared_future<optional_storage<T>> share() noexcept {
        return std::move(this->share());
    }

    T get() {
        return std::move(std::future<optional_storage<T>>::get().get_value());
    }

    bool valid() const noexcept {
        return std::future<optional_storage<T>>::valid();
    }

    void wait() const {
        std::future<optional_storage<T>>::wait();
    }

    template <typename Rep, typename Period>
    future_status wait_for(std::chrono::duration<Rep, Period> const & rel_time)
    const {
        return std::future<optional_storage<T>>::wait_for(rel_time);
    }

    // [sic] `Duration_`; osx typedefs `Duration` for drivers.
    template <typename Clock, typename Duration_>
    future_status wait_until(
        std::chrono::time_point<Clock, Duration_>& abs_time )
    const {
        return std::future<optional_storage<T>>::wait_until(abs_time);
    }
};


template <typename T>
class future<T&> : std::future<T&> {
public:
    future() noexcept = default;
    future(future && rhs) noexcept = default;
    future& operator= (future && rhs) noexcept = default;

    future(future const &) = delete;
    future& operator= (future const &) = delete;

    ~future() = default;
};

template <>
class future<void> : std::future<void> {
    future() noexcept = default;
    future(future && rhs) noexcept = default;
    future& operator= (future && rhs) noexcept = default;

    future(future const &) = delete;
    future& operator= (future const &) = delete;

    ~future() = default;
};






// template <typename T>
// class promise : std::promise<optional_storage<T>> {
// public:
//     promise() = default;

//     // template <typename Allocator>
//     // promise(std::allocator_arg_t, Allocator const & a);

//     promise(promise && rhs) noexcept = default;
//     promise& operator= (promise && rhs) noexcept = default;
//     ~promise() = default;

//     promise(promise const & rhs) = delete;
//     promise& operator= (promise const & rhs) = delete;

//     // void swap(promise& other) noexcept;

//     future<T> get_future() {
//         return
//     }

//     void set_value();
//     void set_exception();

//     void set_value_at_thread_exit(see below);
//     void set_exception_at_thread_exit(exception_ptr p);
// }



} /* namespace nonstd */
