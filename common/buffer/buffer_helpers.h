/* Buffer Helpers Interface
 * ========================
 * These are a series of helper functions whose sole purpose is to make it
 * easier to look up and create Buffer View types.
 * The Platform and Game binaries will each require a separate implementation of
 * the interface defined in this file. For more code, see,
 *   platform/modules/buffer_helpers.cc
 *   game/modules/buffer_helpers.cc
 */

#if !defined(IFACE_BUFFER_HELPERS)
#define IFACE_BUFFER_HELPERS

#include "batteries_included.h"
#include "primitive_types.h"


namespace buffer {

template<typename T>
inline Slice<T>       get_slice(c_cstr name);
template<typename T>
inline Ring<T>        get_ring(c_cstr name);
template<typename T>
inline Stream<T>      get_stream(c_cstr name);
template<typename K, typename V>
inline HashTable<K,V> get_hashtable(c_cstr name);


template<typename T>
inline Slice<T> new_slice(
    c_cstr name,
    u64    capacity = Slice<T>::default_capacity,
    Flags  flags    = PASS);
template<typename T>
inline Ring<T> new_ring(
    c_cstr name,
    u64    capacity = Ring<T>::default_capacity,
    Flags  flags    = PASS);
template<typename T>
inline Stream<T> new_stream(
    c_cstr name,
    u64    capacity = Stream<T>::default_capacity,
    Flags  flags    = PASS);
template<typename K, typename V>
inline HashTable<K,V> new_hashtable(
    c_cstr name,
    u64    capacity       = HashTable<K,V>::default_capacity,
    u64    miss_tolerance = HashTable<K,V>::default_miss_tolerance,
    Flags  flags          = PASS);

} /* namespace buffer */
#endif /* IFACE_BUFFER_HELPERS */
