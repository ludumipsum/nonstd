/* Buffer: Hash Table
 * ========================
 *
 * Map from an entity ID to a 32-bit unsigned index.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "hash.h"
#include "logging.h"
#include "mathutils.h"

#include "api.h"
#include "buffer.h"

namespace buffer {

enum class CellState {
    EMPTY = 0, //< EMPTY _must be_ `0` s.t. explicitly-zero'd Cells are EMPTY.
    DELETED,
    USED,
};

template<typename T_KEY, typename T_VAL>
class HashTable {
protected: /*< ## Inner-Types */
    static const u32 magic = 0xBADB33F;
    struct Cell {
        T_KEY     key;
        T_VAL     value;
        CellState state;
    };
    struct Metadata {
        u32  magic;
        u64  capacity;
        u64  count;
        u64  invalid_count;
        u64  miss_tolerance;
        bool rehash_in_progress;
        Cell map[];
    };

public: /*< ## Class Methods */
    static const u64 default_capacity       = 64;
    static const u64 default_miss_tolerance =  8;

    inline static u64 precomputeSize(u64 capacity = default_capacity) {
        return sizeof(Metadata) + sizeof(Cell) * capacity;
    }

    inline static void initializeBuffer(Descriptor *const bd,
                                        u64 miss_tolerance = 0) {
        Metadata * metadata = (Metadata *)(bd->data);
#if defined(DEBUG)
        N2CRASH_IF(bd->size < sizeof(Metadata), Error::InsufficientMemory,
            "Buffer HashTable is being overlaid onto a Buffer that is too "
            "small (" Fu64 ") to fit the HashTable Metadata (" Fu64 ").\n"
            "Underlying buffer is named %s, and it is located at %p.",
            bd->size, sizeof(Metadata), bd->name, bd);
        N2CRASH_IF(metadata->rehash_in_progress, Error::PEBCAK,
            "Buffer HashTable has been reinitialized while "
            "`rehash_in_progress == true`. This should not be possible.\n"
            "Underlying buffer is named %s, and it is located at %p.",
            bd->name, bd);
#endif
        /* If the type check is correct, no initialization is required. */
        if (metadata->magic == magic) { return; }
        if (metadata->magic && metadata->magic != magic) {
            LOG("WARNING: Buffer HashTable corruption detected. Corruption "
                "detected by magic number -- %x is neither 0 nor %x.\n"
                "Clearing all associated data and reinitializing the map.\n"
                "Underlying buffer is named %s, and is located at %p.",
                metadata->magic, magic, bd->name, bd);
            DEBUG_BREAKPOINT();
        }
        u64 data_region_size = bd->size - sizeof(Metadata);
        u64 capacity         = data_region_size / sizeof(Cell);
        metadata->magic              = magic;
        metadata->capacity           = capacity;
        metadata->count              = 0;
        metadata->invalid_count      = 0;
        metadata->miss_tolerance     = miss_tolerance ? miss_tolerance
                                                      : default_miss_tolerance;
        metadata->rehash_in_progress = false;
        memset(metadata->map, '\0', data_region_size);
    }


protected: /*< ## Public Member Variables */
    Descriptor * const m_bd;
    Metadata   *       m_metadata;
    BufferResizeFn     m_resize;

public: /*< ## Ctors, Detors, and Assignments */
    HashTable(Descriptor *const bd,
              BufferResizeFn resize = nullptr)
        : m_bd       ( bd                      )
        , m_metadata ( (Metadata*)(m_bd->data) )
        , m_resize   ( resize                  ) { }


public: /*< ## Public Memeber Methods */
    inline u64 size()           { return m_bd->size; }
    inline u64 capacity()       { return m_metadata->capacity; }
    inline u64 count()          { return m_metadata->count; }
    inline u64 invalid_count()  { return m_metadata->invalid_count; }
    inline u64 miss_tolerance() { return m_metadata->miss_tolerance; }

    /* Reset this HashTable to empty. */
    inline void drop() {
        memset(m_metadata->map, '\0', (capacity() * sizeof(Cell)));
        m_metadata->count = 0;
        m_metadata->invalid_count = 0;
    }

    inline void resize_to(u64 capacity) {
        _resize(HashTable::precomputeSize(capacity));
    }

    inline void resize_by(f64 growth_factor) {
        u64 capacity = (u64)(this->capacity() * growth_factor);
        _resize(HashTable::precomputeSize(capacity));
    }

    /* Search for the given key, returning an Optional. */
    inline Optional<T_VAL&> operator[](T_KEY key) { return get(key); }
    inline Optional<T_VAL&> get(T_KEY key) {
        Cell *const cell = _lookup_cell(key);
        if (cell == nullptr || cell->state != CellState::USED) return { };
        return { cell->value };
    }

    /* Check for the existence of the given key. */
    inline bool contains(T_KEY key) { return (bool)(get(key)); }

    /* Write the given k/v pair, returning an Optional.
     * The Optional will be only false if the HashTable is completely filled,
     * and unable to resize. */
    inline Optional<T_VAL&> set(T_KEY key, T_VAL value) {
        Cell *const cell = _lookup_cell(key);
        if (cell == nullptr) return { };
        cell->key   = key;
        cell->value = value;
        if (cell->state == CellState::EMPTY) {
            cell->state = CellState::USED;
            m_metadata->count += 1;
        }
        return { cell->value };
    }

    /* Write the given k/v pair if the given key has not yet been written,
     * returning an Optional.
     * The Optional will be false if the given key already exists in the
     * HashTable, or if the HashTable is completely filled, and unable
     * to resize. */
    inline Optional<T_VAL&> create(T_KEY key, T_VAL value) {
        Cell *const cell = _lookup_cell(key);
        if (cell == nullptr || cell->state != CellState::EMPTY) return { };
        cell->key   = key;
        cell->value = value;
        cell->state = CellState::USED;
        m_metadata->count += 1;
        return { cell->value };
    }

    /* Remove the given key from the HashTable.
     * No records are written if the key has not been previously written. */
    inline void destroy(T_KEY key) {
        Cell *const cell = _lookup_cell(key);
        if (cell != nullptr && cell->state == CellState::USED) {
            cell->state = CellState::DELETED;
            m_metadata->count -= 1;
            m_metadata->invalid_count += 1;
        }
    }


protected: /*< ## Protected Member Methods */
    /**
     * Resize `this` HashTable to have room for exactly `capacity` elements.
     * This function should be able to both upscale and downscale HashTables.
     */
    inline void _resize(u64 new_size) {
        u64 data_region_size = new_size - sizeof(Metadata);
        u64 new_capacity     = data_region_size / sizeof(Cell);

#if defined(DEBUG)
        N2CRASH_IF(m_resize == nullptr, Error::NullPtr,
            "Attempting to resize a HashTable that has no associated "
            "resize function.\n"
            "Underlying buffer is named %s, and it is located at %p.",
            m_bd->name, m_bd);
        N2CRASH_IF(m_bd->size < sizeof(Metadata), Error::InsufficientMemory,
            "Buffer HashTable is being resized into a Buffer that is too small "
            "(" Fu64 ") to fit the HashTable Metadata (" Fu64 ").\n"
            "Underlying buffer is named %s, and it is located at %p.",
            m_bd->size, sizeof(Metadata), m_bd->name, m_bd);
        N2CRASH_IF(new_capacity < count(), Error::InsufficientMemory,
            "Resizing a HashTable such that the new capacity (" Fu64 ") is "
            "less than the current count (" Fu64 "). This... is probbaly not "
            "okay. Data should be `destroy`d or `drop`d before downsizing?\n"
            "Underlying buffer is named %s, and it is located at %p.",
            new_capacity, count(), m_bd->name, m_bd);
#endif

        // Copy all current data aside to an intermediate `src` HashTable.
        // TODO: REPLACE ME WITH SCRATCH BUFFER USAGE
        ptr intermediate_data = n2malloc(m_bd->size);
        if (intermediate_data == nullptr) BREAKPOINT();

        memset(intermediate_data, 0xFF, m_bd->size);

        Buffer intermediate_bd = make_buffer(intermediate_data, m_bd->size);
        memcpy(intermediate_bd.data, m_bd->data, m_bd->size);

        HashTable src(&intermediate_bd);

        // Resize the backing buffer.
        // `realloc` "copies as much of the old data pointed to by `ptr` as will
        // fit [in]to the new allocation". This function('s DEBUG checks)
        // guarantee that we will have at least room enough for the previous
        // allocation's Metadata. No guarantees are made about the state of the
        // data region, or the current location of the Buffer.
        m_resize(m_bd, new_size);

        // Re-seat the Metadata member, re-set the Metadata members (save for
        // miss_tolerance, which will remain correct), and zero-out the
        // HashTable data region.
        m_metadata           = (Metadata*)(m_bd->data);
        m_metadata->count    = 0;
        m_metadata->capacity = new_capacity;
        memset(m_metadata->map, '\0', data_region_size);

        // Copy all data from the `src` HashTable into `this`.
        // Use `rehash_in_progress` to prevent the below `set` calls from
        // triggering a rehash while this one is completing.
        m_metadata->rehash_in_progress = true;
        Cell * cell       = src.m_metadata->map;
        Cell * final_cell = src.m_metadata->map + src.capacity();
        for (  ; cell < final_cell; cell++) {
            if (cell->state == CellState::USED) { set(cell->key, cell->value); }
        }
        m_metadata->rehash_in_progress = false;

        // Discard temporary space.
        n2free(intermediate_data);
    }

    /**
     * Lookup the given `key` and return a `Cell *`.
     * If the HashTable is completely filled, `nullptr` will be returned,
     * otherwise a `Cell *` will be returned. The returned Cell may have
     * `state == CellState::EMPTY` or `state == CellState::USED`, (but _not_
     * `state == CellState::DELETED`).
     */
    inline Cell *const _lookup_cell(T_KEY key) {
        auto&  map         = m_metadata->map;
        auto   hash        = shift64(key);
        // Initial index for the given key.
        auto   cell_index  = hash % capacity();
        // Counter for invalid cells checked.
        u64    misses      = 0;
        // Target Cell to return (may remain null).
        Cell * ret = nullptr;

        // This loop will exit when either,
        //  1. We've reached a `USED` Cell associated with the given key.
        //  2. We've reached an `EMPTY` Cell (implying the given key has not yet
        //     been used in this HashTable).
        //  3. We've looked at every Cell in the HashTable an none match the
        //     above criteria.
        // If we loop in this section at least m_metadata->miss_tolerance times,
        // we, rather than returning, will resize the entire HashTable (by 1.2
        // times) (thereby rehashing all keys), then reenter this function with
        // the same key.
        while(misses < capacity()) {
            Cell& cell = map[cell_index];
            bool cell_is_empty = cell.state == CellState::EMPTY;
            bool match_found   = (cell.state  == CellState::USED
                                  && cell.key == key);

            if (match_found || cell_is_empty) {
                ret = &cell;
                break;
            }

            cell_index  = (1 + cell_index) % capacity();
            misses     += 1;
        }

        bool should_resize      = misses >= n2min(miss_tolerance(), capacity());
        bool rehash_in_progress = m_metadata->rehash_in_progress;

        if (ret != nullptr && ! should_resize) {
            return ret;
        } else
        if (should_resize && m_resize != nullptr) {
            resize_by(1.2f);
            return _lookup_cell(key);
        } else
        if (should_resize && m_resize == nullptr) {
#if 0
            LOG("WARNING: This HashTable is full -- and a lookup has failed -- "
                "but increasing the size of the HashTable is disallowed.\n"
                "Resize function ptr : %p\n"
                "A resize is %s in progress.\n"
                "Capacity: %d\n"
                "Count: %d\n"
                "Invalid Count: %d\n"
                "Misses : %d (%d allowed)\n"
                "The backing buffer's name is %s and is located at %p.",
                m_resize, m_metadata->rehash_in_progress ? "currently" : "not",
                capacity(), count(), invalid_count(), misses, miss_tolerance(),
                m_bd->name, m_bd);
#endif
            return nullptr;
        }

#if defined(DEBUG)
        N2CRASH(Error::PEBCAK,
            "Whoever wrote this lookup function thought there was no way to "
            "get to this portion of code. And yet here we are.\n"
            "The backing buffer's name is %s and is located at %p.",
            m_bd->name, m_bd);
#endif

        return nullptr;
    }


    /* Ensure that only POD data is used in these views.*/
    ENFORCE_POD(T_KEY);
    ENFORCE_POD(T_VAL);
};

} /* namespace buffer */
