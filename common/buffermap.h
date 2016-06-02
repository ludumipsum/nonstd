#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"
#include "buffer.h"

/* Buffer Hash Map
   ===============

   BufferMaps provide key/value storage backed by gameplay buffers.
*/
class BufferMap {
public:
    BufferMap(BufferDescriptor* bd, u32 bucket_count=0)
              : m_state ( nullptr )
              , m_bd    ( *bd     ) {
        initialize(bucket_count);
    }
    BufferMap(BufferDescriptor& bd, u32 bucket_count=0)
              : m_state    ( nullptr )
              , m_bd       ( bd      ) {
        initialize(bucket_count);
    }
    BufferMap(GameState& state, c_cstr name, u32 bucket_count=0)
              : m_state    ( &state  )
              , m_bd       ( *state.memory.lookup(name) ) {
        initialize(bucket_count);
    }

    inline void resize(u64 size_bytes) {
        if (m_state) {
            m_state->memory.resize(m_bd, size_bytes);
        } else {
            LOG("Unable to resize a BufferMap that was created from a view "
                "directly. Please create this map from a State if you need "
                "dynamic resizing.");
            BREAKPOINT();
        }
    }

    template<typename T>
    BufferView<T> lookup(c_cstr key) {
        BREAKPOINT();
    }
    void create(c_cstr name, u64 size) {
        auto const& bucket_count = m_metadata->bucket_count;
        auto& map = m_metadata->map;
        auto keyhash = HASH(name);

        /* Scan for an open cell starting at the hash of the given name */
        auto cell_index = keyhash % bucket_count;
        const auto final_cellid = (cell_index - 1) % bucket_count;
        while (cell_index != final_cellid && map[cell_index].used) {
            cell_index += 1;
        }
        // Error out if the map is full. TODO: something better.
        if (map[cell_index].used) BREAKPOINT();

        /* Set up the new cell's metadata */
        Cell& cell = map[cell_index];
        cell.used = true;
        cell.id = keyhash;
        cell.size = size;

        /* Reserve memory for this entry */
        auto const& base = (u8*)m_bd.data;
        auto const& capacity = m_bd.size;
        auto const& brk = (u8*)m_bd.cursor;
        auto required_size = brk + size - base;
        if (required_size > capacity) {
            resize(required_size * 1.2f);
        }
        cell.offset = brk - base;
        m_bd.cursor = brk + size;
    }

protected:
    struct Cell {
        u64  id;
        u64  offset;
        u64  size      : 63;
        bool used      : 1;
    };
    struct Metadata {
        u32  magic;
        u32  bucket_count;
        Cell map[];
    };

    Metadata         *m_metadata;
    GameState        *m_state;
    BufferDescriptor &m_bd;

    /* Set up the metadata structure at the start of the data segment */
    inline void initialize(u32 bucket_count) {
        m_metadata = (Metadata*) m_bd.data;
        /* If the map hasn't been used before, or is corrupted, reset all the
           memory used for metadata. */
        if (m_metadata->magic != 0xDEADC0DE) {
            if (m_metadata->magic) {
                LOG("WARNING: Buffermap corruption detected, clearing all "
                    "associated data and reinitializing the map. Underlying "
                    "buffer is named %s, and begins at %p. Corruption detected "
                    "by magic number (%x is neither 0 nor 0xDEADC0DE).",
                    m_bd.name, m_bd.data, m_metadata->magic);
                DEBUG_BREAKPOINT();
            }
            /* Sanity check -- don't iniaizlie without a size */
            if (bucket_count == 0) {
                LOG("ERROR: Unable to initialize a buffermap for the first "
                    "time without a nonzero number of buckets.");
                BREAKPOINT();
            }
            // Resize if necessary
            auto metadata_size = sizeof(Metadata) + sizeof(Cell) * bucket_count;
            if (m_bd.size < metadata_size) {
                resize(metadata_size);
            }
            // Initialize metadata
            m_metadata->magic = 0xDEADC0DE;
            m_metadata->bucket_count = bucket_count;
            memset(&m_metadata->map, '\0', sizeof(Cell) * bucket_count);
            m_bd.cursor = &m_metadata->map[0];
        }
        /* If the number of buckets in the call differs, complain. */
        if (bucket_count && m_metadata->bucket_count != bucket_count) {
            LOG("WARNING: Caller expects the map to contain %d cells, but "
                "metadata shows it was initailized with %d.",
                bucket_count, m_metadata->bucket_count);
            DEBUG_BREAKPOINT();
        }
    }

};
