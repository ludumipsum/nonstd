/** Typed Buffer-Backed HashTable
 *  =============================
 *  This is a "relatively simple", buffer-backed, typed (POD-only) hash table
 *  that uses Robin Hood, open-power-of-two-hashing. This structure _requires_ a
 *  resize function to be provided at construction time, as it does not make
 *  sense for this style of hash table to be unable to automatically increase
 *  its own size based on sub-optimal key distribution. Guaranteeing a resize
 *  function also lets us get away with some crazy optimizations.
 *
 *  The implementation was heavily influenced by designs outlined by Sebastian
 *  Sylvan and Malte Skarupke, drawing on the core architecture of the first,
 *  and numerous optimizations in the second. Links;
 *  https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation/
 *  https://probablydance.com/2017/02/26/i-wrote-the-fastest-hashtable/
 *
 *  The first major optimization, Robin Hood hashing, is relatively well
 *  understood; inserts into this table are allowed to move previously inserted
 *  k/v pairs within the table, and will do so in an attempt to reduce the
 *  average distance between the "natural" index a key hashes to, and the actual
 *  cell the k/v pair is stored in.
 *
 *  One unexpected side-effect of this is the inability to maintain referential
 *  transparency in any context. The pathological case can be demonstrated by,
 *    1. Inserting a new k/v pair (KV0) with a natural cell index of 0
 *    2. Inserting a new k/v pair (KV1) with a natural cell index of 1
 *    3. Capturing a reference (R1) to the value in cell index 1
 *    4. Inserting a new k/v pair (KV2) with a natural cell index of 0
 *    5. Observing that R1 now corresponds to the value of KV2, rather than KV1.
 *
 *  This occurs because the insertion of KV2 moves KV1 from cell 1 to cell 2,
 *  and inserts KV2 into cell 1 (KV2 "steals from the rich" `distance == 0` KV1,
 *  s.t. both k/v pairs are one cell away from their natural). The reference R1
 *  remains locked to the value member of cell 1, and is therefore invalidated.
 *
 *  Iterator invalidation is well understood, but Get invalidation is a little
 *  too much. As such, this hash table has a relatively simple get / set /
 *  erase / contains interface that never returns references.
 *
 *  A second major optimization allows us to entirely avoid bounds-checking.
 *  When allocating cells for the table, we determine a maximum miss distance
 *  allowed for the given table size. If a new insert would place a cell at
 *  `max_miss_distance` away from its natural index, we resize the table before
 *  proceeding. This lets us allocate `max_miss_distance` extra cells past the
 *  end of the table, ignore them when calculating the natural index for a key,
 *  and know that we will never attempt to write into the very last cell. (Doing
 *  so would require writing a k/v pair to a cell `max_miss_distance` away from
 *  the last valid natural index, which we know will trigger a resize before the
 *  insert completes.) Never writing to the very last cell does mean we allocate
 *  one cell that will never be written to, but it also means that we always have
 *  a valid, dereferenceable, empty cell at the very end of the table.
 *
 *  A more subtle optimization related to the above lead us to 1-index a cell's
 *  stored miss-distance; if the cell is placed in its natural cell, it's
 *  distance will be 1, and a cell stored at the farthest distance will have a
 *  distance of exactly `max_miss_distance`. Empty cells can then be marked with
 *  a distance of 0. This allows us to `memset(_, '\0', _);` the entire cell
 *  table when initializing or resetting. This also means we start `distance` at
 *  1 on key searches, and resize when `distance > max_miss_distance`, rather
 *  than when `distance == max_miss_distance`.
 *
 *  TODO: Handle the accidentally-quadratic resize issue by capturing entropic
 *        data on initialize / resize, and use that data to salt natural hashes.
 *        https://accidentallyquadratic.tumblr.com/post/153545455987/rust-hash-iteration-reinsertion
 *  TODO: Potential optimization; if distance < current_cell->distance
 *        I don't think there's any chance we're looking at a match. Find out if
 *        the double comparison (`if (distance == current_cell->distance &&
 *        n2equals(key, current_cell->key)`) is a speedup on inserts.
 *  TODO: Should we be using std::hash or nonstd::hash? I think we're using
 *        std::hash right now?
 */

#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/math.h>
#include <nonstd/memory.h>
#include <nonstd/error.h>
#include <nonstd/optional.h>
#include <nonstd/type_traits_ext.h>
#include <nonstd/utility_ext.h>



namespace nonstd {

template<typename T_KEY, typename T_VAL>
class HashTable {
protected: /*< ## Inner-Types */

    struct Cell {
        T_KEY key;
        T_VAL value;
        u8    distance;

        inline bool isEmpty()                const noexcept { return distance == 0; }
        inline bool isInUse()                const noexcept { return distance >  0; }
        inline bool isAtNaturalPosition()    const noexcept { return distance == 1; }
        inline bool isNotAtNaturalPosition() const noexcept { return distance >  1; }
    };

    struct Metadata {
        u64  capacity;
        u64  count;
        u8   max_miss_distance;
        bool rehash_in_progress;
        Cell map[];
    };


public: /*< ## Class Methods */
    static constexpr u64 default_capacity = 64;

    static constexpr u8 maxMissDistanceFor(u64 capacity) noexcept {
        return n2max(log2(capacity), 1);
    }

    static constexpr u64 precomputeSize(u64 capacity = default_capacity)
    noexcept {
        using nonstd::roundUpToPowerOfTwo;

        // Round the requested capacity up to the nearest power-of-two, and then
        // tack on additional cells enough to handle the maximum miss distance.
        u64 target_capacity   = roundUpToPowerOfTwo(capacity);
        u64 max_miss_distance = maxMissDistanceFor(target_capacity);
        u64 total_capacity    = target_capacity + max_miss_distance;
        return sizeof(Metadata) + (sizeof(Cell) * total_capacity);
    }

    static inline buffer * initializeBuffer(buffer *const buf) {
        using nonstd::roundDownToPowerOfTwo;

        BREAK_IF(buf->type == buffer::type_id::hash_table,
            nonstd::error::reinitialized_memory,
            "buffer corruption detected by type_id; buffer has already been "
            "correctly initialized as a HashTable.\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            buf->name, buf);
        BREAK_IF(buf->type != buffer::type_id::raw,
            nonstd::error::invalid_memory,
            "buffer corruption detected by type_id; Attempting to initialize a "
            "previously initialized buffer. type_id is currently 0x{:X}\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            buf->type, buf->name, buf);

        Metadata * metadata = (Metadata *)(buf->data);

        u64 data_region_size     = buf->size - sizeof(Metadata);
        u64 data_region_capacity = data_region_size / sizeof(Cell);
        u64 practical_capacity   = roundDownToPowerOfTwo(data_region_capacity);
        u8  max_miss_distance    = maxMissDistanceFor(practical_capacity);

        u64 required_capacity = (practical_capacity + max_miss_distance);

        BREAK_IF(buf->size < sizeof(Metadata),
            nonstd::error::insufficient_memory,
            "This HashTable is being overlaid onto a buffer that is too small "
            "({} bytes) to fit the HashTable Metadata ({}).\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            buf->size, sizeof(Metadata), buf->name, buf);
        BREAK_IF(required_capacity > data_region_capacity,
            nonstd::error::insufficient_memory,
            "This HashTable has been initialized with a data region that does "
            "not have room for overallocation. The data region can store up to "
            "{} cells. The target capacity is {}, and the desired overflow is "
            "{} -- totaling {} cells.\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            data_region_capacity, practical_capacity, max_miss_distance,
            required_capacity, buf->name, buf);

        metadata->count              = 0;
        metadata->capacity           = practical_capacity;
        metadata->max_miss_distance  = max_miss_distance;
        metadata->rehash_in_progress = false;
        memset(metadata->map, '\0', data_region_size);

        buf->type = buffer::type_id::hash_table;

        return buf;
    }


public: /*< ## Contained-Type Accessors */
    using cell_type  = Cell;
    using key_type   = T_KEY;
    using value_type = T_VAL;


protected: /*< ## Public Member Variables */
    buffer   *const  m_buf;
    Metadata *&      m_metadata;


public: /*< ## Ctors, Detors, and Assignments */
    HashTable(buffer *const buf) noexcept
        : m_buf      ( buf                                       )
        , m_metadata ( reinterpret_cast<Metadata*&>(m_buf->data) )
    {
        /* Ensure that only POD types are used by placing ENFORCE_POD here. */
        ENFORCE_POD(T_KEY);
        ENFORCE_POD(T_VAL);
        ENFORCE_POD(Cell);

        ASSERT_M(m_buf->type == buffer::type_id::hash_table,
            "buffer ({}) '{}' has type_id 0x{:X}", m_buf, m_buf->name,
            m_buf->type);
    }
    HashTable(c_cstr name, u64 min_capacity = default_capacity)
        : HashTable ( memory::find(name)
                    ? *memory::find(name)
                    : initializeBuffer(
                            memory::allocate(name, precomputeSize(min_capacity))
                        )
                    )
    {
        ASSERT_M(m_buf->type == buffer::type_id::hash_table,
            "buffer ({}) '{}' has type_id 0x{:X}", m_buf, m_buf->name,
            m_buf->type);

        if (capacity() < min_capacity) { resize(min_capacity); }
    }

private: /*< ## Pseudo Type Traits */
    static constexpr bool hash_key_is_noexcept =
        noexcept(std::hash<T_KEY>{}(std::declval<T_KEY>()));
    static constexpr bool compare_key_is_noexcept =
        noexcept(nonstd::equal_to(std::declval<T_KEY>(), std::declval<T_KEY>()));
    static constexpr bool natural_index_for_is_noexcept = hash_key_is_noexcept;
    static constexpr bool find_cell_is_noexcept = natural_index_for_is_noexcept
                                               && compare_key_is_noexcept;

public: /*< ## Public Member Methods */
    /* ## buffer Accessors */
    inline buffer       * const buf()        noexcept { return m_buf; }
    inline buffer const * const buf()  const noexcept { return m_buf; }
    inline u64                  size() const noexcept { return m_buf->size; }
    inline c_cstr               name() const noexcept { return m_buf->name; }

    /* ## HashTable Accessors */
    inline u64 count()           const noexcept { return m_metadata->count; }
    inline u64 capacity()        const noexcept { return m_metadata->capacity; }
    inline u8  maxMissDistance() const noexcept { return m_metadata->max_miss_distance; }
    // The very last cell can never be written to, so we don't count it here.
    inline u64 totalCapacity()   const noexcept { return (capacity() + maxMissDistance() - 1); }
    inline f32 loadFactor()      const noexcept { return (f32)count() / (f32)capacity(); }


    /* Calculate the natural index for the given key */
    inline u64 naturalIndexFor(T_KEY key) const
    noexcept(natural_index_for_is_noexcept) {
        return ( std::hash<T_KEY>{}(key) & (u64)(capacity() - 1) );
    }

    /* Get a pointer to the first cell in the table (iterator begin). */
    inline Cell * _beginCell() const noexcept
    { return m_metadata->map; }
    /* Get a pointer to the "past-the-end" cell in the table (iterator end).
       NB. This is actually a pointer to the last cell, not the past-the-last,
           but because of the over-allocation optimizations in use, we know that
           last cell will never be written to, and therefore does not need to
           be iterated over. */
    inline Cell * _endCell() const noexcept
    { return m_metadata->map + totalCapacity(); }


    /* Lookup Operations
     * ----------------- */

    /* Search for the given key, returning an `optional<T_VAL>`. */
    inline nonstd::optional<T_VAL> get(T_KEY key)
    noexcept(find_cell_is_noexcept) {
        Cell * const cell = _findCell(key);
        if (cell != nullptr) return { cell->value };
        return { };
    }

    /* Check for the existence of the given key. */
    inline bool contains(T_KEY key)
    noexcept(find_cell_is_noexcept)
    { return (_findCell(key) != nullptr); }


    /* Write Operations
     * ---------------- */

    /* Insert or update the given k/v pair. */
    inline void set(T_KEY key, T_VAL value) {
        u64    cell_index   = naturalIndexFor(key);
        Cell * current_cell = m_metadata->map + cell_index;
        u8     distance     = 1;

        // Loop through cells until either,
        //  - The `key` is found -- which results in a simple update.
        //  - The miss-distance of the current search loop exceeds that of the
        //    `current_cell`. This implies implies `key` is not present in the
        //    table and will have to be added, potentially offsetting cells.
        while (distance <= current_cell->distance) {
            //TODO: Potential optimization; if distance < current_cell->distance
            //      I don't think there's any chance we're looking at a match.
            if (nonstd::equal_to(key, current_cell->key)) {
                current_cell->value = value;
                return;
            }
            current_cell += 1;
            distance     += 1;
        }

        // Loop through cells doing one of three things;
        //  - Inserting the current `key` and `value` into an empty cell (and
        //    terminating the current `set` operation).
        //  - Swapping the current `key` and `value` into a cell that has a
        //    miss-distance lower than that of the current search loop (which
        //    will reduce the miss-distance of the current search loop).
        //  - Pass over cells with a miss-distance equal (or lesser than,
        //    though I believe that's an impossible scenario) to the
        //    miss-distance of the current search loop.
        while (true) {
            if (distance > maxMissDistance()) {
                BREAK_IF(m_metadata->rehash_in_progress,
                    nonstd::error::pebcak,
                    "A resize operation has somehow caused additional "
                    "collisions in existing data, that has resulted in the "
                    "need for a second resize. Our hashtable implementation is "
                    "currently unable to accommodate multiple simultaneous "
                    "resize operations.\n"
                    "It should also be noted that in a single-threaded "
                    "power-of-two Robin Hood Hashing scheme that sets Max Miss "
                    "Distance to `log2(capacity)`, this should be "
                    "_completely impossible_.\n"
                    "Unless you're resizing downward? I think... this might be "
                    "possible in that case... Maybe?\n"
                    "Underlying buffer is named '{}', and it is located at {}.",
                    m_buf->name, m_buf);

                resize();
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
       No records are modified if the key has not been previously written. */
    inline bool erase(T_KEY key) {
        Cell * cell_to_erase = _findCell(key);
        Cell * next_cell     = cell_to_erase + 1;

        if (cell_to_erase) {
            // So long as the cell one past the one we're erasing is not in its
            // natural position, we should swap that cell with the one we're
            // erasing, and look to erase _that_ cell.
            // Additionally, because we know we always have an empty cell at the
            // very end of the table, we don't have to worry about the next_cell
            // being invalid, or out of its natural position.
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
    inline void drop() noexcept {
        memset(m_metadata->map, '\0', (totalCapacity() * sizeof(Cell)));
        m_metadata->count = 0;
    }

    /* Resize this HashTable to at least the given capacity (rounded up to the
       nearest power of two).
       If no capacity is given, double the current capacity. */
    inline void resize(u64 new_capacity = 0) {
        if (new_capacity == 0) { new_capacity = this->capacity() * 2; }
        BREAK_IF(new_capacity < capacity(), nonstd::error::unimplemented,
            "Downsizing HashTables is currently disallowed.");
        return _resize(HashTable::precomputeSize(new_capacity));
    }


protected: /*< ## Protected Member Methods */

    /* Find the pointer to the cell associated with the given key, returning
       nullptr if the key does not exist in the table. */
    inline Cell * _findCell(T_KEY key) const noexcept(find_cell_is_noexcept) {
        u64    cell_index   = naturalIndexFor(key);
        Cell * current_cell = m_metadata->map + cell_index;
        u8     distance     = 1;

        // If the number of misses (distance) is ever greater than the next
        // cell's recorded distance (current_cell->distance), we know that our
        // key cannot be in the table, as it would have stolen a previous cell.
        while (distance <= current_cell->distance) {
            //TODO: Potential optimization; if distance < current_cell->distance
            //      I don't think there's any chance we're looking at a match.
            if (nonstd::equal_to(key, current_cell->key)) {
                return current_cell;
            }
            current_cell += 1;
            distance     += 1;
        }
        return nullptr;
    }


    /* Resize this HashTable s.t. the backing buffer is exactly `new_size`.  */
    inline void _resize(u64 new_size) {
        using nonstd::roundDownToPowerOfTwo;

        u64 data_region_size      = new_size - sizeof(Metadata);
        u64 new_total_capacity    = data_region_size / sizeof(Cell);
        u64 new_capacity          = roundDownToPowerOfTwo(new_total_capacity);
        u8  new_max_miss_distance = maxMissDistanceFor(new_capacity);

#if defined(DEBUG)
        BREAK_IF(m_buf->size < sizeof(Metadata),
            nonstd::error::insufficient_memory,
            "buffer HashTable is being resized into a buffer that is too small "
            "({}) to fit the HashTable Metadata ({}).\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            m_buf->size, sizeof(Metadata), m_buf->name, m_buf);
        BREAK_IF(new_capacity < count(), nonstd::error::insufficient_memory,
            "Resizing a HashTable such that the new capacity ({}) is less than "
            "the current count ({}). This... is probably not okay. Data should "
            "be `destroy`d or `drop`d before downsizing?\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            new_capacity, count(), m_buf->name, m_buf);

        u64 used_capacity = new_capacity + new_max_miss_distance;
        u64 used_size     = sizeof(Metadata) + (sizeof(Cell) * used_capacity);
        BREAK_IF(new_size != used_size, nonstd::error::invalid_memory,
            "HashTable resize may be leaving data unaccessible;\n"
            "  requested size  : {}\n"
            "  calculated size : {}\n"
            "Underlying buffer is named '{}', and it is located at {}.",
            new_size, used_size, m_buf->name, m_buf);
#endif

        // Copy all current data aside to an intermediate `tmp_table` HashTable.
        // TODO: REPLACE ME WITH SCRATCH BUFFER USAGE

        // Allocate enough memory to re-create this HashTable in a temporary.
        ptr tmp_memory = n2malloc(m_buf->size);
        BREAK_IF(!tmp_memory,
            (std::error_code { errno, std::system_category() }),
            "Failed to `n2malloc` temporary memory. Godspeed.");

        // Wrap a local buffer around the temporary memory.
        buffer tmp_buffer = make_buffer(tmp_memory, m_buf->size);

        // Copy all of the current data into the temporary buffer, claim it has
        // been correctly initialized, and wrap it in a temporaray HashTable.
        memcpy(tmp_buffer.data, m_buf->data, m_buf->size);
        tmp_buffer.type = buffer::type_id::hash_table;
        HashTable tmp_table{ &tmp_buffer };

        // Resize the backing buffer.
        // Note that m_buf's data pointer may be changed as part of this call,
        // but m_metadata will still be valid as it is a type-aliased reference
        // to the data pointer.
        memory::resize(m_buf, new_size);

        // Reset the Metadata members and zero the data region to prime `this`
        // to be refilled.
        m_metadata->count             = 0;
        m_metadata->capacity          = new_capacity;
        m_metadata->max_miss_distance = new_max_miss_distance;
        memset(m_metadata->map, '\0', data_region_size);

        // Copy all data from the `tmp_table` HashTable into `this`.
        // Use `rehash_in_progress` to allow for some edge-case error checks.
        // TODO: `rehash_in_progress` is probably not necessary anymore...
        m_metadata->rehash_in_progress = true;
        for (auto&& item : tmp_table.items()) { set(item.key, item.value); }
        m_metadata->rehash_in_progress = false;

        // Discard temporary memory. The local buffer and HashTable will take
        // care of themselves as they fall out of scope.
        n2free(tmp_memory);
    }



    /* Nested Iterator and Related Functions
     * ------------------------------------- */
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
        T_KEY const & key;
        T_VAL       & value;
    } T_ITEM;


    inline KeyIteratorPassthrough   keys()   noexcept { return { *this }; }
    inline ValueIteratorPassthrough values() noexcept { return { *this }; }
    inline ItemIteratorPassthrough  items()  noexcept { return { *this }; }
    inline CellIteratorPassthrough  cells()  noexcept { return { *this }; }


private:
    struct KeyIteratorPassthrough {
        HashTable & table;

        inline KeyIterator begin() const noexcept
        { return { table, table._beginCell()}; }

        inline KeyIterator end()   const noexcept
        { return { table, table._endCell()}; }
    };
    struct ValueIteratorPassthrough {
        HashTable & table;

        inline ValueIterator begin() const noexcept
        { return { table, table._beginCell()}; }

        inline ValueIterator end()   const noexcept
        { return { table, table._endCell()}; }
    };
    struct ItemIteratorPassthrough {
        HashTable & table;

        inline ItemIterator begin() const noexcept
        { return { table, table._beginCell()}; }

        inline ItemIterator end()   const noexcept
        { return { table, table._endCell()}; }
    };
    struct CellIteratorPassthrough {
        HashTable & table;

        inline CellIterator begin() const noexcept
        { return { table, table._beginCell()}; }

        inline CellIterator end()   const noexcept
        { return { table, table._endCell()}; }
    };

    /* Use the CRTP (Curiously Recurring Template Pattern) to know the type of
       the inheriting class (`ITER_T`) when the template is resolved, and use
       that to return references to the derived iterators when needed.
       NB. The derived iterators implement the dereference operator. */
    template<typename ITER_T>
    struct base_iterator {
    protected:
        HashTable & table;    /*< The HashTable being iterated.           */
        Cell *      data;     /*< The data currently referenced.          */
        Cell *      data_end; /*< The pointer past the end of the table.  */

        base_iterator(HashTable & _table,
                      Cell *      _data) noexcept
            : table    ( _table )
            , data     ( _data  )
            , data_end ( (table._endCell()) )
        {
            // The first Cell may be invalid. If so, make sure it's not
            // dereferenced. But don't call `next_valid_cell`, because that
            // would always skip the first cell.
            while( data != data_end && data->isEmpty() ) { data += 1; }
        }

        inline void next_valid_cell() noexcept {
            do { data += 1; } while( data != data_end && data->isEmpty() );
        }
        inline void next_valid_cell(u64 n) noexcept {
            while (data != data_end && n > 0) {
                next_valid_cell();
                n -= 1;
            }
        }

    public:
        inline bool operator==(const ITER_T & other) const noexcept {
            return data == other.data;
        }
        inline bool operator!=(const ITER_T & other) const noexcept {
            return data != other.data;
        }
        /* Pre-increment -- step forward and return `this`. */
        inline ITER_T& operator++() noexcept {
            this->next_valid_cell();
            return *((ITER_T*)this);
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline ITER_T operator++(int) noexcept {
            ITER_T copy = *this;
            this->next_valid_cell();
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`. */
        inline ITER_T& operator+=(u64 n) noexcept {
            this->next_valid_cell(n);
            return *((ITER_T*)this);
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline ITER_T operator+(u64 n) noexcept {
            ITER_T copy = *this;
            copy->next_valid_cell(n);
            return copy;
        }
    };


public:
    struct KeyIterator : public base_iterator<KeyIterator> {
        KeyIterator(HashTable & _table, Cell * _data) noexcept
            : base_iterator<KeyIterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_KEY const& operator*() const {
            return this->data->key;
        }
    };

    struct ValueIterator : public base_iterator<ValueIterator> {
        ValueIterator(HashTable & _table, Cell * _data) noexcept
            : base_iterator<ValueIterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_VAL& operator*() noexcept {
            return this->data->value;
        }
    };

    struct ItemIterator : public base_iterator<ItemIterator> {
        ItemIterator(HashTable & _table, Cell * _data) noexcept
            : base_iterator<ItemIterator>(_table, _data) { }
        /* Dereference -- return the current value. */
        inline T_ITEM operator*() noexcept {
            return { this->data->key, this->data->value };
        }
    };

    /* The Cell Iterator doesn't quite behave like the others (it doesn't skip
       empty cells), so we're not going to inherit from the base iterator. */
    struct CellIterator {
    protected:
        HashTable & table;    /*< The HashTable being iterated.           */
        Cell *      data;     /*< The data currently referenced.          */
        Cell *      data_end; /*< The pointer past the end of the table.  */

    public:
        CellIterator(HashTable & _table,
                      Cell *      _data) noexcept
            : table    ( _table )
            , data     ( _data  )
            , data_end ( (table._endCell()) ) { }

        inline bool operator==(const CellIterator & other) const noexcept {
            return data == other.data;
        }
        inline bool operator!=(const CellIterator & other) const noexcept {
            return data != other.data;
        }
        /* Pre-increment -- step forward and return `this`. */
        inline CellIterator& operator++() noexcept {
            this->data += 1;
            return *this;
        }
        /* Post-increment -- return a copy created before stepping forward. */
        inline CellIterator operator++(int) noexcept {
            CellIterator copy = *this;
            this->data += 1;
            return copy;
        }
        /* Increment and assign -- step forward by `n` and return `this`. */
        inline CellIterator& operator+=(u64 n) noexcept {
            this->data = n2min((this->data + n), this->data_end);
        }
        /* Arithmetic increment -- return an incremented copy. */
        inline CellIterator operator+(u64 n) noexcept {
            CellIterator copy = *this;
            copy->data = n2min((copy->data + n), copy->data_end);
            return copy;
        }
        /* Dereference -- return the current value. */
        inline Cell& operator*() const noexcept {
            return *(this->data);
        }
    };
};

} /* namespace nonstd */
