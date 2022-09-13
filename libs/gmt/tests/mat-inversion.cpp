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

	gmt::detail::resolve(A, b);

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

	gmt::detail::resolve(A, b);

	auto a_is_identity = A == gmt::make_identity_matrix<double, 3>();
	auto b_is_solved = b == gmt::mat{{
		{ 2. },
		{ 3. },
		{ -1. }
	}};

	return a_is_identity && b_is_solved;
}

TEST_CASE("identity matrix", "[mat-inversion]")
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

	STATIC_REQUIRE(gmt::mat{{
		{ 1., 0., 0., 0. },
		{ 0., 1., 0., 0. },
		{ 0., 0., 1., 0. },
		{ 0., 0., 0., 1. }
	}} == gmt::make_identity_matrix<double, 4>());
}

TEST_CASE("system solving", "[mat-inversion]")
{
	STATIC_REQUIRE(test_2x3());
	STATIC_REQUIRE(test_3x4());
}

TEST_CASE("rank", "[mat-inversion]")
{
	{
	constexpr auto m = gmt::mat{{
		{  1.,  0., 1. },
		{ -2., -3., 1. },
		{  3.,  3., 0. }
	}};
	STATIC_REQUIRE(gmt::rank(m) == 2);
	STATIC_REQUIRE_FALSE(gmt::rank(m) == 3);
	STATIC_REQUIRE_FALSE(gmt::rank(m) == 1);
	}

	{
	constexpr auto m = gmt::mat{{
		{  1.,  1., 0.,  2. },
		{ -1., -1., 0., -2. },
	}};
	STATIC_REQUIRE(gmt::rank(m) == 1);
	}

	{
	constexpr auto m = gmt::mat{{
		{ 1., -1. },
		{ 1., -1. },
		{ 0.,  0. },
		{ 2., -2. },
	}};
	STATIC_REQUIRE(gmt::rank(m) == 1);
	}

	{
	constexpr auto m = gmt::mat{{
		{ 1., 2. },
		{ 3., 6. }
	}};
	STATIC_REQUIRE(gmt::rank(m) == 1);
	}

	/*
	 * TODO: make this example work
	 * 	this example do not work because of floating point
	 * 	rounding errors
	 */
	/* { */
	/* constexpr auto m = gmt::mat{{ */
	/* 	{  1.f,  2.f, 1.f }, */
	/* 	{ -2.f, -3.f, 1.f }, */
	/* 	{  3.f,  5.f, 0.f } */
	/* }}; */
	/* STATIC_REQUIRE(gmt::rank(m) == 2); */
	/* } */
}

template<class T, uint64_t N>
static constexpr auto test_for()
{
	auto const inv = gmt::inverse(gmt::make_identity_matrix<T, N>());
	return inv.has_value()
		&& inv.value() == gmt::make_identity_matrix<T, N>();
};

TEST_CASE("inversion", "[mat-inversion]")
{

	SECTION("simple test"){
		constexpr auto m = gmt::mat{{
			{  1.,  2. },
			{ -2., -3. }
		}};

		STATIC_REQUIRE(gmt::inverse(m).value() == gmt::mat{{
			{ -3., -2. },
			{  2.,  1. },
		}});
	}

	SECTION("test for identity matrices"){
		STATIC_REQUIRE(test_for<double, 2>());
		STATIC_REQUIRE(test_for<double, 3>());
		STATIC_REQUIRE(test_for<double, 4>());
	}

	SECTION("test when there is no inverse"){
		STATIC_REQUIRE_FALSE(gmt::inverse(gmt::mat{{
			{ 1., 2., 3. },
			{ 4., 5., 6. },
			{ 7., 8., 9. }
		}}).has_value());
	}

	/*
	 * TODO: make these examples work
	 */
	/* { */
	/* Catch::StringMaker<double>::precision = std::numeric_limits<double>::max_digits10; */
	/* constexpr auto m = gmt::mat{{ */
	/* 	{ 1.,  2.,  3. }, */
	/* 	{ 3.,  2.,  1. }, */
	/* 	{ 2.,  1.,  3. } */
	/* }}; */
	/* REQUIRE(gmt::inverse(m) == gmt::mat{{ */
	/* 	{ -5./12.,   1./4.,  1./3. }, */
	/* 	{ 7./12.,   1./4., -2./3. }, */
	/* 	{ 1./12., -1./4.,  1./3. }, */
	/* }}); */
	/* } */
	/* { */
	/* constexpr auto m = gmt::mat{{ */
	/* 	{ 1.,  4.,  1. }, */
	/* 	{ 2., -1., -2. }, */
	/* 	{ 3., -2.,  0. } */
	/* }}; */
	/* REQUIRE(gmt::inverse(m) == gmt::mat{{ */
	/* 	{ 4./29.,   2./29.,  7./29. }, */
	/* 	{ 6./29.,   3./29., -4./29. }, */
	/* 	{ 1./29., -14./29.,  9./29. }, */
	/* }}); */
	/* } */
}
