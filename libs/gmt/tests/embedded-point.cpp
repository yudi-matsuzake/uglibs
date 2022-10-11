#include "catch2/catch_all.hpp"

#include "gmt/embedded-point.hpp"

TEST_CASE("embedded point tests", "[gmt][embedded-point]")
{
	constexpr auto p = gmt::embedded_point<double, 2, 3>{
		{{1., 1.}},
		{ {{0., 0.5, 0.5}}, 0.0 }
	};

	STATIC_REQUIRE(p.p == gmt::point<double, 2>{ 1., 1. });
}
