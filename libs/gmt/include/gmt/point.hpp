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

template<class T, class DefaultType>
using defaulted_type_t = typename defaulted_type<T, DefaultType>::type;

template<
	class T,
	uint64_t N,
	class OperationOutputType = void>
class point :	public std::array<T, N>,
		public point_operations<
			defaulted_type_t<OperationOutputType, point<T, N>>>
{
public:
	using point_type = point<T, N, OperationOutputType>;
	using element_type = T;
	using operation_output_type = defaulted_type_t<
		OperationOutputType, point<T, N>>;

	static constexpr auto dim = N;

	constexpr point() = default;
	constexpr point(point_type&) = default;
	constexpr point(point_type&&) = default;
	constexpr point(point_type const&) = default;

	constexpr point_type& operator=(point_type&&)	= default;
	constexpr point_type& operator=(point_type const&)= default;

	template<class ... Ts>
	explicit constexpr point(Ts&& ... args)
		requires (std::is_same_v<std::decay_t<Ts>, T> && ...)
		: std::array<T, N>{ args ...  }
	{}

	constexpr point(T const (&a)[N]) noexcept
	{
		for(auto i=0UL; i<N; ++i)
			(*this)[i] = a[i];
	}

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

template<class Point>
using point_type = typename std::decay_t<Point>::point_type;

template<class Point>
using element_type = typename std::decay_t<Point>::element_type;

template<class Point>
using operation_output_type = typename
	std::decay_t<Point>::operation_output_type;

template<class Point>
concept pointlike = requires(Point&& p)
{
	typename element_type<Point>;
	typename point_type<Point>;
	typename operation_output_type<Point>;
	{ Point::dim } -> std::convertible_to<uint64_t>;
} && std::is_base_of_v<point_type<Point>, Point>;

}
