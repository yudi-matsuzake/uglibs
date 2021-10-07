#pragma once

#include <iostream>

#include "util/integer-concepts.hpp"
#include "util/static-string.hpp"
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

template<>
struct integer<8, unsigned_flag> : integer_common<8, unsigned_flag>{
	using type = uint8_t;
};

template<>
struct integer<8, signed_flag> : integer_common<8, signed_flag>{
	using type = int8_t;
};

template<>
struct integer<16, unsigned_flag> : integer_common<16, unsigned_flag>{
	using type = uint16_t;
};

template<>
struct integer<16, signed_flag> : integer_common<16, signed_flag>{
	using type = int16_t;
};

template<>
struct integer<32, unsigned_flag> : integer_common<32, unsigned_flag>{
	using type = uint32_t;
};

template<>
struct integer<32, signed_flag> : integer_common<32, signed_flag>{
	using type = int32_t;
};

template<>
struct integer<64, unsigned_flag> : integer_common<64, unsigned_flag>{
	using type = uint64_t;
};

template<>
struct integer<64, signed_flag> : integer_common<64, signed_flag>{
	using type = int64_t;
};

template<class T>
struct is_specialization_of_integer : std::false_type {};

template<uint32_t N, class S>
struct is_specialization_of_integer<integer<N, S>>: std::true_type {};

} // end of namespace detail

template<uint32_t N, signess S>
using integer = typename detail::integer<N, S>::type;

template<uint32_t N>
using signed_integer = integer<N, signed_flag>;

template<uint32_t N>
using unsigned_integer = integer<N, unsigned_flag>;

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

template<util::arbitrary_integer_or_integral T>
struct integer_info;

template<util::arbitrary_integer T>
struct integer_info<T>{
	static auto constexpr n_bits = T::n_bits;
	using mutability = util::to_mutability_t<T>;
	using signess = typename T::signess;
	static constexpr auto type_name = T::type_name();
};

template<std::integral T>
struct integer_info<T>{
	static auto constexpr n_bits = util::number_of_bits<T>();
	using mutability = util::to_mutability_t<T>;
	using signess = util::to_signess_t<T>;
	using integer = detail::integer<n_bits, signess>;
	static constexpr auto type_name = integer::type_name();
};

} // end of namespace util

template<util::arbitrary_integer T>
class std::numeric_limits<T>{
public:
	using underlying_integer_t = typename util::underlying_integer<
		T::n_bits, typename T::signess, util::mutable_flag>::type;

	static constexpr auto bits_per_underlying_integer = std::numeric_limits<
		std::make_unsigned_t<underlying_integer_t>
		>::digits;

	static constexpr bool is_signed = T::is_signed;
	static constexpr bool is_unsigned = T::is_unsigned;

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
