/* Typed Circular Buffer Stream
 * ============================
 * Buffer Streams are a View over Buffer Descriptors that present a typed
 * circular-buffer stream. Resize operations are permitted, though will never
 * occur automatically. When the Stream is full, additional `push()` and
 * `consume()` operations will overwrite the oldest piece of data in the stream.
 * Both subscript operations (`(*this)[0]`) and iteration start from the oldest
 * object in the Stream, and continue to the newest --- this means that
 * iterations may not yield `capacity()` objects, and that `(*this)[count()+1]`
 * is undefined.
 *
 * The Stream::Metadata tracks both the `read_head` and `write_head`. When
 * `read_head == write_head`, the Stream is empty. On `push()` and `consume()`
 * operations, the `write_head` will be advanced, the `read_head` will
 * optionally be advanced to one past the `write_head`, then writes will be
 * performed.
 *
 * TODO: Fix resizing; define semantics, rework implementation.
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
class Stream {
protected:
    static const u32 magic = 0xDEFACED;
    struct Metadata {
        u32 magic;
        u64 write_head;
        u64 read_head;
        T   data[];
    };

public:
    inline static u64 precomputeSize(u64 capacity) {
        return sizeof(Metadata) + sizeof(T) * capacity;
    }

    inline static void initializeBuffer(Descriptor *const bd) {
        Metadata metadata = (Metadata*)bd->data;
        if (metadata->magic && metadata->magic != magic) {
            LOG("WARNING: Buffer Stream corruption detected.\n"
                "Underlying buffer is named %s, and is located at %p. Magic "
                "number was expected to be %x, but is %x.\n"
                "Clearing all associated data and reinitializing the Stream",
                bd->name, bd, magic, metadata->magic);
            DEBUG_BREAKPOINT();
        }
        metadata->magic      = magic;
        metadata->write_head = 0;
        metadata->read_head  = 0;
        memset(metadata->data, '\0', (bd->size - sizeof(Metadata)));
    }


protected:
    Descriptor * const m_bd;
    Metadata   *       m_metadata;
    BufferResizeFn     m_resize;

public:
    // TODO: Magic number check?
    Stream(Descriptor *const bd,
           BufferResizeFn resize = nullptr)
        : m_bd       ( bd                      )
        , m_metadata ( (Metadata*)(m_bd->data) )
        , m_resize   ( resize                  ) { }


    inline u64 resize(u64 capacity) {
#if 0
        // TODO: So many more safety checks...
        if (m_resize) {
            m_resize(m_bd, precomputeSize(capacity));
        }
        else { BREAKPOINT(); }
#endif
        LOG("This function is currently unimplemented.");
        BREAKPOINT();

    }

    inline void drop() {
        m_metadata->read_head = m_metadata->write_head = 0;
    }

    inline u64 size() {
        return m_bd->size;
    }

    /* Returns the number of objects currently accessible in the Stream.
     * Let's draw a picture.
     *     0       1       2       3       4       5       6       7
     *                     | Read ------------------ Write |          | (6-2)
     *     >-------- Write |                               | Read -<  | (7-6)+2
     */
    inline u64 count() {
        if (m_metadata->write_head > m_metadata->read_head)
            return (m_metadata->write_head - m_metadata->read_head);
        return (capacity() - m_metadata->read_head) + m_metadata->write_head;
    }

    inline u64 capacity() {
        return (m_bd->size - sizeof(Metadata)) / sizeof(T);
    }


    inline T* consume(u64 count) {
        LOG("This function is currently unimplemented.");
        BREAKPOINT();
    }

    inline T& operator[](u64 index) {
#if defined(DEBUG)
        // TODO: Better logging
        if (index >= capacity()) {
            LOG("buffer::Stream -- index out of bounds. %d / %d in %s."
                index, capacity(), m_bd->name);
            BREAKPOINT();
        } else if (index > count()) {
            LOG("buffer::Stream -- invalid data requested. %d / %d in %s."
                index, capacity(), m_bd->name);
            BREAKPOINT();
        }
#endif
        u64 target_index = increment(m_metadata->read_head, index);
        return m_metadata->data[target_index];
    }

    inline T& push(T& value) {
        m_metadata->write_head = increment(m_metadata->write_head);
        (*this)[m_metadata->write_head] = value;
        if (m_metadata->write_head == m_metadata->read_head)
            m_metadata->read_head = increment(m_metadata->read_head);
        return m_metadata->data[m_metadata->write_head];
    }


    class iterator;
    inline iterator begin() { return iterator(*this, m_metadata->read_head);  }
    inline iterator end()   { return iterator(*this, m_metadata->write_head); }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Stream& stream;
        u64     index;

        iterator(Stream& stream,
                 u64 index=0)
            : stream  ( stream   )
            , index ( index ) { }

        inline bool operator==(const iterator& other) const {
            return &stream == &other.stream && index == other.index;
        }
        inline bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        // Pre-increment -- step forward and return `this`.
        inline iterator& operator++() {
            index = stream.increment(index);
            return *this;
        }
        // Post-increment -- return a copy created before stepping forward.
        inline iterator operator++(int) {
            iterator copy = *this;
            index = stream.increment(index);
            return copy;
        }
        // Increment and assign -- step forward by `n` and return `this`.
        inline iterator& operator+=(u64 n) {
            index = stream.increment(index, n);
            return *this;
        }
        // Arithmetic increment -- return an incremented copy.
        inline iterator operator+(u64 n) {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        // Dereference -- return the current value.
        inline T& operator*() const { return stream[index]; }
    };


protected:
    inline u64 increment(u64 index, u64 n = 1) {
        // TODO: Potential divide-by-zero error
        return ((index + n) % capacity());
    }
    inline u64 decrement(u64 index, u64 n = 1) {
        if (n <= index) { return (index - n); }
        u64 past_zero = n - index;
        return (capacity() - past_zero);
    }
};

} /* namespace buffer */
