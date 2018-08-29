pm_autotarget(
    NAME buffer
    HEADERS
        buffer.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME core_functions
    HEADERS
        core_functions.h
    DEPENDS
        nonstd::nonstd
        nonstd::optional
)

