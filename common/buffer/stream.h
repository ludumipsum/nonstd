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
        u64 capacity;
        u64 count;
        u64 write_head;
        u64 read_head;
        T   data[];
    };

public:
    inline static u64 precomputeSize(u64 capacity) {
        return sizeof(Metadata) + sizeof(T) * capacity;
    }

    inline static void initializeBuffer(Descriptor *const bd) {
        Metadata * metadata = (Metadata*)bd->data;
        if (metadata->magic && metadata->magic != magic) {
            LOG("WARNING: Buffer Stream corruption detected.\n"
                "Underlying buffer is named %s, and is located at %p. Magic "
                "number was expected to be %x, but is %x.\n"
                "Clearing all associated data (" Fu64 " elements) and "
                "reinitializing the Stream",
                bd->name, bd, magic, metadata->magic, metadata->count);
            DEBUG_BREAKPOINT();
        }
        metadata->magic      = magic;
        metadata->count      = 0;
        metadata->capacity   = (bd->size - sizeof(Metadata)) / sizeof(T);
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
        m_metadata->read_head = m_metadata->write_head = m_metadata->count = 0;
    }

    inline u64 size() {
        return m_bd->size;
    }

    /* Returns the number of objects currently accessible in the Stream. */
    inline u64 count() {
        return m_metadata->count;
    }

    inline u64 capacity() {
        return m_metadata->capacity;
    }


    inline T* consume(u64 count) {
        LOG("This function is currently unimplemented.");
        BREAKPOINT();
    }

    /**
     * Subscript indexing for reading / writing values in the stream.
     * @param  index Target element index, indexing from the last-written
     *               element; `[0]` will always return the oldest element in the
     *               stream, and `[count()-1]` will always return the newest.
     * @return       A reference to the given element; assignments to the
     *               returned element will be recorded in the Stream.
     * If the element indexed is the first uninitialized element (if `count() <
     * capacity()` and `index == count()`) `count` will be incremented using the
     * same logic that governs `push()`.
     * NOTE: This implementation may be deficient. It may be better to wrap the
     * return of this function in a modified Optional. If `index` is the first
     * uninitialized element, this Optional could "read" as `false`, but the
     * write overload could correctly assign to the Stream and increment
     * `count`. For a rough implementation of this sort of, see,
     * http://stackoverflow.com/questions/3581981
     */
    inline T& operator[](u64 index) {
#if defined(DEBUG)
        // TODO: Better logging
        if (index >= capacity()) {
            LOG("buffer::Stream -- index out of bounds. %d / %d in %s."
                index, capacity(), m_bd->name);
            BREAKPOINT();
        } else if (index > count()) {
            LOG("buffer::Stream -- invalid data requested. %d / %d in %s."
                index, count(), m_bd->name);
            BREAKPOINT();
        }
#endif
        u64 target_index = increment(m_metadata->read_head, index);
        T& ret = m_metadata->data[target_index];

        if (index == count()) { /*< We should increment the write head */
            m_metadata->write_head = increment(m_metadata->write_head);
            if (count() == capacity()) { /* We should increment the read head */
                m_metadata->read_head = increment(m_metadata->read_head);
            } else {
                m_metadata->count += 1;
            }
        }

        return ret;
    }

    inline T& push(T value) {
        T* mem = m_metadata->data + m_metadata->write_head;
        *mem = value;
        m_metadata->write_head = increment(m_metadata->write_head);
        if (count() == capacity()) {
            m_metadata->read_head = increment(m_metadata->read_head);
        } else {
            m_metadata->count += 1;
        }
        return *mem;
    }


    class iterator;
    inline iterator begin() { return { *this }; }
    inline iterator end()   { return { *this, count() }; }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Stream& stream;
        u64     index;

        iterator(Stream& stream,
                 u64     index = 0)
            : stream ( stream )
            , index  ( index  ) { }

        inline bool operator==(const iterator& other) const {
            return &stream == &other.stream && index == other.index;
        }

        inline bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        // Pre-increment -- step forward and return `this`.
        inline iterator& operator++() {
            index += 1;
            return *this;
        }
        // Post-increment -- return a copy created before stepping forward.
        inline iterator operator++(int) {
            iterator copy = *this;
            index += 1;
            return copy;
        }
        // Increment and assign -- step forward by `n` and return `this`.
        // TODO: Verify we don't increment past the end of the stream.
        inline iterator& operator+=(u64 n) {
            index += n;
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


    /* Ensure that only POD data is used in these views.*/
    ENFORCE_POD(T);
};

} /* namespace buffer */
