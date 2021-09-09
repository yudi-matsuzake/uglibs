#pragma once

#include <iterator>
#include <string>
#include <sstream>
#include <array>

namespace util{

template<class str_iterator, class ext_iterator>
bool ends_with(
	str_iterator str_begin, str_iterator str_end,
	ext_iterator ext_begin, ext_iterator ext_end)
{
	auto str_len = std::distance(str_begin, str_end);
	auto ext_len = std::distance(ext_begin, ext_end);

	if(str_len < ext_len)
		return false;
	if(ext_len == 0)
		return true;

	auto ends = std::prev(str_end, ext_len);
	return std::equal(ends, str_end, ext_begin, ext_end);
}

bool ends_with(std::wstring const& str, std::string const& ext);

bool ends_with(std::string const& str, std::string const& ext);

template<class T>
T parse(char const* str)
{
	std::stringstream ss{str};
	T v;
	ss >> v;
	return v;
}

template<class T>
constexpr auto minmax(T&& a, T&& b)
{
	return (a <= b) ? std::tuple{ a, b} : std::tuple{ b, a };
}

template<class T>
constexpr auto make_static_cast()
{
	return [](auto&& x)
	{
		return static_cast<T>(x);
	};
}
template <class T>
constexpr inline auto hash_combine(const T seed, const T v)
{
	std::hash<T> hasher;
	return seed ^ (hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2));
}

template<class T>
constexpr auto make_interpolation(T min_a, T max_a, T min_b, T max_b)
{
	return [min_a, max_a, min_b, max_b](T x)
	{
		return min_b + (x - min_a) / (max_a - min_a) * (max_b - min_b);
	};
}

template<class T, uint64_t N>
constexpr auto generate_array(auto&& f)
{
	std::array<char, N> a;
	std::generate(begin(a), end(a), f);
	return a;
}

template<uint64_t N>
constexpr auto make_random_sparse_char_array(char from, char to, uint64_t sparsity)
{
	auto randc = [from, siz = to - from]{ return from + rand()%siz; };

	auto rand_char = [=, last = randc()]() mutable
	{
		if(rand()%sparsity != 0){
			return last;
		}else{
			return last = randc();
		}
	};

	return generate_array<char, N>(rand_char);
}

template<uint64_t N>
constexpr auto make_random_indices(uint64_t max_index)
{
	return generate_array<uint64_t, N>(
		[max_index]
		{
			return rand() % max_index;
		}
	);
}

}
