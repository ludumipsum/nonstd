/* GUI API
   =======

   NanoVG primitives, and some more logic built on top of them, are defined as
   structures here. Gamecode builds a list of these every frame to define what
   2d graphics should be rendered.

   You can retain these from one frame to the next, and upate in place if you
   like, or create from scratch every frame if you prefer "immedate mode."

   Gamecode has its own builder interface to this structure which can be found
   in game/util/2dgfx_builder.h
*/

#pragma once
#include "util.h"

/* Vector Graphics commandlist format */

enum VGPathType {
    /* Components of a composite path */
    VG_PATH_TYPE_START = 1,
    VG_PATH_TYPE_LINE,
    VG_PATH_TYPE_CUBIC_SPLINE,
    VG_PATH_TYPE_QUAD_SPLINE,
    VG_PATH_TYPE_ARC,
    VG_PATH_TYPE_RECT,
    VG_PATH_TYPE_ROUND_RECT,
    VG_PATH_TYPE_ELLIPSE,
    VG_PATH_TYPE_CIRCLE,

    /* Modifiers that adjust how the whole composite is drawn */
    VG_PATH_TYPE_CMD_WIND_SOLID,
    VG_PATH_TYPE_CMD_WIND_HOLE,
    VG_PATH_TYPE_CMD_FILL,
    VG_PATH_TYPE_CMD_STROKE,

    /* Default no-path-defined type specifier */
    VG_PATH_TYPE_NONE = 0;
};

struct VGPath {
    VGPathType type;


};
