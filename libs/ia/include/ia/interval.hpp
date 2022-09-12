#pragma once

#include <type_traits>
#include <iostream>
#include <algorithm>
#include <limits>

#include "util/misc.hpp"
#include "ia/scoped-round-mode.hpp"

namespace ia{

/**
  * this class represents an interval with arithmetic operations
  */
template<class T>
class interval{
public:

	static_assert(
		std::numeric_limits<T>::has_infinity == true,
		"The interval type must have infinities"
	);

	static constexpr auto minus_infinity = -std::numeric_limits<T>::infinity();
	static constexpr auto infinity = std::numeric_limits<T>::infinity();
	static constexpr auto max_finite = std::numeric_limits<T>::max();
	static constexpr auto min_finite = std::numeric_limits<T>::min();

	/// @return a interval that represents all real numbers
	static constexpr interval<T> R();

	/// @return a interval that represents the empty set
	static constexpr interval<T> empty();

	constexpr interval()
		: m_lo{infinity}, m_hi{minus_infinity}
	{}

	constexpr interval(T scalar)
		: m_lo(scalar), m_hi(scalar)
	{}

	constexpr interval(T lo, T hi)
		: m_lo(lo), m_hi(hi)
	{}

	constexpr bool is_empty() const
	{
		return m_lo > m_hi;
	}

	constexpr bool is_full() const
	{
		return (*this) == R();
	}

	constexpr bool is_trivial() const
	{
		return lo() == hi();
	}

	auto midpoint() const
	{
		if(is_empty() || is_full())
			return T{0};
		else if(lo() == hi())
			return lo();
		else if(lo() == minus_infinity)
			return min_finite;
		else if(hi() == infinity)
			return max_finite;
		
		auto l = execute_upward([this]{ return this->lo()/T{2}; });
		auto h = execute_downward([this]{ return this->hi()/T{2}; });
		return l + h;
	}

	auto half_width() const
	{
		if(!is_empty() && !is_trivial()){
			auto m = midpoint();

			auto [ lo, hi ] = execute_upward([&]
			{
				auto l = m - this->lo();
				auto h = this->hi() - m;
				return std::tuple{ l, h };
			});

			return std::max(lo, hi);
		}

		return T{0};
	}

	constexpr auto lo() const
	{
		return m_lo;
	}

	constexpr auto hi() const
	{
		return m_hi;
	}

	friend std::ostream& operator<<(
		std::ostream& out,
		ia::interval<T> const& a)
	{
		out << a.lo() << ' ' << a.hi() << '\n';
		return out;
	}

	friend std::istream& operator>>(
		std::istream& in,
		ia::interval<T>& a)
	{
		T lo, hi;
		in >> lo;
		in >> hi;

		a.m_lo = lo; 
		a.m_hi = hi; 
		return in;
	}

protected:
	T m_lo;
	T m_hi;
};

template<class T>
constexpr interval<T> interval<T>::R()
{
	return interval<T>{ minus_infinity, infinity };
}

template<class T>
constexpr interval<T> interval<T>::empty()
{
	return interval<T>{};
}

/*
 * operations
 */

template<class T>
constexpr auto operator-(interval<T> const& in)
{
	if(in.is_empty())
		return interval<T>::empty();

	auto [ lo, hi ] = util::minmax(-in.lo(), -in.hi());
	return interval<T>{ lo, hi };
}

template<class T>
constexpr auto operator+(interval<T> const& in)
{
	if(in.is_empty())
		return interval<T>::empty();

	auto [ lo, hi ] = util::minmax(+in.lo(), +in.hi());
	return interval<T>{ lo, hi };
}

template<class A, class B>
constexpr auto operator==(interval<A> const& a, interval<B> const& b)
{
	return (a.is_empty() && b.is_empty()) 
		|| (a.lo() == b.lo() && a.hi() == b.hi());
}

template<class A, class B>
constexpr auto operator!=(interval<A> const& a, interval<B> const& b)
{
	return !(a == b);
}

template<class A, class S>
constexpr auto operator==(interval<A> const& a, S scalar)
{
	return a.lo() == scalar && a.hi() == scalar;
}

template<class A, class S>
constexpr auto operator!=(interval<A> const& a, S scalar)
{
	return !(a == scalar);
}

template<class A, class B>
auto operator+(interval<A> const& a, interval<B> const& b)
{
	using type = decltype(A{} + B{});

	if(a.is_empty() || b.is_empty())
		return interval<type>::empty();

	return interval<type>(
		execute_downward([&]{return a.lo() + b.lo();}),
		execute_upward([&]{return a.hi() + b.hi();})
	);
}

template<class A, class B>
auto operator-(interval<A> const& a, interval<B> const& b)
{
	return a + interval{ -b.hi(), -b.lo() };
}


template<class A, class S>
auto operator+(interval<A> const& a, S scalar)
{
	return a + interval{ scalar };
}

template<class A, class S>
auto operator-(interval<A> const& a, S scalar)
{
	return a - interval{ scalar };
}

template<class A, class S>
auto operator*(interval<A> const& a, S scalar)
{
	using type = decltype(A{} + S{});

	if(a.is_empty())
		return interval<type>::empty();

	if(scalar > 0){
		return interval<type>(
			execute_downward([&a, scalar]
			{
				return a.lo()*static_cast<type>(scalar);
			}),
			execute_upward([&a, scalar]{
				return a.hi()*static_cast<type>(scalar);
			})
		);
	}

	if(scalar < 0){
		return interval<type>(
			execute_downward([&a, scalar]
			{
				return a.hi()*static_cast<type>(scalar);
			}),
			execute_upward([&a, scalar]
			{
				return a.lo()*static_cast<type>(scalar);
			})
		);
	}

	return interval<type>{ type{0}, type{0} };
}

template<class A, class B>
auto operator*(interval<A> const& a, interval<B> const& b)
{
	using type = decltype(A{} + B{});
	if(a.is_empty() || b.is_empty())
		return interval<type>::empty();

	if(a == A{0} || b == B{0})
		return interval<type>{ type{0.0}, type{0.0} };

	auto lo = execute_in(round_mode::DOWNWARD, [&]{
		return std::min({
			a.lo()*b.lo(),
			a.lo()*b.hi(),
			a.hi()*b.lo(),
			a.hi()*b.hi()
		});
	});

	auto hi = execute_in(round_mode::UPWARD, [&]{
		return std::max({
			a.lo()*b.lo(),
			a.lo()*b.hi(),
			a.hi()*b.lo(),
			a.hi()*b.hi()
		});
	});

	return interval{ lo, hi };
}

template<class A, class S>
auto operator/(interval<A> const& a, S scalar)
{
	return a / interval{scalar};
}

template<class A, class B>
auto operator/(interval<A> const& a, interval<B> const& b)
{
	using type = decltype(A{} + B{});

	if(a.is_empty() && b.is_empty())
		return interval<type>::empty();

	if(a == A{0})
		return interval{type{0}};

	if(b.lo() >= B{0}){
		type lo, hi;
		if(a.lo() >= A{0}){
			lo = execute_downward([&]{ return a.lo()/b.hi(); });
			hi = execute_upward([&]{ return a.hi()/b.lo(); });
		}else if(a.hi() <= A{0}){
			lo = execute_downward([&]{ return a.lo()/b.lo(); });
			hi = execute_upward([&]{ return a.hi()/b.hi(); });
		}else{
			lo = execute_downward([&]{ return a.lo()/b.lo(); });
			hi = execute_upward([&]{ return a.hi()/b.lo(); });
		}

		return interval{lo, hi};
	}

	if(b.hi() <= B{0}){
		type lo, hi;

		if(a.lo() >= A{0}){
			lo = execute_downward([&]{ return a.hi()/b.hi(); });
			hi = execute_upward([&]{ return a.lo()/b.lo(); });
		}else if(a.hi() <= A{0}){
			lo = execute_downward([&]{ return a.hi()/b.lo(); });
			hi = execute_upward([&]{ return a.lo()/b.hi(); });
		}else{
			lo = execute_downward([&]{ return a.hi()/b.hi(); });
			hi = execute_upward([&]{ return a.lo()/b.hi(); });
		}

		return interval{lo, hi};
	}

	return interval<type>::empty();
}

template<class A, class B>
auto operator+=(interval<A>& a, interval<B> const& b)
{
	a = a + b;
}

template<class A, class B>
auto operator-=(interval<A>& a, interval<B> const& b)
{
	a = a - b;
}

template<class A, class B>
auto operator/=(interval<A>& a, interval<B> const& b)
{
	a = a / b;
}

template<class A, class B>
auto operator*=(interval<A>& a, interval<B> const& b)
{
	a = a * b;
}

template<class A, class S>
auto operator+=(interval<A>& a, S scalar)
{
	a = a + scalar;
}

template<class A, class S>
auto operator-=(interval<A>& a, S scalar)
{
	a = a - scalar;
}

template<class A, class S>
auto operator/=(interval<A>& a, S scalar)
{
	a = a / scalar;
}

template<class A, class S>
auto operator*=(interval<A>& a, S scalar)
{
	a = a * scalar;
}

} // end of namespace ia

template<class T>
struct std::common_type<T, ia::interval<T>> {
	using type = ia::interval<T>;
};

template<class T>
struct std::common_type<ia::interval<T>, T> {
	using type = ia::interval<T>;
};
