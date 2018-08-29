# Targets listing for nonstd
# ==========================
pm_autotarget(
    NAME nonstd
    HEADERS nonstd.h
    DEPENDS
        nonstd::core
        thirdparty::fmt
)

if(PM_OS_LINUX)
    # This is only actually required when building with sanitizers.
    target_link_libraries(nonstd.nonstd INTERFACE pthread)
endif()

pm_autotarget(
    NAME angle
    HEADERS angle.h
    DEPENDS
        nonstd::nonstd
        nonstd::cx_math
)

pm_autotarget(
    NAME chrono
    HEADERS chrono.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME color
    HEADERS color.h
    DEPENDS
        nonstd::nonstd
        nonstd::angle
)

pm_autotarget(
    NAME cx_math
    HEADERS cx_math.h
)

pm_autotarget(
    NAME enum_class_operators
    HEADERS enum_class_operators.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME four_char_code
    HEADERS four_char_code.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME future
    HEADERS future.h
    DEPENDS
        nonstd::nonstd
        nonstd::chrono
        nonstd::optional
)

pm_autotarget(
    NAME hash
    HEADERS hash.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME keyboard
    HEADERS keyboard.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME lazy
    HEADERS lazy.h
    DEPENDS
        nonstd::nonstd
        nonstd::optional_storage
)

pm_autotarget(
    NAME math
    HEADERS math.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME memory
    HEADERS memory.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory::buffer
        nonstd::memory::core_functions
        nonstd::memory::nr_lazy
        nonstd::memory::nr_ptr
)

pm_autotarget(
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

pm_autotarget(
    NAME optional_storage
    HEADERS optional_storage.h
    DEPENDS
        nonstd::utility_ext
)

pm_autotarget(
    NAME predicate
    HEADERS predicate.h
    DEPENDS
        nonstd::nonstd
        nonstd::utility_ext
)

pm_autotarget(
    NAME ring
    HEADERS ring.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory
        nonstd::scope_guard
)

pm_autotarget(
    NAME scope_guard
    HEADERS scope_guard.h
)

pm_autotarget(
    NAME special_member_filters
    HEADERS special_member_filters.h
)

pm_autotarget(
    NAME stream
    HEADERS stream.h
    DEPENDS
        nonstd::nonstd
        nonstd::memory
)

pm_autotarget(
    NAME type_name
    HEADERS type_name.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME utility_ext
    HEADERS utility_ext.h
    DEPENDS
        nonstd::utility_ext::types
        nonstd::utility_ext::helpers
        nonstd::utility_ext::compare
)

pm_autotarget(
    NAME valid_expression_tester
    HEADERS valid_expression_tester.h
    DEPENDS
        thirdparty::boost::preprocessor
)

pm_autotarget(
    NAME value_category
    HEADERS value_category.h
    DEPENDS
        nonstd::nonstd
)

pm_autotarget(
    NAME wallclock
    HEADERS wallclock.h
    DEPENDS
        nonstd::nonstd
        nonstd::chrono
)

pm_autotarget(
    NAME windows
    HEADERS windows.h
    DEPENDS
        nonstd::nonstd
)


# Tests for nonstd
# ================
n2_platform_test(
    NAME angle.test
    SOURCES angle.test.cc
    DEPENDS
        nonstd::angle
        platform::testrunner
)

n2_platform_test(
    NAME color.test
    SOURCES color.test.cc
    DEPENDS
        nonstd::color
        platform::testrunner
)

n2_platform_test(
    NAME cx_math.test
    SOURCES cx_math.test.cc
    DEPENDS
        nonstd::cx_math
        platform::testrunner
)

n2_platform_test(
    NAME cx.test
    SOURCES cx.test.cc
    DEPENDS
        platform::testrunner
)

n2_platform_test(
    NAME lazy.test
    SOURCES lazy.test.cc
    DEPENDS
        nonstd::lazy
        platform::testrunner
)

n2_platform_test(
    NAME math.test
    SOURCES math.test.cc
    DEPENDS
        nonstd::math
        platform::testrunner
)

n2_platform_test(
    NAME optional_storage.test
    SOURCES optional_storage.test.cc
    DEPENDS
        nonstd::optional_storage
        platform::testrunner
)

n2_platform_test(
    NAME optional.test
    SOURCES optional.test.cc
    HEADERS optional.test.compare_overloads.inl
            optional.test.disabled_special_members.inl
    DEPENDS
        nonstd::optional
        platform::testrunner
)

n2_platform_test(
    NAME predicate.test
    SOURCES predicate.test.cc
    DEPENDS
        nonstd::predicate
        platform::testrunner
)

n2_platform_test(
    NAME ring.test
    SOURCES ring.test.cc
    DEPENDS
        nonstd::ring
        platform::testrunner
)

n2_platform_test(
    NAME scope_guard.test
    SOURCES scope_guard.test.cc
    DEPENDS
        nonstd::scope_guard
        platform::testrunner
)

n2_platform_test(
    NAME special_member_filters.test
    SOURCES special_member_filters.test.cc
    DEPENDS
        nonstd::special_member_filters
        platform::testrunner
)

n2_platform_test(
    NAME stream.test
    SOURCES stream.test.cc
    DEPENDS
        nonstd::stream
        platform::testrunner
)

n2_platform_test(
    NAME valid_expression_tester.test
    SOURCES valid_expression_tester.test.cc
    DEPENDS
        nonstd::valid_expression_tester
        platform::testrunner
)

n2_platform_test(
    NAME wallclock.test
    SOURCES wallclock.test.cc
    DEPENDS
        nonstd::wallclock
        platform::testrunner
)
