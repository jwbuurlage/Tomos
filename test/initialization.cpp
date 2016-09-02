#include "catch.hpp"
#include "tomo.hpp"

using T = float;

TEST_CASE("We can create reconstruction volumes", "[core]") {
    SECTION("2D") {
        int k = 16;
        tomo::volume<2_D> v(k, k);
        REQUIRE(v.cells() == k * k);
    }

    SECTION("3D") {
        int k = 16;
        tomo::volume<3_D> v(k, k, k);
        REQUIRE(v.cells() == k * k * k);
    }
}

TEST_CASE("We can initialize geometry", "[core]") {
    SECTION("2D") {
        int k = 16;
        auto v = tomo::volume<2_D>(k, k);
        auto g = tomo::parallel_geometry<2_D, T>(180, 250, v);
        CHECK(g.lines() == 180 * 250);

        int i = 0;
        for (auto line : g)
            ++i;
        CHECK(i == g.lines());
    }

    SECTION("3D") {
        int k = 16;
        auto v = tomo::volume<3_D>(k, k, k);
        auto g = tomo::parallel_geometry<3_D, T>(180, 250, v);
        CHECK(g.lines() == 180 * 250 * 250);

        int i = 0;
        for (auto line : g)
            ++i;
        CHECK(i == g.lines());
    }
}

TEST_CASE("We can use projectors", "[core]") {
    // FIXME add test case where we check that for each line in any geometry,
    // the origin lies inside the imaging volume
}
