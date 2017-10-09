/** Scope Guard
 *  ===========
 *  Heavily inspired by Andrei Alexandrescu's Systematic Error Handling in C++,
 *  channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C
 *  Additional details inspired by Facebook's Folly implementation
 *
 *  Provides an automated cleanup mechanism to help preserve sane control flows.
 */

#pragma once

#include <nonstd/nonstd.h>


namespace nonstd {

template <typename Fn_T>
class scope_guard {
private:
    Fn_T fn;
    bool active;

public:
    scope_guard(Fn_T const & fn)
    noexcept(IS_NOTHROW_COPY_CONSTRUCTIBLE(Fn_T))
        : fn     ( fn   )
        , active ( true ) { }
    scope_guard(Fn_T && fn)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(Fn_T))
        : fn     ( std::move_if_noexcept(fn) )
        , active ( true ) { }

    scope_guard(scope_guard && other)
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(Fn_T))
        : fn     ( std::move_if_noexcept(other.fn) )
        , active ( other.active )
    {
        other.dismiss();
    }
    scope_guard & operator= (scope_guard && other )
    noexcept(IS_NOTHROW_MOVE_CONSTRUCTIBLE(Fn_T)) {
        fn     = std::move_if_noexcept(other.fn);
        active = other.active;
        other.dismiss();
    }
    scope_guard() = delete;
    scope_guard(scope_guard const &) = delete;
    scope_guard & operator= (scope_guard const &) = delete;
    void * operator new (size_t) = delete;

    ~scope_guard() noexcept {
        if (active) {
            try {
                fn();
            } catch (...) {
                LOG(critical) << "scope_guard destructions cannot correctly "
                                 "throw. Calling std::terminate.";
                std::terminate();
            }
        }
    }

    void dismiss() noexcept { active = false; }
};

template <typename Fn_T>
auto make_guard(Fn_T && fn)
noexcept(IS_NOTHROW_CONSTRUCTIBLE(DECAY_TYPE(Fn_T), Fn_T)) {
    return scope_guard<DECAY_TYPE(Fn_T)>(std::forward<Fn_T>(fn));
}

} /* namespace nonstd */
