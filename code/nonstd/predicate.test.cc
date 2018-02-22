/** Composable Predicate Object Tests
 *  =================================
 */
#include <nonstd/predicate.h>
#include <testrunner/testrunner.h>


namespace nonstd_test::predicate {

using nonstd::predicate_t;

struct is_less_than_t {
    int max; // Set via aggregate init.
    bool operator() (int i) const { return i < max; }
};

struct is_equal_to_t {
    int target;
    bool operator() (int i) const { return i == target; }
};

struct is_greater_than_t {
    int min;
    bool operator() (int i) const { return i > min; }
};

bool greater_than_10(int i) { return i > 10; }


TEST_CASE("Predicate API Demo", "[nonstd][predicate][api]") {

    SECTION("Creating Predicates") {
        // Predicates can be created using simple lambdas,
        predicate_t<int> is_eq_3  { [](int i) { return i == 3; } };
        // ... instances of function objects,
        predicate_t<int> is_lt_5  { is_less_than_t{5} };
        // ... or even simple functions.
        predicate_t<int> is_gt_10 { greater_than_10 };

        REQUIRE(is_eq_3(3));   REQUIRE_FALSE(is_eq_3(-3));
        REQUIRE(is_lt_5(4));   REQUIRE_FALSE(is_lt_5(5));
        REQUIRE(is_gt_10(11)); REQUIRE_FALSE(is_gt_10(10));
    }

    SECTION("Composing Predicates") {
        // From a set of individual predicates,
        predicate_t<int> is_gt_3 { [](int i) { return i > 3; } };
        predicate_t<int> is_lt_5 { [](int i) { return i < 5; } };

        // We can compose more;
        auto is_eq_4 = is_gt_3 && is_lt_5;

        REQUIRE(is_eq_4(4)); REQUIRE_FALSE(is_eq_4(-4));
    }

    SECTION("Composing Composed Predicates") {
        predicate_t<int> eq_4 { is_equal_to_t{4}     };
        predicate_t<int> gt_2 { is_greater_than_t{2} };
        predicate_t<int> lt_2 { is_less_than_t{2}    };
        predicate_t<int> gt_6 { is_greater_than_t{6} };
        predicate_t<int> lt_6 { is_less_than_t{6}    };

        REQUIRE(eq_4(4)); REQUIRE_FALSE(eq_4(3)); REQUIRE_FALSE(eq_4(5));
        REQUIRE(gt_2(3)); REQUIRE_FALSE(gt_2(2)); REQUIRE_FALSE(gt_2(1));
        REQUIRE(lt_2(1)); REQUIRE_FALSE(lt_2(2)); REQUIRE_FALSE(lt_2(3));
        REQUIRE(gt_6(7)); REQUIRE_FALSE(gt_6(6)); REQUIRE_FALSE(gt_6(5));
        REQUIRE(lt_6(5)); REQUIRE_FALSE(lt_6(6)); REQUIRE_FALSE(lt_6(7));

        auto not_4               = !eq_4;
        auto gt_2_but_not_4      = gt_2 && not_4;
        auto gt_6_or_4           = gt_6 || eq_4;
        auto between_6_and_2     = gt_6 == lt_2;
        auto not_between_6_and_2 = gt_6 != lt_2;

        REQUIRE(not_4(3)); REQUIRE_FALSE(not_4(4)); REQUIRE(not_4(5));
        REQUIRE_FALSE(gt_2_but_not_4(1)); REQUIRE(gt_2_but_not_4(3));
            REQUIRE_FALSE(gt_2_but_not_4(4)); REQUIRE(gt_2_but_not_4(5));
        REQUIRE(gt_6_or_4(4)); REQUIRE(gt_6_or_4(7));
            REQUIRE_FALSE(gt_6_or_4(5)); REQUIRE_FALSE(gt_6_or_4(6));
        REQUIRE_FALSE(between_6_and_2(1)); REQUIRE(between_6_and_2(2));
            REQUIRE(between_6_and_2(4)); REQUIRE(between_6_and_2(6));
            REQUIRE_FALSE(between_6_and_2(7));
        REQUIRE(not_between_6_and_2(1)); REQUIRE_FALSE(not_between_6_and_2(2));
            REQUIRE_FALSE(not_between_6_and_2(4));
            REQUIRE_FALSE(not_between_6_and_2(6));
            REQUIRE(not_between_6_and_2(7));

        auto between_6_and_2_but_not_4 = not_4 && between_6_and_2;
        REQUIRE_FALSE(between_6_and_2_but_not_4(1));
            REQUIRE(between_6_and_2_but_not_4(2));
            REQUIRE_FALSE(between_6_and_2_but_not_4(4));
            REQUIRE(between_6_and_2_but_not_4(6));
            REQUIRE_FALSE(between_6_and_2_but_not_4(7));

        auto gt_1 = between_6_and_2_but_not_4 || gt_2;
        REQUIRE_FALSE(gt_1(0)); REQUIRE_FALSE(gt_1(1));
            REQUIRE(gt_1(2));REQUIRE(gt_1(4)); REQUIRE(gt_1(6));
            REQUIRE(gt_1(7));
    }
}

} /* namespace nonstd_test::predicate */
