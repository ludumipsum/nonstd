/** Port-over of Python's enumerate()
 *  =================================
 *  This file defines a type which proxies an iterable thing, yielding a count
 *  of the number of things that it's given you so far along with each element.
 *  This is mostly useful for getting the 'i' back in a range based iteration --
 *  e.g. enumerate("foo") would yield:
 *
 *      (0, 'f'), (1, 'o'), (2, 'o')
 */
#pragma once

#include <thirdparty/fmt.h>
#include <system_error>
#include "primitive_types.h"

#include <functional>
#include <type_traits>

namespace nonstd {

namespace detail {
using std::iterator_traits;

template <typename T>
struct is_reference_wrapper : std::false_type {};

template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <typename Container, typename Iterator, typename Value>
struct enumerate_t {
    template <typename U>
    enumerate_t(U&& u) : container(std::forward<U>(u)) {}

    struct iterator {
        using value_type = typename iterator_traits<Iterator>::value_type;

        struct item {
            std::size_t i;
            Value value;
        };

        iterator(Iterator iter, std::size_t index) :
                iter(iter), index(index) {}

        item operator*() const { return item{index, *iter}; }

        bool operator!=(iterator const& other) const {
            return iter != other.iter;
        }

        iterator& operator++() {
            ++index, ++iter;

            return *this;
        }

    private:
        Iterator    iter;
        std::size_t index;
    };

    iterator begin() { return iterator(std::begin(container), 0); }
    iterator end() { return iterator(std::end(container), std::size(container)); }

private:
    Container container;
};
}

template <typename R>
auto enumerate(R&& r) {
    using namespace detail;
    if constexpr (is_reference_wrapper<std::decay_t<R>>::value) {
        using type = decltype(r.get());
        using iterator = decltype(std::begin(r.get()));
        using value_type = decltype(*std::begin(r.get()));

        return enumerate_t<type, iterator, value_type&>(r);
    }
    else if constexpr (std::is_rvalue_reference_v<R>) {
        using type = std::remove_reference_t<R>;
        using iterator = decltype(std::begin(r));
        using value_type = std::remove_reference_t<decltype(*std::begin(r))>;

        return enumerate_t<type, iterator, value_type>(std::move(r));
    }
    else {
        using type = R;
        using iterator = decltype(std::begin(r));
        using value_type = std::remove_reference_t<decltype(*std::begin(r))>;

        return enumerate_t<type, iterator, value_type&>(r);
    }
}

} /* namespace nonstd */
