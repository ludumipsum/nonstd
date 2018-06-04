# Targets listing for nonstd
# ==========================
tfe_autotarget(
    NAME nonstd
    HEADERS nonstd.h
    DEPENDS
        nonstd::core
        thirdparty::fmt
)

tfe_autotarget(
    NAME angle
    HEADERS angle.h
    DEPENDS
        nonstd::nonstd
        nonstd::cx_math
)

tfe_autotarget(
    NAME array
    HEADERS array.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory
)

tfe_autotarget(
    NAME chrono
    HEADERS chrono.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME color
    HEADERS color.h
    DEPENDS
        nonstd::nonstd
        nonstd::angle
)

tfe_autotarget(
    NAME cx_math
    HEADERS cx_math.h
)

tfe_autotarget(
    NAME enum_class_operators
    HEADERS enum_class_operators.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME four_char_code
    HEADERS four_char_code.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME future
    HEADERS future.h
    DEPENDS
        nonstd::nonstd
        nonstd::chrono
        nonstd::optional
)

tfe_autotarget(
    NAME hash
    HEADERS hash.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME hash_table
    HEADERS hash_table.h
    DEPENDS
        nonstd::nonstd
        nonstd::math
        nonstd::memory
        nonstd::optional
        nonstd::utility_ext
)

tfe_autotarget(
    NAME keyboard
    HEADERS keyboard.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME lazy
    HEADERS lazy.h
    DEPENDS
        nonstd::nonstd
        nonstd::optional_storage
)

tfe_autotarget(
    NAME math
    HEADERS math.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME memory
    HEADERS memory.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory::buffer
        nonstd::memory::core_functions
        nonstd::memory::nr_lazy
        nonstd::memory::nr_ptr
)

tfe_autotarget(
    NAME optional
    HEADERS optional.h
    DEPENDS
        nonstd::nonstd
        nonstd::optional_storage
        nonstd::special_member_filters
        nonstd::type_name
        nonstd::utility_ext
        nonstd::valid_expression_tester
)

tfe_autotarget(
    NAME optional_storage
    HEADERS optional_storage.h
    DEPENDS
        nonstd::utility_ext
)

tfe_autotarget(
    NAME predicate
    HEADERS predicate.h
    DEPENDS
        nonstd::nonstd
        nonstd::utility_ext
)

tfe_autotarget(
    NAME ring
    HEADERS ring.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory
        nonstd::scope_guard
)

tfe_autotarget(
    NAME scope_guard
    HEADERS scope_guard.h
)

tfe_autotarget(
    NAME special_member_filters
    HEADERS special_member_filters.h
)

tfe_autotarget(
    NAME stream
    HEADERS stream.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory
)

tfe_autotarget(
    NAME type_name
    HEADERS type_name.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME utility_ext
    HEADERS utility_ext.h
    DEPENDS
        nonstd::utility_ext::types
        nonstd::utility_ext::helpers
        nonstd::utility_ext::compare
)

tfe_autotarget(
    NAME valid_expression_tester
    HEADERS valid_expression_tester.h
    DEPENDS
        thirdparty::boost::preprocessor
)

tfe_autotarget(
    NAME value_category
    HEADERS value_category.h
    DEPENDS
        nonstd::nonstd
)

tfe_autotarget(
    NAME wallclock
    HEADERS wallclock.h
    DEPENDS
        nonstd::nonstd
        nonstd::chrono
)

tfe_autotarget(
    NAME window
    HEADERS window.h
    DEPENDS
        nonstd::nonstd
)


# Tests for nonstd
# ================
tfe_autotarget(
    TEST NAME angle.test
    SOURCES angle.test.cc
    DEPENDS
        nonstd::angle
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME array.test
    SOURCES array.test.cc
    DEPENDS
        nonstd::array
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME color.test
    SOURCES color.test.cc
    DEPENDS
        nonstd::color
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME cx_math.test
    SOURCES cx_math.test.cc
    DEPENDS
        nonstd::cx_math
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME cx.test
    SOURCES cx.test.cc
    DEPENDS
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME hash_table.test
    SOURCES hash_table.test.cc
    DEPENDS
        nonstd::hash_table
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME lazy.test
    SOURCES lazy.test.cc
    DEPENDS
        nonstd::lazy
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME math.test
    SOURCES math.test.cc
    DEPENDS
        nonstd::math
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME optional_storage.test
    SOURCES optional_storage.test.cc
    DEPENDS
        nonstd::optional_storage
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME optional.test
    SOURCES optional.test.cc
    HEADERS optional.test.compare_overloads.inl
            optional.test.disabled_special_members.inl
    DEPENDS
        nonstd::optional
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME predicate.test
    SOURCES predicate.test.cc
    DEPENDS
        nonstd::predicate
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME ring.test
    SOURCES ring.test.cc
    DEPENDS
        nonstd::ring
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME scope_guard.test
    SOURCES scope_guard.test.cc
    DEPENDS
        nonstd::scope_guard
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME special_member_filters.test
    SOURCES special_member_filters.test.cc
    DEPENDS
        nonstd::special_member_filters
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME stream.test
    SOURCES stream.test.cc
    DEPENDS
        nonstd::stream
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME valid_expression_tester.test
    SOURCES valid_expression_tester.test.cc
    DEPENDS
        nonstd::valid_expression_tester
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME wallclock.test
    SOURCES wallclock.test.cc
    DEPENDS
        nonstd::wallclock
        testrunner::testrunner
)
