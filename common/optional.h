/* Optional (Maybe) Type
   =====================

   Utility type for representing maybe-a-value.
*/

#pragma once
#include "batteries_included.h"
#include "primitive_types.h"

#include "crash.h"

#if !defined(N2_CHECKED_OPTIONALS)
#define N2_CHECKED_OPTIONALS false
#endif

// Helper define to save some space on repetitive SFINAE templates.
// Directly use n2_::enable_if_t because macro-in-macro expansion is hard.
#define __OPT_ENABLE_IF_CONVERTABLE(U,T) \
    typename n2_::enable_if_t<std::is_convertible<U,T>::value> * = nullptr


/* Optional Templatized Forward Declaration
 * This only exists to allow SFINAE resolution to select on partially
 * specialized versions that adhere to certain type traits.
 */
template <typename T, typename Enable = void> class Optional;


/* Helper Creation Functions
 * -------------------------
 * When you want to specify the type as part of an assignment or return.
 */

/* Create an optional with a real value */
template<typename T> Optional<T> just(T value) {
    return Optional<T>(value);
}
template<typename T> Optional<T &> just_ref(T & value) {
    return Optional<T &>(value);
}
template<typename T> Optional<T const &> just_cref(T const& value) {
    return Optional<T const &>(value);
}

/* Create an optional with no value */
template<typename T> Optional<T> none() { return Optional<T>(); }



/* Optional Over Value Type
 * =============================================================================
 */
template <typename T>
class Optional<T, ENABLE_IF_TYPE(IS_NOT_REFERENCE_TYPE(T), void) > {
public:
    using storage_type = T;

private:
    /* This union trick allows for a single initialization or assignment when
     * constructing empty Optionals; only setting `__just` to will let us skip
     * initializing `__value` even if we're about to construct a valid value
     * therein, which lets us faithfully model an uninitialized object.
     * `__padding_for_just` allows us to manipulate `__value` without affecting
     * the state of `__just`.
     */
    union {
        bool __just;
        struct {
            bool         __padding_for_just[1];
            storage_type __value;
        };
    };

public:
    /* Constructors
     * ------------
     */

    /* Default Construction; Creates a non-containing Optional. */
    constexpr Optional() : __just ( false ) { }


    /* Construction / Assignment By Value;
     * Creates containing Optional wrapping a value equivalent to `value`.
     */

    /* Construct via literal or lvalue copy */
    constexpr Optional(T const & value) : __just ( true ) {
        constructValue(value);
    }
    /* Construct via xvalue move */
    constexpr Optional(T && value) : __just ( true ) {
        constructValue(std::forward<T>(value));
    }
    /* Assign via literal or lvalue copy */
    constexpr Optional<T>& operator= (T const & value) {
        __just = true;
        constructValue(value);
        return *this;
    }
    /* Assign via xvalue move*/
    constexpr Optional<T>& operator= (T && value) {
        __just = true;
        constructValue(std::move(value));
        return *this;
    }

    /* Construction / Assignment By Copy/Move;
     * Uses an `other` Optional to construct; will be containing or
     * non-containing based on the state of the given `other`.
     * NB. `typename U` is used to ensure coercion if `other.storage_type` != T,
     * and SFINAE is used to prevent these constructors from being considered in
     * overload resolution if U is not implicitly convertible into T.
     * TODO: Would a static assert be more appropriate for disallowing this? */

    /* Copy ctor */
    template <typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
    constexpr Optional(Optional<U> const & other) : __just ( other.__just ) {
        if ((ptr)(this) != (ptr)(&other)) {
            if (__just) { constructValue(other.__value); }
        }
    }
    /* Move ctor */
    template <typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
    constexpr Optional(Optional<U> && other) : __just ( other.__just) {
        if (__just) {
            other.__just = false;
            constructValue(std::move(other.__value));
        }
    }
    /* Copy assignment */
    template <typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
    constexpr Optional<T>& operator= (Optional<U> const & other) {
        if ((ptr)(this) != (ptr)(&other) ) {
            __just = other.__just;
            if (__just) { constructValue(other.__value); }
        }
        return *this;
    }
    /* Move assignment */
    template <typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
    constexpr Optional<T>& operator= (Optional<U> && other) {
        __just = other.__just;
        if (__just) {
            other.__just = false;
            constructValue(std::move(other.__value));
        }
        return *this;
    }


    /* Helper function to simplify assigning or constructing a valid value.
     * Uses placement `new` to allow for all kinds of ctor / assignments.
     * Casts the address of __value to `void *` to guarantee valid placement.
     */
    template <typename... Args>
    constexpr void constructValue(Args&&... args) {
        new ((void *)(&this->__value)) T(std::forward<Args>(args)...);
    }

    /* Helper function to (optionally) check the validity of this Optional. */
    constexpr void checkValue() const {
#if N2_CHECKED_OPTIONALS
        if (!__just) { BREAKPOINT(); }
#endif
    }


    /* Access operators
     * ---------------- */
    constexpr bool hasValue() const { return __just; }

    constexpr T       &  value()       &  { checkValue(); return __value; }
    constexpr T const &  value() const &  { checkValue(); return __value; }

    constexpr T       && value()       && { checkValue();
                                            return std::move(__value); }
    constexpr T const && value() const && { checkValue();
                                            return std::move(__value); }

    /* Access w/ default;
     * NB. Because the `!__just` clause requires returning the value passed into
     * the function call, these _can not_ return references.
     */
    template <typename U=T>
    constexpr T valueOr(U && _default) const & {
        if (__just) { return __value;                                   }
        else        { return static_cast<T>(std::forward<U>(_default)); }
    }
    template <typename U=T>
    constexpr T valueOr(U && _default) &&      {
        if (__just) { return std::move(__value);                        }
        else        { return static_cast<T>(std::forward<U>(_default)); }
    }

    constexpr T       *  operator -> ()       { return &(value()); }
    constexpr T const *  operator -> () const { return &(value()); }

    constexpr T       &  operator () ()       &  { return value(); }
    constexpr T const &  operator () () const &  { return value(); }
    constexpr T       && operator () ()       && { return std::move(value()); }
    constexpr T const && operator () () const && { return std::move(value()); }

    constexpr T       &  operator *  ()       &  { return value(); }
    constexpr T const &  operator *  () const &  { return value(); }
    constexpr T       && operator *  ()       && { return std::move(value()); }
    constexpr T const && operator *  () const && { return std::move(value()); }

    /* Coercion operators */
    constexpr explicit operator bool() const { return __just; }


    /* Optionals must all be friends to one another. */
    template <typename U, typename V> friend class Optional;

    static_assert(IS_TRIVIALLY_DESTRUCTIBLE(T),
                  "N2 Optionals are currently not designed to wrap objects "
                  "that are not trivially destructible (those that define, or "
                  "contain something that defines, a destructor).");
};



/* Optional Over Reference Type
 * =============================================================================
 */
template <typename T>
class Optional<T, typename ENABLE_IF_TYPE(IS_LVAL_REFERENCE_TYPE(T), void) > {
public:
    using storage_type = REMOVE_REFERENCE_TYPE(T);
    using const_storage_type = ADD_CONST_TYPE(storage_type);

private:
    /* This anonymous union allows us to leverage the same trick that is used
     * Optionals over Value Types (see above for details).
     * Because this version of Optionals are modeling references and the
     * absence-of-references (ex; u32& referencing a valid object, or nothing)
     * we can't directly store the wrapped reference (references sizes are
     * undefined so they can't be members in enums and unions, they can never
     * be invalid, and they can never be reseated). Instead, we allocate
     * storage for a pointer, and perform conversions to and from reference and
     * raw types as needed.
     */
    union {
        bool __just;
        struct {
            bool           __padding_for_just[1];
            storage_type * __value_ptr;
        };
    };

public:
    /* Constructors
     * ------------
     */

    /* Default Construction; Creates a non-containing Optional. */
    constexpr Optional() : __just ( false ) { }


    /* Construction / Assignment By Value;
     * Creates a containing Optional wrapping a pointer to value of `value_ref`.
     * NB. Because T is an lvalue reference, distinguishing between copy and
     *     move semantics isn't meaningful.
     */

    /* Construct via address-of lvalue */
    constexpr Optional(T value_ref) : __just ( true ) {
        __value_ptr = &value_ref;
    }
    /* Assign via address-of lvalue */
    constexpr Optional<T>& operator= (T value_ref) {
        __just = true;
        __value_ptr = &value_ref;
        return *this;
    }

    /* Construction / Assignment By Copy/Move;
     * Uses an `other` Optional to construct; will be containing or
     * non-containing based on the state of the given `other`.
     */

    /* Copy ctor */
    constexpr Optional(Optional<T> const & other) : __just ( other.__just ) {
        if ((ptr)(this) != (ptr)(&other)) {
            if (__just) { __value_ptr = other.__value_ptr; }
        }
    }
    /* Move ctor */
    constexpr Optional(Optional<T> && other) : __just ( other.__just) {
        if (__just) {
            __value_ptr = other.__value_ptr;
            other.__just = false;
            other.__value_ptr = 0;
        }
    }
    /* Copy assignment */
    constexpr Optional<T>& operator= (Optional<T> const & other) {
        if ((ptr)(this) != (ptr)(&other) ) {
            __just = other.__just;
            if (__just) { __value_ptr = other.__value_ptr; }
        }
        return *this;
    }
    /* Move assignment */
    constexpr Optional<T>& operator= (Optional<T> && other) {
        __just = other.__just;
        if (__just) {
            __value_ptr = other.__value_ptr;
            other.__just = false;
            other.__value_ptr = 0;
        }
        return *this;
    }


    /* Helper function to (optionally) check the validity of this Optional. */
    constexpr void checkValue() const {
#if N2_CHECKED_OPTIONALS
        if (!__just) { BREAKPOINT(); }
#endif
    }

    /* Access operators
     * ---------------- */
    constexpr bool hasValue() const { return __just; }

    constexpr storage_type & value() {
        checkValue();
        return *__value_ptr;
    }
    constexpr const_storage_type & value() const {
        checkValue();
        return *__value_ptr;
    }

    /* Access w/ default;
     * NB. Because the `!__just` clause requires returning the value passed into
     * the function call, these _can not_ return references.
     */
    template <typename U=storage_type>
    constexpr storage_type valueOr(U && _default) {
        if (__just) { return *__value_ptr; }
        else {
            return static_cast<storage_type>(std::forward<U>(_default));
        }
    }

    constexpr storage_type       *  operator -> ()       { return &(value()); }
    constexpr const_storage_type *  operator -> () const { return &(value()); }

    constexpr storage_type       &  operator () ()       { return value(); }
    constexpr const_storage_type &  operator () () const { return value(); }

    constexpr storage_type       &  operator *  ()       { return value(); }
    constexpr const_storage_type &  operator *  () const { return value(); }

    /* Coercion operators */
    constexpr explicit operator bool() const { return __just; }


    /* Optionals must all be friends to one another. */
    template <typename U, typename V> friend class Optional;

    static_assert(IS_TRIVIALLY_DESTRUCTIBLE(T),
                  "N2 Optionals are currently not designed to wrap objects "
                  "that are not trivially destructible (those that define, or "
                  "contain something that defines, a destructor).");
};



/* Equality and Relationship Comparisons
 * =============================================================================
 */

/* Compare two Optionals
 * ---------------------
 *  * If both Optionals are containing, compare their values as normal.
 *  * A non-containing Optional is always considered to be less than a
 *    containing Optional.
 *  * If both Optionals are non-containing, they are considered equal.
 */

template <typename T, typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
constexpr bool operator == (Optional<T> const & l, Optional<U> const & r) {
    if ((bool)(l) != (bool)(r)) { return false; }
    if ((bool)(l))              { return l() == r(); }
    return true;
}

template <typename T, typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
constexpr bool operator != (Optional<T> const & l, Optional<U> const & r) {
    if ((bool)(l) != (bool)(r)) { return true; }
    if ((bool)(l))              { return l() != r(); }
    return false;
}

template <typename T, typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
constexpr bool operator >  (Optional<T> const & l, Optional<U> const & r) {
    if ((bool)(l) != (bool)(r)) { return (bool)(l); }
    if ((bool)(l))              { return l() >  r(); }
    return false;
}

template <typename T, typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
constexpr bool operator >= (Optional<T> const & l, Optional<U> const & r) {
    if ((bool)(l) != (bool)(r)) { return (bool)(l); }
    if ((bool)(l))              { return l() >= r(); }
    return true;
}

template <typename T, typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
constexpr bool operator <  (Optional<T> const & l, Optional<U> const & r) {
    if ((bool)(l) != (bool)(r)) { return ! (bool)(l); }
    if ((bool)(l))              { return l() <  r(); }
    return false;
}

template <typename T, typename U, __OPT_ENABLE_IF_CONVERTABLE(U,T)>
constexpr bool operator <= (Optional<T> const & l, Optional<U> const & r) {
    if ((bool)(l) != (bool)(r)) { return ! (bool)(l); }
    if ((bool)(l))              { return l() <= r(); }
    return true;
}

/* Compare an Optional and a value
 *  * An Empty Optional is always considered to be less than a Value.
 */
template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator == (Optional<T> const & l, Value const & r) {
    return   (bool)(l) && l() == r;
}
template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator == (Value const & l, Optional<T> const & r) {
    return   (bool)(r) && l == r();
}

template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator != (Optional<T> const & l, Value const & r) {
    return ! (bool)(l) || l() != r;
}
template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator != (Value const & l, Optional<T> const & r) {
    return ! (bool)(r) || l != r();
}

template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator >  (Optional<T> const & l, Value const & r) {
    return   (bool)(l) && l() > r;
}
template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator >  (Value const & l, Optional<T> const & r) {
    return ! (bool)(r) || l > r();
}

template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator >= (Optional<T> const & l, Value const & r) {
    return   (bool)(l) && l() >= r;
}
template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator >= (Value const & l, Optional<T> const & r) {
    return ! (bool)(r) || l >= r();
}

template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator <  (Optional<T> const & l, Value const & r) {
    return ! (bool)(l) || l() < r;
}
template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator <  (Value const & l, Optional<T> const & r) {
    return   (bool)(r) && l < r();
}

template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator <= (Optional<T> const & l, Value const & r) {
    return ! (bool)(l) || l() <= r;
}
template <typename T, typename Value, __OPT_ENABLE_IF_CONVERTABLE(Value,T)>
constexpr bool operator <= (Value const & l, Optional<T> const & r) {
    return   (bool)(r) && l <= r();
}
