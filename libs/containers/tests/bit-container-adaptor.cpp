#include "catch2/catch_test_macros.hpp"
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

	return rgs::all_of(c, [i = 0, x](auto&& bit) mutable
	{
		return bit == util::get_bit(x, i++, util::bit_order::leftmost{});
	});
}

template<ranges::range R>
bool check_elements(R&& a, ranges::range auto bit_container)
{
	auto to_bits = [](auto const& a)
	{
		return containers::bit_container_adaptor(a);
	};

	using value_t = ranges::range_value_t<R>;
	using vector_t = std::vector<value_t>;

	vector_t v;
	for(auto&& bits : a | ranges::views::transform(to_bits))
		for(auto&& bi : bits)
			v.push_back(bi);

	return ranges::equal(bit_container, v);
}

TEST_CASE("bit container adaptor related tests", "[containers]")
{
	using container_t = containers::bit_container_adaptor<uint8_t>;
	using iterator_t = container_t::iterator<uint8_t>;
	STATIC_REQUIRE(std::input_or_output_iterator<iterator_t>);

	STATIC_REQUIRE(std::is_same_v<
		std::iter_value_t<iterator_t>,
		uint8_t
	>);
	STATIC_REQUIRE(std::is_same_v<
		std::iter_reference_t<iterator_t>,
		util::element_bit_reference<uint8_t>
	>);
	STATIC_REQUIRE(std::is_same_v<
		std::iter_reference_t<iterator_t>,
		util::element_bit_reference<uint8_t>
	>);
	STATIC_REQUIRE(std::is_same_v<
		std::iter_rvalue_reference_t<iterator_t>,
		util::element_bit_reference<uint8_t>
	>);
	STATIC_REQUIRE(
		std::common_reference_with<
			std::iter_rvalue_reference_t<iterator_t>&&,
			const std::iter_value_t<iterator_t>&
		>
	);
	STATIC_REQUIRE(
		std::common_reference_with<
			std::iter_rvalue_reference_t<iterator_t> const&&,
			std::iter_value_t<iterator_t>&
		>
	);
	STATIC_REQUIRE(std::indirectly_readable<
		containers::bit_container_adaptor<uint8_t>::iterator<uint8_t>
	>);
	STATIC_REQUIRE(simple_element_test());

	using T = uint8_t;
	auto a = make_array<T>();
	auto c = containers::bit_container_adaptor(std::span(a));
	constexpr auto n_bits_per_element = util::number_of_bits<T>();

	REQUIRE(a.size()*n_bits_per_element == (c.end() - c.begin()));

	using c_type = decltype(c);
	using c_iterator_type = rgs::iterator_t<c_type>;
	c_iterator_type it;
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
