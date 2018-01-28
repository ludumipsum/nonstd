/** Typesafe Keyboard Input
 *  =======================
 *  Keyboards are hard. Input is hard, generally speaking, and natural languages
 *  are _really_ hard, and keyboards are this wonderful mix of the two. Oh, also
 *  defining and enforcing standards can be a trick, and all of this work is
 *  contingent on keyboard inputput being correctly standardized, so... All of
 *  that to say, this is more complex than it should be, may be quite brittle.
 *  There definitely be dragons here.
 *
 *  The two core problems this set of files is trying solve are 1) platform
 *  independent keyboard input, and 2) language agnostic keyboard input.
 *  Platform agnostic is simple, it just requires a translation at the platform
 *  layer to some common interface. Language agnostic is more interesting,
 *  though; consider the ubiquitous WASD movement keys. If you don't want
 *  players on French AZERTY keyboards to have to learn some massively bizare
 *  muscle memory, their default should be ZQSD. Lucky for us, we're not the
 *  only ones who are interested in these goals.
 *
 *  The USB Human Interface Device specifications lay out a way to generate and
 *  interpret keyboard input in a language agnostic manner; the scancodes
 *  specified are based on the relative location of the key on the keyboard, not
 *  what's printed on the cap or where the keyboard was manufactured. (Note that
 *  the specification tacitly assumes a QWERTY layout when discussing
 *  alphabetical keys. The German QWERTZ keyboard is explicitly told to send the
 *  scancode for "Y" when "Z" is pressed, because the scancode for "Y" is
 *  actually the scancode for "the sixth key from the left on the top row." It
 *  works out, though, I promise.)
 *
 *  The below `enum class` was built using the values specified by the USB HID
 *  Usage Tables v1.12 (http://www.usb.org/developers/hidpage/Hut1_12v2.pdf),
 *  with inspiration from SDL's input implementation and Dietrich Epp's keycode
 *  project (https://github.com/depp/keycode).
 *  Translating platform-specific input to the HID values is left as an exercise
 *  for the reader, but has been done before (check out that keycode project).
 *
 *
 *  A maskable `enum class modifier` is provided in keyboard/modifier.h as a
 *  helper type, and does not correspond to any HID scancodes. Use the
 *  `keyboard::scancode_to_modifier` helper function to translate between
 *  scancodes and modifiers.
 *
 *  The `struct keyboard::qwerty` is what most will want to use when using
 *  keyboard input in a program; it's values are pulled from the HID scancodes,
 *  but are guaranteed to correspond to the common English-Language keycaps.
 *  Human-friendly strings can be extracted with the `scancode_as_qwerty`.
 *
 *  The other reason to use the keyboard::qwerty abstraction is so we can expand
 *  into a keyboard::qwertz, keyboard::azerty, etc.
 */
#pragma once

#include <nonstd/nonstd.h>


namespace keyboard {

enum class scancode : u8 {
    _first = 0,

    a = 4,
    b = 5,
    c = 6,
    d = 7,
    e = 8,
    f = 9,
    g = 10,
    h = 11,
    i = 12,
    j = 13,
    k = 14,
    l = 15,
    m = 16,
    n = 17,
    o = 18,
    p = 19,
    q = 20,
    r = 21,
    s = 22,
    t = 23,
    u = 24,
    v = 25,
    w = 26,
    x = 27,
    y = 28,
    z = 29,

    one   = 30,
    two   = 31,
    three = 32,
    four  = 33,
    five  = 34,
    six   = 35,
    seven = 36,
    eight = 37,
    nine  = 38,
    zero  = 39,

    enter = 40,
    escape = 41,
    backspace = 42,
    tab = 43,
    space = 44,

    minus = 45,
    equals = 46,

    left_bracket = 47,
    right_bracket = 48,

    backslash = 49,
    non_us_hash = 50,

    semicolon = 51,
    apostrophe = 52,
    grave = 53,

    comma = 54,
    period = 55,

    forward_slash = 56,

    caps_lock = 57,

    f1  = 58,
    f2  = 59,
    f3  = 60,
    f4  = 61,
    f5  = 62,
    f6  = 63,
    f7  = 64,
    f8  = 65,
    f9  = 66,
    f10 = 67,
    f11 = 68,
    f12 = 69,

    print_screen = 70,
    scroll_lock = 71,
    pause = 72,
    insert = 73,

    home = 74,
    page_up = 75,
    del = 76,
    end = 77,
    page_down = 78,

    right_arrow = 79,
    left_arrow = 80,
    down_arrow = 81,
    up_arrow = 82,

    numpad_num_lock = 83,

    numpad_divide = 84,
    numpad_multiply = 85,
    numpad_minus = 86,
    numpad_plus = 87,

    numpad_enter = 88,

    numpad_one   = 89,
    numpad_two   = 90,
    numpad_three = 91,
    numpad_four  = 92,
    numpad_five  = 93,
    numpad_six   = 94,
    numpad_seven = 95,
    numpad_eight = 96,
    numpad_nine  = 97,
    numpad_zero  = 98,

    numpad_period = 99,

    non_us_backslash = 100,

    application = 101,
    power = 102,

    numpad_equals = 103,

    f13 = 104,
    f14 = 105,
    f15 = 106,
    f16 = 107,
    f17 = 108,
    f18 = 109,
    f19 = 110,
    f20 = 111,
    f21 = 112,
    f22 = 113,
    f23 = 114,
    f24 = 115,

    execute = 116,
    help = 117,
    menu = 118,
    select = 119,
    stop = 120,
    again = 121,
    undo = 122,
    cut = 123,
    copy = 124,
    paste = 125,
    find = 126,

    mute = 127,
    volume_up = 128,
    volume_down = 129,

    numpad_comma = 133,
    numpad_equals_as400 = 134,

    international1 = 135,
    international2 = 136,
    international3 = 137,
    international4 = 138,
    international5 = 139,
    international6 = 140,
    international7 = 141,
    international8 = 142,
    international9 = 143,
    lang1 = 144,
    lang2 = 145,
    lang3 = 146,
    lang4 = 147,
    lang5 = 148,
    lang6 = 149,
    lang7 = 150,
    lang8 = 151,
    lang9 = 152,

    alternate_erase = 153,
    sys_req = 154,
    cancel = 155,
    clear = 156,
    prior = 157,
    alternate_return = 158,
    separator = 159,
    out = 160,
    oper = 161,
    clearagain = 162,
    cr_sel = 163,
    ex_sel = 164,

    numpad_00 = 176,
    numpad_000 = 177,

    thousands_separator = 178,
    decimal_separator = 179,

    currency_unit = 180,
    currency_subunit = 181,

    numpad_left_paren = 182,
    numpad_right_paren = 183,
    numpad_left_brace = 184,
    numpad_right_brace = 185,
    numpad_tab = 186,
    numpad_backspace = 187,
    numpad_a = 188,
    numpad_b = 189,
    numpad_c = 190,
    numpad_d = 191,
    numpad_e = 192,
    numpad_f = 193,
    numpad_xor = 194,
    numpad_exponent = 195,
    numpad_percent = 196,
    numpad_less = 197,
    numpad_greater = 198,
    numpad_ampersand = 199,
    numpad_double_ampersand = 200,
    numpad_vertical_bar = 201,
    numpad_double_vertical_bar = 202,
    numpad_colon = 203,
    numpad_hash = 204,
    numpad_space = 205,
    numpad_at = 206,
    numpad_bang = 207,
    numpad_memory_store = 208,
    numpad_memory_recall = 209,
    numpad_memory_clear = 210,
    numpad_memory_add = 211,
    numpad_memory_subtract = 212,
    numpad_memory_multiply = 213,
    numpad_memory_divide = 214,
    numpad_plus_or_minus = 215,
    numpad_clear = 216,
    numpad_clear_entry = 217,
    numpad_binary = 218,
    numpad_octal = 219,
    numpad_decimal = 220,
    numpad_hexadecimal = 221,

    left_ctrl = 224,
    left_shift = 225,
    left_alt = 226,
    left_gui = 227,
    right_ctrl = 228,
    right_shift = 229,
    right_alt = 230,
    right_gui = 231,

    _last = std::numeric_limits<std::underlying_type_t<scancode>>::max(),
};

} /* namespace keyboard */
