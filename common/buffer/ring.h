/* Typed Rings Over Buffers
 * ========================
 *
 * Buffer Rings are a variant of Buffer Slices, which expose a ring buffer
 * as a view over a Descriptor. Imagine that.
 *
 * This Ring is being written s.t. resizes never automatically occur; writes
 * past the object `count()` will cause the oldest object in the ring to
 * be overwritten. This also means we need some way of storing the current Head
 * and Tail between frames (read: outside of the Ring view). Currently, I'm
 * subverting the buffer Descriptor's cursor for this task. See below for
 * ASCII-art visual aids.
 *
 * Like all Buffer views, these are meant to be lightweight, created  on demand.
 *
 *
 *
 * |.data                             |(.data + .size)
 * |----------------------------------|
 * |Tail{0}                    Head{N}|.cursor{= 0}
 *
 *
 * |.data                             |(.data + .size)
 * |Head{0}                           |
 * ||---------------------------------|=|
 * =|Tail{1}                            |.cursor{= sizeof(T)*1}
 *
 *
 * |.data                   Head{N-1}||(.data + .size)
 * |---------------------------------||===================================|
 * ===================================|Tail{N}       .cursor{=sizeof(T)*N}|
 *
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "mathutils.h"
#include "logging.h"

#include "buffer.h"
#include "buffer/slice.h"


namespace buffer {

template<typename T>
class Ring : protected Slice<T> {
public:
    inline static u64 precomputeSize() {
        return Slice<T>::precomputeSize();
    }

protected:
    /* Indexes of the write-head and read-tail. */
    u64 m_head;
    u64 m_tail;


    /* Helper function to correctly increment Ring indexes. */
    inline u64 increment(u64 index, u64 n = 1) {
        // TODO: Potential divide-by-zero error
        return ((index + n) % count());
    }
    /* Helper function to correctly decrement Ring indexes */
    inline u64 decrement(u64 index, u64 n = 1) {
        if (index >= n) { return (index - n); }
        u64 past_zero = n - index;
        return (count() - past_zero);
    }

public:
    Ring(Descriptor *const bd,
         BufferResizeFn resize = nullptr)
        : Slice<T>(bd, resize)
        , m_head ( 0 )
        , m_tail ( 0 )
    {
        ptr buffer_end = this->m_bd->data + this->m_bd->size;
        if (this->m_bd->cursor > buffer_end) {
            // We're in a full ring.
            ptrdiff tail_offset = this->m_bd->cursor - buffer_end;
            m_tail = tail_offset / sizeof(T);
            m_head = decrement(m_tail);
        } else {
            // The ring isn't full, and we're essentially in a Slice.
            ptrdiff head_offset = this->m_bd->cursor - this->m_bd->data;
            m_tail = 0;
            m_head = head_offset / sizeof(T);
        }
    }

    inline T& push(T& value) {
        /* If the ring is full, overwrite the element at `m_tail`. */
        if (count() == Slice<T>::max_count()) {
            this[m_tail] = value;
            /* Increment `m_head` and `m_tail` by one (wrapping when necessary)
             * and set the Descriptor's cursor past the end of the buffer based
             * on the calculated value of `m_tail`. */
            increment(m_head);
            increment(m_tail);
            this->m_bd->cursor = this->m_bd->data +
                                 this->m_bd->size +
                                 (sizeof(T) * m_tail);
        } else {
            return Slice<T>::push(value);
            m_head++;
        }
    }

    /* Return the number of objects currently stored in the Ring. */
    inline u64 count() {
        ptr buffer_end = this->m_bd->data + this->m_bd->size;
        // Ring is full, count == max_count
        if (this->m_bd->cursor > buffer_end) { return Slice<T>::max_count(); }
        return Slice<T>::count();
    }

    /* Iterator respecting extents modulo ring size */
    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Ring& ring;
        u64   index;

        iterator(Ring& ring,
                 u64 index=0)
            : ring  ( ring   )
            , index ( index ) { }

        inline bool operator==(const iterator& other) const {
            return &ring == &other.ring && index == other.index;
        }
        inline bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        // Pre-increment -- step forward and return `this`.
        inline iterator& operator++() {
            index = ring.increment(index);
            return *this;
        }
        // Post-increment -- return a copy created before stepping forward.
        inline iterator operator++(int) {
            iterator copy = *this;
            index = ring.increment(index);
            return copy;
        }
        // Increment and assign -- step forward by `n` and return `this`.
        inline iterator& operator+=(u64 n) {
            index = (index + n) % ring.count();
            return *this;
        }
        // Arithmetic increment -- return an incremented copy.
        inline iterator operator+(u64 n) {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        // Dereference -- return the current value.
        inline T& operator*() const { return ring[index]; }
    };


    /* The first element the iterator should touch is at `m_tail`. */
    inline iterator begin() { return iterator(*this, m_tail); }
    /* The last element the iterator should touch is at `m_head`. */
    inline iterator end()   { return iterator(*this, m_head); }
};

} /* namespace buffer */
