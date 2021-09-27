#pragma once

#include <iostream>

#include "util/integer-concepts.hpp"
#include "util/static-string.hpp"
#include "util/variant.hpp"
#include "util/meta.hpp"
#include "util/underlying-integer.hpp"

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

template<class T>
struct is_specialization_of_integer : std::false_type {};

template<uint32_t N, class S>
struct is_specialization_of_integer<integer<N, S>>: std::true_type {};

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
	&& detail::is_specialization_of_integer<T>::value
	&& requires(T v)
{
	typename T::type;
	{ T::n_bits } -> std::convertible_to<uint32_t>;
};

template<class T>
concept arbitrary_integer_or_integral = arbitrary_integer<T>
	|| std::integral<T>;

} // end of namespace util

template<util::arbitrary_integer T>
class std::numeric_limits<T>{
public:
	using underlying_integer_t = typename util::underlying_integer<
		T::n_bits, typename T::signess, util::mutable_flag>::type;

	static constexpr auto bits_per_underlying_integer = std::numeric_limits<
		std::make_unsigned_t<underlying_integer_t>
		>::digits;

	static constexpr underlying_integer_t min()
		requires T::is_unsigned
	{
		return 0;
	}

	static constexpr underlying_integer_t max()
		requires T::is_unsigned
	{
		auto n = bits_per_underlying_integer - T::n_bits;
		underlying_integer_t x = ~underlying_integer_t{0};
		x <<= n;
		x >>= n;
		return x;
	}

	static constexpr underlying_integer_t max()
		requires T::is_signed
	{
		using tighter_integer_t = util::integer<
			T::n_bits - 1,
			util::unsigned_flag>;

		return std::numeric_limits<tighter_integer_t>::max();
	}

	static constexpr underlying_integer_t min()
		requires T::is_signed
	{
		using tighter_integer_t = util::integer<
			T::n_bits - 1, util::unsigned_flag>;

		return -(std::numeric_limits<tighter_integer_t>::max() + 1);
	}

};
