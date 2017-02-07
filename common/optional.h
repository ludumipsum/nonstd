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
    /* For reference types (i.e. T is a reference, something like u32&) we can't
     * store the value in the usual enum directly, because reference sizes are
     * undefined, and reference members in enums are illegal according to the
     * standard. We therefore strip the referentiality off for those and store
     * them as pointers that we then convert to in _getValue/_setValue, which
     * preserves the API contract of operating on reference types without having
     * to directly store one.
     */
    union {
        DECAY_TYPE(T)  value;
        DECAY_TYPE(T)* value_ptr;
        void *junk;
    };
    bool just;

    /* Accessor for option-by-value instances */
    TEMPLATE_ENABLE(IS_NOT_REFERENCE_TYPE(T), T)
    ADD_REFERENCE_TYPE(T) _getValue() { return value; }

    /* Accessor for option-by-reference instances */
    TEMPLATE_ENABLE(IS_REFERENCE_TYPE(T), T)
    ADD_REFERENCE_TYPE(T) _getValue() { return *value_ptr; }

public:
    /* Constructor for option-by-value instances */
    TEMPLATE_ENABLE(IS_NOT_REFERENCE_TYPE(T), T)
    Optional(T value) : value(value), just(true) { }

    /* Constructor for option-by-reference instances */
    TEMPLATE_ENABLE(IS_REFERENCE_TYPE(T), T)
    Optional(T value) : value_ptr((decltype(value_ptr))(&value)), just(true) { }

    /* Constructor for empty (None) optionals */
    Optional() : junk(nullptr), just(false) { }

    /* Coercion operators */
    explicit inline operator bool() const { return just; }

    /* Access operators */
    inline auto  operator -> () { return &(this->operator*()); }
    inline auto& operator () () { return   this->operator*();  }
    inline auto& operator * () {
#if N2_CHECKED_OPTIONALS
        if (!storage.just) { BREAKPOINT(); }
#endif
        return _getValue();
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
