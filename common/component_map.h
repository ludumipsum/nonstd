/* Typed ID:Component Map
   ======================

   Mapping from externally-provided entity ID to a component instance. Provides
   two APIs: random access via entity ID, and sequential instance access, along
   with methods for modifying the sequential data block without invalidating
   ID indirection.
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"
#include "buffer.h"

template<typename T>
class IDMap {
protected:
    BufferHashTable m_index;
    BufferView      m_data;

public:
    IDMap(GameState& state)
          : m_index (state, T::metadata_storage_name)
          , m_data  (state, T::data_storage_name) {}

    inline void resize(u64 count) {
        m_index.rehash_to(count);
        m_data.resize(count);   // TODO: this is wrong, some of our size tihngs
                                //       are in bytes, some in counts.
    }
};
