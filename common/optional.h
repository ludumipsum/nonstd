/* OPTIONAL TYPE
   =============

   Utility type for representing maybe-a-value.
*/

#pragma once
#include "batteries_included.h"
#include "primitive_types.h"

/* Represents a Maybe-value -- either a value of type T, or nothing. */
template<typename T>
struct Optional {
    T value;
    bool just;

    Optional(T value) : value(value), just(true) { }
    Optional() : just(false) { }

    inline operator bool() const { return just; }
    inline operator T() { return value; }
    inline T& operator *  () { return value; }
    inline T& operator () () { return value; }
};

/* Create an optional with a real value */
template<typename T> Optional<T> just(T value) { return Optional<T>(value); }

/* Create an optional with no value */
template<typename T> Optional<T> none() { return Optional<T>(); }
