/* N2 Buffers
 * ==========
 * Memory management is hard. Memory management in a hot-loading, code-swapping,
 * volatile-memory, context is more so.
 *
 * N2's solution is to allocate/deallocate memory ('Buffers') from exactly one
 * module in the platform layer (the platform::mem module, probably surprising
 * everyone), and provide handles to that data via the below
 * `buffer::Descriptor` structure.
 *
 * The retention pattern for any given Buffer is defined at create time (and
 * cannot be modified?) using the below `buffer::Flags`. The default behavior
 * for buffers is to make _no guarantee that data will be saved between frames_.
 * Once a Buffer is created, it will always be accessible via `lookup` but --
 * unless the `PERSIST` flag is set -- the data stored in that buffer might not
 * be accessible once a frame boundary is reached.
 *
 * Game code may `sate.memory.create/3` and `state.memory.lookup/1` named memory
 * regions to receive a `Descriptor` from platform code. That data may then be
 * directly modified via the `Descriptor->data` pointer, or a typed 'Buffer
 * View' may be overlaid on top of the `Descriptor` to allow interaction with
 * individual objects.
 *
 * NB. As there is no real guarantee of what the platform layer will or won't do
 * to your data pointers between frames -- or even if your game state is
 * actually "between" frames; think of state loads, A-B repeats, state shared
 * across networks, etc. -- only Trivially Copyable data (not including
 * pointers) should be stored in retained buffers. Descriptor existence (name,
 * properties, and presence in the platform's lookup table) will remain valid
 * between frames but there is _never_ a guarantee that the same memory location
 * will be retained between frames.
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
 *
 * If the Buffer is expected to be used in a Buffer View, `CLEAR_DATA` will
 * likely be detrimental (`.drop()` should be explicitly called instead to
 * retain metadata), and `CLEAR_CURSOR` is likely to be meaningless. As such,
 * neither should be passed.
 *
 * NB. Unless buffer::PERSIST is set, there is _no guarantee_ that data will be
 *     retained between frames.
 */
enum Flags {
    /* Default behavior — pass the buffer unaltered to the next frame */
    PASS         = (0     ),
    /* The buffer's data shall be cleared to 0 between every frame. */
    CLEAR_DATA   = (1 << 0),
    /* The buffer's cursor shall be set to 0 between every frame. */
    CLEAR_CURSOR = (1 << 1),
    /* The buffer shall be persisted between frames, and previous frame's data
     * shall be accessible for... some number of frames? */
    /* TODO: Figure out how long we retain persisted buffers. */
    PERSIST      = (1 << 2),
};


/**
 * Buffer Descriptor
 * -----------------
 * Lightweight description of a memory region freely usable by any platform,
 * game, or shared code.
 *
 * The `cursor` can be considered user data. If code is directly modifying
 * Buffer data, the `cursor` should be used to track written data as it will be
 * cleared if buffer data is cleared between frames (or if `CLEAR_CURSOR` is
 * set). If Buffer Views are used, the cursor might not be used.
 *
 * Note that the `data` pointer is the first member of the `Descriptor`. This
 * allows us to make the mistake of directly casting or dereferencing the
 * `Descriptor` and still receive a valid data handle.
 * Please use `(ptr)(bd->data)` and not `(prt)(bd)`, but know that both work.
 */
struct Descriptor {
    ptr    data;
    ptr    cursor;
    u64    size;
    Flags  flags;
    c_cstr name;
}; ENFORCE_POD(Descriptor);


/**
 * `make_buffer`
 * Lightweight helper function that allows us to use any data region (ex; those
 * received from the Scratch Buffer) as though it were a valid `Descriptor`.
 * Descriptors created in this way are not added to the platform's Buffer Map,
 * so nor do they persist between frames.
 */
inline Descriptor make_buffer(ptr p, u64 size, Flags flags = CLEAR_CURSOR) {
    return Descriptor { p, p, size, flags, "transient_buffer" };
}

} /* namespace buffer */


typedef buffer::Descriptor Buffer;

typedef u64 (*BufferResizeFn) (Buffer *const, u64);
