#include "catch2/catch.hpp"

#include <ranges>

#include "util/misc-adaptors.hpp"
#include "containers/tight-integers-container.hpp"

namespace rgs = std::ranges;
namespace vws = std::ranges::views;

TEST_CASE("underlying integer", "[tight-integers-container]")
{
	STATIC_REQUIRE(sizeof(containers::underlying_integer<1>::type) == 1);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<8>::type) == 1);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<9>::type) == 2);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<16>::type) == 2);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<17>::type) == 4);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<32>::type) == 4);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<33>::type) == 8);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<64>::type) == 8);

}

TEST_CASE(
	"reserve, clear, size and capacity",
	"[tight-integers-container]")
{
	constexpr auto integer_bit_size = 9;
	using tight_container_t = containers::tight_integer_container<
		integer_bit_size,
		containers::unsigned_flag>;

	using underint_t = tight_container_t::underlying_integer_t;

	tight_container_t v;

	REQUIRE(v.empty() == true);

	v.resize(10);
	v.clear();
	REQUIRE(v.size() == 0);
	REQUIRE(v.capacity() >= 0);

	for(auto i=0UL; i<30; ++i){
		INFO(i);
		v.reserve(i);
		REQUIRE(v.size() == 0);
		REQUIRE(v.capacity() >= i);
	}

	for(auto i=0UL; i<30; ++i){
		INFO(i);
		v.resize(i);
		REQUIRE(v.size() == i);
		REQUIRE(v.capacity() >= i);
		REQUIRE(v.empty() == (i == 0UL));

	}

}

TEST_CASE("unsigned bit size = 9", "[tight-integers-container]")
{
	constexpr auto integer_bit_size = 9;
	using tight_container_t = containers::tight_integer_container<
		integer_bit_size,
		containers::unsigned_flag>;

	using underint_t = tight_container_t::underlying_integer_t;

	tight_container_t v;

	STATIC_REQUIRE(tight_container_t::min_value() == underint_t{ 0 });
	STATIC_REQUIRE(tight_container_t::max_value() == underint_t{ 511 });
	constexpr underint_t n_values = tight_container_t::max_value() + 1;
	v.resize(n_values);

	for(auto i : vws::iota(underint_t{0}, n_values)){
		v[i] = i;
		REQUIRE(v[i] == i);
	}

	/* STATIC_REQUIRE(rgs::range<tight_container_t>); */

	/* auto seq = vws::iota(0UL, v.size()) | */
	/* 	vws::transform(util::make_static_cast<short>()); */

	/* rgs::copy(seq, rgs::begin(v)); */
	/* REQUIRE(rgs::equal(seq, v)); */

}

TEST_CASE("signed bit size = 9", "[tight-integers-container]")
{
	constexpr auto integer_bit_size = 9;
	using tight_container_t = containers::tight_integer_container<
		integer_bit_size,
		containers::signed_flag>;

	using underint_t = tight_container_t::underlying_integer_t;

	STATIC_REQUIRE(std::numeric_limits<int8_t>::min() == -128);
	STATIC_REQUIRE(std::numeric_limits<int8_t>::max() == 127);

	STATIC_REQUIRE(tight_container_t::min_value() == underint_t{ -256 });
	STATIC_REQUIRE(tight_container_t::max_value() == underint_t{ +255 });

	tight_container_t v;

	constexpr underint_t min = tight_container_t::min_value();
	constexpr underint_t max = tight_container_t::max_value();
	constexpr underint_t n_values = (max - min) + 1;
	v.resize(n_values);

	for(auto i : vws::iota(underint_t{0}, n_values)){
		auto value = min + i;
		v[i] = value;
		REQUIRE(v[i] == value);
	}

}
