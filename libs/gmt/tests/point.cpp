#include <catch2/catch.hpp>

#include "gmt/point.hpp"
#include "gmt/vector.hpp"

TEST_CASE("pointlike concept", "[gmt]")
{
	STATIC_REQUIRE(gmt::pointlike<gmt::point<double, 2>>);
	STATIC_REQUIRE(gmt::pointlike<gmt::point<int, 2>>);
	STATIC_REQUIRE(gmt::pointlike<gmt::point<double, 3>>);
	STATIC_REQUIRE(gmt::pointlike<gmt::point<char, 2>>);
	STATIC_REQUIRE(gmt::pointlike<gmt::vector<double, 2>>);
	STATIC_REQUIRE(gmt::pointlike<gmt::vector<double, 3>>);
	STATIC_REQUIRE(gmt::pointlike<gmt::vector<double, 2>&>);
	STATIC_REQUIRE(gmt::pointlike<gmt::vector<double, 2>&&>);
	STATIC_REQUIRE(gmt::pointlike<gmt::vector<double, 2> const&>);
}

TEST_CASE("simple point arithmetic", "[gmt]")
{
	using point2d = gmt::point<double, 2>;
	point2d p{ 1., 2. };
	point2d q{ 2., 2. };

	REQUIRE((p + q)*2. == point2d{ 6., 8. });
	REQUIRE((p - q)/2. == point2d{ -0.5, 0. });
	REQUIRE((p * q) == point2d{ 2., 4. });
	REQUIRE((p / q) == point2d{ 0.5, 1. });
	REQUIRE(point2d::all(5.) == point2d{ 5., 5. });

	point2d one = point2d::all(1.);
	REQUIRE(one == point2d{1., 1.});
}
