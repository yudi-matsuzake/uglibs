#pragma once

#include <concepts>

namespace util{

/**
  * sets the `n`-th bit of `x` to 1
  */
constexpr
auto set_bit(std::integral auto x, std::integral auto n)
{
	using T = decltype(decltype(x){} | decltype(n){});
	return x | (T{1} << n);
}

/**
  * sets the `n`-th bit of `x` to 0
  */
constexpr
auto clear_bit(std::integral auto x, std::integral auto n)
{
	using T = decltype(decltype(x){} & decltype(n){});
	return x & ~(T{1} << n);
}

/**
  * sets the `n`-th bit of `x` to the last bit of `b`,
  * in other words, set to !!`b` or `b&1`
  */
constexpr
auto set_bit(std::integral auto x, std::integral auto n, std::integral auto b)
{
	b = !!b;
	return clear_bit(x, n) | (b << n);
}

} // end of namespace util
