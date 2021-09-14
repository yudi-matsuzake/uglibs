#include "catch2/catch.hpp"

#include "containers/bit-container-adaptor.hpp"

namespace rgs = std::ranges;

constexpr auto make_array()
{
	constexpr auto N = 1 << util::number_of_bits<uint8_t>();

	auto a = std::array<uint8_t, N>{};

	constexpr auto min = std::numeric_limits<uint8_t>::min();
	constexpr auto max = std::numeric_limits<uint8_t>::max();
	std::generate_n(
		a.begin(),
		N,
		[v = min]() mutable { return v++; });
	return a;
}

constexpr bool simple_element_test()
{
	uint8_t x = 0b10011100;
	auto c = containers::bit_container_adaptor(x);

	return rgs::all_of(c, [i = 0, x](auto bit) mutable
	{
		return bit == util::get_bit(x, i++, util::bit_order::leftmost{});
	});
}

TEST_CASE("bit container adaptor related tests", "[containers]")
{
	static constexpr auto a = make_array();
	constexpr auto c = containers::bit_container_adaptor(std::span(a));
	using c_type = decltype(c);

	c_type::iterator it;

	it = c.begin();

	static_assert(rgs::range<c_type>);
	STATIC_REQUIRE(simple_element_test());

}
