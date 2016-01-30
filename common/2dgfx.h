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

enum VGCommandType {
    /* Default no-command-defined type specifier */
    VG_COMMAND_TYPE_NONE = 0,

    /* Path Commands
     * ============= */
    /* Marker for start/end of composite path */
    VG_COMMAND_TYPE_PATH_START,
    VG_COMMAND_TYPE_PATH_END,

    /* Components of a composite path */
    VG_COMMAND_TYPE_PATH_LINE,                  // nvgLineTo(x, y);
    VG_COMMAND_TYPE_PATH_CUBIC_SPLINE,          // nvgBezierTo(c1x, c1y, c2x, c2y, x, y);
    VG_COMMAND_TYPE_PATH_QUAD_SPLINE,           // nvgQuadTo(cx, cy, x, y);
    // Adds an arc segment at the corner defined by the last path point, and two specified points.
    //TODO: .... What!?
    VG_COMMAND_TYPE_PATH_ARC_TO,                // nvgArcTo(x1, y1, x2, y2, radius)

    /* Fully defined shapes (added to current path) */
    VG_COMMAND_TYPE_PATH_ARC,                   // nvgArc(cx, cy, r, a0, a1, int dir);
    VG_COMMAND_TYPE_PATH_RECT,                  // nvgRect(x, y, w, h);
    VG_COMMAND_TYPE_PATH_ROUND_RECT,            // nvgRoundedRect(x, y, w, h, r);
    VG_COMMAND_TYPE_PATH_ELLIPSE,               // nvgEllipse(cx, cy, rx, ry);
    VG_COMMAND_TYPE_PATH_CIRCLE,                // nvgCircle(cx, cy, r);

    /* Modifiers that adjust how the next composite is drawn */
    VG_COMMAND_TYPE_PATH_SET_WINDING,

    /* Commands that adjust how the entire composite is drawn */
    VG_COMMAND_TYPE_PATH_APPLY_FILL,
    VG_COMMAND_TYPE_PATH_APPLY_STROLE,

    /* Render Style Commands
     * ===================== */
    VG_COMMAND_TYPE_RENDER_SET_STROKE_COLOR,    // nvgStrokeColor(NVGcolor color);
    //TODO: Figure out how to implement paints.
    // VG_COMMAND_TYPE_RENDER_SET_STROKE_PAINT,    // nvgStrokePaint(NVGpaint paint);
    VG_COMMAND_TYPE_RENDER_SET_FILL_COLOR,      // nvgFillColor(NVGcolor color);
    //TODO: Figure out how to implement paints.
    // VG_COMMAND_TYPE_RENDER_SET_FILL_PAINT,      // nvgFillPaint(NVGpaint paint);
    VG_COMMAND_TYPE_RENDER_SET_MITER_LIMIT,     // nvgMiterLimit(float limit);
    VG_COMMAND_TYPE_RENDER_SET_STROKE_WIDTH,    // nvgStrokeWidth(float size);
    VG_COMMAND_TYPE_RENDER_SET_LINE_CAP_STYLE,  // nvgLineCap(int cap);
    VG_COMMAND_TYPE_RENDER_SET_LINE_JOIN_STYLE, // nvgLineJoin(int join);
    VG_COMMAND_TYPE_RENDER_SET_GLOBAL_ALPHA,    // nvgGlobalAlpha(float alpha);
};

enum VGPathWinding {
    VG_PATH_WINDING_TYPE_SOLID = 1,
    VG_PATH_WINDING_TYPE_HOLE = 2,
};

enum VGPathDirection {
    VG_PATH_DIRECTION_TYPE_CW = 1,
    VG_PATH_DIRECTION_TYPE_CCW = 2,
};

enum VGLineCapStyle {
    VG_RENDER_LINE_CAP_TYPE_BUTT,
    VG_RENDER_LINE_CAP_TYPE_ROUND,
    VG_RENDER_LINE_CAP_TYPE_SQUARE,
};

enum VGLineJoinStyle {
    VG_RENDER_LINE_JOIN_TYPE_MITER,
    VG_RENDER_LINE_JOIN_TYPE_ROUND,
    VG_RENDER_LINE_JOIN_TYPE_BEVEL,
};


struct VGCommand {
    ID id;
    VGCommandType type;
    union {
        struct { /* VG_COMMAND_TYPE_PATH_<TYPE> */
            // VGSubPathType type; // Do we want one of these?
            union {
                struct { f32 x, y; };
                struct { f32 center_x, center_y; };
            };
            union {
                struct {
                    f32 ctrl_pt_x, ctrl_pt_y;
                };
                struct {
                    f32 ctrl_pt_1_x, ctrl_pt_1_y,
                        ctrl_pt_2_x, ctrl_pt_2_y;
                };
                struct {
                    f32 angle_start, angle_end;
                };
                struct {
                    f32 width, height;
                };
            };
            VGPathDirection direction;
            f32 radius;
        } path;
        struct { /* VG_COMMAND_TYPE_PATH_SET_WINDING */
            VGPathWinding winding;
        }
        struct { /* VG_COMMAND_TYPE_RENDER_SET_* */
            union {
                N2Color         color; // stroke or fill color
                f32             miter_limit;
                f32             stroke_width;
                VGLineCapStyle  line_cap;
                VGLineJoinStyle line_join;
                f32             alpha;
            };
        };
        // Instead of the above, should we split those commands out father?
        // struct /* SetFill */ {
        // };
        // struct /* SetStroke */ {
        // };
    };
}; ENFORCE_POD(VGCommand);
