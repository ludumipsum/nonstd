/** Iteration Tool Extensions
 *  =========================
 *  C++11/14 added great automatic deduction and iteration facilities, but it's
 *  often missing little conveniences to make the task of using them as simple
 *  as it could be.
 */

#pragma once

#include <iterator>
#include <nonstd/nonstd.h>


namespace nonstd {


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

} /* namespace nonstd */
