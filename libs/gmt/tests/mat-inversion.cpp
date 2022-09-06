#include "catch2/catch.hpp"

#include "gmt/mat-inversion.hpp"

TEST_CASE("system solving", "[mat-inversion]")
{
	{
	constexpr auto A = gmt::mat{{
		{1., 2.},
		{4., 5.}
	}};

	constexpr auto B = gmt::mat{{
		{3.},
		{6.}
	}};

	static constexpr auto x = gmt::resolve(A, B);
	STATIC_REQUIRE(x == gmt::mat{{
		{-1.},
		{2.}
	}});
	}

	{
	constexpr auto A = gmt::mat{{
		{ 2.f,  1.f, -1.f},
		{-3.f, -1.f,  2.f},
		{-2.f,  1.f,  2.f}
	}};

	constexpr auto B = gmt::mat{{
		{  8.f},
		{-11.f},
		{- 3.f}
	}};

	static constexpr auto x = gmt::resolve(A, B);
	STATIC_REQUIRE(x[0][0] == 2.f);
	STATIC_REQUIRE(x[1][0] == 2.99999976f);
	STATIC_REQUIRE(x[2][0] == -0.99999994f);
	}
}
