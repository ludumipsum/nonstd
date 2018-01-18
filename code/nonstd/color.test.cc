/** Color Smoke Tests
 *  =================
 *  Colors. How they do and how to make 'em.
 */

#include <nonstd/color.h>
#include <testrunner/testrunner.h>


namespace nonstd_test {
namespace color {

using nonstd::RGBAu;
using nonstd::RGBAf;
using nonstd::HSVA;
using nonstd::angle;
using namespace nonstd::literals::angle_literals;


TEST_CASE("Color API Demos", "[nonstd][color]") {
    SECTION("There are three different kinds of colors") {
        RGBAu unsigned_rgba       { 255, 255, 255, 255 };
        RGBAf floating_point_rgba { 1.0, 1.0, 1.0, 1.0 };
        HSVA  angular_hsva        { 360_degs, 1.0, 1.0, 1.0 };

        REQUIRE(sizeof(unsigned_rgba)       == 4);
        REQUIRE(sizeof(floating_point_rgba) == 16);
        REQUIRE(sizeof(angular_hsva)        == 16);
    }

    SECTION("Constructing `RGBAu`s") {
        // Create a soft gray
        auto a = RGBAu { 178 };
        // Create a dark gray w/ an alpha
        auto b = RGBAu { 48, 200 };
        // You can also use a floating point for the alpha
        auto c = RGBAu { 48, 0.5 };
        // Fully specify all color, optionally alpha, optionally as a float
        auto d = RGBAu { 128, 128, 128 };
        auto e = RGBAu { 128, 128, 128, 64   };
        auto f = RGBAu { 128, 128, 128, 0.25 };
        // If you need to, you can build an RGBu from floats (alpha is optional)
        auto g = RGBAu { 0.75, 0.50, 0.25 };
        auto h = RGBAu { 0.75, 0.50, 0.25, 1.0 };
    }

    SECTION("Constructing `RGBAf`s") {
        // Create a soft gray
        auto a = RGBAf { .68 };
        // Create a dark gray w/ an alpha
        auto b = RGBAf { .22, .84 };
        // Fully specify all color, optionally alpha
        auto c = RGBAf { 0.75, 0.50, 0.25 };
        auto d = RGBAf { 0.75, 0.50, 0.25, 1.0 };
        // If you need to, you can build an RGBu from integers
        // Alpha is optional, and can be either an int or a float in this form
        auto e = RGBAf { 128, 128, 128, 64   };
        auto f = RGBAf { 128, 128, 128, 0.25 };
    }

    SECTION("Constructing `HSVA`s") {
        // Create a soft gray
        auto a = HSVA { .68 };
        // Create a dark gray w/ an alpha
        auto b = HSVA { .22, .84 };
        // Create a fully saturated, max value color at the given angle
        auto c = HSVA { 5.16_rads };
        // Stack saturation, value, and alpha on the above
        auto d = HSVA { 5.16_rads, .45 };
        auto e = HSVA { 5.16_rads, .45, .92 };
        auto f = HSVA { 5.16_rads, .45, .92, .22 };
    }

    SECTION("The different color types are interchangeable") {
        // Note that the conversaion to and from HSVA is an expensive one
        auto  u = RGBAu { 255, 255, 255 };

        RGBAf f = u;
        REQUIRE(f.r == 1.0); REQUIRE(f.g == 1.0); REQUIRE(f.b == 1.0);

        HSVA  h = u;
        REQUIRE(h.h == 0_degs); REQUIRE(h.s == 0.0); REQUIRE(h.v == 1.0);
    }

    SECTION("Colors correctly promote types") {
        u8  r = 42;
        u16 g = 199;
        i32 b = 96;
        i16 a = 255;
        auto rgb_color = RGBAu { r, g, b, a };

        auto rgb_colorf = RGBAf { 0.40f, 0.99, 1.0l };
    }
}

} /* namespace color */
} /* namespace nonstd_test */
