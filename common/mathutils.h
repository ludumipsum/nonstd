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
template<typename T>
inline T rescale(T value, T domain_min, T domain_max,
                   T range_min,  T range_max) {
    T range_size = range_max - range_min;
    return range_min + range_size * (   (    value   - domain_min )
                                      / ( domain_max - domain_min ) );
}

template<typename T>
inline T rescale(T value, T domain_max, T range_max) {
    return rescale(value, (T)0, domain_max, (T)0, range_max);
}
