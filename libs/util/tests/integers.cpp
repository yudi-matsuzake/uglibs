
#include <string_view>
#include <optional>
#include <variant>

#include "catch2/catch.hpp"
#include "util/integers.hpp"

using opt_integer_t = std::optional<util::integer_variant>;

namespace detail{

template<class S, class T, T first, T ... ints>
util::integer_variant make_integer(
	uint32_t n, std::integer_sequence<T, first, ints ...>)
{
	if(n == first)
		return util::integer<first, S>{};
	if constexpr(sizeof...(ints) > 0){
		return make_integer<S>(n, std::integer_sequence<T, ints...>{});
	}else{
		throw std::runtime_error("there is no integer with this size");
	}
}

template<class T, T ... I>
util::integer_variant make_unsigned_integer(
	uint32_t n, std::integer_sequence<T, I...> ints)
{
	return make_integer<util::unsigned_flag>(n, ints);
}

template<class T, T ... I>
util::integer_variant make_signed_integer(
	uint32_t n, std::integer_sequence<T, I...> ints)
{
	return make_integer<util::signed_flag>(n, ints);
}

}

opt_integer_t integer_from_string(std::string_view str)
{
	if(str.size() < 4 || str.size() > 6)
		return std::nullopt;

	if(str.substr(0, 4) == "uint"){
		auto n = std::strtoul(str.substr(4).data(), nullptr, 10);
		return detail::make_unsigned_integer(
			n,
			util::variant_integer_precisions_t{});
	}else if(str.substr(0, 3) == "int"){
		auto n = std::strtoul(str.substr(3).data(), nullptr, 10);
		return detail::make_signed_integer(
			n,
			util::variant_integer_precisions_t{});
	}

	return std::nullopt;
}

template<class IntegerType>
auto test_integer_from_string(std::string_view str)
{
	if(auto i = integer_from_string(str)){
		return util::match(
			i.value(),
			[](auto){ return false; },
			[](IntegerType){ return true; }
		);
	}

	return false;
}

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

TEST_CASE("integers variant", "[util]")
{
	util::integer_variant test = util::unsigned_integer<3>{};

	REQUIRE(test_integer_from_string<util::signed_integer<8>>("int8"));
	REQUIRE(test_integer_from_string<util::signed_integer<18>>("int18"));
	REQUIRE(test_integer_from_string<util::signed_integer<33>>("int33"));
	REQUIRE(test_integer_from_string<util::signed_integer<24>>("int24"));
	REQUIRE(test_integer_from_string<util::signed_integer<58>>("int58"));
	REQUIRE(test_integer_from_string<util::unsigned_integer<8>>("uint8"));
	REQUIRE(test_integer_from_string<util::unsigned_integer<15>>("uint15"));
	REQUIRE(test_integer_from_string<util::unsigned_integer<20>>("uint20"));
	REQUIRE(test_integer_from_string<util::unsigned_integer<60>>("uint60"));
}
