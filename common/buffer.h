/* N2 Buffers
 * ==========
 * Memory management is hard. Memory management in a hot-loading, code-swapping,
 * volatile-memory, context is more so.
 *
 * N2's solution is to allocate/free memory ('Buffers') from exactly one module
 * in the platform layer (the platform::mem module, probably surprising
 * everyone), provide handles to that data via the `buffer::Descriptor` POD
 * structure (defined in the buffer/descriptor.cc module, again shocking all
 * involved), and expose buffers and buffer views to both the platform and game
 * layers through helper functions defined below. The platform layer will
 * directly access the mem:: module, and the game layer will call through
 * function pointers exposed from the GameState API.
 *
 * The retention pattern for any given Buffer is defined at create time (and
 * cannot be modified?) using the `buffer::Flags` defined alongside the
 * Buffer:Descriptor. The default behavior for buffers is to make _no guarantee
 * that data will be saved between frames_. Once a Buffer is created, it will
 * always be accessible via `lookup` but -- unless the `PERSIST` flag is set --
 * the data stored in that buffer might not be accessible once a frame boundary
 * is reached.
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
 * You can play as fast and loose with pointers within a frame, but expect that
 * the last frame's pointers, and pointers to other data locations, will be in
 * "interesting" states by the time the next frame starts.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "buffer/descriptor.h"
#include "buffer/hashtable.h"
#include "buffer/ring.h"
#include "buffer/slice.h"
#include "buffer/stream.h"


namespace buffer {

/** Common Interface
 *  ================
 *  The Platform and Game binaries will each require a separate implementation of
 *  the interface defined in this file. For more code, see,
 *    platform/modules/buffer.cc
 *    game/modules/buffer.cc
 */

/* Look up a game buffer by name */
inline Optional<Buffer *const> find(c_cstr name);
/* Create a new game buffer -- will crash if */
inline Buffer *const create(c_cstr name, u64 size, buffer::Flags flags);
/* Resize a game buffer */
inline u64 resize(Buffer *const bd, u64 new_size);
/* Destroy a game buffer */
inline void destroy(Buffer *const bd);


/** Buffer Helpers
 *  --------------
 *  These are a series of helper functions whose sole purpose is to make it
 *  easier to look up and create Buffer View types.
 */

template < typename T >
inline Optional< Slice<T> > find_slice(c_cstr name);
template < typename T >
inline Optional< Ring<T> > find_ring(c_cstr name);
template < typename T >
inline Optional< Stream<T> > find_stream(c_cstr name);
template < typename K, typename V >
inline Optional< HashTable<K,V> > find_hashtable(c_cstr name);


template < typename T >
inline Slice<T> create_slice(
    c_cstr name,
    u64    capacity = Slice<T>::default_capacity,
    Flags  flags    = PASS);
template < typename T >
inline Ring<T> create_ring(
    c_cstr name,
    u64    capacity = Ring<T>::default_capacity,
    Flags  flags    = PASS);
template < typename T >
inline Stream<T> create_stream(
    c_cstr name,
    u64    capacity = Stream<T>::default_capacity,
    Flags  flags    = PASS);
template < typename K, typename V >
inline HashTable<K,V> create_hashtable(
    c_cstr name,
    u64    capacity       = HashTable<K,V>::default_capacity,
    u64    miss_tolerance = HashTable<K,V>::default_miss_tolerance,
    Flags  flags          = PASS);

} /* namespace buffer */
