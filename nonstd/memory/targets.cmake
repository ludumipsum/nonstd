tfe_autotarget(
    NAME buffer
    HEADERS
        buffer.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME core_functions
    HEADERS
        core_functions.h
    DEPENDS
        nonstd::nonstd
        nonstd::optional
)

tfe_autotarget(
    NAME nr_lazy
    HEADERS
        nr_lazy.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory::nr_ptr
        nonstd::lazy
)

tfe_autotarget(
    NAME nr_ptr
    HEADERS
        nr_ptr.h
    DEPENDS
        nonstd::nonstd
        nonstd::utility_ext
        nonstd::memory::buffer
        nonstd::memory::core_functions
)


tfe_autotarget(
    TEST NAME nr_lazy.test
    SOURCES nr_lazy.test.cc
    DEPENDS
        nonstd::memory::nr_lazy
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME nr_ptr.test
    SOURCES nr_ptr.test.cc
    DEPENDS
        nonstd::memory::nr_ptr
        testrunner::testrunner
)

