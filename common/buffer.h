#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"

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
class Buffer {
protected:
    GameState        *m_state;
    BufferDescriptor  m_bd;
public:
    Buffer(BufferDescriptor bd)
        : m_state(nullptr)
        , m_bd(bd) { }
    Buffer(GameState& state, cstr name)
        : m_state(&state)
        , m_bd(state.memory.lookup(state.memory.map, name)) { }

    void resize(u64 bytes) {
        if (m_state) {
            m_bd = m_state->memory.resize(m_state->memory.map, bytes);
        } else {
            BREAKPOINT;
        }
    }

    /* Get a pointer to `count` consecutive elements in the view, resizing
       if necessary. No initialization is done on this data. */
    inline T* consume(u64 count=1) {
        if (((T*)m_bd.cursor) + count + 1 <= ((T*)m_bd.data) + m_bd.size) {
            resize(sizeof(T) * m_bd.size);
        } else {
            resize((m_bd.size + count + 1) * ceil(m_bd.size * 0.2f));
        }
        T* ret = (T*)m_bd.cursor;
        m_bd.cursor = (void*) ( ((T*)m_bd.cursor) + count );
        return ret;
    }

    /* Push a value on the back of the Buffer */
    inline T& push(T value) {
        T* mem = consume(1);
        *mem = value;
        return *mem;
    }
    inline T& push_back(T value) { return push(value); }

    /* Construct a value in place at the back of the buffer */
    template<typename ...ctor_arg_types>
    inline T& emplace_back(ctor_arg_types && ... _ctor_args) {
        T* mem = consume(1);
        return *(::new (mem)
                      T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    /* Construct a value in place at any location in the buffer */
    template<typename ...ctor_arg_types>
    inline T& emplace(u64 offset, ctor_arg_types && ... _ctor_args) {
        if (offset > m_bd.size) {
            resize((offset * sizeof(T) - m_bd.size) * 1.2f);
            return emplace(offset, _ctor_args);
        }
        return *(::new (((T*)m_bd.cursor) + offset)
                      T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    /* Iterator access to support range-based for */
    inline T* begin(void) const { return (T*)m_bd.data; }
    inline T* end(void) const { return (T*)m_bd.cursor; }
    inline T* buffer_end(void) const {
        return (T*)((u8*)m_bd.data + m_bd.size);
    }

    // General-purpose map over all active elements
    inline void map(std::function<void(T&)> fn) {
        for (T* iter = begin(); iter != end(); ++iter) {
            fn(*iter);
        }
    }

    /* Drop all elements of the region without reinitializing memory.
       NB: this forwards to _drop; visual studio chokes on enable_if directly */
    inline void drop() {
        m_bd.cursor = m_bd.data;
    }

    inline u64 size() { return m_size; }
};
