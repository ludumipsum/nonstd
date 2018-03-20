# Targets listing for nonstd

tfe_library(
    NAME nonstd-windows
    HEADERS windows.h
)

# TODO: Pull core out to a separate library
tfe_library(
    NAME nonstd
    HEADERS
        # TODO: Fix the header list too
        nonstd.h
    DEPENDS
        # TODO: Fix deps for this target
        Boost::boost fmt::fmt
)
