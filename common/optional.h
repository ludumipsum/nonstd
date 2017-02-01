/* Optional (Maybe) Type
   =====================

   Utility type for representing maybe-a-value.
*/

#pragma once
#include "batteries_included.h"
#include "primitive_types.h"

#if !defined(N2_CHECKED_OPTIONALS)
#define N2_CHECKED_OPTIONALS false
#endif

/* Represents a Maybe-value -- either a value of type T, or nothing. */
template<typename T>
struct Optional {
protected:
    /* This anonymous-union-struct nesting is required to allow the compiler to
     * generate a form of the optional that doesn't include a valid reference,
     * which is otherwise in violation of the "never nullable" contract provided
     * by reference variables. The inner struct is required on some compilers
     * which would otherwise complain about reference members of a union since
     * according to the standard, the storage required by a reference is
     * implementation-specific, and it is therefore undefined behavior to rely
     * on a particular size of a reference when sizing a struct, breaking the
     * portability requirement of POD structs.
     */
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
#if N2_CHECKED_OPTIONALS
        if (!just) { BREAKPOINT(); }
#endif
        return value;
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
