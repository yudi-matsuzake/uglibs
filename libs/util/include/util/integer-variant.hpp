#pragma once

#include "util/integers.hpp"

namespace util{

namespace detail{

template<util::signess S, uint32_t ... ints>
constexpr auto make_integer_variant(std::integer_sequence<uint32_t, ints...>)
	-> std::variant<util::integer<ints, S> ... >;

template<util::signess S, class ... I>
struct integer_precisions;

template<util::signess S, class I>
struct integer_precisions<S, I>{
	using type = std::conditional_t<
		std::is_same_v<typename util::integer_info<I>::signess, S>,
		std::integer_sequence<uint32_t, util::integer_info<I>::n_bits>,
		std::integer_sequence<uint32_t>
	>;
};

template<util::signess S, class I, class ... Is>
struct integer_precisions<S, I, Is...>{
	using type = std::conditional_t<
		std::is_same_v<typename util::integer_info<I>::signess, S>,
		util::integer_queue_push_front_t<
			typename integer_precisions<S, Is...>::type,
			util::integer_info<I>::n_bits>,
		typename integer_precisions<S, Is...>::type
	>;
};

template<util::signess S, class I, class ... Is>
auto integer_precisions_f(std::variant<I, Is...>&&)
	-> typename integer_precisions<S, I, Is...>::type;

} // end of namespace detail

/**
  * make a variant of integers with sign `S` with precision bits
  * in [`First` ... `Last`]
  */
template<util::signess S, uint32_t First, uint32_t Last>
using make_integer_variant_t = decltype(
	detail::make_integer_variant<S>(
		util::make_integer_range<uint32_t, First, Last - First + 1>()
	)
);

/**
  * make a std::integer_sequence type with all precisions of integers in the 
  * variant `IntegerVariant` with signess equal to `S`
  */
template<util::signess S, class IntegerVariant>
using integer_precisions_t = decltype(
	detail::integer_precisions_f<S>(IntegerVariant{}));

namespace detail{

template<class S, class ... Is>
constexpr auto has_variant_with_sign(std::variant<Is ... >)
{
	return (std::is_same_v<typename integer_info<Is>::signess, S> || ...);
}

template<class IntegerVariant, class S, class T, T first, T ... ints>
constexpr auto make_integer(uint32_t n, std::integer_sequence<T, first, ints ...>)
	-> std::optional<IntegerVariant>
{
	if(n == first)
		return util::integer<first, S>{};
	if constexpr(sizeof...(ints) > 0){
		return make_integer<IntegerVariant, S>(
			n, std::integer_sequence<T, ints...>{});
	}else{
		return std::nullopt;
	}
}

} // end of namespace detail

/**
  * given a variant of integers `IntegerVariant`, returns a type of
  * signed integer with `n` bits such that this type is in the variant
  */
template<class IntegerVariant>
constexpr auto unsigned_integer_from_precision(uint32_t n)
	-> std::optional<IntegerVariant>
{
	return detail::make_integer<IntegerVariant, util::unsigned_flag>(
		n,
		util::integer_precisions_t<
			util::unsigned_flag,
			IntegerVariant
		>{}
	);
}

/**
  * given a variant of integers `IntegerVariant`, returns a type of
  * signed integer with `n` bits such that this type is in the variant
  */
template<class IntegerVariant>
constexpr auto signed_integer_from_precision(uint32_t n)
	-> std::optional<IntegerVariant>
{
	return detail::make_integer<IntegerVariant, util::signed_flag>(
		n,
		util::integer_precisions_t<
			util::signed_flag,
			IntegerVariant
		>{}
	);
}


namespace detail {

template<class IntegerVariant>
auto unsigned_integer_from_string(std::string_view str)
	-> std::optional<IntegerVariant>
{
	auto n = std::strtoul(str.substr(4).data(), nullptr, 10);

	if(n == ULONG_MAX)
		return std::nullopt;

	return unsigned_integer_from_precision<IntegerVariant>(n);
}

template<class IntegerVariant>
auto signed_integer_from_string(std::string_view str)
	-> std::optional<IntegerVariant>
{
	auto n = std::strtoul(str.substr(3).data(), nullptr, 10);

	if(n == ULONG_MAX)
		return std::nullopt;

	return signed_integer_from_precision<IntegerVariant>(n);
}

} // end of namespace detail

/**
  * converts the string to one of the types in `IntegerVariant`
  *
  * for instance, int32  -> integer<32, signed_flag>
  *               uint32 -> integer<32, unsigned_flag>
  *               uint2 -> integer<2, unsigned_flag>
  *
  * if the integer could not be defined or it is not in `IntegerVariant`
  * returns nullopt
  */
template<class IntegerVariant>
auto integer_from_string(std::string_view str)
	-> std::optional<IntegerVariant>
{
	if(str.size() < 4 || str.size() > 6)
		return std::nullopt;

	static constexpr auto has_unsigned = has_variant_with_sign<
		util::unsigned_flag>(IntegerVariant{});

	static constexpr auto has_signed = has_variant_with_sign<
		util::signed_flag>(IntegerVariant{});

	if constexpr (has_unsigned){
		if(str.substr(0, 4) == "uint"){
			return detail::unsigned_integer_from_string<
				IntegerVariant>(str);
		}
	}

	if constexpr (has_signed){
		if(str.substr(0, 3) == "int"){
			return detail::signed_integer_from_string<
				IntegerVariant>(str);
		}
	}

	return std::nullopt;
}

} // end of namespace util
