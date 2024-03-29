/** No-op Instructions
 *  ==================
 *  Being idle can be important, and occasionally you need to convince the
 *  compiler or the typesystem of that fact. These utility functions are for
 *  doing just that.
 */

#pragma once

#include <nonstd/nonstd.h>

#if defined(NONSTD_OS_WINDOWS)
#include <intrin.h>
#pragma intrinsic(__nop)
#endif

namespace nonstd {

/** Simple Elidable NOP
 *  -------------------
 *  This function literally does nothing. Your compiler should optimize it out.
 */
constexpr inline void nop() noexcept {
    return;
}

/** Inline ASM NOP
 *  --------------
 *  For any platform with an assembly 'nop' instruction, this function generates
 *  one of those within the call. Generally your compiler will be reluctant to
 *  discard crap that happens in an inline asm block, so only use this if you
 *  really want to make sure there's a function call at the site of your nop.
 */
inline void asm_nop() noexcept {
    #if defined(NONSTD_COMPILER_MSVC)
        __nop();  // This intrinsic is msft's way of getting at the asm instr.
    #else
        asm("nop");
    #endif
}

} /* namespace nonstd */
