/** Type Traits
 *  ===========
 *  More accurately, helpers to make deailing with them simpler.
 *
 *  Author's Note: I've given up the 80-column limit for this file, because the
 *  macro names should be clear enough to demarcate intention, and keeping a
 *  one-line structure for the definitions provides more clarity than when the
 *  definitions are split.
 */

#pragma once

#include <type_traits>


/** Types
 *  =====
 *  Mapping exactly to `[type_trait]<Ts..>::type`.
 *
 *  NB. If we didn't want to remain consistent with the ::value helpers, these
 *  could (probably?) be accomplished with, ex;
 *  ```
 *  template<T>
 *  using type_trait_t = ::std::trait<T>::type
 *  ```
 */

/** ENABLE_IF_TYPE
 *  --------------
 *  NB. std::enable_if defaults the returned type to `void`. Macros don't let me
 *  define default arguments, so we have two `ENABLE_IF_*` macros; one
 *  (`_DTYPE`) returns the dependent type (the second argument), the other
 *  (`_TYPE`) allows the default to come into play.
 */
#define ENABLE_IF_TYPE(B)    typename ::std::enable_if<B>::type
#define ENABLE_IF_DTYPE(B,T) typename ::std::enable_if<B,T>::type

/** Type Modifiers
 *  -------------- */
#define REMOVE_REFERENCE_TYPE(T)   typename ::std::remove_reference<T>::type

#define ADD_LVAL_REFERENCE_TYPE(T) typename ::std::add_lvalue_reference<T>::type
#define ADD_RVAL_REFERENCE_TYPE(T) typename ::std::add_rvalue_reference<T>::type


#define REMOVE_CV_TYPE(T)       typename ::std::remove_cv<T>::type
#define REMOVE_CONST_TYPE(T)    typename ::std::remove_const<T>::type
#define REMOVE_VOLATILE_TYPE(T) typename ::std::remove_volatile<T>::type

#define ADD_CV_TYPE(T)          typename ::std::add_cv<T>::type
#define ADD_CONST_TYPE(T)       typename ::std::add_const<T>::type
#define ADD_VOLATILE_TYPE(T)    typename ::std::add_volatile<T>::type


#define MAKE_SIGNED(T)   typename ::std::make_signed<T>::type
#define MAKE_UNSIGNED(T) typename ::std::make_unsigned<T>::type


#define DECAY_TYPE(T) typename ::std::decay<T>::type

/** Values
 *  ======
 *  Mapping exactly to `[type_trait]<Ts...>::value`.
 *
 *  NB. We're using macros rather than inline constexpr `n2_::[type_trait]_v`
 *  variables (that C++17 will define) because inline variables (which are
 *  required for making the `_v` helpers useful) are a C++1z extension at this
 *  point. No idea when MSVC will get them, or if they're consistently available
 *  in GCC.
 */

/** IS_SAME_TYPE
 *  ------------
 *  Macros wrapping std::is_same<T,U>::value, s.t. types can be compared.
 */
#define IS_SAME_TYPE(LEFT,RIGHT)       (::std::is_same<LEFT,RIGHT>::value)
#define IS_DIFFERENT_TYPE(LEFT,RIGHT) !(::std::is_same<LEFT,RIGHT>::value)

/** HAS_SAME_TYPE
 *  -------------
 *  Macros wrapping std::is_same<T,U>::value and decltype, s.t. types of
 *  instances can be compared.
 */
#define HAS_SAME_TYPE(LEFT,RIGHT)       (::std::is_same<decltype(LEFT),decltype(RIGHT)>::value)
#define HAS_DIFFERENT_TYPE(LEFT,RIGHT) !(::std::is_same<decltype(LEFT),decltype(RIGHT)>::value)

/** Type Properties
 *  ---------------
 *  More macros wrapping `std::is_[type_trait}<T>::value`. This set focus on the
 *  properties of the given type, rather than supported operations. */
#define IS_TRIVIAL(T)      (::std::is_trivial<T>::value)
#define IS_NOT_TRIVIAL(T) !(::std::is_trivial<T>::value)

#define IS_TRIVIALLY_COPYABLE(T)      (::std::is_trivially_copyable<T>::value)
#define IS_NOT_TRIVIALLY_COPYABLE(T) !(::std::is_trivially_copyable<T>::value)

#define IS_STANDARD_LAYOUT(T)      (::std::is_standard_layout<T>::value)
#define IS_NOT_STANDARD_LAYOUT(T) !(::std::is_standard_layout<T>::value)

#define IS_POD(T)      (::std::is_pod<T>::value)
#define IS_NOT_POD(T) !(::std::is_pod<T>::value)

/** IS_REFERENCE (and friends)
 *  --------------------------
 *  Macros wrapping std::is_reference<T>::value (and l/r value specifiers) to
 *  extract the referentiality of an object.
 *  Answers the "Is this type a reference? If so, what kind?" question.
 */
#define IS_REFERENCE(T)      (::std::is_reference<T>::value)
#define IS_NOT_REFERENCE(T) !(::std::is_reference<T>::value)

#define IS_LVAL_REFERENCE(T)      (::std::is_lvalue_reference<T>::value)
#define IS_NOT_LVAL_REFERENCE(T) !(::std::is_lvalue_reference<T>::value)

#define IS_RVAL_REFERENCE(T)      (::std::is_rvalue_reference<T>::value)
#define IS_NOT_RVAL_REFERENCE(T) !(::std::is_rvalue_reference<T>::value)


/** Primary Type Properties
 *  ----------------------- */
#define IS_VOID(T)      (std::is_void<t>::value)
#define IS_NOT_VOID(T) !(std::is_void<t>::value)

#define IS_POINTER(T)      (std::is_pointer<T>::value)
#define IS_NOT_POINTER(T) !(std::is_pointer<T>::value)

#define IS_NULL_POINTER(T)      (std::is_null_pointer<T>::value)
#define IS_NOT_NULL_POINTER(T) !(std::is_null_pointer<T>::value)

#define IS_INTEGRAL(T)      (std::is_integral<T>::value)
#define IS_NOT_INTEGRAL(T) !(std::is_integral<T>::value)

#define IS_FLOATING_POINT(T)      (std::is_floating_point<T>::value)
#define IS_NOT_FLOATING_POINT(T) !(std::is_floating_point<T>::value)


/** IS_SCALAR (and friends)
 *  -----------------------
 *  Macros wrapping std::is_scalar<T>::value, and specializations thereon.
 */
#define IS_ARITHMETIC(T)      (std::is_arithmetic<T>::value)
#define IS_NOT_ARITHMETIC(T) !(std::is_arithmetic<T>::value)

#define IS_SCALAR(T)      (std::is_scalar<T>::value)
#define IS_NOT_SCALAR(T) !(std::is_scalar<T>::value)

#define IS_ENUM(T)      (std::is_enum<T>::value)
#define IS_NOT_ENUM(T) !(std::is_enum<T>::value)

#define IS_MEMBER_POINTER(T)      (std::is_member_pointer<T>::value)
#define IS_NOT_MEMBER_POINTER(T) !(std::is_member_pointer<T>::value)


/** IS_CONVERTIBLE
 *  --------------
 *  Macros wrapping std::is_convertible<TO, FROM>::value. Useful in copy/move
 *  assignment operators and constructors.
 *
 *  Note 'convertible' in this case means implicitly convertible. Specifically,
 *  this will be true iff the below imaginary function is well formed;
 *
 *      TO test() { return std::declval<FROM>(); }
 */
#define IS_CONVERTIBLE(FROM,TO)      (::std::is_convertible<FROM,TO>::value)
#define IS_NOT_CONVERTIBLE(FROM,TO) !(::std::is_convertible<FROM,TO>::value)

/** Initialization/Deinitialization Checks
 *  --------------------------------------
 *  Macros wrapping `std::is_[trivially_][type_trait]<T>::value`. Tells us what
 *  the given class can or cannot do, and the trivially_ checks will tell us
 *  what the compiler will automatically generate for us.
 */
#define IS_CONSTRUCTIBLE(T, ...)      (::std::is_constructible<T,__VA_ARGS__>::value)
#define IS_NOT_CONSTRUCTIBLE(T, ...) !(::std::is_constructible<T,__VA_ARGS__>::value)
#define IS_TRIVIALLY_CONSTRUCTIBLE(T, ...)      (::std::is_trivially_constructible<T,__VA_ARGS__>::value)
#define IS_NOT_TRIVIALLY_CONSTRUCTIBLE(T, ...) !(::std::is_trivially_constructible<T,__VA_ARGS__>::value)
#define IS_NOTHROW_CONSTRUCTIBLE(T, ...)      (::std::is_nothrow_constructible<T,__VA_ARGS__>::value)
#define IS_NOT_NOTHROW_CONSTRUCTIBLE(T, ...) !(::std::is_nothrow_constructible<T,__VA_ARGS__>::value)

#define IS_DEFAULT_CONSTRUCTIBLE(T)      (::std::is_default_constructible<T>::value)
#define IS_NOT_DEFAULT_CONSTRUCTIBLE(T) !(::std::is_default_constructible<T>::value)
#define IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)      (::std::is_trivially_default_constructible<T>::value)
#define IS_NOT_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T) !(::std::is_trivially_default_constructible<T>::value)
#define IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)      (::std::is_nothrow_default_constructible<T>::value)
#define IS_NOT_NOTHROW_DEFAULT_CONSTRUCTIBLE(T) !(::std::is_nothrow_default_constructible<T>::value)

#define IS_COPY_CONSTRUCTIBLE(T)      (::std::is_copy_constructible<T>::value)
#define IS_NOT_COPY_CONSTRUCTIBLE(T) !(::std::is_copy_constructible<T>::value)
#define IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)      (::std::is_trivially_copy_constructible<T>::value)
#define IS_NOT_TRIVIALLY_COPY_CONSTRUCTIBLE(T) !(::std::is_trivially_copy_constructible<T>::value)
#define IS_NOTHROW_COPY_CONSTRUCTIBLE(T)      (::std::is_nothrow_copy_constructible<T>::value)
#define IS_NOT_NOTHROW_COPY_CONSTRUCTIBLE(T) !(::std::is_nothrow_copy_constructible<T>::value)

#define IS_MOVE_CONSTRUCTIBLE(T)      (::std::is_move_constructible<T>::value)
#define IS_NOT_MOVE_CONSTRUCTIBLE(T) !(::std::is_move_constructible<T>::value)
#define IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)      (::std::is_trivially_move_constructible<T>::value)
#define IS_NOT_TRIVIALLY_MOVE_CONSTRUCTIBLE(T) !(::std::is_trivially_move_constructible<T>::value)
#define IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)      (::std::is_nothrow_move_constructible<T>::value)
#define IS_NOT_NOTHROW_MOVE_CONSTRUCTIBLE(T) !(::std::is_nothrow_move_constructible<T>::value)


#define IS_ASSIGNABLE(T,U)      (::std::is_assignable<T,U>::value)
#define IS_NOT_ASSIGNABLE(T,U) !(::std::is_assignable<T,U>::value)
#define IS_TRIVIALLY_ASSIGNABLE(T,U)      (::std::is_trivially_assignable<T,U>::value)
#define IS_NOT_TRIVIALLY_ASSIGNABLE(T,U) !(::std::is_trivially_assignable<T,U>::value)
#define IS_NOTHROW_ASSIGNABLE(T,U)      (::std::is_nothrow_assignable<T,U>::value)
#define IS_NOT_NOTHROW_ASSIGNABLE(T,U) !(::std::is_nothrow_assignable<T,U>::value)

#define IS_COPY_ASSIGNABLE(T)      (::std::is_copy_assignable<T>::value)
#define IS_NOT_COPY_ASSIGNABLE(T) !(::std::is_copy_assignable<T>::value)
#define IS_TRIVIALLY_COPY_ASSIGNABLE(T)      (::std::is_trivially_copy_assignable<T>::value)
#define IS_NOT_TRIVIALLY_COPY_ASSIGNABLE(T) !(::std::is_trivially_copy_assignable<T>::value)
#define IS_NOTHROW_COPY_ASSIGNABLE(T)      (::std::is_nothrow_copy_assignable<T>::value)
#define IS_NOT_NOTHROW_COPY_ASSIGNABLE(T) !(::std::is_nothrow_copy_assignable<T>::value)

#define IS_MOVE_ASSIGNABLE(T)      (::std::is_move_assignable<T>::value)
#define IS_NOT_MOVE_ASSIGNABLE(T) !(::std::is_move_assignable<T>::value)
#define IS_TRIVIALLY_MOVE_ASSIGNABLE(T)      (::std::is_trivially_move_assignable<T>::value)
#define IS_NOT_TRIVIALLY_MOVE_ASSIGNABLE(T) !(::std::is_trivially_move_assignable<T>::value)
#define IS_NOTHROW_MOVE_ASSIGNABLE(T)      (::std::is_nothrow_move_assignable<T>::value)
#define IS_NOT_NOTHROW_MOVE_ASSIGNABLE(T) !(::std::is_nothrow_move_assignable<T>::value)


#define IS_DESTRUCTIBLE(T)      (::std::is_destructible<T>::value)
#define IS_NOT_DESTRUCTIBLE(T) !(::std::is_destructible<T>::value)
#define IS_TRIVIALLY_DESTRUCTIBLE(T)      (::std::is_trivially_destructible<T>::value)
#define IS_NOT_TRIVIALLY_DESTRUCTIBLE(T) !(::std::is_trivially_destructible<T>::value)
#define IS_NOTHROW_DESTRUCTIBLE(T)      (::std::is_nothrow_destructible<T>::value)
#define IS_NOT_NOTHROW_DESTRUCTIBLE(T) !(::std::is_nothrow_destructible<T>::value)


/** TEMPLATE_ENABLE
 *  ===============
 *  std::enable_if convenience wrapper.
 *  Utility tools for easily enabling/disabling functions in templated
 *  types based on a trait or boolean expression. Usage requires passing
 *  in a parameter from the class template in the first variable of the
 *  macro. For example, if you had:
 *
 *      template<bool ConfigBoolean, class T> MyClass { ... };
 *
 *  You'd use use the macro like so:
 *
 *      TEMPLATE_ENABLE(ConfigBoolean, T) void EitherOr() { ... };
 *      TEMPLATE_ENABLE(!ConfigBoolean, T) void EitherOr() { ... };
 *
 *  Which will yield only the correct version of that function being chosen
 *  during template specialization based on the template parameter.
 *  Poor man's type-level overloading.
 *  Poorer man's pattern matching.
 *
 *  Notes on the implementation;
 *  - For this form to correctly operate using SFINAE, the `enable_if`
 *    expression needs to return a dependent type. `_DEP_T` is explicitly
 *    defined based on `T` -- but is not T -- to guarantee a dependent type.
 *    TODO: Experiment some more and guarantee this is the most robust form we
 *          can safely use... I have a feeling this... isn't.
 *  - `_DEP_T` needs to be a type decayed from `T` for the cases in which T is a
 *    reference type. When SFINAE succeeds, we're defaulting the second template
 *    parameter to `_DEP_T * = nullptr`, and `T& * = nullptr` ain't so good.
 *  - Because TEMPLATE_ENABLE is defining and operating on a dependent type that
 *    is not cleanly accessible outside of the macro, users will need to be very
 *    careful with the signatures of functions they disable with this macro.
 *    Take for example the below;
 *
 *        TEMPLTE_ENABLE(IS_NOT_REFERENCE_TYPE(T), T)
 *        T * getAPointer() { ... }
 *
 *    We want this function to be defined when T is not a reference type ---
 *    when `T *` is a valid expression.
 *    ** This will fail to compile. **
 *    `T` (which is a clas-template type, defined as part of the class
 *    declaration) will be defined before template argument deduction occurs for
 *    this function, so before the `enable_if` clause can remove this function
 *    from the overload set. `T *` will be resolved to ex; `u32 & *`, which is a
 *    hard error outside of the immediate resolution context, so will not be
 *    ignored by SFINAE.
 */
#define TEMPLATE_ENABLE(COND, T)            \
    template<typename _DEP_T=DECAY_TYPE(T), \
             ENABLE_IF_DTYPE(COND,_DEP_T) * = nullptr>
