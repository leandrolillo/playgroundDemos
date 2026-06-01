#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <HeightMapResource.h>
#include <cstring>

TEST_CASE("PlaygroundDemos Test case")
{
	REQUIRE(1 == 1);

}

TEST_CASE("HeightMapResource::normalAt returns a flat normal at right edge for a flat map")
{
	ImageResource image(3, 3, 24);
	std::memset(image.getData(), 255, image.getBufferSize());

	HeightMapResource heightMap(&image, vector(1, 1, 1));
	vector normal = heightMap.normalAt(heightMap.getWidth(), 1.0);

	REQUIRE(normal.x == Catch::Approx(0.0).margin(1e-6));
	REQUIRE(normal.y == Catch::Approx(-1.0).margin(1e-6));
	REQUIRE(normal.z == Catch::Approx(0.0).margin(1e-6));
}
