#include "catch2/catch.hpp"

#include "gmt/mat-inversion.hpp"

static constexpr auto test_2x3()
{
	auto A = gmt::mat{{
		{1., 2.},
		{4., 5.}
	}};

	auto b = gmt::mat{{
		{3.},
		{6.}
	}};

	gmt::resolve(A, b);

	auto a_is_identity = A == gmt::make_identity_matrix<double, 2>();
	auto b_is_solved = b == gmt::mat{{
		{-1.},
		{2.}
	}};

	return a_is_identity && b_is_solved;
}

static constexpr auto test_3x4()
{
	auto A = gmt::mat{{
		{ 2.,  1., -1.},
		{-3., -1.,  2.},
		{-2.,  1.,  2.}
	}};

	auto b = gmt::mat{{
		{  8.},
		{-11.},
		{- 3.}
	}};

	gmt::resolve(A, b);

	auto a_is_identity = A == gmt::make_identity_matrix<double, 3>();
	auto b_is_solved = b == gmt::mat{{
		{ 2. },
		{ 3. },
		{ -0.99999999999999989 }
	}};

	return a_is_identity && b_is_solved;
}

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

	STATIC_REQUIRE(test_2x3());
	STATIC_REQUIRE(test_3x4());

}
