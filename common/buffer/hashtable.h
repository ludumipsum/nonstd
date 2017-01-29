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
protected: /*< ## Sub-Types */
    static const u32 magic = 0xBADB33F;
    struct Cell {
        HTK  key;
        HTV  value;
    };
    struct Metadata {
        u32  magic;
        u64  cell_capacity;
        u64  miss_tolerance;
        u64  cell_count;
        bool rehash_in_progress;
        Cell map[];
    };


public: /*< ## Class Methods */
    static const u64 default_cell_capacity  = 64;
    static const u64 default_miss_tolerance = 16;

    inline static u64 precomputeSize(u64 capacity = default_cell_capacity) {
        return sizeof(Metadata) + sizeof(Cell) * capacity;
    }
    inline static void initializeBuffer(Descriptor *const bd,
                                        u64 miss_tolerance = 0) {
        Metadata * metadata = (Metadata *)bd->data;
        if (metadata->magic && metadata->magic != magic) {
            LOG("WARNING: Buffer HashTable corruption detected.\n"
                "Underlying buffer is named %s, and is located at %p.\n"
                "Corruption detected by magic numer --- %x is neither 0 nor %x."
                "If there's more information to print about this HashTable, we "
                "should add it to the code that prints this message.\n"
                "Clearing all associated data and reinitializing the map.",
                bd->name, bd, metadata->magic, magic);
            DEBUG_BREAKPOINT();
        }
        if (metadata->rehash_in_progress) {
            LOG("ERROR: Buffer HashTable has been reinitialized while "
                "`rehash_in_progress == true`. I don't know what to do here.\n"
                "Underlying buffer is named %s, and it is located at %p.",
                bd->name, bd);
            BREAKPOINT();
        }
        u64 data_region_size = bd->size - sizeof(Metadata);
        u64 capacity         = data_region_size / sizeof(Cell);
        metadata->magic              = magic;
        metadata->cell_capacity      = capacity;
        metadata->miss_tolerance     = miss_tolerance ? miss_tolerance
                                                      : default_miss_tolerance;
        metadata->cell_count         = 0;
        metadata->rehash_in_progress = false; /*< TODO: uhh... This? y/n? */
        memset(&(metadata->map), '\0', data_region_size);
    }


protected: /*< ## Public Member Variables */
    Descriptor *const m_bd;
    Metadata   *      m_metadata;
    BufferResizeFn    m_resize;

public: /*< ## Ctors, Detors, and Assignments */
    HashTable(Descriptor *const bd,
              BufferResizeFn resize = nullptr)
        : m_bd       ( bd                      )
        , m_metadata ( (Metadata*)(m_bd->data) )
        , m_resize   ( resize                  ) { }

public: /*< ## Public Memeber Methods */
    // TODO: Error checking / reporting?
    inline void resize(u64 capacity) {
        _resize_to(capacity);
    }

    // TODO: Error checking / reporting?
    inline void resize_by(f64 growth_factor) {
        _resize_to((u64)(m_metadata->cell_capacity * growth_factor));
    }

    inline void drop() {
        LOG("Does this function even make sense? Drop :allthethings:? ");
        BREAKPOINT();
    }

    inline u64 size()           { return m_bd->size; }
    inline u64 count()          { return m_metadata->cell_count; }
    inline u64 capacity()       { return m_metadata->cell_capacity; }
    inline u64 miss_tolerance() { return m_metadata->miss_tolerance; }

    /* Read/Write operation
     * Returns valid Optional on key match and empty cell.
     * TODO: This API is... not good. I don't think.
     */
    inline Optional<HTV> operator[](HTK key) {
        Cell *const cell = _lookup_cell(key);
        bool valid_key = cell->key == key || cell->key == 0;
        if (cell == nullptr || ! valid_key) return { };
        return { cell->value };
    }

    /* Read operation
     * Returns valid Optional only on key match.
     */
    inline Optional<HTV> lookup(HTK key) {
        Cell *const cell = _lookup_cell(key);
        bool valid_key = cell->key == key;
        if (cell == nullptr || ! valid_key) return { };
        return { cell->value };
    }

    /* Write operation
     * Happy to overwrite if a key already exists. Returns a valid Optional so
     * long as there's room in the HashTable.
     */
    inline Optional<HTV> create(HTK key, HTV value) {
        Cell *const cell = _lookup_cell(key);
        if (cell == nullptr) return { }; /*< TODO: No room for a key? */
        cell->key   = key;
        cell->value = value;
        return { cell->value };
    }

    inline void destroy(HTK key) {
        Cell *const cell = _lookup_cell(key);
        if (cell != nullptr) {
            cell->key   = ID_DELETED; /*< TODO: Use a HT-specific sentinel */
            cell->value = 0;
        }
    }

    inline bool contains(HTK key) { return (bool)(lookup(key)); }


protected: /*< Protected Member Methods */
    inline void _resize_to(u64 capacity) {
        LOG("This function is currently unimplemented.");
        BREAKPOINT();

        /* Below is what came... _Before_ */

        // Mark this hashtable as already in a rehash, so lookup_cell may not
        // heuristically trigger another rehash if we get unlucky
        m_metadata->rehash_in_progress = true;

        // FIXME: This is a super dumb hack to deal with our cell lookup code
        //        not being the most robust thing ever. Also to deal with the
        //        interface being kinda bad.
        capacity = n2max(16, capacity);

        // Copy all the data aside
        // TODO: REPLACE ME WITH SCRATCH BUFFER USAGE
        ptr intermediate = n2malloc(m_bd->size);
        if (intermediate == nullptr) BREAKPOINT();

        memset(intermediate, 0xFF, m_bd->size);
        auto intermediate_bd = make_buffer(intermediate, m_bd->size);
        memcpy(intermediate_bd.data, m_bd->data, m_bd->size);

        // Create an intermediate HashTable using the data (and metadata) copied
        // from the current HashTable.
        HashTable src(&intermediate_bd);

        // Resize the backing buffer
        m_resize(m_bd, HashTable::precomputeSize(capacity));
        m_metadata->cell_capacity = capacity;

        // Rehash the table
        // TODO: Drew, you should learn how this part works.
        Cell* final_cell = src.m_metadata->map + src.m_metadata->cell_capacity;
        for(Cell *scell     = src.m_metadata->map,
                 *final_cell = src.m_metadata->map + src.m_metadata->cell_capacity;
            scell <= final_cell;
            scell++)
        {
            create(scell->key, scell->value);
        }

        // Discard temporary space (TODO: replace with scratch)
        n2free(intermediate);

        // Mark the rehash complete
        m_metadata->rehash_in_progress = false;
    }

    inline Cell *const _lookup_cell(HTK key) {
        auto const cell_capacity = m_metadata->cell_capacity;
        auto& map  = m_metadata->map;
        auto  hash = shift64(key);

        /* Initial index for the given key. */
        auto cell_index = hash % cell_capacity;
        /* The last index that may terminate the below loop. */
        auto const final_index = (cell_index - 1) % cell_capacity;

        Cell* ptr = nullptr;
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
        while(cell_index != final_index && map[cell_index].key >= ID_DELETED) {
            Cell& cell = map[cell_index];
            if (cell.key == key) {
                ptr = &cell;
                break;
            }
            cell_index = (1 + cell_index) % cell_capacity;
            misses += 1;
        }

        if (cell_index != final_index) {
            ptr = &map[cell_index];
        }

        bool exceeded_miss_tolerance = misses > m_metadata->miss_tolerance;
        bool rehash_allowed = ! m_metadata->rehash_in_progress;
        if (exceeded_miss_tolerance && rehash_allowed) {
            resize_by(1.2f);
        } else if (! rehash_allowed && cell_index == final_index) {
            LOG("ERROR: Table is full, and I can't resize.");
            BREAKPOINT();
        }

        return ptr;
    }


    // Once we switch to a templated class, we should ensure POD.
    // /* Ensure that only POD data is used in these views.*/
    // ENFORCE_POD(T);
};

} /* namespace buffer */
