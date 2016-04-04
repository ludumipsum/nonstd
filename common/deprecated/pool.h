/* Object Pooling
   ==============
  Freelist-based static size object pool based on Region<T>
*/
#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "logging.h"
#include "region.h"
#include <algorithm>

#include "../platform/crash.h"

// Bit pattern to select the index portion of an ID
#define INDEX_MASK 0xffff

// LSB of the object-id portion of an ID
#define OBJECT_LSB 0x10000

// Indirect index used to query object pools for a given object
struct PoolIndex {
    // General-purpose ID number -- encodes two pieces of information
    //  1. Record position in the index array (lower two bytes)
    //  2. Object identity / sequence number (upper two bytes)
    //
    // The object sequence number permits reuse of positions in the index array
    // while preserving the ability to detect dangling references (that is to
    // say, a stale ID used to access position in the index array that has been
    // reused will (correctly) error out).
    ID id;
    uint16_t index, next;
}; ENFORCE_POD(PoolIndex);


// Freelist-based object pool backed by a pair of memory regions
// TODO: Full docs here (e.g. Noting that `m_head == USHRT_MAX` marks an invalid
//       freelist)
// TODO: Consider implementing downsizng -- currently triggers EINVAL
template<typename T, bool IsResizable=false>
class Pool {
public:
    ID                             id;
    uint16_t                       m_head;
    uint16_t                       m_tail;
    // These, annoyingly, need to be before the memory regions to initialize
    // them to store region names before the regions are initialized. *sigh*
    std::string                    _m_object_region_name;
    std::string                    _m_index_region_name;
    Region<T, IsResizable>         m_objects;
    Region<PoolIndex, IsResizable> m_indices;
    char const*                    m_name;

    Pool(uint16_t count, char const* name)
        : id                    ( 0                                    )
        , m_head                ( 0                                    )
        , m_tail                ( count-1                              )
        , _m_object_region_name ( std::string(name) + "/Region"        )
        , _m_index_region_name  ( std::string(name) + "/Index"         )
        , m_objects             ( count, _m_object_region_name.c_str() )
        , m_indices             ( count, _m_index_region_name.c_str()  )
        , m_name                ( name                                 )
    {
        if (count == USHRT_MAX) {
            CRASH(ENOMEM, "Tried to initialize pool %s to be larger than "
                          "the max size of %d elements", m_name,
                          USHRT_MAX-1);
        }
        if (count == 0) {
            count = 1;
            m_tail = 0;
            // m_objects and m_indicies will have already bumped from 0 to 1
        }
        _initialize_freelist(count);
    }
    Pool(Pool const& other)
        : id                    ( other.id                    )
        , m_head                ( other.m_head                )
        , m_tail                ( other.m_tail                )
        , _m_object_region_name ( other._m_object_region_name )
        , _m_index_region_name  ( other._m_index_region_name  )
        , m_objects             ( other.m_objects             )
        , m_indices             ( other.m_indices             )
        , m_name                ( other.m_name                )
    { }
    Pool& operator=(Pool const& other)
    {
        id                      = other.id;
        m_head                  = other.m_head;
        m_tail                  = other.m_tail;
        _m_object_region_name   = other._m_object_region_name;
        _m_index_region_name    = other._m_index_region_name;
        m_objects               = other.m_objects;
        m_indices               = other.m_indices;
        m_name                  = other.m_name;
        return *this;
    }
    Pool(Pool&& other)
        : id                    ( other.id                               )
        , m_head                ( other.m_head                           )
        , m_tail                ( other.m_tail                           )
        , _m_object_region_name ( std::move(other._m_object_region_name) )
        , _m_index_region_name  ( std::move(other._m_index_region_name)  )
        , m_objects             ( std::move(other.m_objects)             )
        , m_indices             ( std::move(other.m_indices)             )
        , m_name                ( other.m_name                           )
    { }
    Pool& operator=(Pool&& other)
    {
        id                      = other.id;
        m_head                  = other.m_head;
        m_tail                  = other.m_tail;
        _m_object_region_name   = std::move(other._m_object_region_name);
        _m_index_region_name    = std::move(other._m_index_region_name);
        m_objects               = std::move(other.m_objects);
        m_indices               = std::move(other.m_indices);
        m_name                  = other.m_name;
        return *this;
    }
    ~Pool() = default;


    // Resize Policy

    // MSVC has an internal compiler error if things outside the class
    // definition call _resize directly. This, otoh, works.
    void resize(uint16_t new_size) {
        _resize(new_size);
    }

    TEMPLATE_ENABLE((IsResizable == false), T)
    inline void _resize(uint16_t new_size) {
        UNUSED(new_size);
        CRASH(ENOMEM, "Pool %s is non-resizable. Attempted resize from " PRIu64
                      " to " PRIu64 ".",
                      m_name);
    }

    TEMPLATE_ENABLE(IsResizable, T)
    inline void _resize(uint16_t new_size) {
        uint16_t old_size = (uint16_t) m_indices.capacity();

        // Early-Out conditions
        if (new_size == old_size) return;
        if (new_size <  old_size) {
            CRASH(EINVAL, "Shrinking object Pools is not supported. (Tried to "
                          "resize \"%s\" from %d to %d.)",
                          m_name, old_size, new_size);
        }
        #if defined(DEBUG_MEMORY)
            if (new_size >= USHRT_MAX) {
                CRASH(ENOMEM, "Tried to resize pool %s to be larger than the "
                              "max size of %d elements", m_name, USHRT_MAX-1);
            }
        #endif

        // Explicitly resize the index region (let the object region float)
        m_indices.reserve(new_size);

        // Fixup the freelist...
        // Initialize the new elements of the index array.
        this->_initialize_freelist((new_size - old_size), old_size);

        if (m_head == USHRT_MAX) {
            // If there isn't currently a freelist, simply set the head to the
            // start of the new segment...
            m_head = old_size;
        } else {
            // ... otherwise, append the newly initialized segment to the
            // tail of the current list...
            m_indices[m_tail].next = old_size;
        }
        // ... in either case, the tail becomes the end of the new segment.
        m_tail = new_size-1;
    }

    // Random-access via id through index array
    inline T& lookup(ID id) const {
        #if defined(DEBUG_MEMORY)
            if (!contains(id)) {
                CRASH(EFAULT, "Memory pool bounds-check failed; id %d "
                              "is past the end of the object table ("
                              "%d > %" PRIuPTR ").");
            }
        #endif
        return m_objects[m_indices[id & INDEX_MASK].index];
    }
    inline T& operator[](ID id) const { return lookup(id); }

    // Iterator-access for underlying object region
    inline T* begin(void) { return m_objects.begin(); }
    inline T* end(void) { return m_objects.end(); }
    inline T* region_end(void) { m_objects.region_end(); }

    // Arbitrary-predicate sort, updates index array on swap
    inline void sort(std::function<bool(T&, T&)> predicate) {
        auto& indices = m_indices;
        auto swap = [&indices](T& a, T& b) -> void {
            PoolIndex& a_idx = indices[a.id & INDEX_MASK];
            PoolIndex& b_idx = indices[b.id & INDEX_MASK];

            assert(a_idx.index != USHRT_MAX);
            assert(b_idx.index != USHRT_MAX);

            // Swap indices
            auto tmp_idx = a_idx.index;
            a_idx.index = b_idx.index;
            b_idx.index = tmp_idx;

            // Swap objects
            T tmp = std::move(a);
            a = std::move(b);
            b = std::move(tmp);
        };
        m_objects.sort(predicate, swap);
    }

    // Query an ID to see if the entry is valid.
    inline bool contains(ID id) const {
        #if defined(DEBUG_MEMORY)
            // If no object entries are used, all IDs are invalid
            if (m_objects.used() == 0) return false;
            // If a number that was never a valid ID was passed, return false
            if ((id & INDEX_MASK) > m_indices.used()) return false;
        #endif

        //TODO: Find out if we can trigger an invalid access with a constructed
        // ID. Catch/prevent such errors?
        // Convenience reference to the corresponding index entry
        PoolIndex &index_entry = m_indices[id & INDEX_MASK];

        // Two conditions produce an invalid reference -- either the index entry
        // has been reused, in which case the id field will differ, or the index
        // entry is in the freelist, in which case the index entry's index field
        // (the index into the objects array) will be USHRT_MAX.
        return index_entry.id == id && index_entry.index != USHRT_MAX;
    }

    // Create a new object and return its ID
    template<typename ...ctor_arg_types>
    inline ID create(ctor_arg_types && ... _ctor_args) {
        // If the pool is full, attempt a resize
        if (m_head == USHRT_MAX) {
            uint16_t new_size = n2min(m_indices.capacity() * 1.2f + 1,
                                    USHRT_MAX - 1);
            _resize(new_size);
        }

        // Pull an index entry off the freelist
        PoolIndex &idx = m_indices[m_head];

        // Update the freelist
        if (m_head != m_tail) {
            m_head = idx.next;
        } else {
            // If this is the last free index (m_head == m_tail, but neither
            // equal to USHRT_MAX), mark the freelist invalid
            m_head = USHRT_MAX;
            m_tail = USHRT_MAX;
        }

        // Increment the object id sequence number (provides index collision
        // detection)
        idx.id += OBJECT_LSB;

        // Fetch the next available entry in the object region
        idx.index = (uint16_t) m_objects.used();

        // Construct the object in-place
        T& object = m_objects.construct(std::forward<ctor_arg_types>(_ctor_args)...);
        object.id = idx.id;

        #if defined(DEBUG_MEMORY)
            idx.next = USHRT_MAX;
        #endif

        return object.id;
    }

    // Create a new object at the specified ID, returning the ID. If an object
    // already exists at that ID, this is a no-op.
    template<typename ...ctor_arg_types>
    inline ID create_at(ID id, ctor_arg_types && ... _ctor_args) {
        #if defined(DEBUG_MEMORY)
        if ((id & INDEX_MASK) > USHRT_MAX-2) {
            CRASH(EINVAL, "Pools can hold at most %d elements, with the "
                          "greatest representable ID of %d. You requested "
                          "an index entry with id %d.",
                          USHRT_MAX-1, USHRT_MAX-2, id & INDEX_MASK);
        }
        #endif

        // If we're being asked to create an entity for an ID past the end of
        // our useful range, grow the index array
        if ((id & INDEX_MASK) >= m_indices.capacity()) {
            uint16_t new_size = n2min((id & INDEX_MASK) * 1.2f, USHRT_MAX - 1);
            _resize(new_size);
        }

        // Locate this ID's index in the pool freelist and pull it out
        PoolIndex *cursor = &m_indices[m_head];
        if ((id & INDEX_MASK) == m_head) {
            // We're replacing the head of the freelist, just move m_head
            m_head = cursor->next;
            cursor->next = USHRT_MAX;
            if (m_head == USHRT_MAX) {
              // If this was the last element in the freelist, mark the tail
              m_tail = USHRT_MAX;
            }
        } else {
            // We're replacing out of the middle or end of the freelist, so we
            // need to find the previous entry and fix it up to point to our
            // next element.
            while(cursor->next != USHRT_MAX &&
                    (m_indices[cursor->next].id & INDEX_MASK)
                 != (id & INDEX_MASK)) {
                cursor = &m_indices[cursor->next];
            }
            if (cursor->next != USHRT_MAX) {
                // Once the scan is complete, cursor points to the freelist
                // element before the one we're removing. Before we patch up
                // the list, though, we need to move m_tail if that's what we're
                // replacing.
                if (m_tail == cursor->next) {
                    m_tail = cursor->id & INDEX_MASK;
                }
                cursor->next = m_indices[cursor->next].next;
            } else {
                // Otherwise the index element is already in use. Fail out
                CRASH(EFAULT, "Trying to reuse already-in-use index %d in "
                              "pool %s.", id & INDEX_MASK, m_name);
            }
        }

        // Update the index entry and create the object
        PoolIndex& idx = m_indices[id & INDEX_MASK];
        idx.id = id;
        idx.index = (uint16_t) m_objects.used();
        T& object = m_objects.construct(std::forward<ctor_arg_types>(_ctor_args)...);
        object.id = idx.id;
        return object.id;
    }

    // Mark a given object invalid and available for reuse
    inline void destroy(ID id) {
        // Bind the corresponding index
        PoolIndex &idx = m_indices[id & INDEX_MASK];

        // Assert that we're not trying to read an invalid index
        assert(idx.index != USHRT_MAX);

        // Bind a local reference to the object slot we're destroying
        T& object = m_objects[idx.index];

        // Conditionally destruct the object that we're destorying.
        // (Template conditional on whether or not T is POD)
        _placement_delete(object);

        // Move the last live object in the pool in to this slot
        object = std::move(m_objects[--m_objects.m_next]);

        // Update the moved object's index entry to point to the new location
        m_indices[object.id & INDEX_MASK].index = idx.index;

        // Mark the old index unused and add it to the freelist
        idx.index = USHRT_MAX;
        if (m_head == USHRT_MAX) {
            // In the degenerate case where the freelist contains no valid
            // elements we need to fix up the freelist head and tail to point at
            // the newly liberated index record.
            m_head = m_tail = (idx.id & INDEX_MASK);
        } else {
            m_indices[m_tail].next = id & INDEX_MASK;
            m_tail = id & INDEX_MASK;
        }
    }

    // If the object is POD, this function is a noop.
    TEMPLATE_ENABLE(std::is_pod<T>::value, T)
    inline void _placement_delete(T& object) {
        UNUSED(object);
    }

    // If it's not POD, manually call the destructor.
    TEMPLATE_ENABLE(!std::is_pod<T>::value, T)
    inline void _placement_delete(T& object) {
        object.~T();
    }

    // Drop all existing data from this table by re-adding all index
    // entries to the freelist. This does not reinitialize any objects,
    // it simply logically marks them as available by freeing indices.
    // NOTE: this freelist init is copypasta from the ctor
    inline void drop() {
        m_indices.drop();
        m_objects.drop();
        m_head = 0;
        m_tail = (uint16_t) m_indices.capacity() - 1;
        _initialize_freelist((uint16_t) m_indices.capacity());
    }

    // This pool's name
    inline char const* name() { return m_name; }

    // The number of objects currently alive in this pool
    inline uint16_t used() { return (uint16_t) m_objects.used(); }

    // The maximum number of objects this pool can contain
    inline uint16_t capacity() { return (uint16_t) m_indices.capacity(); }

    // The number of bytes used by live objects in the pool
    inline uint64_t used_bytes() { return m_objects.bytes_used(); }

    // The number of bytes used for pool bookkeeping
    inline uint64_t overhead_bytes() { return sizeof(decltype(*this)); }

    // Total bytes allocated for this pool
    inline uint64_t total_bytes() { return sizeof(decltype(*this)) + m_indices.capacity_bytes() + m_objects.capacity_bytes(); }

    // Log this pool's memory stats at the provided severity
    // TODO: Move to generating a struct instead of "lol print it"
    inline void logStats() {
        LOG("Memory stats for pool %s\n"
            "\tMEMORY USE/NAME  TOTAL BYTES  USEFUL BYTES  USED BYTES  OVERHEAD\n"
            "\t---------------  -----------  ------------  ----------  --------\n"
            "\tObject Region:   %11" PRIu64 "  %12" PRIu64 "  %10" PRIu64 "  %8" PRIu64 "\n"
            "\tIndex Region:    %11" PRIu64 "  %12" PRIu64 "  %10" PRIu64 "  %8" PRIu64 "\n"
            "\tPool Overhead:           ---           ---         ---  %8" PRIu64 "\n"
            "\tΣ -> %.2fkB across 3 memory locations with %.2f%% overhead",
            m_name,
            m_objects.capacity_bytes()+sizeof(m_objects), m_objects.capacity_bytes(), m_objects.used_bytes(), sizeof(m_objects),
            m_indices.capacity_bytes()+sizeof(m_indices), m_indices.capacity_bytes(), m_indices.used_bytes(), sizeof(m_indices),
            overhead_bytes(),
            float(total_bytes())/1024.0f,
            100 * float(overhead_bytes() + m_indices.capacity_bytes())
                / float(total_bytes()));
    }

    // Log memory stats for this pool's constituent regions
    // TODO: Move to generating a struct instead of "lol print it"
    inline void logRegionStats() {
        m_indices.logStats();
        m_objects.logStats();
    }

protected:
    // Initialization logic for overlaying a freelist on the pool index table
    inline void _initialize_freelist(uint16_t count, uint16_t start = 0) {
        #if defined(DEBUG_MEMORY)
            if (count + start > m_indices.capacity()) {
                CRASH(EFAULT, "Tried to initialize pool freelist with an "
                              "end point past the size of the index array");
            }
        #endif

        uint16_t end = start + count;
        for (uint16_t i = start; i < end; /**/) {
            m_indices.construct(PoolIndex{ i, 0, ++i });
        }

        #if defined(DEBUG_MEMORY)
            // Set the last index's freelist pointer to be invalid.
            m_indices[end-1].next = USHRT_MAX;
        #endif
    }
};
