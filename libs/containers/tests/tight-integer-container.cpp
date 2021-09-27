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

template<bool is_signed>
constexpr static auto make_values(
	std::integral auto n_values,
	std::integral auto max)
{
	auto default_size = max < n_values ? max : n_values;

	if constexpr(is_signed){
		int64_t const first_value = -default_size/2;
		int64_t const last_value = first_value+default_size;
		return rgs::iota_view(first_value, last_value);
	}else{
		uint64_t const first_value = 0;
		uint64_t const last_value = default_size;
		return rgs::iota_view(first_value, last_value);
	}
};

TEST_CASE("underlying integer", "[tight-integers-container]")
{
	using bin_container_t = util::underlying_integer<
		1, util::unsigned_flag>::type;

	STATIC_REQUIRE(sizeof(bin_container_t) == 1);
	STATIC_REQUIRE(sizeof(util::underlying_integer<8>::type) == 1);
	STATIC_REQUIRE(sizeof(util::underlying_integer<9>::type) == 2);
	STATIC_REQUIRE(sizeof(util::underlying_integer<16>::type) == 2);
	STATIC_REQUIRE(sizeof(util::underlying_integer<17>::type) == 4);
	STATIC_REQUIRE(sizeof(util::underlying_integer<32>::type) == 4);
	STATIC_REQUIRE(sizeof(util::underlying_integer<33>::type) == 8);
	STATIC_REQUIRE(sizeof(util::underlying_integer<64>::type) == 8);
}

TEST_CASE(
	"reserve, clear, size and capacity",
	"[tight-integers-container]")
{
	using integer_t = util::unsigned_integer<9>;

	using tight_container_t = containers::tight_integer_container<
		integer_t>;

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
	using integer_t = util::integer<N, S>;
	using tight_container_t = containers::tight_integer_container<
		integer_t>;

	using underint_t = typename tight_container_t::underlying_integer_t;

	constexpr bool is_signed = std::is_same_v<
		S, util::signed_flag>;

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
	test_edge_case_for<7, util::unsigned_flag>();
	test_edge_case_for<8, util::unsigned_flag>();
	test_edge_case_for<9, util::unsigned_flag>();

	test_edge_case_for<15, util::unsigned_flag>();
	test_edge_case_for<16, util::unsigned_flag>();
	test_edge_case_for<17, util::unsigned_flag>();

	test_edge_case_for<31, util::unsigned_flag>();
	test_edge_case_for<32, util::unsigned_flag>();
	test_edge_case_for<33, util::unsigned_flag>();

	test_edge_case_for<63, util::unsigned_flag>();
	test_edge_case_for<64, util::unsigned_flag>();

	test_edge_case_for<7, util::signed_flag>();
	test_edge_case_for<8, util::signed_flag>();
	test_edge_case_for<9, util::signed_flag>();

	test_edge_case_for<15, util::signed_flag>();
	test_edge_case_for<16, util::signed_flag>();
	test_edge_case_for<17, util::signed_flag>();

	test_edge_case_for<31, util::signed_flag>();
	test_edge_case_for<32, util::signed_flag>();
	test_edge_case_for<33, util::signed_flag>();

	test_edge_case_for<63, util::signed_flag>();
	test_edge_case_for<64, util::signed_flag>();
}

template<int N, class S>
void test_sorting_for()
{
	using integer_t = util::integer<N, S>;
	using tight_container_t = containers::tight_integer_container<
		integer_t>;
	using underint_t = tight_container_t::underlying_integer_t;
	constexpr bool is_signed = tight_container_t::is_signed;

	auto const values = make_values<is_signed>(
		20, tight_container_t::max_value());

	auto const v_siz = rgs::size(values);
	tight_container_t v(v_siz);

	using iterator_t = typename tight_container_t::iterator;
	using reference_t = iterator_t::reference;

	rgs::copy(values | vws::reverse, v.begin());
	REQUIRE(rgs::equal(values | vws::reverse, v));

	STATIC_REQUIRE(std::sortable<iterator_t>);
	rgs::sort(v);

	REQUIRE(rgs::equal(v, values));
}

TEST_CASE("sorting tight-integers", "[tight-integers-container]")
{

	test_sorting_for<2, util::unsigned_flag>();
	test_sorting_for<3, util::unsigned_flag>();
	test_sorting_for<4, util::unsigned_flag>();
	test_sorting_for<5, util::unsigned_flag>();

	test_sorting_for<7, util::unsigned_flag>();
	test_sorting_for<8, util::unsigned_flag>();
	test_sorting_for<9, util::unsigned_flag>();

	test_sorting_for<15, util::unsigned_flag>();
	test_sorting_for<16, util::unsigned_flag>();
	test_sorting_for<17, util::unsigned_flag>();

	test_sorting_for<31, util::unsigned_flag>();
	test_sorting_for<32, util::unsigned_flag>();
	test_sorting_for<33, util::unsigned_flag>();

	test_sorting_for<63, util::unsigned_flag>();
	test_sorting_for<64, util::unsigned_flag>();

	test_sorting_for<7, util::signed_flag>();
	test_sorting_for<8, util::signed_flag>();
	test_sorting_for<9, util::signed_flag>();

	test_sorting_for<15, util::signed_flag>();
	test_sorting_for<16, util::signed_flag>();
	test_sorting_for<17, util::signed_flag>();

	test_sorting_for<31, util::signed_flag>();
	test_sorting_for<32, util::signed_flag>();
	test_sorting_for<33, util::signed_flag>();

	test_sorting_for<63, util::signed_flag>();
	test_sorting_for<64, util::signed_flag>();
}

template<class T>
void integral_sorting()
{
	using integer_t = T;
	using tight_container_t = containers::tight_integer_container<
		integer_t>;

	STATIC_REQUIRE(std::is_base_of_v<std::vector<T>, tight_container_t>);
	using underint_t = tight_container_t::underlying_integer_t;

	constexpr auto is_signed = tight_container_t::is_signed;

	auto const values = make_values<is_signed>(
		20, tight_container_t::max_value());
	auto const v_siz = rgs::size(values);
	tight_container_t v(v_siz);

	using iterator_t = typename tight_container_t::iterator;
	using reference_t = iterator_t::reference;

	rgs::copy(values | vws::reverse, v.begin());
	REQUIRE(rgs::equal(v, values | vws::reverse));

	STATIC_REQUIRE(std::sortable<iterator_t>);
	rgs::sort(v);
	REQUIRE(rgs::equal(v, values));
}

TEST_CASE("integral types sorting", "[tight-integers-container]")
{
	integral_sorting<uint8_t>();
	integral_sorting<int8_t>();

	integral_sorting<uint16_t>();
	integral_sorting<int16_t>();

	integral_sorting<uint32_t>();
	integral_sorting<int32_t>();

	integral_sorting<uint64_t>();
	integral_sorting<int64_t>();
}

TEST_CASE("binary tight integer container", "[tight-integers-container]")
{
	using binary_tight_t = containers::tight_integer_container<
		util::unsigned_integer<1>>;

	binary_tight_t c(8);

	uint8_t x = 42;
	auto x_bits = containers::bit_container_adaptor(x);
	rgs::copy(x_bits, c.begin());

	REQUIRE(rgs::equal(x_bits, c));

	rgs::sort(c);
	REQUIRE(rgs::is_sorted(c));

	rgs::copy(c, x_bits.begin());
	REQUIRE(rgs::equal(x_bits, c));
	REQUIRE(x == 7);
}
