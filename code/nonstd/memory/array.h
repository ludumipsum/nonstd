/** Array
 *  =====
 *  Arrays present a typed, std::vector-like abstraction over Buffers, allowing
 *  their use as iterable containers of a given type.
 *  Note that resizes may be automatically performed on `consume` and `push`
 *  calls, and that bounds-checking on subscript operators will be performed
 *  when the DEBUG preprocessor symbol is set.
 */

#pragma once

#include <nonstd/cpp1z/type_trait_assertions.h>
#include <nonstd/core/break.h>
#include <nonstd/core/primitive_types.h>
#include <nonstd/c_ish/math.h>

#include "buffer.h"
#include "core_functions.h"


namespace nonstd {

template<typename T>
class Array {
public: /*< ## Class Methods */
    static constexpr u64 default_capacity = 64;

    static constexpr u64 precomputeSize(u64 capacity = default_capacity)
    noexcept {
        return sizeof(T) * capacity;
    }

    static inline Buffer * initializeBuffer(Buffer *const buf) {
        N2BREAK_IF(buf->type == Buffer::type_id::array,
                   N2Error::DoubleInitialization,
                   "Buffer corruption detected by type_id; Buffer has already "
                   "been correctly initialized as an Array.\n"
                   "Underlying buffer is named '{}', and it is located at {}.",
                   buf->name, buf);
        N2BREAK_IF(buf->type != Buffer::type_id::raw,
                   N2Error::InvalidMemory,
                   "Buffer corruption detected by type_id; Attempting to "
                   "initialize a previously initialized Buffer. type_id is "
                   "currently 0x{:X}\n"
                   "Underlying buffer is named '{}', and it is located at {}.",
                   buf->type, buf->name, buf);

        buf->type = Buffer::type_id::array;

        return buf;
    }


protected: /*< ## Protected Member Variables */
    Buffer * m_buf;

public: /*< ## Ctors, Detors, and Assignments */
    Array(Buffer * buf) noexcept
        : m_buf ( buf )
    {
        /* Ensure that only POD types are used by placing ENFORCE_POD here. */
        ENFORCE_POD(T);

        /* Verify `buf` has been correctly initialized. */
        N2BREAK_IF(m_buf->type != Buffer::type_id::array,
            N2Error::InvalidMemory,
            "Array corruption detected by type_id; Buffer has not been "
            "initialized as an Array.\n"
            "type_id is currently 0x{:X}\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            m_buf->type, m_buf->name, m_buf);
    }
    Array(c_cstr name, u64 min_capacity = default_capacity)
        : Array ( memory::find(name)
                ? *memory::find(name)
                : initializeBuffer(
                        memory::allocate(name, precomputeSize(min_capacity))
                    )
                )
    {
        if (capacity() < min_capacity) {
            resize(min_capacity);
        }
    }

public: /*< ## Public Memebr Methods */
    /* ## Buffer Accessors */
    inline Buffer       * const buffer()       noexcept { return m_buf;       }
    inline Buffer const * const buffer() const noexcept { return m_buf;       }
    inline u64                  size()   const noexcept { return m_buf->size; }
    inline c_cstr               name()   const noexcept { return m_buf->name; }

    /* ## Array Accessors */
    // Give up the 80 column width for this boilerplate.
    inline u64       & write_index()       noexcept { return m_buf->userdata1.u_int;  }
    inline u64 const & write_index() const noexcept { return m_buf->userdata1.u_int;  }
    inline u64 const   count()       const noexcept { return write_index();           }
    inline u64 const   capacity()    const noexcept { return m_buf->size / sizeof(T); }

    /** Get / Set Methods
     *  -----------------
     */

    /* Push a value on the back of the Buffer */
    inline T& push(T value) {
        T* mem = consume();
        *mem = value;
        return *mem;
    }
    inline T& push_back(T value) { return push(value); }

    /* Get a pointer to `count` consecutive elements in the view, resizing
       if necessary. No initialization is done on this data. */
    inline T* consume(u64 count=1) {
        // Resize if this consume call would stretch past the end of the buffer.
        if (write_index() + count > capacity()) {
            u64 requested_count = (write_index() + count);
            u64 padded_count    = (u64)(1.2f * requested_count);
            u64 clamped_count   = n2max(padded_count, requested_count+1);
            resize(clamped_count);
        }

        // Find the address of the request data region, increment the write
        // index, and return.
        T* ret = (T*)(m_buf->data) + write_index();
        write_index() += count;

        return ret;
    }

    /* Direct index operator. */
    inline T const & operator[](u64 index) const {
#if defined(DEBUG)
        N2BREAK_IF(index >= capacity(), N2Error::OutOfBounds,
            "Array index operation exceeds maximum capacity.\n"
            "Entry {} / {} (capacity is {}).\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            index, (count() > 0 ? std::to_string(count()) : "-"), capacity(),
            m_buf->name, m_buf);
        N2BREAK_IF(index >= count(), N2Error::OutOfBounds,
            "Array index operation exceeds current count.\n"
            "Entry {} / {} (capacity is {}).\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            index, (count() > 0 ? std::to_string(count()) : "-"), capacity(),
            m_buf->name, m_buf);
#endif
        return *((T*)(m_buf->data) + index);
    }
    inline T& operator[](u64 index) {
        return const_cast<T&>(static_cast<const Array<T> &>(*this)[index]);
    }

    /* Drop all elements of the region without reinitializing memory. */
    inline void drop() noexcept {
        write_index() = 0;
    }

    /* Erase a range of objects from this Array.
       This will correctly adjust the mem::Buffer's user data, and correctly
       shift existing data s.t. the contiguity of data remains consistent. */
    inline void erase(T* range_begin, T* range_end) {
#if defined(DEBUG)
        bool begins_before_buffer  = range_begin < begin(),
             ends_before_beginning = range_end   < range_begin,
             ends_after_buffer     = range_end   > end();
        if (begins_before_buffer  ||
            ends_before_beginning ||
            ends_after_buffer)
        {
            N2BREAK(N2Error::OutOfBounds,
                "Erasing invalid index ranges;\n"
                "  begin       : {}\n"
                "  range begin : {}\n"
                "  range end   : {}\n"
                "  end         : {}\n"
                "Underlying buffer is named '{}', and it is located at {}.",
                begin(), range_begin, range_end, end(), m_buf->name, m_buf);
        }
#endif
        memmove(range_begin, range_end, (end() - range_end) * sizeof(T));
        write_index() -= (range_end - range_begin);
    }
    inline void erase(u64 index_begin, u64 index_end = 0) {
        // Poor-man's relative default. If only index_begin is given, erase it.
        if (index_end == 0) { index_end = (index_begin + 1); }
        erase( (T*)(m_buf->data) + index_begin,
               (T*)(m_buf->data) + index_end    );
    }

    inline u64 resize(u64 new_capacity) {
        auto required_size = Array<T>::precomputeSize(new_capacity);
        memory::resize(m_buf, required_size);
        return capacity();
    }

    /* Iterator access to support range-based for */
    inline T* begin(void) const { return (T*)(m_buf->data);                 }
    inline T* end(void)   const { return (T*)(m_buf->data) + write_index(); }
    inline T* buffer_end(void) const {
        return (T*)(m_buf->data + m_buf->size);
    }
};


/** Print Overloads for Array<char>
 *  ============================================================================
 */

/** OStream Insertion Operator
 *  -------------------------- */
inline std::ostream& operator<< (std::ostream & s, Array<char> const & arr) {
    return s.write(&arr[0], arr.count());
}

/** {fmt} args overload
 *  ------------------- */
inline void format_arg(fmt::BasicFormatter<char> & f,
                       c_cstr                    & format_str,
                       Array<char> const         & arr) {
    // NB. The Array<char>'s data is not guaranteed to be null terminated, and
    // fmt writers have no way of accepting a specific `count` of chars. One
    // workaround is to use a std::string conversion.
    f.writer().write("{}", std::string(&arr[0], arr.count()));
}

} /* namespace nonstd */
