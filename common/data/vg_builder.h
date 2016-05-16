#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "buffer.h"
#include "vg_command.h"

#include "api.h"

class VG {
protected:
    using VGCommandList = BufferView<VGCommand>;

    VGCommand        m_current;
    VGCommandList    m_vgcl;

    /* Retained state */
    bool  m_fill,
          m_stroke;
    Color m_fill_color,
          m_stroke_color;
    f32   m_stroke_width;

    inline void commit() {
        if (m_current.type != VG_COMMAND_TYPE_NONE) {
            m_vgcl.push(m_current);
        }
        m_current = {0};
        m_current.type = VG_COMMAND_TYPE_NONE;
    }

public:
    inline VG(BufferDescriptor& buffer)
        : m_current      ( { 0 }  )
        , m_vgcl         ( buffer )
        , m_fill         ( false  )
        , m_fill_color   ( { 0 }  )
        , m_stroke       ( false  )
        , m_stroke_color ( { 0 }  )
        , m_stroke_width ( 0      )
        , path           ( *this  )
        , shape          ( *this  )
    {
        m_current.type = VG_COMMAND_TYPE_META_STATE_FRAME_PUSH;
        commit();
    }
    inline VG(GameState& state)
        : m_current      ( { 0 }                                 )
        , m_vgcl         ( state, state.out.vg_command_buffer_id )
        , m_fill         ( false                                 )
        , m_fill_color   ( { 0 }                                 )
        , m_stroke       ( false                                 )
        , m_stroke_color ( { 0 }                                 )
        , m_stroke_width ( 0                                     )
        , path           ( *this                                 )
        , shape          ( *this                                 )
    {
        m_current.type = VG_COMMAND_TYPE_META_STATE_FRAME_PUSH;
        commit();
    }
    inline ~VG() {
        if (m_fill) {
            commit();
            m_current.type = VG_COMMAND_TYPE_RENDER_FILL_COLOR;
            m_current.color = m_fill_color;
            commit();
            m_current.type = VG_COMMAND_TYPE_RENDER_FILL;
        }
        if (m_stroke) {
            commit();
            m_current.type = VG_COMMAND_TYPE_RENDER_STROKE_COLOR;
            m_current.color = m_stroke_color;
            commit();
            m_current.type = VG_COMMAND_TYPE_RENDER_STROKE_WIDTH;
            m_current.stroke_width = m_stroke_width;
            commit();
            m_current.type = VG_COMMAND_TYPE_RENDER_STROKE;
        }
        commit();
        m_current.type = VG_COMMAND_TYPE_META_STATE_FRAME_POP;
        commit();
    }

    /* Get a reference to the VGCommand currently being constructed */
    inline VGCommand& get() { return m_current; }

    /*********************
     * PATH CONSTRUCTION *
     *********************/

    /* Tools for constructing composite paths from primitive commands. */
    class VGPath {
    private:
        VG& vg;
    public:
        VGPath(VG& vg) : vg(vg) { }
        /* Move the vector graphics pen */
        inline VG& move() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_PATH_MOVE;
            return vg;
        }
        /* Draw a line from the pen position to the provided <x,y> coordinates */
        inline VG& line() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_PATH_LINE;
            return vg;
        }
        /* Draw a cubic spline from the pen position to the target <x,y> coordinates,
         * using control_1 and control_2 to determine the shape of the spline */
        inline VG& spline() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_PATH_CUBIC_SPLINE;
            return vg;
        }
        /* Draw a quadratic spline from the pen position to the target <x,y>
         * coordinates, via a control point */
        inline VG& quad_spline() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_PATH_QUAD_SPLINE;
            return vg;
        }
        /* Draw an arc with a given radius from the pen position, through a control
         * point, to the target <x,y> coordinates. */
        inline VG& arc() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_PATH_ARC;
            return vg;
        }
        /* Close the current subpath by drawing a line from the pen position to the
         * starting point of this subpath. Implicitly calls done(). */
        inline VG& close() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_PATH_CLOSE;
            return vg;
        }
    } path;

    /**********************
     * SHAPE CONSTRUCTION *
     **********************/

    /* Tools for constructing entire primitive shapes as paths */
    class VGShape {
    private:
        VG& vg;
    public:
        VGShape(VG& vg) : vg(vg) { }
        /* Draw an arc centered at the target <x,y> coordinates,
         * with a given radius and subtending the given angle. */
        inline VG& arc() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_SHAPE_ARC;
            return vg;
        }
        /* Draw a rectangle from the given <x,y> coordinates with extents of
         * <width,height>. */
        inline VG& rectangle() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_SHAPE_ARC;
            return vg;
        }
        /* Draw a rounded rectangle from the given <x,y> coordiantes with extents of
         * <width,height> and corners rounded with the given radius. */
        inline VG& rounded_rectangle() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_SHAPE_ROUND_RECT;
            return vg;
        }
        /* Draw an ellipse with center <x,y> and per-axis radii
         * <radius_x,radius_y>. */
        inline VG& ellipse() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_SHAPE_ELLIPSE;
            return vg;
        }
        /* Draw a circle with center <x,y> and the given radius */
        inline VG& circle() {
            vg.commit();
            vg.m_current.type = VG_COMMAND_TYPE_SHAPE_CIRCLE;
            return vg;
        }
    } shape;


    /**********************
     * DRAWING PARAMETERS *
     **********************/

    /* Set the destination point of this subpath */
    inline VG& to(f32 x, f32 y) {
        m_current.x = x, m_current.y = y;
        return *this;
    }
    /* Set the destination point of this subpath */
    inline VG& at(f32 x, f32 y) { return to(x, y); };
    /* Set the destination point of this subpath */
    inline VG& position(f32 x, f32 y) { return to(x, y); };
    /* Set the destination X of this subpath */
    inline VG& x(f32 x) { m_current.x = x; return *this; }
    /* Set the destination Y of this subpath */
    inline VG& y(f32 y) { m_current.y = y; return *this; }

    /* Set the control point used to draw this subpath */
    inline VG& control(f32 x, f32 y) {
        m_current.ctrl_1_x = x;
        m_current.ctrl_1_y = y;
        return *this;
    }
    /* Set the first control point used to draw this subpath */
    inline VG& control_1(f32 x, f32 y) { return control(x, y); };
    /* Set the control point used to draw this subpath */
    inline VG& through(f32 x, f32 y) { return control(x, y); };
    /* Set the midpoint of the arc */
    inline VG& midpoint(f32 x, f32 y) { return control(x, y); };

    /* Set the X value of the control point used to draw this subpath */
    inline VG& control_x(f32 x) { m_current.ctrl_1_x = x; return *this; }
    /* Set the X value of the first control point used to draw this subpath */
    inline VG& control_1_x(f32 x) { return control_x(x); };

    /* Set the Y value of the control point used to draw this subpath */
    inline VG& control_y(f32 y) { m_current.ctrl_1_y = y; return *this; }
    /* Set the y value of the first control point used to draw this subpath */
    inline VG& control_1_y(f32 y) { return control_y(y); };

    /* Set the second control point used to draw this subpath */
    inline VG& control_2(f32 x, f32 y) {
        m_current.ctrl_2_x = x;
        m_current.ctrl_2_y = y;
        return *this;
    }
    /* Set the X value of the second control point used to draw this subpath */
    inline VG& control_2_x(f32 x) { m_current.ctrl_2_x = x; return *this; }
    /* Set the Y value of the second control point used to draw this subpath */
    inline VG& control_2_y(f32 y) { m_current.ctrl_2_y = y; return *this; }

    /* Set the start and end angles of the arc shape */
    inline VG& angle(f32 theta_0, f32 theta_1) {
        m_current.angle_1 = theta_0;
        m_current.angle_2 = theta_1;
        return *this;
    }
    /* Set the start angle of the arc shape */
    inline VG& angle_start(f32 theta) {
        m_current.angle_1 = theta;
        return *this;
    }
    /* Set the end angle of the arc shape */
    inline VG& angle_end(f32 theta) {
        m_current.angle_1 = theta;
        return *this;
    }

    /* Set the width and height of the shape */
    inline VG& extents(f32 width, f32 height) {
        m_current.width = width;
        m_current.height = height;
        return *this;
    }
    /* Set the width of the shape */
    inline VG& width(f32 width) { m_current.width = width; return *this; }
    /* Set the height of the shape */
    inline VG& height(f32 height) { m_current.height = height; return *this; }

    /* Set the radius */
    inline VG& radius(f32 radius) { m_current.radius = radius; return *this; }
    /* Set the radii of the ellipse */
    inline VG& radius(f32 x, f32 y) { return control(x, y); }
    /* Set the X radius of the ellipse */
    inline VG& radius_x(f32 x) { return control_x(x); };
    /* Set the Y radius of the ellipse */
    inline VG& radius_y(f32 y) { return control_y(y); };

    /*************************
     * RENDER STATE CONTROLS *
     *************************/

    /* Set the global alpha for all paths */
    inline VG& global_alpha(f32 alpha) {
        commit();
        m_current.type = VG_COMMAND_TYPE_RENDER_GLOBAL_ALPHA;
        m_current.global_alpha = alpha;
        return *this;
    }

    /* Switch the render mode to drawing holes for subsequent paths */
    inline VG& hole() {
        commit();
        m_current.type = VG_COMMAND_TYPE_RENDER_HOLE;
        return *this;
    }
    /* Switch the render mode to solid for subsequent paths (default) */
    inline VG& solid() {
        commit();
        m_current.type = VG_COMMAND_TYPE_RENDER_SOLID;
        return *this;
    }

    /* Fill the path with this color */
    inline VG& fill(Color color) {
        m_fill = true;
        m_fill_color = color;
        return *this;
    }
    /* Stroke the path with this width and color */
    inline VG& stroke(f32 width, Color color) {
        m_stroke = true;
        m_stroke_width = width;
        m_stroke_color = color;
        return *this;
    }
};
