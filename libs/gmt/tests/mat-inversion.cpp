#include "catch2/catch.hpp"

#include "gmt/mat-inversion.hpp"

TEST_CASE("matrix inversion", "[mat-inversion]")
{
	constexpr auto a = gmt::mat{
		std::array{1., 2.},
		std::array{3., 4.}
	};

	STATIC_REQUIRE(a == gmt::mat{
		std::array{1., 2.},
		std::array{3., 4.}
	});

	STATIC_REQUIRE_FALSE(a == gmt::mat{
		std::array{4., 3.},
		std::array{2., 1.}
	});

	STATIC_REQUIRE(a == gmt::mat{{{1.,2.}, {3., 4.}}});
}
