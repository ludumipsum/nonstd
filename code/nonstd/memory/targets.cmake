tfe_autotarget(
    TEST NAME nr_lazy
    SOURCES nr_lazy.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME nr_ptr
    SOURCES nr_ptr.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

