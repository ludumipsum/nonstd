/** Typed Stream View
 *  =================
 *  Stream Views present a typed circular buffer over a subsection of a Memory
 *  Buffer. Unlike Rings, this view is aware of both its capacity and its
 *  count, and tracks usage within the metadata block. Only the used sub-section
 *  of a Stream's data will be accessible through indexing or iterating, so no
 *  `\0`-initialized data will ever be accessible. When in DEBUG mode, bounds
 *  checking is performed on index operations to insure that out-of-bounds data
 *  is never read.
 *
 *  Iteration and subscript operations are 0-indexed to the oldest element in
 *  the Stream. Because only the used sub-section of data is available to be
 *  read, iterations across a Stream may not yield `capacity()` elements, and
 *  `capacity()-1` may not be a valid index. When `push()` or `consume()` are
 *  called against a full Stream, the oldest data in the Stream will be replaced
 *  with incoming data.
 *
 *  Stream::Metadata tracks both the `read_head` and `write_head`. When
 *  `read_head == write_head`, the Stream is empty. On `push()` and `consume()`,
 *  the `write_head` will be advanced, the `read_head` will optionally be
 *  advanced to one past the `write_head`.
 *
 *  TODO: Figure out consume. Does it mean anything in this context? If not, are
 *        user's going to be limited to adding objects one at a time? If so, how
 *        do we deal with split memory regions? (Scratch buffer, maybe? Unless
 *        the scratch is a ring....)
 *  TODO: Tests.
 */

#pragma once

#include <nonstd/cpp1z/type_trait_assertions.h>
#include <nonstd/core/break.h>
#include <nonstd/core/primitive_types.h>
#include <nonstd/memory/buffer.h>


namespace nonstd {

template<typename T>
class Stream {
protected: /*< ## Innter-Types */
    struct Metadata {
        u64 capacity;
        u64 count;
        u64 write_head;
        u64 read_head;
        T   data[];
    };


public: /*< ## Class Methods */
    static const u64 default_capacity = 64;

    inline static u64 precomputeSize(u64 capacity = default_capacity) {
        return sizeof(Metadata) + sizeof(T) * capacity;
    }

    inline static void initializeBuffer(Buffer *const buf) {
        Metadata * metadata = (Metadata*)buf->data;
        /* If the type check is correct, no initialization is required. */
        if (buf->type_id == Buffer::type_id::stream) { return; }

#if defined(DEBUG)
        N2BREAK_IF((buf->type_id != Buffer::type_id::raw &&
                    buf->type_id != Buffer::type_id::array),
                   N2Error::InvalidMemory,
                   "Array corruption detected by type_id --- 0x%X is neither 0 "
                   "nor 0x%X.\n"
                   "Underlying buffer is named %s and is located at %p.",
                   buf->type_id, Buffer::type_id::array, buf->name, buf);
#endif

        N2BREAK_IF(buf->size < sizeof(Metadata), N2Error::InsufficientMemory,
            "Buffer Stream is being overlaid onto a Buffer that is too small ("
            Fu64 ") to fit the Stream Metadata (" Fu64 ").\n"
            "Underlying buffer is named %s, and it is located at %p.",
            buf->size, sizeof(Metadata),
            buf->name, buf);

        buf->type_id = Buffer::type_id::stream;
        metadata->capacity   = (buf->size - sizeof(Metadata)) / sizeof(T);
        metadata->count      = 0;
        metadata->write_head = 0;
        metadata->read_head  = 0;
        memset(metadata->data, '\0', (buf->size - sizeof(Metadata)));
    }


protected: /*< ## Public Member Variables */
    Buffer   *const  m_buf;
    Metadata *       m_metadata;
    Buffer::ResizeFn m_resize;
    // TODO: This is an anti-pattern (consider reseating the metadata member
    //       after a resize). If we ever use this class again, fix this.
    u64 &            m_capacity;
    u64 &            m_count;
    u64 &            m_write_head;
    u64 &            m_read_head;


public: /*< ## Ctors, Detors, and Assignments */
    Stream(Buffer *const buf, Buffer::ResizeFn resize = nullptr)
        : m_buf        ( buf                    )
        , m_metadata   ( (Metadata*)(buf->data) )
        , m_resize     ( resize                 )
        , m_capacity   ( m_metadata->capacity   )
        , m_count      ( m_metadata->count      )
        , m_write_head ( m_metadata->write_head )
        , m_read_head  ( m_metadata->read_head  )
    {
        /* Ensure that only POD data is used in Streams. */
        ENFORCE_POD(T);
    }


public: /*< ## Public Memeber Methods */
    inline u64 size()     { return m_buf->size; }
    inline u64 count()    { return m_count; }
    inline u64 capacity() { return m_capacity; }

    inline void drop() {
        m_read_head = m_write_head = m_count = 0;
    }

    /* Push a new value into the Stream. */
    inline T& push(T value) {
        T& mem = m_metadata->data[m_write_head];
        mem = value;

        /* The write head needs to be incremented, and either
         *  - The Stream is full, the oldest element in the stream has been
         *    overwritten, and the read head needs to be similarly bumped.
         *  - The Stream has one more element in it than it did previously, and
         *    the current count() needs to be incremented.
         */
        m_write_head = increment(m_write_head);
        if (count() == capacity()) {
            m_read_head = increment(m_read_head);
        } else {
            m_count += 1;
        }

        return mem;
    }

    inline T* consume(u64 count) {
        N2BREAK(N2Error::UnimplementedCode, "");
    }

    /* Direct subscript operator. */
    inline T& operator[](u64 index) {
#if defined(DEBUG)
        N2BREAK_IF(index >= capacity(), N2Error::OutOfBounds,
            "Stream index access exceeds maximum capacity.\n"
            "Entry (1-indexed) " Fu64 " / " Fu64 " (" Fu64 " maximum).\n"
            "Underlying buffer is named %s, and it is located at %p.",
            index+1, count(), capacity(), m_buf->name, m_buf);
        N2BREAK_IF(index >= count(), N2Error::OutOfBounds,
            "Stream index access exceeds current count.\n"
            "Entry (1-indexed) " Fu64 " / " Fu64 " (" Fu64 " maximum).\n"
            "Underlying buffer is named %s, and it is located at %p.",
            index+1, count(), capacity(), m_buf->name, m_buf);
#endif
        u64 target_index = increment(m_read_head, index);
        return m_metadata->data[target_index];
    }

    inline u64 resize(u64 capacity) {
        N2BREAK(N2Error::UnimplementedCode, "");
    }


    /* ## Nested Iterorator class */
    class iterator;
    inline iterator begin() { return { *this }; }
    inline iterator end()   { return { *this, count() }; }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Stream& stream; /*< The stream being iterated.                */
        u64     index;  /*< The index this iterator is "referencing". */

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
        /* Increment and assign -- step forward by `n` and return `this`. */
        // TODO: Verify we don't increment past the end of the stream.
        inline iterator& operator+=(u64 n) {
            index = n2min((index + n), stream.capacity());
            return *this;
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline iterator operator+(u64 n) {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        /* Dereference -- return the current value. */
        inline T& operator*() const { return stream[index]; }
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
};

} /* namespace nonstd */
