#include <catch2/catch.hpp>

#include "util/static-string.hpp"

TEST_CASE("static string", "[util]")
{
	constexpr auto a = util::to_static_string<42>();

	STATIC_REQUIRE(a.size() == 3);
	STATIC_REQUIRE(a == util::static_string("42"));

	constexpr auto concat_64 = util::static_string("uint")
		+ util::to_static_string<64>()
		+ "_t";

	STATIC_REQUIRE(concat_64 == "uint64_t");

	constexpr auto concat_32 = "uint" + util::to_static_string<32>() + "_t";
	STATIC_REQUIRE(concat_32 == "uint32_t");
}
