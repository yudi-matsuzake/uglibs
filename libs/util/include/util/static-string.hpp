#pragma once

#include <array>
#include <iostream>

#include "util/misc.hpp"

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
		for(auto i = 0UL; i < ss.size(); ++i)
			ss[i] = (*this)[S + i];
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
	for(auto i=0UL; (i + 1UL) < a.size(); ++i)
		result[i] = a[i];

	for(auto i=0UL; i<b.size(); ++i)
		result[a.size() - 1 + i] = b[i];

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
	for(auto i=0UL; i<N; ++i)
		if(a[i] != b[i])
			return false;
	return true;
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
	for(auto i=0UL; i<s.size(); ++i)
		out << s[i];
	return out;
}

template<uint64_t N>
class static_string : public basic_static_string<char, N> {
public:
	using basic_static_string<char, N>::basic_static_string;
};

template<uint64_t N>
static_string(char const (&)[N]) -> static_string<N>;

template<uint64_t N>
constexpr auto to_static_string()
{
	constexpr auto n_digits = number_of_digits(N, 10UL);
	static_string<n_digits + 1> a;

	auto make_next = [n = N]() mutable {
		auto const ni = n % 10;
		n /= 10;
		return '0' + ni;
	};

	for(auto i=0UL; (i+1UL) < a.size(); ++i) {
		a[a.size() - i - 2] = make_next();
	}

	a.back() = '\0';

	return a;
}

template<uint64_t N>
constexpr uint64_t from_static_string(static_string<N> const& str)
{
	uint64_t acc = 0;

	for(auto i=0; (i + 1UL) < str.size(); ++i) {
		auto const c = str[str.size() - i - 2];
		uint64_t ci = c - '0';
		acc += ci * pow(10, i);
	}

	return acc;
}

} // end of namespace util
