#pragma once

#include <array>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <iterator>

#include "util/misc.hpp"

#include "range/v3/numeric/accumulate.hpp"

namespace util{

template<class T, uint64_t N>
class basic_static_string : public std::array<T, N> {
public:
	using std::array<T, N>::array;

	constexpr basic_static_string() = default;

	constexpr basic_static_string(T const (&a)[N]) noexcept
		: std::array<T, N>{ std::to_array(a) }
	{}

	template<uint64_t S, uint64_t E>
	constexpr auto substring() const noexcept
		requires (S <= E && S >= 0 && E <= N)
	{
		basic_static_string<T, E - S + 1> ss;
		rgs::copy(this->begin() + S, this->begin() + E, ss.begin());
		ss.back() = '\0';
		return ss;
	}

};

template<class T, uint64_t N, uint64_t M>
constexpr auto operator+(
	basic_static_string<T, N> const& a,
	basic_static_string<T, M> const& b)
{
	basic_static_string<T, N + M - 1> result;
	auto a_str = rgs::subrange(a.begin(), a.end() - 1);
	rgs::copy(a_str, result.begin());
	rgs::copy(b, result.begin() + rgs::size(a_str));
	return result;
}

template<class T, uint64_t N, uint64_t M>
constexpr auto operator+(
	basic_static_string<T, N> const& a,
	const T (&b)[M])
{
	return a + basic_static_string{b};
}

template<class T, uint64_t N, uint64_t M>
constexpr auto operator+(
	const T (&a)[N],
	basic_static_string<T, M> const& b)
{
	return basic_static_string{a} + b;
}

template<class T, uint64_t N>
constexpr auto operator==(
	basic_static_string<T, N> const& a,
	basic_static_string<T, N> const& b)
{
	return rgs::equal(a, b);
}

template<class T, uint64_t N>
constexpr auto operator==(
	basic_static_string<T, N> const& a,
	T const (&b)[N])
{
	return a == basic_static_string{b};
}

template<class T, uint64_t N>
constexpr auto operator==(
	T const (&a)[N],
	basic_static_string<T, N> const& b)
{
	return basic_static_string{a} == b;
}

template<class T, uint64_t N>
constexpr std::ostream& operator<<(
	std::ostream& out,
	basic_static_string<T, N> const& s)
{
	rgs::copy(s, std::ostream_iterator<T>{out, ""});
	return out;
}

template<uint64_t N>
using static_string = basic_static_string<char, N>;

template<uint64_t N>
constexpr auto to_static_string()
{
	constexpr auto n_digits = number_of_digits(N, 10UL);
	static_string<n_digits + 1> a;

	auto n = N;

	a.back() = '\0';

	rgs::generate(
		vws::reverse(a) | vws::drop(1),
		[n]() mutable
		{
			auto const ni = n % 10;
			n /= 10;
			return '0' + ni;
		}
	);
	
	return a;
}

template<uint64_t N>
constexpr uint64_t from_static_string(static_string<N> const& str)
{
	uint64_t acc = 0;

	rgs::for_each(
		vws::reverse(str) | vws::drop(1),
		[i=0, &acc](char c) mutable
		{
			uint64_t ci = c - '0';
			acc += ci * pow(10, i++);
		}
	);

	return acc;
}

} // end of namespace util
