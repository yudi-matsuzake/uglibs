/** @file views.hpp
  * Implementation of views to help common 
  * objects manipulation
  */

#pragma once

#include <concepts>

#include "range/v3/view.hpp"

namespace util{

namespace rgs = std::ranges;
namespace vws = std::views;

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

struct with_temporary_view{};
static constexpr auto with_temporary = with_temporary_view{};

template<rgs::input_range R>
constexpr rgs::view auto operator|(R&& r, with_temporary_view)
	requires (std::default_initializable<rgs::range_value_t<R>>
		&& std::copy_constructible<rgs::range_value_t<R>>)
{
	using T = rgs::range_value_t<R>;
	return r | vws::transform(
		[tmp = T{}](T&& x) mutable -> T&
		{
			tmp = x;
			return tmp;
		}
	);
}


} // end of namespace util
