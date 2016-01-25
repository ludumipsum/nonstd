/* Game Component Types
   ====================

    All game component types are defined here.

    Note that all Components must be plain-old-data types -- if you need to
    refer to some other piece of data, do so with an ID.

    Any time you make a change to this file, you *must* recompile the platform
    and relaunch -- live edits to the data format are not supported.

   Creating Components
   ===================

   To define a new component type, you'll need to do 4 things:

    1. Define the type in this file using the COMPONENT() macro.
    2. Implement all the game loop hooks your component type cares about in a
       new file -- game/components/YourComponent.cc
    3. Store your component's state somewhere on the GameState struct (look at
       state.audio.audio_sources for an example of this)
    4. Add calls to all the game loop hooks at the appropriate places:
       * `tick()` and `postTick()` in the fixed update portion of the game step
       * `blendView()` in the varupdate/render portion of the game step
       * `beginPlay()` in the game global beginPlay() function

    Note that the COMPONENT() macro takes care of a number of boilerplate tasks
    for you, but if you want to you can instead do these things manually:
     * Alias YourComponentState to ComponentState<YourComponent>
     * Declare overloads for beginPlay, tick, postTick, and blendView
*/

#pragma once

#include "component_state.h"

/* Defines a source of audio associated with a game entity */
COMPONENT(AudioSource) {
    ID id;
    ID wav_id;
    u64 start_time, run_time, cursor;
    bool loop;
};
