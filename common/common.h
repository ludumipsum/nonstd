/* COMMON CODE API MASTER
   ======================

   Inlcude this file to get all common/ functionality in your module.

   All files located under the util/ directory should be include here to allow
   for this file's universal, single-header-include pattern.

   This file should only be included by game and platform code, not other common
   code, since doing that would create a cycle in the include graph.

   NB. The order in which these files are included should *never* be
       significant. Logical groupings only.

       All common/ code explicitly includes the other common/ code on which it
       depends, so as long as you don't either fail to do that or introduce a
       cycle, everything will work. The actual include order in the preprocessed
       file may differ from the logical order presented here.
*/

#pragma once

/* Platform <----> Gamecode API */
#include "api.h"

/* Base includes for allthethings */
#include "batteries_included.h"
#include "primitive_types.h"

/* Should-be-standard Library */
#include "cvar.h"
#include "debug.h"
#include "hash.h"
#include "itertools.h"
#include "logging.h"

/* Data Interchange Definitions */
#include "data/input_event.h"
#include "data/sdl_keymap.h"
#include "data/ui_builder.h"
#include "data/ui_command.h"
#include "data/vg_builder.h"
#include "data/vg_command.h"

/* Collections & Views */
// #include "buffer_view.h"

/* Please to be fucking off */
#include "deprecated/region.h"
#include "deprecated/pool.h"
#include "deprecated/ring.h"
