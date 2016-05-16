#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "api.h"
#include "buffer.h"

/* Buffer Hash Map
   ===============

   BufferMaps provide key/value storage backed by gameplay buffers.
*/
template<typename T>
class BufferMap {
protected:
    class BucketMetadata {
        union {
            struct {
                u64  id;
                bool used      : 1;
                bool tombstone : 1;
            };
            T padding;
        };
        T data[]
    };
    class BufferMapMetadata {
        u32            magic;
        u32            bucket_count;
        u64            bucket_size;
        BucketMetadata buckets[];
    };

    GameState         *m_state;
    BufferDescriptor  &m_bd;
    BufferMapMetadata *m_metadata;

    static u64 magic() { return (u64)BufferMap<T>::magic; };
    static const u64 elem_size = sizeof(BucketMetadata);

    /* Set up the metadata structure at the start of the data segment */
    inline void initialize(u32 bucket_count, u64 bucket_size) {
        m_metadata = (BufferMapMetadata*) m_bd.data;
        /* Quick assertion that we haven't reused a buffer to make a map of a
           different type */
        if (m_metadata->magic) {
            if (m_metadata->magic != magic()) {
                LOG("WARNING: Reinterpreting a BufferMap! Underlying buffer is "
                    "named %s, and begins at %p. Got magic number %p, "
                    "expected %p", m_bd.name, m_bd.data,
                    m_metadata->magic, magic());
                DEBUG_BREAKPOINT();
            } else {
                /* Success case */
            }
        } else {
            m_metadata->magic = magic();
            m_metadata->bucket_count = bucket_count;
            m_metadata->bucket_size = bucket_size;
            // Warn if sizes don't match up
            auto total_size = (1 + bucket_count) * bucket_size * elem_size;
            if (total_size > m_bd.size) {
                if (m_state) {
                    resize(total_size);
                } else {
                    LOG("WARNING: Buffer %s is too small to contain %d map "
                        "buckets with %d elements of %d bytes each. %dB > %dB. "
                        "(including one slot overhead per bucket for metadata)",
                        m_bd.name, bucket_count, bucket_size, elem_size,
                        total_size, m_bd.size);
                    BREAKPOINT();
                }
            }
        }
    }

    /* Look up a bucket by hashing the key and searching for the first unused
       bucket after that point in the bucket list */
    T* oa_lookup(u64 requested_id) {
        auto bucket_id = requested_id % m_metadata->bucket_count;
        auto index = bucket_id * m_metadata->bucket_size;
        auto bucket = (BucketMetadata*) &(m_metadata->buckets[index]);
        if (bucket->used && !bucket->tombstone && bucket->id == requested_id) {
            return &bucket->data;
        }
        return nullptr;
    }

    void oa_insert(u64 id) {
        auto bucket_id = requested_id % m_metadata->bucket_count;
        auto index = bucket_id * m_metadata->bucket_size;
        auto bucket = (BucketMetadata*) &(m_metadata->buckets[index]);

        u32 search = bucket_id;
        while (bucket->used && search < m_metadata->bucket_count) {
            bucket += m_metadata->bucket_size;
            search++;
        }
        if (search >= m_metadata->bucket_count) DEBUG_BREAKPOINT;
    }

public:
    BufferMap(BufferDescriptor* bd, u32 bucket_count, u64 bucket_size)
              : m_state ( nullptr )
              , m_bd    ( *bd     ) {
        initialize(bucket_count, bucket_size);
    }
    BufferMap(BufferDescriptor& bd, u32 bucket_count, u64 bucket_size)
              : m_state    ( nullptr )
              , m_bd       ( bd      ) {
        initialize(bucket_count, bucket_size);
    }
    BufferMap(GameState& state, c_cstr name, u32 bucket_count, u64 bucket_size)
              : m_state    ( &state  )
              , m_bd       ( *state.memory.lookup(name) ) {
        initialize(bucket_count, bucket_size);
    }

    inline void resize(u64 size_bytes) {
        if (m_state) {
            m_state->memory.resize(m_bd, size_bytes);
        } else {
            BREAKPOINT();
        }
    }

    BufferView<T> operator[](c_cstr key) {
        auto bd = make_buffer(oa_lookup(HASH(key)),
                              m_metadata->bucket_size * elem_size);
        return BufferView<T>(bd);
    }

};
