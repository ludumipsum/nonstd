/* Optional (Maybe) Type
   =====================

   Utility type for representing maybe-a-value.
*/

#pragma once
#include "batteries_included.h"
#include "primitive_types.h"

#if !defined(N2_UNCHECKED_OPTIONALS)
#define N2_UNCHECKED_OPTIONALS false
#endif

/* Represents a Maybe-value -- either a value of type T, or nothing. */
template<typename T>
struct Optional {
protected:
    union {
        struct { T value; };
        void *_junk;
    };
    bool just;

public:
    Optional(T value) : value(value), just(true) { }
    Optional() : _junk(nullptr), just(false) { }

    /* Coercion operators */
    explicit inline operator bool() const { return just; }

    /* Access operators */
    inline auto operator -> ()    { return &(this->operator*()); }
    inline T& operator () ()      { return   this->operator*(); }
    inline T& operator *  ()      {
#if N2_UNCHECKED_OPTIONALS
        return value;
#else
        if (!just) { BREAKPOINT(); }
        return value;
#endif
    }
};

/* Create an optional with a real value */
template<typename T> Optional<T> just(T value) { return Optional<T>(value); }
template<typename T> Optional<T&> just_ref(T& value) {
    return Optional<T&>(value);
}
template<typename T> Optional<T const&> just_cref(T const& value) {
    return Optional<T const&>(value);
}

/* Create an optional with no value */
template<typename T> Optional<T> none() { return Optional<T>(); }
