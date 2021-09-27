#pragma once
#include <type_traits>

#include "util/bit.hpp"
#include "util/integers.hpp"

namespace util{

/**
  * given a bitsize N, this struct determines which std integers must me used,
  * for instance,
  * underlying_integer<8>::type = int8_t
  * underlying_integer<32>::type = int32_t
  * underlying_integer<32>::type = int32_t
  * underlying_integer<64, unsigned_flag>::type = uint64_t
  */
template<
	uint8_t N,
	util::signess S = util::signed_flag,
	util::mutability M = util::mutable_flag>
struct underlying_integer
{

	static constexpr auto is_signed = std::is_same_v<S, util::signed_flag>;
	static constexpr auto is_unsigned = std::is_same_v<
		S, util::unsigned_flag>;

	static constexpr auto is_const = std::is_same_v<M, util::const_flag>;
	static constexpr auto is_mutable = std::is_same_v<
		M, util::mutable_flag>;

	static_assert(
		N > 0 && N <= util::number_of_bits<uint64_t>(),
		"underlying_integer integer only supports 1 to 64 bits");

	static_assert(
		N > 1 || is_unsigned,
		"underlying_integer does not support binary signed integer");

	static_assert(is_signed xor is_unsigned);
	static_assert(is_const xor is_mutable);

	using unsigned_type = std::conditional_t<
		(N <= util::number_of_bits<uint8_t>()),
		uint8_t,
		std::conditional_t<
			(N <= util::number_of_bits<uint16_t>()),
			uint16_t,
			std::conditional_t<
				(N <= util::number_of_bits<uint32_t>()),
				uint32_t,
				uint64_t
			>
		>
	>;

	using type_with_signess = std::conditional_t<
		is_signed,
		std::make_signed_t<unsigned_type>,
		unsigned_type
	>;

	using type = std::conditional_t<
		is_const,
		std::add_const_t<unsigned_type>,
		type_with_signess
	>;
};


} // end of namespace util
