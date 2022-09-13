#include "catch2/catch.hpp"

#include "gmt/mat.hpp"
#include "gmt/mat-inversion.hpp"

static constexpr auto n_rows = 5UL;
static constexpr auto n_cols = 10UL;

static constexpr auto enumeration_check(
	gmt::mat<uint64_t, n_rows, n_cols> const& m)
{
	for(auto i=0UL; i<n_rows; i++)
		for(auto j=0UL; j<n_cols; j++)
			if(m[i][j] != i*n_cols + j)
				return false;

	return true;
}

TEST_CASE("matrix initialization", "[mat]")
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

TEST_CASE("Simple operations on mat", "[mat]")
{

	constexpr auto m = []
	{
		gmt::mat<uint64_t, n_rows, n_cols> tmp;
		for(auto i=0UL; i<n_rows; i++)
			for(auto j=0UL; j<n_cols; j++)
				tmp[i][j] = i*n_cols + j;

		return tmp;
	}();

	STATIC_REQUIRE(enumeration_check(m));
}

TEST_CASE("Determinant", "[mat]")
{

	SECTION("n = 2"){
		constexpr auto m = gmt::mat{{
			{ 1LL, 7LL },
			{ 2LL, 5LL }
		}};
		STATIC_REQUIRE(determinant(m) == -9L);
	}

	SECTION("n = 3"){
		constexpr auto m = gmt::mat{{
			{ 1,  4,  1 },
			{ 2, -1, -2 },
			{ 3, -2,  0 }
		}};
		STATIC_REQUIRE(determinant(m) == -29);
	}

	SECTION("n = 4"){
		constexpr auto m = gmt::mat{{
			{ 1,  4,  1,  1 },
			{ 2, -1, -2, -2 },
			{ 3, -2,  1, -1 },
			{ 3, -2, -1,  2 },
		}};
		STATIC_REQUIRE(determinant(m) == -154);
	}
}

TEST_CASE("mat multiplication", "[mat]")
{
	static constexpr auto mult_with_identity = []<class T, uint64_t N>
		(gmt::mat<T, N, N> const& a)
	{
		auto I = gmt::make_identity_matrix<T, N>();
		return a*I == a;
	};

	SECTION("identity"){
		STATIC_REQUIRE(mult_with_identity(
			gmt::make_identity_matrix<int64_t, 1>()
		));
		STATIC_REQUIRE(mult_with_identity(
			gmt::make_identity_matrix<int64_t, 2>()
		));
		STATIC_REQUIRE(mult_with_identity(
			gmt::make_identity_matrix<int64_t, 3>()
		));
		STATIC_REQUIRE(mult_with_identity(
			gmt::make_identity_matrix<int64_t, 4>()
		));
		STATIC_REQUIRE(mult_with_identity(
			gmt::make_identity_matrix<int64_t, 5>()
		));
	}

	SECTION("2x2"){
		static constexpr auto a = gmt::mat{{
			{ 1,  2 },
			{ 5,  6 }
		}};

		STATIC_REQUIRE(mult_with_identity(a));

		static constexpr auto b = gmt::mat{{
			{ 6,  5 },
			{ 2,  1 }
		}};

		STATIC_REQUIRE(a*b == gmt::mat{{
			{10, 7},
			{42, 31}
		}});
	}

	SECTION("3x3"){
		static constexpr auto a = gmt::mat{{
			{ 1,   2,  3 },
			{ 5,   6,  7 },
			{ 9,  10, 11 },
		}};
		STATIC_REQUIRE(mult_with_identity(a));

		static constexpr auto b = gmt::mat{{
			{ 11, 10, 9 },
			{  7,  6, 5 },
			{  3,  2, 1 },
		}};

		STATIC_REQUIRE(a*b == gmt::mat{{
			{ 34,  28,  22},
			{118, 100,  82},
			{202, 172, 142}
		}});
	}

	SECTION("5x2"){
		static constexpr auto a = gmt::mat{{
			{ 1,  2},
			{ 3,  4},
			{ 5,  6},
			{ 7,  8},
			{ 9, 10}
		}};
		static constexpr auto b = gmt::mat{{
			{ 1, 2, 3, 4, 5 },
			{ 6, 7, 8, 9, 10}
		}};
		STATIC_REQUIRE(a*b == gmt::mat{{
			{ 13, 16,  19,  22,  25 },
			{ 27, 34,  41,  48,  55 },
			{ 41, 52,  63,  74,  85 },
			{ 55, 70,  85, 100, 115 },
			{ 69, 88, 107, 126, 145 }
		}});
	}
}

TEST_CASE("translation matrix", "[mat]")
{
	using vec_t = gmt::vector<double, 3>;
	constexpr auto t = gmt::make_translation_matrix(vec_t{1., 2., 3.});
	STATIC_REQUIRE(t*vec_t::all(0.0) == vec_t{ 1., 2., 3. });
	STATIC_REQUIRE(t*t*vec_t::all(0.0) == vec_t{ 2., 4., 6. });
}

TEST_CASE("basis matrix", "[mat]")
{

	SECTION("simple initialization"){
		constexpr auto m = gmt::make_basis_matrix(
			gmt::vector{{ 1., 2., 3. }},
			gmt::vector{{ 4., 5., 6. }},
			gmt::vector{{ 7., 8., 9. }}
		);

		STATIC_REQUIRE(m == gmt::mat{{
			{ 1., 4., 7. },
			{ 2., 5., 8. },
			{ 3., 6., 9. }
		}});
	}


	constexpr auto b = gmt::make_basis_matrix(
		gmt::vector{{ 1., 2., 3. }},
		gmt::vector{{ 1., 0., 1. }}
	);

	constexpr auto vb = gmt::vector{{ 7., -4. }};
	STATIC_REQUIRE(b*vb == gmt::vector{{ 3., 14., 17. }});

	constexpr auto vd = gmt::vector{{ 8., -6.,  2. }};
	constexpr auto x = gmt::resolve(b, to_mat(vd));

	INFO("x = " << x[0][0] << ' ' << x[0][1] << ' ' << x[0][2]);
	STATIC_REQUIRE(x == to_mat(gmt::vector{{ -3., 11., 0.0}}));
}
