#include <catch2/catch.hpp>

#include <ranges>
#include <vector>
#include <list>

#include "util/misc-adaptors.hpp"
#include "range/v3/algorithm/all_of.hpp"

TEST_CASE("enumerate adaptor ", "[util]")
{
	auto const a = std::array{1, 2, 3, 4, 5};

	for(auto&& [i, value] : util::enumerate(a)){
		REQUIRE(a[i] == value);
	}

	REQUIRE(ranges::all_of(util::enumerate(a),
		[&a](auto&& pack)
		{
			auto&& [i, value] = pack;
			return a[i] == value;
		}
	));
}
