#pragma once
#include <cmath>
#include <type_traits>

#include "util/misc-adaptors.hpp"
#include "util/misc.hpp"
#include "ia/interval.hpp"

namespace ia{

enum class interval_comparison{
	UNDEFINED,
	SMALLER,
	GREATER,
	INTERSECT
};

/**
  * compare the relation between the interval a to b
  *
  * @see interval_comparison
  */
template<class T, class U>
constexpr interval_comparison compare(
	interval<T> const& a,
	interval<U> const& b)
{
	if(a.is_empty() || b.is_empty())
		return interval_comparison::UNDEFINED;

	if(a.hi() < b.lo())
		return interval_comparison::SMALLER;

	if(a.lo() > b.hi())
		return interval_comparison::GREATER;

	return interval_comparison::INTERSECT;
}

template<class T, class F>
auto cast(F a)
{
	auto to_type = util::make_static_cast<T>();

	return interval<T>{
		execute_downward(to_type, a),
		execute_upward(to_type, a),
	};
}

template<class T, class F>
auto cast(interval<F> const& a)
{
	auto to_type = util::make_static_cast<T>();

	return interval<T>{
		execute_downward(to_type, a.lo()),
		execute_upward(to_type, a.hi()),
	};
}

template<class T>
auto sqrt(interval<T> const& in)
{
	if(in.is_empty() || in.hi() < T{0}){
		return interval<T>::empty();
	}else if(in.lo() <= T{0}){
		return interval<T>{ T{0}, execute_upward(::sqrt, in.hi()) };
	}

	return interval<T>{
		execute_downward(::sqrt, in.lo()),
		execute_upward(::sqrt, in.hi())
	};
}

template<class T>
constexpr auto abs(interval<T> const& in)
{
	if(in.is_empty())
		return interval<T>::empty();

	auto [ lo, hi ] = util::minmax(std::abs(in.lo()), std::abs(in.hi()));

	return (in.lo() < T{0} && in.hi() > T{0})
		? interval<T>{ T{0}, hi }
		: interval<T>{ lo, hi };
}

template<class T>
constexpr auto min(interval<T> const& a, interval<T> const& b)
{
	if(a.is_empty())
		return b;
	if(b.is_empty())
		return a;

	return interval{ std::min(a.lo(), b.lo()), std::min(a.hi(), b.hi()) };
}

template<class T>
constexpr auto max(interval<T> const& a, interval<T> const& b)
{
	if(a.is_empty())
		return b;
	if(b.is_empty())
		return a;

	return interval{ std::max(a.lo(), b.lo()), std::max(a.hi(), b.hi()) };
}

template<class T>
constexpr auto join(interval<T> const& a, interval<T> const& b)
{
	if(a.is_empty())
		return b;
	else if(b.is_empty())
		return a;

	return interval{
		std::min(a.lo(), b.lo()),
		std::max(a.hi(), b.hi())
	};
}

template<class T>
constexpr auto smaller_or_join(
	interval<T> const& a,
	interval<T> const& b)
{
	switch(compare(a, b)){
	case interval_comparison::SMALLER:	return a;
	case interval_comparison::GREATER:	return b;
	case interval_comparison::INTERSECT:	return join(a, b);
	default:
	case interval_comparison::UNDEFINED:	return interval<T>::empty();
	}
}

template<class T>
constexpr auto greater_or_join(
	interval<T> const& a,
	interval<T> const& b)
{
	switch(compare(a, b)){
	case interval_comparison::GREATER:	return a;
	case interval_comparison::SMALLER:	return b;
	case interval_comparison::INTERSECT:	return join(a, b);
	default:
	case interval_comparison::UNDEFINED:	return interval<T>::empty();
	}
}

template<class P, class Q, class T>
auto euclidean_distance(P p_begin, P p_end, Q q_begin, T init)
{
	auto d = init;

	auto q_it = q_begin;

	for(auto&& i : util::range(p_begin, p_end)){
		auto p = T{ i };
		auto q = T{ *q_it };
		auto t = abs(p - q);

		d += t*t;

		q_it++;
	}

	return sqrt(d);
}

/**
  * computes the manhatan distance from a point p and a point q
  */
template<class P, class Q>
auto manhatan_distance(P p_begin, P p_end, Q q_begin)
{
	using p_type = typename std::remove_reference<decltype(*p_begin)>::type;
	using q_type = typename std::remove_reference<decltype(*q_begin)>::type;
	using type = decltype(p_type{} - q_type{});

	interval<type> m{ type{0}, type{0} };

	auto q_it = q_begin;

	for(auto&& i : util::range(p_begin, p_end)){
		auto p = interval{ i };
		auto q = interval{ *q_it };

		m = max(m, abs(p - q));

		q_it++;
	}

	return m;
}

template<class T, class U>
constexpr auto distance(
	interval<T> const& a,
	interval<U> const& b)
{
	using result_interval = interval<decltype(T{} - U{})>;

	switch(ia::compare(a, b)){
	case interval_comparison::GREATER:
		return result_interval{ a.lo() } - b.hi();
	case interval_comparison::SMALLER:
		return result_interval{ b.lo() } - a.hi();
	case interval_comparison::INTERSECT:
		return result_interval{ 0.0 };
	case interval_comparison::UNDEFINED:
	default:
		return result_interval::empty();
	}
}

template<class T, class U, class V>
constexpr auto clamp(
	interval<T> const& x,
	interval<U> const& lo,
	interval<V> const& hi)
{
	if(x.is_empty() || lo.is_empty() || hi.is_empty())
		return interval<T>::empty();

	if(lo.hi() > hi.lo())
		return interval<T>::empty();

	return interval<T>{
		std::clamp(x.lo(), lo.lo(), hi.hi()),
		std::clamp(x.hi(), lo.lo(), hi.hi())
	};
}

} // end of namespace ia
