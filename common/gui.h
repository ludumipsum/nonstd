/* GUI API
   =======

   Blendish is basically directly translated to structs and exposed here.

   You can retain state and upate it if you like, or create these every
   frame if you prefer immediate mode.

   Gamecode has its own builder interface to this structure which can be found
   in game/util/gui_builder.h
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
  UI_LABEL = 1,
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

/* TODO: Optimize this by unioning away things that will never occur together */
struct UIControl {
  ID id;
  UIWidgetType type;
  f32 x, y;
  f32 w, h;
  f32 x_end, y_end;
  f32 offset, size;
  f32 progress;
  i32 flags, icon_id, cbegin, cend;
  UIState state, state_end;
  f32 color_0[4], color_1[4];
  bool vertical, mirror;
  char const* text_0,
       const* text_1;
}; ENFORCE_POD(UIControl);
