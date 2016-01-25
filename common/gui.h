/* GUI API
   =======

   Blendish is basically directly translated to structs and exposed here.

   You can retain state and upate it if you like, or create these every
   frame if you prefer immediate mode.
*/

#pragma once

/* ## Blendish-as-data */

enum UIDefaults {
  UI_WIDGET_HEIGHT = 21,
  UI_TOOL_WIDTH = 20,

  UI_NODE_PORT_RADIUS = 5,
  UI_NODE_MARGIN_TOP = 25,
  UI_NODE_MARGIN_DOWN = 5,
  UI_NODE_MARGIN_SIDE = 10,
  UI_NODE_ARROW_AREA_WIDTH = 20,

  UI_SPLITTER_AREA_SIZE = 12,

  UI_SCROLLBAR_WIDTH = 13,
  UI_SCROLLBAR_HEIGHT = 14,

  UI_VSPACING = 1,
  UI_VSPACING_GROUP = 8,
  UI_HSPACING = 8,
};

enum UIWidgetType {
  UI_LABEL,
  UI_TOOL_BUTTON,
  UI_RADIO_BUTTON,
  UI_TEXT_FIELD,
  UI_OPTION_BUTTON,
  UI_CHOICE_BUTTON,
  UI_COLOR_BUTTON,
  UI_NUMBER_FIELD,
  UI_SLIDER,
  UI_SCROLLBAR,
  UI_MENU_BACKGROUND,
  UI_MENU_LABEL,
  UI_MENU_ITEM,
  UI_TOOLTIP_BACKGROUND,
  UI_NODE_PORT,
  UI_NODE_WIRE,
  UI_COLORED_NODE_WIRE,
  UI_NODE_BACKGROUND,
  UI_SPLITTER_WIDGET,
  UI_JOIN_AREA_OVERLAY,
};

enum UIState {
  UI_STATE_DEFAULT = 0,
  UI_STATE_HOVER,
  UI_STATE_ACTIVE,
};

struct UILabel {
  f32 x, y, w, h;
  i32 icon_id;
  char const* label;
};

struct UIToolButton {
  f32 x, y, w, h;
  i32 flags;
  UIState state;
  i32 icon_id;
  char const* label;
};

struct UIRadioButton {
  f32 x, y, w, h;
  i32 flags;
  UIState state;
  i32 icon_id;
  char const* label;
};

struct UITextField {
  f32 x, y, w, h;
  i32 flags;
  UIState state;
  i32 icon_id;
  char const* text;
  i32 cbegin, cend;
};

struct UIOptionButton {
  f32 x, y, w, h;
  UIState state;
  char const* label;
};

struct UIChoiceButton {
  f32 x, y, w, h;
  i32 flags;
  UIState state;
  i32 icon_id;
  char const* label;
};

struct UIColorButton {
  f32 x, y, w, h;
  i32 flags;
  f32 color[4];
};

struct UINumberField {
  f32 x, y, w, h;
  i32 flags;
  UIState state;
  char const* label;
  char const* value;
};

struct UISlider {
  f32 x, y, w, h;
  i32 flags;
  UIState state;
  f32 progress;
  char const* label;
  char const* value;
};

struct UIScrollBar {
  f32 x, y, w, h;
  UIState state;
  f32 offset;
  f32 size;
};

struct UIMenuBackground {
  f32 x, y, w, h;
  i32 flags;
};

struct UIMenuLabel {
  f32 x, y, w, h;
  i32 icon_id;
  char const* label;
};

struct UIMenuItem {
  f32 x, y, w, h;
  UIState state;
  i32 icon_id;
  char const* label;
};

struct UITooltipBackground {
  f32 x, y, w, h;
};

struct UINodePort {
  f32 x, y;
  UIState state;
  f32 color[4];
};

struct UINodeWire {
  f32 x0, y0, x1, y1;
  UIState state0, state1;
};

struct UIColoredNodeWire {
  f32 x0, y0, x1, y1;
  f32 color0[4];
  f32 color1[4];
};

struct UINodeBackground {
  f32 x, y, w, h;
  UIState state;
  i32 icon_id;
  char const* label;
  f32 title_color[4];
};

struct UISplitterWidget {
  f32 x, y, w, h;
};

struct UIJoinAreaOverlay {
  f32 x, y, w, h;
  i32 vertical, mirror;
};

struct UIControl {
  ID id;
  UIWidgetType type;
  union {
    UILabel label;
    UIToolButton tool_button;
    UIRadioButton radio_button;
    UITextField text_field;
    UIOptionButton option_button;
    UIChoiceButton choice_button;
    UIColorButton color_button;
    UINumberField number_field;
    UISlider slider;
    UIScrollBar scrollbar;
    UIMenuBackground menu_background;
    UIMenuLabel menu_label;
    UIMenuItem menu_item;
    UITooltipBackground tooltip_background;
    UINodePort node_port;
    UINodeWire node_wire;
    UIColoredNodeWire colored_node_wire;
    UINodeBackground node_background;
    UISplitterWidget splitter_widget;
    UIJoinAreaOverlay join_area_overlay;
  };
}; ENFORCE_POD(UIControl);
