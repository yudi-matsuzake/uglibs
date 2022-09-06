#include "catch2/catch.hpp"

#include "gmt/mat-inversion.hpp"

TEST_CASE("system solving", "[mat-inversion]")
{
	STATIC_REQUIRE(gmt::mat{{
		{ 1., 0. },
		{ 0., 1. }
	}} == gmt::make_identity_matrix<double, 2>());

	STATIC_REQUIRE(gmt::mat{{
		{ 1., 0., 0. },
		{ 0., 1., 0. },
		{ 0., 0., 1. }
	}} == gmt::make_identity_matrix<double, 3>());

	SECTION("2x3"){
		auto A = gmt::mat{{
			{1., 2.},
			{4., 5.}
		}};

		auto b = gmt::mat{{
			{3.},
			{6.}
		}};

		auto x = gmt::resolve(A, b);
		REQUIRE(x == gmt::mat{{
			{-1.},
			{2.}
		}});

		REQUIRE(A == gmt::make_identity_matrix<double, 2>());
	}

	SECTION("3x4"){
		auto A = gmt::mat{{
			{ 2.,  1., -1.},
			{-3., -1.,  2.},
			{-2.,  1.,  2.}
		}};

		auto B = gmt::mat{{
			{  8.},
			{-11.},
			{- 3.}
		}};

		gmt::resolve(A, B);
		REQUIRE(A == gmt::make_identity_matrix<double, 3>());
		INFO("B[0] = " << B[0][0]);
		INFO("B[1] = " << B[1][0]);
		INFO("B[2] = " << B[2][0]);
		REQUIRE(B[0][0] == 2.);
		REQUIRE(B[1][0] == 3.);
		REQUIRE(B[2][0] == -0.99999999999999989);
	}
}
