/* Iteration Tools
   ===============

   C++11/14 added great automatic deduction and iteration facilities, but it's
   often missing little conveniences to make the task of using them as simple
   as it could be.
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"


/* Lazily yield Ts in the range provided. Emulates python 3's range() */
template <typename T>
class Range {
public:
    /* named start / stop to avoid colliding with begin() / end() */
    T start, stop, step;

    inline Range(T begin, T end, T step=1)
        : start ( begin )
        , stop  ( end   )
        , step  ( step  ) { }
    inline Range(T end)
        : start ( 0   )
        , stop  ( end )
        , step  ( 1   ) { }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Range const& r;
        T value;

        inline iterator(Range const& r, T value)
            : r     ( r     )
            , value ( value ) { }
        inline iterator(iterator const& i)
            : r     ( i.r     )
            , value ( i.value ) { }
        inline iterator& operator=(iterator const& i) {
            if (this != &i) {
                this->r = i.r;
                this->value = i.value;
            }
        }

        inline T operator*() { return value; }

        inline bool operator==(iterator const& i) { return value == i.value; }
        inline bool operator!=(iterator const& i) { return value != i.value; }

        inline iterator& operator++() {
            value += r.step;
            return *this;
        }
        inline iterator operator++(int) {
            iterator ret(r, value);
            value += r.step;
            return ret;
        }

        inline iterator& operator--() {
            value -= r.step;
            return *this;
        }
        inline iterator operator--(int) {
            iterator ret(r, value);
            value -= r.step;
            return ret;
        }

        inline iterator& operator+=(T steps) {
            value += (steps * r.step);
            return *this;
        }
        inline iterator& operator-=(T steps) {
            value -= (steps * r.step);
            return *this;
        }
    };

    inline iterator begin() const { return Range<T>::iterator(*this, start); }
    inline iterator end() const { return Range<T>::iterator(*this, stop); }
};

/* Lazily yield Ts in the range provided. Emulates python 3's range() */
template<typename T>
Range<T> range(T begin, T end, T step = 1) {
    return Range<T>(begin, end, step);
}
/* Lazily yield Ts in the range provided. Emulates python 3's range() */
template<typename T>
Range<T> range(T end) { return Range<T>(0, end, 1); }



/* Lazily iterate over `count` elements from the typed pointer `data` . */
template<typename T>
class Slice {
public:
    T*  start;
    T*  stop;
    u64 stride;

    Slice(T* data, u64 count, u64 stride = 1)
        : start  ( data       )
        , stop   ( data+count )
        , stride ( stride     ) { }
    Slice(void* data, u64 count, u64 stride = 1)
        : start  ( (T*)data           )
        , stop   ( ((T*)data) + count )
        , stride ( stride             ) { }


    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Slice const& s;
        T* cursor;

        inline iterator(Slice const& s, u64 offset = 0)
            : s      ( s                )
            , cursor ( s.start + offset ) { }
        inline iterator(iterator const& i)
            : s      ( i.s      )
            , cursor ( i.cursor ) { }
        inline iterator& operator=(iterator const& i) {
            if (this != &i) {
                this->s      = i.s;
                this->cursor = i.cursor;
            }
        }

        inline T& operator*() { return *cursor; }

        inline bool operator==(iterator const& i) { return cursor == i.cursor; }
        inline bool operator!=(iterator const& i) { return cursor != i.cursor; }

        inline iterator& operator++() {
            cursor += s.step;
            return *this;
        }
        inline iterator operator++(int) {
            iterator ret(*this);
            cursor += s.step;
            return ret;
        }

        inline iterator& operator--() {
            cursor -= s.step;
            return *this;
        }
        inline iterator operator--(int) {
            iterator ret(*this);
            cursor -= s.step;
            return ret;
        }

        inline iterator& operator+=(u64 steps) {
            cursor += (steps * s.step);
            return *this;
        }
        inline iterator& operator-=(u64 steps) {
            cursor -= (steps * s.step);
            return *this;
        }
    };

    inline iterator begin() const { return Slice<T>::iterator(*this, start); }
    inline iterator end() const   { return Slice<T>::iterator(*this, stop); }
};

/* Create a lazy iterator over `count` elements from the typed pointer `data`. */
template<typename T>
Slice<T> slice(T* data, u64 count, u64 stride = 1) {
    return Slice<T>(data, count, stride);
}
