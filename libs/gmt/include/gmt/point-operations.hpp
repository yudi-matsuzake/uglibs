#pragma once

#include <ranges>

#include "range/v3/algorithm.hpp"

#include "gmt/point-promotion.hpp"

namespace gmt{

namespace detail{

	template<class OutputType,
		std::ranges::input_range R0,
		std::ranges::input_range R1,
		std::copy_constructible BinaryOperator>
	[[nodiscard]] constexpr OutputType
		binary_transform(R0 a, R1 b, BinaryOperator f)
		requires (std::default_initializable<OutputType> &&
				std::weakly_incrementable<
				std::ranges::iterator_t<OutputType>>)
	{
		OutputType r{};
		ranges::transform(a, b, r.begin(), f);
		return r;
	}

	template<class OutputType,
		std::ranges::input_range R0,
		class BinaryOperator>
	[[nodiscard]] constexpr OutputType
		unary_transform(R0 a, BinaryOperator f)
		requires std::default_initializable<OutputType>
	{
		OutputType r;
		std::ranges::transform(a, std::begin(r), f);
		return r;
	}
}

template<class T>
class point_operations{
public:

	template<class Q>
	friend constexpr auto operator+(T const& a, Q const& b)
		requires has_promotion_v<T, Q>
	{
		return detail::binary_transform<point_promotion_t<T, Q>>(
			a, b, [](auto&& x, auto&& y){ return x+y; }
		);
	}

	template<class Q>
	friend constexpr auto operator-(T const& a, Q const& b)
		requires has_promotion_v<T, Q>
	{
		return detail::binary_transform<point_promotion_t<T, Q>>(
			a, b, [](auto&& x, auto&& y){ return x-y; }
		);
	}

	template<class Q>
	friend constexpr auto operator*(T const& a, Q const& b)
		requires has_promotion_v<T, Q>
	{
		return detail::binary_transform<point_promotion_t<T, Q>>(
			a, b, [](auto&& x, auto&& y){ return x*y; }
		);
	}

	template<class Q>
	friend constexpr auto operator/(T const& a, Q const& b)
		requires has_promotion_v<T, Q>
	{
		return detail::binary_transform<point_promotion_t<T, Q>>(
			a, b, [](auto&& x, auto&& y){ return x/y; }
		);
	}

	template<class Scalar>
	friend constexpr T operator+(T const& a, Scalar const& b)
	{
		return detail::unary_transform<T>(
			a,
			[&b](auto&& i){ return i + b; }
		);
	}

	template<class Scalar>
	friend constexpr T operator-(T const& a, Scalar const& b)
	{
		return detail::unary_transform<T>(
			a,
			[&b](auto&& i){ return i - b; }
		);
	}

	template<class Scalar>
	friend constexpr T operator*(T const& a, Scalar const& b)
	{
		return detail::unary_transform<T>(
			a,
			[&b](auto&& i){ return i * b; }
		);
	}

	template<class Scalar>
	friend constexpr T operator/(T const& a, Scalar const& b)
	{
		return detail::unary_transform<T>(
			a,
			[&b](auto&& i){ return i / b; }
		);
	}

	friend constexpr T operator-(T const& a)
	{
		return a*-1;
	}

	template<class U>
	friend T& operator+=(T& a, U const& b)
	{
		a = a + b;
		return a;
	}

	template<class U>
	friend T& operator-=(T& a, U const& b)
	{
		a = a - b;
		return a;
	}

	template<class U>
	friend T& operator*=(T& a, U const& b)
	{
		a = a * b;
		return a;
	}

	template<class U>
	friend T& operator/=(T& a, U const& b)
	{
		a = a / b;
		return a;
	}

	template<class Scalar>
	static constexpr T all(Scalar&& s)
	{
		T r;
		std::ranges::fill(r, s);
		return r;
	}
};

} // end of namespace gmt
