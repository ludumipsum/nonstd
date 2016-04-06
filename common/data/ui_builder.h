#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

#include "ui_command.h"

using UICommandList = Pool<UICommand, true>;
class UI {
    UICommand current;
    UICommandList& uicl;
    inline void commit() {
        if (current.type != UI_TYPE_NONE) {
            uicl.create(current);
        }
        current = {0};
        current.type = UI_TYPE_NONE;
        current.icon = UI_ICON_NONE;
        current.state = UI_STATE_DEFAULT;
    }
public:
    inline UI(UICommandList& uicl) : current({0}), uicl(uicl) { }
    inline ~UI() { commit(); }

    inline UI& label() { commit(); current.type = UI_TYPE_LABEL; return *this;}
    inline UI& toolButton() { commit(); current.type = UI_TYPE_TOOL_BUTTON; return *this;}
    inline UI& radioButton() { commit(); current.type = UI_TYPE_RADIO_BUTTON; return *this;}
    inline UI& textField() { commit(); current.type = UI_TYPE_TEXT_FIELD; return *this;}
    inline UI& optionButton() { commit(); current.type = UI_TYPE_OPTION_BUTTON; return *this;}
    inline UI& choiceButton() { commit(); current.type = UI_TYPE_CHOICE_BUTTON; return *this;}
    inline UI& colorButton() { commit(); current.type = UI_TYPE_COLOR_BUTTON; return *this;}
    inline UI& numberField() { commit(); current.type = UI_TYPE_NUMBER_FIELD; return *this;}
    inline UI& slider() { commit(); current.type = UI_TYPE_SLIDER; return *this;}
    inline UI& scrollBar() { commit(); current.type = UI_TYPE_SCROLLBAR; return *this;}
    inline UI& menuBackground() { commit(); current.type = UI_TYPE_MENU_BACKGROUND; return *this;}
    inline UI& menuLabel() { commit(); current.type = UI_TYPE_MENU_LABEL; return *this;}
    inline UI& menuItem() { commit(); current.type = UI_TYPE_MENU_ITEM; return *this;}
    inline UI& tooltipBackground() { commit(); current.type = UI_TYPE_TOOLTIP_BACKGROUND; return *this;}
    inline UI& nodePort() { commit(); current.type = UI_TYPE_NODE_PORT; return *this;}
    inline UI& nodeWire() { commit(); current.type = UI_TYPE_NODE_WIRE; return *this;}
    inline UI& coloredNodeWire() { commit(); current.type = UI_TYPE_COLORED_NODE_WIRE; return *this;}
    inline UI& nodeBackground() { commit(); current.type = UI_TYPE_NODE_BACKGROUND; return *this;}
    inline UI& splitterWidget() { commit(); current.type = UI_TYPE_SPLITTER_WIDGET; return *this;}
    inline UI& joinAreaOverlay() { commit(); current.type = UI_TYPE_JOIN_AREA_OVERLAY; return *this;}

    /* Get a reference to the UICommand we're currently constructing. */
    inline UICommand& get() { return current; }

    /* Set the screen position of this control */
    inline UI& offset(f32 x, f32 y) { current.x = x; current.y = y; return *this; }
    /* Set the screen position of this control */
    inline UI& position(f32 x, f32 y) { return offset(x, y); }
    /* Set the screen position of this control */
    inline UI& at(f32 x, f32 y) { return offset(x, y); }
    /* Set the screen position X of this control */
    inline UI& x(f32 x) { current.x = x; return *this; }
    /* Set the screen position Y of this control */
    inline UI& y(f32 y) { current.y = y; return *this; }

    /* Set the width and height of this control */
    inline UI& dimensions(f32 w, f32 h) { current.w = w; current.h = h; return *this; }
    /* Set the width and height of this control */
    inline UI& extents(f32 w, f32 h) { return dimensions(w, h); }
    /* Set the width of this control */
    inline UI& width(f32 w = UI_TOOL_WIDTH) { current.w = w; return *this; }
    /* Set the height of this control */
    inline UI& height(f32 h = UI_WIDGET_HEIGHT) { current.h = h; return *this; }

    /* Set the ending position for this wire (valid for wires only) */
    inline UI& position_end(f32 x_end, f32 y_end) {
        current.x_end = x_end;
        current.y_end = y_end;
        return *this;
    }
    /* Set the ending position for this wire (valid for wires only) */
    inline UI& offset_end(f32 x_end, f32 y_end) { return position_end(x_end, y_end); }
    /* Set the X-axis end point of this wire (valid for wires only) */
    inline UI& x_end(f32 x_end) { current.x_end = x_end; return *this; }
    /* Set the Y-axis end point of this wire (valid for wires only) */
    inline UI& y_end(f32 y_end) { current.y_end = y_end; return *this; }

    /* Set the text associated with this control */
    inline UI& text(char const* text) { current.text_0 = text; return *this; }
    /* Set the secondary text associated with this control. This is used for
         controls which have both a title and a caption, or a label and a value.
         For example, the label & value of a slider or numberField. */
    inline UI& secondary_text(char const* stext) {
        current.text_1 = stext;
        return *this;
    }

    /* Set the color of this control */
    inline UI& color(f32 color[4]) {
        current.color_0[0] = color[0];
        current.color_0[1] = color[1];
        current.color_0[2] = color[2];
        current.color_0[3] = color[3];
        return *this;
    }
    /* Set the secondary color of this control. This is used for controls which
       have two tones, or blend from one color to another. For example, the
       coloredNodeWire. */
    inline UI& secondary_color(f32 scolor[4]) {
        current.color_1[0] = scolor[0];
        current.color_1[1] = scolor[1];
        current.color_1[2] = scolor[2];
        current.color_1[3] = scolor[3];
        return *this;
        return *this;
    }

    /* Set the state of this control. Valid states are:
         * UI_STATE_DEFAULT
         * UI_STATE_HOVER
         * UI_STATE_ACTIVE
       The state controls the colors used to render the control, and is used to
       indicate interaction with it. */
    inline UI& state(UIState state) { current.state = state; return *this; }

    /* Set the icon to draw next to this control. TODO: document which is which */
    inline UI& icon(UIIcon icon) { current.icon = icon; return *this; }

    /* Set the flags for this control. The meaning of these vary by control. See
         the blendish header (blendish.h) for what exactly they do. */
    inline UI& flags(i32 flags) { current.flags = flags; return *this; }

    /* Highlights a substring of a text control, from character #begin to
         character #end. Only valid on textFields. */
    inline UI& highlight_substring(i32 begin, i32 end) {
        current.cbegin = begin;
        current.cend = end;
        return *this;
    }
    /* Set the start point of the highlighted substring in a textField */
    inline UI& highlight_begin(i32 carat_begin) {
        current.cbegin = carat_begin;
        return *this;
    }
    /* Set the end point of the highlighted substring in a textField */
    inline UI& highlight_end(i32 carat_end) {
        current.cend = carat_end;
        return *this;
    }

    /* Set the progress through the slider -- this is a value from 0.f to 1.f that
       determines the percentage of a slider control which is "done". Meant to be
       used to build progress bars out of the slider control, and to indicate how
       far through a number space your slider's current value is. */
    inline UI& progress(f32 progress) { current.progress = progress; return *this; }

    /* Set the offset into the scroll space at which the current viewport begins.
       This is sort-of the "X" of the scrollbar, and is expressed on [0.f, 1.f].
       It is also only valid on scrollbars. */
    inline UI& offset(f32 offset) { current.offset = offset; return *this; }
    /* Set the size of the scrollbar. This is used to indicate how big the
       underlying scrollable space is relative to the viewport. Kind of a "W" for
       scrollbars. Unsurprisingly only valid on scrollbars. */
    inline UI& size(f32 size) { current.size = size; return *this; }

    /* TODO: Rewrite this to a single joinArrow or similar */
    /* Sets the axis on which the join area overlay arrow should be drawn. */
    inline UI& joinArrowVertical(bool is_vertical) {
        current.vertical = is_vertical;
        return *this;
    }
    /* Sets the direction in which the join area overlay points on its axis
       (faces toward positive on false, negative on true). */
    inline UI& joinArrowMirror(bool is_mirrored) {
        current.mirror = is_mirrored;
        return *this;
    }

    /******************************
     * AUTOMATIC LAYOUT FUNCTIONS *
     ******************************/

    /* Automatically position this control based screen parameters. Assumes you
       have already set dimensions for the control, and will use those
       values in computing the final position.

       Final positions are calculated by applying an offset from the anchor
       point to the origin point of the control.

       * Offsets are taken from the control's X and Y position.

       * Anchor points are specified in terms of viewport percentages (floats in
         the range [0,1]). An anchor of <1,0> would place a control relative to
         the top right corner of the screen.

       * Origin points are specified in terms of control percentages (floats in
         the range [0,1]). An origin of <1,0> would place the top right corner
         of the control at (anchor + offset).
    */
    inline UI& anchorPosition(f32 viewport_width, f32 viewport_height,
                              f32 anchor_x,       f32 anchor_y,
                              f32 origin_x,       f32 origin_y) {
        current.x = viewport_width * anchor_x
                  + current.x - current.w * origin_x;
        current.y = viewport_height * anchor_y
                  + current.y - current.h * origin_y;
        return *this;
    }

    /***************************
     * MOUSE PICKING AND STATE *
     ***************************/

    /* Set the appropriate UI control state based on the mouse position and left
       click state */
    inline UI& pickWithMouse(f32 mouse_x, f32 mouse_y, bool mouse_clicked) {
        commit();  // Commit the last entity, if one is pending
        for (UICommand &c : uicl) {
            bool mouse_over_x = (mouse_x > c.x && mouse_x < c.x + c.w);
            bool mouse_over_y = (mouse_y > c.y && mouse_y < c.y + c.h);
            if (mouse_over_x && mouse_over_y) {
                c.state = (mouse_clicked) ? UI_STATE_ACTIVE : UI_STATE_HOVER;
                break;
            }
        }
        return *this;
    }
};