/** Optional Storage Smoke Tests
 *  ============================
 *  GOAL: Validate the basic functionality of the `optional_storage<T>` class,
 *  and guarantee correct constexpr / trivial propagation for special member
 *  functions (notably dtor, copy ctor, and move ctor).
 */

#include <nonstd/utility/optional_storage.h>
#include <nonstd/core/primitive_types.h>

#include <type_traits>
#include <nonstd/cpp1z/type_traits_ext.h>

#include <testrunner/testrunner.h>


namespace nonstd_test {
namespace optional_storage {

using nonstd::optional_storage;

using std::is_trivially_copy_constructible_v;
using std::is_trivially_copy_constructible_v;
using std::is_trivially_destructible_v;
using std::is_trivially_move_constructible_v;
using std::is_trivially_move_constructible_v;


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
}

} /* namespace optional */
} /* namespace nonstd_test */
