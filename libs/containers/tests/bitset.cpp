#include "catch2/catch_all.hpp"

#include <ranges>
#include <algorithm>

#include "containers/bitset.hpp"

namespace rgs = std::ranges;
namespace vws = std::views;

TEST_CASE("Bit set simple tests", "[containers][bitset]")
{
	{
	containers::bitset<8> bs;
	REQUIRE(bs.size() == 8);
	rgs::fill(bs, 0);
	REQUIRE(rgs::all_of(bs, [](auto&& x){ return x == 0; }));
	}

	{
	containers::bitset<3> bs;
	REQUIRE(bs.size() == 3);
	bs[0] = 1;
	bs[1] = 1;
	bs[2] = 0;

	REQUIRE(rgs::equal(bs, std::array{ 1, 1, 0 }));
	REQUIRE(rgs::equal(bs | vws::reverse, std::array{ 0, 1, 1 }));
	}

	{
	containers::bitset<3> bs;

	rgs::fill(bs, 0);
	REQUIRE(rgs::equal(bs, std::array{ 0, 0, 0 }));

	containers::add_one(bs);
	REQUIRE(rgs::equal(bs, std::array{ 0, 0, 1 }));

	containers::add_one(bs);
	REQUIRE(rgs::equal(bs, std::array{ 0, 1, 0 }));

	containers::add_one(bs);
	REQUIRE(rgs::equal(bs, std::array{ 0, 1, 1 }));

	containers::add_one(bs);
	REQUIRE(rgs::equal(bs, std::array{ 1, 0, 0 }));

	containers::add_one(bs);
	REQUIRE(rgs::equal(bs, std::array{ 1, 0, 1 }));

	containers::add_one(bs);
	REQUIRE(rgs::equal(bs, std::array{ 1, 1, 0 }));

	containers::add_one(bs);
	REQUIRE(rgs::equal(bs, std::array{ 1, 1, 1 }));

	}
}
