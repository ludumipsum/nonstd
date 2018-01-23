#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/enum_class_operators.h>
#include <nonstd/keyboard.h>


// The USB HID tables list the nonstandard modifier key (Apple, ⌘, Windows,
// Prompt, Meta, [Enter Proprietary Crap Here], etc.) as the Left and Right GUI
// keys. This name dissatisfies me (because I'm liable to forget that), so thse
// macros expand to a platfor-specific name.
#if   defined(NONSTD_OS_MACOS)
#  define GUI_KEY_NAME "Command"
#elif defined(NONSTD_OS_WINDOWS)
#  define GUI_KEY_NAME "Windows"
#else
#  define GUI_KEY_NAME "Meta"
#endif


namespace keyboard {

enum class modifier : u16 {
    None = 0,
    LeftShift = 1 << 0,
    LeftCtrl  = 1 << 1,
    LeftAlt   = 1 << 2,
    LeftGUI   = 1 << 3,
    RightShift = 1 << 4,
    RightCtrl  = 1 << 5,
    RightAlt   = 1 << 6,
    RightGUI   = 1 << 7,
    Shift = LeftShift | RightShift,
    Ctrl  = LeftCtrl  | RightCtrl,
    Alt   = LeftAlt   | RightAlt,
    GUI   = LeftGUI   | RightGUI,
    Fn = 1 << 8,
    CapsLock     = 1 << 9,
    NumberLock   = 1 << 10,
    ScrollLock   = 1 << 11,
    FunctionLock = 1 << 12,
    All = 0xffff,
};
GENERATE_OPERATOR_OVERLOADS_FOR_ENUM_CLASS(modifier);


/** Convert a keyboard scancode to its corresponding modifier mask. */
inline constexpr keyboard::modifier
modifier_from_scancode(keyboard::scancode code)
noexcept {
    switch (code) {
    case keyboard::scancode::LeftCtrl:   { return modifier::LeftCtrl;   }
    case keyboard::scancode::LeftShift:  { return modifier::LeftShift;  }
    case keyboard::scancode::LeftAlt:    { return modifier::LeftAlt;    }
    case keyboard::scancode::LeftGUI:    { return modifier::LeftGUI;    }
    case keyboard::scancode::RightCtrl:  { return modifier::RightCtrl;  }
    case keyboard::scancode::RightShift: { return modifier::RightShift; }
    case keyboard::scancode::RightAlt:   { return modifier::RightAlt;   }
    case keyboard::scancode::RightGUI:   { return modifier::RightGUI;   }
    default:                             { return modifier::None;       }
    } /* switch (code) */
}


/** Types, Data, and Helpers for Print Overloads
 *  ============================================
 */
namespace detail {
    struct KeyboardModifierNameMapping {
        modifier modifier;
        c_cstr           name;
    };

    constexpr KeyboardModifierNameMapping keyboard_modifier_name_map[] = {
        { modifier::LeftShift, "Left Shift" },
        { modifier::LeftCtrl, "Left Ctrl" },
        { modifier::LeftAlt, "Left Alt" },
        { modifier::LeftGUI, "Left " GUI_KEY_NAME },
        { modifier::RightShift, "Right Shift" },
        { modifier::RightCtrl, "Right Ctrl" },
        { modifier::RightAlt, "Right Alt" },
        { modifier::RightGUI, "Right " GUI_KEY_NAME },
        { modifier::Fn, "Fn" },
        { modifier::CapsLock, "Caps Lock" },
        { modifier::NumberLock, "Number Lock" },
        { modifier::ScrollLock, "Scroll Lock" },
        { modifier::FunctionLock, "Function Lock" },
    };

    /**
     * Iterate over the keyboard_modifier_map looking for a matching modifier
     * value; return the corresponding human-readable name.
     *
     * TODO: This currently only works iff one modifier bit is set. This may be
     * insufficient, surprising, and/or bad.
     * TODO: We probably want to abstract this to part of a runtime process that
     * checks what layout is in use, then translates scancodes accordingly.
     * Relying on hardcoded transformations sounds like a recipe for bad.
     */
    inline constexpr c_cstr _get_modifier_name(
            modifier const modifier,
            KeyboardModifierNameMapping const * map =
                &keyboard_modifier_name_map[0])
    noexcept {
        return map != nullptr
             ? ( modifier == map->modifier
               ? map->name
               : _get_modifier_name(modifier, map+1) )
             : nullptr;
    }
} /* namespace detail */

inline void format_arg(fmt::BasicFormatter<char> & f,
                       c_cstr                    & format_str,
                       modifier const            & modifier) {
    f.writer().write( detail::_get_modifier_name(modifier) );
}
inline std::ostream & operator<< (std::ostream & stream,
                                  modifier const & modifier) {
    return stream << detail::_get_modifier_name(modifier);
}

} /* namespace keyboard */
