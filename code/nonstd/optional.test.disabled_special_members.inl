// Possibly duplicate includes from inlined code
#include <nonstd/optional.h>
#include <thirdparty/catch.h>

#include <nonstd/nonstd.h>
#include <nonstd/type_traits_ext.h>

namespace disabled_special_members {

using nonstd::optional;

using std::is_copy_assignable_v;
using std::is_copy_constructible_v;
using std::is_move_assignable_v;
using std::is_move_constructible_v;



struct Trivial {
    int i;

    constexpr Trivial(int _i) : i ( _i ) { }

    Trivial() = default;
    Trivial(Trivial const & rhs) = default;
    Trivial(Trivial && rhs) = default;
    Trivial& operator= (Trivial const & rhs) = default;
    Trivial& operator= (Trivial && rhs) = default;
};
ENFORCE( is_copy_assignable_v<Trivial>);
ENFORCE( is_copy_constructible_v<Trivial>);
ENFORCE( is_move_assignable_v<Trivial>);
ENFORCE( is_move_constructible_v<Trivial>);


struct Has_CopyCtor {
    int i;

    constexpr Has_CopyCtor(int _i) : i ( _i ) { }

    Has_CopyCtor() = default;
    Has_CopyCtor(Has_CopyCtor const & rhs) = default;
    Has_CopyCtor(Has_CopyCtor && rhs) = delete;
    Has_CopyCtor& operator= (Has_CopyCtor const & rhs) = delete;
    Has_CopyCtor& operator= (Has_CopyCtor && rhs) = delete;
};
ENFORCE(!is_copy_assignable_v<Has_CopyCtor>);
ENFORCE( is_copy_constructible_v<Has_CopyCtor>);
ENFORCE(!is_move_assignable_v<Has_CopyCtor>);
ENFORCE(!is_move_constructible_v<Has_CopyCtor>);


struct Has_MoveCtor {
    int i;

    constexpr Has_MoveCtor(int _i) : i ( _i ) { }

    Has_MoveCtor() = default;
    Has_MoveCtor(Has_MoveCtor const & rhs) = delete;
    Has_MoveCtor(Has_MoveCtor && rhs) = default;
    Has_MoveCtor& operator= (Has_MoveCtor const & rhs) = delete;
    Has_MoveCtor& operator= (Has_MoveCtor && rhs) = delete;
};
ENFORCE(!is_copy_assignable_v<Has_MoveCtor>);
ENFORCE(!is_copy_constructible_v<Has_MoveCtor>);
ENFORCE(!is_move_assignable_v<Has_MoveCtor>);
ENFORCE( is_move_constructible_v<Has_MoveCtor>);


struct Has_CopyAssign {
    int i;

    constexpr Has_CopyAssign(int _i) : i ( _i ) { }

    Has_CopyAssign() = default;
    Has_CopyAssign(Has_CopyAssign const & rhs) = delete;
    Has_CopyAssign(Has_CopyAssign && rhs) = delete;
    Has_CopyAssign& operator= (Has_CopyAssign const & rhs) = default;
    Has_CopyAssign& operator= (Has_CopyAssign && rhs) = delete;
};
ENFORCE( is_copy_assignable_v<Has_CopyAssign>);
ENFORCE(!is_copy_constructible_v<Has_CopyAssign>);
ENFORCE(!is_move_assignable_v<Has_CopyAssign>);
ENFORCE(!is_move_constructible_v<Has_CopyAssign>);


struct Has_CopyCtor_CopyAssign {
    int i;

    constexpr Has_CopyCtor_CopyAssign(int _i) : i ( _i ) { }

    Has_CopyCtor_CopyAssign() = default;
    Has_CopyCtor_CopyAssign(Has_CopyCtor_CopyAssign const & rhs) = default;
    Has_CopyCtor_CopyAssign(Has_CopyCtor_CopyAssign && rhs) = delete;
    Has_CopyCtor_CopyAssign& operator= (Has_CopyCtor_CopyAssign const & rhs) = default;
    Has_CopyCtor_CopyAssign& operator= (Has_CopyCtor_CopyAssign && rhs) = delete;
};
ENFORCE( is_copy_assignable_v<Has_CopyCtor_CopyAssign>);
ENFORCE( is_copy_constructible_v<Has_CopyCtor_CopyAssign>);
ENFORCE(!is_move_assignable_v<Has_CopyCtor_CopyAssign>);
ENFORCE(!is_move_constructible_v<Has_CopyCtor_CopyAssign>);


struct Has_MoveAssign {
    int i;

    constexpr Has_MoveAssign(int _i) : i ( _i ) { }

    Has_MoveAssign() = default;
    Has_MoveAssign(Has_MoveAssign const & rhs) = delete;
    Has_MoveAssign(Has_MoveAssign && rhs) = delete;
    Has_MoveAssign& operator= (Has_MoveAssign const & rhs) = delete;
    Has_MoveAssign& operator= (Has_MoveAssign && rhs) = default;
};
ENFORCE(!is_copy_assignable_v<Has_MoveAssign>);
ENFORCE(!is_copy_constructible_v<Has_MoveAssign>);
ENFORCE( is_move_assignable_v<Has_MoveAssign>);
ENFORCE(!is_move_constructible_v<Has_MoveAssign>);


struct Has_MoveCtor_MoveAssign {
    int i;

    constexpr Has_MoveCtor_MoveAssign(int _i) : i ( _i ) { }

    Has_MoveCtor_MoveAssign() = default;
    Has_MoveCtor_MoveAssign(Has_MoveCtor_MoveAssign const & rhs) = delete;
    Has_MoveCtor_MoveAssign(Has_MoveCtor_MoveAssign && rhs) = default;
    Has_MoveCtor_MoveAssign& operator= (Has_MoveCtor_MoveAssign const & rhs) = delete;
    Has_MoveCtor_MoveAssign& operator= (Has_MoveCtor_MoveAssign && rhs) = default;
};
ENFORCE(!is_copy_assignable_v<Has_MoveCtor_MoveAssign>);
ENFORCE(!is_copy_constructible_v<Has_MoveCtor_MoveAssign>);
ENFORCE( is_move_assignable_v<Has_MoveCtor_MoveAssign>);
ENFORCE( is_move_constructible_v<Has_MoveCtor_MoveAssign>);



TEST_CASE("Optionals Types", "[nonstd][optional]") {
SECTION("that wrap types that are")
    SECTION("fully trivial") {
        // [sic] a valid copy operation maybe use used in place of a move.
        REQUIRE( is_copy_assignable_v<optional<Trivial>>);
        REQUIRE( is_copy_constructible_v<optional<Trivial>>);
        REQUIRE( is_move_assignable_v<optional<Trivial>>);
        REQUIRE( is_move_constructible_v<optional<Trivial>>);
    }

    SECTION("only copy constructible") {
        // [sic] a valid copy operation maybe use used in place of a move.
        REQUIRE(!is_copy_assignable_v<optional<Has_CopyCtor>>);
        REQUIRE( is_copy_constructible_v<optional<Has_CopyCtor>>);
        REQUIRE(!is_move_assignable_v<optional<Has_CopyCtor>>);
        REQUIRE( is_move_constructible_v<optional<Has_CopyCtor>>);
    }

    SECTION("only move constructible") {
        REQUIRE(!is_copy_assignable_v<optional<Has_MoveCtor>>);
        REQUIRE(!is_copy_constructible_v<optional<Has_MoveCtor>>);
        REQUIRE(!is_move_assignable_v<optional<Has_MoveCtor>>);
        REQUIRE( is_move_constructible_v<optional<Has_MoveCtor>>);

    }

    SECTION("only copy assignable") {
        // [sic] optionals can only assign if the relevant constructor exists.
        REQUIRE(!is_copy_assignable_v<optional<Has_CopyAssign>>);
        REQUIRE(!is_copy_constructible_v<optional<Has_CopyAssign>>);
        REQUIRE(!is_move_assignable_v<optional<Has_CopyAssign>>);
        REQUIRE(!is_move_constructible_v<optional<Has_CopyAssign>>);
    }

    SECTION("copy constructible and copy assignable") {
        // [sic] a valid copy operation maybe use used in place of a move.
        REQUIRE( is_copy_assignable_v<optional<Has_CopyCtor_CopyAssign>>);
        REQUIRE( is_copy_constructible_v<optional<Has_CopyCtor_CopyAssign>>);
        REQUIRE( is_move_assignable_v<optional<Has_CopyCtor_CopyAssign>>);
        REQUIRE( is_move_constructible_v<optional<Has_CopyCtor_CopyAssign>>);
    }

    SECTION("only move assignable") {
        // [sic] optionals can only assign if the relevant constructor exists.
        REQUIRE(!is_copy_assignable_v<optional<Has_MoveAssign>>);
        REQUIRE(!is_copy_constructible_v<optional<Has_MoveAssign>>);
        REQUIRE(!is_move_assignable_v<optional<Has_MoveAssign>>);
        REQUIRE(!is_move_constructible_v<optional<Has_MoveAssign>>);
    }

    SECTION("move constructible and move assignable") {
        REQUIRE(!is_copy_assignable_v<optional<Has_MoveCtor_MoveAssign>>);
        REQUIRE(!is_copy_constructible_v<optional<Has_MoveCtor_MoveAssign>>);
        REQUIRE( is_move_assignable_v<optional<Has_MoveCtor_MoveAssign>>);
        REQUIRE( is_move_constructible_v<optional<Has_MoveCtor_MoveAssign>>);
    }
}

} /* namespace disabled_special_members */
