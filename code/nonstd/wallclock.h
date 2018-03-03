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

} /* namespace nonstd */
