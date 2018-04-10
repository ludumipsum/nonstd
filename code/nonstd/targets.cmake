# Targets listing for nonstd

tfe_target(
    NAME nonstd-windows
    INCLUDE ..
    HEADERS windows.h
)

# TODO: Pull core out to a separate library
tfe_target(
    NAME nonstd
    INCLUDE ..
    HEADERS
        # TODO: Fix the header list too
        nonstd.h
    DEPENDS
        # TODO: Fix deps for this target
        Boost::boost fmt::fmt
)
