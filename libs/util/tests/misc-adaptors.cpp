#include <catch2/catch.hpp>

#include <ranges>
#include <vector>
#include <list>

#include "util/misc-adaptors.hpp"

TEST_CASE("enumerate adaptor ", "[util]")
{
	SECTION("enumerate adaptor"){
		auto const a = std::array{1, 2, 3, 4, 5};

		for(auto&& [i, value] : util::enumerate(a)){
			REQUIRE(a[i] == value);
		}
	}
}
