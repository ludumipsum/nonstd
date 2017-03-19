/* Buffer: Hash Table
 * ========================
 *
 * Map from an entity ID to a 32-bit unsigned index.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "compare.h"
#include "hash.h"
#include "logging.h"
#include "mathutils.h"

#include "api.h"
#include "buffer.h"

namespace buffer {

template<typename T_KEY, typename T_VAL>
class HashTable {
protected: /*< ## Inner-Types */
    struct Cell {
        T_KEY     key;
        T_VAL     value;
        i8        distance;

        inline bool is_empty()                   { return distance == -1; }
        inline bool is_in_use()                  { return distance >=  0; }
        inline bool is_at_natural_position()     { return distance ==  0; }
        inline bool is_not_at_natural_position() { return distance >   0; }
    }; ENFORCE_POD(Cell);

    static const u32 magic = 0xBADB33F;
    struct Metadata {
        u32  magic;
        u64  capacity;
        u64  count;
        u64  deleted_count;
        f32  max_load_factor;
        u8   max_miss_distance;
        bool rehash_in_progress;
        Cell map[];
    };

public: /*< ## Class Methods */
    static constexpr u64 default_capacity        = 64;
    static constexpr f32 default_max_load_factor = 0.6f;

    inline static u64 precomputeSize(u64 capacity = default_capacity) {
        auto required_capacity = next_power_of_two(capacity);
        return sizeof(Metadata) + sizeof(Cell) * required_capacity;
    }

    inline static void initializeBuffer(Descriptor *const bd,
                                        u64 max_load_factor = 0) {
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
        auto required_size = precomputeSize(metadata->capacity);
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
        u64 capacity         = data_region_size / sizeof(Cell);
        metadata->magic              = magic;
        metadata->capacity           = capacity;
        metadata->count              = 0;
        metadata->deleted_count      = 0;
        metadata->max_load_factor    = max_load_factor ?
                                       max_load_factor :
                                       default_max_load_factor;
        metadata->max_miss_distance  = log2(capacity);
        metadata->rehash_in_progress = false;
        memset(metadata->map, '\0', data_region_size);
        // As an optimization, all empty cells _must_ have their distance from
        // the natural hash set to -1
        //TODO: Improve the -1 distance optimzation
        for (auto&& cell : HashTable(bd).cells()) { cell->distance = -1; }
    }


public: /*< ## Contained-Type Accessors */
    using cell_type  = Cell;
    using key_type   = T_KEY;
    using value_type = T_VAL;


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
    inline u64 size()            { return m_bd->size; }
    inline u64 capacity()        { return m_metadata->capacity; }
    inline u64 count()           { return m_metadata->count; }
    inline u64 deleted_count()   { return m_metadata->deleted_count; }
    inline f32 max_load_factor() { return m_metadata->max_load_factor; }
    inline f32 max_load_factor(f32 factor) {
        return (m_metadata->max_load_factor = factor);
    }
    inline f32 load_factor() {
        f32 used = count() + deleted_count();
        return used / (f32)capacity();
    }
    inline u8  max_miss_distance() { return m_metadata->max_miss_distance; }


    /* Calculate the natural index for the key. */
    inline u64 natural_index_for(T_KEY key) {
        return ( n2hash(key) & (u64)(capacity() - 1) );
    }

    inline Cell * begin_cell() {
        return m_metadata->map;
    }
    inline Cell * end_cell() {
        return m_metadata->map + capacity();
    }


    /* Lookup Operations
     * ----------------- */

    /* Search for the given key, returning an Optional. */
    inline Optional<T_VAL&> operator[](T_KEY key) { return get(key); }
    inline Optional<T_VAL&> get(T_KEY key) {
        Cell * const cell = _find(key);
        if (cell != nullptr) return { cell->value };
        return { };
    }

    /* Check for the existence of the given key. */
    inline bool contains(T_KEY key) { return (_find(key) != nullptr); }


    /* Write Operations
     * ---------------- */

    /* Insert the given k/v pair, if the key has not yet been written.
     * The returned Optional will be false if the given key already exists in
     * the HashTable, or if the insert fails to find a valid bucket within
     * `max_miss_distance` probes. */
    inline Optional<T_VAL&> insert(T_KEY key, T_VAL value) {
        Cell * const cell = _insert(key, value);
        if (cell != nullptr) return { cell->value };
        return { };
    }

    /* Insert or update the given k/v pair.
     * The returned Optional will be only false if the insert fails to find a
     * valid bucket within `max_miss_distance` probes to hold a new k/v pair. */
    inline Optional<T_VAL&> set(T_KEY key, T_VAL value) {
        Cell * const cell = _insert_or_assign(key, value);
        if (cell != nullptr) return { cell->value };
        return { };
    }

    /* Remove the given key from the HashTable.
     * No records are written if the key has not been previously written. */
    inline bool erase(T_KEY key) {
        return _erase(key);
    }


    /* Storage Manipulations
     * --------------------- */

    /* Reset this HashTable to empty. */
    inline void drop() {
        memset(m_metadata->map, '\0', (capacity() * sizeof(Cell)));
        m_metadata->count = 0;
        m_metadata->deleted_count = 0;
    }

    /* Resize this HashTable to at least the given capacity (rounded up to the
     * nearest power of two). */
    inline void resize_to(u64 new_capacity) {
        _resize(HashTable::precomputeSize(new_capacity));
    }

    /* Resize this HashTable by a given growth factor (rounded up to the nearest
     * power of two). */
    inline void resize_by(f32 growth_factor) {
        u64 new_capacity = (u64)(this->capacity() * growth_factor);
        _resize(HashTable::precomputeSize(new_capacity));
    }


protected: /*< ## Protected Member Methods */
    /**
     * Check the load factor for this table and resize if necessary
     */
    inline void _check_load() {
        bool overloaded = load_factor() > max_load_factor();
        bool rehashing  = m_metadata->rehash_in_progress;
        if (!overloaded || m_resize == nullptr || rehashing) { return; }
        resize_by(size() * 2.f);
    }

    /**
     * Resize `this` HashTable to have room for exactly `capacity` elements.
     * This function should be able to both upscale and downscale HashTables.
     */
    inline void _resize(u64 new_size) {
        u64 data_region_size = new_size - sizeof(Metadata);
        u64 new_capacity     = data_region_size / sizeof(Cell);

#if defined(DEBUG)
        N2CRASH_IF(m_resize == nullptr, N2Error::NullPtr,
            "Attempting to resize a HashTable that has no associated "
            "resize function.\n"
            "Underlying buffer is named %s, and it is located at %p.",
            m_bd->name, m_bd);
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
        // max_load_factor, which will remain correct), and zero-out the
        // HashTable data region.
        m_metadata                    = (Metadata*)(m_bd->data);
        m_metadata->count             = 0;
        m_metadata->capacity          = new_capacity;
        // TODO: Verify log2 is correct.
        m_metadata->max_miss_distance = log2(new_capacity);
        memset(m_metadata->map, '\0', data_region_size);
        // As an optimization, all empty cells _must_ have their distance from
        // the natural hash set to -1
        //TODO: Improve the -1 distance optimzation
        for (auto&& cell : this->cells()) { cell->distance = -1; }

        // Copy all data from the `src` HashTable into `this`.
        // Use `rehash_in_progress` to prevent the below `_insert` calls from
        // triggering a rehash while this one is completing.
        m_metadata->rehash_in_progress = true;
        for (auto&& item : src.items()) { _insert(item.key, item.value); }
        m_metadata->rehash_in_progress = false;

        // Discard temporary space.
        n2free(intermediate_data);
    }


    inline Cell * _find(T_KEY key) {
        u64    cell_index   = natural_index_for(key);
        Cell * current_cell = m_metadata->map + cell_index;
        Cell * end_cell     = this->end_cell();
        i8     distance     = 0;

        while (distance <= current_cell->distance) {
            if (n2equals(key, current_cell->key)) {
                return current_cell;
            }
            current_cell += 1;
            distance     += 1;
            if (current_cell == end_cell) { current_cell = m_metadata->map; }
        }
        return nullptr;
    }


    inline Cell * const _insert(T_KEY key, T_VAL value) {
        _check_load();

        u64    cell_index   = natural_index_for(key);
        Cell * current_cell = m_metadata->map + cell_index;
        Cell * end_cell     = this->end_cell();
        i8     distance     = 0;

        while (distance <= current_cell->distance) {
            if (n2equals(key, current_cell->key)) {
                return nullptr;
            }
            current_cell += 1;
            distance     += 1;
            if (current_cell == end_cell) { current_cell = m_metadata->map; }
        }

        return _do_new_insert(key, value, current_cell, distance);
    }


    inline Cell * const _insert_or_assign(T_KEY key, T_VAL value) {
        _check_load();

        u64    cell_index   = natural_index_for(key);
        Cell * current_cell = m_metadata->map + cell_index;
        Cell * end_cell     = this->end_cell();
        i8     distance     = 0;

        while (distance <= current_cell->distance) {
            if (n2equals(key, current_cell->key)) {
                current_cell->value = value;
                return current_cell;
            }
            current_cell += 1;
            distance     += 1;
            if (current_cell == end_cell) { current_cell = m_metadata->map; }
        }

        return _do_new_insert(key, value, current_cell, distance);
    }


    inline Cell * const _do_new_insert(T_KEY key,
                                       T_VAL value,
                                       Cell * current_cell,
                                       i8 distance)
    {
        Cell * const end_cell = this->end_cell();

        while (true) {
            if ( current_cell->is_empty() )
            {
                current_cell->key      = key;
                current_cell->value    = value;
                current_cell->distance = distance;
                m_metadata->count += 1;

                return current_cell;
            }
            else if (distance > current_cell->distance)
            {
                std::swap(current_cell->key, key);
                std::swap(current_cell->value, value);
                std::swap(current_cell->distance, distance);
            }

            current_cell += 1;
            distance     += 1;
            if (current_cell == end_cell) { current_cell = m_metadata->map; }

            if (distance == m_metadata->max_miss_distance) {
                if (m_resize == nullptr || m_metadata->rehash_in_progress) {
                    return nullptr;
                }
                resize_by(2.f);
                return _insert(key, value);
            }
        }
    }


    inline bool _erase(T_KEY key) {
        Cell * cell_to_erase = _find(key);

        if (cell_to_erase) {
            Cell * end_cell  = this->end_cell();
            Cell * next_cell = cell_to_erase + 1;
            if (next_cell == end_cell) { next_cell = m_metadata->map; }

            while( next_cell->is_not_at_natural_position() ) {
                cell_to_erase->key      = next_cell->key;
                cell_to_erase->value    = next_cell->value;
                cell_to_erase->distance = next_cell->distance - 1;

                cell_to_erase = next_cell;
                next_cell +=  1;
                if (next_cell == end_cell) { next_cell = m_metadata->map; }
            }

            cell_to_erase->distance = -1;

            m_metadata->count -= 1;

            return true;
        }
        return false;
    }


    /* Nested Iterator and Related Functions
     * -------------------------------------
     */
public:
    struct key_iterator;
    struct value_iterator;
    struct item_iterator;
    struct cell_iterator;

private:
    struct key_iterator_passthrough;
    struct value_iterator_passthrough;
    struct item_iterator_passthrough;
    struct cell_iterator_passthrough;


public:
    typedef struct item_t {
        T_KEY const& key;
        T_VAL&       value;
    } T_ITEM;


    inline key_iterator_passthrough   keys()   { return { *this }; }
    inline value_iterator_passthrough values() { return { *this }; }
    inline item_iterator_passthrough  items()  { return { *this }; }
    inline cell_iterator_passthrough  cells()  { return { *this }; }


private:
    struct key_iterator_passthrough {
        HashTable & table;
        inline key_iterator begin() { return { table, table.begin_cell()}; }
        inline key_iterator end()   { return { table, table.end_cell()}; }
    };
    struct value_iterator_passthrough {
        HashTable & table;
        inline value_iterator begin() { return { table, table.begin_cell()}; }
        inline value_iterator end()   { return { table, table.end_cell()}; }
    };
    struct item_iterator_passthrough {
        HashTable & table;
        inline item_iterator begin() { return { table, table.begin_cell()}; }
        inline item_iterator end()   { return { table, table.end_cell()}; }
    };
    struct cell_iterator_passthrough {
        HashTable & table;
        inline cell_iterator begin() { return { table, table.begin_cell()}; }
        inline cell_iterator end()   { return { table, table.end_cell()}; }
    };

    /* Use the CRTP (Curiously Recurring Template Pattern) to return references
     * to the dervied iterators when needed. */
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
            , data_end ( (table.end_cell()) )
        {
            // The first Cell may be invalid. If so, make sure it's not returned
            while( data != data_end && data->is_empty() ) { data += 1; }
        }

        inline void next_valid_cell() {
            do { data += 1; } while( data != data_end && data->is_empty() );
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
    struct key_iterator : public base_iterator<key_iterator> {
        key_iterator(HashTable & _table, Cell * _data)
            : base_iterator<key_iterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_KEY const& operator*() const {
            return this->data->key;
        }
    };

    struct value_iterator : public base_iterator<value_iterator> {
        value_iterator(HashTable & _table, Cell * _data)
            : base_iterator<value_iterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_VAL& operator*() {
            return this->data->value;
        }
    };

    struct item_iterator : public base_iterator<item_iterator> {
        item_iterator(HashTable & _table, Cell * _data)
            : base_iterator<item_iterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_ITEM operator*() {
            return { this->data->key, this->data->value };
        }
    };

    /* The Cell Iterator doesn't quite behave like the others, so we're not
     * going to inherit from the base iterator. */
    struct cell_iterator {
    protected:
        HashTable & table;    /*< The HashTable being iterated.           */
        Cell *      data;     /*< The data currently referenced.          */
        Cell *      data_end; /*< The pointer past the end of the table.  */

    public:
        cell_iterator(HashTable & _table,
                      Cell *      _data)
            : table    ( _table )
            , data     ( _data  )
            , data_end ( (table.end_cell()) ) { }

        inline bool operator==(const cell_iterator & other) const {
            return data == other.data;
        }
        inline bool operator!=(const cell_iterator & other) const {
            return data != other.data;
        }
        /* Pre-increment -- step forward and return `this`. */
        inline cell_iterator& operator++() {
            this->data += 1;
            return *this;
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline cell_iterator operator++(int) {
            cell_iterator copy = *this;
            this->data += 1;
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`. */
        inline cell_iterator& operator+=(u64 n) {
            this->data = n2min((this->data + n), this->data_end);
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline cell_iterator operator+(u64 n) {
            cell_iterator copy = *this;
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

} /* namespace buffer */
