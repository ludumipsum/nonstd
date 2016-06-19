#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "mathutils.h"

#include "buffer.h"
#include "buffer_view.h"

/* Typed Rings Over Buffers
   ========================

   BufferRings are a variant of BufferViews which expose a ring buffer
   as a view over a BufferDescriptor. Imagine that.

   Like all Buffer* abstractions, these are meant to be lightweight, created
   on demand
*/

template<typename T>
class BufferRing : protected BufferView<T> {
public:
    BufferRing(BufferDescriptor *const bd) : BufferView<T>(bd) { }
    BufferRing(GameState& state, c_cstr name) : BufferView<T>(state, name) { }

    inline T& push(T value) {
        if (this->m_bd == nullptr) return T;
        u64* frameno = (u64*)&value;
        f64* data = (f64*)(frameno + 1);
        auto buffer_start   = (u8*)this->m_bd->data,
             buffer_end     = (u8*)this->m_bd->cursor;
        u64  currently_used = buffer_end - buffer_start;
        u64  currently_free = this->m_bd->size - currently_used;
        if (currently_free < sizeof(T)) {
            this->m_bd->cursor = this->m_bd->data;
        }
        return BufferView<T>::push(value);
    }

    inline void drop()  { return BufferView<T>::drop();   }
    inline u64 size()   { return BufferView<T>::size();   }
    inline u64 length() { return BufferView<T>::length(); }

    /* Iterator respecting extents modulo ring size */
    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        BufferRing &ring;
        ptrdiff     offset;

        iterator(BufferRing& ring, u64 offset=0)
            : ring(ring)
            , offset(offset) { }

        inline bool operator==(const iterator& other) const {
            return &ring  == &other.ring
                && offset ==  other.offset;
        }
        inline bool operator!=(const iterator& other) const {
            return ! (*this == other);
        }

        // Pre-increment — step forward and return this
        inline iterator& operator++() {
            offset = (offset + 1) % ring.length();
            return *this;
        }
        // Post-increment — return a copy created before stepping forward
        inline iterator operator++(int) {
            iterator copy = *this;
            offset = (offset + 1) % ring.length();
            return copy;
        }
        template<typename U>
        inline iterator& operator+=(U n) {
            offset = (offset + n) % ring.length();
            return *this;
        }
        template<typename U>
        inline iterator operator+(U n) {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        inline T& operator*() const { return ring[offset]; }
    };


    /* When we've rolled through the entire loop and wound up at the current
       cursor location, we're done cycling through the ring */
    inline iterator end() {
        if (this->m_bd == nullptr) { return iterator(*this, 0); }
        ptrdiff offset = (T*)this->m_bd->cursor - (T*)this->m_bd->data;
        if (offset >= length()) {
            offset = 0;
        }
        return iterator(*this, offset);
    }
    /* Grab the start of the iteration range, respecting the modulus at the
       end of the ring */
    inline iterator begin() {
        if (this->m_bd == nullptr) { return end(); }
        T* iter_begin = (T*)this->m_bd->cursor + 1;
        T const* wrap_at = (T*)this->m_bd->data + length();
        if (iter_begin >= wrap_at) {
            iter_begin = (T*)this->m_bd->data;
        }
        /* Build an iterator object from the computed start point */
        ptrdiff offset = iter_begin - (T*)this->m_bd->data;
        return iterator(*this, offset);
    }
};
