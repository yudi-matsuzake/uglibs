#include "catch2/catch.hpp"

#include <string_view>

#include "util/integers.hpp"

/* uint32_t get_integer_precision(std::string_view str) */
/* { */
/* 	/1* if(str[0] == 'u') *1/ */
/* 	/1* 	return std::strtoi( *1/ */
/* } */

/* constexpr util::integer_variant unsigned_integer_from_string(std::string_view str) */
/* { */
/* 	return util::unsigned_integer<3>{}; */
/* } */

/* constexpr util::integer_variant signed_integer_from_string(std::string_view str) */
/* { */
/* 	return util::signed_integer<3>{}; */
/* } */

/* constexpr util::integer_variant integer_from_string(std::string_view str) */
/* { */
/* 	if(auto c = str[0]; c == 'u') */
/* 		return unsigned_integer_from_string(str); */
/* 	else if(c == 'i') */
/* 		return signed_integer_from_string(str); */
/* 	return {}; */
/* } */

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

	util::integer_variant test = util::unsigned_integer<3>{};
}
