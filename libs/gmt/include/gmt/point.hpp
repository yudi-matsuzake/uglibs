/** @file point.hpp
  * generic point definition
  */

#pragma once

#include <concepts>
#include <algorithm>
#include <array>
#include <functional>
#include <type_traits>
#include <ranges>
#include <cmath>

#include "util/views.hpp"
#include "util/misc.hpp"

#include "gmt/point-operations.hpp"

namespace gmt{

namespace rgs = std::ranges;
namespace vws = std::views;

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
	static constexpr auto dim = N;

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
	using T = typename ToPointType::element_type;

	ToPointType q;
	std::ranges::transform(p, begin(q), util::make_static_cast<T>());
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

template<class PointType>
constexpr auto point_ceil(PointType const& p)
{
	PointType r;
	rgs::copy(p | vws::transform([](auto x){ return std::ceil(x); }),
		r.begin()
	);
	return r;
}

template<class PointType>
constexpr auto point_floor(PointType const& p)
{
	PointType r;
	rgs::copy(p | vws::transform([](auto x){ return std::floor(x); }),
		r.begin()
	);
	return r;
}

}
