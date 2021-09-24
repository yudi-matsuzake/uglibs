#include "catch2/catch.hpp"

#include "util/integers.hpp"

TEST_CASE("integers", "[util]")
{
	STATIC_REQUIRE(util::unsigned_integer<2>::type_name() == "uint2");
	STATIC_REQUIRE(util::unsigned_integer<5>::type_name() == "uint5");
	STATIC_REQUIRE(util::unsigned_integer<8>::type_name() == "uint8");
	STATIC_REQUIRE(util::unsigned_integer<32>::type_name() == "uint32");
	STATIC_REQUIRE(util::unsigned_integer<64>::type_name() == "uint64");

	STATIC_REQUIRE(util::signed_integer<2>::type_name() == "int2");
	STATIC_REQUIRE(util::signed_integer<5>::type_name() == "int5");
	STATIC_REQUIRE(util::signed_integer<8>::type_name() == "int8");
	STATIC_REQUIRE(util::signed_integer<32>::type_name() == "int32");
	STATIC_REQUIRE(util::signed_integer<64>::type_name() == "int64");
}
