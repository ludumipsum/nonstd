/* Buffer:Buffer Hash Map
   ======================

   BufferMaps build a table of buffer::Descriptor+data blocks accessible by hashed
   strings.

   TODO: Come up with a better name for this
   TODO: Refactor this out of the project. It's too complicated, too hard to
         maintain, and contray to the newer style of buffer management.
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"
#include "buffer.h"

class BufferMap {
protected:
    struct Cell {
        u64     id;
        ptrdiff offset;
        u64     size;
        c_cstr  name;
        bool    used;
    };
    struct Metadata {
        u32  magic;
        u32  bucket_count;
        Cell (&map)[];
    };

    Metadata         *      m_metadata;
    GameState        *      m_state;
    buffer::Descriptor *const m_bd;

public:
    BufferMap(buffer::Descriptor *const bd, u32 bucket_count=0)
              : m_metadata ( nullptr )
              , m_state    ( nullptr )
              , m_bd       ( bd      )
    {
        initialize(bucket_count);
    }
    BufferMap(GameState& state, c_cstr name, u32 bucket_count=0)
              : m_metadata ( nullptr)
              , m_state    ( &state )
              , m_bd       ( state.memory.lookup(name) )
    {
        initialize(bucket_count);
    }

protected:
    /* Set up the metadata structure at the start of the data segment */
    inline void initialize(u32 bucket_count) {
        m_metadata = (Metadata*) m_bd->data;
        /* If the map hasn't been used before, or is corrupted, reset all the
           memory used for metadata. */
        if (m_metadata->magic != 0xBADC0DE) {
            if (m_metadata->magic) {
                LOG("WARNING: Buffermap corruption detected, clearing all "
                    "associated data and reinitializing the map. Underlying "
                    "buffer is named %s, and is located at %p. Corruption "
                    "detected by magic number (%x is neither 0 nor 0xBADC0DE)",
                    m_bd->name, m_bd, m_metadata->magic);
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
            if (m_bd->size < metadata_size) {
                resize(metadata_size);
            }
            // Initialize metadata
            m_metadata->magic = 0xBADC0DE;
            m_metadata->bucket_count = bucket_count;
            memset(&m_metadata->map, '\0', sizeof(Cell) * bucket_count);
            m_bd->cursor = (ptr)( &m_metadata->map[0] + bucket_count );
        }
        /* If the number of buckets in the call differs, complain. */
        if (bucket_count && m_metadata->bucket_count != bucket_count) {
            LOG("WARNING: Caller expects the map to contain %d cells, but "
                "metadata shows it was initailized with %d.",
                bucket_count, m_metadata->bucket_count);
            DEBUG_BREAKPOINT();
        }
    }

    Cell* lookup_cell(c_cstr key) {
        auto const bucket_count = m_metadata->bucket_count;
        auto& map = m_metadata->map;
        auto keyhash = HASH(key);

        auto cell_index = keyhash % bucket_count;
        const auto final_cellid = (cell_index - 1) % bucket_count;
        while(cell_index != final_cellid && map[cell_index].used) {
            Cell& cell = map[cell_index];
            if (cell.id == keyhash && cell.name
                                   && 0 == strcmp(cell.name, key)) {
                //TODO: Check if this strcmp is too slow
                return &cell;
            }
            cell_index += 1;
        }

        return nullptr;
    }

public:
    inline void resize(u64 size_bytes) {
        if (m_state) {
            if (size_bytes <= m_bd->size) {
                LOG("Skipping resize (would shrink)");
            } 
            m_state->memory.resize(m_bd, size_bytes);
            m_metadata = (Metadata*) m_bd->data;
        } else {
            LOG("Unable to resize a BufferMap that was created from a view "
                "directly. Please create this map from a State if you need "
                "dynamic resizing.");
            BREAKPOINT();
        }
    }

    buffer::Descriptor *const lookup(c_cstr key) {
        Cell* cellptr = lookup_cell(key);
        if (cellptr != nullptr) {
            return (buffer::Descriptor *const)
                   ((u8*)(m_bd->data) + cellptr->offset);
        }
        return nullptr;
    }

    buffer::Descriptor *const create(c_cstr name, u64 size) {
        /* Determine if the underlying buffer has enough space and resize it
           if necessary */ {
            auto full_size = size + sizeof(buffer::Descriptor);
            u64 required_size = m_bd->cursor - m_bd->data + full_size;
            if (required_size > m_bd->size) {
                resize(required_size * 1.2f);
                LOG("Grew live buffer map %s from " Fu64 "B to " Fu64 "B",
                    m_bd->name, m_bd->size, (u64)(required_size * 1.2f));
            }
        }

        /* Consume a cell with metadata, and allocate `size` bytes, plus enough
           space for a ::bufferdescriptor. */
        auto const bucket_count = m_metadata->bucket_count;
        auto& map = m_metadata->map;
        auto keyhash = HASH(name);
        auto full_size = size + sizeof(buffer::Descriptor);

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
        cell.name = name;
        cell.id = keyhash;
        cell.size = full_size;

        /* Reserve memory for this entry */
        u64 required_size = m_bd->cursor - m_bd->data + full_size;
        if (required_size > m_bd->size) {
            resize(required_size * 1.2f);
        }
        cell.offset = m_bd->cursor - m_bd->data;
        m_bd->cursor = m_bd->cursor + full_size;

        /* Set up the buffer::Descriptor for this block */
        u8 const*const block_begin = m_bd->data + cell.offset;
        buffer::Descriptor *const desc = (buffer::Descriptor *const)block_begin;
        desc->data = (ptr)( desc + 1 );
        desc->cursor = desc->data;
        desc->size = size;
        desc->flags = buffer::PASS;
        desc->name = name;

        return desc;
    }
};
