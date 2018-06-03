tfe_autotarget(
    NAME types
    HEADERS
        any_t.h
        in_place_t.h
        void_t.h
)

tfe_autotarget(
    NAME helpers
    HEADERS
        nop.h
        to_cstr.h
        variadic_expand.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME compare
    HEADERS
        compare.h
    DEPENDS
        nonstd::nonstd
)


tfe_autotarget(
    TEST NAME compare.test
    SOURCES compare.test.cc
    DEPENDS
        nonstd::utility_ext::compare
        testrunner::testrunner
)
