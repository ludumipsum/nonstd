#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"
#include "buffer.h"

/* Typed Buffer Views
   ==================

   BufferViews provide a simple abstraction over game memory buffers, allowing
   their use as iterable containers of a given type. Since game buffers are
   not stored with a type, take some care to not use buffer views of different
   types over the same buffer, or you're likely to get "interesting" results.

   Do not retain a buffer view across frames unless you really know what you're
   trying to accomplish by doing that -- it will refer to a buffer backed by a
   previous frame's state, which is unlikely to be quite what you want.
*/

template<typename T>
class BufferView {
protected:
    GameState        *      m_state;
    BufferDescriptor *const m_bd;

public:
    BufferView(BufferDescriptor *const bd)
              : m_state ( nullptr )
              , m_bd    ( bd      ) { }
    BufferView(GameState& state, c_cstr name)
              : m_state    ( &state  )
              , m_bd       ( state.memory.lookup(name) ) { }

    inline void resize(u64 size_bytes) {
        if (m_state) {
            m_state->memory.resize(m_bd, size_bytes);
        } else {
            BREAKPOINT();
        }
    }

    /* Get a pointer to `count` consecutive elements in the view, resizing
       if necessary. No initialization is done on this data. */
    inline T* consume(u64 count=1) {
        // Compute the buffer endpoint, and the end of the memory we want
        T *region_end    = (T*)( (u8*) m_bd->data + m_bd->size   ),
          *requested_end = (T*)( (T*)  m_bd->cursor + count );

        // Resize if this consume call would stretch past the end of the buffer
        if (requested_end > region_end) {
            u64 requested_size = m_bd->size + ( sizeof(T) * count );
            u64 padded_size = n2max((u64)(1.2f * requested_size),
                                    requested_size + sizeof(T));
            resize(padded_size);
        }

        // Return the address of the requested amount of space
        T* ret = (T*) m_bd->cursor;
        m_bd->cursor = (void*) ( ((T*) m_bd->cursor) + count );
        return ret;
    }

    /* Push a value on the back of the Buffer */
    inline T& push(T value) {
        T* mem = consume();
        *mem = value;
        return *mem;
    }
    inline T& push_back(T value) { return push(value); }

    inline T& push_ring(T value) {
        auto buffer_start   = (u8*)m_bd->data,
             buffer_end     = (u8*)m_bd->cursor;
        u64  currently_used = buffer_end - buffer_start;
        u64  currently_free = m_bd->size - currently_used;
        if (currently_free < sizeof(T)) {
            m_bd->cursor = m_bd->data;
        }
        return push(value);
    }

    /* Construct a value in place at the back of the buffer */
    template<typename ...ctor_arg_types>
    inline T& emplace_back(ctor_arg_types && ... _ctor_args) {
        T* mem = consume();
        return *(::new (mem) T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    /* Construct a value in place at any location in the buffer */
    template<typename ...ctor_arg_types>
    inline T& emplace(u64 offset, ctor_arg_types && ... _ctor_args) {
        if (offset > m_bd->size) {
            resize((offset * sizeof(T) - m_bd->size) * 1.2f);
            return emplace(offset, std::forward<ctor_arg_types>(_ctor_args)...);
        }
        return *(::new (((T*)m_bd->cursor) + offset)
                      T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    /* Iterator access to support range-based for */
    inline T* begin(void) const      { return (T*)m_bd->data; }
    inline T* end(void) const        { return (T*)m_bd->cursor; }
    inline T* buffer_end(void) const {
        return (T*)((u8*)m_bd->data + m_bd->size);
    }

    // General-purpose map over all active elements
    inline void map(std::function<void(T&)> fn) {
        for (T* iter = begin(); iter != end(); ++iter) {
            fn(*iter);
        }
    }

    /* Drop all elements of the region without reinitializing memory.
       NB: this forwards to _drop; visual studio chokes on enable_if directly */
    /*FIXME: The above is a lie. Should it be a lie? Should we fix the below? */
    inline void drop() {
        m_bd->cursor = m_bd->data;
    }

    inline void erase(T* range_begin, T* range_end) {
        if(range_end <= end()) abort();
        if(range_begin >= begin()) abort();
        memmove(range_begin, range_end, end() - range_end);
        m_bd->cursor = range_end;
    }

    inline u64 size() { return m_bd->size; }
};
