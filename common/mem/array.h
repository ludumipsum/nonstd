/** Typed Array View
 *  ================
 *  Array views present a typed, std::vector-like abstraction over memory
 *  buffers, allowing their use as iterable containers of a given type. Note
 *  that resizes may be automatically performed on `consume` and `push` calls,
 *  and that bounds-checking on subscript operators will be performed in
 *  DEBUG mode.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "mathutils.h"
#include "logging.h"
#include "crash.h"

#include "mem/buffer.h"


namespace mem {
namespace view {

template<typename T>
class Array {
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
    Array(Buffer *const buf, ResizeFn resize = nullptr)
        : m_buf         ( buf                  )
        , m_resize      ( resize               )
        , m_write_index ( buf->userdata1.u_int ) { }


public: /*< ## Public Memeber Methods */
    inline u64    size()     { return m_buf->size;             }
    inline u64    count()    { return m_write_index;           }
    inline u64    capacity() { return m_buf->size / sizeof(T); }
    inline c_cstr name()     { return m_buf->name;             }

    /* Drop all elements of the region without reinitializing memory. */
    inline void drop() {
        m_write_index = 0;
    }

    inline u64 resize(u64 new_capacity) {
#if defined(DEBUG)
        N2CRASH_IF(m_resize == nullptr, N2Error::NullPtr,
            "Attempting to resize a Array that has no associated "
            "resize function.\n"
            "Underlying buffer is named %s, and it is located at %p.",
            m_buf->name, m_buf);
#endif
        auto required_size = Array<T>::precomputeSize(new_capacity);
        m_resize(m_buf, required_size);
        return capacity();
    }

    /* Get a pointer to `count` consecutive elements in the view, resizing
     * if necessary. No initialization is done on this data. */
    inline T* consume(u64 count=1) {
        // Resize if this consume call would stretch past the end of the buffer.
        if (m_write_index + count > capacity()) {
            u64 requested_count = (m_write_index + count);
            u64 padded_count    = (u64)(1.2f * requested_count);
            u64 clamped_count   = n2max(padded_count, requested_count+1);
            resize(clamped_count);
        }

        // Find the address of the request data region, increment the write
        // index, and return.
        T* ret = (T*)(m_buf->data) + m_write_index;
        m_write_index += count;

        return ret;
    }

    /* Push a value on the back of the Buffer */
    inline T& push(T value) {
        T* mem = consume();
        *mem = value;
        return *mem;
    }
    inline T& push_back(T value) { return push(value); }

    /* Iterator access to support range-based for */
    inline T* begin(void) const { return (T*)(m_buf->data);                 }
    inline T* end(void) const   { return (T*)(m_buf->data) + m_write_index; }
    inline T* buffer_end(void) const {
        return (T*)(m_buf->data + m_buf->size);
    }

    /* Direct index operator. */
    inline T& operator[](u64 index) {
#if defined(DEBUG)
        N2CRASH_IF(index >= capacity(), N2Error::OutOfBounds,
            "Entry " Fu64 " / " Fu64 ".\n"
            "Underlying buffer is named %s, and it is located at %p.",
            index, capacity()-1, m_buf->name, m_buf);
        N2CRASH_IF(index >= count(), N2Error::OutOfBounds,
            "Entry " Fu64 " / " Fu64 " (of " Fu64 ").\n"
            "Underlying buffer is named %s, and it is located at %p.",
            index, count()-1, capacity()-1, m_buf->name, m_buf);
#endif
        return *((T*)(m_buf->data) + index);
    }

    /* Erase a range of objects from this Array.
     * This will correctly adjust the mem::Buffer's user data, and correctly
     * shift existing data s.t. the contiguity of data remains consistent. */
    inline void erase(T* range_begin, T* range_end) {
#if defined(DEBUG)
        bool begins_before_buffer  = range_begin < begin(),
             ends_before_beginning = range_end   < range_begin,
             ends_after_buffer     = range_end   > end();
        if (begins_before_buffer  ||
            ends_before_beginning ||
            ends_after_buffer)
        {
            N2CRASH(N2Error::OutOfBounds,
                "Erasing invalid index ranges;\n"
                "  begin       : %p\n"
                "  range begin : %p\n"
                "  range end   : %p\n"
                "  end         : %p\n",
                "Underlying buffer is named %s, and it is located at %p.",
                begin(), range_begin, range_end, end(), m_buf->name, m_buf);
        }
#endif
        memmove(range_begin, range_end, end() - range_end);
        m_write_index -= (range_end - range_begin) / sizeof(T);
    }
    inline void erase(u64 index_begin, u64 index_end) {
        erase( (T*)(m_buf->data) + index_begin,
               (T*)(m_buf->data) + index_end    );
    }


    /* Ensure that only POD data is used in these views.*/
    ENFORCE_POD(T);
};

} /* namespace view */
} /* namespace mem  */
