#pragma once

#include <string_view>

#include "misc.hpp"

/**
  * @file integers.hpp defines integers with abstract types
  *
  * note that most of these types are opaque
  *
  * these types are useful in containers and others structures
  * that optimize time
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
struct integer : uninstantiable {
	using type = integer<N, S>;
	using signess = S;
	static constexpr auto n_bits = N;
};

template<>
struct integer<8, signed_flag>{
	using type = int8_t;
	using signess = signed_flag;
	static constexpr auto n_bits = 8;
};

template<>
struct integer<8, unsigned_flag>{
	using type = int8_t;
	using signess = unsigned_flag;
	static constexpr auto n_bits = 8;
};

template<>
struct integer<16, signed_flag>{
	using type = int16_t;
	using signess = signed_flag;
	static constexpr auto n_bits = 16;
};

template<>
struct integer<16, unsigned_flag>{
	using type = uint16_t;
	using signess = unsigned_flag;
	static constexpr auto n_bits = 16;
};

template<>
struct integer<32, signed_flag>{
	using type = int32_t;
	using signess = signed_flag;
	static constexpr auto n_bits = 32;
};

template<>
struct integer<32, unsigned_flag>{
	using type = uint32_t;
	using signess = unsigned_flag;
	static constexpr auto n_bits = 32;
};

template<>
struct integer<64, signed_flag>{
	using type = int64_t;
	using signess = signed_flag;
	static constexpr auto n_bits = 64;
};

template<>
struct integer<64, unsigned_flag>{
	using type = uint64_t;
	using signess = unsigned_flag;
	static constexpr auto n_bits = 64;
};

} // end of namespace detail

template<int N>
using signed_integer = typename detail::integer<N, signed_flag>::type;

template<int N>
using unsigned_integer = typename detail::integer<N, unsigned_flag>::type;

} // end of namespace util
