/** Compoasable Predicate Objects
 *  =============================
 *  This file defines a type that implements the Predicate concept from the C++
 *  standard library in a form that allows for composition into boolean logic
 *  trees. The two interesting interactions this system allows for are 1)
 *  decoupling the composition of the boolean test from the object being tested
 *  and 2) arbitrarily deep composition with previously defined predicate
 *  objets.
 *
 *  Any tree of boolean expressions that test against a single object can be
 *  expressed using this system. For example,
 *
 *  ```
 *  bool A(X const & x) { ... }
 *  bool C(X const & x) { ... }
 *  bool B(X const & x) { ... }
 *  X x;
 *  bool b = (A(x) && (B(x) || !C(x)));
 *  ```
 *
 *  Can be encoded as,
 *
 *  ```
 *  predicate_t<X> A { ... };
 *  predicate_t<X> B { ... };
 *  predicate_t<X> C { ... };
 *  predicate_t<X> complete_test = A && (B || !C);
 *  X x;
 *  bool b = complete_test(x);
 *  ````
 *
 *  The later requires template specializations, an additional variable
 *  definition, and more lines of code. This should tell you that this tool is
 *  not a replacement for boolean expressions. It's an example of C++ solutions
 *  making simple tasks somewhat difficult so that it can make difficult tasks
 *  somewhat simple.
 */

#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/utility_ext.h>
#include <functional>
#include <iostream>


namespace nonstd {

/** Logical Predicate Object
 *  ========================
 *  A `predicate_t` object can be initialized by providing an instance of any
 *  callable object -- typically a lambda or a function-object, but a free
 *  function also works -- so long as that object fulfills the interface
 *  `bool operator() (T const &) const`. ex;
 *
 *      predicate_t<int> is_three { [](int const & i) { return i == 3; } };
 *      ASSERT(is_three(3));
 *
 *  This interface is encoded in the `predicate_t<T>::interface_t` pure-virtual,
 *  which is the key to this class's type erasure. If the given callable is not
 *  already derived from the interface type, a new class will be instantiated
 *  (from the `predicate_passthrough_t` class template) to store the given
 *  callable as a member. (Types derived from `interface_t` receive special
 *  treatment; see below.) No mater the inheritance tree of the callable, each
 *  `predicate_t` will maintain a `shared_ptr<interface_t>` as a member that
 *  encapsulates the callable in its type-erased form. It's important to note;
 *  this does mean every `predicate_t` implies a heap allocation and leverages a
 *  virtual table, but these details are just that --- details entirely private
 *  to the `predicate_t`. This allows us to store, pass, construct, and othewise
 *  reason about e.g. 'predicates that test the value of `int`s'
 *  (`predicate_t<int>`s) without needing to worry about how the specific test
 *  is implemented, from what base class the tests are derived, or the lifetime
 *  of the function-object that will perform the test.
 *
 *  For those following along at home, this is an instance of the concept-model
 *  idiom as described by Sean Parent. The `interface_t` is the abstract
 *  `concept` of this implementation, and the `predicate_passthrough_t` is the
 *  idiom's canonical, templatized `model`. If you want to learn more about the
 *  idiom, Parent's CppCon 2017 "Better Code: Runtime Polymorphism" talk is the
 *  place to look;
 *  youtube.com/watch?v=QGcVXgEVMJg
 *
 *  After construction, predicate objects can be composed, using C++'s logical
 *  operators, to construct new a predicate that tests the logical combination
 *  you'd expect; `==`, `!=`, `&&`, `||`, and the unary `!`. These operations
 *  will not modify the composed predicates, and will copy the minimal set of
 *  state required to construct a new predicate object.
 *
 *  The `predicate_t<T>::interface_t` can derived from directly, and such
 *  objects will be given special treatment in the context of predicate
 *  objects. They can be composed with pre-existing predicates, with one
 *  another, or with themselves to generate new predicate objects. This allows
 *  us to reuse the bulk of a single test's logic, but vary e.g. target values,
 *  default returns, persisted state. I think of them sort of like predicate
 *  factories, but instead of calling a functions you just initialize an object.
 */
template <typename T>
class predicate_t {
public:
    /** The Interface
     *  -------------
     *  A callable object that accepts a `T` and returns a `bool`.
     */
    struct interface_t {
        virtual ~interface_t() = default;
        virtual bool operator() (T const & obj) const = 0;

        // Please read through `predicate_t` before looking at this section.
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // If I thought it was possible, these would be declarations, and their
        // definitions would appear out-of-line next to the `predicate_t`'s
        // logical operator overloads. I do not think this is possible. As such
        // I'm left using a number of private implementation details before
        // they've been declared. This is gross, and I am sorry.

        /** Helper trait to make friend declarations easier to write. */
        template <typename U>
        static inline constexpr
        bool is_parent_of_v = std::is_base_of_v<interface_t, U>;

        /** Boolean-Logic, Free-Function, Operator Overload Templates
         *  ---------------------------------------------------------
         *  Compose any two (or one) subclasses (or subclass) of `interface_t`
         *  into a new predicate object, copying the given instances (or
         *  instance) as necessary.
         */

        template <typename U, typename V>
        friend auto operator== (U const & lhs, V const & rhs)
        -> /* predicate_t */
        std::enable_if_t<is_parent_of_v<U> && is_parent_of_v<V>, predicate_t> {
            return equal_to_predicate_t { lhs, rhs };
        }

        template <typename U, typename V>
        friend auto operator!= (U const & lhs, V const & rhs)
        -> /* predicate_t */
        std::enable_if_t<is_parent_of_v<U> && is_parent_of_v<V>, predicate_t> {
            return not_equal_to_predicate_t { lhs, rhs };
        }

        template <typename U, typename V>
        friend auto operator&& (U const & lhs, V const & rhs)
        -> /* predicate_t */
        std::enable_if_t<is_parent_of_v<U> && is_parent_of_v<V>, predicate_t> {
            return logical_and_predicate_t { lhs, rhs };
        }

        template <typename U, typename V>
        friend auto operator|| (U const & lhs, V const & rhs)
        -> /* predicate_t */
        std::enable_if_t<is_parent_of_v<U> && is_parent_of_v<V>, predicate_t> {
            return logical_or_predicate_t { lhs, rhs };
        }

        template <typename U>
        friend auto operator! (U const & rhs)
        -> /* predicate_t */
        std::enable_if_t<is_parent_of_v<U>, predicate_t> {
            return logical_not_predicate_t { rhs };
        }
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Carry on.
    };

    // Actually, no wait
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // MSCV and GCC/Clang disagree on the default friendliness of these friend
    // operator templates --- inner-class friend-function templates are not
    // friends of the outer class in MSVC. As such, we need to explicitly define
    // these as friends to the `predicate_t`.
    // Note, we still need to define them in `interface_t` to make sure ADL
    // works correctly.
    // Also note, we need to use the _exact_ same signature as the functions
    // above, template parameters -- and their default arguments -- included.
    // That last piece means we have to perform SFINAE in the return type, since
    // we can't redefine default template arguments.
    template <typename U, typename V>
    friend auto operator== (U const & lhs, V const & rhs)
    -> /* predicate_t */
    std::enable_if_t<    interface_t::is_parent_of_v<U>
                      && interface_t::is_parent_of_v<V>
                    , predicate_t>;

    template <typename U, typename V>
    friend auto operator!= (U const & lhs, V const & rhs)
    -> /* predicate_t */
    std::enable_if_t<    interface_t::is_parent_of_v<U>
                      && interface_t::is_parent_of_v<V>
                    , predicate_t>;

    template <typename U, typename V>
    friend auto operator&& (U const & lhs, V const & rhs)
    -> /* predicate_t */
    std::enable_if_t<    interface_t::is_parent_of_v<U>
                      && interface_t::is_parent_of_v<V>
                    , predicate_t>;

    template <typename U, typename V>
    friend auto operator|| (U const & lhs, V const & rhs)
    -> /* predicate_t */
    std::enable_if_t<    interface_t::is_parent_of_v<U>
                      && interface_t::is_parent_of_v<V>
                    , predicate_t>;

    template <typename U>
    friend auto operator!  (U const & rhs)
    -> /* predicate_t */
    std::enable_if_t<interface_t::is_parent_of_v<U>, predicate_t>;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Okay, we're actually done now.


private:
    // Heap-allocated, type-erased predicate function-object.
    std::shared_ptr<interface_t> m_predicate_fn;


public:
    /** Construct a generalized `predicate_t` from a callable (`U`), if that
     *  callable able to fulfil the correct interface.
     *
     *  NB. If `U` is convertible to `predicate_t`, we should use the (implicit)
     *  copy or move constructor rather than instantiating and constructing a
     *  new `predicate_passthrough_t`.
     */
    template <
        typename U,
        typename std::enable_if_t<    !std::is_base_of_v<interface_t, U>
                                   && !std::is_convertible_v<U, predicate_t>
                                   &&  std::is_invocable_r_v<bool, U, T const &>
                                 , int> = 0>
    predicate_t(U callable)
        : m_predicate_fn (
            std::make_shared<predicate_passthrough_t<U>>(std::move(callable))
        )
    { }

    /** Construct a predicate with the given `interface_t`-derived object. */
    //TODO: Consider if it's acceptable to require that `U` be move ctor'able.
    template <
        typename U,
        typename std::enable_if_t< std::is_base_of_v<interface_t, U>
                                 , int> = 1 >
    predicate_t(U interface)
        : m_predicate_fn ( std::make_shared<U>(std::move(interface)) )
    { }

    /** Compare the given object with this predicate's test. */
    bool operator() (T const & obj) const { return (*m_predicate_fn)(obj); }


private:
    /** Passthrough Predicate Function-Objects
     *  --------------------------------------
     *  The `predicate_passthrough_t` template is what it claims to be; a
     *  template for passthrough types that only exist to store and pass calls
     *  through to the callable provided as its template argument. Unless the
     *  callabe used to initialize the given `predicate_t` is derived from
     *  `interface_t`, this template will be instantiatied and stored in the
     *  `predicate_t`'s shared's pointer.
     *
     *  Attempting to intantiate this teamplate using an object that cannot be
     *  called using an `operator()` overload identical to `interface_t`'s will,
     *  correctly, result in a compilation error.
     *
     *  In terms of the concept-model idiom, this is the canonical model that
     *  will be instantiated for any callable that fulfils the concepts.
     */
    template <typename U>
    struct predicate_passthrough_t final : public interface_t {
        U m_test;
        predicate_passthrough_t(U x)
            : m_test ( std::move(x) )
        { }
        bool operator() (T const & obj) const override {
            return m_test(obj);
        }
    };

    /** Compositional Predicate Function-Objects
     *  ----------------------------------------
     *  Knowing that a primary usecase for predicates is to compose them with
     *  one another, we can build specialized function-objects for the expected
     *  compositions. All of the below classes store type-erased
     *  `shared_ptr<interface_t>`s, rather than complete `predicate_t` objects.
     *  This lets us skip allocations if a compose-ee is already on the heap.
     *
     *  NB. We take `predicate_t`s as arguments --- not `interface_t`s, or
     *  shared pointers to anything -- to guarantee that the pointed-to
     *  `interface_t`s have already been correctly heap-allocated by one of the
     *  `predicate_t` constructors.
     */

    /** Specialized Callable for `==`
     *  ------------------------------
     */
    struct equal_to_predicate_t final : public interface_t {
        std::shared_ptr<interface_t> m_lhs;
        std::shared_ptr<interface_t> m_rhs;

        equal_to_predicate_t(predicate_t const & lhs,
                             predicate_t const & rhs)
            : m_lhs ( lhs.m_predicate_fn )
            , m_rhs ( rhs.m_predicate_fn )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) == (*m_rhs)(obj);
        }
    };

    /** Specialized Callable for `!=`
     *  -----------------------------
     */
    struct not_equal_to_predicate_t final : public interface_t {
        std::shared_ptr<interface_t> m_lhs;
        std::shared_ptr<interface_t> m_rhs;

        not_equal_to_predicate_t(predicate_t const & lhs,
                                 predicate_t const & rhs)
            : m_lhs ( lhs.m_predicate_fn )
            , m_rhs ( rhs.m_predicate_fn )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) != (*m_rhs)(obj);
        }
    };

    /** Specialized Callable for `&&`
     *  -----------------------------
     */
    struct logical_and_predicate_t final : public interface_t {
        std::shared_ptr<interface_t> m_lhs;
        std::shared_ptr<interface_t> m_rhs;

        logical_and_predicate_t(predicate_t const & lhs,
                                predicate_t const & rhs)
            : m_lhs ( lhs.m_predicate_fn )
            , m_rhs ( rhs.m_predicate_fn )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) && (*m_rhs)(obj);
        }
    };

    /** Specialized Callable for `||`
     *  -----------------------------
     */
    struct logical_or_predicate_t final : public interface_t {
        std::shared_ptr<interface_t> m_lhs;
        std::shared_ptr<interface_t> m_rhs;

        logical_or_predicate_t(predicate_t const & lhs,
                               predicate_t const & rhs)
            : m_lhs ( lhs.m_predicate_fn )
            , m_rhs ( rhs.m_predicate_fn )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) || (*m_rhs)(obj);
        }
    };

    /** Specialized Callable for `!`
     *  ----------------------------
     */
    struct logical_not_predicate_t final : public interface_t {
        std::shared_ptr<interface_t> m_rhs;

        logical_not_predicate_t(predicate_t const & rhs)
            : m_rhs ( rhs.m_predicate_fn )
        { }
        bool operator() (T const & obj) const override {
            return !(*m_rhs)(obj);
        }
    };


public:
    /** Boolean-Logic Free-Function Operator Overloads
     *  ==============================================
     *  Friend, free-function, operator overloads to allow for composition via
     *  logical operators. Each of these will take two `predicate_t`s by
     *  reference, create copies or claim references to the salient state as
     *  necessary, and return a new `predicate_t`.
     *
     *  It sure would be nice to declare thse above the `provate:` section
     *  above, and define them out-of-line, buuuut c'est la C.
     */
    friend
    predicate_t operator== (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t { equal_to_predicate_t { lhs, rhs } };
    }
    friend
    predicate_t operator!= (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t { not_equal_to_predicate_t { lhs, rhs } };
    }
    friend
    predicate_t operator&& (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t { logical_and_predicate_t { lhs, rhs } };
    }
    friend
    predicate_t operator|| (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t { logical_or_predicate_t { lhs, rhs } };
    }
    friend predicate_t operator!  (predicate_t const & rhs) {
        return predicate_t { logical_not_predicate_t { rhs } };
    }


    /** Boolean-Logic Free-Function Operator Overload Templates
     *  =======================================================
     *  Friend, free-function, operator overload templates to allow for
     *  composition between `predicate_t`s and any instance derived from
     *  `interface_t`.
     *
     *  NB. These functions are written for brevity, and so rely on a number of
     *  implicit conversions. Fully expanded, and sans SFINAE, the first of
     *  these would look like,
     *
     *  template <typename U>
     *  friend predicate_t operator== (predicate_t const & lhs, U const & rhs) {
     *      return predicate_t {
     *          equal_to_predicate_t {
     *              lhs,
     *              predicate_t { rhs }
     *          }
     *      };
     *  }
     */
    template <typename U>
    friend auto operator== (predicate_t const & lhs, U const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t> {
        return equal_to_predicate_t { lhs, rhs };
    }
    template <typename U>
    friend auto operator== (U const & lhs, predicate_t const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t>
    {
        return equal_to_predicate_t { lhs, rhs };
    }

    template <typename U>
    friend auto operator!= (predicate_t const & lhs, U const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t>
    {
        return not_equal_to_predicate_t { lhs, rhs };
    }
    template <typename U>
    friend auto operator!= (U const & lhs, predicate_t const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t>
    {
        return not_equal_to_predicate_t { lhs, rhs };
    }

    template <typename U>
    friend auto operator&& (predicate_t const & lhs, U const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t>
    {
        return logical_and_predicate_t { lhs, rhs };
    }
    template <typename U>
    friend auto operator&& (U const & lhs, predicate_t const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t>
    {
        return logical_and_predicate_t { lhs, rhs };
    }

    template <typename U>
    friend auto operator|| (predicate_t const & lhs, U const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t>
    {
        return logical_or_predicate_t { lhs, rhs };
    }
    template <typename U>
    friend auto operator|| (U const & lhs, predicate_t const & rhs)
    -> /* predicate_t */
    std::enable_if_t<std::is_base_of_v<interface_t, U>, predicate_t>
    {
        return logical_or_predicate_t { lhs, rhs };
    }
};

} /* namespace nonstd */
