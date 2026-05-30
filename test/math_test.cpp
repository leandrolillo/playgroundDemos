#include <catch2/catch_test_macros.hpp>
#include <Math3d.h>

TEST_CASE("Math3d compile test") {
    vector3 v(1.0f, 2.0f, 3.0f);
    REQUIRE(v.x == 1.0f);
}
