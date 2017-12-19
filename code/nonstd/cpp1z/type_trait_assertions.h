/** Type Trait Assertions
 *  =====================
 *  Macros that cause compile-time errors when we build non-compliant datatypes.
 */
#pragma once

#include "nonstd/preprocessor/symbol_stringifier.h"
#include "type_traits.h"


/** ENFORCE
 *  -------
 *  General purpose enforcement macro to simplify one-off static_asserts.
 */
#define ENFORCE(condition) \
    static_assert(condition, "Failed to enforce '" #condition "'.")


/** ENFOCE_POD
 *  ----------
 *  Verifies the given type is a POD datatype. The last assertion should be
 *  redundant, but better safe than sorry.
 */
#define ENFORCE_POD(T) \
    static_assert(IS_TRIVIALLY_COPYABLE(T), "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially copyable. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]");                           \
    static_assert(IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T), "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially default constructible. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(IS_STANDARD_LAYOUT(T), "Type '" STRING(T) "' was marked as Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")

/** ENFORCE_PODish
 *  ---------------
 *  Verifies T is almost-POD. POD-enough for government work. POD-lite.
 *  POD types are
 *    1. Standard Layout
 *    2. Trivially Copyable
 *    3. Trivially Default Constructible
 *    4. (Implicitly) Trivially Destructible
 *
 *  For PODish types, we drop the trivially default constructible requirement,
 *  because that isn't the only way to guarantee that
 *  `auto foo = reinterpret_cast<Type*>(malloc(sizeof(Type)));`
 *  yields a well constructed, correct object. PODish types can still be
 *  used in tightly packed blocks (Standard Layout), and can be cleanly and
 *  correctly set with assignment (Trivially Copyable). Creating large of arrays
 *  of PODish types is still very expensive, but that is far from an
 *  insurmountable concern.
 */
#define ENFORCE_PODish(T) \
    static_assert(IS_TRIVIALLY_COPYABLE(T), "Type '" STRING(T) "' was marked as MOSTLY Plain Old Data, but is not trivially copyable. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(IS_STANDARD_LAYOUT(T), "Type '" STRING(T) "' was marked as MOSTLY Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")

/** ENFORCE_SIZE (and friends)
 *  --------------------------
 *  Verifies the given type is exactly / under / above / within the range of
 *  the give size.
 */
#define ENFORCE_SIZE(T, bytes) \
    static_assert(sizeof(T) == bytes, "Type '" STRING(T) "' is the wrong size (it is required to be " STRING(bytes) " bytes).")
#define ENFORCE_MAX_SIZE(T, max_bytes) \
    static_assert(sizeof(T) <= bytes, "Type '" STRING(T) "' is the wrong size (it is required to be at most " STRING(bytes) " bytes).")

/** ENFORCE_[ARE/HAVE]_SAME_TYPE
 *  ----------------------------
 */
#define ENFORCE_ARE_SAME_TYPE(LEFT, RIGHT) \
    static_assert(IS_SAME_TYPE(LEFT, RIGHT), "'" STRING(LEFT) "' doesn't have the same type as '" STRING(RIGHT) "'.")

#define ENFORCE_HAVE_SAME_TYPE(LEFT, RIGHT) \
    static_assert(HAS_SAME_TYPE(LEFT, RIGHT), "Type '" STRING(LEFT) "' is not the same as type '" STRING(RIGHT) "'.")
