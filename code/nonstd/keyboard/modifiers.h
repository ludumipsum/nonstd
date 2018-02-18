#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/enum_class_operators.h>
#include <nonstd/keyboard.h>
#include <nonstd/math.h>


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
    none = 0,
    left_shift = 1 << 0,
    left_ctrl  = 1 << 1,
    left_alt   = 1 << 2,
    left_gui   = 1 << 3,
    right_shift = 1 << 4,
    right_ctrl  = 1 << 5,
    right_alt   = 1 << 6,
    right_gui   = 1 << 7,
    shift = left_shift | right_shift,
    ctrl  = left_ctrl  | right_ctrl,
    alt   = left_alt   | right_alt,
    gui   = left_gui   | right_gui,
    fn = 1 << 8,
    caps_lock     = 1 << 9,
    num_lock      = 1 << 10,
    scroll_lock   = 1 << 11,
    function_lock = 1 << 12,
    all = 0xffff,
};
GENERATE_OPERATOR_OVERLOADS_FOR_ENUM_CLASS(modifier);


/** Convert a keyboard scancode to its corresponding modifier mask. */
inline constexpr keyboard::modifier
modifier_from_scancode(keyboard::scancode code)
noexcept {
    using keyboard::scancode;

    switch (code) {
    case scancode::left_ctrl:           { return modifier::left_ctrl;   }
    case scancode::left_shift:          { return modifier::left_shift;  }
    case scancode::left_alt:            { return modifier::left_alt;    }
    case scancode::left_gui:            { return modifier::left_gui;    }
    case scancode::right_ctrl:          { return modifier::right_ctrl;  }
    case scancode::right_shift:         { return modifier::right_shift; }
    case scancode::right_alt:           { return modifier::right_alt;   }
    case scancode::right_gui:           { return modifier::right_gui;   }
    case scancode::caps_lock:           /* fallthrough */
    case scancode::locking_caps_lock:   { return modifier::caps_lock;   }
    case scancode::numpad_num_lock:     /* fallthrough */
    case scancode::locking_num_lock:    { return modifier::num_lock;    }
    case scancode::scroll_lock:         /* fallthrough */
    case scancode::locking_scroll_lock: { return modifier::scroll_lock; }
    default:                            { return modifier::none;        }
    } /* switch (code) */
}


/** Types, Data, and Helpers for Print Overloads
 *  ============================================
 */
namespace detail {
    struct keyboard_modifier_name_mapping {
        modifier mod;
        c_cstr   name;
    };

    constexpr keyboard_modifier_name_mapping keyboard_modifier_name_map[] = {
        { modifier::left_shift,    "Left Shift"          },
        { modifier::left_ctrl,     "Left Ctrl"           },
        { modifier::left_alt,      "Left Alt"            },
        { modifier::left_gui,      "Left " GUI_KEY_NAME  },
        { modifier::right_shift,   "Right Shift"         },
        { modifier::right_ctrl,    "Right Ctrl"          },
        { modifier::right_alt,     "Right Alt"           },
        { modifier::right_gui,     "Right " GUI_KEY_NAME },
        { modifier::fn,            "Fn"                  },
        { modifier::caps_lock,     "Caps Lock"           },
        { modifier::num_lock,      "Num Lock"            },
        { modifier::scroll_lock,   "Scroll Lock"         },
        { modifier::function_lock, "Function Lock"       },
    };

    /**
     * Iterate over the keyboard_modifier_map looking for a matching modifier
     * value; return the corresponding human-readable name.
     *
     * TODO: We probably want to abstract this to part of a runtime process that
     * checks what layout is in use, then translates scancodes accordingly.
     * Relying on hardcoded transformations sounds like a recipe for bad.
     */
    inline constexpr
    c_cstr _get_modifier_name(modifier const modifier,
                              keyboard_modifier_name_mapping const * map =
                                  &keyboard_modifier_name_map[0])
    noexcept {
        // This function _only_ works on a single modifier which -- knowing that
        // keyboard::modifier models a bitfield -- means the underlying value of
        // the given modifier will always be a power of two.
        ASSERT(nonstd::is_power_of_two(modifier));
        return map != nullptr
             ? ( modifier == map->mod
               ? map->name
               : _get_modifier_name(modifier, map+1) )
             : nullptr;
    }

    inline
    std::string _get_modifier_list(modifier const modifiers,
                                   keyboard_modifier_name_mapping const * map =
                                       &keyboard_modifier_name_map[0])
    noexcept {
        std::vector<c_cstr> mod_list;

        if (is_any((modifiers & keyboard::modifier::left_shift))) {
            mod_list.push_back(_get_modifier_name(modifier::left_shift));
        }
        if (is_any((modifiers & keyboard::modifier::left_ctrl))) {
            mod_list.push_back(_get_modifier_name(modifier::left_ctrl));
        }
        if (is_any((modifiers & keyboard::modifier::left_alt))) {
            mod_list.push_back(_get_modifier_name(modifier::left_alt));
        }
        if (is_any((modifiers & keyboard::modifier::left_gui))) {
            mod_list.push_back(_get_modifier_name(modifier::left_gui));
        }
        if (is_any((modifiers & keyboard::modifier::right_shift))) {
            mod_list.push_back(_get_modifier_name(modifier::right_shift));
        }
        if (is_any((modifiers & keyboard::modifier::right_ctrl))) {
            mod_list.push_back(_get_modifier_name(modifier::right_ctrl));
        }
        if (is_any((modifiers & keyboard::modifier::right_alt))) {
            mod_list.push_back(_get_modifier_name(modifier::right_alt));
        }
        if (is_any((modifiers & keyboard::modifier::right_gui))) {
            mod_list.push_back(_get_modifier_name(modifier::right_gui));
        }
        if (is_any((modifiers & keyboard::modifier::fn))) {
            mod_list.push_back(_get_modifier_name(modifier::fn));
        }
        if (is_any((modifiers & keyboard::modifier::caps_lock))) {
            mod_list.push_back(_get_modifier_name(modifier::caps_lock));
        }
        if (is_any((modifiers & keyboard::modifier::num_lock))) {
            mod_list.push_back(_get_modifier_name(modifier::num_lock));
        }
        if (is_any((modifiers & keyboard::modifier::scroll_lock))) {
            mod_list.push_back(_get_modifier_name(modifier::scroll_lock));
        }
        if (is_any((modifiers & keyboard::modifier::function_lock))) {
            mod_list.push_back(_get_modifier_name(modifier::function_lock));
        }

        return fmt::format("{}", fmt::join(mod_list, " + "));
    }

} /* namespace detail */

inline void format_arg(fmt::BasicFormatter<char> & f,
                       c_cstr                    & format_str,
                       modifier const            & modifier) {
    f.writer().write( detail::_get_modifier_list(modifier) );
}
inline std::ostream & operator<< (std::ostream   & stream,
                                  modifier const & modifier) {
    return stream << detail::_get_modifier_list(modifier);
}

} /* namespace keyboard */
