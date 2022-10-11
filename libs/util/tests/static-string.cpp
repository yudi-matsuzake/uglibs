#include "catch2/catch_test_macros.hpp"

#include "util/static-string.hpp"

template<util::static_string S>
struct template_string
{
	static constexpr util::static_string str = S;
};

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

	template_string<"test"> e;
	STATIC_REQUIRE(std::remove_reference_t<decltype(e)>::str == "test");

	constexpr auto s = util::static_string("42");
	STATIC_REQUIRE(util::from_static_string(s) == 42);

	STATIC_REQUIRE(concat_64.substring<1, 4>() == "int");

}
