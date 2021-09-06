#pragma once
#include <variant>
#include <iterator>
#include <string>
#include <sstream>

#include <tuple>
#include <utility>

namespace util{

namespace detail{

template<class ... V0, class ... V1>
constexpr auto merge_variant(std::variant<V0...>, std::variant<V1...>)
{
	return std::variant<V0..., V1...>{};
}

} // end of namespace detail


// helper type for visit
template<class... Ts> struct visitor : Ts... { using Ts::operator()...; };
template<class... Ts> visitor(Ts...) -> visitor<Ts...>;

// list of types
template<class ... T>
struct list_of_types{};

/**
  * struct to merge variants
  */
template<class...V>
struct merged_variant;

template<class V>
struct merged_variant<V>{
	using type = V;
};

template<class V0, class V1, class ... V2>
struct merged_variant<V0, V1, V2 ...>{
	using type = decltype(
		detail::merge_variant(
			V0{},
			typename merged_variant<V1, V2...>::type{}
		)
	);
};

template<class ... V>
using merged_variant_t = typename merged_variant<V...>::type;

/**
  * converts the variant type V1 to V0
	*/
template<class V0, class V1>
V0 variant_convert(V1&& a)
{
	return std::visit([](auto&& c) -> V0
	{
		if constexpr(std::is_convertible_v<decltype(c), V0>){
			return c;
		}else{
			throw std::runtime_error("Variant converter: could not convert variant");
		}
	}, a);
}

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

template <class T, class F, std::uint64_t ...I>
constexpr void tuple_foreach(T&& tuple, F&& f, std::index_sequence<I...>) {
	((f(std::get<I>(std::forward<T>(tuple)))), ...);
}

template <class T, class F>
constexpr void tuple_foreach(T&& tuple, F&& f) {
	using noref_t = std::remove_reference_t<T>;
	constexpr std::uint64_t N = std::tuple_size<noref_t>::value;
	tuple_foreach(
		std::forward<T>(tuple),
		std::forward<F>(f),
		std::make_index_sequence<N>{}
	);
}

template <class T, class F, std::uint64_t ...I>
constexpr auto tuple_transform(T&& tuple, F&& f, std::index_sequence<I...>) {
	return std::make_tuple(f(std::get<I>(std::forward<T>(tuple))) ... );
}

template <class T, class F>
constexpr auto tuple_transform(T&& tuple, F&& f) {
	using noref_t = std::remove_reference_t<T>;
	constexpr std::uint64_t N = std::tuple_size<noref_t>::value;

	return tuple_transform(
		std::forward<T>(tuple),
		std::forward<F>(f),
		std::make_index_sequence<N>{}
	);
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
