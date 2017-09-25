/** FOURCC Construction Shorthand
 *  =============================
 *  Many data file formats, especially microsoft ones, use four-character-codes
 *  to identify segments. These are represented as a 32-bit integer with the
 *  value of four adjacent ASCII characters.
 */

#pragma once

#include "nonstd/core/primitive_types.h"


namespace nonstd {

constexpr inline u32 four_char_code(u8 a, u8 b, u8 c, u8 d) noexcept {
    return (u32)( (u32(d) << 24 )
                | (u32(c) << 16 )
                | (u32(b) <<  8 )
                | (u32(a)       ));
}
constexpr inline u32 four_char_code(c_cstr code) {
    return (u32)( (u32(code[3]) << 24)
                | (u32(code[2]) << 16)
                | (u32(code[1]) <<  8)
                | (u32(code[0])      ));
}

} /* namespace nonstd */
