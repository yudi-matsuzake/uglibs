#pragma once

#include <variant>
#include <stdexcept>
#include "util/meta.hpp"

namespace util{


// helper type for visit
template<class... Ts> struct visitor : Ts... {
	using Ts::operator()...;
};
template<class... Ts> visitor(Ts...) -> visitor<Ts...>;

template<class T, class ... Ts>
constexpr auto match(T&& arg, Ts&& ... fs)
{
	return std::visit(
		visitor{ std::forward<Ts>(fs) ... },
		std::forward<T>(arg)
	);
}

template<class T>
struct to_variant{};

template <class... Ts>
struct to_variant<list_of_types<Ts...>> {
	using type = std::variant<Ts...>;
};

template<class T>
using to_variant_t = typename to_variant<T>::type;

/**
  * merge variants
  */
namespace detail{

template<class ... V0, class ... V1>
constexpr auto merge_variant(std::variant<V0...>, std::variant<V1...>)
	-> std::variant<V0..., V1...>;


} // end of namespace detail

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
	}, std::forward<V1>(a));
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

template<class Variant>
constexpr bool variant_equality(Variant&& a, Variant&& b)
{
	return std::visit(
		util::visitor{
			[]<class T>(T&& av, T&& vb){ return av == vb; },
			[](auto&&, auto&&){ return false; }
		},
		a, b
	);
}

template<class T, T Begin, T Size>
using make_integer_range_t = decltype(
	make_integer_range<T, Begin, Size>());

namespace detail {

template<class ... Vs>
using merged_variant_t = typename merged_variant<Vs...>::type;

template<template <auto,class...> class T, auto ... Values>
constexpr auto generate_variant_with(list_of_values<Values...>) noexcept
	-> std::variant<T<Values>...>;

template<template<class,class...> class T, class...Ts>
constexpr auto generate_variant_with(list_of_types<Ts...>) noexcept
	-> std::variant<T<Ts>...>;
} // end of namespace detail

template<template <auto, class...> class T, class ValueList>
using generate_variant_with_v_t = decltype(
	detail::generate_variant_with<T>(std::declval<ValueList>()));

template<template <class, class...> class T, class TypeList>
using generate_variant_with_t_t = decltype(
	detail::generate_variant_with<T>(std::declval<TypeList>()));

namespace detail {

template<template<class, auto, class...> class T, class Type, class ValueList>
struct generate_variant_with_tv_helper {
	template<auto V>
	using pivo = T<Type, V>;
	using type = generate_variant_with_v_t<pivo, ValueList>;
};

template<template<class, auto, class...> class T, class Type, class ValueList>
using generate_variant_with_tv_helper_t = typename generate_variant_with_tv_helper<
	T, Type, ValueList>::type;


template<template<class, auto, class...> class T, class ValueList, class...Ts>
constexpr auto generate_variant_with_tv(list_of_types<Ts...>, ValueList) noexcept
	-> merged_variant_t<
		generate_variant_with_tv_helper_t<T, Ts, ValueList> ...
	>;

} // end of namespace detail

template<template <class, auto, class...> class T, class TypeList, class ValueList>
using generate_variant_with_tv_t = decltype(
  detail::generate_variant_with_tv<T>(std::declval<TypeList>(), std::declval<ValueList>()));

namespace detail {

template<template<auto, class, class...> class T, auto Value, class TypeList>
struct generate_variant_with_vt_helper {
	template<class Type>
	using pivo = T<Value, Type>;
	using type = generate_variant_with_t_t<pivo, TypeList>;
};

template<template<auto, class, class...> class T, auto Value, class TypeList>
using generate_variant_with_vt_helper_t = typename generate_variant_with_vt_helper<
	T, Value, TypeList>::type;


template<template<auto, class, class...> class T, class TypeList, auto...Vs>
constexpr auto generate_variant_with_vt(list_of_values<Vs...>, TypeList) noexcept
	-> merged_variant_t<
		generate_variant_with_vt_helper_t<T, Vs, TypeList> ...
	>;

} // end of namespace detail

template<template <auto, class, class...> class T, class ValueList, class TypeList>
using generate_variant_with_vt_t = decltype(
  detail::generate_variant_with_vt<T>(std::declval<ValueList>(), std::declval<TypeList>()));

template<class First, class ... Ts, class F>
constexpr auto for_each_type(list_of_types<First, Ts...>, F&& f)
{
  f(First{});
  if constexpr (sizeof...(Ts) > 0) {
    for_each_type(list_of_types<Ts...>{}, std::forward<F>(f));
  }
}


} // end of namespace util
