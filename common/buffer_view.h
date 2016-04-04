#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

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
    using T_PTR = T*;
public:
    T_PTR & m_data;
    T_PTR & m_cursor;
    u64  & m_size;

    // Allows resizing the underlying buffer on demand
    GameState*  m_originating_state;
    char const* m_name;

public:
    inline BufferView(GameBuffer& buffer)
        : m_data              ( (T_PTR&)buffer.data   )
        , m_cursor            ( (T_PTR&)buffer.cursor )
        , m_size              ( buffer.size           )
        , m_originating_state ( buffer.game_state     )
        , m_name              ( buffer.name           ) { }
    BufferView() = default;
    ~BufferView() = default;
    BufferView(BufferView const& other) = default;
    BufferView(BufferView&& other) = default;

    inline BufferView& operator=(GameBuffer buf) {
        *this = BufferView(buf); return *this;
    }

    /* Resize the underlying buffer */
    inline void resize(u64 new_size) {
        GameData& gd = m_originating_state->data;
        gd.resize(gd._backing_store, m_name, new_size);
        m_size = new_size;
    }

    /* Get a pointer to `count` consecutive elements in the view, resizing
       if necessary. No initialization is done on this data. */
    inline T* consume(u64 count=1) {
        if (count + m_cursor > m_data + m_size) {
            resize(m_size + count + 1);
        }
        T* ret = m_cursor;
        m_cursor += count;
        return ret;
    }

    /* Add a new T at the end of the buffer */
    inline T& push(T const& value) {
        T* mem = consume(1);
        *mem = value;
        return *mem;
    }
    inline T& push_back(T const& value) { return push(value); }
    /* Move a T on to the end of the buffer */
    inline T& push(T && value) {
        T* mem = consume(1);
        *mem = std::move(value);
        return *mem;
    }
    inline T& push_back(T && value) { return push(value); }

    /* Construct a T in place at the end of the buffer */
    template<typename ...ctor_arg_types>
    inline T& emplace_back(ctor_arg_types && ... _ctor_args) {
        if (m_cursor >= m_data + m_size) {
            resize(m_size + (u64) ceil(m_size * 0.2f));
        }
        return *(::new (m_cursor++)
                      T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    /* Construct a T in place at any location in the buffer */
    template<typename ...ctor_arg_types>
    inline T& emplace(u64 offset, ctor_arg_types && ... _ctor_args) {
        if (offset > m_size) {
            CRASH(EINVAL, "Error emplacing in a BufferView: requested "
                          "offset %" PRIu64 " is past the end of the buffer "
                          "(length %" PRIu64 ")",
                          offset, m_size);
        }
        return *(::new (m_data + offset)
                      T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    // #### Access, Query, and Drop
    // Random-access lookup within the region
    inline T& operator[](uint64_t index) const {
        if (index >= m_size) {
            CRASH(EFAULT, "Buffer view bounds-check failed; %" PRIu64
                          " is outside the %" PRIu64 " long buffer.",
                          index, m_size);
        }
        return m_data[index];
    }

    // Iterator access to support range-based for
    inline T* begin(void) const { return m_data; }
    inline T* end(void) const { return m_cursor; }
    inline T* buffer_end(void) const { return m_data + m_size; }

    // General-purpose map over all active elements
    inline void map(std::function<void(T&)> fn) {
        for (T* iter = begin(); iter != end(); ++iter) {
            fn(*iter);
        }
    }

    /* Drop all elements of the region without reinitializing memory.
     * NB: this forwards to _drop; visual studio chokes on enable_if directly */
    inline void drop() { _drop(); }
    TEMPLATE_ENABLE(std::is_pod<T>::value, T)
    inline void _drop() {
        m_cursor = m_data;
    }

    TEMPLATE_ENABLE(!std::is_pod<T>::value, T)
    inline void _drop() {
        for (auto& entry : *this) {
            entry.~T();
        }
        m_cursor = m_data;
    }

    /* Get the number of elements the buffer can hold */
    inline u64 size() { return m_size; }
};
