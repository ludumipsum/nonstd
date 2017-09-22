/** Iteration Tools
 *  ===============
 *  C++11/14 added great automatic deduction and iteration facilities, but it's
 *  often missing little conveniences to make the task of using them as simple
 *  as it could be.
 */

#pragma once

#include <iterator>

#include "nonstd/core/primitive_types.h"


/** Range
 *  =====
 *  Lazily yield Ts in the range provided. Emulates python 3's range()
 */
template <typename T>
class Range {
public:
    /* named start / stop to avoid colliding with begin() / end() */
    T start, stop, step;

    constexpr inline Range(T begin, T end, T step=1)
        : start ( begin )
        , stop  ( end   )
        , step  ( step  ) { }
    constexpr inline Range(T end)
        : start ( 0   )
        , stop  ( end )
        , step  ( 1   ) { }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Range const& r;
        T value;

        constexpr inline iterator(Range const& r, T value)
            : r     ( r     )
            , value ( value ) { }
        constexpr inline iterator(iterator const& i)
            : r     ( i.r     )
            , value ( i.value ) { }
        constexpr inline iterator& operator=(iterator const& i) {
            if (this != &i) {
                this->r = i.r;
                this->value = i.value;
            }
        }

        constexpr inline T operator*() { return value; }

        constexpr inline bool operator==(iterator const& i) {
            return value == i.value;
        }
        constexpr inline bool operator!=(iterator const& i) {
            return value != i.value;
        }

        constexpr inline iterator& operator++() {
            value += r.step;
            return *this;
        }
        constexpr inline iterator operator++(int) {
            iterator ret(r, value);
            value += r.step;
            return ret;
        }

        constexpr inline iterator& operator--() {
            value -= r.step;
            return *this;
        }
        constexpr inline iterator operator--(int) {
            iterator ret(r, value);
            value -= r.step;
            return ret;
        }

        constexpr inline iterator& operator+=(T steps) {
            value += (steps * r.step);
            return *this;
        }
        constexpr inline iterator& operator-=(T steps) {
            value -= (steps * r.step);
            return *this;
        }
    };

    constexpr inline iterator begin() const {
        return Range<T>::iterator(*this, start);
    }
    constexpr inline iterator end() const {
        return Range<T>::iterator(*this, stop);
    }
};

template<typename T>
constexpr inline Range<T> range(T begin, T end, T step = 1) {
    return Range<T>(begin, end, step);
}

template<typename T>
inline constexpr Range<T> range(T end) {
    return Range<T>(0, end, 1);
}


/** Slice
 *  =====
 *  Lazily iterate over `count` elements from the typed pointer `data`.
 */
template<typename T>
class Slice {
public:
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

    T& operator[](u64 index) {
        return *( start + (index * stride) );
    }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

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

    constexpr inline iterator begin() const {
        return Slice<T>::iterator(*this, 0);
    }
    constexpr inline iterator end()   const {
        return Slice<T>::iterator(*this, stop - start);
    }
};

/* Create a lazy iterator over `count` elements from the typed pointer `data`. */
template<typename T> inline
Slice<T> slice(T* data, u64 count, u64 stride = 1) {
    return Slice<T>(data, count, stride);
}
