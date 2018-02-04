#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/nonstdexcept.h>
#include <nonstd/optional.h>

#include "buffer.h"


namespace nonstd::memory {

/* Create a new memory buffer */
using  allocate_fn = nonstd::Buffer *const (*)(c_cstr name, u64 size);
inline allocate_fn allocate =
    [](c_cstr name, u64 size) -> nonstd::Buffer *const {
        N2BREAK(nonstd::error::module_not_started,
                "Attempting to use nonstd::memory::allocate without a memory "
                "backend. Please be sure to set up nonstd::memory function "
                "pointers before doing... well... anything else.");
                return nullptr;
    };

/* Resize a memory buffer (using `realloc` memory-movement semantics) */
using  resize_fn = u64 (*)(nonstd::Buffer *const bd, u64 new_size);
inline resize_fn resize =
    [](nonstd::Buffer *const bd, u64 new_size) -> u64 {
        N2BREAK(nonstd::error::module_not_started,
                "Attempting to use nonstd::memory::resize without a memory "
                "backend. Please be sure to set up nonstd::memory function "
                "pointers before doing... well... anything else.");
                return 0;
    };

/* Release a memory buffer (`free`ing the memory stored therein) */
using  release_fn = void (*)(nonstd::Buffer *const bd);
inline release_fn release =
    [](nonstd::Buffer *const bd) -> void {
        N2BREAK(nonstd::error::module_not_started,
                "Attempting to use nonstd::memory::release without a memory "
                "backend. Please be sure to set up nonstd::memory function "
                "pointers before doing... well... anything else.");
                return;
    };

/* Look up a memory buffer by name */
using  find_fn = nonstd::optional<nonstd::Buffer *const> (*)(c_cstr name);
inline find_fn find =
    [](c_cstr name) -> nonstd::optional<nonstd::Buffer *const> {
        N2BREAK(nonstd::error::module_not_started,
                "Attempting to use nonstd::memory::find without a memory "
                "backend. Please be sure to set up nonstd::memory function "
                "pointers before doing... well... anything else.");
                return { };
    };

} /* namespace nonstd::memory */
