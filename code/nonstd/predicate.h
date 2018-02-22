/** Compoasable Predicate Objects
 *  =============================
 *  This file defines a type that implements the Predicate Concept from the C++
 *  standard library in a form that allows for clean and clear composition into
 *  boolean logic trees. Predicate object can be created by wrapping an instance
 *  of an explicitly defined function object, a free function, or a lambda.
 *
 *  Any tree of boolean expressions can be expressed using this system.
 *  For example,
 *
 *  ```
 *  bool A(X) {...}
 *  bool C(X) {...}
 *  bool B(X) {...}
 *  X x;
 *  auto ret = (A(x) && (B(x) || !C(x)));
 *  ```
 *
 *  Can be encoded as,
 *
 *  ```
 *  predicate_t<X> A {...};
 *  predicate_t<X> B {...};
 *  predicate_t<X> C {...};
 *  predicate test = A && (B || !C);
 *  X x;
 *  auto ret = test(x);
 *  ````
 *
 *  The later requires template specializations, an additional variable
 *  definition, and more lines of code. This should tell you that this tool is
 *  not a replacement for boolean expressions. It's an example of C++ solutions
 *  making simple tasks somewhat difficult so that it can make complex tasks
 *  only somewhat difficult.
 *
 *  The two interesting interactions this system allows for are 1) decoupling
 *  the composition of the logical test from the object being tested -- the
 *  `test` object in the above example is designed to be passed into functions
 *  and stored in std:: containers -- and 2) arbitrarily deep composition --
 *  the `test` object can be negated, OR'd against another `predicate_t<int>`,
 *  and negated again to build a new predicate object.
 */

#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/utility_ext.h>
#include <functional>
#include <iostream>


namespace nonstd {

/** Logical Predicate Object
 *  ========================
 *  The `predicate_t` type is implemented with the [Concept-Model] idiom, so
 *  users will be able to pass any type that fulfils the concept's interface;
 *  specifically, the conceptual C++ [Predicate].
 *
 *  [Concept-Model]: https://www.youtube.com/watch?v=QGcVXgEVMJg
 *  [Predicate]: http://en.cppreference.com/w/cpp/concept/Predicate
 */
template <typename T>
class predicate_t {
private:
    /** The Concept
     *  -----------
     * Callable with a `T`, returning a `bool`. To be inherited by concrete
     * `*_model_t` type.
     */
    struct concept_t {
        virtual ~concept_t() = default;
        virtual bool operator() (T const & obj) const = 0;
    };

    /** The predicate's (heap-allocated) model. */
    std::shared_ptr<concept_t> m_model;


public:
    /** Construct a generalized predicate with the give `U`. The program will
     *  fail to compile if `U` doesn't provide the correct interface.
     */
    template <typename U>
    predicate_t(U tester)
        : m_model ( std::make_shared<general_model_t<U>>(std::move(tester)) )
    { }

    /** Compare this predicate with the given object; passes through to the
     *  wrapped model.
     */
    bool operator() (T const & obj) const { return (*m_model)(obj); }

private:
    /** General Model
     *  -------------
     *  As this class uses the concept-model idiom, we need to provide it with
     *  one or more models that implement the given concept. In the general
     *  case, we will simply wrap the given object, function, or lambda in a new
     *  template-generated class, and pass `operator()` calls through to the
     *  wrapped object.
     */
    template <typename U>
    struct general_model_t final : public concept_t {
        U m_test;
        general_model_t(U x)
            : m_test ( std::move(x) )
        { }
        bool operator() (T const & obj) const override {
            return m_test(obj);
        }
    };

    /** Specialized Models
     *  ------------------
     *  Because we know instances of this class are going to be composed using
     *  logical operations, we can skip a bit (potentially, a _lot_) of template
     *  specialization and at least one level of indirection per composition by
     *  building a specialized model for each of the logical operations we want
     *  to support.
     *
     *  We don't want to expose this system to the user outside of the operator
     *  overloads, and we don't want to mess with the interface of the public
     *  construction. To differentiate between predicates using these
     *  specializations and the generic `<typename U>` initialization, we use
     *  tag-dispatch in the specialized constructor.
     */

    /** Specialized Predicate Tag Type
     */
    struct specialized_model_t {
        // For all tag, accidental construction to be impossible.
        specialized_model_t() = delete;
        // Use nonstd::in_place for explicit construction.
        constexpr explicit
        specialized_model_t(nonstd::in_place_t /* unused */) noexcept { }
    };
    /** Specialized Predicate Tag
     */
    static constexpr specialized_model_t specialized_model { nonstd::in_place };


    /** Initialize a specialized predicate using a pre-constructed model. */
    predicate_t(specialized_model_t /* unused */,
                std::shared_ptr<concept_t> model)
        : m_model ( std::move(model) )
    { }


    /** Specialized Model for `==`
     *  --------------------------
     *  Note that this specialization -- and all of those following -- store
     *  `shared_ptr<concept_t>`s as their members. This lets us re-use the
     *  models created by the composed `predicate_t` instances, without
     *  additional local- or heap-allocations.
     */
    struct equal_to_model_t final : public concept_t {
        std::shared_ptr<concept_t> m_lhs;
        std::shared_ptr<concept_t> m_rhs;

        equal_to_model_t(predicate_t const & lhs,
                         predicate_t const & rhs)
            : m_lhs ( lhs.m_model )
            , m_rhs ( rhs.m_model )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) == (*m_rhs)(obj);
        }
    };

    /** Specialized Model for `!=` */
    struct not_equal_to_model_t final : public concept_t {
        std::shared_ptr<concept_t> m_lhs;
        std::shared_ptr<concept_t> m_rhs;

        not_equal_to_model_t(predicate_t const & lhs, predicate_t const & rhs)
            : m_lhs ( lhs.m_model )
            , m_rhs ( rhs.m_model )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) != (*m_rhs)(obj);
        }
    };

    /** Specialized Model for `&&` */
    struct logical_and_model_t final : public concept_t {
        std::shared_ptr<concept_t> m_lhs;
        std::shared_ptr<concept_t> m_rhs;

        logical_and_model_t(predicate_t const & lhs, predicate_t const & rhs)
            : m_lhs ( lhs.m_model )
            , m_rhs ( rhs.m_model )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) && (*m_rhs)(obj);
        }
    };

    /** Specialized Model for `||` */
    struct logical_or_model_t final : public concept_t {
        std::shared_ptr<concept_t> m_lhs;
        std::shared_ptr<concept_t> m_rhs;

        logical_or_model_t(predicate_t const & lhs, predicate_t const & rhs)
            : m_lhs ( lhs.m_model )
            , m_rhs ( rhs.m_model )
        { }
        bool operator() (T const & obj) const override {
            return (*m_lhs)(obj) || (*m_rhs)(obj);
        }
    };

    /** Specialized Model for `!` */
    struct logical_not_model_t final : public concept_t {
        std::shared_ptr<concept_t> m_rhs;

        logical_not_model_t(predicate_t const & rhs)
            : m_rhs ( rhs.m_model )
        { }
        bool operator() (T const & obj) const override {
            return !(*m_rhs)(obj);
        }
    };


public:
    /** Operator Overloads
     *  ==================
     *  Friend-free functions to allow for ease of use / compiler discovery.
     *
     *  It sure would be nice to define or even declare these above the private
     *  section above, buuuut c'est la C.
     */
    friend
    predicate_t operator == (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t {
            specialized_model,
            std::make_shared<equal_to_model_t>(lhs, rhs)
        };
    }
    friend
    predicate_t operator != (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t {
            specialized_model,
            std::make_shared<not_equal_to_model_t>(lhs, rhs)
        };
    }
    friend
    predicate_t operator && (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t {
            specialized_model,
            std::make_shared<logical_and_model_t>(lhs, rhs)
        };
    }
    friend
    predicate_t operator || (predicate_t const & lhs, predicate_t const & rhs) {
        return predicate_t {
            specialized_model,
            std::make_shared<logical_or_model_t>(lhs, rhs)
        };
    }
    friend
    predicate_t operator !  (predicate_t const & rhs) {
        return predicate_t {
            specialized_model,
            std::make_shared<logical_not_model_t>(rhs)
        };
    }

};

} /* namespace nonstd */
