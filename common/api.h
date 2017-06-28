/** Platform API
 *  ============
 *  This file defines the API for all platform-layer code. Each platform's
 *  implementation can be found in modules prefixed with the corresponding
 *  platform name (e.g. win32_all.cc or win32_audio.cc).
 *
 *  Because it is included everywhere, it also transitively includes common
 *  utility macros we need in all sources.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

// NB. We don't `using mem::Buffer` after this include, because api.h is
// included _everywhere_, and we don't want to prematurely pollute the global
// namespace with that alias.
#include "mem.h"
#include "crash.h"

#include "cvar.h"
#include "data/ui_command.h"
#include "data/vg_command.h"
#include "data/input_event.h"


/** Game State
 *  ==========
 *  All state used by the game is stored in this structure: memory pools,
 *  platform resources, timing information, and so on.
 */

struct GameState {
    /* Memory backing all game buffers */
    struct MemoryAPI {
        mem::Buffer *const           (*allocate)     (c_cstr name, u64 size,
                                                      mem::Flags flags);
        u64                          (*resize)       (mem::Buffer *const bd,
                                                      u64 new_size);
        void                         (*release)      (mem::Buffer *const bd);
        Optional<mem::Buffer *const> (*find)         (c_cstr name);
        Optional<mem::Buffer *const> (*findHistoric) (c_cstr name, u64 frame);
    } memory;

    struct crashAPI {
      /* Signal the platform that the game has encountered an unrecoverable
         error. The platform may or may not itself die in response. */
      int (*crash)(N2Error error, c_cstr reason,
                  c_cstr file, u64 line, c_cstr funcsig);
    } crash;

    struct chronoAPI {
        /* Get the current time */
        u64 (*now)();
    } chrono;

    struct CVarAPI {
      /* Configuration variable accessors */
      CVar_i* (*find_cvar_i)(c_cstr name);
      CVar_f* (*find_cvar_f)(c_cstr name);
      CVar_b* (*find_cvar_b)(c_cstr name);
      CVar_s* (*find_cvar_s)(c_cstr name);
    } cvar;

    struct RngAPI {
        u64  (*integer)  (u64 min, u64 max);
        f64  (*fraction) (f64 min, f64 max);
        void (*seed)     (u64 new_seed);
    } rng;

    /* Read-only data populated by the platform */
    struct IncomingData {
        /* Stream of input events since the last frame. */
        c_cstr events_buffer_id;
        /* Number of audio bytes consumed by the platform since the
           last frame. */
        u16 audio_bytes_consumed;
        /* Wall time since the last frame began */
        u64 delta_ns;
        /* High clamp for time elapsed between frames -- constrained to reduce
           physical simulation insanity/instability. */
        u32 max_delta_ns;
        /* Period at which simulation & logic updates occur */
        u32 fixed_delta_ns;
        /* Width of the render target, in pixels */
        u16 window_width;
        /* Height of the render target, in pixels */
        u16 window_height;
    } in;

    struct OutgoingData {
        /* ID of the buffer used to output UI commands */
        c_cstr ui_command_buffer_id;
        /* ID of the buffer used to output vector graphics commands */
        c_cstr vg_command_buffer_id;
    } out;

    /* Variable-timing (rendering) frame number */
    u64 frame;
    /* Sequence number of the current simulation iteration */
    u64 step;
    /* Blend factor for how far between simulation states we are */
    f32 simulation_alpha;
    /* Fraction of a frame period left over after rendering the current frame */
    f32 accumulator;

}; ENFORCE_POD(GameState);


/** Platform Hooks
 *  ==============
 *  Functions exposed to the platform code, called at particular times in the
 *  game loop to generate data the platform layer needs per frame.
 */

extern "C" {

/* Simulation Step
   Given the current game state, step it forward once.
*/
void onStep(GameState const & prev, GameState& state);

/* Library Load/Init Hook
   Give each gamecode library a chance to initialize the state it needs when
   it's first loaded. Implementers of this function can expect at least one
   call to it, possibly more, and so should take action idempotently.
*/
void onLoadLibrary(GameState& state);

} /* extern "C" */
