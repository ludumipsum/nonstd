/* Typed Rings Buffer
 * ==================

 * Buffer Rings are a View over Buffer Descriptors that present a simple typed
 * ring-buffer over an entire buffer descriptor. These buffers have no concept
 * of partial-fullness, so iterations over a Ring will always yield `capacity()`
 * objects. The buffer's data is assumed to be initialized to `\0`, so "empty"
 * data should be an expected and valid return for all iterations and subscript
 * operations.
 *
 * The write head of the ring buffer will be stored directly in the buffer
 * descriptor as the `.cursor`, and will point to the last object written. For
 * writes, the write_head will be incremented, then the write will be performed.
 * For iterations and for the subscripting, the first object returned will
 * always be one index past the write_head.
 *
 * Resizes are permitted (assuming a resize callback has been provided), though
 * will never occur automatically.
 * TODO: Define resize semantics. For upward resizes, will we shift memory s.t.
 *       the original object ordering is retained? For downward resizes, will we
 *       do anything to maintain object ordering, or just `drop()` then resize?
 * TODO: Figure out consume. Does it mean anything in this context? If not, are
 *       user's going to be limited to adding objects one at a time? If so, how
 *       do we deal with split memory regions? (Scratch buffer, maybe? Unless
 *       the scratch is a ring....)
 * TODO: Better comments.
 * TODO: Tests.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "logging.h"

#include "buffer.h"
#include "buffer/slice.h"


namespace buffer {

template<typename T>
class Ring {
public:
    inline static u64 precomputeSize() {
        return Slice<T>::precomputeSize();
    }

protected:
    Descriptor * const m_bd;
    BufferResizeFn     m_resize;
    u64                m_capacity;
    u64                m_write_head;

public:
    Ring(Descriptor *const bd,
         BufferResizeFn resize = nullptr)
        : m_bd         ( bd                   )
        , m_resize     ( resize               )
        , m_capacity   ( bd->size / sizeof(T) )
        , m_write_head ( (bd->cursor - bd->data) / sizeof(T) )
    { }

    inline u64 resize(u64 capacity) {
        LOG("This function is currently unimplemented.");
        BREAKPOINT();
    }

    inline void drop() {
        memset(m_bd->data, '\0', m_bd->size);
        m_bd->cursor = m_bd->data;
    }

    inline u64 size()     { return m_bd->size; }
    inline u64 count()    { return m_capacity; }
    inline u64 capacity() { return m_capacity; }


    inline T* consume(u64 count) {
        LOG("This function is currently unimplemented.");
        BREAKPOINT();
    }

    inline T& operator[](u64 index) {
#if defined(DEBUG)
        // TODO: Better logging
        if (index >= capacity()) {
            LOG("buffer::Ring -- index out of bounds. %d / %d in %s."
                index, capacity(), m_bd->name);
            BREAKPOINT();
        }
#endif
        u64 target_index = increment(m_write_head, (1 + index));
        return *((T*)(m_bd->data) + target_index);
    }

    inline T& push(T& value) {
        m_write_head = increment(m_write_head);
        T* mem       = (T*)(m_bd->data) + m_write_head;
        *mem         = value;
        m_bd->cursor = (ptr)mem;
        return *mem;
    }


    class iterator;
    inline iterator begin() { return iterator(*this, increment(m_write_head)); }
    inline iterator end()   { return iterator(*this, m_write_head);  }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Ring& ring;
        u64   index;

        iterator(Ring& ring,
                 u64 index=0)
            : ring  ( ring  )
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
            index = ring.increment(index, n);
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


protected:
    inline u64 increment(u64 index, u64 n = 1) {
        // TODO: Potential divide-by-zero error
        return ((index + n) % capacity());
    }

    inline u64 decrement(u64 index, u64 n = 1) {
        if (index >= n) { return (index - n); }
        u64 past_zero = n - index;
        return (capacity() - past_zero);
    }
};

} /* namespace buffer */
