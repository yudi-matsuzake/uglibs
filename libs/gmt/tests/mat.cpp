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
		constexpr auto m = []
		{
			gmt::mat<int64_t, 2, 2> tmp;
		
			tmp[0][0] = 1;
			tmp[0][1] = 7;
			tmp[1][0] = 2;
			tmp[1][1] = 5;
		
			return tmp;
		}();
		
		STATIC_REQUIRE(determinant(m) == -9L);
		
	}

	SECTION("n = 3"){
		constexpr auto m = []
		{
			gmt::mat<int64_t, 3, 3> tmp;
		
			tmp[0][0] = 1;
			tmp[0][1] = 4;
			tmp[0][2] = 1;
			tmp[1][0] = 2;
			tmp[1][1] = -1;
			tmp[1][2] = -2;
			tmp[2][0] = 3;
			tmp[2][1] = -2;
			tmp[2][2] = 0;
		
			return tmp;
		}();

		STATIC_REQUIRE(determinant(m) == -29L);
		
	}

	SECTION("n = 4"){
		constexpr auto m = []
		{
			gmt::mat<int64_t, 4, 4> tmp;
		
			tmp[0][0] = 1;
			tmp[0][1] = 4;
			tmp[0][2] = 1;
			tmp[0][3] = 1;

			tmp[1][0] = 2;
			tmp[1][1] = -1;
			tmp[1][2] = -2;
			tmp[1][3] = -2;

			tmp[2][0] = 3;
			tmp[2][1] = -2;
			tmp[2][2] = 1;
			tmp[2][3] = -1;
		
			tmp[3][0] = 3;
			tmp[3][1] = -2;
			tmp[3][2] = -1;
			tmp[3][3] = 2;
		
			return tmp;
		}();

		STATIC_REQUIRE(determinant(m) == -154L);
		
	}
}
