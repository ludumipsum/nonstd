/* DEBUGGING & PROFILING TOOLS
   ===========================

   This file holds all the utility functions and types we use to debug... stuff.
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

/* SCOPE TIMER
   -----------
   Tiny utility for timing segments of code. Given a now function which reports
   nanoseconds, this defaults to microseconds.
*/
class ScopeTimer {
public:
    u64 start, unit_divisor;
    u64 &target;
    u64 (*now)(void);

    ScopeTimer(u64& target, u64 (*now)(void), u64 unit_divisor = NS_PER_US)
              : start        ( now()        )
              , unit_divisor ( unit_divisor )
              , target       ( target       )
              , now          ( now          ) { }

  ~ScopeTimer() { target = (now() - start) / unit_divisor; }
  ScopeTimer(ScopeTimer const&) = delete;
  ScopeTimer& operator=(ScopeTimer const&) = delete;
  ScopeTimer(ScopeTimer&&) = delete;
  ScopeTimer& operator=(ScopeTimer&&) = delete;
};

// Easymode macro for the above. Designed to be used when opening a scope. Ex:
//      /* Handle incoming inputs */
//      { TIME_SCOPE_US(step_stat.input_poll_usec, state.functions.now);
//          handleInput(state);
//      }
#define TIME_SCOPE(target, now_function) ScopeTimer st(target, now_function)
#define TIME_SCOPE_NS(target, now_function) ScopeTimer st(target, now_function, 1)
#define TIME_SCOPE_US(target, now_function) ScopeTimer st(target, now_function, NS_PER_US)
#define TIME_SCOPE_MS(target, now_function) ScopeTimer st(target, now_function, NS_PER_MS)
#define TIME_SCOPE_SEC(target, now_function) ScopeTimer st(target, now_function, NS_PER_SEC)
