#pragma once

#include "util/misc.hpp"

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


} // end of namespace util
