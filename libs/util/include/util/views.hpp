/** @file views.hpp
  * Implementation of views to help common 
  * objects manipulation
  */

#pragma once

#include <concepts>

#include "util/ranges.hpp"

namespace util{



template<class T>
auto numbers(T start, T finish, T step = T{1})
{
	auto n = static_cast<int64_t>(std::ceil((finish - start)/step));
	return rg::v3::vw::iota(0LL, n) | rg::v3::vw::transform(
		[start, step](auto i) -> T
		{
			return start + step*static_cast<T>(i);
		}
	);
}

struct with_temporary_view{};
static constexpr auto with_temporary = with_temporary_view{};

template<rg::input_range R>
constexpr rg::view auto operator|(R&& r, with_temporary_view)
	requires (std::default_initializable<rg::range_value_t<R>>
		&& std::copy_constructible<rg::range_value_t<R>>)
{
	using T = rg::range_value_t<R>;
	return r | rg::vw::transform(
		[tmp = T{}](T&& x) mutable -> T&
		{
			tmp = x;
			return tmp;
		}
	);
}


} // end of namespace util
