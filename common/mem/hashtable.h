/* Typed Buffer-Backed HashTable
 * =============================
 * This is a relatively simple, buffer-backed, typed (POD-only) hash table that
 * uses Robin Hood, open-power-of-two-hashing. This structure _requires_ a
 * resize function to be provided at construction time, as it does not make
 * sense for this style of hash table to be unable to automatically increase its
 * own size based on load factor or sub-optimal key distribution. It also lets
 * us get away with some crazy optimizations.
 *
 * The implementation was heavily influenced by designs outlined by Sebastian
 * Sylvan and Malte Skarupke, drawing on the core architecture of the first, and
 * numerous optimizations in the second. Links;
 * https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation/
 * https://probablydance.com/2017/02/26/i-wrote-the-fastest-hashtable/
 *
 * The first major optimization, Robin Hood hashing, is relatively well
 * understood; inserts into this table are allowed to move previously inserted
 * k/v pairs within the table, and will do so in an attempt to reduce the
 * average distance between the "natural" index a key hashes to, and the actual
 * cell the k/v pair is stored in.
 *
 * One unexpected side-effect of this is the inability to maintain referential
 * transparency in any context. The pathological case can be demonstrated by,
 *   1. Inserting a new k/v pair (KV0) with a natural cell index of 0
 *   2. Inserting a new k/v pair (KV1) with a natural cell index of 1
 *   3. Capturing a reference (R1) to the value in cell index 1
 *   4. Inserting a new k/v pair (KV2) with a natural cell index of 0
 *   5. Observing that R1 now corresponds to the value of KV2, rather than KV1.
 *
 * This occurs because the insertion of KV2 moves KV1 from cell 1 to cell 2, and
 * inserts KV2 into cell 1 (KV2 "steals from the rich" `distance == 0` KV1, s.t.
 * both k/v pairs are one cell away from their natural). The reference R1
 * remains locked to the value member of cell 1, and is therefore invalidated.
 * Iterator invalidation is well understood, but Get invalidation is a little
 * too much. As such, this hash table has a relatively simple
 * get/set/erase/contains interface that never returns references.
 *
 * A second major optimization allows us to entirely avoid bounds-checking. When
 * allocating cells for the table, we determine a maximum miss distance allowed
 * for the given table size. If a new insert would place a cell at
 * `max_miss_distance` away from its natural index, we resize the table before
 * proceeding. We can then allocate `max_miss_distance` extra cells past the end
 * of the table, ignore them when calculating the natural index for a key, and
 * know that we will never attempt to write into the very last cell. (Doing so
 * would require writing a k/v pair to a cell `max_miss_distance` away from the
 * last valid natural index, which we know will trigger a resize before the
 * insert completes.)
 *
 * A more subtle optimization related to the above lead us to 1-index a cell's
 * stored miss-distance; if the cell is placed in its natural cell, it's
 * distance will be 1, and a cell stored at the farthest distance will have a
 * distance of exactly `max_miss_distance`. This allows us to
 * `memset(_, '\0', _);` the entire cell table, and mark cells empty. This means
 * we start `distance` at 1 on get and set, and resize when `distance >
 * max_miss_distance`, rather than when `distance == max_miss_distance`.
 *
 * TODO: Handle the accidentally-quadratic resize issue by capturing entropic
 *       data on initialize / resize, and use that data to salt natural hashes.
 *       https://accidentallyquadratic.tumblr.com/post/153545455987/rust-hash-iteration-reinsertion
 * TODO: Potential optimization; if distance < current_cell->distance
 *       I don't think there's any chance we're looking at a match. Find out if
 *       the double comparison (`if (distance == current_cell->distance &&
 *       n2equals(key, current_cell->key)`) is a speedup on inserts.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "logging.h"
#include "crash.h"
#include "compare.h"
#include "hash.h"
#include "optional.h"
#include "mathutils.h"

#include "mem/buffer.h"

namespace mem {
namespace view {

template<typename T_KEY, typename T_VAL>
class HashTable {
protected: /*< ## Inner-Types */

    struct Cell {
        T_KEY key;
        T_VAL value;
        u8    distance;

        inline bool isEmpty()                { return distance == 0; }
        inline bool isInUse()                { return distance >  0; }
        inline bool isAtNaturalPosition()    { return distance == 1; }
        inline bool isNotAtNaturalPosition() { return distance >  1; }
    }; ENFORCE_POD(Cell);

    static const u32 magic = 0xBADB33F;
    struct Metadata {
        u32  magic;
        u64  capacity;
        u64  count;
        f32  max_load_factor;
        u8   max_miss_distance;
        bool rehash_in_progress;
        Cell map[];
    };


public: /*< ## Class Methods */
    static constexpr u64 default_capacity        = 64;
    static constexpr f32 default_max_load_factor = 0.6f;

    inline static u64 precomputeSize(u64 capacity = default_capacity) {
        // Round the requested capacity up to the nearest power-of-two, and then
        // tack on additional cells enough to handle the maximum miss distance.
        u64 required_capacity = next_power_of_two(capacity);
        u8  max_miss_distance = (u8)n2clamp(
            log2(required_capacity),
            1,
            std::numeric_limits<u8>::max()
        );
        return (sizeof (Metadata) +
                (sizeof(Cell) * (required_capacity + max_miss_distance)));
    }

    inline static void initializeBuffer(Buffer *const bd,
                                        f32 max_load_factor = 0) {
        Metadata * metadata = (Metadata *)(bd->data);
#if defined(DEBUG)
        N2CRASH_IF(bd->size < sizeof(Metadata), N2Error::InsufficientMemory,
                   "Buffer HashTable is being overlaid onto a Buffer that is "
                   "too small (" Fu64 "B) to fit the HashTable Metadata "
                   "(" Fu64 ").\n"
                   "Underlying buffer is named %s, and it is located at %p.",
                   bd->size, sizeof(Metadata), bd->name, bd);
        N2CRASH_IF(metadata->rehash_in_progress, N2Error::PEBCAK,
                   "Buffer HashTable has been reinitialized while "
                   "`rehash_in_progress == true`. This shouldn't be possible.\n"
                   "Underlying buffer is named %s, and it is located at %p.",
                   bd->name, bd);
        u64 required_size = precomputeSize(metadata->capacity);
        N2CRASH_IF(bd->size < required_size, N2Error::InsufficientMemory,
                   "Buffer HashTable is being overlaid onto a Buffer that is "
                   "too small (" Fu64 "B) to fit the whole table (" Fu64 "B).\n"
                   "Underlying buffer is named %s, and it is located at %p.",
                   bd->size, required_size, bd->name, bd);
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
        u64 total_capacity   = data_region_size / sizeof(Cell);
        metadata->magic              = magic;
        metadata->capacity           = previous_power_of_two(total_capacity);
        metadata->count              = 0;
        metadata->max_load_factor    = max_load_factor ?
                                       max_load_factor :
                                       default_max_load_factor;
        metadata->max_miss_distance  = (u8)log2(metadata->capacity);
        metadata->rehash_in_progress = false;
        memset(metadata->map, '\0', data_region_size);
#if defined(DEBUG)
        u64 used_capacity = (metadata->capacity + metadata->max_miss_distance);
        N2CRASH_IF(used_capacity > total_capacity, N2Error::InvalidMemory,
                   "Buffer HashTable has been initialized with a data region "
                   "that does not have room for overallocation. The data "
                   "region can store up to " Fu64 " cells. The natural "
                   "capacity is " Fu64 ", and the desired overflow is " Fu64
                   " -- totaling " Fu64 ".\n"
                   "Underlying buffer is named %s, and is located at %p.",
                   total_capacity, metadata->capacity,
                   metadata->max_miss_distance, used_capacity, bd->name, bd);
#endif
    }


public: /*< ## Contained-Type Accessors */
    using cell_type  = Cell;
    using key_type   = T_KEY;
    using value_type = T_VAL;


protected: /*< ## Public Member Variables */
    Buffer   *const m_bd;
    Metadata *      m_metadata;
    ResizeFn        m_resize;


public: /*< ## Ctors, Detors, and Assignments */
    HashTable(Buffer *const bd,
              ResizeFn resize)
        : m_bd       ( bd                      )
        , m_metadata ( (Metadata*)(m_bd->data) )
        , m_resize   ( resize                  ) { }


public: /*< ## Public Member Methods */
    inline u64 size()            { return m_bd->size; }
    inline u64 capacity()        { return m_metadata->capacity; }
    inline u64 count()           { return m_metadata->count; }
    inline f32 maxLoadFactor()   { return m_metadata->max_load_factor; }
    inline f32 maxLoadFactor(f32 factor) {
        return (m_metadata->max_load_factor = factor);
    }
    inline f32 loadFactor()      { return (f32)count() / (f32)capacity(); }
    inline u8  maxMissDistance() { return m_metadata->max_miss_distance; }
    inline u64 totalCapacity()   { return (capacity() + maxMissDistance()); }


    /* Calculate the natural index for the given key */
    inline u64 naturalIndexFor(T_KEY key) {
        return ( std::hash<T_KEY>{}(key) & (u64)(capacity() - 1) );
    }

    /* Get a pointer to the first cell in the table (iterator begin). */
    inline Cell * _beginCell() { return m_metadata->map; }
    /* Get a pointer to the past-the-end cell in the table (iterator end). */
    inline Cell * _endCell()   { return m_metadata->map + totalCapacity(); }


    /* Lookup Operations
     * ----------------- */

    /* Search for the given key, returning an Optional. */
    inline Optional<T_VAL> get(T_KEY key) {
        Cell * const cell = _findCell(key);
        if (cell != nullptr) return { cell->value };
        return { };
    }

    /* Check for the existence of the given key. */
    inline bool contains(T_KEY key) { return (_findCell(key) != nullptr); }


    /* Write Operations
     * ---------------- */

    /* Insert or update the given k/v pair. */
    inline void set(T_KEY key, T_VAL value) {
        _checkLoad();

        u64    cell_index   = naturalIndexFor(key);
        Cell * current_cell = m_metadata->map + cell_index;
        u8     distance     = 1;

        // If the number of misses (distance) is ever greater than the next
        // cell's recorded distance (current_cell->distance), we know that our
        // key cannot be in the table, as it would have stolen a previous cell.
        while (distance <= current_cell->distance) {
            //TODO: Potential optimization; if distance < current_cell->distance
            //      I don't think there's any chance we're looking at a match.
            if (n2equals(key, current_cell->key)) {
                current_cell->value = value;
                return;
            }
            current_cell += 1;
            distance     += 1;
        }

        while (true) {
            if (distance > maxMissDistance()) {
                N2CRASH_IF(m_metadata->rehash_in_progress, N2Error::PEBCAK,
                    "Attempting to resize a HashTable that has no associated "
                    "resize function due to an insert exceeding the maximum "
                    "miss distance (%u) _during a resize operation_.\n"
                    "How does that even happen?\n"
                    "Underlying buffer is named %s, and it is located at %p.",
                    maxMissDistance(), m_bd->name, m_bd);

                resizeBy(2.f);
                return set(key, value);
            }

            if (current_cell->isEmpty()) {
                current_cell->key      = key;
                current_cell->value    = value;
                current_cell->distance = distance;
                m_metadata->count += 1;

                return;
            }
            else if (distance > current_cell->distance) {
                std::swap(current_cell->key, key);
                std::swap(current_cell->value, value);
                std::swap(current_cell->distance, distance);
            }

            current_cell += 1;
            distance     += 1;
        }
    }

    /* Remove the given key from the HashTable.
     * No records are modified if the key has not been previously written. */
    inline bool erase(T_KEY key) {
        Cell * cell_to_erase = _findCell(key);
        Cell * next_cell     = cell_to_erase + 1;

        if (cell_to_erase) {
            // We can know that the last cell in the table will never be
            // written, so will never register true for `distance > 1`.
            while (next_cell->isNotAtNaturalPosition()) {
                std::swap(cell_to_erase->key, next_cell->key);
                std::swap(cell_to_erase->value, next_cell->value);

                cell_to_erase->distance = next_cell->distance - 1;

                cell_to_erase += 1;
                next_cell     += 1;
            }

            cell_to_erase->distance = 0;
            m_metadata->count -= 1;

            return true;
        }
        return false;
    }


    /* Storage Manipulations
     * --------------------- */

    /* Reset this HashTable to empty. */
    inline void drop() {
        memset(m_metadata->map, '\0', (totalCapacity() * sizeof(Cell)));
        m_metadata->count = 0;
    }

    /* Resize this HashTable to at least the given capacity (rounded up to the
     * nearest power of two). */
    inline void resizeTo(u64 new_capacity) {
        _resize(HashTable::precomputeSize(new_capacity));
    }

    /* Resize this HashTable by a given growth factor (rounded up to the nearest
     * power of two). */
    inline void resizeBy(f32 growth_factor) {
        u64 new_capacity = (u64)(this->capacity() * growth_factor);
        _resize(HashTable::precomputeSize(new_capacity));
    }


protected: /*< ## Protected Member Methods */

    /* Find the pointer to the cell associated with the given key, returning
     * nullptr if the key does not exist in the table. */
    inline Cell * _findCell(T_KEY key) {
        u64    cell_index   = naturalIndexFor(key);
        Cell * current_cell = m_metadata->map + cell_index;
        u8     distance     = 1;

        // If the number of misses (distance) is ever greater than the next
        // cell's recorded distance (current_cell->distance), we know that our
        // key cannot be in the table, as it would have stolen a previous cell.
        while (distance <= current_cell->distance) {
            //TODO: Potential optimization; if distance < current_cell->distance
            //      I don't think there's any chance we're looking at a match.
            if (n2equals(key, current_cell->key)) {
                return current_cell;
            }
            current_cell += 1;
            distance     += 1;
        }
        return nullptr;
    }


    /* Check the load factor for this table and resize if necessary */
    inline void _checkLoad() {
        bool overloaded = loadFactor() > maxLoadFactor();
        bool rehashing  = m_metadata->rehash_in_progress;
        if (!overloaded || rehashing) { return; }
        resizeBy(2.f);
    }

    /* Resize `this` HashTable to have room for exactly `capacity` elements.
     * This function should be able to both upscale and downscale HashTables. */
    inline void _resize(u64 new_size) {
        u64 data_region_size   = new_size - sizeof(Metadata);
        u64 new_total_capacity = data_region_size / sizeof(Cell);
        u64 new_capacity       = previous_power_of_two(new_total_capacity);

#if defined(DEBUG)
        N2CRASH_IF(m_bd->size < sizeof(Metadata), N2Error::InsufficientMemory,
            "Buffer HashTable is being resized into a Buffer that is too small "
            "(" Fu64 ") to fit the HashTable Metadata (" Fu64 ").\n"
            "Underlying buffer is named %s, and it is located at %p.",
            m_bd->size, sizeof(Metadata), m_bd->name, m_bd);
        N2CRASH_IF(new_capacity < count(), N2Error::InsufficientMemory,
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

        Buffer intermediate_bd = makeBuffer(intermediate_data, m_bd->size);
        memcpy(intermediate_bd.data, m_bd->data, m_bd->size);

        HashTable src(&intermediate_bd, nullptr);

        // Resize the backing buffer.
        // `realloc` "copies as much of the old data pointed to by `ptr` as will
        // fit [in]to the new allocation". This function('s DEBUG checks)
        // guarantee that we will have at least room enough for the previous
        // allocation's Metadata. No guarantees are made about the state of the
        // data region, or the current location of the Buffer.
        m_resize(m_bd, new_size);

        // Re-seat the Metadata member, re-set the Metadata members (save for
        // max_load_factor, which will remain correct), and zero-out the
        // HashTable data region.
        m_metadata                    = (Metadata*)(m_bd->data);
        m_metadata->count             = 0;
        m_metadata->capacity          = new_capacity;
        m_metadata->max_miss_distance = (u8)log2(new_capacity);
        memset(m_metadata->map, '\0', data_region_size);

        // Copy all data from the `src` HashTable into `this`.
        // Use `rehash_in_progress` to prevent the below `set` calls from
        // triggering a rehash while this one is completing.
        m_metadata->rehash_in_progress = true;
        for (auto&& item : src.items()) { set(item.key, item.value); }
        m_metadata->rehash_in_progress = false;

        // Discard temporary space.
        n2free(intermediate_data);
    }



    /* Nested Iterator and Related Functions
     * -------------------------------------
     */
public:
    struct KeyIterator;
    struct ValueIterator;
    struct ItemIterator;
    struct CellIterator;

private:
    struct KeyIteratorPassthrough;
    struct ValueIteratorPassthrough;
    struct ItemIteratorPassthrough;
    struct CellIteratorPassthrough;


public:
    typedef struct item_t {
        T_KEY const& key;
        T_VAL&       value;
    } T_ITEM;


    inline KeyIteratorPassthrough   keys()   { return { *this }; }
    inline ValueIteratorPassthrough values() { return { *this }; }
    inline ItemIteratorPassthrough  items()  { return { *this }; }
    inline CellIteratorPassthrough  cells()  { return { *this }; }


private:
    struct KeyIteratorPassthrough {
        HashTable & table;
        inline KeyIterator begin() { return { table, table._beginCell()}; }
        inline KeyIterator end()   { return { table, table._endCell()}; }
    };
    struct ValueIteratorPassthrough {
        HashTable & table;
        inline ValueIterator begin() { return { table, table._beginCell()}; }
        inline ValueIterator end()   { return { table, table._endCell()}; }
    };
    struct ItemIteratorPassthrough {
        HashTable & table;
        inline ItemIterator begin() { return { table, table._beginCell()}; }
        inline ItemIterator end()   { return { table, table._endCell()}; }
    };
    struct CellIteratorPassthrough {
        HashTable & table;
        inline CellIterator begin() { return { table, table._beginCell()}; }
        inline CellIterator end()   { return { table, table._endCell()}; }
    };

    /* Use the CRTP (Curiously Recurring Template Pattern) to return references
     * to the derived iterators when needed. */
    template<typename ITER_T>
    struct base_iterator {
    protected:
        HashTable & table;    /*< The HashTable being iterated.           */
        Cell *      data;     /*< The data currently referenced.          */
        Cell *      data_end; /*< The pointer past the end of the table.  */

        base_iterator(HashTable & _table,
                      Cell *      _data)
            : table    ( _table )
            , data     ( _data  )
            , data_end ( (table._endCell()) )
        {
            // The first Cell may be invalid. If so, make sure it's not returned
            while( data != data_end && data->isEmpty() ) { data += 1; }
        }

        inline void next_valid_cell() {
            do { data += 1; } while( data != data_end && data->isEmpty() );
        }
        inline void next_valid_cell(u64 n) {
            while (data != data_end && n > 0) {
                next_valid_cell();
                n -= 1;
            }
        }

    public:
        inline bool operator==(const ITER_T & other) const {
            return data == other.data;
        }
        inline bool operator!=(const ITER_T & other) const {
            return data != other.data;
        }
        /* Pre-increment -- step forward and return `this`. */
        inline ITER_T& operator++() {
            this->next_valid_cell();
            return *((ITER_T*)this);
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline ITER_T operator++(int) {
            ITER_T copy = *this;
            this->next_valid_cell();
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`. */
        inline ITER_T& operator+=(u64 n) {
            this->next_valid_cell(n);
            return *((ITER_T*)this);
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline ITER_T operator+(u64 n) {
            ITER_T copy = *this;
            copy->next_valid_cell(n);
            return copy;
        }
    };


public:
    struct KeyIterator : public base_iterator<KeyIterator> {
        KeyIterator(HashTable & _table, Cell * _data)
            : base_iterator<KeyIterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_KEY const& operator*() const {
            return this->data->key;
        }
    };

    struct ValueIterator : public base_iterator<ValueIterator> {
        ValueIterator(HashTable & _table, Cell * _data)
            : base_iterator<ValueIterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_VAL& operator*() {
            return this->data->value;
        }
    };

    struct ItemIterator : public base_iterator<ItemIterator> {
        ItemIterator(HashTable & _table, Cell * _data)
            : base_iterator<ItemIterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_ITEM operator*() {
            return { this->data->key, this->data->value };
        }
    };

    /* The Cell Iterator doesn't quite behave like the others, so we're not
     * going to inherit from the base iterator. */
    struct CellIterator {
    protected:
        HashTable & table;    /*< The HashTable being iterated.           */
        Cell *      data;     /*< The data currently referenced.          */
        Cell *      data_end; /*< The pointer past the end of the table.  */

    public:
        CellIterator(HashTable & _table,
                      Cell *      _data)
            : table    ( _table )
            , data     ( _data  )
            , data_end ( (table._endCell()) ) { }

        inline bool operator==(const CellIterator & other) const {
            return data == other.data;
        }
        inline bool operator!=(const CellIterator & other) const {
            return data != other.data;
        }
        /* Pre-increment -- step forward and return `this`. */
        inline CellIterator& operator++() {
            this->data += 1;
            return *this;
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline CellIterator operator++(int) {
            CellIterator copy = *this;
            this->data += 1;
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`. */
        inline CellIterator& operator+=(u64 n) {
            this->data = n2min((this->data + n), this->data_end);
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline CellIterator operator+(u64 n) {
            CellIterator copy = *this;
            copy->data = n2min((copy->data + n), copy->data_end);
            return copy;
        }
        //TODO: This should probably be a referene, not a ptr?
        /* Dereference -- return the current value. */
        inline Cell * const operator*() const {
            return this->data;
        }
    };


    /* Ensure that only POD data is used in these views.*/
    ENFORCE_POD(T_KEY);
    ENFORCE_POD(T_VAL);
};

} /* namespace view */
} /* namespace mem  */
