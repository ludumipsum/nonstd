# nonstd::core
pm_autotarget(
    DEPENDS
        nonstd::core::core
        nonstd::core::mem
        nonstd::core::enumerate
        nonstd::core::error
        nonstd::core::range
        nonstd::core::stacktrace
)

pm_autotarget(
    NAME core
    HEADERS
        disallow_copy_and_assign.h
        homogenize.h
        math.h
        primitive_types.h
        require_linkage.h
        symbol_stringifier.h
        type_traits_ext.h
        unused.h
)

pm_autotarget(
    NAME mem
    HEADERS
        mem.h
    DEPENDS
        nonstd::core::core
)

pm_autotarget(
    NAME enumerate
    HEADERS
        enumerate.h
    DEPENDS
        nonstd::core::core
)

pm_autotarget(
    NAME error
    HEADERS
        error.h
    DEPENDS
        nonstd::core::core
        thirdparty::fmt
)

pm_autotarget(
    NAME range
    HEADERS
        range.h
    DEPENDS
        nonstd::core::error
        nonstd::core::core
)

pm_autotarget(
    NAME stacktrace
    HEADERS
        stacktrace.h
    DEPENDS
        nonstd::core::core
)



n2_platform_test(
    NAME primitive_types.test
    SOURCES primitive_types.test.cc
    DEPENDS
        nonstd::core::core
        platform::testrunner
)

n2_platform_test(
    NAME type_traits_ext.test
    SOURCES type_traits_ext.test.cc
    DEPENDS
        nonstd::core::core
        platform::testrunner
)

n2_platform_test(
    NAME enumerate.test
    SOURCES enumerate.test.cc
    DEPENDS
        nonstd::core::enumerate
        platform::testrunner
)
n2_platform_test(
    NAME range.test
    SOURCES range.test.cc
    DEPENDS
        nonstd::core::range
        platform::testrunner
)
