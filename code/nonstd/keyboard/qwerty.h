#pragma once

#include <nonstd/keyboard.h>
#include <nonstd/keyboard/modifiers.h>


namespace keyboard {

struct qwerty {
    /** Return the QWERTY symbol corresponding to the given scancode. */
    static inline constexpr c_cstr to_cstr(scancode code) noexcept;

    static constexpr scancode a = scancode::a;
    static constexpr scancode b = scancode::b;
    static constexpr scancode c = scancode::c;
    static constexpr scancode d = scancode::d;
    static constexpr scancode e = scancode::e;
    static constexpr scancode f = scancode::f;
    static constexpr scancode g = scancode::g;
    static constexpr scancode h = scancode::h;
    static constexpr scancode i = scancode::i;
    static constexpr scancode j = scancode::j;
    static constexpr scancode k = scancode::k;
    static constexpr scancode l = scancode::l;
    static constexpr scancode m = scancode::m;
    static constexpr scancode n = scancode::n;
    static constexpr scancode o = scancode::o;
    static constexpr scancode p = scancode::p;
    static constexpr scancode q = scancode::q;
    static constexpr scancode r = scancode::r;
    static constexpr scancode s = scancode::s;
    static constexpr scancode t = scancode::t;
    static constexpr scancode u = scancode::u;
    static constexpr scancode v = scancode::v;
    static constexpr scancode w = scancode::w;
    static constexpr scancode x = scancode::x;
    static constexpr scancode y = scancode::y;
    static constexpr scancode z = scancode::z;

    static constexpr scancode one   = scancode::one;
    static constexpr scancode two   = scancode::two;
    static constexpr scancode three = scancode::three;
    static constexpr scancode four  = scancode::four;
    static constexpr scancode five  = scancode::five;
    static constexpr scancode six   = scancode::six;
    static constexpr scancode seven = scancode::seven;
    static constexpr scancode eight = scancode::eight;
    static constexpr scancode nine  = scancode::nine;
    static constexpr scancode zero  = scancode::zero;

    static constexpr scancode enter = scancode::enter;
    static constexpr scancode escape = scancode::escape;
    static constexpr scancode backspace = scancode::backspace;
    static constexpr scancode tab = scancode::tab;
    static constexpr scancode space = scancode::space;

    static constexpr scancode minus = scancode::minus;
    static constexpr scancode equals = scancode::equals;

    static constexpr scancode left_bracket = scancode::left_bracket;
    static constexpr scancode right_bracket = scancode::right_bracket;

    static constexpr scancode backslash = scancode::backslash;

    static constexpr scancode semicolon = scancode::semicolon;
    static constexpr scancode apostrophe = scancode::apostrophe;
    static constexpr scancode grave = scancode::grave;

    static constexpr scancode comma = scancode::comma;
    static constexpr scancode period = scancode::period;

    static constexpr scancode forward_slash = scancode::forward_slash;

    static constexpr scancode caps_lock = scancode::caps_lock;

    static constexpr scancode f1  = scancode::f1;
    static constexpr scancode f2  = scancode::f2;
    static constexpr scancode f3  = scancode::f3;
    static constexpr scancode f4  = scancode::f4;
    static constexpr scancode f5  = scancode::f5;
    static constexpr scancode f6  = scancode::f6;
    static constexpr scancode f7  = scancode::f7;
    static constexpr scancode f8  = scancode::f8;
    static constexpr scancode f9  = scancode::f9;
    static constexpr scancode f10 = scancode::f10;
    static constexpr scancode f11 = scancode::f11;
    static constexpr scancode f12 = scancode::f12;
    static constexpr scancode f13 = scancode::f13;
    static constexpr scancode f14 = scancode::f14;
    static constexpr scancode f15 = scancode::f15;
    static constexpr scancode f16 = scancode::f16;
    static constexpr scancode f17 = scancode::f17;
    static constexpr scancode f18 = scancode::f18;
    static constexpr scancode f19 = scancode::f19;
    static constexpr scancode f20 = scancode::f20;
    static constexpr scancode f21 = scancode::f21;
    static constexpr scancode f22 = scancode::f22;
    static constexpr scancode f23 = scancode::f23;
    static constexpr scancode f24 = scancode::f24;

    static constexpr scancode print_screen = scancode::print_screen;
    static constexpr scancode scroll_lock = scancode::scroll_lock;
    static constexpr scancode pause = scancode::pause;
    static constexpr scancode insert = scancode::insert;

    static constexpr scancode home      = scancode::home;
    static constexpr scancode page_up   = scancode::page_up;
    static constexpr scancode del       = scancode::del;
    static constexpr scancode end       = scancode::end;
    static constexpr scancode page_down = scancode::page_down;

    static constexpr scancode right_arrow = scancode::right_arrow;
    static constexpr scancode left_arrow = scancode::left_arrow;
    static constexpr scancode down_arrow = scancode::down_arrow;
    static constexpr scancode up_arrow = scancode::up_arrow;

    static constexpr scancode mute = scancode::mute;
    static constexpr scancode volume_up = scancode::volume_up;
    static constexpr scancode volume_down = scancode::volume_down;

    static constexpr scancode left_ctrl = scancode::left_ctrl;
    static constexpr scancode left_shift = scancode::left_shift;
    static constexpr scancode left_alt = scancode::left_alt;
    static constexpr scancode left_gui = scancode::left_gui;
    static constexpr scancode right_ctrl = scancode::right_ctrl;
    static constexpr scancode right_shift = scancode::right_shift;
    static constexpr scancode right_alt = scancode::right_alt;
    static constexpr scancode right_gui = scancode::right_gui;

    struct numpad {
        static constexpr scancode num_lock = scancode::numpad_num_lock;

        static constexpr scancode divide = scancode::numpad_divide;
        static constexpr scancode multiply = scancode::numpad_multiply;
        static constexpr scancode minus = scancode::numpad_minus;
        static constexpr scancode plus = scancode::numpad_plus;

        static constexpr scancode enter = scancode::numpad_enter;

        static constexpr scancode one   = scancode::numpad_one;
        static constexpr scancode two   = scancode::numpad_two;
        static constexpr scancode three = scancode::numpad_three;
        static constexpr scancode four  = scancode::numpad_four;
        static constexpr scancode five  = scancode::numpad_five;
        static constexpr scancode six   = scancode::numpad_six;
        static constexpr scancode seven = scancode::numpad_seven;
        static constexpr scancode eight = scancode::numpad_eight;
        static constexpr scancode nine  = scancode::numpad_nine;
        static constexpr scancode zero  = scancode::numpad_zero;

        static constexpr scancode period = scancode::numpad_period;

        static constexpr scancode equals = scancode::numpad_equals;
        static constexpr scancode comma = scancode::numpad_comma;
    };
};


/** Types, Data, and Helpers for Print Overloads
 *  ============================================
 */
namespace detail {
    struct scancode_to_qwerty_mapping {
        scancode code;
        c_cstr   name;
    };

    constexpr scancode_to_qwerty_mapping scancode_to_qwerty_map[] = {
        { keyboard::qwerty::a, "A" },
        { keyboard::qwerty::b, "B" },
        { keyboard::qwerty::c, "C" },
        { keyboard::qwerty::d, "D" },
        { keyboard::qwerty::e, "E" },
        { keyboard::qwerty::f, "F" },
        { keyboard::qwerty::g, "G" },
        { keyboard::qwerty::h, "H" },
        { keyboard::qwerty::i, "I" },
        { keyboard::qwerty::j, "J" },
        { keyboard::qwerty::k, "K" },
        { keyboard::qwerty::l, "L" },
        { keyboard::qwerty::m, "M" },
        { keyboard::qwerty::n, "N" },
        { keyboard::qwerty::o, "O" },
        { keyboard::qwerty::p, "P" },
        { keyboard::qwerty::q, "Q" },
        { keyboard::qwerty::r, "R" },
        { keyboard::qwerty::s, "S" },
        { keyboard::qwerty::t, "T" },
        { keyboard::qwerty::u, "U" },
        { keyboard::qwerty::v, "V" },
        { keyboard::qwerty::w, "W" },
        { keyboard::qwerty::x, "X" },
        { keyboard::qwerty::y, "Y" },
        { keyboard::qwerty::z, "Z" },

        { keyboard::qwerty::one,   "1" },
        { keyboard::qwerty::two,   "2" },
        { keyboard::qwerty::three, "3" },
        { keyboard::qwerty::four,  "4" },
        { keyboard::qwerty::five,  "5" },
        { keyboard::qwerty::six,   "6" },
        { keyboard::qwerty::seven, "7" },
        { keyboard::qwerty::eight, "8" },
        { keyboard::qwerty::nine,  "9" },
        { keyboard::qwerty::zero,  "0" },

        { keyboard::qwerty::enter, "Return" },
        { keyboard::qwerty::escape, "Escape" },
        { keyboard::qwerty::backspace, "Backspace" },
        { keyboard::qwerty::tab, "Tab" },
        { keyboard::qwerty::space, "Space" },

        { keyboard::qwerty::minus, "-" },
        { keyboard::qwerty::equals, "=" },

        { keyboard::qwerty::left_bracket, "[" },
        { keyboard::qwerty::right_bracket, "]" },

        { keyboard::qwerty::backslash, "\\" },

        { keyboard::qwerty::semicolon, ";" },
        { keyboard::qwerty::apostrophe, "'" },
        { keyboard::qwerty::grave, "`" },

        { keyboard::qwerty::comma, "," },
        { keyboard::qwerty::period, "." },

        { keyboard::qwerty::forward_slash, "/" },

        { keyboard::qwerty::caps_lock, "Caps Lock" },

        { keyboard::qwerty::f1,  "F1" },
        { keyboard::qwerty::f2,  "F2" },
        { keyboard::qwerty::f3,  "F3" },
        { keyboard::qwerty::f4,  "F4" },
        { keyboard::qwerty::f5,  "F5" },
        { keyboard::qwerty::f6,  "F6" },
        { keyboard::qwerty::f7,  "F7" },
        { keyboard::qwerty::f8,  "F8" },
        { keyboard::qwerty::f9,  "F9" },
        { keyboard::qwerty::f10, "F10" },
        { keyboard::qwerty::f11, "F11" },
        { keyboard::qwerty::f12, "F12" },
        { keyboard::qwerty::f13, "F13" },
        { keyboard::qwerty::f14, "F14" },
        { keyboard::qwerty::f15, "F15" },
        { keyboard::qwerty::f16, "F16" },
        { keyboard::qwerty::f17, "F17" },
        { keyboard::qwerty::f18, "F18" },
        { keyboard::qwerty::f19, "F19" },
        { keyboard::qwerty::f20, "F20" },
        { keyboard::qwerty::f21, "F21" },
        { keyboard::qwerty::f22, "F22" },
        { keyboard::qwerty::f23, "F23" },
        { keyboard::qwerty::f24, "F24" },

        { keyboard::qwerty::print_screen, "Print Screen" },
        { keyboard::qwerty::scroll_lock, "Scroll Lock" },
        { keyboard::qwerty::pause, "Pause" },
        { keyboard::qwerty::insert, "Insert" },

        { keyboard::qwerty::home,      "Home" },
        { keyboard::qwerty::page_up,   "Page Up" },
        { keyboard::qwerty::del,       "Delete" },
        { keyboard::qwerty::end,       "End" },
        { keyboard::qwerty::page_down, "Page Down" },

        { keyboard::qwerty::right_arrow, "Right Arrow" },
        { keyboard::qwerty::left_arrow, "Left Arrow" },
        { keyboard::qwerty::down_arrow, "Down Arrow" },
        { keyboard::qwerty::up_arrow, "Up Arrow" },

        { keyboard::qwerty::mute, "Mute" },
        { keyboard::qwerty::volume_up, "Volume Up" },
        { keyboard::qwerty::volume_down, "Volume Down" },

        { keyboard::qwerty::left_ctrl, "Left Ctrl" },
        { keyboard::qwerty::left_shift, "Left Shift" },
        { keyboard::qwerty::left_alt, "Left Alt" },
        { keyboard::qwerty::left_gui, "Left " GUI_KEY_NAME },
        { keyboard::qwerty::right_ctrl, "Right Ctrl" },
        { keyboard::qwerty::right_shift, "Right Shift" },
        { keyboard::qwerty::right_alt, "Right Alt" },
        { keyboard::qwerty::right_gui, "Right " GUI_KEY_NAME },

        { keyboard::qwerty::numpad::num_lock, "NumberPad Num Lock" },

        { keyboard::qwerty::numpad::divide, "NumberPad Divide" },
        { keyboard::qwerty::numpad::multiply, "NumberPad Multiply" },
        { keyboard::qwerty::numpad::minus, "NumberPad Minus" },
        { keyboard::qwerty::numpad::plus, "NumberPad Plus" },

        { keyboard::qwerty::numpad::enter, "NumberPad Enter" },

        { keyboard::qwerty::numpad::one,   "NumberPad 1" },
        { keyboard::qwerty::numpad::two,   "NumberPad 2" },
        { keyboard::qwerty::numpad::three, "NumberPad 3" },
        { keyboard::qwerty::numpad::four,  "NumberPad 4" },
        { keyboard::qwerty::numpad::five,  "NumberPad 5" },
        { keyboard::qwerty::numpad::six,   "NumberPad 6" },
        { keyboard::qwerty::numpad::seven, "NumberPad 7" },
        { keyboard::qwerty::numpad::eight, "NumberPad 8" },
        { keyboard::qwerty::numpad::nine,  "NumberPad 9" },
        { keyboard::qwerty::numpad::zero,  "NumberPad 0" },

        { keyboard::qwerty::numpad::period, "NumberPad Period" },

        { keyboard::qwerty::numpad::equals, "NumberPad Equals" },
        { keyboard::qwerty::numpad::comma, "NumberPad Comma" },
    };


    inline constexpr c_cstr _get_qwerty_name(
            scancode const code,
            scancode_to_qwerty_mapping const * map = &scancode_to_qwerty_map[0])
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
