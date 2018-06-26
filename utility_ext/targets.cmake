pm_autotarget(
    NAME types
    HEADERS
        any_t.h
        in_place_t.h
        void_t.h
)

pm_autotarget(
    NAME helpers
    HEADERS
        nop.h
        to_cstr.h
        variadic_expand.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME compare
    HEADERS
        compare.h
    DEPENDS
        nonstd::nonstd
)


n2_platform_test(
    NAME compare.test
    SOURCES compare.test.cc
    DEPENDS
        nonstd::utility_ext::compare
        testrunner::testrunner
)
