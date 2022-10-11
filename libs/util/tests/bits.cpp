#include "catch2/catch_all.hpp"

#include "util/bit.hpp"

template<class T, class F, class Init>
[[nodiscard]] constexpr auto for_all_bits(
	util::bit_index_order order, F f, Init init) noexcept
{
	auto x = init;
	for(auto i=T{0}; i<util::number_of_bits<T>(); ++i)
		x = f(x, i, order);
	return x;
}

template<class T>
[[nodiscard]] constexpr auto set_all(util::bit_index_order order) noexcept
{
	return for_all_bits<T>(order, util::set_bit<T, T>, T{0});
}

template<class T>
[[nodiscard]] constexpr auto clear_all(util::bit_index_order order) noexcept
{
	return for_all_bits<T>(order, util::clear_bit<T, T>, set_all<T>(order));
}

template<class T>
[[nodiscard]] constexpr bool all_is(
	T n,
	std::integral auto b,
	util::bit_index_order order)
{
	constexpr auto n_bits = util::number_of_bits<T>();
	for(auto i=T{0}; i<n_bits; ++i)
		if(!(util::get_bit(n, i, order) == b))
			return false;
	return true;
}

template<class T>
constexpr auto make_bits_array(util::bit_index_order order)
{
	constexpr uint64_t x = 0b1010011101101010110110100100110100111010111111111000010011010011;

	constexpr auto n = util::number_of_bits<T>();
	std::array<bool, n> a;

	T y = 0;

	for(int i=0; i<n; ++i){
		a[i] = util::get_bit(x, i, order) == 1;
		y = util::set_bit(y, i, a[i], order);
	}

	return std::tuple{ a, y };
}

template<class T>
[[nodiscard]] constexpr
bool test_set_and_get(util::bit_index_order order) noexcept
{
	T x = {};

	auto [ bits, y ] = make_bits_array<T>(order);

	for(int i=0; i<util::number_of_bits<T>(); ++i){
		x = util::set_bit(x, i, bits[i], order);
		if(util::get_bit(x, i, order) != bits[i])
			return false;
	}

	return x == y;
}

template<class T>
constexpr auto test_type() noexcept
{
	using namespace util;
	constexpr auto r = bit_order::rightmost{};
	constexpr auto l = bit_order::leftmost{};

	STATIC_REQUIRE(all_is<T>(set_all<T>(r), T{1}, r));
	STATIC_REQUIRE(all_is<T>(set_all<T>(l), T{1}, l));

	STATIC_REQUIRE(clear_all<T>(r) == T{0});
	STATIC_REQUIRE(clear_all<T>(l) == T{0});

	STATIC_REQUIRE(test_set_and_get<T>(r));
	STATIC_REQUIRE(test_set_and_get<T>(l));
}

TEST_CASE("number of bits", "[util]")
{
	STATIC_REQUIRE(util::number_of_bits<int8_t>() == 8);
	STATIC_REQUIRE(util::number_of_bits<uint8_t>() == 8);
	STATIC_REQUIRE(util::number_of_bits<int16_t>() == 16);
	STATIC_REQUIRE(util::number_of_bits<uint16_t>() == 16);
	STATIC_REQUIRE(util::number_of_bits<int32_t>() == 32);
	STATIC_REQUIRE(util::number_of_bits<uint32_t>() == 32);
	STATIC_REQUIRE(util::number_of_bits<int64_t>() == 64);
	STATIC_REQUIRE(util::number_of_bits<uint64_t>() == 64);
	STATIC_REQUIRE(util::number_of_bits<bool>() == 1);
}

TEST_CASE("set bit tests", "[util]")
{

	test_type<int8_t>();
	test_type<uint8_t>();
	test_type<int16_t>();
	test_type<uint16_t>();
	test_type<int32_t>();
	test_type<uint32_t>();
	test_type<int64_t>();
	test_type<uint64_t>();

	STATIC_REQUIRE(util::set_bit(0b0000, 0) == 0b0001);
	STATIC_REQUIRE(util::set_bit(0b0000, 1) == 0b0010);
	STATIC_REQUIRE(util::set_bit(0b0000, 2) == 0b0100);
	STATIC_REQUIRE(util::set_bit(0b0000, 3) == 0b1000);

	STATIC_REQUIRE(util::set_bit(0b1111, 0) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 2) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 3) == 0b1111);

	STATIC_REQUIRE(util::set_bit(0b000000000000,  8) == 0b000100000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000,  9) == 0b001000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 10) == 0b010000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 11) == 0b100000000000);

	STATIC_REQUIRE(util::set_bit(0b111111111111, 0) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 2) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 3) == 0b111111111111);
}

TEST_CASE("clear bit tests", "[util]")
{
	STATIC_REQUIRE(util::clear_bit(0b1111, 0) == 0b1110);
	STATIC_REQUIRE(util::clear_bit(0b1111, 1) == 0b1101);
	STATIC_REQUIRE(util::clear_bit(0b1111, 2) == 0b1011);
	STATIC_REQUIRE(util::clear_bit(0b1111, 3) == 0b0111);

	STATIC_REQUIRE(util::clear_bit(0b0000, 0) == 0b0000);
	STATIC_REQUIRE(util::clear_bit(0b0000, 1) == 0b0000);
	STATIC_REQUIRE(util::clear_bit(0b0000, 2) == 0b0000);
	STATIC_REQUIRE(util::clear_bit(0b0000, 3) == 0b0000);

	STATIC_REQUIRE(util::clear_bit(0b111111111111,  8) == 0b111011111111);
	STATIC_REQUIRE(util::clear_bit(0b111111111111,  9) == 0b110111111111);
	STATIC_REQUIRE(util::clear_bit(0b111111111111, 10) == 0b101111111111);
	STATIC_REQUIRE(util::clear_bit(0b111111111111, 11) == 0b011111111111);

	STATIC_REQUIRE(util::clear_bit(0b000000000000, 0) == 0b000000000000);
	STATIC_REQUIRE(util::clear_bit(0b000000000000, 1) == 0b000000000000);
	STATIC_REQUIRE(util::clear_bit(0b000000000000, 2) == 0b000000000000);
	STATIC_REQUIRE(util::clear_bit(0b000000000000, 3) == 0b000000000000);
}

TEST_CASE("set bit to tests", "[util]")
{
	STATIC_REQUIRE(util::set_bit(0b0000, 0, 1) == 0b0001);
	STATIC_REQUIRE(util::set_bit(0b0000, 1, 1) == 0b0010);
	STATIC_REQUIRE(util::set_bit(0b0000, 2, 1) == 0b0100);
	STATIC_REQUIRE(util::set_bit(0b0000, 3, 1) == 0b1000);

	STATIC_REQUIRE(util::set_bit(0b1111, 0, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 1, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 2, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 3, 1) == 0b1111);

	STATIC_REQUIRE(util::set_bit(0b000000000000,  8, 1) == 0b000100000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000,  9, 1) == 0b001000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 10, 1) == 0b010000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 11, 1) == 0b100000000000);

	STATIC_REQUIRE(util::set_bit(0b111111111111, 0, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 1, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 2, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 3, 1) == 0b111111111111);

	STATIC_REQUIRE(util::set_bit(0b1111, 0, 0) == 0b1110);
	STATIC_REQUIRE(util::set_bit(0b1111, 1, 0) == 0b1101);
	STATIC_REQUIRE(util::set_bit(0b1111, 2, 0) == 0b1011);
	STATIC_REQUIRE(util::set_bit(0b1111, 3, 0) == 0b0111);

	STATIC_REQUIRE(util::set_bit(0b0000, 0, 0) == 0b0000);
	STATIC_REQUIRE(util::set_bit(0b0000, 1, 0) == 0b0000);
	STATIC_REQUIRE(util::set_bit(0b0000, 2, 0) == 0b0000);
	STATIC_REQUIRE(util::set_bit(0b0000, 3, 0) == 0b0000);

	STATIC_REQUIRE(util::set_bit(0b111111111111,  8, 0) == 0b111011111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111,  9, 0) == 0b110111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 10, 0) == 0b101111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 11, 0) == 0b011111111111);

	STATIC_REQUIRE(util::set_bit(0b000000000000, 0, 0) == 0b000000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 1, 0) == 0b000000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 2, 0) == 0b000000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 3, 0) == 0b000000000000);

}

TEST_CASE("get bits test", "[util]")
{
	STATIC_REQUIRE(util::get_bit(0b0110, 0) == 0);
	STATIC_REQUIRE(util::get_bit(0b0110, 1) == 1);
	STATIC_REQUIRE(util::get_bit(0b0110, 2) == 1);
	STATIC_REQUIRE(util::get_bit(0b0110, 3) == 0);

	STATIC_REQUIRE(util::get_bit(0b11110010, 0) == 0);
	STATIC_REQUIRE(util::get_bit(0b11110010, 1) == 1);
	STATIC_REQUIRE(util::get_bit(0b11110010, 2) == 0);
	STATIC_REQUIRE(util::get_bit(0b11110010, 3) == 0);
	STATIC_REQUIRE(util::get_bit(0b11110010, 4) == 1);
	STATIC_REQUIRE(util::get_bit(0b11110010, 5) == 1);
	STATIC_REQUIRE(util::get_bit(0b11110010, 6) == 1);
	STATIC_REQUIRE(util::get_bit(0b11110010, 7) == 1);
}

