/** Optional Storage Smoke Tests
 *  ============================
 *  GOAL: Validate the basic functionality of the `optional_storage<T>` class,
 *  and guarantee correct constexpr / trivial propagation for special member
 *  functions (notably dtor, copy ctor, and move ctor).
 */

#include <nonstd/optional_storage.h>
#include <testrunner/testrunner.h>

#include <nonstd/nonstd.h>


namespace nonstd_test {
namespace optional_storage {

using nonstd::optional_storage;

using std::is_const_v;
using std::is_trivially_copy_constructible_v;
using std::is_trivially_copy_constructible_v;
using std::is_trivially_destructible_v;
using std::is_trivially_move_constructible_v;
using std::is_trivially_move_constructible_v;
using std::remove_reference_t;


struct Trivial {
    int i;

    constexpr Trivial(int _i) : i ( _i ) { }

    Trivial() = default;
    Trivial(Trivial const & rhs) = default;
    Trivial(Trivial && rhs) = default;
    ~Trivial() = default;
};
ENFORCE( is_trivially_copy_constructible_v<Trivial>);
ENFORCE( is_trivially_move_constructible_v<Trivial>);
ENFORCE( is_trivially_destructible_v<Trivial>);

struct NonTrivial_Copy {
    int i;

    constexpr NonTrivial_Copy(int _i) : i ( _i ) { }

    NonTrivial_Copy() = default;
    constexpr NonTrivial_Copy(NonTrivial_Copy const & rhs) noexcept : i ( rhs.i ) { }
    NonTrivial_Copy(NonTrivial_Copy && rhs) = default;
    ~NonTrivial_Copy() = default;
};
ENFORCE(!is_trivially_copy_constructible_v<NonTrivial_Copy>);
ENFORCE( is_trivially_move_constructible_v<NonTrivial_Copy>);
ENFORCE( is_trivially_destructible_v<NonTrivial_Copy>);

struct NonTrivial_Move {
    int i;

    constexpr NonTrivial_Move(int _i) : i ( _i ) { }

    NonTrivial_Move() = default;
    NonTrivial_Move(NonTrivial_Move const & rhs) = default;
    constexpr NonTrivial_Move(NonTrivial_Move && rhs) noexcept : i ( std::move(rhs.i) ) { }
    ~NonTrivial_Move() = default;
};
ENFORCE( is_trivially_copy_constructible_v<NonTrivial_Move>);
ENFORCE(!is_trivially_move_constructible_v<NonTrivial_Move>);
ENFORCE( is_trivially_destructible_v<NonTrivial_Move>);

struct NonTrivial_CopyMove {
    int i;

    constexpr NonTrivial_CopyMove(int _i) : i ( _i ) { }

    NonTrivial_CopyMove() = default;
    constexpr NonTrivial_CopyMove(NonTrivial_CopyMove const & rhs) noexcept : i ( rhs.i ) { }
    constexpr NonTrivial_CopyMove(NonTrivial_CopyMove && rhs) noexcept : i ( std::move(rhs.i) ) { }
    ~NonTrivial_CopyMove() = default;
};
ENFORCE(!is_trivially_copy_constructible_v<NonTrivial_CopyMove>);
ENFORCE(!is_trivially_move_constructible_v<NonTrivial_CopyMove>);
ENFORCE( is_trivially_destructible_v<NonTrivial_CopyMove>);

struct NonTrivial_Dtor {
    int i;

    constexpr NonTrivial_Dtor(int _i) : i ( _i ) { }

    NonTrivial_Dtor() = default;
    NonTrivial_Dtor(NonTrivial_Dtor const & rhs) = default;
    NonTrivial_Dtor(NonTrivial_Dtor && rhs) = default;
    ~NonTrivial_Dtor() noexcept {
        //noop
    }
};
static_assert(!is_trivially_copy_constructible_v<NonTrivial_Dtor>);
static_assert(!is_trivially_move_constructible_v<NonTrivial_Dtor>);
static_assert(!is_trivially_destructible_v<NonTrivial_Dtor>);


/** Optional Storage -- Fully Trivial Types
 *  ---------------------------------------
 */
constexpr bool play_with_Trivial() {
    optional_storage<Trivial> empty { };
    optional_storage<Trivial> full { Trivial { 42 } };
    optional_storage<Trivial> copy { full };
    optional_storage<Trivial> move { std::move(full) };
    return true;
}

/** Optional Storage -- Non-Trivially Copy Ctor
 *  -------------------------------------------
 */
constexpr bool play_with_NonTrivial_Copy() {
    optional_storage<NonTrivial_Copy> empty { };
    optional_storage<NonTrivial_Copy> full { NonTrivial_Copy { 42 } };
    // Can't copy
 // optional_storage<NonTrivial_Copy> copy { full };
    optional_storage<NonTrivial_Copy> move { std::move(full) };
    return true;
}

/** Optional Storage -- Non-Trivially Move Ctor
 *  -------------------------------------------
 */
constexpr bool play_with_NonTrivial_Move() {
    optional_storage<NonTrivial_Move> empty { };
    optional_storage<NonTrivial_Move> full { NonTrivial_Move { 42 } };
    optional_storage<NonTrivial_Move> copy { full };
    // Can't move
 // optional_storage<NonTrivial_Move> move { std::move(full) };
    return true;
}

/** Optional Storage -- Non-Trivially Copy & Move Ctors
 *  ---------------------------------------------------
 */
constexpr bool play_with_NonTrivial_CopyMove() {
    optional_storage<NonTrivial_CopyMove> empty { };
    optional_storage<NonTrivial_CopyMove> full { NonTrivial_CopyMove { 42 } };
    // Can't move or copy
 // optional_storage<NonTrivial_CopyMove> copy { full };
 // optional_storage<NonTrivial_CopyMove> move { std::move(full) };
    return true;
}

/** Optional Storage -- Non-Trivially Dtor
 *  --------------------------------------
 */
constexpr bool play_with_NonTrivial_Dtor() {
    // Can't do anything.
 // optional_storage<NonTrivial_Dtor> empty { };
 // optional_storage<NonTrivial_Dtor> full { NonTrivial_Dtor { 42 } };
 // optional_storage<NonTrivial_Dtor> copy { full };
 // optional_storage<NonTrivial_Dtor> move { std::move(full) };
    return true;
}


TEST_CASE("Optional Storage Types", "[nonstd][optional_storage]") {
    SECTION("that wrap types that are fully trivial") {
        ENFORCE( is_trivially_copy_constructible_v<optional_storage<Trivial>>);
        ENFORCE( is_trivially_move_constructible_v<optional_storage<Trivial>>);
        ENFORCE( is_trivially_destructible_v<optional_storage<Trivial>>);

        constexpr bool works_in_constexpr_context = play_with_Trivial();
        REQUIRE(works_in_constexpr_context);
    }
    SECTION("for `T`s that are non-trivially copy constructible") {
        ENFORCE(!is_trivially_copy_constructible_v<optional_storage<NonTrivial_Copy>>);
        ENFORCE( is_trivially_move_constructible_v<optional_storage<NonTrivial_Copy>>);
        ENFORCE( is_trivially_destructible_v<optional_storage<NonTrivial_Copy>>);

        constexpr bool works_in_constexpr_context = play_with_NonTrivial_Copy();
        REQUIRE(works_in_constexpr_context);
    }
    SECTION("for `T`s that are non-trivially move constructible") {
        ENFORCE( is_trivially_copy_constructible_v<optional_storage<NonTrivial_Move>>);
        ENFORCE(!is_trivially_move_constructible_v<optional_storage<NonTrivial_Move>>);
        ENFORCE( is_trivially_destructible_v<optional_storage<NonTrivial_Move>>);

        constexpr bool works_in_constexpr_context = play_with_NonTrivial_Move();
        REQUIRE(works_in_constexpr_context);
    }
    SECTION("for `T`s that are non-trivially copy or move constructible") {
        ENFORCE(!is_trivially_copy_constructible_v<optional_storage<NonTrivial_CopyMove>>);
        ENFORCE(!is_trivially_move_constructible_v<optional_storage<NonTrivial_CopyMove>>);
        ENFORCE( is_trivially_destructible_v<optional_storage<NonTrivial_CopyMove>>);

        constexpr bool works_in_constexpr_context = play_with_NonTrivial_CopyMove();
        REQUIRE(works_in_constexpr_context);
    }
    SECTION("for `T`s that are non-trivially destructible") {
        ENFORCE(!is_trivially_copy_constructible_v<optional_storage<NonTrivial_Dtor>>);
        ENFORCE(!is_trivially_move_constructible_v<optional_storage<NonTrivial_Dtor>>);
        ENFORCE(!is_trivially_destructible_v<optional_storage<NonTrivial_Dtor>>);

        constexpr bool works_in_constexpr_context = play_with_NonTrivial_Dtor();
        REQUIRE(works_in_constexpr_context);
    }

    SECTION("should be constructible and observable") {
        optional_storage<int> val_primitive               { 1 };
        optional_storage<int> empty_primitive             {   };
        optional_storage<Trivial> val_trivial             { 1 };
        optional_storage<Trivial> empty_trivial           {   };
        optional_storage<NonTrivial_Copy> val_nt_c        { 1 };
        optional_storage<NonTrivial_Copy> empty_nt_c      {   };
        optional_storage<NonTrivial_Move> val_nt_m        { 1 };
        optional_storage<NonTrivial_Move> empty_nt_m      {   };
        optional_storage<NonTrivial_CopyMove> val_nt_cm   { 1 };
        optional_storage<NonTrivial_CopyMove> empty_nt_cm {   };
        optional_storage<NonTrivial_Dtor> val_nt_d        { 1 };
        optional_storage<NonTrivial_Dtor> empty_nt_d      {   };

        REQUIRE(val_primitive.has_value()   == true);
        REQUIRE(val_primitive.get_value()   == 1);
        REQUIRE(empty_primitive.has_value() == false);

        REQUIRE(val_trivial.has_value()   == true);
        REQUIRE(val_trivial.get_value().i == 1);
        REQUIRE(empty_trivial.has_value() == false);

        REQUIRE(val_nt_c.has_value()   == true);
        REQUIRE(val_nt_c.get_value().i == 1);
        REQUIRE(empty_nt_c.has_value() == false);

        REQUIRE(val_nt_m.has_value()   == true);
        REQUIRE(val_nt_m.get_value().i == 1);
        REQUIRE(empty_nt_m.has_value() == false);

        REQUIRE(val_nt_cm.has_value()   == true);
        REQUIRE(val_nt_cm.get_value().i == 1);
        REQUIRE(empty_nt_cm.has_value() == false);

        REQUIRE(val_nt_d.has_value()   == true);
        REQUIRE(val_nt_d.get_value().i == 1);
        REQUIRE(empty_nt_d.has_value() == false);
    }

    SECTION("should be able to defer construction") {
        optional_storage<int>                 maybe_primitive { };
        optional_storage<Trivial>             maybe_trivial   { };
        optional_storage<NonTrivial_Copy>     maybe_nt_c      { };
        optional_storage<NonTrivial_Move>     maybe_nt_m      { };
        optional_storage<NonTrivial_CopyMove> maybe_nt_cm     { };
        optional_storage<NonTrivial_Dtor>     maybe_nt_d      { };

        REQUIRE(maybe_primitive.has_value() == false);
        REQUIRE(maybe_trivial.has_value()   == false);
        REQUIRE(maybe_nt_c.has_value()      == false);
        REQUIRE(maybe_nt_m.has_value()      == false);
        REQUIRE(maybe_nt_cm.has_value()     == false);
        REQUIRE(maybe_nt_d.has_value()      == false);

        auto& maybe_primitive_val = maybe_primitive.construct_value(1);
        auto& maybe_trivial_val = maybe_trivial.construct_value(1);
        auto& maybe_nt_c_val = maybe_nt_c.construct_value(1);
        auto& maybe_nt_m_val = maybe_nt_m.construct_value(1);
        auto& maybe_nt_cm_val = maybe_nt_cm.construct_value(1);
        auto& maybe_nt_d_val = maybe_nt_d.construct_value(1);

        REQUIRE(maybe_primitive_val == 1);
        REQUIRE(maybe_trivial_val.i == 1);
        REQUIRE(maybe_nt_c_val.i    == 1);
        REQUIRE(maybe_nt_m_val.i    == 1);
        REQUIRE(maybe_nt_cm_val.i   == 1);
        REQUIRE(maybe_nt_d_val.i    == 1);


        REQUIRE(maybe_primitive.has_value() == true);
        REQUIRE(maybe_trivial.has_value()   == true);
        REQUIRE(maybe_nt_c.has_value()      == true);
        REQUIRE(maybe_nt_m.has_value()      == true);
        REQUIRE(maybe_nt_cm.has_value()     == true);
        REQUIRE(maybe_nt_d.has_value()      == true);

        REQUIRE(maybe_primitive.get_value() == 1);
        REQUIRE(maybe_trivial.get_value().i == 1);
        REQUIRE(maybe_nt_c.get_value().i    == 1);
        REQUIRE(maybe_nt_m.get_value().i    == 1);
        REQUIRE(maybe_nt_cm.get_value().i   == 1);
        REQUIRE(maybe_nt_d.get_value().i    == 1);
    }

    SECTION("should be able to construct and then destruct a value") {
        optional_storage<int>                 maybe_primitive { 1 };
        optional_storage<Trivial>             maybe_trivial   { 1 };
        optional_storage<NonTrivial_Copy>     maybe_nt_c      { 1 };
        optional_storage<NonTrivial_Move>     maybe_nt_m      { 1 };
        optional_storage<NonTrivial_CopyMove> maybe_nt_cm     { 1 };
        optional_storage<NonTrivial_Dtor>     maybe_nt_d      { 1 };

        REQUIRE(maybe_primitive.has_value() == true);
        REQUIRE(maybe_trivial.has_value()   == true);
        REQUIRE(maybe_nt_c.has_value()      == true);
        REQUIRE(maybe_nt_m.has_value()      == true);
        REQUIRE(maybe_nt_cm.has_value()     == true);
        REQUIRE(maybe_nt_d.has_value()      == true);

        maybe_primitive.remove_value();
        maybe_trivial.remove_value();
        maybe_nt_c.remove_value();
        maybe_nt_m.remove_value();
        maybe_nt_cm.remove_value();
        maybe_nt_d.remove_value();

        REQUIRE(maybe_primitive.has_value() == false);
        REQUIRE(maybe_trivial.has_value()   == false);
        REQUIRE(maybe_nt_c.has_value()      == false);
        REQUIRE(maybe_nt_m.has_value()      == false);
        REQUIRE(maybe_nt_cm.has_value()     == false);
        REQUIRE(maybe_nt_d.has_value()      == false);
    }

    SECTION("that wrap const types") {
        SECTION("should be constructible and observable") {
            optional_storage<const int> val_primitive               { 1 };
            optional_storage<const int> empty_primitive             {   };
            optional_storage<const Trivial> val_trivial             { 1 };
            optional_storage<const Trivial> empty_trivial           {   };
            optional_storage<const NonTrivial_Copy> val_nt_c        { 1 };
            optional_storage<const NonTrivial_Copy> empty_nt_c      {   };
            optional_storage<const NonTrivial_Move> val_nt_m        { 1 };
            optional_storage<const NonTrivial_Move> empty_nt_m      {   };
            optional_storage<const NonTrivial_CopyMove> val_nt_cm   { 1 };
            optional_storage<const NonTrivial_CopyMove> empty_nt_cm {   };
            optional_storage<const NonTrivial_Dtor> val_nt_d        { 1 };
            optional_storage<const NonTrivial_Dtor> empty_nt_d      {   };

            REQUIRE(val_primitive.has_value()   == true);
            REQUIRE(val_primitive.get_value()   == 1);
            REQUIRE(empty_primitive.has_value() == false);

            REQUIRE(val_trivial.has_value()   == true);
            REQUIRE(val_trivial.get_value().i == 1);
            REQUIRE(empty_trivial.has_value() == false);

            REQUIRE(val_nt_c.has_value()   == true);
            REQUIRE(val_nt_c.get_value().i == 1);
            REQUIRE(empty_nt_c.has_value() == false);

            REQUIRE(val_nt_m.has_value()   == true);
            REQUIRE(val_nt_m.get_value().i == 1);
            REQUIRE(empty_nt_m.has_value() == false);

            REQUIRE(val_nt_cm.has_value()   == true);
            REQUIRE(val_nt_cm.get_value().i == 1);
            REQUIRE(empty_nt_cm.has_value() == false);

            REQUIRE(val_nt_d.has_value()   == true);
            REQUIRE(val_nt_d.get_value().i == 1);
            REQUIRE(empty_nt_d.has_value() == false);

            SECTION("and maintain const-ness") {
                // Use `auto&` to maintain mid-level cv qualifiers
                auto& primitive_v = val_primitive.get_value();
                auto& trivial_v   = val_trivial.get_value();
                auto& nt_c_v      = val_nt_c.get_value();
                auto& nt_m_v      = val_nt_m.get_value();
                auto& nt_cm_v     = val_nt_cm.get_value();
                auto& nt_d_v      = val_nt_d.get_value();

                // `remove_reference_t` to check `T[cv]` rather than `T[cv]&`
                REQUIRE(is_const_v<remove_reference_t<decltype(primitive_v)>>);
                REQUIRE(is_const_v<remove_reference_t<decltype(trivial_v  )>>);
                REQUIRE(is_const_v<remove_reference_t<decltype(nt_c_v     )>>);
                REQUIRE(is_const_v<remove_reference_t<decltype(nt_m_v     )>>);
                REQUIRE(is_const_v<remove_reference_t<decltype(nt_cm_v    )>>);
                REQUIRE(is_const_v<remove_reference_t<decltype(nt_d_v     )>>);

            }
        }

        SECTION("should be able to defer construction") {
            optional_storage<const int>                 maybe_primitive { };
            optional_storage<const Trivial>             maybe_trivial   { };
            optional_storage<const NonTrivial_Copy>     maybe_nt_c      { };
            optional_storage<const NonTrivial_Move>     maybe_nt_m      { };
            optional_storage<const NonTrivial_CopyMove> maybe_nt_cm     { };
            optional_storage<const NonTrivial_Dtor>     maybe_nt_d      { };

            REQUIRE(maybe_primitive.has_value() == false);
            REQUIRE(maybe_trivial.has_value()   == false);
            REQUIRE(maybe_nt_c.has_value()      == false);
            REQUIRE(maybe_nt_m.has_value()      == false);
            REQUIRE(maybe_nt_cm.has_value()     == false);
            REQUIRE(maybe_nt_d.has_value()      == false);

            auto& maybe_primitive_val = maybe_primitive.construct_value(1);
            auto& maybe_trivial_val = maybe_trivial.construct_value(1);
            auto& maybe_nt_c_val = maybe_nt_c.construct_value(1);
            auto& maybe_nt_m_val = maybe_nt_m.construct_value(1);
            auto& maybe_nt_cm_val = maybe_nt_cm.construct_value(1);
            auto& maybe_nt_d_val = maybe_nt_d.construct_value(1);

            REQUIRE(maybe_primitive_val == 1);
            REQUIRE(maybe_trivial_val.i == 1);
            REQUIRE(maybe_nt_c_val.i    == 1);
            REQUIRE(maybe_nt_m_val.i    == 1);
            REQUIRE(maybe_nt_cm_val.i   == 1);
            REQUIRE(maybe_nt_d_val.i    == 1);


            REQUIRE(maybe_primitive.has_value() == true);
            REQUIRE(maybe_trivial.has_value()   == true);
            REQUIRE(maybe_nt_c.has_value()      == true);
            REQUIRE(maybe_nt_m.has_value()      == true);
            REQUIRE(maybe_nt_cm.has_value()     == true);
            REQUIRE(maybe_nt_d.has_value()      == true);

            REQUIRE(maybe_primitive.get_value() == 1);
            REQUIRE(maybe_trivial.get_value().i == 1);
            REQUIRE(maybe_nt_c.get_value().i    == 1);
            REQUIRE(maybe_nt_m.get_value().i    == 1);
            REQUIRE(maybe_nt_cm.get_value().i   == 1);
            REQUIRE(maybe_nt_d.get_value().i    == 1);
        }

        SECTION("should be able to construct and then destruct a value") {
            optional_storage<const int>                 maybe_primitive { 1 };
            optional_storage<const Trivial>             maybe_trivial   { 1 };
            optional_storage<const NonTrivial_Copy>     maybe_nt_c      { 1 };
            optional_storage<const NonTrivial_Move>     maybe_nt_m      { 1 };
            optional_storage<const NonTrivial_CopyMove> maybe_nt_cm     { 1 };
            optional_storage<const NonTrivial_Dtor>     maybe_nt_d      { 1 };

            REQUIRE(maybe_primitive.has_value() == true);
            REQUIRE(maybe_trivial.has_value()   == true);
            REQUIRE(maybe_nt_c.has_value()      == true);
            REQUIRE(maybe_nt_m.has_value()      == true);
            REQUIRE(maybe_nt_cm.has_value()     == true);
            REQUIRE(maybe_nt_d.has_value()      == true);

            maybe_primitive.remove_value();
            maybe_trivial.remove_value();
            maybe_nt_c.remove_value();
            maybe_nt_m.remove_value();
            maybe_nt_cm.remove_value();
            maybe_nt_d.remove_value();

            REQUIRE(maybe_primitive.has_value() == false);
            REQUIRE(maybe_trivial.has_value()   == false);
            REQUIRE(maybe_nt_c.has_value()      == false);
            REQUIRE(maybe_nt_m.has_value()      == false);
            REQUIRE(maybe_nt_cm.has_value()     == false);
            REQUIRE(maybe_nt_d.has_value()      == false);
        }
    }
}

} /* namespace optional */
} /* namespace nonstd_test */
