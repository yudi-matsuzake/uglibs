#include <catch2/catch.hpp>

#include <array>
#include <ranges>
#include <algorithm>

#include "util/misc-adaptors.hpp"
#include "range/v3/view.hpp"

namespace rgs = std::ranges;
namespace vws = std::views;

namespace views = ranges::views;

TEST_CASE("numbers view", "[util]")
{

	REQUIRE(rgs::equal(util::numbers(0, 10), vws::iota(0, 10)));

	REQUIRE(rgs::equal(
		util::numbers(0.0, 1.5),
		std::array{ 0.0, 1.0 }
	));

	REQUIRE(rgs::equal(
		util::numbers(0.0, 0.7, 0.3),
		std::array{ 0.0, 0.3, 0.6 }
	));
}
