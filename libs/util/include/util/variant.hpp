#pragma once

#include <variant>
#include <stdexcept>

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
			throw std::runtime_error(
				"Variant converter: could not convert variant");
		}
	}, a);
}

/*
 * sums the value x to every number of the sequence
 */
template<class T, T X, T ... ints>
constexpr auto sum_to_the_sequence(std::integer_sequence<T, ints...>)
{
	return std::integer_sequence<T, ints+X...>{};
}

// integer range
template<class T, T Begin, T Size>
constexpr auto make_integer_range()
{
	return sum_to_the_sequence<T, Begin>(
		std::make_integer_sequence<T, Size>{}
	);
}

template<class T, T Begin, T Size>
using make_integer_range_t = decltype(
	make_integer_range<T, Begin, Size>());

} // end of namespace util
