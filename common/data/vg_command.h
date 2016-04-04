/* VECTOR GRAPHICS API
   ===================

   NanoVG primitives, and some more logic built on top of them, are defined as
   structures here. Gamecode builds a list of these every frame to define what
   2d graphics should be rendered.

   You can retain these from one frame to the next, and upate in place if you
   like, or create from scratch every frame if you prefer "immedate mode."

   Gamecode has its own builder interface to this structure which can be found
   in game/util/vg_builder.h
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

/* Vector Graphics commandlist type set */
enum VGCommandType {
    /* Default no-command-defined type specifier. */
    VG_COMMAND_TYPE_NONE = 0,

    /* Meta Commands
     * ------------- */
    /* Push a new state frame on the stack */
    VG_COMMAND_TYPE_META_STATE_FRAME_PUSH,
    /* Pop a state frame off the stack */
    VG_COMMAND_TYPE_META_STATE_FRAME_POP,

    /* Path Commands
     * ------------- */
    /* Move the current offset to <x,y> without drawing anything in between. */
    VG_COMMAND_TYPE_PATH_MOVE,

    /* Subpath Drawing Commands
     * ------------------------ */
    /* Draw a line subpath from the current offset to the start of this path.,
     * ensuring a closed shape. */
    VG_COMMAND_TYPE_PATH_CLOSE,
    /* Draw a line subpath from the current offset to `<x,y>`. */
    VG_COMMAND_TYPE_PATH_LINE,
    /* Draw a cubic spline subpath from the current offset, via control points
     * `ctrl_1` and `ctrl_2`, to `<x,y>`. */
    VG_COMMAND_TYPE_PATH_CUBIC_SPLINE,
    /* Draw a quadratic spline subpath from the current offset, via control
     * point `ctrl_1`, to `<x,y>`. */
    VG_COMMAND_TYPE_PATH_QUAD_SPLINE,
    /* Draw an arc subpath from the current offset, via midpoint ctrl_1,
     * to `<x,y>` along the circle with radius `radius`. */
    VG_COMMAND_TYPE_PATH_ARC,

    /* Shape Primitives
     * ---------------- */
    /* Insert a fully-formed subpath along an arc of the circle centered at
     * `<x,y>` with radius `radius`, subtending the angle from `angle_1`
     * to `angle_2`. */
    VG_COMMAND_TYPE_SHAPE_ARC,
    /* Insert a fully-formed rectangular subpath with origin `<x,y>` and
     * extents `width` by `height`. */
    VG_COMMAND_TYPE_SHAPE_RECT,
    /* Insert a fully-formed rectangular subpath with origin `<x,y>`, extents
     * `width` by `height`, and corner rounding radius `radius`. */
    VG_COMMAND_TYPE_SHAPE_ROUND_RECT,
    /* Insert a fully-formed elliptical subpath with center `<x,y>` and axial
     * radii `ctrl_1_x` and `ctrl_1_y` */
    VG_COMMAND_TYPE_SHAPE_ELLIPSE,
    /* Insert a fully-formed circular subpath with center `<x,y>`
     * and radius `radius`. */
    VG_COMMAND_TYPE_SHAPE_CIRCLE,

    /* Render Controls
     * --------------- */
    /* Set the minimum transparency for all subsequent vector graphics. */
    VG_COMMAND_TYPE_RENDER_GLOBAL_ALPHA,

    /* Render this path as a solid shape, overlapping any shapes behind it. */
    VG_COMMAND_TYPE_RENDER_SOLID,
    /* Render this path as a hole in a shape behind it. */
    VG_COMMAND_TYPE_RENDER_HOLE,

    /* Render a solid fill for this path. */
    VG_COMMAND_TYPE_RENDER_FILL,
    /* Set the color of the solid fill for this path. Overrides FILL_PAINT. */
    VG_COMMAND_TYPE_RENDER_FILL_COLOR,
    /* Set the paint pattern or texture of the solid fill for this path.
     * Overrides FILL_COLOR. */
    VG_COMMAND_TYPE_RENDER_FILL_PAINT,

    /* Render an outline (stroke) for this path. */
    VG_COMMAND_TYPE_RENDER_STROKE,
    /* Set the width of the stroke for this path. */
    VG_COMMAND_TYPE_RENDER_STROKE_WIDTH,
    /* Set the color of the stroke for this path. */
    VG_COMMAND_TYPE_RENDER_STROKE_COLOR,
    /* Set the paint pattern or texture of the stroke for this path. */
    VG_COMMAND_TYPE_RENDER_STROKE_PAINT,

    /* Set the threshold at which sharp corners are beveled. */
    VG_COMMAND_TYPE_RENDER_MITER_LIMIT,

    /* Terminate unjoined lines with a butt cap. */
    VG_COMMAND_TYPE_RENDER_LINE_CAP_BUTT,
    /* Terminate unjoined lines with a round cap. */
    VG_COMMAND_TYPE_RENDER_LINE_CAP_ROUND,
    /* Terminate unjoined lines with a square cap. */
    VG_COMMAND_TYPE_RENDER_LINE_CAP_SQUARE,

    /* Join connected lines with a miter (sharp/bevel depending on angle) */
    VG_COMMAND_TYPE_RENDER_LINE_JOIN_MITER,
    /* Join connected lines with a round cap. */
    VG_COMMAND_TYPE_RENDER_LINE_JOIN_ROUND,
    /* Join connected lines with a beveled cap. */
    VG_COMMAND_TYPE_RENDER_LINE_JOIN_BEVEL,
};

struct VGCommand {
    ID id;
    VGCommandType type;
    union {
        struct { /* VG_COMMAND_TYPE_PATH_<TYPE> */
            struct { f32 x, y; };
            f32 radius;
            union {
                struct { f32 ctrl_1_x, ctrl_1_y,
                             ctrl_2_x, ctrl_2_y; };
                struct { f32 angle_1, angle_2; };
                struct { f32 width, height; };
            };
        };
        struct { /* VG_COMMAND_TYPE_RENDER_* */
            union {
                Color color;
                f32   miter_limit;
                f32   stroke_width;
                f32   global_alpha;
            };
        };
    };
}; ENFORCE_POD(VGCommand);
