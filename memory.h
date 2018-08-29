/** Nonstd Memory System
 *  --------------------
 *  This is the foundation of Nitrogen's shared / persistent memory system.
 *  It includes the nonstd::buffer type, the nonstd::nr_ptr helper class,
 *  and the all important core memory functions that allow for ex;
 *  `nonstd::memory::allocate("Buffer Name", sizeof(target))`.
 */
#pragma once

#include "memory/core_functions.h"
#include "memory/buffer.h"
