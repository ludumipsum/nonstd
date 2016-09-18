/* Platform API
   ============

   This file defines the API for all platform-layer code. Each platform's
   implementation can be found in modules prefixed with the corresponding
   platform name (e.g. win32_all.cc or win32_audio.cc).

   Because it is included everywhere, it also transitively includes common
   utility macros we need in all sources.
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "cvar.h"
#include "data/ui_command.h"
#include "data/vg_command.h"
#include "data/input_event.h"


/* Forward declaration; defined and used in buffer.h */
struct BufferDescriptor;

// TODO: extern "C" the whole API file

struct Entity {
    ID id;
    f32 x, y, z;
    f32 xr, yr, zr;
}; ENFORCE_POD(Entity);

struct SimulationStat {
    u64 step;
    u64 tick_usec,
        post_tick_usec,
        total_usec;
};
struct StepStat {
    u64 frame;
    u64 platform_total_usec;
    u64 input_poll_usec,
        blend_view_usec,
        draw_gui_usec,
        total_usec;
    f32 blend_alpha;
    u16 sim_frames_run;
};

/* Game State
   ==========

   All state used by the game is stored in this structure: memory pools,
   platform resources, timing information, and so on.
*/

struct GameState {
    /* Memory backing all game buffers */
    struct Memory {
        BufferDescriptor *const (*create)         (c_cstr name, u64 size,
                                                   BufferFlags flags);
        u64                     (*resize)         (BufferDescriptor *const bd,
                                                   u64 new_size);
        void                    (*destroy)        (BufferDescriptor *const bd);
        BufferDescriptor *const (*lookup)         (c_cstr name);
        BufferDescriptor *const (*lookupHistoric) (c_cstr name, u64 frame);
        c_cstr strings_buffer_id;
    } memory;

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

    /* Platform functions exposed directly to gamecode */
    struct PlatformFunctions {
        /* Configuration variable accessors */
        CVar_i* (*find_cvar_i)(c_cstr name);
        CVar_f* (*find_cvar_f)(c_cstr name);
        CVar_b* (*find_cvar_b)(c_cstr name);
        CVar_s* (*find_cvar_s)(c_cstr name);

        /* Get the current time */
        u64 (*now)();

        /* Signal the platform that the game has encountered an unrecoverable
           error. The platform may or may not itself die in response. */
        void (*crash)(i32 error_number, c_cstr reason,
                      c_cstr file, u64 line, c_cstr funcsig);
    } functions;
}; ENFORCE_POD(GameState);


/* Platform Hooks
   ==============

   Functions exposed to the platform code, called at particular times in the
   game loop to generate data the platform layer needs per frame.
*/

extern "C" {

/* Simulation Step
   Given the current game state, step it forward once.
*/
void step(GameState const & prev, GameState& state);

//TODO: notion of worlds and world start vs whole game initialization
/* Start of Play Hook
   Initialize the current play session
*/
void beginPlay(GameState& state);

} /* extern "C" */
