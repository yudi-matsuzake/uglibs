/** @file point.hpp
  * generic point definition
  */

#pragma once

#include <concepts>
#include <ranges>
#include <algorithm>
#include <array>
#include <functional>
#include <type_traits>

#include "util/misc-adaptors.hpp"
#include "util/misc.hpp"

#include "gmt/point-operations.hpp"

namespace gmt{

/**
  * the type member of this class will be `T` only if `T` is not void
	* otherwise the type member will be DefaultType
	*/
template<class T, class DefaultType>
struct defaulted_type{
	using type = std::conditional_t<std::is_same_v<T, void>, DefaultType, T>;
};

template<
	class T,
	uint64_t N,
	class OperationOutputType = void>
class point :	public std::array<T, N>,
		public point_operations<
			typename defaulted_type<OperationOutputType, point<T, N>>::type>
{
public:
	using this_type = point<T, N, OperationOutputType>;
	using element_type = T;

	constexpr point() = default;
	constexpr point(this_type&) = default;
	constexpr point(this_type&&) = default;
	constexpr point(this_type const&) = default;

	constexpr this_type& operator=(this_type&&)	= default;
	constexpr this_type& operator=(this_type const&)= default;

	template<class ... Ts>
	constexpr point(Ts&& ... args)
		requires (std::is_same_v<std::decay_t<Ts>, T> && ...)
		: std::array<T, N>{ args ...  }
	{}

};

template<class ToPointType, class PointType>
constexpr auto point_convert(PointType&& p)
{
	namespace rgs = std::ranges;
	using T = typename ToPointType::element_type;

	ToPointType q;
	rgs::transform(p, begin(q), util::make_static_cast<T>());
	return q;
}

template<class PointType>
constexpr auto make_point_converter()
{
	return [](auto&& p)
	{
		return point_convert<PointType>(p);
	};
}

}
