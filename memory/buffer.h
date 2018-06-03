/** N2 Memory Buffer
 *  ================
 *  This low level type is uncommon to use directly in practice. Prefer one of
 *  the typed containers, unless you are absolutely sure this is what you need.
 *
 *  TODO: Hide the type_id enum & buffer member behind the DEBUG flag, maybe?
 *        Currently that won't work because container `initialzeBuffer()`
 *        functions check type_id and skip init calls when the types match.
 */

#pragma once

#include <nonstd/nonstd.h>


namespace nonstd {

/** Buffer Descriptor
 *  -----------------
 *  Lightweight description of a memory region freely usable by any platform,
 *  game, or shared code. Most commonly used as the backing store for one of the
 *  memory containers (array, HashTable, Ring, etc.), but also used for
 *  transferring ownership of / sharing information about transient data regions
 *  (scratch memory space, sub-sections of retained buffers, etc.).
 *
 *  In an attempt to maintain encapsulation without creating ambiguous
 *  namespaces, this class is being filled with some type members. It might be
 *  more conceptually sound to be consider those as part of a hypothetical
 *  buffer:: namespace, but this is the language we've got.
 */
struct buffer {

    /** "Namespace" Types
     *  --------------------------------------------------------------------- */
    /** ### Type ID for "Typed" Memory Containers
     *  Used to mark a buffer's memory as initialized to a certain "type" of
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
     *  least the right function signature -- everywhere. The buffer::ResizeFn
     *  type defines that signature.
     */
    using ResizeFn = u64 (*)(buffer * const, u64);


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
     *  Note: The `data` pointer is the first member of the memory buffer. This
     *  allows us to make the mistake of directly casting or dereferencing a
     *  buffer and still receive a valid data handle.
     *  Please use `(ptr)(buf->data)` and not `(ptr)(buf)`, but know that both
     *  will work.
     */
    ptr         data;
    u64         size;
    c_cstr      name;
    UserData    userdata1;
    UserData    userdata2;
    type_id::_t type;

}; ENFORCE_POD(buffer);


/** Make Buffer Helper Function
 *  ---------------------------
 *  Lightweight helper function that allows us to use any data region (ex; those
 *  received from the scratch buffer) as though it were a valid memory buffer.
 *  Buffers created in this way are not added to the platform's buffer map, so
 *  they never persist between frames.
 */
inline buffer make_buffer(ptr p, u64 size, c_cstr name = "transient_buffer") {
    return buffer { p, size, name, { 0 }, { 0 }, buffer::type_id::raw };
}


/** Print Overloads
 *  ===============
 *  `"buffer{ 'buffer/name' }(0x123456789abc)"`
 */
inline std::ostream& operator << (std::ostream & s, buffer const * const buf) {
    return s << "buffer{{'{}'}}"_format(buf->name);
}
inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       buffer const * const buf) {
    f.writer().write("buffer{{'{}'}}", buf->name);
}

} /* namespace nonstd */
