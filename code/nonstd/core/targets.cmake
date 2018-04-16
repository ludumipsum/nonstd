tfe_autotarget(
    NAME core
    HEADERS
        disallow_copy_and_assign.h
        enumerate.h
        error.h
        homogenize.h
        math.h
        primitive_types.h
        range.h
        require_linkage.h
        stacktrace.h
        symbol_stringifier.h
        type_traits_ext.h
        unused.h
    DEPENDS
        fmt::fmt
)

tfe_autotarget(
    TEST NAME enumerate
    SOURCES enumerate.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME primitive_types
    SOURCES primitive_types.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME range
    SOURCES range.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME type_traits_ext
    SOURCES type_traits_ext.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)
