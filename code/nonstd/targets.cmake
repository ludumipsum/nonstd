# Targets listing for nonstd

tfe_autotarget(
    NAME keyboard
    HEADERS
        keyboard.h
        keyboard/modifiers.h
        keyboard/qwerty.h
)

tfe_autotarget(
    NAME memory
    HEADERS
        memory.h
        memory/buffer.h
        memory/core_functions.h
        memory/nr_lazy.h
        memory/nr_ptr.h
)

tfe_autotarget(
    NAME utility_ext
    HEADERS
        utility_ext/any_t.h
        utility_ext/compare.h
        utility_ext/in_place_t.h
        utility_ext/nop.h
        utility_ext/to_cstr.h
        utility_ext/variadic_expand.h
        utility_ext/void_t.h
)

tfe_autotarget(
    NAME windows
    HEADERS
        windows.h
)

# TODO: Pull core out to a separate library
tfe_autotarget(
    NAME nonstd
    HEADERS
        # TODO: Fix the header list too
        nonstd.h
        angle.h
        array.h
        chrono.h
        color.h
        cx_math.h
        enum_class_operators.h
        four_char_code.h
        future.h
        hash_table.h
        hash.h
        lazy.h
        math.h
        optional_storage.h
        optional.h
        predicate.h
        ring.h
        scope_guard.h
        special_member_filters.h
        stream.h
        type_name.h
        utility_ext.h
        valid_expression_tester.h
        value_category.h
        wallclock.h
    DEPENDS
        nonstd::core::core
        nonstd::keyboard
        nonstd::memory
        nonstd::utility_ext
        nonstd::windows
        thirdparty::boost::preprocessor
        thirdparty::fmt
)

tfe_autotarget(
    TEST NAME angle
    SOURCES angle.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME array
    SOURCES array.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME color
    SOURCES color.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME cx_math
    SOURCES cx_math.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME cx
    SOURCES cx.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME hash_table
    SOURCES hash_table.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME lazy
    SOURCES lazy.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME optional_storage
    SOURCES optional_storage.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME optional
    SOURCES optional.test.cc
    HEADERS optional.test.compare_overloads.inl
            optional.test.disabled_special_members.inl
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME predicate
    SOURCES predicate.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME ring
    SOURCES ring.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME scope_guard
    SOURCES scope_guard.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME special_member_filters
    SOURCES special_member_filters.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME stream
    SOURCES stream.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME valid_expression_tester
    SOURCES valid_expression_tester.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)

tfe_autotarget(
    TEST NAME wallclock
    SOURCES wallclock.test.cc
    DEPENDS
        nonstd::nonstd
        testrunner::testrunner
)
