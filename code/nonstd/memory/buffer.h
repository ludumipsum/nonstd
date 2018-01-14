/** N2 Memory Buffer
 *  ================
 *  This low level type is uncommon to use directly in practice. Prefer one of
 *  the typed containers, unless you are absolutely sure this is what you need.
 *
 *  TODO: Hide the type_id enum & Buffer member behind the DEBUG flag, maybe?
 *        Currently that won't work because container `initialzeBuffer()`
 *        functions check type_id and skip init calls when the types match.
 */

#pragma once

#include <nonstd/type_traits_ext.h>
#include <nonstd/core/primitive_types.h>


namespace nonstd {

/** Buffer Descriptor
 *  -----------------
 *  Lightweight description of a memory region freely usable by any platform,
 *  game, or shared code. Most commonly used as the backing store for one of the
 *  memory containers (Array, HashTable, Ring, etc.), but also used for
 *  transferring ownership of / sharing information about transient data regions
 *  (scratch memory space, sub-sections of retained buffers, etc.).
 *
 *  In an attempt to maintain encapsulation without creating ambiguous
 *  namespaces, this class is being filled with some type members. It might be
 *  more conceptually sound to be consider those as part of a hypothetical
 *  buffer:: namespace, but this is the language we've got.
 */
struct Buffer {

    /** "Namespace" Types
     *  --------------------------------------------------------------------- */
    /** ### Type ID for "Typed" Memory Containers
     *  Used to mark a Buffer's memory as initialized to a certain "type" of
     *  container. We use hex-words s.t. this can (hopefully) be both a
     *  programmatic and human-readable check.
     *
     *  Both `std::cout` and `fmt::format` have trouble printing enums typed with
     *  `u8`s and `u16`s on GCC and Clang (But not MSVC? I have... no clue why),
     *  so these type IDs have to be >= 32 bits. As such, we get <= 8 hex digits
     *  to play with when coming up with recognizable hex-words for these types.
     */
    struct type_id {
        enum _t : u32 {
            raw          = 0,
            array        = 0xACED,
            single_value = 0xBABE,
            hash_table   = 0xCAFE,
            ring         = 0xBEEF,
            stream       = 0x57AB,
        };
    };


    /** ### Resize Function Signature
     *  This resize function gets use by and passed into a broad set of
     *  contexts. As such, it's very important we use the function -- or at
     *  least the right function signature -- everywhere. The Buffer::ResizeFn
     *  type defines that signature.
     */
    using ResizeFn = u64 (*)(Buffer * const, u64);


    /** "Actual" Buffer Members
     *  --------------------------------------------------------------------- */
    /** ### User Data
     *  The UserData nested-union is designed to allow users or containers  to
     *  persist state (meta)data without increasing the size of the stored data
     *  region, or requiring an understanding of where in that data region
     *  persisted metadata should be written to / read from.
     */
    union UserData {
        u64     u_int;
        i64     i_int;
        ptrdiff ptr_diff;
    };

    /** ### Buffer Members
     *  Note: The `data` pointer is the first member of the Memory Buffer. This
     *  allows us to make the mistake of directly casting or dereferencing a
     *  Buffer and still receive a valid data handle.
     *  Please use `(ptr)(buf->data)` and not `(ptr)(buf)`, but know that both
     *  will work.
     */
    ptr         data;
    u64         size;
    c_cstr      name;
    UserData    userdata1;
    UserData    userdata2;
    type_id::_t type;

}; ENFORCE_POD(Buffer);


/** Make Buffer Helper Function
 *  ---------------------------
 *  Lightweight helper function that allows us to use any data region (ex; those
 *  received from the Scratch Buffer) as though it were a valid Memory Buffer.
 *  Buffers created in this way are not added to the platform's Buffer Map, so
 *  they never persist between frames.
 */
inline Buffer makeBuffer(ptr p, u64 size, c_cstr name = "transient_buffer") {
    return Buffer { p, size, name, { 0 }, { 0 }, Buffer::type_id::raw };
}

} /* namespace nonstd */
