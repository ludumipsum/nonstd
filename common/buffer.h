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
    c_cstr      name;

#if !defined(NDEBUG) || defined(DEBUG)
    BufferDescriptor() = default;
    BufferDescriptor(void* data, void* cursor, u64 size,
                     BufferFlags flags, c_cstr name)
        : data   ( data   )
        , cursor ( cursor )
        , size   ( size   )
        , flags  ( flags  )
        , name   ( name   ) { }
protected:
    friend BufferDescriptor make_buffer(void*, u64, BufferFlags);
    BufferDescriptor(BufferDescriptor const &) = default;
    BufferDescriptor(BufferDescriptor &&) = default;
#else
    ENFORCE_POD(BufferDescriptor);
#endif
};

inline BufferDescriptor make_buffer(void* ptr,
                                    u64 size,
                                    BufferFlags flags = BUFFER_CLEAR_CURSOR) {
    return BufferDescriptor { ptr, ptr, size, flags, "transient_buffer" };
}
