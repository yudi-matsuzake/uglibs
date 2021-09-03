#pragma once

#include <cstdint>
#include <concepts>
#include <variant>
#include <span>

#include "util/misc.hpp"

namespace util{

struct bit_order{
	struct leftmost{};
	struct rightmost{};
};

using bit_index_order = std::variant<bit_order::leftmost, bit_order::rightmost>;

template<std::integral T>
constexpr auto number_of_bits()
	requires (not std::same_as<T, bool>)
{
	return std::numeric_limits<std::make_unsigned_t<T>>::digits;
}

template<std::same_as<bool> T>
constexpr auto number_of_bits()
{
	return std::numeric_limits<T>::digits;
}

/**
  * sets the (`n` + 1)-th bit of `x` to 1
  *
  * the (`n` + 1)-th bit is computed based on `order`,
  * if `order` is bit_order::leftmost than the first bit is the
  * leftmost
  */
[[nodiscard]] constexpr
auto set_bit(
	std::integral auto x,
	std::integral auto n,
	bit_index_order order = bit_order::rightmost{}) noexcept
{
	using x_type = decltype(x);
	using T = decltype(x_type{} | decltype(n){});

	constexpr auto s = number_of_bits<x_type>() - 1;

	return std::visit(util::visitor{
		[x, n](bit_order::rightmost){ return x | (T{1} << n); },
		[x, n](bit_order::leftmost){ return x | (T{1} << (s - n)); }
	}, order);
}

/**
  * sets the (`n` + 1)-th bit of `x` to 0
  *
  * the (`n` + 1)-th bit is computed based on `order`,
  * if `order` is bit_order::leftmost than the first bit is the
  * leftmost
  */
[[nodiscard]] constexpr
auto clear_bit(
	std::integral auto x,
	std::integral auto n, 
	bit_index_order order = bit_order::rightmost{}) noexcept
{
	using x_type = decltype(x);
	using T = decltype(decltype(x){} & decltype(n){});
	constexpr auto s = number_of_bits<x_type>() - 1;

	return std::visit(util::visitor{
		[&](bit_order::rightmost){ return x & ~(T{1} << n); },
		[&](bit_order::leftmost){ return x & ~(T{1} << (s - n)); }
	}, order);
}

/**
  * sets the (`n` + 1)-th bit to the last bit of `b`
  * in other words, set to !!`b` or `b&1`
  *
  * the (`n` + 1)-th bit is computed based on `order`,
  * if `order` is bit_order::leftmost than the first bit is the
  * leftmost
  */
[[nodiscard]] constexpr
auto set_bit(
	std::integral auto x,
	std::integral auto n,
	std::integral auto bit_value,
	bit_index_order order = bit_order::rightmost{}) noexcept
{

	using x_type = decltype(x);
	constexpr auto s = number_of_bits<x_type>() - 1;

	x_type b = bit_value & 1;

	return std::visit(util::visitor{
		[&](bit_order::rightmost)
		{
			return clear_bit(x, n) | (b << n);
		},
		[&, n=(s - n)](bit_order::leftmost)
		{
			return clear_bit(x, n) | (b << n);
		},
	}, order);
}

/**
  * returns the (`n` + 1)-th bit of `x`
  *
  * the (`n` + 1)-th bit is computed based on `order`,
  * if `order` is bit_order::leftmost than the first bit is the
  * leftmost
  */
[[nodiscard]] constexpr
auto get_bit(
	std::integral auto x,
	std::integral auto n,
	bit_index_order order = bit_order::rightmost{}) noexcept
{
	using x_type = decltype(x);
	constexpr auto s = number_of_bits<x_type>() - 1;

	return std::visit(util::visitor{
		[&](bit_order::rightmost){ return (x >> n) & 1; },
		[&](bit_order::leftmost){ return (x >> (s - n)) & 1; },
	}, order);
}

} // end of namespace util
