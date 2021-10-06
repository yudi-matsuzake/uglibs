#include <type_traits>
#include <utility>

#include "catch2/catch.hpp"

#include "util/integers.hpp"
#include "util/integer-variant.hpp"
#include "util/meta.hpp"


TEST_CASE("integer_precisions_t", "[integer-variant]")
{
	using variant_unsigned_integer_t = util::merged_variant_t<
		util::make_integer_variant_t<util::unsigned_flag, 1, 5>,
		std::variant<uint8_t, uint16_t>
	>;

	using variant_signed_integer_t = util::merged_variant_t<
		util::make_integer_variant_t<util::signed_flag, 2, 7>,
		std::variant<int8_t, int16_t, int32_t>
	>;

	using variant_integer_t = util::merged_variant_t<
		variant_unsigned_integer_t,
		variant_signed_integer_t>;

	STATIC_REQUIRE(std::is_same_v<
		util::integer_precisions_t<util::unsigned_flag, variant_integer_t>,
		std::integer_sequence<uint32_t, 1, 2, 3, 4, 5, 8, 16>
	>);

	STATIC_REQUIRE(std::is_same_v<
		util::integer_precisions_t<util::signed_flag, variant_integer_t>,
		std::integer_sequence<uint32_t, 2, 3, 4, 5, 6, 7, 8, 16, 32>
	>);
}


using unsigned_integer_variant_32 = util::make_integer_variant_t<
	util::unsigned_flag, 1, 32>;

using signed_integer_variant_32 = util::make_integer_variant_t<
	util::signed_flag, 2, 32>;

using integer_32 = util::merged_variant_t<
	unsigned_integer_variant_32,
	signed_integer_variant_32>;

template<class IntegerType>
auto test_string_to_integer(std::string_view str)
{
	if(auto i = util::integer_from_string<integer_32>(str)){
		return util::match(
			i.value(),
			[](auto){ return false; },
			[](IntegerType){ return true; }
		);
	}

	return false;
}

template<uint32_t N>
using sinteger = util::signed_integer<N>;

template<uint32_t N>
using uinteger = util::unsigned_integer<N>;

TEST_CASE("supported integers variant", "[integer-variant]")
{
	integer_32 test = util::unsigned_integer<3>{};

	REQUIRE(test_string_to_integer<sinteger<8>>("int8"));
	REQUIRE(test_string_to_integer<sinteger<18>>("int18"));
	REQUIRE(test_string_to_integer<sinteger<24>>("int24"));
	REQUIRE(test_string_to_integer<uinteger<8>>("uint8"));
	REQUIRE(test_string_to_integer<uinteger<15>>("uint15"));
	REQUIRE(test_string_to_integer<uinteger<20>>("uint20"));
	REQUIRE_FALSE(test_string_to_integer<uinteger<20>>("int20"));
	REQUIRE_FALSE(test_string_to_integer<uinteger<1>>("int1"));
}
