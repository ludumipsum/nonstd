/** Selective Special Member Filter Smoke Tests
 *  ===========================================
 */

#include <nonstd/special_member_filters.h>
#include <testrunner/testrunner.h>


namespace nonstd_test {
namespace special_member_filters {

using std::is_copy_assignable_v;
using std::is_copy_constructible_v;
using std::is_move_assignable_v;
using std::is_move_constructible_v;

using nonstd::EnableCopyCtorIf;
using nonstd::EnableCopyAssignIf;
using nonstd::EnableMoveCtorIf;
using nonstd::EnableMoveAssignIf;


struct Trivial
    : private EnableCopyCtorIf<true, Trivial>
    , private EnableCopyAssignIf<true, Trivial>
    , private EnableMoveCtorIf<true, Trivial>
    , private EnableMoveAssignIf<true, Trivial> { };


struct Disable_CopyCtor
    : private EnableCopyCtorIf<false, Disable_CopyCtor>
    , private EnableCopyAssignIf<true, Disable_CopyCtor>
    , private EnableMoveCtorIf<true, Disable_CopyCtor>
    , private EnableMoveAssignIf<true, Disable_CopyCtor> { };


struct Disable_CopyAssign
    : private EnableCopyCtorIf<true, Disable_CopyAssign>
    , private EnableCopyAssignIf<false, Disable_CopyAssign>
    , private EnableMoveCtorIf<true, Disable_CopyAssign>
    , private EnableMoveAssignIf<true, Disable_CopyAssign> { };


struct Disable_CopyCtor_MoveCtor
    : private EnableCopyCtorIf<false, Disable_CopyCtor_MoveCtor>
    , private EnableCopyAssignIf<true, Disable_CopyCtor_MoveCtor>
    , private EnableMoveCtorIf<false, Disable_CopyCtor_MoveCtor>
    , private EnableMoveAssignIf<true, Disable_CopyCtor_MoveCtor> { };


struct Disable_CopyAssign_MoveAssign
    : private EnableCopyCtorIf<true, Disable_CopyAssign_MoveAssign>
    , private EnableCopyAssignIf<false, Disable_CopyAssign_MoveAssign>
    , private EnableMoveCtorIf<true, Disable_CopyAssign_MoveAssign>
    , private EnableMoveAssignIf<false, Disable_CopyAssign_MoveAssign> { };


struct Disable_All
    : private EnableCopyCtorIf<false, Disable_All>
    , private EnableCopyAssignIf<false, Disable_All>
    , private EnableMoveCtorIf<false, Disable_All>
    , private EnableMoveAssignIf<false, Disable_All> { };


TEST_CASE("Special Member Filters", "[nonstd][special_member_filters]") {
SECTION("can disable")
    SECTION("nothing") {
        REQUIRE( is_copy_constructible_v<Trivial>);
        REQUIRE( is_copy_assignable_v<Trivial>);
        REQUIRE( is_move_constructible_v<Trivial>);
        REQUIRE( is_move_assignable_v<Trivial>);
    }

    SECTION("the copy constructor") {
        REQUIRE(!is_copy_constructible_v<Disable_CopyCtor>);
        REQUIRE( is_copy_assignable_v<Disable_CopyCtor>);
        REQUIRE( is_move_constructible_v<Disable_CopyCtor>);
        REQUIRE( is_move_assignable_v<Disable_CopyCtor>);
    }

    SECTION("the copy assignment operator") {
        REQUIRE( is_copy_constructible_v<Disable_CopyAssign>);
        REQUIRE(!is_copy_assignable_v<Disable_CopyAssign>);
        REQUIRE( is_move_constructible_v<Disable_CopyAssign>);
        REQUIRE( is_move_assignable_v<Disable_CopyAssign>);
    }

    SECTION("the move (and copy) constructor") {
        // If only the move ctor is disabled, a copy may be substituted. To
        // fully disable moves, you have to disable copies, too.
        REQUIRE(!is_copy_constructible_v<Disable_CopyCtor_MoveCtor>);
        REQUIRE( is_copy_assignable_v<Disable_CopyCtor_MoveCtor>);
        REQUIRE(!is_move_constructible_v<Disable_CopyCtor_MoveCtor>);
        REQUIRE( is_move_assignable_v<Disable_CopyCtor_MoveCtor>);
    }

    SECTION("the move (and copy) assignment operator") {
        // If only move assignment is disabled, a copy may be substituted. To
        // fully disable moves, you have to disable copies, too.
        REQUIRE( is_copy_constructible_v<Disable_CopyAssign_MoveAssign>);
        REQUIRE(!is_copy_assignable_v<Disable_CopyAssign_MoveAssign>);
        REQUIRE( is_move_constructible_v<Disable_CopyAssign_MoveAssign>);
        REQUIRE(!is_move_assignable_v<Disable_CopyAssign_MoveAssign>);
    }

    SECTION("all special members") {
        REQUIRE(!is_copy_constructible_v<Disable_All>);
        REQUIRE(!is_copy_assignable_v<Disable_All>);
        REQUIRE(!is_move_constructible_v<Disable_All>);
        REQUIRE(!is_move_assignable_v<Disable_All>);
    }

}

} /* namespace special_member_filters */
} /* namespace nonstd_test */
