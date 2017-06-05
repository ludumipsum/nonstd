/** Buffer Descriptors (aka `Buffers`)
 *  ==================================
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"


namespace mem {

/** Buffer Clear Flags
 *  ------------------
 *  Mode flags for controlling buffer clear behavior between frames.
 *
 *  If the Buffer is expected to be used in a Buffer View, `CLEAR_DATA` will
 *  likely be detrimental (`.drop()` should be explicitly called instead to
 *  retain metadata), and `CLEAR_CURSOR` is likely to be meaningless. As such,
 *  neither should be passed.
 *
 *  NB. Unless buffer::PERSIST is set, there is _no guarantee_ that data will be
 *      retained between frames.
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


/** Buffer Descriptor
 *  -----------------
 *  Lightweight description of a memory region freely usable by any platform,
 *  game, or shared code.
 *
 *  The `cursor` can be considered user data. If code is directly modifying
 *  Buffer data, the `cursor` should be used to track written data as it will be
 *  cleared if buffer data is cleared between frames (or if `CLEAR_CURSOR` is
 *  set). If Buffer Views are used, the cursor might not be used.
 *
 *  Note that the `data` pointer is the first member of the `Descriptor`. This
 *  allows us to make the mistake of directly casting or dereferencing the
 *  `Descriptor` and still receive a valid data handle.
 *  Please use `(ptr)(bd->data)` and not `(prt)(bd)`, but know that both work.
 */
struct Buffer {
    ptr    data;
    ptr    cursor;
    u64    size;
    Flags  flags;
    c_cstr name;
}; ENFORCE_POD(Buffer);


/** Resize Function Signature
 *  -------------------------
 *  The mem::resize function gets use in and passed to a broad set of contexts.
 *  As such, it's very important we use the right signature, everywhere. The
 *  mem::ResizeFn type defines that signature.
 */
using ResizeFn = u64 (*)(Buffer * const, u64);


/** Lightweight helper function that allows us to use any data region (ex; those
 *  received from the Scratch Buffer) as though it were a valid `Descriptor`.
 *  Descriptors created in this way are not added to the platform's Buffer Map,
 *  so nor do they persist between frames.
 */
inline Buffer makeBuffer(ptr p, u64 size, Flags flags = CLEAR_CURSOR) {
    return Buffer { p, p, size, flags, "transient_buffer" };
}

} /* namespace buffer */
