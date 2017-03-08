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
        f32  max_load_factor;
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
        N2CRASH_IF(bd->size < sizeof(Metadata), Error::InsufficientMemory,
                   "Buffer HashTable is being overlaid onto a Buffer that is "
                   "too small (" Fu64 "B) to fit the HashTable Metadata "
                   "(" Fu64 ").\n"
                   "Underlying buffer is named %s, and it is located at %p.",
                   bd->size, sizeof(Metadata), bd->name, bd);
        N2CRASH_IF(metadata->rehash_in_progress, Error::PEBCAK,
                   "Buffer HashTable has been reinitialized while "
                   "`rehash_in_progress == true`. This shouldn't be possible.\n"
                   "Underlying buffer is named %s, and it is located at %p.",
                   bd->name, bd);
        auto required_size = precomputeSize(metadata->capacity);
        N2CRASH_IF(bd->size < required_size, Error::InsufficientMemory,
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
        metadata->invalid_count      = 0;
        metadata->max_load_factor    = max_load_factor ?
                                       max_load_factor :
                                       default_max_load_factor;
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
    inline u64 size()            { return m_bd->size; }
    inline u64 capacity()        { return m_metadata->capacity; }
    inline u64 count()           { return m_metadata->count; }
    inline u64 invalid_count()   { return m_metadata->invalid_count; }
    inline f32 max_load_factor() { return m_metadata->max_load_factor; }
    inline f32 load_factor()     {
        f64 used = count() + invalid_count();
        return used / (f64)capacity();
    }

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
        _check_load();
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
        _check_load();
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

    /**
     * Constrain a hash value down to an entry in the cell table.
     * TODO: Consider making this protected (again), or exposing it in a
     *       different way
     */
    inline u64 constrain_key(u64 hash) {
        return hash & (u64)(capacity() - 1);
    }


protected: /*< ## Protected Member Methods */
    /**
     * Check the load factor for this table and resize if necessary
     */
    inline void _check_load() {
        bool overloaded = load_factor() > max_load_factor();
        if (!overloaded || m_metadata->rehash_in_progress) { return; }
        _resize(size() * 2);
    }

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
        // max_load_factor, which will remain correct), and zero-out the
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
     *TODO: Refactor should_resize out of this function (and probably stop
     *      tracking misses, too).
     */
    inline Cell *const _lookup_cell(T_KEY key) {
        auto&  map         = m_metadata->map;
        auto   hash        = n2hash(key);
        // Initial index for the given key.
        auto   cell_index  = constrain_key(hash);
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
        // If we loop in this section for `capacity()` times, we, rather than
        // returning, will resize the entire HashTable (thereby rehashing all
        // keys), then reenter this function with the same key.
        while(misses < capacity()) {
            Cell& cell = map[cell_index];
            bool cell_is_empty = cell.state   == CellState::EMPTY;
            bool match_found   = (cell.state  == CellState::USED
                                  && n2equals(cell.key, key));

            if (match_found || cell_is_empty) {
                ret = &cell;
                break;
            }

            cell_index  = constrain_key(1 + cell_index);
            misses     += 1;
        }

        // The termination of this function is a pretty complicated piece of
        // state checking. We expect the only outcomes to be,
        // 1. Returning a  valid `Cell *`
        // 2. Returning a `nullptr`
        // 3. Resizing the HashTable, and re-entering this function.
        //
        // I was having a lot of trouble keeping the potential outcomes in my
        // head, so I went ahead and whipped up this logic table.
        //
        //     | Outcome | ret | should_resize | m_resize | resize_in_progress |
        //     |---------|-----|---------------|----------|--------------------|
        //     | ret     | Yes | No            | No       | No                 |
        //     | ret     | Yes | No            | Yes      | No                 |
        //     | ret     | Yes | No            | Yes      | Yes                |
        //     | ret     | Yes | Yes           | Yes      | Yes                |
        //     | ret     | Yes | Yes           | No       | No                 |
        //     | Resize  | Yes | Yes           | Yes      | No                 |
        //     | nul     | No  | Yes           | No       | No                 |
        //     | Resize  | No  | Yes           | Yes      | No                 |
        //     | Error   | No  | Yes           | Yes      | Yes                |
        //
        // Note that all unlisted combinations are what I consider to be invalid
        // states because,
        // 1. `ret == nullptr` (ret == No) implies `should_resize`.
        // 2. `resize_in_progress` implies `m_resize`.
        //
        // The below if-soup was written in an attempt to be optimistic; the
        // most common results are (hopefully) written first. This was done
        // assuming branch prediction will be optimistic, and that optimizing
        // for correct branch predictions will meaningfully improve system
        // performance.
        bool should_resize = misses >= capacity();
        bool rehash_in_progress = m_metadata->rehash_in_progress;

        if (ret) {
            if (! should_resize     ||
                m_resize == nullptr ||
                rehash_in_progress)
            {
                return ret;
            }
            resize_by(1.2f);
            return _lookup_cell(key);

        } else if (ret == nullptr) {

            if (m_resize == nullptr) {
                return nullptr;
            }
            if (should_resize       &&
                m_resize != nullptr &&
                ! rehash_in_progress)
            {
                resize_by(1.2f);
                return _lookup_cell(key);
            }
            if (m_resize != nullptr &&
                rehash_in_progress)
            {
                N2CRASH(Error::PEBCAK,
                    "This HashTable has somehow entered a bad state. A resize "
                    "is in progress, but the table has been completely filled. "
                    "This is supposed to be an impossible state, but... Here "
                    "we are.\n"
                    "Underlying buffer is named %s, and it is located at %p.\n",
                    m_bd->name, m_bd);
            }
        }

        N2CRASH(Error::PEBCAK,
            "Whoever wrote this lookup function thought there was no way to "
            "get to this portion of code. And yet here we are.\n"
            "The backing buffer's name is %s and is located at %p.",
            m_bd->name, m_bd);
        return nullptr;
    }

    /* Contained-Type Accessors
     * ------------------------
     */
public:
    using cell_type  = Cell;
    using key_type   = T_KEY;
    using value_type = T_VAL;


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
        inline key_iterator begin() { return { table, table.m_metadata->map}; }
        inline key_iterator end()   {
            return { table, (table.m_metadata->map + table.capacity())};
        }
    };
    struct value_iterator_passthrough {
        HashTable & table;
        inline value_iterator begin() { return { table, table.m_metadata->map}; }
        inline value_iterator end()   {
            return { table, (table.m_metadata->map + table.capacity())};
        }
    };
    struct item_iterator_passthrough {
        HashTable & table;
        inline item_iterator begin() { return { table, table.m_metadata->map}; }
        inline item_iterator end()   {
            return { table, (table.m_metadata->map + table.capacity())};
        }
    };
    struct cell_iterator_passthrough {
        HashTable & table;
        inline cell_iterator begin() { return { table, table.m_metadata->map}; }
        inline cell_iterator end()   {
            return { table, (table.m_metadata->map + table.capacity())};
        }
    };

    /* Use the CRTP (Curiously Recurring Template Pattern) to return references
     * to the dervied iterators when needed. */
    template<typename ITTR_T>
    struct base_iterator {
    protected:
        HashTable & table;    /*< The HashTabl being iterated.           */
        Cell *      data;     /*< The data currently referenced.         */
        Cell *      data_end; /*< The pointer past the end of the table. */

        base_iterator(HashTable & _table,
                     Cell *       _data)
            : table    ( _table )
            , data     ( _data  )
            , data_end ( (table.m_metadata->map + table.capacity()) )
        {
            // The first Cell may be invalid. If so, make sure it's not returned
            while( data != data_end &&
                   data->state != CellState::USED ) {
                data += 1;
            }
        }

        inline void next_valid_cell() {
            do {
                data += 1;
            } while( data != data_end && data->state != CellState::USED );
        }
        inline void next_valid_cell(u64 n) {
            while (data != data_end && n > 0) {
                next_valid_cell();
                n -= 1;
            }
        }

    public:
        inline bool operator==(const base_iterator & other) const {
            return &table == &other.table && data == other.data;
        }
        inline bool operator!=(const base_iterator & other) const {
            return &table != &other.table || data != other.data;
        }
        /* Pre-increment -- step forward and return `this`. */
        inline ITTR_T& operator++() {
            this->next_valid_cell();
            return *((ITTR_T*)this);
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline ITTR_T operator++(int) {
            ITTR_T copy = *this;
            this->next_valid_cell();
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`. */
        inline ITTR_T& operator+=(u64 n) {
            this->next_valid_cell(n);
            return *((ITTR_T*)this);
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline ITTR_T operator+(u64 n) {
            ITTR_T copy = *this;
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

    struct cell_iterator : public base_iterator<cell_iterator> {
        cell_iterator(HashTable & _table, Cell * _data)
            : base_iterator<cell_iterator>(_table, _data)
        {
            // We don't want to skip over invalid cells, so reset this->data to
            // whatever was initially set.
            this->data = _data;
        }
        /* Dereference -- return the current value. */
        inline Cell * const operator*() const {
            return this->data;
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
    };


    /* Ensure that only POD data is used in these views.*/
    ENFORCE_POD(T_KEY);
    ENFORCE_POD(T_VAL);
};

} /* namespace buffer */
