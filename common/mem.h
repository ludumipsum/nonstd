/** N2 Memory Management
 *  ====================
 *  Memory management is hard. Memory management in a hot-loading,
 *  code-swapping, volatile-memory, context is more so.
 *
 *  N2's solution is to allocate/free memory ('Buffers') from exactly one module
 *  in the platform layer (the platform::mem implementation, probably surprising
 *  everyone), provide handles to that data via the `mem::Buffer` POD structure,
 *  and expose Buffers and Buffer Views to both the platform and game layers
 *  through this shared interface. The platform layer will allocate, free, and
 *  directly manage the memory for N2, and the game layer will use the same
 *  system via function pointers exposed through the GameState API.
 *
 *  TODO: Work out how the retention system _should_ function. We probably don't
 *  need or want to keep persist- and clear-mode metadata in the buffer itself.
 *  Rather, we should store that only in the memory subsystem.
 *
 *  The retention pattern for any given Buffer is defined at create time (and
 *  cannot be modified?) using the `buffer::Flags` defined alongside the
 *  buffer::Descriptor. The default behavior for buffers is to make _no
 *  guarantee that data will be saved between frames_. Once a Buffer is created,
 *  it will always be accessible via `find` but -- unless the `PERSIST` flag is
 *  set -- the data stored in that buffer might not be accessible once a frame
 *  boundary is reached.
 *
 *  NB. As there is no real guarantee of what the platform layer will or won't
 *  do to your data pointers between frames -- or even if your game state is
 *  actually "between" frames; think of state loads, A-B repeats, state shared
 *  across networks, etc. -- only Trivially Copyable data (not including
 *  pointers) should be stored in retained buffers. Descriptor existence (name,
 *  properties, and presence in the platform's lookup table) will remain valid
 *  between frames but there is _never_ a guarantee that the same memory
 *  location will be retained between frames.
 *
 *  You can play as fast and loose with pointers within a frame, but expect that
 *  the last frame's pointers, and pointers to other data locations, will be in
 *  "interesting" states by the time the next frame starts.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "mem/buffer.h"
#include "mem/hashtable.h"
#include "mem/ring.h"
#include "mem/slice.h"
#include "mem/stream.h"


namespace mem {

/** Simple Buffer Interface
 *  -----------------------
 *  These helper functions allow direct manipulation of mem::Buffer objects, and
 *  the memory stored therein.
 */

/* Create a new memory buffer */
inline Buffer *const allocate(c_cstr name, u64 size, mem::Flags flags = PASS);
/* Resize a memory buffer (using `realloc` memory-movement semantics) */
inline u64 resize(Buffer *const bd, u64 new_size);
/* Release a memory buffer (`free`ing the memory stored therein) */
inline void release(Buffer *const bd);

/* Look up a memory buffer by name */
inline Optional<Buffer *const> find(c_cstr name);
/* Look up a persisted buffer from a previous frame */
inline Optional<Buffer *const> findHistoric(c_cstr name, u64 frame);


namespace view {

/** Buffer View Helper Interface
 *  ----------------------------
 *  These helper functions are designed to make it easier to look up and create
 *  Buffer Views, reducing the need to manually perform the allocate->initialize
 *  ->wrap steps manually (and possibly mess those steps up).
 */

template < typename T >
inline Optional< Slice<T> > findSlice(c_cstr name);
template < typename T >
inline Optional< Ring<T> > findRing(c_cstr name);
template < typename T >
inline Optional< Stream<T> > findStream(c_cstr name);
template < typename K, typename V >
inline Optional< HashTable<K,V> > findHashTable(c_cstr name);


template < typename T >
inline Slice<T> allocateSlice(
    c_cstr name,
    u64    capacity = Slice<T>::default_capacity,
    Flags  flags    = PASS);
template < typename T >
inline Ring<T> allocateRing(
    c_cstr name,
    u64    capacity = Ring<T>::default_capacity,
    Flags  flags    = PASS);
template < typename T >
inline Stream<T> allocateStream(
    c_cstr name,
    u64    capacity = Stream<T>::default_capacity,
    Flags  flags    = PASS);
template < typename K, typename V >
inline HashTable<K,V> allocateHashTable(
    c_cstr name,
    u64    capacity       = HashTable<K,V>::default_capacity,
    u64    miss_tolerance = HashTable<K,V>::default_miss_tolerance,
    Flags  flags          = PASS);

} /* namespace view */
} /* namespace mem  */
