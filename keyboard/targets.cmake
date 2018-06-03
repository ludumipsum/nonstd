tfe_autotarget(
    NAME modifiers
    HEADERS
        modifiers.h
    DEPENDS
        nonstd::nonstd
        nonstd::enum_class_operators
        nonstd::keyboard
        nonstd::math
)

tfe_autotarget(
    NAME qwerty
    HEADERS
        qwerty.h
    DEPENDS
        nonstd::nonstd
        nonstd::keyboard::modifiers
)
