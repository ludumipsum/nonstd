#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"

/* Buffer Descriptors
   ==================

   BufferDescriptors provide descriptions of memory locations that are available
   to be used by game code but created and maintained by the platform layer.
   Game code requests buffers from the platform layer via `state.memory.create`
   or `state.memory.lookup`, both of which yield a pointer to a BufferDescriptor
   created by the platform layer. Game code can then either directly modify the
   data location described by the BufferDescriptor, or create a typed BufferView
   over it to operate on individual objects.

   NB. As there is no real guarantee of what the platform layer will or won't do
   to your game's pointers between frames -- or even if your game state is
   actually "between" frames; think of state loads, A-B repeats, state shared
   across networks, etc. -- only Trivially Copyable data should be stored in
   retained buffers. BufferDescriptor existence (name, properties, and presence
   in the platform's lookup table) will remain valid between frames but there is
   _never_ a guarantee that the same memory location will be retained between
   frames.

   You can play as fast and as loose with pointers within a frame, but expect
   last frame's pointers, and pointers to other data locations, will be in
   "interesting" states by the time the next frame starts.
*/

struct BufferDescriptor {
    void*       data;
    void*       cursor;
    u64         size;
    BufferFlags flags;

#if !defined(NDEBUG) || defined(DEBUG)
    BufferDescriptor() = default;
    BufferDescriptor(void* data, void* cursor, u64 size, BufferFlags flags)
        : data   ( data   )
        , cursor ( cursor )
        , size   ( size   )
        , flags  ( flags  ) { }
protected:
    friend BufferDescriptor make_buffer(void*, u64, BufferFlags);
    BufferDescriptor(BufferDescriptor const&) = default;
    BufferDescriptor(BufferDescriptor &&) = default;
#else
    ENFORCE_POD(BufferDescriptor);
#endif
};

inline BufferDescriptor make_buffer(void* ptr,
                                    u64 size,
                                    BufferFlags flags = BUFFER_CLEAR_CURSOR) {
    return BufferDescriptor { ptr, ptr, size, flags };
}

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
    GameState        *m_state;
    BufferDescriptor &m_bd;


public:
    BufferView(BufferDescriptor* bd)
              : m_state ( nullptr )
              , m_bd    ( *bd     ) { }
    BufferView(BufferDescriptor& bd)
              : m_state    ( nullptr )
              , m_bd       ( bd      ) { }
    BufferView(GameState& state, c_cstr name)
              : m_state    ( &state  )
              , m_bd       ( *state.memory.lookup(name) ) { }

    inline void resize(u64 size_bytes) {
        if (m_state) {
            m_state->memory.resize(m_bd, size_bytes);
        } else {
            BREAKPOINT;
        }
    }

    /* Get a pointer to `count` consecutive elements in the view, resizing
       if necessary. No initialization is done on this data. */
    inline T* consume(u64 count=1) {
        if (((T*)m_bd.cursor) + count + 1 > ((T*)m_bd.data) + m_bd.size) {
            resize((m_bd.size + (count + 1) * sizeof(T)) * 0.2f);
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
            return emplace(offset,
                           std::forward<ctor_arg_types>(_ctor_args)...);
        }
        return *(::new (((T*)m_bd.cursor) + offset)
                      T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    /* Iterator access to support range-based for */
    inline T* begin(void) const      { return (T*)m_bd.data; }
    inline T* end(void) const        { return (T*)m_bd.cursor; }
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
    /*FIXME: The above is a lie. Should it be a lie? Should we fix the below? */
    inline void drop() {
        m_bd.cursor = m_bd.data;
    }

    inline u64 size() { return m_bd.size; }
};
