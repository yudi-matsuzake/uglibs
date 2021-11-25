/** @file misc-adaptors.hpp
  * Implementation of adaptors to help common 
  * objects manipulation
  */

#pragma once

#include <ranges>

namespace util{

namespace vws = std::views;

template<class T>
auto numbers(T start, T finish, T step = T{1})
{
	auto n = static_cast<int64_t>(std::ceil((finish - start)/step));
	return vws::iota(0LL, n) | vws::transform(
		[start, step](auto i) -> T
		{
			return start + step*static_cast<T>(i);
		}
	);
}

} // end of namespace util
