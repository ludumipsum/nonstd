/** Hash Functions
 *  ==============
 *  General-purpose hash functions. You probably already know which one you want.
 */

#pragma once

#include <cstring>

#include "nonstd/core/primitive_types.h"


namespace nonstd {

constexpr inline u64 shift64(u64 key) noexcept;
inline u64  djb2(c_cstr str);
inline void sha1(u8 const * const data, u64 num_bytes, cstr sha_out);


/** nonstd::hash
 *  ------------
 *  Our default string hash is djb2.
 *  Our default numerical hash is shift64.
 *
 *  NB. We overload -- rather than specialize on -- hash to allow us to change
 *  the noexcept specifier.
 *
 *  TODO: Write hash verifier to make sure our resources directory doesn't
 *        produce djb2 collisions on any content or filename.
 *  TODO: Consider passing-through to `std::hash<T>{}(key)` for most operations.
 */
template<typename T>
inline u64 hash(T key);

inline u64 hash(c_cstr key) { return djb2(key);    }

constexpr inline u64 hash(u8  key) noexcept { return shift64(key); }
constexpr inline u64 hash(u16 key) noexcept { return shift64(key); }
constexpr inline u64 hash(u32 key) noexcept { return shift64(key); }
constexpr inline u64 hash(u64 key) noexcept { return shift64(key); }
constexpr inline u64 hash(i8  key) noexcept { return shift64(key); }
constexpr inline u64 hash(i16 key) noexcept { return shift64(key); }
constexpr inline u64 hash(i32 key) noexcept { return shift64(key); }
constexpr inline u64 hash(i64 key) noexcept { return shift64(key); }


/** shift64 Hash
 *  ------------
 *  Integer hash based on bitshifts and xors, taken from
 *  [here](https://gist.github.com/badboy/6267743).
 */
constexpr inline u64 shift64(u64 key) noexcept {
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}


/** DJB2 Hash
 *  ---------
 *  Simple bytestring to 64bit integer hash function. It's blazing fast and
 *  probably won't corrupt your data. Probably.
 */
inline u64 djb2(c_cstr str) {
  u64 hash = 5381;
  i32 c;
  while ((c=*str++))
      hash = ((hash << 5) + hash) + c;
  return hash;
};


/** SHA1
 *  ----
 *  Use this API for slower hashes where you're super freaked out about
 *  collisions or cryptographic manipulation by dictators (hi nsa).
 *
 *  This code is also shamelessly stolen from the internet. It appears to
 *  produce the same hashes as the sha1 binary on the computer I used to steal
 *  it, but I can't vouch for it beyond that. It may contain snakes, dragons,
 *  and stuxnet.
 */

/* Macros to make stolen code build better */

namespace stolen {

#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ true
#endif

#ifdef __BIG_ENDIAN__
#  define SHA_BIG_ENDIAN
#elif defined __LITTLE_ENDIAN__
/* override */
#elif defined __BYTE_ORDER
#  if __BYTE_ORDER__ ==  __ORDER_BIG_ENDIAN__
#    define SHA_BIG_ENDIAN
#  endif
#else // ! defined __LITTLE_ENDIAN__
#  include <endian.h> // machine/endian.h
#  if __BYTE_ORDER__ ==  __ORDER_BIG_ENDIAN__
#    define SHA_BIG_ENDIAN
#  endif
#endif

#define HASH_LENGTH 20
#define BLOCK_LENGTH 64


/* Shamelessly Stolen Types */

typedef struct sha1nfo {
    uint32_t buffer[BLOCK_LENGTH/4];
    uint32_t state[HASH_LENGTH/4];
    uint32_t byteCount;
    uint8_t bufferOffset;
    uint8_t keyBuffer[BLOCK_LENGTH];
    uint8_t innerHash[HASH_LENGTH];
} sha1nfo;


/* Shamelessly Stolen Implementation */

#define SHA1_K0  0x5a827999
#define SHA1_K20 0x6ed9eba1
#define SHA1_K40 0x8f1bbcdc
#define SHA1_K60 0xca62c1d6

inline void sha1_init(sha1nfo *s) {
    s->state[0] = 0x67452301;
    s->state[1] = 0xefcdab89;
    s->state[2] = 0x98badcfe;
    s->state[3] = 0x10325476;
    s->state[4] = 0xc3d2e1f0;
    s->byteCount = 0;
    s->bufferOffset = 0;
}

inline uint32_t sha1_rol32(uint32_t number, uint8_t bits) {
    return ((number << bits) | (number >> (32-bits)));
}

inline void sha1_hashBlock(sha1nfo *s) {
    uint8_t i;
    uint32_t a,b,c,d,e,t;

    a=s->state[0];
    b=s->state[1];
    c=s->state[2];
    d=s->state[3];
    e=s->state[4];
    for (i=0; i<80; i++) {
        if (i>=16) {
            t = s->buffer[(i+13)&15] ^ s->buffer[(i+8)&15] ^ s->buffer[(i+2)&15] ^ s->buffer[i&15];
            s->buffer[i&15] = sha1_rol32(t,1);
        }
        if (i<20) {
            t = (d ^ (b & (c ^ d))) + SHA1_K0;
        } else if (i<40) {
            t = (b ^ c ^ d) + SHA1_K20;
        } else if (i<60) {
            t = ((b & c) | (d & (b | c))) + SHA1_K40;
        } else {
            t = (b ^ c ^ d) + SHA1_K60;
        }
        t+=sha1_rol32(a,5) + e + s->buffer[i&15];
        e=d;
        d=c;
        c=sha1_rol32(b,30);
        b=a;
        a=t;
    }
    s->state[0] += a;
    s->state[1] += b;
    s->state[2] += c;
    s->state[3] += d;
    s->state[4] += e;
}

inline void sha1_addUncounted(sha1nfo *s, uint8_t data) {
    uint8_t * const b = (uint8_t*) s->buffer;
#ifdef SHA_BIG_ENDIAN
    b[s->bufferOffset] = data;
#else
    b[s->bufferOffset ^ 3] = data;
#endif
    s->bufferOffset++;
    if (s->bufferOffset == BLOCK_LENGTH) {
        sha1_hashBlock(s);
        s->bufferOffset = 0;
    }
}

inline void sha1_writebyte(sha1nfo *s, uint8_t data) {
    ++s->byteCount;
    sha1_addUncounted(s, data);
}

inline void sha1_write(sha1nfo *s, c_cstr data, size_t len) {
    for (;len--;) sha1_writebyte(s, (uint8_t) *data++);
}

inline void sha1_pad(sha1nfo *s) {
    // Implement SHA-1 padding (fips180-2 Â§5.1.1)

    // Pad with 0x80 followed by 0x00 until the end of the block
    sha1_addUncounted(s, 0x80);
    while (s->bufferOffset != 56) sha1_addUncounted(s, 0x00);

    // Append length in the last 8 bytes
    sha1_addUncounted(s, 0); // We're only using 32 bit lengths
    sha1_addUncounted(s, 0); // But SHA-1 supports 64 bit lengths
    sha1_addUncounted(s, 0); // So zero pad the top bits
    sha1_addUncounted(s, s->byteCount >> 29); // Shifting to multiply by 8
    sha1_addUncounted(s, s->byteCount >> 21); // as SHA-1 supports bitstreams as well as
    sha1_addUncounted(s, s->byteCount >> 13); // byte.
    sha1_addUncounted(s, s->byteCount >> 5);
    sha1_addUncounted(s, s->byteCount << 3);
}

inline uint8_t* sha1_result(sha1nfo *s) {
    // Pad to complete the last block
    sha1_pad(s);

#ifndef SHA_BIG_ENDIAN
    // Swap byte order back
    int i;
    for (i=0; i<5; i++) {
        s->state[i]=
              (((s->state[i])<<24)& 0xff000000)
            | (((s->state[i])<<8) & 0x00ff0000)
            | (((s->state[i])>>8) & 0x0000ff00)
            | (((s->state[i])>>24)& 0x000000ff);
    }
#endif

    // Return pointer to hash (20 characters)
    return (uint8_t*) s->state;
}

#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5c

inline void sha1_initHmac(sha1nfo *s, const uint8_t* key, int keyLength) {
    uint8_t i;
    memset(s->keyBuffer, 0, BLOCK_LENGTH);
    if (keyLength > BLOCK_LENGTH) {
        // Hash long keys
        sha1_init(s);
        for (;keyLength--;) sha1_writebyte(s, *key++);
        memcpy(s->keyBuffer, sha1_result(s), HASH_LENGTH);
    } else {
        // Block length keys are used as is
        memcpy(s->keyBuffer, key, keyLength);
    }
    // Start inner hash
    sha1_init(s);
    for (i=0; i<BLOCK_LENGTH; i++) {
        sha1_writebyte(s, s->keyBuffer[i] ^ HMAC_IPAD);
    }
}

inline uint8_t* sha1_resultHmac(sha1nfo *s) {
    uint8_t i;
    // Complete inner hash
    memcpy(s->innerHash,sha1_result(s),HASH_LENGTH);
    // Calculate outer hash
    sha1_init(s);
    for (i=0; i<BLOCK_LENGTH; i++) sha1_writebyte(s, s->keyBuffer[i] ^ HMAC_OPAD);
    for (i=0; i<HASH_LENGTH; i++) sha1_writebyte(s, s->innerHash[i]);
    return sha1_result(s);
}

} /* namespace stolen */


/* Hash `num_bytes` from `data` into 41 bytes at sha_out. This is done by
 * representing the 20-byte sha1 as 40 bytes in the ASCII range -- it's the same
 * format you get from git, for example.
 *
 * TODO: Use a more flexible string representation to return the hash instead of
 *       the c-style unowned-return-pointer-as-parameter. */
inline void sha1(u8 const*const data, u64 num_bytes, cstr sha_out) {
    using namespace stolen;
    sha1nfo si;
    u8* bin_hash;
    sha1_init(&si);
    sha1_write(&si, (c_cstr)data, num_bytes);
    bin_hash = sha1_result(&si);
    for (u8 i=0; i<20; ++i) {
        sprintf((cstr)sha_out+(2*i), "%02x", bin_hash[i]);
    }
    sha_out[40] = '\0';
}

} /* namespace nonstd */
