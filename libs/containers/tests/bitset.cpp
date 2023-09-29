#include "catch2/catch_test_macros.hpp"

#include "util/ranges.hpp"
#include <algorithm>

#include "containers/bitset.hpp"


TEST_CASE("Bit set simple tests", "[containers][bitset]")
{
	{
	containers::bitset<8> bs;
	REQUIRE(bs.size() == 8);
	rg::fill(bs, 0);
	REQUIRE(rg::all_of(bs, [](auto&& x){ return x == 0; }));
	}

	{
	containers::bitset<3> bs;
	REQUIRE(bs.size() == 3);
	bs[0] = 1;
	bs[1] = 1;
	bs[2] = 0;

	REQUIRE(rg::equal(bs, std::array{ 1, 1, 0 }));
	REQUIRE(rg::equal(bs | rg::vw::reverse, std::array{ 0, 1, 1 }));
	}

	{
	containers::bitset<3> bs;

	rg::fill(bs, 0);
	REQUIRE(rg::equal(bs, std::array{ 0, 0, 0 }));

	containers::add_one(bs);
	REQUIRE(rg::equal(bs, std::array{ 0, 0, 1 }));

	containers::add_one(bs);
	REQUIRE(rg::equal(bs, std::array{ 0, 1, 0 }));

	containers::add_one(bs);
	REQUIRE(rg::equal(bs, std::array{ 0, 1, 1 }));

	containers::add_one(bs);
	REQUIRE(rg::equal(bs, std::array{ 1, 0, 0 }));

	containers::add_one(bs);
	REQUIRE(rg::equal(bs, std::array{ 1, 0, 1 }));

	containers::add_one(bs);
	REQUIRE(rg::equal(bs, std::array{ 1, 1, 0 }));

	containers::add_one(bs);
	REQUIRE(rg::equal(bs, std::array{ 1, 1, 1 }));

	}
}
