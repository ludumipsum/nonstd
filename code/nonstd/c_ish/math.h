/** Min/Max Macros
 *  ==============
 *  Both type- and compiler-safe.
 */

#pragma once


#ifndef n2max
#define n2max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef n2min
#define n2min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef n2clamp
#define n2clamp(v, a, b) (n2min((b), (((v) > (a)) ? v : a)))
#endif
#ifndef n2abs
#define n2abs(n) std::abs(n)
#endif