#include "catch2/catch.hpp"

#include <ranges>

#include "util/misc-adaptors.hpp"
#include "containers/tight-integers-container.hpp"

namespace rgs = std::ranges;
namespace vws = std::ranges::views;
namespace acs = ranges::actions;

TEST_CASE("underlying integer", "[tight-integers-container]")
{
	using bin_container_t = containers::underlying_integer<
		1, containers::unsigned_flag>::type;

	STATIC_REQUIRE(sizeof(bin_container_t) == 1);
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

template<size_t N, class S>
static auto test_edge_case_for()
{
	constexpr auto integer_bit_size = N;
	using tight_container_t = containers::tight_integer_container<
		integer_bit_size, S>;

	using underint_t = tight_container_t::underlying_integer_t;

	constexpr bool is_signed = std::is_same_v<
		S, containers::signed_flag>;

	tight_container_t v;
	STATIC_REQUIRE(rgs::range<tight_container_t>);

	constexpr auto max = is_signed
		? ((1 << (N - 1)) - 1)
		: ((1 << N) - 1);

	constexpr auto min = is_signed
		? (-(max+1))
		: 0;

	STATIC_REQUIRE(tight_container_t::min_value() == min);
	STATIC_REQUIRE(tight_container_t::max_value() == max);
	constexpr int64_t n_edge_values = 100;
	v.resize(n_edge_values * 2);

	auto make_array = [](auto&& v)
	{
		std::array<underint_t, n_edge_values> a;
		rgs::copy_n(v.begin(), n_edge_values, a.begin());
		return a;
	};

	auto const values = std::vector{
		make_array(vws::iota(min, min + n_edge_values)),
		make_array(vws::iota(max - n_edge_values + 1, max + 1))
	};

	rgs::copy(values | vws::join, v.begin());
	REQUIRE(rgs::equal(v, values | vws::join));
}

TEST_CASE("bit size checks", "[tight-integers-container]")
{
	test_edge_case_for<7, containers::unsigned_flag>();
	test_edge_case_for<8, containers::unsigned_flag>();
	test_edge_case_for<9, containers::unsigned_flag>();

	test_edge_case_for<7, containers::signed_flag>();
	test_edge_case_for<8, containers::signed_flag>();
	test_edge_case_for<9, containers::signed_flag>();
}
