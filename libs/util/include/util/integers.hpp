#pragma once

#include <string_view>

#include "util/misc.hpp"
#include "util/static-string.hpp"
#include "util/variant.hpp"
#include "util/meta.hpp"

/**
  * @file integers.hpp defines integers with abstract types
  *
  * note that most of these types are opaque
  *
  * these types are useful in containers and others structures
  * that perform optimizations for integers with arbitrary number of
  * bits
  */

namespace util{

struct signed_flag   : uninstantiable{};
struct unsigned_flag : uninstantiable {};

struct const_flag : uninstantiable {};
struct mutable_flag : uninstantiable {};

template<class T>
using to_signess_t = std::conditional_t<
	std::is_signed_v<T>,
	util::signed_flag,
	util::unsigned_flag
>;

template<class T>
using to_mutability_t = std::conditional_t<
	std::is_const_v<T>,
	util::const_flag,
	util::mutable_flag
>;

template<class T>
concept signess = std::is_same_v<T, signed_flag>
	|| std::is_same_v<T, unsigned_flag>;

template<class T>
concept mutability = std::is_same_v<T, const_flag>
	|| std::is_same_v<T, mutable_flag>;

namespace detail{

template<uint32_t N, signess S>
struct integer_common{
	static_assert(
		N >= 1 && N <= 64,
		"only support at least 1 to 64 bit integers");

	static constexpr bool is_signed = std::is_same_v<S, signed_flag>;
	static constexpr bool is_unsigned = std::is_same_v<S, unsigned_flag>;

	static constexpr auto n_bits = N;
	using signess = S;

	static constexpr auto type_name()
		requires is_unsigned
	{
		return "uint" + to_static_string<N>();
	}

	static constexpr auto type_name()
		requires is_signed
	{
		return "int" + to_static_string<N>();
	}
};

template<uint32_t N, signess S>
struct integer : integer_common<N, S> {
	using type = integer<N, S>;
};

template<>
struct integer<8, signed_flag>
	: integer_common<8, signed_flag> {
	using type = int8_t;
};

template<>
struct integer<8, unsigned_flag>
	: integer_common<8, unsigned_flag> {
	using type = uint8_t;
};

template<>
struct integer<16, signed_flag>
	: integer_common<16, signed_flag> {
	using type = int16_t;
};

template<>
struct integer<16, unsigned_flag>
	: integer_common<16, unsigned_flag> {
	using type = uint16_t;
};

template<>
struct integer<32, signed_flag>
	: integer_common<32, signed_flag> {
	using type = int32_t;
};

template<>
struct integer<32, unsigned_flag>
	: integer_common<32, unsigned_flag> {
	using type = uint32_t;
};

template<>
struct integer<64, signed_flag>
	: integer_common<64, signed_flag> {
	using type = int64_t;
};

template<>
struct integer<64, unsigned_flag>
	: integer_common<64, unsigned_flag> {
	using type = uint64_t;
};

template<signess S, uint32_t ... ints>
constexpr auto make_variant_integer(std::integer_sequence<uint32_t, ints...>)
	-> std::variant<integer<ints, S> ... >;

template<signess S, class IntegerSequence>
using variant_integer_t = decltype(make_variant_integer<S>(IntegerSequence{}));

using variant_integer_base_t =
	merged_variant_t<
		variant_integer_t<
			signed_flag, make_integer_range_t<uint32_t, 1, 64>>,
		variant_integer_t<
			unsigned_flag, make_integer_range_t<uint32_t, 1, 64>>
	>;

template<class T, template<int, class> class Ref>
struct is_specialization_of_integer : std::false_type {};

template<uint32_t N, class S>
struct is_specialization_of_integer<integer<N, S>, integer>: std::true_type {};

} // end of namespace detail

template<int N, signess S>
using integer = typename detail::integer<N, S>;

template<int N>
using signed_integer = integer<N, signed_flag>;

template<int N>
using unsigned_integer = integer<N, unsigned_flag>;

struct integer_variant : public detail::variant_integer_base_t
{
	using base_t = detail::variant_integer_base_t;
	using base_t::base_t;
	using base_t::operator=;
};

template<class T>
concept arbitrary_integer = ((T::is_signed ^ T::is_unsigned) == 1)
	&& detail::is_specialization_of_integer<T, integer>::value
	&& requires(T v)
{
	typename T::type;
	{ T::n_bits } -> std::convertible_to<uint32_t>;
};

template<class T>
concept arbitrary_integer_or_integral = arbitrary_integer<T>
	|| std::integral<T>;

} // end of namespace util

/* template<uint32_t N, util::signess S> */
/* class std::numeric_limits<util::integer<N, S>>{ */
/* public: */
/* 	using type = util::integer<N, S>; */

/* 	static constexpr auto max() */
/* 	{ */
/* 		return type::max_value(); */
/* 	} */
/* }; */
