#include "catch2/catch.hpp"

#include "util/bit-adaptors.hpp"

constexpr bool test_get_value_int()
{
	int32_t x = 0b10001001001010100101001010010101;
	util::element_bit_reference r{ x, 0 };
	for(; r.bit_index < util::number_of_bits<int32_t>(); ++r.bit_index){
		auto const ground_truth = util::get_bit(
			x, r.bit_index, util::bit_order::leftmost{});

		if(r.get_bit_value() != ground_truth)
			return false;
	}
	return true;
}

constexpr bool test_get_value_array()
{
	using namespace util;
	std::array a = {
		uint8_t{ 0b10011001 },
		uint8_t{ 0b11011101 },
		uint8_t{ 0b00111101 },
		uint8_t{ 0b00111101 },
		uint8_t{ 0b10111101 },
		uint8_t{ 0b10111101 },
		uint8_t{ 0b10110101 },
		uint8_t{ 0b00110101 },
		uint8_t{ 0b00110101 },
		uint8_t{ 0b01010101 },
		uint8_t{ 0b01010101 },
		uint8_t{ 0b10111011 }
	};
	
	element_bit_reference r(std::span<uint8_t>{a}, 0);
	auto const n_bits = number_of_bits<uint8_t>();
	auto const n = n_bits * a.size();

	for(; r.bit_index<n; ++r.bit_index){
		auto const ai = r.bit_index / n_bits;
		auto const bi = r.bit_index % n_bits;
		auto const ground_truth = get_bit(
			a[ai], bi, bit_order::leftmost{});

		if(r.get_bit_value() != ground_truth)
			return false;
	}
	return true;
}

constexpr bool test_set_value_int()
{
	uint8_t x = 0b11111111;
	util::element_bit_reference r{ x, 1 };
	r = 0;

	if(x != 0b10111111) return false;

	r.bit_index = 2;
	r = 0;

	if(x != 0b10011111) return false;

	util::element_bit_reference l{ x, 7 };

	l = r;

	if(l != r) return false;

	if(x != 0b10011110) return false;

	return true;
}

constexpr bool test_const_int()
{
	uint8_t x = 0b100;
	util::element_bit_reference r{ x, 6 };
	uint8_t const b = 1;

	r = b;

	return x == 0b110;
}

TEST_CASE("element_bit_reference simple tests", "[util]")
{
	STATIC_REQUIRE(test_get_value_int());
	STATIC_REQUIRE(test_get_value_array());
	STATIC_REQUIRE(test_set_value_int());
	STATIC_REQUIRE(test_const_int());
}
