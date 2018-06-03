/** Typed Ring View
 *  ===============
 *  Ring Views present a typed ring-buffer over an entire memory buffer. These
 *  views have no concept of partial-fullness, so iterations over a ring will
 *  always yield `capacity()` objects. The buffer's data is assumed to be
 *  initialized to `\0`, so "empty" data should be an expected and valid return
 *  for all iterations and subscript operations over rings.
 *
 *  The write head of the ring will be stored directly in the memory buffer's
 *  UserData `d1.u`, and will point to the last object written. For writes, the
 *  write head will be incremented, then the write will be performed. For
 *  iterations and subscripting the zero'th object will always be one index past
 *  the write head.
 *
 *  Resizes are permitted, though will never occur automatically.
 *
 *  TODO: Figure out consume. Does it mean anything in this context? If not, are
 *        user's going to be limited to adding objects one at a time? If so, how
 *        do we deal with split memory regions? (Scratch buffer, maybe? Unless
 *        the scratch is a ring....)
 *  TODO: Tests.
 */

#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/memory.h>
#include <nonstd/scope_guard.h>


namespace nonstd {

/* NB. This can only handle 63 bits of addressing, so if you have more than
 *     9.2 Exabytes of data in one ring, rethink life.
 */
template<typename T>
class ring {
public: /*< ## Class Methods */
    static constexpr u64 default_capacity = 32;

    static constexpr u64 precompute_size(u64 capacity)
    noexcept {
        return sizeof(T) * n2max(capacity, 1);
    }

    static inline buffer * initialize_buffer(buffer *const buf) {
        BREAK_IF(buf->type == buffer::type_id::ring,
            nonstd::error::reinitialized_memory,
            "Buffer corruption detected by type_id; buffer has already been "
            "correctly initialized as a ring.\n"
            "Underlying buffer: {}.", buf);
        BREAK_IF(buf->type != buffer::type_id::raw,
            nonstd::error::invalid_memory,
            "Buffer corruption detected by type_id; Attempting to initialize a "
            "previously-initialized buffer. type_id is currently 0x{:X}.\n"
            "Underlying buffer: {}.",
            buf->type, buf);

        BREAK_IF(buf->size < sizeof(T),
            nonstd::error::insufficient_memory,
            "This ring is being overlaid onto a buffer that is too small "
            "({} bytes) to fit at least one <{}> ({}  bytes). Rings _must_ be "
            "able to store at least one element.\n"
            "Underlying buffer: {}.",
            buf->size, type_name<T>(), sizeof(T),
            buf);

        buf->type = buffer::type_id::ring;

        return buf;
    }


protected: /*< ## Public Member Variables */
    buffer *const m_buf;

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
    ring(buffer *const buf) noexcept
        : m_buf ( buf )
    {
        /* Ensure that only POD types are used in containers.
         * We place ENFORCE_POD here s.t. it's only checked when the container
         * constructor is instantiated. This lets us declare containers that
         * wrap incomplete types, so long as those types are complete prior to
         * container construction.
         */
        ENFORCE_POD(T);

        ASSERT_M(m_buf->type == buffer::type_id::ring,
            "{} has type_id 0x{:X}", m_buf, m_buf->type);
    }
    ring(c_cstr name) noexcept
        : ring ( find_or_allocate_buffer(name) )
    { }
    ring(c_cstr name, u64 min_capacity) noexcept
        : ring ( find_or_allocate_buffer(name, min_capacity) )
    {
        if (capacity() < min_capacity) { resize(min_capacity); }
    }


public: /*< ## Public Member Methods */
    /* ## buffer Accessors */
    inline buffer       * const buf()        noexcept { return m_buf; }
    inline buffer const * const buf()  const noexcept { return m_buf; }
    inline u64                  size() const noexcept { return m_buf->size; }
    inline c_cstr               name() const noexcept { return m_buf->name; }

    /* ## Ring Accessors */
    inline u64       & write_index()       noexcept { return m_buf->userdata1.u_int;  }
    inline u64 const & write_index() const noexcept { return m_buf->userdata1.u_int;  }
    inline u64 const   count()       const noexcept { return capacity();              }
    inline u64 const   capacity()    const noexcept { return m_buf->size / sizeof(T); }

    /** Get / Set Methods
     *  -----------------
     */
    inline T& push(T value) noexcept {
        T* mem        = (T*)(m_buf->data) + write_index();
        *mem          = value;
        write_index() = _increment(write_index(), 1);
        return *mem;
    }

    inline T* consume(u64 count) {
        BREAK(nonstd::error::unimplemented, "");
    }

    inline T& operator[](i64 index) noexcept {
        u64 target_index = _increment(write_index(), index);
        return *((T*)(m_buf->data) + target_index);
    }

    inline void drop() noexcept {
        memset(m_buf->data, '\0', m_buf->size);
        write_index() = 0;
    }

    /** Resize Methods
     *  --------------
     *  These resizes involve moving memory around, so they're a bit tricky
     *  to reason about. As such, we're going to use pictures! Remember that
     *  there are no empty elements in rings, so all indexes must be
     *  considered when when moving data for the resize. All of our examples
     *  are going to start with,
     *
     *        B       A
     *  /----------/V---/
     *  #################
     *
     *  `V` indicated the read/write head, `/`s are there primarily to show
     *  the wrap point between the wrapped section (B) and the non-wrapped
     *  section (A), but will remain "after" the "resize" for clarity's sake.
     *  When upsizing, a new section (C) will be added. When downsizing, some
     *  elements from either section A or B will be removed from the buffer.
     */
    inline u64 resize(u64 new_capacity) {
        return resize_shifting_left(new_capacity);
    }
    inline u64 resize_shifting_left(u64 new_capacity) {
        using nonstd::make_guard;

        size_t required_size = ring<T>::precompute_size(new_capacity);

        ptr section_a = (ptr)((T*)(m_buf->data) + write_index());
        ptr section_b = m_buf->data;
        size_t size_of_b = write_index() * sizeof(T);
        size_t size_of_a = m_buf->size - size_of_b;

        if (new_capacity > capacity()) {
            /** Upsize Shifting Left
             *  ----------------------
             *        B       A
             *  /----------/V---/
             *  #################
             *
             *    A        B         C
             *  V---//----------//--------
             *  ##########################
             */
            auto bytes_added = required_size - m_buf->size;

            // Fetch enough scratch space to move section B aside.
            ptr scratch = n2malloc(size_of_b);
            BREAK_IF(!scratch,
                (std::error_code { errno, std::system_category() }),
                "Failed to `n2malloc` temporary memory. Godspeed.");
            auto guard = make_guard([scratch](){ n2free(scratch); });

            // Move section B aside.
            memmove(scratch, section_b, size_of_b);

            // Move section A to the front.
            memmove(m_buf->data, section_a, size_of_a);

            // Reinsert section B.
            memmove((m_buf->data + size_of_a), scratch, size_of_b);

            // Perform the resize, and reset the write index.
            memory::resize(m_buf, required_size);
            write_index() = 0;

            // Null the newly allcoated region
            memset(m_buf->data + size_of_a + size_of_b, '\0', bytes_added);
        } else if (new_capacity < capacity()) {
            /** Downsize Shifting Left
             *  ----------------------
             *        B       A
             *         abcde12345
             *  /----------/V---/
             *  #################
             *
             *    A      B
             *  12345       ab
             *  V---//-------/
             *  ##############
             */

            auto bytes_removed = m_buf->size - required_size;

            if (size_of_b > bytes_removed) {
                // There will be data retained from section B. Fetch enough
                // scratch space to move that data aside.
                size_t bytes_retained_from_b = size_of_b - bytes_removed;
                ptr scratch = n2malloc(bytes_retained_from_b);
                BREAK_IF(!scratch,
                    (std::error_code { errno, std::system_category() }),
                    "Failed to `n2malloc` temporary memory. Godspeed.");
                auto guard = make_guard([scratch](){ n2free(scratch); });

                // Move section B aside.
                memmove(scratch, section_b, bytes_retained_from_b);

                // Move section A to the front.
                memmove(m_buf->data, section_a, size_of_a);

                // Reinsert what's left of section B.
                memmove((m_buf->data + size_of_a),
                        scratch,
                        bytes_retained_from_b);

                // Perform the resize, and reset the write index.
                memory::resize(m_buf, required_size);
                write_index() = 0;
            } else {
                // There will not be data retained from section B, so all
                // of the data saved will come from section A. Helpfully,
                // this means we don't have to move data aside.
                size_t bytes_retained_from_a = required_size;

                // Move what's left of section A to the front.
                memmove(m_buf->data, section_a, bytes_retained_from_a);

                // Perform the resize, and reset the write index.
                memory::resize(m_buf, required_size);
                write_index() = 0;
            }
        }

        return capacity();
    }

    inline u64 resize_shifting_right(u64 new_capacity) {
        using nonstd::make_guard;

        size_t required_size = ring<T>::precompute_size(new_capacity);

        ptr section_a = (ptr)((T*)(m_buf->data) + write_index());
        ptr section_b = m_buf->data;
        size_t size_of_b = write_index() * sizeof(T);
        size_t size_of_a = m_buf->size - size_of_b;

        if (new_capacity > capacity()) {
            /** Upsizing Shifting Right
             *  -------------------------
             *        B       A
             *  /----------/V---/
             *  #################
             *
             *      C      A        B
             *  V-------//---//----------/
             *  ##########################
             */

            auto bytes_added = required_size - m_buf->size;

            // Perform the resize.
            memory::resize(m_buf, required_size);

            // Recapture the locations of section_a and section_b; the n2realloc
            // call may have moved the base data pointer.
            section_a = (ptr)((T*)(m_buf->data) + write_index());
            section_b = m_buf->data;

            // Fetch enough scratch space to move section B aside.
            ptr scratch = n2malloc(size_of_b);
            BREAK_IF(!scratch,
                (std::error_code { errno, std::system_category() }),
                "Failed to `n2malloc` temporary memory. Godspeed.");
            auto guard = make_guard([scratch](){ n2free(scratch); });

            // Move section B aside.
            memmove(scratch, section_b, size_of_b);

            // Move section A into place (bytes_added away from the front).
            memmove((m_buf->data + bytes_added),
                    section_a,
                    size_of_a);

            // Reinsert section B.
            memmove((m_buf->data + bytes_added + size_of_a),
                    scratch,
                    size_of_b);

            // Reset the write index to the beginning of the ring.
            write_index() = 0;

            // Null the newly allcoated region
            memset(m_buf->data, '\0', bytes_added);
        } else if (new_capacity < capacity()) {
            /** Downsizing Shifting Right
             *  -------------------------
             *        B       A
             *         abcde12345
             *  /----------/V---/
             *  #################
             *
             *   A       B
             *  345       abcde
             *  V-//----------/
             *  ##############
             */

            auto bytes_removed = m_buf->size - required_size;

            if (size_of_a > bytes_removed) {
                // There will be data retained from section A, so we have to
                // move section B aside to make room for that. Fetch enough
                // scratch space to move section B aside.
                ptr scratch = n2malloc(size_of_b);
                BREAK_IF(!scratch,
                    (std::error_code { errno, std::system_category() }),
                    "Failed to `n2malloc` temporary memory. Godspeed.");
                auto guard = make_guard([scratch](){ n2free(scratch); });

                // Move section B aside.
                memmove(scratch, section_b, size_of_b);

                // Move what's left of section A to the front.
                size_t bytes_retained_from_a = size_of_a - bytes_removed;
                memmove(m_buf->data,
                        (section_a + bytes_removed),
                        bytes_retained_from_a);

                // Reinsert section B.
                memmove((m_buf->data + bytes_retained_from_a),
                        scratch,
                        size_of_b);

                // Perform the resize, and reset the write index.
                memory::resize(m_buf, required_size);
                write_index() = 0;
            } else {
                // There will not be data retained from section A, so all
                // of the data saved will come from section B. Helpfully,
                // this means we don't need to move data aside.
                size_t bytes_retained_from_b = required_size;

                // Move what's left of section B to the front.
                memmove(m_buf->data,
                        (section_b + size_of_b - bytes_retained_from_b),
                        bytes_retained_from_b);

                // Perform the resize, and reset the write index.
                memory::resize(m_buf, required_size);
                write_index() = 0;
            }
        }

        return capacity();
    }

    inline u64 resize_after_dropping(u64 new_capacity) {
        size_t required_size = ring<T>::precompute_size(new_capacity);
        memory::resize(m_buf, required_size);

        // This will correctly null the ring's data, and reset the write index.
        drop();

        return capacity();
    }


    /* ## Nested Iterator class */
    class iterator;
    inline iterator begin() noexcept { return { *this, 0          }; }
    inline iterator end()   noexcept { return { *this, capacity() }; }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        ring& _ring;  /*< The ring being iterated.                  */
        u64   _index; /*< The index this iterator is "referencing". */

        iterator(ring& ring,
                 u64   index = 0) noexcept
            : _ring  ( ring  )
            , _index ( index ) { }

        inline bool operator==(const iterator& other) const noexcept {
            return &_ring == &other._ring && _index == other._index;
        }
        inline bool operator!=(const iterator& other) const noexcept {
            return &_ring != &other._ring || _index != other._index;
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
        /* Increment and assign -- step forward by `n` and return `this`.
         * Be sure not to iterate past `end()`. */
        inline iterator& operator+=(u64 n) noexcept {
            _index = n2min((_index + n), _ring.capacity());
            return *this;
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline iterator operator+(u64 n) noexcept {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        /* Dereference -- return the current value. */
        inline T& operator*() const noexcept { return _ring[_index]; }
    };


protected: /*< ## Protected Member Methods */
    inline u64 _increment(u64 index, i64 n = 1) noexcept {
        if (n >= 0) {
            // TODO: Potential divide-by-zero error
            return ((index + n) % capacity());
        } else {
            return _decrement(index, -n);
        }
    }

    inline u64 _decrement(u64 index, i64 n = 1) noexcept {
        if (n >= 0) {
            while (index < (u64)n) {
                index += capacity();
            }
            return index - n;
        } else {
            return _increment(index, -n);
        }
    }
};

} /* namespace nonstd */
