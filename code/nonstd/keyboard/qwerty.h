#pragma once

#include <nonstd/keyboard.h>
#include <nonstd/keyboard/modifiers.h>


namespace keyboard {

struct qwerty {
    /** Return the QWERTY symbol corresponding to the given scancode. */
    static inline constexpr c_cstr to_cstr(scancode code) noexcept;

    static constexpr scancode A = scancode::A;
    static constexpr scancode B = scancode::B;
    static constexpr scancode C = scancode::C;
    static constexpr scancode D = scancode::D;
    static constexpr scancode E = scancode::E;
    static constexpr scancode F = scancode::F;
    static constexpr scancode G = scancode::G;
    static constexpr scancode H = scancode::H;
    static constexpr scancode I = scancode::I;
    static constexpr scancode J = scancode::J;
    static constexpr scancode K = scancode::K;
    static constexpr scancode L = scancode::L;
    static constexpr scancode M = scancode::M;
    static constexpr scancode N = scancode::N;
    static constexpr scancode O = scancode::O;
    static constexpr scancode P = scancode::P;
    static constexpr scancode Q = scancode::Q;
    static constexpr scancode R = scancode::R;
    static constexpr scancode S = scancode::S;
    static constexpr scancode T = scancode::T;
    static constexpr scancode U = scancode::U;
    static constexpr scancode V = scancode::V;
    static constexpr scancode W = scancode::W;
    static constexpr scancode X = scancode::X;
    static constexpr scancode Y = scancode::Y;
    static constexpr scancode Z = scancode::Z;

    static constexpr scancode n1 = scancode::kb_1;
    static constexpr scancode n2 = scancode::kb_2;
    static constexpr scancode n3 = scancode::kb_3;
    static constexpr scancode n4 = scancode::kb_4;
    static constexpr scancode n5 = scancode::kb_5;
    static constexpr scancode n6 = scancode::kb_6;
    static constexpr scancode n7 = scancode::kb_7;
    static constexpr scancode n8 = scancode::kb_8;
    static constexpr scancode n9 = scancode::kb_9;
    static constexpr scancode n0 = scancode::kb_0;

    static constexpr scancode Return = scancode::Return;
    static constexpr scancode Escape = scancode::Escape;
    static constexpr scancode Backspace = scancode::Backspace;
    static constexpr scancode Tab = scancode::Tab;
    static constexpr scancode Space = scancode::Space;

    static constexpr scancode Minus = scancode::kb_Minus;
    static constexpr scancode Equals = scancode::kb_Equals;

    static constexpr scancode LeftBracket = scancode::LeftBracket;
    static constexpr scancode RightBracket = scancode::RightBracket;

    static constexpr scancode Backslash = scancode::Backslash;

    static constexpr scancode Semicolon = scancode::Semicolon;
    static constexpr scancode Apostrophe = scancode::Apostrophe;
    static constexpr scancode Grave = scancode::Grave;

    static constexpr scancode Comma = scancode::Comma;
    static constexpr scancode Period = scancode::Period;

    static constexpr scancode ForwardSlash = scancode::ForwardSlash;

    static constexpr scancode CapsLock = scancode::CapsLock;

    static constexpr scancode F1  = scancode::F1;
    static constexpr scancode F2  = scancode::F2;
    static constexpr scancode F3  = scancode::F3;
    static constexpr scancode F4  = scancode::F4;
    static constexpr scancode F5  = scancode::F5;
    static constexpr scancode F6  = scancode::F6;
    static constexpr scancode F7  = scancode::F7;
    static constexpr scancode F8  = scancode::F8;
    static constexpr scancode F9  = scancode::F9;
    static constexpr scancode F10 = scancode::F10;
    static constexpr scancode F11 = scancode::F11;
    static constexpr scancode F12 = scancode::F12;
    static constexpr scancode F13 = scancode::F13;
    static constexpr scancode F14 = scancode::F14;
    static constexpr scancode F15 = scancode::F15;
    static constexpr scancode F16 = scancode::F16;
    static constexpr scancode F17 = scancode::F17;
    static constexpr scancode F18 = scancode::F18;
    static constexpr scancode F19 = scancode::F19;
    static constexpr scancode F20 = scancode::F20;
    static constexpr scancode F21 = scancode::F21;
    static constexpr scancode F22 = scancode::F22;
    static constexpr scancode F23 = scancode::F23;
    static constexpr scancode F24 = scancode::F24;

    static constexpr scancode PrintScreen = scancode::PrintScreen;
    static constexpr scancode ScrollLock = scancode::ScrollLock;
    static constexpr scancode Pause = scancode::Pause;
    static constexpr scancode Insert = scancode::Insert;

    static constexpr scancode Home = scancode::Home;
    static constexpr scancode PageUp = scancode::PageUp;
    static constexpr scancode Delete = scancode::Delete;
    static constexpr scancode End = scancode::End;
    static constexpr scancode PageDown = scancode::PageDown;

    static constexpr scancode RightArrow = scancode::RightArrow;
    static constexpr scancode LeftArrow = scancode::LeftArrow;
    static constexpr scancode DownArrow = scancode::DownArrow;
    static constexpr scancode UpArrow = scancode::UpArrow;

    static constexpr scancode Mute = scancode::Mute;
    static constexpr scancode VolumeUp = scancode::VolumeUp;
    static constexpr scancode VolumeDown = scancode::VolumeDown;

    static constexpr scancode LeftCtrl = scancode::LeftCtrl;
    static constexpr scancode LeftShift = scancode::LeftShift;
    static constexpr scancode LeftAlt = scancode::LeftAlt;
    static constexpr scancode LeftGUI = scancode::LeftGUI;
    static constexpr scancode RightCtrl = scancode::RightCtrl;
    static constexpr scancode RightShift = scancode::RightShift;
    static constexpr scancode RightAlt = scancode::RightAlt;
    static constexpr scancode RightGUI = scancode::RightGUI;

    struct keypad {
        static constexpr scancode NumLock = scancode::kp_NumLock;

        static constexpr scancode Divide = scancode::kp_Divide;
        static constexpr scancode Multiply = scancode::kp_Multiply;
        static constexpr scancode Minus = scancode::kp_Minus;
        static constexpr scancode Plus = scancode::kp_Plus;

        static constexpr scancode Enter = scancode::kp_Enter;

        static constexpr scancode n1 = scancode::kp_1;
        static constexpr scancode n2 = scancode::kp_2;
        static constexpr scancode n3 = scancode::kp_3;
        static constexpr scancode n4 = scancode::kp_4;
        static constexpr scancode n5 = scancode::kp_5;
        static constexpr scancode n6 = scancode::kp_6;
        static constexpr scancode n7 = scancode::kp_7;
        static constexpr scancode n8 = scancode::kp_8;
        static constexpr scancode n9 = scancode::kp_9;
        static constexpr scancode n0 = scancode::kp_0;

        static constexpr scancode Period = scancode::kp_Period;

        static constexpr scancode Equals = scancode::kp_Equals;
        static constexpr scancode Comma = scancode::kp_Comma;
    };
};


/** Types, Data, and Helpers for Print Overloads
 *  ============================================
 */
namespace detail {
    struct ScancodeToQwertyMapping {
        scancode code;
        c_cstr name;
    };

    constexpr ScancodeToQwertyMapping scancode_to_qwerty_map[] = {
        { keyboard::qwerty::A, "A" },
        { keyboard::qwerty::B, "B" },
        { keyboard::qwerty::C, "C" },
        { keyboard::qwerty::D, "D" },
        { keyboard::qwerty::E, "E" },
        { keyboard::qwerty::F, "F" },
        { keyboard::qwerty::G, "G" },
        { keyboard::qwerty::H, "H" },
        { keyboard::qwerty::I, "I" },
        { keyboard::qwerty::J, "J" },
        { keyboard::qwerty::K, "K" },
        { keyboard::qwerty::L, "L" },
        { keyboard::qwerty::M, "M" },
        { keyboard::qwerty::N, "N" },
        { keyboard::qwerty::O, "O" },
        { keyboard::qwerty::P, "P" },
        { keyboard::qwerty::Q, "Q" },
        { keyboard::qwerty::R, "R" },
        { keyboard::qwerty::S, "S" },
        { keyboard::qwerty::T, "T" },
        { keyboard::qwerty::U, "U" },
        { keyboard::qwerty::V, "V" },
        { keyboard::qwerty::W, "W" },
        { keyboard::qwerty::X, "X" },
        { keyboard::qwerty::Y, "Y" },
        { keyboard::qwerty::Z, "Z" },

        { keyboard::qwerty::n1, "1" },
        { keyboard::qwerty::n2, "2" },
        { keyboard::qwerty::n3, "3" },
        { keyboard::qwerty::n4, "4" },
        { keyboard::qwerty::n5, "5" },
        { keyboard::qwerty::n6, "6" },
        { keyboard::qwerty::n7, "7" },
        { keyboard::qwerty::n8, "8" },
        { keyboard::qwerty::n9, "9" },
        { keyboard::qwerty::n0, "0" },

        { keyboard::qwerty::Return, "Return" },
        { keyboard::qwerty::Escape, "Escape" },
        { keyboard::qwerty::Backspace, "Backspace" },
        { keyboard::qwerty::Tab, "Tab" },
        { keyboard::qwerty::Space, "Space" },

        { keyboard::qwerty::Minus, "-" },
        { keyboard::qwerty::Equals, "=" },

        { keyboard::qwerty::LeftBracket, "[" },
        { keyboard::qwerty::RightBracket, "]" },

        { keyboard::qwerty::Backslash, "\\" },

        { keyboard::qwerty::Semicolon, ";" },
        { keyboard::qwerty::Apostrophe, "'" },
        { keyboard::qwerty::Grave, "`" },

        { keyboard::qwerty::Comma, "," },
        { keyboard::qwerty::Period, "." },

        { keyboard::qwerty::ForwardSlash, "/" },

        { keyboard::qwerty::CapsLock, "Caps Lock" },

        { keyboard::qwerty::F1, "F1" },
        { keyboard::qwerty::F2, "F2" },
        { keyboard::qwerty::F3, "F3" },
        { keyboard::qwerty::F4, "F4" },
        { keyboard::qwerty::F5, "F5" },
        { keyboard::qwerty::F6, "F6" },
        { keyboard::qwerty::F7, "F7" },
        { keyboard::qwerty::F8, "F8" },
        { keyboard::qwerty::F9, "F9" },
        { keyboard::qwerty::F10, "F10" },
        { keyboard::qwerty::F11, "F11" },
        { keyboard::qwerty::F12, "F12" },
        { keyboard::qwerty::F13, "F13" },
        { keyboard::qwerty::F14, "F14" },
        { keyboard::qwerty::F15, "F15" },
        { keyboard::qwerty::F16, "F16" },
        { keyboard::qwerty::F17, "F17" },
        { keyboard::qwerty::F18, "F18" },
        { keyboard::qwerty::F19, "F19" },
        { keyboard::qwerty::F20, "F20" },
        { keyboard::qwerty::F21, "F21" },
        { keyboard::qwerty::F22, "F22" },
        { keyboard::qwerty::F23, "F23" },
        { keyboard::qwerty::F24, "F24" },

        { keyboard::qwerty::PrintScreen, "Print Screen" },
        { keyboard::qwerty::ScrollLock, "Scroll Lock" },
        { keyboard::qwerty::Pause, "Pause" },
        { keyboard::qwerty::Insert, "Insert" },

        { keyboard::qwerty::Home, "Home" },
        { keyboard::qwerty::PageUp, "Page Up" },
        { keyboard::qwerty::Delete, "Delete" },
        { keyboard::qwerty::End, "End" },
        { keyboard::qwerty::PageDown, "Page Down" },

        { keyboard::qwerty::RightArrow, "Right Arrow" },
        { keyboard::qwerty::LeftArrow, "Left Arrow" },
        { keyboard::qwerty::DownArrow, "Down Arrow" },
        { keyboard::qwerty::UpArrow, "Up Arrow" },

        { keyboard::qwerty::Mute, "Mute" },
        { keyboard::qwerty::VolumeUp, "Volume Up" },
        { keyboard::qwerty::VolumeDown, "Volume Down" },

        { keyboard::qwerty::LeftCtrl, "Left Ctrl" },
        { keyboard::qwerty::LeftShift, "Left Shift" },
        { keyboard::qwerty::LeftAlt, "Left Alt" },
        { keyboard::qwerty::LeftGUI, "Left " GUI_KEY_NAME },
        { keyboard::qwerty::RightCtrl, "Right Ctrl" },
        { keyboard::qwerty::RightShift, "Right Shift" },
        { keyboard::qwerty::RightAlt, "Right Alt" },
        { keyboard::qwerty::RightGUI, "Right " GUI_KEY_NAME },

        { keyboard::qwerty::keypad::NumLock, "Keypad Numlock" },

        { keyboard::qwerty::keypad::Divide, "Keypad Divide" },
        { keyboard::qwerty::keypad::Multiply, "Keypad Multiply" },
        { keyboard::qwerty::keypad::Minus, "Keypad Minus" },
        { keyboard::qwerty::keypad::Plus, "Keypad Plus" },

        { keyboard::qwerty::keypad::Enter, "Keypad Enter" },

        { keyboard::qwerty::keypad::n1, "Keypad 1" },
        { keyboard::qwerty::keypad::n2, "Keypad 2" },
        { keyboard::qwerty::keypad::n3, "Keypad 3" },
        { keyboard::qwerty::keypad::n4, "Keypad 4" },
        { keyboard::qwerty::keypad::n5, "Keypad 5" },
        { keyboard::qwerty::keypad::n6, "Keypad 6" },
        { keyboard::qwerty::keypad::n7, "Keypad 7" },
        { keyboard::qwerty::keypad::n8, "Keypad 8" },
        { keyboard::qwerty::keypad::n9, "Keypad 9" },
        { keyboard::qwerty::keypad::n0, "Keypad 0" },

        { keyboard::qwerty::keypad::Period, "Keypad Period" },

        { keyboard::qwerty::keypad::Equals, "Keypad Equals" },
        { keyboard::qwerty::keypad::Comma, "Keypad Comma" },
    };


    inline constexpr c_cstr _get_qwerty_name(
            scancode const code,
            ScancodeToQwertyMapping const * map = &scancode_to_qwerty_map[0])
    noexcept {
        constexpr c_cstr unknown = "/unknown/";
        return map != nullptr
             ? ( code == map->code
               ? map->name
               : _get_qwerty_name(code, map+1) )
             : unknown;
    }
} /* namespace detail */


/** Return the QWERTY symbol corresponding to the given scancode. */
inline constexpr c_cstr qwerty::to_cstr(scancode code) noexcept {
    return detail::_get_qwerty_name(code);
}

} /* namespace keyboard */

#undef GUI_KEY_NAME
