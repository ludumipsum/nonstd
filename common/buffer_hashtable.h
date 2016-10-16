/* Buffer:Buffer Hash Table
   ========================

   TODO: Document me
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"
#include "buffer.h"

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
    inline void rehash(f32 growth_factor) {
        BREAKPOINT();
    }

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
            /* Sanity check -- don't iniaizlie without a size */
            if (cell_count == 0) {
                LOG("ERROR: Unable to initialize a BufferHashTable for the first "
                    "time without a nonzero number of buckets.");
                BREAKPOINT();
            }
            // Resize if necessary
            auto metadata_size = sizeof(Metadata) + sizeof(Cell) * cell_count;
            if (m_bd->size < metadata_size) {
                rehash(n2min(metadata_size/(f64)m_bd->size, 1.2f));
            }
            // Initialize metadata
            m_metadata->magic = 0xBADB33F;
            m_metadata->cell_count = cell_count;
            m_metadata->miss_tolerance = miss_tolerance;
            memset(&m_metadata->map, '\0', sizeof(Cell) * cell_count);
            m_bd->cursor = &m_metadata->map[0] + cell_count;
        }
        /* If the number of buckets in the call differs, complain. */
        if (cell_count && m_metadata->cell_count != cell_count) {
            LOG("WARNING: Caller expects the map to contain %d cells, but "
                "metadata shows it was initailized with %d.",
                cell_count, m_metadata->cell_count);
            DEBUG_BREAKPOINT();
        }
    }

    Cell *const lookup_cell(ID id) {
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
        while(cell_index != final_cellid && map[cell_index].id > ID_FIRST) {
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
            LOG("EINVAL, Rehashes aren't implemented yet.");
            BREAKPOINT();
        }

        return ptr;
    }

public:
    Optional<u32> lookup(ID id) {
        Cell *const cell = lookup_cell(id);
        if (cell == nullptr || cell->id != id) return {};
        return { cell->index };
    }

    bool contains(ID id) {
        return (bool)(lookup(id));
    }

    Optional<u64> create(ID id, u64 index) {
        Cell *const cell = lookup_cell(id);
        if (cell == nullptr) return {};
        cell->id = id;
        cell->index = index;
        return { index };
    }

    void destroy(ID id) {
        Cell *const cell = lookup_cell(id);
        if (cell != nullptr) {
            cell->id = ID_NOTFOUND;
            cell->index = 0;
        }
    }
};
