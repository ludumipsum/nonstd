/** N2 Memory Buffer
 *  ================
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"


namespace mem {

/** Buffer Descriptor
 *  -----------------
 *  Lightweight description of a memory region freely usable by any platform,
 *  game, or shared code. Most commonly used as the backing store for a Memory
 *  View (anything in the `mem::view` namespace), but also used for transferring
 *  ownership of or sharing information about transient data regions (scratch
 *  memory space, sub-sections of retained buffers, etc.).
 *
 *  Note that the `data` pointer is the first member of the Memory Buffer. This
 *  allows us to make the mistake of directly casting or dereferencing a Buffer
 *  and still receive a valid data handle. Please use `(ptr)(buf->data)` and not
 *  `(ptr)(buf)`, but know that both work.
 */
struct Buffer {
    ptr    data;
    ptr    cursor;
    u64    size;
    c_cstr name;
}; ENFORCE_POD(Buffer);


/** Resize Function Signature
 *  -------------------------
 *  The mem::resize function gets use in and passed to a broad set of contexts.
 *  As such, it's very important we use the right signature, everywhere. The
 *  mem::ResizeFn type defines that signature.
 */
using ResizeFn = u64 (*)(Buffer * const, u64);


/** Make Buffer Helper Function
 *  ---------------------------
 *  Lightweight helper function that allows us to use any data region (ex; those
 *  received from the Scratch Buffer) as though it were a valid Memory Buffer.
 *  Buffers created in this way are not added to the platform's Buffer Map, so
 *  they never persist between frames.
 */
inline Buffer makeBuffer(ptr p, u64 size, c_cstr name = "transient_buffer") {
    return Buffer { p, p, size, name };
}

} /* namespace buffer */
