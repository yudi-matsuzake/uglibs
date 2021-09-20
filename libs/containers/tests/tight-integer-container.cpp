#include "catch2/catch.hpp"

#include <numeric>
#include <ranges>

#include "util/misc.hpp"
#include "util/misc-adaptors.hpp"
#include "containers/tight-integers-container.hpp"

#include "range/v3/all.hpp"

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

	constexpr auto min = tight_container_t::min_value();
	constexpr auto max = tight_container_t::max_value();

	constexpr int64_t n_edge_values = 100;
	v.resize(n_edge_values * 2);

	auto make_array = [](underint_t b)
	{
		std::array<underint_t, n_edge_values> a;
		std::iota(a.begin(), a.end(), b);
		return a;
	};

	auto const test_intervals = std::vector{
		make_array(min),
		make_array(max - n_edge_values - 1)
	};

	auto const values = test_intervals | vws::join;

	rgs::copy(values, v.begin());
	INFO("min: " << min);
	INFO("max: " << max);
	INFO("values size: " << test_intervals.size() * n_edge_values);
	INFO("v size: " << rgs::size(v));
	INFO("values size: " << test_intervals.size() * n_edge_values);
	REQUIRE(rgs::equal(v, values));
}

TEST_CASE("bit size checks", "[tight-integers-container]")
{
	test_edge_case_for<7, containers::unsigned_flag>();
	test_edge_case_for<8, containers::unsigned_flag>();
	test_edge_case_for<9, containers::unsigned_flag>();

	test_edge_case_for<15, containers::unsigned_flag>();
	test_edge_case_for<16, containers::unsigned_flag>();
	test_edge_case_for<17, containers::unsigned_flag>();

	test_edge_case_for<31, containers::unsigned_flag>();
	test_edge_case_for<32, containers::unsigned_flag>();
	test_edge_case_for<33, containers::unsigned_flag>();

	test_edge_case_for<63, containers::unsigned_flag>();
	test_edge_case_for<64, containers::unsigned_flag>();

	test_edge_case_for<7, containers::signed_flag>();
	test_edge_case_for<8, containers::signed_flag>();
	test_edge_case_for<9, containers::signed_flag>();

	test_edge_case_for<15, containers::signed_flag>();
	test_edge_case_for<16, containers::signed_flag>();
	test_edge_case_for<17, containers::signed_flag>();

	test_edge_case_for<31, containers::signed_flag>();
	test_edge_case_for<32, containers::signed_flag>();
	test_edge_case_for<33, containers::signed_flag>();

	test_edge_case_for<63, containers::signed_flag>();
	test_edge_case_for<64, containers::signed_flag>();
}

TEST_CASE("sorting tight-integers", "[tight-integers-container]")
{
	constexpr auto integer_bit_size = 8;
	using tight_container_t = containers::tight_integer_container<
		integer_bit_size, containers::unsigned_flag>;
	using underint_t = tight_container_t::underlying_integer_t;

	tight_container_t v;
	auto const v_siz = 100;
	v.resize(v_siz);

	using iterator_t = tight_container_t::iterator<tight_container_t>;
	using reference_t = iterator_t::reference;

	rgs::copy(
		vws::iota(0)
			| vws::take(v_siz)
			| vws::reverse
			| vws::transform(util::make_static_cast<underint_t>()),
		v.begin());

	STATIC_REQUIRE(std::sortable<iterator_t>);
	rgs::sort(v);
	REQUIRE(rgs::is_sorted(v));
}
