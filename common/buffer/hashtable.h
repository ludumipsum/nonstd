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

/* HashTableKey.   To help kick-off the switch to a templated class. */
#define HTK ID
/* HashTableValue. To help kick-off the switch to a templated class. */
#define HTV u32

namespace buffer {

class HashTable {
protected: /*< ## Inner-Types */
    static const u32 magic = 0xBADB33F;
    struct Cell {
        HTK  key;
        HTV  value;
    };
    struct Metadata {
        u32  magic;
        u64  capacity;
        u64  count;
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
        N2CRASH_IF(metadata->rehash_in_progress, Error::InvalidState,
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
    inline u64 miss_tolerance() { return m_metadata->miss_tolerance; }

    /* Reset this HashTable to empty. */
    inline void drop() {
        memset(m_metadata->map, '\0', (capacity() * sizeof(Cell)));
        m_metadata->count = 0;
    }

    inline void resize_to(u64 capacity) {
        _resize(HashTable::precomputeSize(capacity));
    }

    inline void resize_by(f64 growth_factor) {
        u64 capacity = (u64)(this->capacity() * growth_factor);
        _resize(HashTable::precomputeSize(capacity));
    }

    /* Search for the given key, returning an Optional. */
    inline Optional<HTV&> operator[](HTK key) { return get(key); }
    inline Optional<HTV&> get(HTK key) {
        Cell *const cell = _lookup_cell(key);
        if (cell == nullptr || cell->key != key) return { };
        return { cell->value };
    }

    /* Check for the existence of the given key. */
    inline bool contains(HTK key) { return (bool)(get(key)); }

    /* Write the given k/v pair, returning an Optional.
     * The Optional will be only false if the HashTable is completely filled,
     * and unable to resize. */
    inline Optional<HTV&> set(HTK key, HTV value) {
        Cell *const cell = _lookup_cell(key);
        if (cell == nullptr) return { };
        if (cell->key == 0) { m_metadata->count += 1; }
        cell->key   = key;
        cell->value = value;
        return { cell->value };
    }

    /* Write the given k/v pair if the given key has not yet been written,
     * returning an Optional.
     * The Optional will be false if the given key already exists in the
     * HashTable, or if the HashTable is completely filled, and unable
     * to resize. */
    inline Optional<HTV&> create(HTK key, HTV value) {
        Cell *const cell = _lookup_cell(key);
        if (cell == nullptr || cell->key != 0) return { };
        m_metadata->count += 1;
        cell->key   = key;
        cell->value = value;
        return { cell->value };
    }

    /* Remove the given key from the HashTable.
     * No records are written if the key has not been previously written. */
    inline void destroy(HTK key) {
        Cell *const cell = _lookup_cell(key);
        if (cell != nullptr && cell->key != 0) {
            m_metadata->count -= 1;
            cell->key   = ID_DELETED; //< TODO: Use a HT-specific sentinel
            cell->value = 0;
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
            if (cell->key) { set(cell->key, cell->value); }
        }
        m_metadata->rehash_in_progress = false;

        // Discard temporary space.
        n2free(intermediate_data);
    }

    /**
     * Lookup the given `key` and return a `Cell *`, which may have `key == 0`.
     * If the HashTable is completely filled, `nullptr` will be returned,
     * otherwise a deref'able `Cell *` will be returned.
     */
    inline Cell *const _lookup_cell(HTK key) {
        auto& map  = m_metadata->map;
        auto  hash = shift64(key);

        // Initial index for the given key.
        auto cell_index = hash % capacity();
        u64 misses = 0;

        // This loop will exit when either,
        //  1. We've wrapped the entire cell table and no viable cell has been
        //     found -- nullptr will be returned.
        //  2. We've found no cell associated with the given `key`, but we have
        //     found an empty cell that may be associated with the `key` -- an
        //     uninitialized Cell will be returned.
        //  3. We've found an initialized cell associated with the given `key`
        //     -- an initialized Cell will be returned.
        // NB. Any key with a value `< ID_DELETED` shall be considered empty,
        //     and trigger the `break`.
        while(misses < capacity() && map[cell_index].key >= ID_DELETED) {
            Cell& cell = map[cell_index];
            if (cell.key == key) { break; }
            cell_index = (1 + cell_index) % capacity();
            misses += 1;
        }

        bool have_resize_function    = m_resize != nullptr;
        bool exceeded_miss_tolerance = misses > m_metadata->miss_tolerance;
        bool rehash_allowed          = ! m_metadata->rehash_in_progress;
        bool hashtable_is_full       = misses == capacity();
        bool should_resize = have_resize_function    &&
                             exceeded_miss_tolerance &&
                             rehash_allowed;

#if defined(DEBUG)
        N2CRASH_IF(hashtable_is_full && !should_resize, Error::InvalidState,
            "Table is full, but I'm not allowed to resize. Halp?\n"
            "The backing buffer's name is %s and is located at %p.",
            m_bd->name, m_bd);
#endif
        // Expect that we won't need to resize, and return the target.
        if (! should_resize) {
            return &map[cell_index];;
        }

        // If we do need to resize, do so, and re-enter the lookup function.
        resize_by(1.2f);
        return _lookup_cell(key);
    }


    // Once we switch to a templated class, we should ensure POD.
    // /* Ensure that only POD data is used in these views.*/
    // ENFORCE_POD(T);
};

} /* namespace buffer */
