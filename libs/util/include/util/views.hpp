/** @file views.hpp
  * Implementation of views to help common 
  * objects manipulation
  */

#pragma once

#include "range/v3/view.hpp"

namespace util{

namespace views = ranges::views;

template<class T>
auto numbers(T start, T finish, T step = T{1})
{
	auto n = static_cast<int64_t>(std::ceil((finish - start)/step));
	return views::iota(0LL, n) | views::transform(
		[start, step](auto i) -> T
		{
			return start + step*static_cast<T>(i);
		}
	);
}

} // end of namespace util
