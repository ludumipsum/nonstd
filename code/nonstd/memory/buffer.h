/** N2 Memory Buffer
 *  ================
 *  This low level type is uncommon to use directly in practice. Prefer one of
 *  the typed containers, unless you are absolutely sure this is what you need.
 */

#pragma once

#include <nonstd/core/primitive_types.h>


namespace nonstd {
namespace memory {

/** Type ID for "typed" memory containers
 *  -------------------------------------
 *  The type of the enum can be accessed via `nonstd::memory::type_ids::_t`.
 *  Both `std::cout` and `fmt::format` have trouble printing enums typed with
 *  `u8`s and `u16`s (I have... no clue why), so these type IDs have to be >=
 *  32 bits.
 *  Because of the above, we get <= 8 hex digits to play with when coming up
 *  with recognizable hex-words for these types.
 */
namespace type_id {

enum _t : u32 {
    raw        = 0,
    array      = 0xACED,
    hash_table = 0xCAFE,
    ring       = 0xBEEF,
    stream     = 0x57AB,
};

} /* namespace type_id */


/** Buffer Descriptor
 *  -----------------
 *  Lightweight description of a memory region freely usable by any platform,
 *  game, or shared code. Most commonly used as the backing store for a Memory
 *  View (anything in the `mem::view` namespace), but also used for transferring
 *  ownership of or sharing information about transient data regions (scratch
 *  memory space, sub-sections of retained buffers, etc.).
 *
 *  The UserData nested union is designed to allow users or Buffer Views to
 *  persist state data without increasing the size of the stored data region, or
 *  requring an understanding of where in that data region persisted metadata
 *  should be written to / read from.
 *
 *  Note: The `data` pointer is the first member of the Memory Buffer. This
 *  allows us to make the mistake of directly casting or dereferencing a Buffer
 *  and still receive a valid data handle. Please use `(ptr)(buf->data)` and not
 *  `(ptr)(buf)`, but know that both work.
 *
 *  TODO: Hide the "type" ids behind the DEBUG flag, maybe?
 */
struct Buffer {

    union UserData {
        u64     u_int;
        i64     i_int;
        ptrdiff ptr_diff;
    };

    ptr         data;
    u64         size;
    c_cstr      name;
    UserData    userdata1;
    UserData    userdata2;
    type_id::_t type_id;

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
    return Buffer { p, size, name, { 0 }, { 0 } };
}

} /* namespace memory */
} /* namespace nonstd */
