
#include <string_view>
#include <optional>
#include <variant>

#include "catch2/catch_test_macros.hpp"
#include "util/integers.hpp"

template<class Integer, util::static_string str>
static constexpr auto check_type_name()
{
	return util::integer_info<Integer>::type_name == str;
}


TEST_CASE("integers", "[util]")
{
	STATIC_REQUIRE(util::integer_info<util::unsigned_integer<2>>::type_name == "uint2");
	STATIC_REQUIRE(check_type_name<util::unsigned_integer<5>, "uint5">());
	STATIC_REQUIRE(check_type_name<util::unsigned_integer<8>, "uint8">());
	STATIC_REQUIRE(check_type_name<util::unsigned_integer<32>, "uint32">());
	STATIC_REQUIRE(check_type_name<util::unsigned_integer<64>, "uint64">());

	STATIC_REQUIRE(check_type_name<util::signed_integer<2>, "int2">());
	STATIC_REQUIRE(check_type_name<util::signed_integer<5>, "int5">());
	STATIC_REQUIRE(check_type_name<util::signed_integer<8>, "int8">());
	STATIC_REQUIRE(check_type_name<util::signed_integer<32>, "int32">());
	STATIC_REQUIRE(check_type_name<util::signed_integer<64>, "int64">());

}
