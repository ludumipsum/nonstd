/* Optional (Maybe) Type
   =====================

   Utility type for representing maybe-a-value.
*/

#pragma once
#include "batteries_included.h"
#include "primitive_types.h"

/* Represents a Maybe-value -- either a value of type T, or nothing. */
template<typename T>
struct Optional {
    T * value;
    bool just;

    Optional(T& value) : value(&value), just(true) { }

    inline T operator=(T _value) { *value = _value; return *value; }

    TEMPLATE_ENABLE(!std::is_pointer<T>::value, T)
    Optional() : just(false) { }
    TEMPLATE_ENABLE(std::is_pointer<T>::value, T)
    Optional() : value(nullptr), just(false) { }

    explicit inline operator bool() const { return just; }
    explicit inline operator T&() { return *value; }
    inline T& operator *  ()      { return *value; }
    inline T& operator () ()      { return *value; }
};

/* Create an optional with a real value */
template<typename T> Optional<T> just(T value) { return Optional<T>(value); }

/* Create an optional with no value */
template<typename T> Optional<T> none() { return Optional<T>(); }
