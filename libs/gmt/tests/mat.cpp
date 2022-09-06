#include "catch2/catch.hpp"

#include "gmt/mat.hpp"

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
