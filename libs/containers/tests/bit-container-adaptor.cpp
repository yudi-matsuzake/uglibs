#include "catch2/catch.hpp"
#include <ranges>

#include "containers/bit-container-adaptor.hpp"
#include "range/v3/all.hpp"

namespace rgs = std::ranges;

template<std::integral T>
auto make_array()
{

	constexpr auto min = std::numeric_limits<T>::min();
	constexpr auto max = std::numeric_limits<T>::max();
	constexpr auto N = max - min;
	auto a = std::array<T, N>{};

	rgs::copy(rgs::views::iota(min, max), a.begin());
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

bool check_elements(ranges::range auto a, ranges::range auto bit_container)
{
	auto to_bits = [](auto const& a)
	{
		return containers::bit_container_adaptor(a);
	};

	auto const all_bits = a
		| ranges::views::transform(to_bits)
		| ranges::to<std::vector>;

	return ranges::equal(bit_container, all_bits | ranges::views::join);
}

TEST_CASE("bit container adaptor related tests", "[containers]")
{
	STATIC_REQUIRE(simple_element_test());

	using T = uint8_t;
	auto a = make_array<T>();
	auto c = containers::bit_container_adaptor(std::span(a));
	constexpr auto n_bits_per_element = util::number_of_bits<T>();

	REQUIRE(a.size()*n_bits_per_element == (c.end() - c.begin()));

	using c_type = decltype(c);
	c_type::iterator it;
	it = c.begin();
	STATIC_REQUIRE(rgs::range<c_type>);

	REQUIRE(check_elements(a, c));

	auto b = c.begin() + (42 * n_bits_per_element);
	auto bits_c42 = rgs::subrange(b, b + n_bits_per_element);
	auto v42 = T{42};

	auto bits_42 = containers::bit_container_adaptor(v42);

	REQUIRE(rgs::size(bits_c42) == rgs::size(bits_42));
	REQUIRE(rgs::equal(bits_c42, bits_42));
}
