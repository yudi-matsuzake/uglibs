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

template<class IntegerVariant, class S, class T, T first, T ... ints>
auto make_integer(uint32_t n, std::integer_sequence<T, first, ints ...>)
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

	if(str.substr(0, 4) == "uint"){
		auto n = std::strtoul(str.substr(4).data(), nullptr, 10);
		return detail::make_integer<IntegerVariant, util::unsigned_flag>(
			n,
			util::integer_precisions_t<
				util::unsigned_flag,
				IntegerVariant
			>{}
		);
	}else if(str.substr(0, 3) == "int"){
		auto n = std::strtoul(str.substr(3).data(), nullptr, 10);
		return detail::make_integer<IntegerVariant, util::signed_flag>(
			n,
			util::integer_precisions_t<
				util::signed_flag,
				IntegerVariant
			>{}
		);
	}

	return std::nullopt;
}

} // end of namespace util
