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

    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,

    kb_1 = 30,
    kb_2 = 31,
    kb_3 = 32,
    kb_4 = 33,
    kb_5 = 34,
    kb_6 = 35,
    kb_7 = 36,
    kb_8 = 37,
    kb_9 = 38,
    kb_0 = 39,

    Return = 40,
    Escape = 41,
    Backspace = 42,
    Tab = 43,
    Space = 44,

    kb_Minus = 56,
    kb_Equals = 46,

    LeftBracket = 47,
    RightBracket = 48,

    Backslash = 49,
    NonUSHash = 50,

    Semicolon = 51,
    Apostrophe = 52,
    Grave = 53,

    Comma = 54,
    Period = 55,

    ForwardSlash = 56,

    CapsLock = 57,

    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,

    PrintScreen = 70,
    ScrollLock = 71,
    Pause = 72,
    Insert = 73,

    Home = 74,
    PageUp = 75,
    Delete = 76,
    End = 77,
    PageDown = 78,

    RightArrow = 79,
    LeftArrow = 80,
    DownArrow = 81,
    UpArrow = 82,

    kp_NumLock = 83,

    kp_Divide = 84,
    kp_Multiply = 85,
    kp_Minus = 86,
    kp_Plus = 87,

    kp_Enter = 88,

    kp_1 = 89,
    kp_2 = 90,
    kp_3 = 91,
    kp_4 = 92,
    kp_5 = 93,
    kp_6 = 94,
    kp_7 = 95,
    kp_8 = 96,
    kp_9 = 97,
    kp_0 = 98,

    kp_Period = 99,

    NonUSBackslash = 100,

    Application = 101,
    Power = 102,

    kp_Equals = 103,

    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,

    Execute = 116,
    Help = 117,
    Menu = 118,
    Select = 119,
    Stop = 120,
    Again = 121,
    Undo = 122,
    Cut = 123,
    Copy = 124,
    Paste = 125,
    Find = 126,

    Mute = 127,
    VolumeUp = 128,
    VolumeDown = 129,

    kp_Comma = 133,
    kp_Equals_AS400 = 134,

    International1 = 135,
    International2 = 136,
    International3 = 137,
    International4 = 138,
    International5 = 139,
    International6 = 140,
    International7 = 141,
    International8 = 142,
    International9 = 143,
    Lang1 = 144,
    Lang2 = 145,
    Lang3 = 146,
    Lang4 = 147,
    Lang5 = 148,
    Lang6 = 149,
    Lang7 = 150,
    Lang8 = 151,
    Lang9 = 152,

    AlternateErase = 153,
    SysReq = 154,
    Cancel = 155,
    Clear = 156,
    Prior = 157,
    AlternateReturn = 158,
    Separator = 159,
    Out = 160,
    Oper = 161,
    Clearagain = 162,
    CrSel = 163,
    ExSel = 164,

    kp_00 = 176,
    kp_000 = 177,

    ThousandsSeparator = 178,
    DecimalSeparator = 179,

    CurrencyUnit = 180,
    CurrencySubunit = 181,

    kp_LeftParen = 182,
    kp_RightParen = 183,
    kp_LeftBrace = 184,
    kp_RightBrace = 185,
    kp_TAB = 186,
    kp_BACKSPACE = 187,
    kp_A = 188,
    kp_B = 189,
    kp_C = 190,
    kp_D = 191,
    kp_E = 192,
    kp_F = 193,
    kp_XOR = 194,
    kp_Exponent = 195,
    kp_Percent = 196,
    kp_Less = 197,
    kp_Greater = 198,
    kp_Ampersand = 199,
    kp_DoubleAmpersand = 200,
    kp_VerticalBar = 201,
    kp_DoubleVerticalBar = 202,
    kp_Colon = 203,
    kp_Hash = 204,
    kp_Space = 205,
    kp_At = 206,
    kp_Bang = 207,
    kp_MemoryStore = 208,
    kp_MemoryRecall = 209,
    kp_MemoryClear = 210,
    kp_MemoryAdd = 211,
    kp_MemorySubtract = 212,
    kp_MemoryMultiply = 213,
    kp_MemoryDivide = 214,
    kp_PlusOrMinus = 215,
    kp_Clear = 216,
    kp_ClearEntry = 217,
    kp_Binary = 218,
    kp_Octal = 219,
    kp_Decimal = 220,
    kp_Hexadecimal = 221,

    LeftCtrl = 224,
    LeftShift = 225,
    LeftAlt = 226,
    LeftGUI = 227,
    RightCtrl = 228,
    RightShift = 229,
    RightAlt = 230,
    RightGUI = 231,

    _last = std::numeric_limits<std::underlying_type_t<scancode>>::max(),
};

} /* namespace keyboard */
