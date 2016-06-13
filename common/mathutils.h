/* Math Utilities
   ==============

   A small library of commonly-used math functions. Stuff like interpolators
   and rescalers.
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

/* Min/Max Macros
   --------------
   Both type- and compiler-safe.
*/
#ifndef n2max
#define n2max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef n2min
#define n2min(a,b) (((a) < (b)) ? (a) : (b))
#endif


/* Range Rescaling
   ---------------
   Take a value in a given domain and rescale it to the provided range.
*/
inline template<typename T>
T rescale(T value, decltype(value) domain_min, decltype(value) domain_max,
                   decltype(value) range_min,  decltype(value) range_max) {
    T range_size = range_max - range_min;
    return range_min + range_size * (   (    value   - domain_min )
                                      / ( domain_max - domain_min ) );
}

inline template<typename T>
T rescale(T value, decltype(value) domain_max,
                   decltype(value) range_max) {
    return rescale(value, (T)0, domain_max, (T)0, range_max);
}
