/** A Clock on the Wall
 *  ===================
 *  Uses nonstd::chrono for all of its timing durations, and std::thread for sleep.
 */
#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/chrono.h>
#include <thread>


namespace nonstd {

struct wallclock {
    /** Query the system high-frequency monotonic clock. */
    inline static nonstd::chrono::nanoseconds now() noexcept {
        return nonstd::chrono::steady_clock::now().time_since_epoch();
    }
    inline static nonstd::chrono::microseconds now_us() noexcept {
        return nonstd::chrono::round<nonstd::chrono::microseconds>(now());
    }
    inline static nonstd::chrono::milliseconds now_ms() noexcept {
        return nonstd::chrono::round<nonstd::chrono::milliseconds>(now());
    }
    inline static nonstd::chrono::seconds now_sec() noexcept {
        return nonstd::chrono::round<nonstd::chrono::seconds>(now());
    }

    /** Sleep the calling thread for the provided number of milliseconds */
    template <typename Rep, typename Period = std::ratio<1>>
    static void delay(nonstd::chrono::duration<Rep, Period> const & duration) {
        std::this_thread::sleep_for(duration);
    }
};

} /* namespace nonstd */
