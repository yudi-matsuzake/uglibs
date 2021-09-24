#pragma once

#include <string_view>

#include "util/misc.hpp"
#include "util/static-string.hpp"

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
concept signess = std::is_same_v<T, signed_flag>
	|| std::is_same_v<T, unsigned_flag>;

template<class T>
concept mutability = std::is_same_v<T, const_flag>
	|| std::is_same_v<T, mutable_flag>;

namespace detail{

template<uint32_t N, signess S>
struct integer_common{
	static_assert(N <= 64, "only support until 64 bit integers");

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
struct integer : uninstantiable, integer_common<N, S> {
	using type = integer<N, S>;
};

template<>
struct integer<8, signed_flag>
	: uninstantiable,
	  integer_common<8, signed_flag> {
	using type = int8_t;
};

template<>
struct integer<8, unsigned_flag>
	: uninstantiable,
	  integer_common<8, unsigned_flag> {
	using type = uint8_t;
};

template<>
struct integer<16, signed_flag>
	: uninstantiable,
	  integer_common<16, signed_flag> {
	using type = int16_t;
};

template<>
struct integer<16, unsigned_flag>
	: uninstantiable,
	  integer_common<16, unsigned_flag> {
	using type = uint16_t;
};

template<>
struct integer<32, signed_flag>
	: uninstantiable,
	  integer_common<32, signed_flag> {
	using type = int32_t;
};

template<>
struct integer<32, unsigned_flag>
	: uninstantiable,
	  integer_common<32, unsigned_flag> {
	using type = uint32_t;
};

template<>
struct integer<64, signed_flag>
	: uninstantiable,
	  integer_common<64, signed_flag> {
	using type = int64_t;
};

template<>
struct integer<64, unsigned_flag>
	: uninstantiable,
	  integer_common<64, unsigned_flag> {
	using type = uint64_t;
};

} // end of namespace detail

template<int N>
using signed_integer = typename detail::integer<N, signed_flag>;

template<int N>
using unsigned_integer = typename detail::integer<N, unsigned_flag>;

} // end of namespace util