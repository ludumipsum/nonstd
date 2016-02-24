/*# Platform API

  This file defines the API for all platform-layer code. Each platform's
  implementation can be found in modules prefixed with the corresponding
  platform name (e.g. win32_all.cc or win32_audio.cc).

  Because it is included everywhere, it also transitively includes common
  utility macros we need in all sources.
*/

#pragma once

#include "util.h"

#include "gui.h"
#include "2dgfx.h"
#include "components.h"

extern "C" {

#include "input.h"

struct Entity {
    ID id;
    f32 x, y, z;
    f32 xr, yr, zr;
}; ENFORCE_POD(Entity);

struct SimulationStat {
  u64 sim_frame;
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

/*## Game State
  All state used by the game is stored in this structure: memory pools, platform
  resources, timing information, and so on. */
/* TODO: Split GameState into three:
            * platform handles (texture IDs etc)
            * debug data
            * actual game state
          Eventually the actual game state portion will need some more
          cleverness around what gets network replicated and what doesn't
          too. */
struct GameState {
  /* Scratch memory, cleared every frame */
  // Region<u8[4096], true> frame_arena;

  /* Game entity pool */
  Pool<Entity, true> entities;

  struct TimingData {
    /* Variable-timing (rendering) frame number */
    u64 frame;
    /* Wall time since the last frame began */
    u64 time_since_last_frame;
    /* Sequence number of the current simulation iteration */
    u64 simulation_frame;
    /* Blend factor for how far between simulation states we are */
    f32 simulation_alpha;
    /* Fraction of a frame period left over after rendering the current frame */
    f32 accumulator;
    /* Period at which simulation & logic updates occur */
    u32 fixed_timestep_ns;
    /* High clamp for time elapsed between frames -- constrained to reduce
       physical simulation insanity/instability. */
    u32 max_timestep_ns;
    inline u64 size() {
      return sizeof(*this);
    }
  } timing;

  struct InputData {
    Pool<InputEvent> events;
    MouseState mouse;
    inline u64 size() {
      return sizeof(*this)
           + events.total_bytes();
    }
  } input;

  /* Contains pointers to master buffer, read and write heads.
     Contains actual audio resources and audio sources pools.
     (buffer and read/write heads are static globals defined in
      platform_audio.cc)
  */
  struct AudioData {
    /* Gamecode audio buffer */
    Region<u8, true> buffer;
    /* The number of bytes SDL has consumed this frame (NB: this is GameState's
     * record of sdl_consume; platform has its own record) */
    u16 sdl_consume;
    /* Loaded audio resources */
    Pool<Region<u8, true>> resources;
    std::unordered_map<std::string, ID> resource_map;
    /* Audio sources */
    AudioSourceState sources;
    // Pool<AudioSource> sources;
    std::unordered_map<std::string, ID> source_map;
    inline u64 size() {
      return sizeof(*this)
           + buffer.capacity_bytes()
           + resources.total_bytes()
           + resource_map.size()
           + sources.total_bytes()
           + source_map.size();
    }
  } audio;

  struct GraphicsData {
    Pool<UIControl, true> gui;
    Pool<VGCommand, true> vector2d;
    /* Width of the viewport */
    u16 width;
    /* Height of the viewport */
    u16 height;
    inline u64 size() {
      return sizeof(*this)
           + gui.total_bytes();
    }
  } graphics;

  struct DebugData {
    Ring<SimulationStat> simulation_stats;
    Ring<StepStat> step_stats;
    inline u64 size() {
      return sizeof(*this)
           + simulation_stats.capacity_bytes()
           + step_stats.capacity_bytes();
    }
  } debug;

  /* Platform functions exposed directly to gamecode */
  struct PlatformFunctions {
    /* Configuration variable accessors */
    CVar_i* (*find_cvar_i)(char const* name);
    CVar_f* (*find_cvar_f)(char const* name);
    CVar_b* (*find_cvar_b)(char const* name);
    CVar_s* (*find_cvar_s)(char const* name);

    /* Get the current time */
    u64 (*now)();

    // TODO: File IO
    // TOOD: Network IO
    inline u64 size() { return sizeof(*this); }
  } functions;

  inline u64 size() {
    return /*frame_arena.capacity_bytes()
         + */entities.total_bytes()
         + timing.size()
         + input.size()
         + audio.size()
         + graphics.size()
         + debug.size()
         + functions.size();
  }
};


/* GameState Member Types */
using TimingData = GameState::TimingData;
using InputData = GameState::InputData;
using AudioData = GameState::AudioData;
using GraphicsData = GameState::GraphicsData;
using DebugData = GameState::DebugData;
using PlatformFunctions = GameState::PlatformFunctions;

/*## Platform Hooks
  Functions exposed to the platform code, called at particular times in the
  game loop to generate data the platform layer needs per frame. */

/*### Simulation Step
  Given the current game state, step it forward once. */
void step(GameState const& prev, GameState& state);

//TODO: notion of worlds and world start vs whole game initialization
/*### Start of play hook
  Initialize the current play session */
void beginPlay(GameState& state);

}
