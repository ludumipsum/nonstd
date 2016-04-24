/* Memory Regions
   ==============

   Regions provide monotonically-growing memory consumption. This doesn't sound
   super useful at first glance, but they're helpful any time you know you're
   going to have a lot of objects allocated, and you'd like to discard them
   all at once.

   Regions are also used as the backing store for [Pools](pool.html).

   Regions are parameterized over the contained object type, and a resize
   policy, which dictates how, or if, a region may be resized when full.

   **Notes**
   1.  We only report ENOMEM on Windows, because an EINVAL
       crash for invalid allocation parameters will yield to
       the current invalid parameter handler before hitting
       the CRASH statement. This should be configured to point
       to our crash dump/reporter instead of MSFT's watson.
*/
#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "logging.h"

#include "../platform/crash.h"

template<typename T, bool IsResizable> class Region;
template<typename T, bool IsResizable> class Pool;

// Region
// ------
//
// Regions are type-generic memory block allocators. As long as the Region
// object is in scope, all the data contained in the memory block remains
// accessible.
//
// When a region drops out of scope, the whole block is freed _without_ calling
// any destructors. For this reason, `Region<T>` requires that T be Plain Old
// Data, as a convenient shorthand for trivially destructible.
//
// Regions may be freely *moved*, but may not be implicitly copied. If you need
// to copy a region, a *duplicate* method is available.
//
// Non-POD types aren't (quite) supported yet. Construction is in place, but
// moving objects in/out of the collection, and destruction are not yet
// handled.
template<typename T, bool IsResizable=false>
class Region {
public:
    ID     id;
    bool   m_aligned;
    u64    m_next;
    u64    m_size;
    T *    m_buffer;
    c_cstr m_name;

protected:
    friend class Pool<T, IsResizable>;

public:
    // ### Region Lifespan Management

    // Create a new memory region to hold a given
    Region(u64 count, c_cstr name)
        : id(0)
        , m_aligned(false)
        , m_next(0)
        , m_size(count)
        , m_buffer(nullptr)
        , m_name(name)
    {
        if (nullptr == m_name) {
            m_name = "Unnamed Memory Region";
        }

        // Determine whether or not T is a power of two alignment. If so, we'll
        // align allocations for it on alignof(T), if not... we can't on gcc.
        // Ditto for confirming that alignof(T) is a multiple of sizeof(void*).
        m_aligned = !(alignof(T) % sizeof(void*)) &&
                    !((alignof(T) != 1) && (alignof(T) & (alignof(T) - 1)));

        // Force at least one element worth of storage
        if (count == 0) {
            count = 1;
            m_size = 1;
        }

        // Allocate the whole region in one go
        if (m_aligned) {
            #if !defined(_MSC_VER) && !defined(__MINGW32__)
                auto err_number = posix_memalign((void**)&m_buffer,
                                                 alignof(T),
                                                 m_size * sizeof(T));
                if (err_number == EINVAL) {
                    CRASH(EINVAL, "Error determining memory alignment "
                                  "requirements for region %s.",
                                  m_name);
                }
                if (err_number == ENOMEM) {
                    CRASH(ENOMEM, "Unable to allocate %" PRIu64 "B "
                                  "aligned to %" PRIu64 "B for region %s.",
                                  capacity_bytes(),
                                  alignment(), m_name);
                }
            #else
                m_buffer = (T*) _aligned_malloc((size_t) m_size * sizeof(T),
                                                alignof(T));
                if (m_buffer == nullptr) {
                    // NB -- see #1 from "notes" above
                    CRASH(ENOMEM, "Unable to allocate %" PRIu64 "B "
                                  "aligned to %" PRIu64 "B for region %s.",
                                  capacity_bytes(),
                                  alignment(),
                                  m_name);
                }
            #endif
        } else {
            m_buffer = (T*) malloc((size_t) m_size * sizeof(T));
            if (m_buffer == nullptr) {
                CRASH(ENOMEM, "Unable to allocate %" PRIu64 "B "
                              "for unaligned region %s.",
                              capacity_bytes(),
                              m_name);
            }
        }
    }
    Region(Region const& other)
        : m_buffer(nullptr)
    {
        *this = std::move(Region(other.m_size, other.m_name));
        id = other.id;
        //TODO: This memcpy allows for easy shallow copying, but causes runtime
        //      failures when deep-copying is required. We need to break out the
        //      construction and copying logic of this class into helper
        //      functions that are selected based on whether or not T is
        //      trivially copyable. The FBVector may be good inspiration for
        //      this improvement.
        //      For now we're going to brute-force this and reconstruct every
        //      element in the `this` region via copy-ctor into the new region.
        // memcpy(m_buffer, other.m_buffer, capacity_bytes());
        for (T & element : other) { this->construct(element); }
        /* m_next will be implicitly set by the above. */
        m_name = other.m_name;
    }
    Region& operator=(Region const& other)
    {
        *this = std::move(Region(other.m_size, other.m_name));
        //TODO: See the above note.
        // memcpy(m_buffer, other.m_buffer, capacity_bytes());
        for (T & element : other) { this->construct(element); }
        /* m_next will be implicitly set by the above. */
        return *this;
    }
    // Move construction -- invalidates the source of the move.
    Region(Region&& other)
        : id(other.id)
        , m_aligned(other.m_aligned)
        , m_next(other.m_next)
        , m_size(other.m_size)
        , m_buffer(other.m_buffer)
        , m_name(other.m_name)
    {
        // Mark other's buffer as invalid
        other.m_next = other.m_size = 0;
        other.m_aligned = false;
        other.m_buffer = nullptr;
    }
    // Move by assignment -- tears down previously bound Region and invalidates
    // the source of the move
    Region& operator=(Region&& other)
    {
        if (this == &other) return *this;
        // Tear down current region, if mapped
        this->~Region();

        // Copy over all bookkeeping data
        m_next = other.m_next;
        m_size = other.m_size;
        m_aligned = other.m_aligned;
        m_name = other.m_name;
        m_buffer = other.m_buffer;

        // Mark other's buffer as invalid
        other.m_next = other.m_size = 0;
        other.m_aligned = false;
        other.m_buffer = nullptr;

        return *this;
    }
    // Destructor only deletes buffers for Regions that still own their buffer
    // when falling out of scope.
    ~Region() {
        if (m_buffer != nullptr) {
            drop();
            alignment_correct_free(m_buffer, m_aligned);
            m_buffer = nullptr;
            m_aligned = 0;
            m_next = 0;
            m_size = 0;
        }
    }


    // ### Resize Policy

    // MSVC has an internal compiler error if things outside the class
    // definition call _reserve directly. This, otoh, works.
    void reserve(u64 new_size) {
      _reserve(new_size);
    }

    // #### No Resize
    //
    // Dead-simple -- call CRASH() with ENOMEM.
    // The other option would be to leave this as a noop (when DEBUG_MEMORY
    // isn't defined) and let something else fail down the line---likely an
    // invalid access that would arise from the assumption that the reserve
    // succeeded.
    TEMPLATE_ENABLE(false == IsResizable, void)
    inline void _reserve(u64 new_size) {
        CRASH(ENOMEM, "%" PRIu64 "B %s region %s is non-resizable. Attempted "
                      "resize from " PRIu64 " to " PRIu64 ".",
                      m_size*sizeof(T), (m_aligned ? "aligned" : "unaligned"),
                      m_name, m_size, new_size);
    }

    // #### Resize Allowed
    //
    // Create a new buffer of the requested size, aligned the same as the old
    // one, then copy over all the data from the old buffer. Finally, fix up the
    // region state to reflect the new buffer address and size.
    TEMPLATE_ENABLE(true == IsResizable, void)
    inline void _reserve(u64 new_size) {
        if (m_size == new_size) return;
        if (new_size == 0) {
            LOG("Tried to shrink Region \"%s\" down to 0. Zero-sized Regions "
                "are not supported, so size has been set to 1", m_name);
            new_size = 1;
        }
        if (new_size < used()) CRASH(EINVAL, "Tried to shrink Region \"%s\" "
                                             "to be smaller than the number of "
                                             "elements used (%d < %d).",
                                             m_name, new_size, used());
        auto old_buffer = m_buffer;
        m_buffer = nullptr;
        if (m_aligned) {
            #if !defined(_MSC_VER) && !defined(__MINGW32__)
                auto err_number = posix_memalign((void**)&m_buffer,
                                                 alignof(T),
                                                 new_size * sizeof(T));
                if (err_number == EINVAL) {
                    CRASH(EINVAL, "Error determining memory alignment "
                                  "requirements for region %s.", m_name);
                }
                if (err_number == ENOMEM) {
                    CRASH(ENOMEM, "Unable to allocate %" PRIu64 "B aligned "
                                  "to %" PRIu64 "B for region %s. Out of memory.",
                                  capacity_bytes(),
                                  alignment(),
                                  m_name);
                }
            #else
                m_buffer = (T*) _aligned_malloc((size_t) new_size * sizeof(T),
                                                alignof(T));
                if (m_buffer == nullptr) {
                    // NB -- see #1 from "notes" above
                    CRASH(ENOMEM, "Unable to allocate %" PRIu64 "B aligned to "
                                  "%" PRIu64 "B for region %s. Out of memory.",
                                  capacity_bytes(),
                                  alignment(),
                                  m_name);
                }
            #endif
        } else {
            m_buffer = (T*) malloc((size_t) new_size * sizeof(T));
            if (m_buffer == nullptr) {
                CRASH(ENOMEM, "Unable to allocate %" PRIu64 "B for "
                              "unaligned region %s.",
                              capacity_bytes(),
                              m_name);
            }
        }
        memcpy(m_buffer, old_buffer,
               (size_t) ((new_size>m_size) ? m_size : new_size) * sizeof(T));
        m_size = new_size;
        alignment_correct_free(old_buffer, m_aligned);
    }

    // ### Access & Allocation

    // #### Non-initializing mark-as-used
    // Get a buffer of n consecutive elements in the region. Always includes
    // bounds-check branch, but will silently no-op and return a pointer to a
    // memory region of insufficient length if bounds-checking is disabled.
    inline T* consume(u64 count=1) {
        #if defined(DEBUG_MEMORY)
            if(m_buffer == nullptr) {
                CRASH(ENOMEM, "Attempted to acquire memory from an "
                              "invalid copy of region %s! (Did you "
                              "move this region elsewhere?)",
                              m_name);
            }
        #endif
        if (count + m_next > m_size) {
            _reserve(m_next + count + 1);
        }
        m_next += count;
        return &m_buffer[m_next - count];
    }

    // #### In-place construction
    // We allow all types to construct via placement-new, through any of their
    // defined constructors.

    // In-place construction at `m_next`.
    template<typename ...ctor_arg_types>
    inline T& construct(ctor_arg_types && ... _ctor_args) {
        #if defined(DEBUG_MEMORY)
            if(m_buffer == nullptr) {
                CRASH(ENOMEM, "Attempted to acquire memory from an invalid "
                              "copy of region %s! (Did you move this region "
                              "elsewhere?)", m_name);
            }
        #endif
        if(m_next >= m_size) {
            _reserve(m_size + (u64) ceil(m_size*0.2f));
        }
        return *(::new (m_buffer + m_next++) T(std::forward<ctor_arg_types>(_ctor_args)...));
    }
    // For POD types' default constructors, we do no initialization and simply
    // dereference the next object and return it.
    template<class U=T, class = typename std::enable_if<std::is_pod<U>::value>::type>
    inline T& construct(void) {
        #if defined(DEBUG_MEMORY)
            if(m_buffer == nullptr) {
                CRASH(ENOMEM, "Attempted to acquire memory from an invalid "
                              "copy of region %s! (Did you move this region "
                              "elsewhere?)", m_name);
            }
        #endif
        if(m_next >= m_size) {
            _reserve(m_size + (u64) ceil(m_size*0.2f));
        }
        return *(m_buffer + m_next++);
    }
    // Emplacement -- In-pace construction at a given position
    // If `position == m_next` this function is identical to `construct()`.
    // Otherwise, does not increment m_next, and bounds-checks to verify the
    // emplacement is occurring in already-initialized memory. We do this to
    // avoid creating "holes" in their object array, as contiguity of data is a
    // guarantee of the Region.
    template<typename ...ctor_arg_types>
    inline T& emplace(u64 position, ctor_arg_types && ... _ctor_args) {
        #if defined(DEBUG_MEMORY)
            if(m_buffer == nullptr) {
                CRASH(ENOMEM, "Attempted to acquire memory from an invalid "
                              "copy of region %s! (Did you move this region "
                              "elsewhere?)", m_name);
            }
        #endif
        if(position == m_next) {
            return construct(std::forward<ctor_arg_types>(_ctor_args)...);
        } else if (position > m_next) {
            CRASH(EFAULT, "Unable to acquire (emplace) memory in region %s; "
                          "position %" PRIu64 " is outside  of initialized "
                          "memory (%" PRIu64 ").",
                          m_name, position, (m_next-1));
        }

        return *(::new (m_buffer + position) T(std::forward<ctor_arg_types>(_ctor_args)...));
    }

    // #### Access, Query, and Drop
    // Random-access lookup within the region
    inline T& operator[](u64 index) const {
        #if defined(DEBUG_MEMORY)
        if (index >= m_size) {
            CRASH(EFAULT, "Memory region bounds-check failed; %" PRIu64
                          " is outside the %" PRIu64 " long buffer.",
                          index, m_size);
        }
        #endif
        return m_buffer[index];
    }

    // Iterator access to support range-based for
    inline T* begin(void) const { return m_buffer; }
    inline T* end(void) const { return m_buffer + m_next; }
    inline T* region_end(void) const { return m_buffer + m_size; }

    // General-purpose map over all active elements
    inline void map(std::function<void(T&)> fn) {
        for (T* iter = begin(); iter != end(); ++iter) {
            fn(*iter);
        }
    }

    // Non-destructive copying map over all active elements
    template<typename R=T, bool OutputResizable=IsResizable>
    inline Region<R, OutputResizable> copy_map(
            std::function<R(const T&)> fn
    ) const {
        Region<R, OutputResizable> ret(used(), "Temp/MapResult");
        for (T& iter : *this) {
            ret.construct(fn(iter));
        }
        return ret;
    }

    /* Drop all elements of the region without reinitializing memory.
     * NB: this forwards to _drop; visual studio chokes on enable_if directly */
    inline void drop() { _drop(); }
    TEMPLATE_ENABLE(std::is_pod<T>::value, T)
    inline void _drop() {
        m_next = 0;
    }

    TEMPLATE_ENABLE(!std::is_pod<T>::value, T)
    inline void _drop() {
        for (auto& entry : *this) {
            entry.~T();
        }
        m_next = 0;
    }

    // Default copying-swap quicksort, arbitrary predicate
    inline void sort(std::function<bool(T&, T&)> predicate) {
        return sort(predicate, [](T& a, T& b) {
            T swp = a;
            a = b;
            b = swp;
        });
    }
    // Arbitrary-predicate quicksort with a configurable swap operation
    inline void sort(std::function<bool(T&, T&)> predicate,
                     std::function<void(T& a, T& b)> swap) {
        // In-place quicksort partition algorithm from wikipedia
        auto partition = [&predicate, &swap](T* buffer,
                                             u64 left,
                                             u64 right,
                                             u64 pivot) {
            // Stow the pivot out of the way
            T pivotObj = buffer[pivot];
            swap(buffer[pivot], buffer[right]);

            u64 idx = left;
            for(u64 i = left; i<right; ++i) {
                // If the value under inspection passes the predicate
                // swap it left and move the left index forward.
                if (predicate(buffer[i], pivotObj)) {
                    if (i != idx)
                        swap(buffer[i], buffer[idx]);
                    ++idx;
                }
            }
            // Move the pivot back
            swap(buffer[idx], buffer[right]);

            return idx;
        };

        std::function<void(T*, u64, u64)> qs;
        qs = [&qs, &predicate, &partition](T* buffer, u64 left, u64 right) {
            if (left < right) {
                u64 pivot = left + (right-left)/2;
                pivot = partition(buffer, left, right, pivot);
                qs(buffer, left, (pivot) ? pivot - 1 : 0);
                qs(buffer, pivot + 1, right);
            }
        };
        qs(m_buffer, 0, (m_next) ? m_next - 1 : 0);
    }

    // ### Memory Statistics

    // This region's name
    inline c_cstr name(void) const { return m_name; }

    // The number of object slots currently occupied
    inline u64 used(void) const { return m_next; }

    // The number of object slots available
    inline u64 capacity(void) const { return m_size; }

    // The byte-alignment of elements in this structure, or 0 if unaligned
    inline u64 alignment(void) const { return m_aligned ? alignof(T) : 0; }

    // Bytes used by objects in the pool
    inline u64 used_bytes(void) const { return m_next * sizeof(T); }

    // Total bytes allocated for the pool
    inline u64 capacity_bytes (void) const { return m_size * sizeof(T); }

    // Log this pool's vitals at the provided severity
    inline void logStats() const {
        LOG("Memory region stats:\n"
            "\tRegion ID:  %s\n"
            "\tAlignment:  %s on %" PRIu64 "B\n"
            "\t  Objects:  %" PRIu64 " / %" PRIu64 " at %" PRIu64 "B\n"
            "\t     Byte:  %" PRIu64 " / %" PRIu64 "",
                name(),
                (m_aligned ? "aligned" : "unaligned"), alignof(T),
                used(), capacity(), sizeof(T),
                used_bytes(), capacity_bytes()
            );
    }
};
