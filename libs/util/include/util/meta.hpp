#pragma once

#include <cinttypes>
#include <variant>
#include <tuple>
#include <utility>

namespace util{

template <class T, class F, uint64_t ...I>
constexpr void tuple_foreach(T&& tuple, F&& f, std::index_sequence<I...>) {
	((f(std::get<I>(std::forward<T>(tuple)))), ...);
}

template <class T, class F>
constexpr void tuple_foreach(T&& tuple, F&& f) {
	using noref_t = std::remove_reference_t<T>;
	constexpr uint64_t N = std::tuple_size<noref_t>::value;
	tuple_foreach(
		std::forward<T>(tuple),
		std::forward<F>(f),
		std::make_index_sequence<N>{}
	);
}

template <class T, class F, uint64_t ...I>
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
	constexpr uint64_t N = std::tuple_size<noref_t>::value;

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

namespace detail{

template<class ... T, uint64_t ... I>
constexpr auto tuple_reverse(
	std::tuple<T...> const& t,
	std::integer_sequence<uint64_t, I ...>) noexcept
{
	using tuple_type = std::tuple<T...>;
	constexpr auto tuple_size = std::tuple_size_v<tuple_type>;

	using reverse_tuple_t = std::tuple<
		std::tuple_element_t<tuple_size - I - 1, tuple_type> ...
	>;

	reverse_tuple_t r = { std::get<tuple_size - I - 1>(t) ...  };

	return r;
}

} // end of namespace detail

/**
  * reverses a tuple $t = (T_0, T_1, ... T_{n-1}$ to
  * $(T_{n-1}, T_{n-2}, ..., T_1, T_0}$
  */
template<class...T>
constexpr auto tuple_reverse(std::tuple<T...> const& t) noexcept
{
	return detail::tuple_reverse(t, std::make_index_sequence<sizeof...(T)>{});
}


template<class T, template<class...> class Primary>
struct is_specialization_of : std::false_type {};

template<template <class...> class Primary, class ... Args>
struct is_specialization_of<Primary<Args...>, Primary>  : std::true_type {};

template<class T, template<class...> class Primary>
constexpr bool is_specialization_of_v = is_specialization_of<T, Primary>::value;

// queue

template<class T>
using integer_queue_empty = std::integer_sequence<T>;

template<class T, T ... elements>
constexpr T integer_queue_type(std::integer_sequence<T, elements...>);

template<class Q>
using integer_queue_type_t = decltype(integer_queue_type(std::declval<Q>()));

template<class T, T e, T ... elements>
constexpr auto integer_queue_push_back(std::integer_sequence<T, elements...>)
	-> std::integer_sequence<T, elements ..., e>;

template<class Q, integer_queue_type_t<Q> e>
using integer_queue_push_back_t = decltype(
	integer_queue_push_back<
		integer_queue_type_t<Q>,
		e>(Q{})
);

template<class T, T e, T ... elements>
constexpr auto integer_queue_push_front(std::integer_sequence<T, elements...>)
	-> std::integer_sequence<T, e, elements ...>;

template<class Q, integer_queue_type_t<Q> e>
using integer_queue_push_front_t = decltype(
	integer_queue_push_front<
		integer_queue_type_t<Q>,
		e>(Q{})
);

} // end of namespace util
