/** Min/Max Macros
 *  ==============
 *  Both type- and compiler-safe.
 */
//TODO: I'm not sure these are really type-safe. We should probably turn these
// into function templates and move them into nonstd::

#pragma once

#include <cmath>


#ifndef n2max
#define n2max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef n2min
#define n2min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef n2clamp
#define n2clamp(val, min, max) (n2min((max), n2max((val), (min))))
#endif
#ifndef n2abs
#define n2abs(n) std::abs(n)
#endif
