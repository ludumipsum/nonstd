/* N2 Buffers
 * ==========
 *
 * TODO: These docs are out of date. Make sure they're made correct.
 *
 * BufferDescriptors provide descriptions of memory locations that are available
 * to be used by game code but created and maintained by the platform layer.
 * Game code requests buffers from the platform layer via `state.memory.create`
 * or `state.memory.lookup`, both of which yield a pointer to a BufferDescriptor
 * created by the platform layer. Game code can then either directly modify the
 * data location described by the BufferDescriptor, or create a typed BufferView
 * over it to operate on individual objects.
 *
 * NB. As there is no real guarantee of what the platform layer will or won't do
 * to your game's pointers between frames -- or even if your game state is
 * actually "between" frames; think of state loads, A-B repeats, state shared
 * across networks, etc. -- only Trivially Copyable data should be stored in
 * retained buffers. BufferDescriptor existence (name, properties, and presence
 * in the platform's lookup table) will remain valid between frames but there is
 * _never_ a guarantee that the same memory location will be retained between
 * frames.
 *
 * You can play as fast and as loose with pointers within a frame, but expect
 * last frame's pointers, and pointers to other data locations, will be in
 * "interesting" states by the time the next frame starts.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"


namespace buffer {

/**
 * Buffer Clear Flags
 * ------------------
 * Mode flags for controlling buffer clear behavior between frames.
 * NB. Unless buffer::PERSIST is set, there is _no guarantee_ that data will be
 *     retained between frames.
 */
enum Flags {
    /* Default behavior — pass the buffer unaltered to the next frame */
    PASS         = (0     ),
    /* The buffer's cursor shall be set to 0 between every frame. */
    CLEAR_CURSOR = (1 << 0),
    /* The buffer's data shall be cleared to 0 between every frame. */
    CLEAR_DATA   = (1 << 1),
    /* The buffer shall be persisted between frames, and previous frame's data
       shall be accessible for... some number of frames?
       TODO: Figure out how long we retain persisted buffers. */
    PERSIST      = (1 << 2),
};


/**
 * Buffer Descriptor
 * -----------------
 *
 * Badass thing that does badass things.
 * TODO: Real docs, Drew.
 */
struct Descriptor {
    ptr    data;
    ptr    cursor;
    u64    size;
    Flags  flags;
    c_cstr name;
}; ENFORCE_POD(Descriptor);


/**
 * `make_buffer` Helper Function.
 * TODO: talk about making buffer from arbitrary data regions.
 */
inline Descriptor make_buffer(ptr p, u64 size,
                              Flags flags = CLEAR_CURSOR) {
    return Descriptor { p, p, size, flags, "transient_buffer" };
}

} /* namespace buffer */

typedef buffer::Descriptor Buffer;

typedef u64 (*BufferResizeFn) (Buffer *const, u64);
