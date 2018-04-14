# Targets listing for nonstd

tfe_autotarget(
    NAME core
    HEADERS
        core/disallow_copy_and_assign.h
        core/enumerate.h
        core/error.h
        core/homogenize.h
        core/math.h
        core/primitive_types.h
        core/range.h
        core/require_linkage.h
        core/stacktrace.h
        core/symbol_stringifier.h
        core/type_traits_ext.h
        core/unused.h
    DEPENDS
        fmt::fmt
)

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
        nonstd::core
        nonstd::keyboard
        nonstd::memory
        nonstd::utility_ext
        nonstd::windows

        # TODO: Fix thirdparty deps
        Boost::boost fmt::fmt
)
