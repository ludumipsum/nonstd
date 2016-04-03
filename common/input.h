/* Input Datatypes
   =============

   Defines types that represent inputs to the game.
*/

#pragma once

/* Tag for InputEvent types */
enum InputType {
    N2_INPUT_KEYBOARD    = 1,
    N2_INPUT_MOUSE       = 2,
    N2_INPUT_TOUCH       = 3,
    N2_INPUT_CONTROLLER  = 4,
    N2_INPUT_NULL        = 0,
};

enum InputFlag {
    N2_INPUT_PRESSED     = 1,
};

/* Platform-abstracted input datatype */
struct InputEvent {
    InputType type;
    union {
        i64 i;
        f64 f;
    } data1;
    union {
        i64 i;
        f64 f;
    } data2;
    ID id;
};

/* Platform-abstracted mouse input type */
struct MouseState {
    i32 x, y;
    union {
        u8 buttons;
        struct {
            u8 button_1 : 1,
               button_2 : 1,
               button_3 : 1,
               button_4 : 1,
               button_5 : 1;
        };
    };
};
