#pragma once
#include <concepts>

namespace util{

template<class T, class Q>
concept sum_assignable_from = requires(T& t, Q const& q)
{
	{ t += q } -> std::convertible_to<T&>;
};

template<class T, class Q>
concept sub_assignable_from = requires(T& t, Q const& q)
{
	{ t -= q } -> std::convertible_to<T&>;
};

template<class T, class Q>
concept mul_assignable_from = requires(T& t, Q const& q)
{
	{ t *= q } -> std::convertible_to<T&>;
};

template<class T, class Q>
concept div_assignable_from = requires(T& t, Q const& q)
{
	{ t /= q } -> std::convertible_to<T&>;
};

template<class Derived>
struct arithmetic_friends{

	template<class Q>
	friend constexpr auto operator+(Derived const& a, Q const& b)
		requires (sum_assignable_from<Derived, Q>)
	{
		auto c = a;
		return c += b;
	}

	template<class Q>
	friend constexpr auto operator-(Derived const& a, Q const& b)
		requires (sub_assignable_from<Derived, Q>)
	{
		auto c = a;
		return c -= b;
	}

	template<class Q>
	friend constexpr auto operator*(Derived const& a, Q const& b)
		requires (mul_assignable_from<Derived, Q>)
	{
		auto c = a;
		return c *= b;
	}

	template<class Q>
	friend constexpr auto operator/(Derived const& a, Q const& b)
		requires (div_assignable_from<Derived, Q>)
	{
		auto c = a;
		return c /= b;
	}

};


} // end of namespace util
