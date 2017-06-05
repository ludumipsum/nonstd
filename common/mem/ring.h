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
#include "crash.h"

#include "mem/buffer.h"


namespace mem {
namespace view {

/* NB. This can only handle 63 bits of addressing, so if you have more than
 *     9.2 Exabytes of data in one Ring, rethink life.
 */
template<typename T>
class Ring {
public: /*< ## Class Methods */
    static const u64 default_capacity = 64;

    inline static u64 precomputeSize(u64 capacity = default_capacity) {
        return sizeof(T) * capacity;
    }


protected: /*< ## Public Member Variables */
    Buffer *const m_bd;
    ResizeFn      m_resize;
    u64           m_capacity;
    u64           m_write_head;

public: /*< ## Ctors, Detors, and Assignments */
    Ring(Buffer *const bd,
         ResizeFn resize = nullptr)
        : m_bd         ( bd                   )
        , m_resize     ( resize               )
        , m_capacity   ( bd->size / sizeof(T) )
        , m_write_head ( (bd->cursor - bd->data) / sizeof(T) ) { }


public: /*< ## Public Memeber Methods */
    inline u64    size()     { return m_bd->size; }
    inline u64    capacity() { return m_capacity; }
    inline u64    count()    { return m_capacity; }
    inline c_cstr name()     { return m_bd->name; }

    inline void drop() {
        memset(m_bd->data, '\0', m_bd->size);
        m_bd->cursor = m_bd->data;
    }

    inline u64 resize(u64 capacity) {
        N2CRASH_IF(m_resize == nullptr, N2Error::MissingData,
                   "Unable to resize ring %s (resize function not set)",
                   name());
        auto required_size = Ring<T>::precomputeSize(capacity);
        auto new_size = m_resize(m_bd, required_size);
        m_capacity = new_size / sizeof(T);
        return m_capacity;
    }

    inline T* consume(u64 count) {
        N2CRASH(N2Error::UnimplementedCode, "");
    }

    inline T& operator[](i64 index) {
        u64 target_index = increment(m_write_head, (index));
        return *((T*)(m_bd->data) + target_index);
    }

    inline T& push(T value) {
        T* mem       = (T*)(m_bd->data) + m_write_head;
        *mem         = value;
        m_write_head = increment(m_write_head);
        m_bd->cursor = (ptr)((T*)(m_bd->data) + m_write_head);
        return *mem;
    }


    /* ## Nested Iterorator class */
    class iterator;
    inline iterator begin() { return { *this }; }
    inline iterator end()   { return { *this,  capacity() }; }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Ring& ring;  /*< The ring being iterated.                  */
        u64   index; /*< The index this iterator is "referencing". */

        iterator(Ring& ring,
                 u64   index = 0)
            : ring  ( ring  )
            , index ( index ) { }

        inline bool operator==(const iterator& other) const {
            return &ring == &other.ring && index == other.index;
        }
        inline bool operator!=(const iterator& other) const {
            return &ring != &other.ring || index != other.index;
        }

        /* Pre-increment -- step forward and return `this`. */
        inline iterator& operator++() {
            index += 1;
            return *this;
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline iterator operator++(int) {
            iterator copy = *this;
            index += 1;
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`.
         * Be sure not to iterate past `end()`. */
        inline iterator& operator+=(u64 n) {
            index = n2min((index + n), ring.capacity());
            return *this;
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline iterator operator+(u64 n) {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        /* Dereference -- return the current value. */
        inline T& operator*() const { return ring[index]; }
    };


protected: /*< ## Protected Member Methods */
    inline u64 increment(u64 index, i64 n = 1) {
        if (n >= 0) {
            // TODO: Potential divide-by-zero error
            return ((index + n) % capacity());
        } else {
            return decrement(index, -n);
        }
    }

    inline u64 decrement(u64 index, i64 n = 1) {
        if (n >= 0) {
            while (index < (u64)n) {
                index += capacity();
            }
            return index - n;
        } else {
            return increment(index, -n);
        }
    }


    /* Ensure that only POD data is used in these views.*/
    ENFORCE_POD(T);
};

} /* namespace view */
} /* namespace mem  */
