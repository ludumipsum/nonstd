/* Typed Buffer Slice
 * ==================
 *
 * Buffer Slices provide simple, std::array-like abstraction over game memory
 * buffers, allowing their use as iterable containers of a given type. Since
 * game buffers are not stored with a type, take some care to not use slices of
 * different types over the same buffer, or you're likely to get
 * "interesting" results.
 *
 * Do not retain a buffer slice across frames unless you really know what you're
 * trying to accomplish by doing that --- it will, _at best_  refer to a buffer
 * backed by a previous frame's state, which is unlikely to be quite what
 * you want.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "mathutils.h"
#include "logging.h"

#include "api.h"
#include "buffer.h"


namespace buffer {

template<typename T>
class Slice {
public:
    inline static u64 precomputeSize(u64 count) {
        return (sizeof(T) * count);
    }

protected:
    Descriptor *const m_bd;
    BufferResizeFn    m_resize;

public:
    Slice(Descriptor *const bd, BufferResizeFn resize = nullptr)
        : m_bd     ( bd      )
        , m_resize ( resize  ) { }

    inline void resize(u64 size_bytes) {
        if (m_resize) {
            m_resize(m_bd, size_bytes);
        } else {
            BREAKPOINT();
        }
    }

    /* Get a pointer to `count` consecutive elements in the view, resizing
     * if necessary. No initialization is done on this data. */
    inline T* consume(u64 count=1) {
        // Compute the buffer endpoint, and the end of the memory we want
        T *region_end    = (T*)( (u8*)(m_bd->data) + m_bd->size   ),
          *requested_end = (T*)( (T*) (m_bd->cursor) + count );

        // Resize if this consume call would stretch past the end of the buffer.
        if (m_resize && requested_end > region_end) {
            u64 requested_size = m_bd->size + ( sizeof(T) * count );
            u64 padded_size = n2max((u64)(1.2f * requested_size),
                                    requested_size + sizeof(T));
            resize(padded_size);
        }

        // Return the address of the requested amount of space
        T* ret = (T*) m_bd->cursor;
        m_bd->cursor = (ptr) ( ((T*)(m_bd->cursor)) + count );
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
    inline T* begin(void) const      { return (T*)(m_bd->data); }
    inline T* end(void) const        { return (T*)(m_bd->cursor); }
    inline T* buffer_end(void) const {
        return (T*)((u8*)(m_bd->data) + m_bd->size);
    }

    /* Direct index operator. */
    inline T& operator[](u64 index) {
#if defined(DEBUG)
        if (index > count() && index < max_count()) {
            LOG("Accessing uninitialized object in %s at slice[%d].\n"
                "This will invalidate count() and range-based iterators."
                "Please be sure you're remaining within the bounds of consumed "
                "data in this Slice.", m_bd->name, index);
        }
        if (index >= max_count()) {
            LOG("Out of bounds access via buffer::Slice: entry %d is past the "
                "end of the %d-long buffer %s.",
                index, max_count(), m_bd->name);
            BREAKPOINT();
        }
#endif
        return *((T*)(m_bd->data) + index);
    }

    /* Drop all elements of the region without reinitializing memory. */
    inline void drop() {
        m_bd->cursor = m_bd->data;
    }

    /* Erase a range of objects from this Slice.
     * This will correctly adjust the buffer::Descriptor's cursor, and correctly
     * sift existing data s.t. the contiguity of data remains consistent. */
    inline void erase(T* range_begin, T* range_end) {
#if defined(DEBUG)
        bool begins_before_buffer  = range_begin < begin(),
             ends_before_beginning = range_end   < range_begin,
             ends_after_buffer     = range_end   > end();
        if (begins_before_buffer  ||
            ends_before_beginning ||
            ends_after_buffer)
        {
            LOG("Erasing invalid index ranges;\n"
                "  begin       : %p\n"
                "  range begin : %p\n"
                "  range end   : %p\n"
                "  end         : %p",
                begin(), range_begin, range_end, end());
            BREAKPOINT();
        }
#endif
        memmove(range_begin, range_end, end() - range_end);
        m_bd->cursor = range_end;
    }
    inline void erase(u64 index_begin, u64 index_end) {
        erase(((T*)(m_bd->data) + index_begin), ((T*)(m_bd->data) + index_end));
    }

    // TODO: Consider memoizing `count`, `max_count`?
    /* Return the size of the Slice in bytes. */
    inline u64 size()      { return m_bd->size; }
    /* Return the number of objects currently stored in the Slice. */
    inline u64 count()     { return (m_bd->data - m_bd->cursor) / sizeof(T); }
    /* Return the maximum number of objects the Slice may store. */
    inline u64 max_count() { return m_bd->size / sizeof(T); }


    ENFORCE_POD(T);
};

} /* namespace buffer */
