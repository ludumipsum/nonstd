/* Buffer:Buffer Hash Table
   ========================

   Map from an entity ID to a 32-bit unsigned index.
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"
#include "buffer.h"

#define DEFAULT_BHT_CELL_COUNT 64

class BufferHashTable {
protected:
    struct Cell {
        ID      id;
        u32     index;
    };
    struct Metadata {
        u32  magic;
        u32  cell_count;
        u64  miss_tolerance;
        Cell map[];
    };

    Metadata         *      m_metadata;
    GameState        *      m_state;
    BufferDescriptor *const m_bd;

public:
    BufferHashTable(BufferDescriptor *const bd, u32 cell_count=0)
        : m_metadata ( nullptr )
        , m_state    ( nullptr )
        , m_bd       ( bd      )
    {
        // Since instances created with this constructor can't be resized, we
        // set the miss_tolerance to something that can never be reached: one
        // more than the number of cells in the table
        initialize(cell_count, cell_count + 1);
    }
    BufferHashTable(GameState& state, c_cstr name,
                    u32 cell_count=0, u64 miss_tolerance=32)
        : m_metadata ( nullptr)
        , m_state    ( &state )
        , m_bd       ( state.memory.lookup(name) )
    {
        miss_tolerance = n2min(cell_count, miss_tolerance);
        initialize(cell_count, miss_tolerance);
    }

protected:

    /* Set up the metadata structure at the start of the data segment */
    inline void initialize(u32 cell_count, u64 miss_tolerance) {
        m_metadata = (Metadata*) m_bd->data;
        /* If the map hasn't been used before, or is corrupted, reset all the
           memory used for metadata. */
        if (m_metadata->magic != 0xBADB33F) {
            if (m_metadata->magic) {
                LOG("WARNING: BufferHashTable corruption detected, clearing "
                    "all associated data and reinitializing the map. "
                    "Underlying buffer is named %s, and is located at %p. "
                    "Corruption detected by magic number (%x is neither 0 "
                    "nor 0xBADB33F)",
                    m_bd->name, m_bd, m_metadata->magic);
                DEBUG_BREAKPOINT();
            }
            // Default size if none is specified
            if (cell_count == 0) {
                cell_count = DEFAULT_BHT_CELL_COUNT;
            }
            // Resize if necessary
            auto metadata_size = sizeof(Metadata) + sizeof(Cell) * cell_count;
            if (m_bd->size < metadata_size) {
                m_state->memory.resize(m_bd, metadata_size);
                m_metadata = (Metadata*) m_bd->data;
            }
            // Initialize metadata
            m_metadata->magic = 0xBADB33F;
            m_metadata->cell_count = cell_count;
            m_metadata->miss_tolerance = miss_tolerance;
            memset(&m_metadata->map, '\0', sizeof(Cell) * cell_count);
            m_bd->cursor = (ptr)( &m_metadata->map[0] + cell_count );
        }
        /* If the number of buckets in the call differs, complain. */
        if (cell_count && m_metadata->cell_count != cell_count) {
            LOG("WARNING: Caller expects the map to contain %d cells, but "
                "metadata shows it was initailized with %d.",
                cell_count, m_metadata->cell_count);
            DEBUG_BREAKPOINT();
        }
    }

    inline Cell *const lookup_cell(ID id) {
        auto const cell_count = m_metadata->cell_count;
        auto& map = m_metadata->map;
        auto hash = shift64(id);

        auto cell_index = hash % cell_count;
        auto const final_cellid = (cell_index - 1) % cell_count;

        Cell* ptr = nullptr;
        u64 misses = 0;

        // This loop will exit when either,
        //  1. We've wrapped the entire cell table and no viable cell has been
        //     found -- nullptr will be returned.
        //  2. We've found no cell associated with the given `id`, but we have
        //     found an empty cell that may be associated with the `id` -- an
        //     uninitialized cell will be returned.
        //  3. We've found an initialized cell associated with the given `id` --
        //     an initialized cell will be returned.
        while(cell_index != final_cellid && map[cell_index].id >= ID_DELETED) {
            Cell& cell = map[cell_index];
            if (cell.id == id) {
                ptr = &cell;
                break;
            }
            cell_index += 1;
            misses += 1;
        }

        if (cell_index != final_cellid) {
            ptr = &map[cell_index];
        }

        if (misses > m_metadata->miss_tolerance) {
            rehash_by(1.2f);
        }

        return ptr;
    }

public:
    inline void rehash_to(u64 cell_count) {
        if (m_state == nullptr) {
            LOG("ERROR: Can't resize a hashtable without a state ref.");
            BREAKPOINT();
        }

        // Copy all the data aside
        // TODO: REPLACE ME WITH SCRATCH BUFFER USAGE
        ptr intermediate = n2malloc(m_bd->size);
        // TODO: AUGH MALLOC SADNESS
        if (intermediate == nullptr) BREAKPOINT();
        memset(intermediate, 0xFF, m_bd->size);
        auto intermediate_bd = make_buffer(intermediate, m_bd->size);
        memcpy(intermediate_bd.data, m_bd->data, m_bd->size);
        ptrdiff cur_offset = m_bd->cursor - m_bd->data;
        intermediate_bd.cursor = intermediate_bd.data + cur_offset;
        BufferHashTable src(&intermediate_bd);

        // Resize the backing buffer
        auto needed_bytes = sizeof(Metadata) + sizeof(Cell) * cell_count;
        m_state->memory.resize(m_bd, needed_bytes);
        m_metadata->cell_count = cell_count;
        m_bd->cursor = (ptr)( &m_metadata->map[0] + cell_count );

        // Rehash the table
        Cell* final_cell = src.m_metadata->map + src.m_metadata->cell_count;
        for(Cell *scell = src.m_metadata->map,
                 *final_cell = src.m_metadata->map + src.m_metadata->cell_count;
            scell <= final_cell;
            scell++) {
            create(scell->id, scell->index);
        }

        // Discard temporary space (TODO: replace with scratch)
        n2free(intermediate);
    }

    inline void rehash_by(f32 growth_factor) {
        rehash_to(growth_factor * m_metadata->cell_count);
    }

    inline Optional<u32> lookup(ID id) {
        Cell *const cell = lookup_cell(id);
        if (cell == nullptr || cell->id != id) return {};
        return { cell->index };
    }

    inline bool contains(ID id) {
        return (bool)(lookup(id));
    }

    inline Optional<u64> create(ID id, u64 index) {
        Cell *const cell = lookup_cell(id);
        if (cell == nullptr) return {};
        cell->id = id;
        cell->index = index;
        return { index };
    }

    inline void destroy(ID id) {
        Cell *const cell = lookup_cell(id);
        if (cell != nullptr) {
            cell->id = ID_DELETED;
            cell->index = 0;
        }
    }
};
