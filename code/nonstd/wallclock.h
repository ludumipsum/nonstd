/** A Clock on the Wall
 *  ===================
 *  Uses std::chrono for all of its timing durations, and std::thread for sleep.
 */
#pragma once

#include <nonstd/nonstd.h>
#include <chrono>
#include <thread>


namespace nonstd {

struct wallclock {
    /** Query the system high-frequency monotonic clock. */
    inline static std::chrono::nanoseconds now() noexcept {
        return std::chrono::steady_clock::now().time_since_epoch();
    }
    inline static std::chrono::microseconds now_us() noexcept {
        return std::chrono::round<std::chrono::microseconds>(now());
    }
    inline static std::chrono::milliseconds now_ms() noexcept {
        return std::chrono::round<std::chrono::milliseconds>(now());
    }
    inline static std::chrono::seconds now_sec() noexcept {
        return std::chrono::round<std::chrono::seconds>(now());
    }

    /** Sleep the calling thread for the provided number of milliseconds */
    template <typename Rep, typename Period = std::ratio<1>>
    static void delay(std::chrono::duration<Rep, Period> const & duration) {
        std::this_thread::sleep_for(duration);
    }
};


namespace literals::chrono_literals {

using namespace std::literals::chrono_literals;

constexpr auto operator "" _hz (udl_int_t hz) {
    return 1.0s / hz;
}
constexpr auto operator "" _hz (udl_float_t hz) {
    return 1.0s / hz;
}

// TODO: Strictly speaking, these UDLs aren't returning hertz -- they're not
// returning a frequency (1/200 seconds) -- they're returning a duration (0.005
// seconds). It would be most correct (and maybe kinda fun) to build a
// nonstd::chrono::frequency type template, modeled after and designed to
// interact with the std::chrono::duration template type.

} /* namespace literals::chrono_literals */


} /* namespace nonstd */
