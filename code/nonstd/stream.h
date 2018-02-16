/** Typed Stream View
 *  =================
 *  Streams present a typed circular buffer over a buffer. Unlike Rings, this
 *  container is aware of both its capacity and its count, and tracks usage
 *  within the metadata block. The read and write heads are also tracked using
 *  the buffer userdata.uint members. When indexing or iterating, only the used
 *  sub-section of a stream's data will be accessible, so no `\0`-initialized
 *  data will ever be accessible.
 *
 *  Iteration and subscript operations are 0-indexed to the oldest element in
 *  the stream. Because only the used sub-section of data is available to be
 *  read, iterations across a stream may not yield `capacity()` elements, and
 *  `capacity()-1` may not be a valid index. When `push()` or `consume()` are
 *  called against a full stream, the oldest data in the stream will be replaced
 *  with incoming data.
 *
 *  When in DEBUG mode, bounds checking is performed on index operations to
 *  insure that out-of-bounds data is never read.
 *
 *  TODO: Figure out consume. Does it mean anything in this context? If not, are
 *        user's going to be limited to adding objects one at a time? If so, how
 *        do we deal with split memory regions? (Scratch buffer, maybe? Unless
 *        the scratch is a ring....)
 */

#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/memory.h>
#include <nonstd/type_traits_ext.h>


namespace nonstd {

template<typename T>
class stream {

protected: /*< ## Inner-Types */

    struct Metadata {
        u64 count;
        u64 capacity;
        T   data[];
    };


public: /*< ## Class Methods */
    static constexpr u64 default_capacity = 64;

    static constexpr u64 precompute_size(u64 capacity)
    noexcept {
        return sizeof(Metadata) + (sizeof(T) * n2max(capacity, 1));
    }

    static inline buffer * initialize_buffer(buffer *const buf) {
        BREAK_IF(buf->type == buffer::type_id::stream,
            nonstd::error::reinitialized_memory,
            "Buffer corruption detected by type_id; buffer has already been "
            "correctly initialized as a stream.\n"
            "Underlying buffer: {}.", buf);
        BREAK_IF(buf->type != buffer::type_id::raw,
            nonstd::error::invalid_memory,
            "Buffer corruption detected by type_id; Attempting to initialize a "
            "previously-initialized buffer. type_id is currently 0x{:X}.\n"
            "Underlying buffer: {}.",
            buf->type, buf);

        Metadata * metadata = (Metadata *)(buf->data);

        u64 data_region_size = buf->size - sizeof(Metadata);
        u64 capacity         = (data_region_size) / sizeof(T);

        BREAK_IF(buf->size < (sizeof(Metadata) + sizeof(T)),
            nonstd::error::insufficient_memory,
            "This stream is being overlaid onto a buffer that is too small "
            "({} bytes) to fit the stream Metadata ({} bytes) and at least one "
            "<{}> ({} bytes). Streams _must_ be able to store at least "
            "one element.\n"
            "Underlying buffer: {}.",
            buf->size, sizeof(Metadata), type_name<T>(), sizeof(T), buf);

        metadata->count    = 0;
        metadata->capacity = capacity;
        memset(metadata->data, '\0', data_region_size);

        buf->type = buffer::type_id::stream;

        return buf;
    }


protected: /*< ## Public Member Variables */
    buffer   *const  m_buf;
    Metadata *&      m_metadata;

    static inline
    buffer * find_or_allocate_buffer(c_cstr name,
                                     u64 capacity = default_capacity)
    noexcept {
        using memory::find;
        using memory::allocate;

        auto maybe_buf = find(name);
        return maybe_buf
             ? *maybe_buf
             : initialize_buffer(allocate(name, precompute_size(capacity)));
    }


public: /*< ## Ctors, Detors, and Assignments */
    stream(buffer *const buf) noexcept
        : m_buf      ( buf                                     )
        , m_metadata ( reinterpret_cast<Metadata*&>(buf->data) )
    {
        /* Ensure that only POD types are used by placing ENFORCE_POD here. */
        ENFORCE_POD(T);

        ASSERT_M(m_buf->type == buffer::type_id::stream,
            "{} has type_id 0x{:X}", m_buf, m_buf->type);
    }
    stream(c_cstr name)
        : stream ( find_or_allocate_buffer(name) )
    { }
    stream(c_cstr name, u64 min_capacity)
        : stream ( find_or_allocate_buffer(name, min_capacity) )
    {
        if (capacity() < min_capacity) { resize(min_capacity); }
    }


public: /*< ## Public Member Methods */
    inline buffer       * const buf()        noexcept { return m_buf; }
    inline buffer const * const buf()  const noexcept { return m_buf; }
    inline u64                  size() const noexcept { return m_buf->size; }
    inline c_cstr               name() const noexcept { return m_buf->name; }


public: /*< ## Stream Accessors */
    inline u64       & count()          noexcept { return m_metadata->count;    }
    inline u64 const & count()    const noexcept { return m_metadata->count;    }
    inline u64       & capacity()       noexcept { return m_metadata->capacity; }
    inline u64 const & capacity() const noexcept { return m_metadata->capacity; }

    /** Get / Set Methods
     *  -----------------
     */
    inline T& push(T value) noexcept {
        T& mem = m_metadata->data[_write_index()];
        mem = value;

        _write_index() = _increment_index(_write_index());
        if (count() == capacity()) {
            _read_index() = _increment_index(_read_index());
        } else {
            count() += 1;
        }

        return mem;
    }

    inline T* consume(u64 count) {
        BREAK(nonstd::error::unimplemented, "");
    }

    inline T& operator[](i64 index) noexcept {
#if defined(DEBUG)
        if (index >= count()) {
            throw std::out_of_range {
                "Stream index access exceeds current count.\n"
                "Entry (1-indexed) {} / {} ({} maximum).\n"
                "{}"_format(
                index+1, count(), capacity(), m_buf)
            };
        }
#endif
        u64 target_index = _increment_index(_read_index(), index);
        return m_metadata->data[target_index];
    }

    inline void drop() noexcept {
        count() = _write_index() = _read_index() = 0;
    }

    /** Resize Methods
     *  --------------
     *  Currently unimplemented. Look to ring.h for inspiration (and a warning
     *  about how complex this shit is gonna be).
     */
    inline u64 resize(u64 capacity) {
        BREAK(nonstd::error::unimplemented, "");
        return 0;
    }


protected: /*< ## Helper Methods */
    inline u64       & _write_index()       noexcept { return m_buf->userdata1.u_int; }
    inline u64 const & _write_index() const noexcept { return m_buf->userdata1.u_int; }
    inline u64       & _read_index()        noexcept { return m_buf->userdata2.u_int; }
    inline u64 const & _read_index()  const noexcept { return m_buf->userdata2.u_int; }

    inline u64 _increment_index(u64 index, i64 n = 1) const noexcept {
        if (n >= 0) {
            return ((index + n) % capacity());
        } else {
            return _decrement_index(index, -n);
        }
    }

    inline u64 _decrement_index(u64 index, i64 n = 1) const noexcept {
        if (n >= 0) {
            while (index < (u64)n) {
                index += capacity();
            }
            return index - n;
        } else {
            return _increment_index(index, -n);
        }
    }


public:
    /** Nested Iterator class
     *  ---------------------
     */
    class iterator;
    inline iterator begin() noexcept { return { *this, 0       }; }
    inline iterator end()   noexcept { return { *this, count() }; }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        stream& _stream; /*< The stream being iterated.                */
        u64     _index;  /*< The index this iterator is "referencing". */

        iterator(stream& stream,
                 u64     _index = 0)
        noexcept
            : _stream ( stream )
            , _index  ( _index  ) { }

        inline bool operator==(const iterator& other) const noexcept {
            return &_stream == &other._stream && _index == other._index;
        }

        inline bool operator!=(const iterator& other) const noexcept {
            return !(*this == other);
        }

        /* Pre-increment -- step forward and return `this`. */
        inline iterator& operator++() noexcept {
            _index += 1;
            return *this;
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline iterator operator++(int) noexcept {
            iterator copy = *this;
            _index += 1;
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`. */
        // TODO: Verify we don't increment past the end of the stream.
        inline iterator& operator+=(u64 n) noexcept {
            _index = n2min((_index + n), _stream.capacity());
            return *this;
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline iterator operator+(u64 n) noexcept {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        /* Dereference -- return the current value. */
        inline T& operator*() const noexcept { return _stream[_index]; }
    };

};

} /* namespace nonstd */
