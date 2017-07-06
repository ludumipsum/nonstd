/** Typed Ring View
 *  ===============
 *  Ring Views present a typed ring-buffer over an entire memory buffer. These
 *  views have no concept of partial-fullness, so iterations over a Ring will
 *  always yield `capacity()` objects. The buffer's data is assumed to be
 *  initialized to `\0`, so "empty" data should be an expected and valid return
 *  for all iterations and subscript operations over Rings.
 *
 *  The write head of the Ring will be stored directly in the memory buffer's
 *  UserData `d1.u`, and will point to the last object written. For writes, the
 *  write head will be incremented, then the write will be performed. For
 *  iterations and subscripting the zero'th object will always be one index past
 *  the write head.
 *
 *  Resizes are permitted, though will never occur automatically.
 *  TODO: Define resize semantics.
 *  TODO: Figure out consume. Does it mean anything in this context? If not, are
 *        user's going to be limited to adding objects one at a time? If so, how
 *        do we deal with split memory regions? (Scratch buffer, maybe? Unless
 *        the scratch is a ring....)
 *  TODO: Better comments.
 *  TODO: Tests.
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
    Buffer *const m_buf;
    ResizeFn      m_resize;
    u64 &         m_write_index;

public: /*< ## Ctors, Detors, and Assignments */
    Ring(Buffer *const buf, ResizeFn resize = nullptr)
        : m_buf         ( buf                  )
        , m_resize      ( resize               )
        , m_write_index ( buf->userdata1.u_int ) { }


public: /*< ## Public Memeber Methods */
    inline u64    size()     { return m_buf->size;             }
    inline u64    count()    { return capacity();              }
    inline u64    capacity() { return m_buf->size / sizeof(T); }
    inline c_cstr name()     { return m_buf->name;             }

    inline void drop() {
        memset(m_buf->data, '\0', m_buf->size);
        m_write_index = 0;
    }

    inline u64 resize(u64 new_capacity) {
#if defined(DEBUG)
        N2CRASH_IF(m_resize == nullptr, N2Error::NullPtr,
            "Attempting to resize a Ring that has no associated "
            "resize function.\n"
            "Underlying buffer is named %s, and it is located at %p.",
            m_buf->name, m_buf);
#endif
        auto required_size = Ring<T>::precomputeSize(new_capacity);
        m_resize(m_buf, required_size);
        return capacity();
    }

    inline T* consume(u64 count) {
        N2CRASH(N2Error::UnimplementedCode, "");
    }

    inline T& operator[](i64 index) {
        u64 target_index = increment(m_write_index, index);
        return *((T*)(m_buf->data) + target_index);
    }

    inline T& push(T value) {
        T* mem       = (T*)(m_buf->data) + m_write_index;
        *mem         = value;
        m_write_index = increment(m_write_index, 1);
        return *mem;
    }


    /* ## Nested Iterorator class */
    class iterator;
    inline iterator begin() { return { *this, 0          }; }
    inline iterator end()   { return { *this, capacity() }; }

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
