/** Slice
 *  =====
 *  C++11/14 added great automatic deduction and iteration facilities, but it's
 *  often missing little conveniences to make the task of using them as simple
 *  as it could be. This utility will lazily iterate over `count` elements from
 *  the typed pointer given.
 */

#pragma once

#include <iterator>
#include "primitive_types.h"


namespace nonstd {

// Forward declaration
template <typename T> struct Slice;

/** `nonstd::slice` Free Functions
 *   =============================
 */
template<typename T> inline
Slice<T> slice(T* data, u64 count, u64 stride = 1) {
    return Slice<T>(data, count, stride);
}

template<typename T>
struct Slice {
    struct iterator;

    T*  start;
    T*  stop;
    u64 stride;

    constexpr inline Slice(T* data, u64 count, u64 stride = 1)
        : start  ( data       )
        , stop   ( data+count )
        , stride ( stride     ) { }
    constexpr inline Slice(void* data, u64 count, u64 stride = 1)
        : start  ( (T*)data           )
        , stop   ( ((T*)data) + count )
        , stride ( stride             ) { }

    constexpr inline iterator begin() const {
        return { *this, 0 };
    }
    constexpr inline iterator end()   const {
        return { *this, stop - start };
    }

    T& operator[](u64 index) {
        return *(start + (index * stride));
    }

    struct iterator {
        /* Iterator type trait boilerplate. See,
         * http://en.cppreference.com/w/cpp/iterator/iterator_traits
         * http://www.cplusplus.com/reference/iterator/
         */
        using difference_type   = ptrdiff;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using iterator_category = std::output_iterator_tag;

        Slice const& s;
        T* cursor;

        constexpr inline iterator(Slice const& s, u64 offset = 0)
            : s      ( s                )
            , cursor ( s.start + offset ) { }
        constexpr inline iterator(iterator const& i)
            : s      ( i.s      )
            , cursor ( i.cursor ) { }
        constexpr inline iterator& operator=(iterator const& i) {
            if (this != &i) {
                this->s      = i.s;
                this->cursor = i.cursor;
            }
        }

        constexpr inline T& operator*() { return *cursor; }

        constexpr inline bool operator==(iterator const& i) {
            return cursor == i.cursor;
        }
        constexpr inline bool operator!=(iterator const& i) {
            return cursor != i.cursor;
        }

        constexpr inline iterator& operator++() {
            cursor += s.stride;
            return *this;
        }
        constexpr inline iterator operator++(int) {
            iterator ret(*this);
            cursor += s.stride;
            return ret;
        }

        constexpr inline iterator& operator--() {
            cursor -= s.stride;
            return *this;
        }
        constexpr inline iterator operator--(int) {
            iterator ret(*this);
            cursor -= s.stride;
            return ret;
        }

        constexpr inline iterator& operator+=(u64 steps) {
            cursor += (steps * s.stride);
            return *this;
        }
        constexpr inline iterator& operator-=(u64 steps) {
            cursor -= (steps * s.stride);
            return *this;
        }
    };
};

} /* namespace nonstd */
