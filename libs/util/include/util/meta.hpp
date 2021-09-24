#pragma once

#include <variant>
#include <tuple>
#include <utility>

namespace util{

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
constexpr auto tuple_transform(T&& tuple, F&& f, std::index_sequence<I...>)
{
	using tuple_type = std::tuple<
		std::invoke_result_t<
			F, std::tuple_element_t<I, std::remove_reference_t<T>>
		> ... 
	>;
	return tuple_type{ f(std::get<I>(std::forward<T>(tuple))) ... };
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

template<class ... Ts>
using tuple_cat_invoke_result_t = typename std::invoke_result_t<
	decltype(std::tuple_cat<Ts...>),
	Ts...
>;

namespace detail{
	/**
	  * pairwise tuple is a tuple that arranges the elements in a pairwise manner
	  * given a pair-type, for instance:
	  *
	  * for instance:
	  * pairwise_tuple<std::pair, int, double, std::string, float>
	  * ==
	  * std::tuple<std::pair<int, double>, std::pair<std::string, float>>
	  */
	template<template<class...> class P, class ... Ts>
	struct pairwise_tuple_impl;

	template<template<class...> class P>
	struct pairwise_tuple_impl<P>{
		using type = std::tuple<>;
	};

	template<template<class...> class P, class F, class S, class ... Ts>
	struct pairwise_tuple_impl<P, F, S, Ts ... >{
		static_assert(
			sizeof ... (Ts) % 2 == 0,
			"pairwise tuple must have a even number of types"
		);

		using type = tuple_cat_invoke_result_t<
			std::tuple<P<F, S>>,
			typename pairwise_tuple_impl<P, Ts...>::type
		>;
	};
} // end of namespace detail

template<template<class...> class P, class ... Ts>
using pairwise_tuple = typename detail::pairwise_tuple_impl<P, Ts ...>::type;

} // end of namespace util
