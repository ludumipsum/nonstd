/** Quantity::Angle Smoke Tests
 *  ===========================
 */

#include <testrunner/testrunner.h>

#include <nonstd/std_ish/cx_math.h>
#include <nonstd/quantity/angle.h>


namespace nonstd_test {
namespace quantity {

TEST_CASE("Quantity Angle API Demo", "[nonstd][api][scop guards]") {

    /** Creating Angles
     *  ---------------
     *  An `angle` is exactly what they describes. Because the `struct angle` is
     *  a POD type that still hides its internal representation, there are a lot
     *  of ways to create and interact with the concept.
     *
     *  In general, if you can't decide which initializer to use, prefer the
     *  one highest up in this section (since they're ordered in terms
     *  of simplicity).
     */
    SECTION("Creating Angles") {
        // Quantities are, for better or worse, pretty heavily namespaced. As
        // such, you'll probably want to declare them in scope.
        using nonstd::quantity::angle;
        // To make creation easy, a number of user-defined literals are provided
        // in the `nonstd::literals::angle_literals` scope. You'll have to pull
        // them in with a `using namespace` directive.
        using namespace nonstd::literals::angle_literals;

        // As suggested above, the easiest way to create a `angle`s is with our
        // user-defined literals.
        // Note: `360 / 7 ~=~ 51.4`.
        auto radian_semicircle  = 3.14_radians;
        auto degree_semicircle  = 180_degrees;
        auto degree_septicircle = 51.4_degrees;

        // If you're lazy, you can use shorthands.
        auto rad_semi  = 3.14_rads;
        auto deg_semi  = 180_degs;
        auto deg_septi = 51.4_degs;

        // If you need to be specific (of if you don't want to use literals) you
        // can used angle's "name constructor" static functions.
        auto named_radian_semicircle  = angle::in_radians(3.14);
        auto named_degree_semicircle  = angle::in_degrees(180);
        auto named_degree_septicircle = angle::in_degrees(51.4);
        auto n_rad_semi               = angle::in_rads(3.14);
        auto n_deg_semi               = angle::in_degs(180);
        auto n_deg_septi              = angle::in_degs(51.4);

        // `angle`s are trivially constructible, so you can instantiate
        // uninitialized `angle`s, and assign to them later.
        angle some_angles[6]; // No ctor calls.
        // Time passes...
        some_angles[0].set_radians(3.14);
        some_angles[1].set_degrees(180);
        some_angles[2].set_degrees(51.4);
        some_angles[3].rads(3.14);
        some_angles[4].degs(180);
        some_angles[5].degs(51.4);

        // A note about negating user-defined literals:
        // The negative in `-1.0_rads` is _not_ parsed as part of the user-
        // defined literal. It is applied as a unary `-` operator on the created
        // `angle` _after_ creation.
        // That said, both the `+` and `-` operators can be applied to `angle`s
        // both during and after creation.
        auto unary_1 = +180_degs;  // ==  180 degrees
        auto unary_2 = -180_degs;  // == -180 degrees
        auto unary_3 = -unary_1;   // == -180 degrees
        auto unary_4 = -unary_2;   // ==  180 degrees
        auto unary_5 = +unary_3;   // == -180 degrees [sic; negative]
    }

    /** Observing Angles
     *  ----------------
     *  Getting data out of an angle is... Pretty easy. There's not much to say.
     */
    SECTION("Observing Angles") {
        using nonstd::quantity::angle;
        using namespace nonstd::literals::angle_literals;

        auto half  = angle::in_degs(180);
        auto whole = angle::in_degs(360);

        // Note: `.rads()` is shorthand for (calls into) `.radians()`, and
        // `.degs()` is shorthand for `.degrees()`. I'm only demoing the
        // shorthand here for brevity's sake, and because calling the shorthand
        // will give us coverage of the longform.
        f32 rads_half  = half.rads();  REQUIRE(rads_half  == angle::pi().rads());
        f32 degs_half  = half.degs();  REQUIRE(degs_half  == 180.f);
        f32 rads_whole = whole.rads(); REQUIRE(rads_whole == angle::tau().rads());
        f32 degs_whole = whole.degs(); REQUIRE(degs_whole == 360.f);

        // If you have an `angle` less than 0.0 or greater than `angle::tau`
        // you can access an `angle`'s value normalized to [0.0,tau].
        // Note: We use a simulacra of "roughly equals" because of floating
        // point precision woes.
        auto less_than_zero = angle::in_rads(-angle::pi_radians);
        auto more_than_tau  = angle::in_rads(3 * angle::pi_radians);
        REQUIRE(less_than_zero.normalized_radians() < angle::pi().rads() + 0.01);
        REQUIRE(less_than_zero.normalized_radians() > angle::pi().rads() - 0.01);
        REQUIRE(more_than_tau.normalized_radians()  < angle::pi().rads() + 0.01);
        REQUIRE(more_than_tau.normalized_radians()  > angle::pi().rads() - 0.01);
        REQUIRE(less_than_zero.normalized_degrees() < angle::pi().degs() + 0.01);
        REQUIRE(less_than_zero.normalized_degrees() > angle::pi().degs() - 0.01);
        REQUIRE(more_than_tau.normalized_degrees()  < angle::pi().degs() + 0.01);
        REQUIRE(more_than_tau.normalized_degrees()  > angle::pi().degs() - 0.01);

        // There are also shorthand for normalized values.
        REQUIRE(less_than_zero.rads_norm() < angle::pi().rads() + 0.01);
        REQUIRE(less_than_zero.rads_norm() > angle::pi().rads() - 0.01);
        REQUIRE(more_than_tau.rads_norm()  < angle::pi().rads() + 0.01);
        REQUIRE(more_than_tau.rads_norm()  > angle::pi().rads() - 0.01);
        REQUIRE(less_than_zero.degs_norm() < angle::pi().degs() + 0.01);
        REQUIRE(less_than_zero.degs_norm() > angle::pi().degs() - 0.01);
        REQUIRE(more_than_tau.degs_norm()  < angle::pi().degs() + 0.01);
        REQUIRE(more_than_tau.degs_norm()  > angle::pi().degs() - 0.01);

        // You can also create a new angle from the noramalized value.
        auto less_than_zero_normalized = less_than_zero.normalized();
        auto more_than_tau_normalized  = more_than_tau.normalized();
        REQUIRE(less_than_zero_normalized.rads_norm() < angle::pi().rads() + 0.01);
        REQUIRE(less_than_zero_normalized.rads_norm() > angle::pi().rads() - 0.01);
        REQUIRE(more_than_tau_normalized.rads_norm()  < angle::pi().rads() + 0.01);
        REQUIRE(more_than_tau_normalized.rads_norm()  > angle::pi().rads() - 0.01);

        // Now that we know we can compare angles, let's make sure unary
        // operators operate as advertised.
        auto unary_1 = +180_degs;  // ==  180 degrees
        auto unary_2 = -180_degs;  // == -180 degrees
        auto unary_3 = -unary_1;   // == -180 degrees
        auto unary_4 = -unary_2;   // ==  180 degrees
        auto unary_5 = +unary_3;   // == -180 degrees [sic; negative]
        REQUIRE(unary_1.degs() ==  180.f);
        REQUIRE(unary_2.degs() == -180.f);
        REQUIRE(unary_3.degs() == -180.f);
        REQUIRE(unary_4.degs() ==  180.f);
        REQUIRE(unary_5.degs() == -180.f);
    }

    /** Comparing Angles
     *  ----------------
     *  You can directly compare an `angle` with another `angle`. Think of it
     *  like comparing two floating point values.
     *  But really. Do exactly that.
     *  Because that's exactly what's happening.
     */
    SECTION("Comparing Angles") {
        using nonstd::quantity::angle;
        using namespace nonstd::literals::angle_literals;

        auto smaller = 1.0_rads;
        auto bigger  = 1.1_rads;

        // The spaceship operator is correctly implemented.
        REQUIRE(smaller == smaller);
        REQUIRE(smaller != bigger);
        REQUIRE(smaller <  bigger);
        REQUIRE(bigger  >  smaller);
        REQUIRE(smaller <= bigger);
        REQUIRE(bigger  >= smaller);

        // You can also use temporaries created with user-defined literals in
        // comparison operations.
        REQUIRE(smaller == 1.0_rads);
        REQUIRE(smaller != 0.0_rads);
        REQUIRE(smaller <  1.1_rads);
        REQUIRE(smaller >  0.9_rads);
        REQUIRE(smaller <= 1.0_rads);
        REQUIRE(smaller >= 1.0_rads);

        // Because angles are floating point, they inherit some of the strange
        // behavior of direct comparisons via `==`...
        f32 a_f = -3.14;
        REQUIRE_FALSE(a_f+6.28 == 3.14);
        angle a_a = -3.14_rads;
        REQUIRE_FALSE(a_a.normalized() == 3.14_rads);
        // ... to get around this, we offer a `.nearly_equal` helper that
        // compares angles to within a given epsilon.
        REQUIRE(a_a.normalized().nearly_equal(3.14_rads));
        // The epsilon is configurable as a angle.
        REQUIRE((3.14_rads).nearly_equal(3.13_rads, 0.02_rads));
    }

    /** Modifying Angles
     *  ----------------
     *  Modifying angles is pretty straight forward. Either you compose angles,
     *  or you compose angles and a scalar.
     */
    SECTION("Modifying Angles") {
        using nonstd::quantity::angle;
        using namespace nonstd::literals::angle_literals;

        // You can compose angels with addition and subtraction.
        auto one_rad = 0.5_rads + 0.5_rads;
        REQUIRE(one_rad.nearly_equal(1.0_rads));
        auto two_rads = 3.0_rads - 1.0_rads;
        REQUIRE(two_rads.nearly_equal(2.0_rads));

        // You can compose an angle with a scalar.
        auto three_rads = 2 * 1.5_rads;
        REQUIRE(three_rads.nearly_equal(3.0_rads));
        auto four_rads = 8.0_rads / 2;
        REQUIRE(four_rads.nearly_equal(4.0_rads));

        // You can modify an angles in place, in the same fashion.
        auto a = 1.0_rads;
        a += 1.0_rads;
        REQUIRE(a.nearly_equal(2.0_rads));
        a -= 1.5_rads;
        REQUIRE(a.nearly_equal(0.5_rads));
        a *= 2;
        REQUIRE(a.nearly_equal(1.0_rads));
        a /= 4;
        REQUIRE(a.nearly_equal(.25_rads));
    }
}

} /* namespace quantity */
} /* namespace nonstd_test */
