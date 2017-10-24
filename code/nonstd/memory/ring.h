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

/* NB. This can only handle 63 bits of addressing, so if you have more than
 *     9.2 Exabytes of data in one Ring, rethink life.
 */
template<typename T>
class Ring {
public: /*< ## Class Methods */
    static constexpr u64 default_capacity = 64;

    static constexpr u64 precomputeSize(u64 capacity = default_capacity)
    noexcept {
        return sizeof(T) * n2max(capacity, 1);
    }

    static inline void initializeBuffer(Buffer *const buf) {
        N2BREAK_IF(buf->type == Buffer::type_id::ring,
                   N2Error::DoubleInitialization,
                   "Buffer corruption detected by type_id; Buffer has already "
                   "been correctly initialized as a Ring.\n"
                   "Underlying buffer is named '{}', and it is located at {}.",
                   buf->name, buf);
        N2BREAK_IF(buf->type != Buffer::type_id::raw,
                   N2Error::InvalidMemory,
                   "Buffer corruption detected by type_id; Attempting to "
                   "initialize a previously initialized Buffer. type_id is "
                   "currently 0x{:X}\n"
                   "Underlying buffer is named '{}', and it is located at {}.",
                   buf->type, buf->name, buf);

        N2BREAK_IF(buf->size < sizeof(T),
                   N2Error::InsufficientMemory,
                   "This Ring is being overlaid onto a Buffer that is too "
                   "small ({}B) to fit at least one <{}>({}B). Rings _must_ be "
                   "able to store at least one element.\n"
                   "Underlying buffer is named '{}', and it is located at {}.",
                   buf->size, type_name<T>(), sizeof(T),
                   buf->name, buf->data);

        buf->type = Buffer::type_id::ring;
    }


protected: /*< ## Public Member Variables */
    Buffer *const    m_buf;
    Buffer::ResizeFn m_resize;


public: /*< ## Ctors, Detors, and Assignments */
    Ring(Buffer *const buf, Buffer::ResizeFn resize = nullptr) noexcept
        : m_buf    ( buf    )
        , m_resize ( resize )
    {
        /* Ensure that only POD types are used by placing ENFORCE_POD here. */
        ENFORCE_POD(T);

        /* Verify `buf` has been correctly initialized. */
        N2BREAK_IF(m_buf->type != Buffer::type_id::ring,
            N2Error::InvalidMemory,
            "Ring corruption detected by type_id; Buffer has not been "
            "initialized as a Ring.\n"
            "type_id is currently 0x{:X}\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            m_buf->type, m_buf->name, m_buf);
    }


public: /*< ## Public Member Methods */
    /* ## Buffer Accessors */
    inline Buffer       * const buffer()       noexcept { return m_buf; }
    inline Buffer const * const buffer() const noexcept { return m_buf; }
    inline u64                  size()   const noexcept { return m_buf->size; }
    inline c_cstr               name()   const noexcept { return m_buf->name; }

    /* ## HashTable Accessors */
    inline u64       & write_index()       noexcept { return m_buf->userdata1.u_int;  }
    inline u64 const & write_index() const noexcept { return m_buf->userdata1.u_int;  }
    inline u64 const count()         const noexcept { return capacity();              }
    inline u64 const capacity()      const noexcept { return m_buf->size / sizeof(T); }

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
        N2BREAK(N2Error::UnimplementedCode, "");
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
     *  there are no empty elements in Rings, so all indexes must be
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
        return resizeShiftingLeft(new_capacity);
    }
    inline u64 resizeShiftingLeft(u64 new_capacity) {
        N2BREAK_IF(m_resize == nullptr, N2Error::NullPtr,
                   "Unable to resize ring '{}'; resize function not set",
                   name());

        size_t required_size = Ring<T>::precomputeSize(new_capacity);

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
            N2BREAK_IF(scratch == nullptr, N2Error::System,
                       "Failed to `n2malloc` memory. Godspeed.");

            // Move section B aside.
            memmove(scratch, section_b, size_of_b);

            // Move section A to the front.
            memmove(m_buf->data, section_a, size_of_a);

            // Reinsert section B.
            memmove((m_buf->data + size_of_a), scratch, size_of_b);

            // Perform the resize, and reset the write index.
            m_resize(m_buf, required_size);
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
                N2BREAK_IF(scratch == nullptr, N2Error::System,
                           "Failed to `n2malloc` memory. Godspeed.");

                // Move section B aside.
                memmove(scratch, section_b, bytes_retained_from_b);

                // Move section A to the front.
                memmove(m_buf->data, section_a, size_of_a);

                // Reinsert what's left of section B.
                memmove((m_buf->data + size_of_a),
                        scratch,
                        bytes_retained_from_b);

                // Perform the resize, and reset the write index.
                m_resize(m_buf, required_size);
                write_index() = 0;
            } else {
                // There will not be data retained from section B, so all
                // of the data saved will come from section A. Helpfully,
                // this means we don't have to move data aside.
                size_t bytes_retained_from_a = required_size;

                // Move what's left of section A to the front.
                memmove(m_buf->data, section_a, bytes_retained_from_a);

                // Perform the resize, and reset the write index.
                m_resize(m_buf, required_size);
                write_index() = 0;
            }
        }

        return capacity();
    }

    inline u64 resizeShiftingRight(u64 new_capacity) {
        N2BREAK_IF(m_resize == nullptr, N2Error::NullPtr,
                   "Unable to resize ring '{}'; resize function not set",
                   name());

        size_t required_size = Ring<T>::precomputeSize(new_capacity);

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
            m_resize(m_buf, required_size);

            // Recapture the locations of section_a and section_b; the n2realloc
            // call may have moved the base data pointer.
            section_a = (ptr)((T*)(m_buf->data) + write_index());
            section_b = m_buf->data;

            // Fetch enough scratch space to move section B aside.
            ptr scratch = n2malloc(size_of_b);
            N2BREAK_IF(scratch == nullptr, N2Error::System,
                       "Failed to `n2malloc` memory. Godspeed.");

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

            // Reset the write index to the beginning of the Ring.
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
                N2BREAK_IF(scratch == nullptr, N2Error::System,
                           "Failed to `n2malloc` memory. Godspeed.");

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
                m_resize(m_buf, required_size);
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
                m_resize(m_buf, required_size);
                write_index() = 0;
            }
        }

        return capacity();
    }

    inline u64 resizeAfterDropping(u64 new_capacity) {
        N2BREAK_IF(m_resize == nullptr, N2Error::NullPtr,
                   "Unable to resize ring '{}'; resize function not set",
                   name());

        size_t required_size = Ring<T>::precomputeSize(new_capacity);
        m_resize(m_buf, required_size);

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

        Ring& ring;  /*< The ring being iterated.                  */
        u64   index; /*< The index this iterator is "referencing". */

        iterator(Ring& ring,
                 u64   index = 0) noexcept
            : ring  ( ring  )
            , index ( index ) { }

        inline bool operator==(const iterator& other) const noexcept {
            return &ring == &other.ring && index == other.index;
        }
        inline bool operator!=(const iterator& other) const noexcept {
            return &ring != &other.ring || index != other.index;
        }

        /* Pre-increment -- step forward and return `this`. */
        inline iterator& operator++() noexcept {
            index += 1;
            return *this;
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline iterator operator++(int) noexcept {
            iterator copy = *this;
            index += 1;
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`.
         * Be sure not to iterate past `end()`. */
        inline iterator& operator+=(u64 n) noexcept {
            index = n2min((index + n), ring.capacity());
            return *this;
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline iterator operator+(u64 n) noexcept {
            iterator copy = *this;
            copy += n;
            return copy;
        }

        /* Dereference -- return the current value. */
        inline T& operator*() const noexcept { return ring[index]; }
    };


protected: /*< ## Protected Member Methods */
    inline u64 _increment(u64 index, i64 n = 1) noexcept {
        if (n >= 0) {
            // TODO: Potential divide-by-zero error
            return ((index + n) % capacity());
        } else {
            return decrement(index, -n);
        }
    }

    inline u64 decrement(u64 index, i64 n = 1) noexcept {
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
