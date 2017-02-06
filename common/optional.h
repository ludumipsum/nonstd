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
        n2_decay_t<T>  value;
        n2_decay_t<T>* value_ptr;
        void *_junk;
    };
    bool just;

    /* Autodetect whether T is a reference or not, and compile the appropriate
     * version of _getValue based on that. */
    template<bool = std::is_reference<T>::value>
    T& _getValue();
	template<> /* Accessor for option-by-reference instances */
    T& _getValue<true>() {
        return *value_ptr;
    }
	template<> /* Accessor for option-by-value instances */
    T& _getValue<false>() {
        return value;
    }

    /* Value setter templated on option-by-reference vs option-by-value */
    template<bool = std::is_reference<T>::value>
    void _setValue(T& _value);
    template<> /* Setter for option-by-reference instances */
    void _setValue<true>(T& _value) {
        value_ptr = (decltype(value_ptr))(&_value);
    }
    template<> /* Setter for option-by-value instances */
    void _setValue<false>(T& _value) {
        value = _value;
    }

public:
    Optional(T& _value) : _junk(nullptr), just(true) {
        _setValue(_value);
    }

    /* Constructor for empty (None) optionals */
    Optional() : _junk(nullptr), just(false) { }

    /* Coercion operators */
    explicit inline operator bool() const { return just; }

    /* Access operators */
    inline auto operator -> () { return &(this->operator*()); }
    inline auto operator () () { return   this->operator*();  }
    inline T& operator * () {
#if N2_CHECKED_OPTIONALS
        if (!just) { BREAKPOINT(); }
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
