/** @file container-operations.hpp
  * This file define and implements various generic operations in containers
  */

#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>

#include "util/misc-adaptors.hpp"

namespace util::cop{

namespace ranges = std::ranges;

/* template<class I, class S, class O> */
/* void mult(I begin, I end, S const& scalar, O output) */
/* { */
/* 	for(auto&& i : util::range(begin, end)) */
/* 		*output++ = i * scalar; */
/* } */

/* template<class I, class S, class O> */
/* void div(I begin, I end, S const& scalar, O output) */
/* { */
/* 	for(auto&& i : util::range(begin, end)) */
/* 		*output++ = i / scalar; */
/* } */

/* template<class I, class S, class O> */
/* void sum(I begin, I end, S const& scalar, O output) */
/* { */
/* 	for(auto&& i : util::range(begin, end)) */
/* 		*output++ = i + scalar; */
/* } */

/**
  * computes the square distance from p to q of type T
  * is assumed that p and q are the same size/dimension
  */
template<class P, class Q>
double squared_distance(P p_begin, P p_end, Q q_begin)
{
	auto squared_difference = [](auto&& a, auto&& b)
	{
		auto diff = a - b;
		return diff*diff;
	};

	auto accumulate = [](auto&& init, auto&& acc)
	{
		init += acc;
		return init;
	};

	return std::inner_product(
		p_begin,
		p_end,
		q_begin,
		0.0,
		accumulate,
		squared_difference
	);
}

/**
  * computes the euclidean distance from p to q of type T
  */
template<class P, class Q>
double euclidean_distance(P p_begin, P p_end, Q q_begin)
{
	return std::sqrt(squared_distance(p_begin, p_end, q_begin));
}

template<class I>
void normalize(I begin, I end)
{
	auto s = std::accumulate(
		begin, end,
		0.0, 
		[](auto x, auto y)
		{
			return std::abs(x) + std::abs(y);
		}
	);

	div(begin, end, s, begin);
}


template<class I>
struct print{
public:

	print(I a_begin, I a_end)
		: m_begin_it(a_begin),
		  m_end_it(a_end)
	{}

	print& begin(std::string const& a_begin)
	{
		m_begin = a_begin;
		return *this;
	}

	print& end(std::string const& a_end)
	{
		m_end = a_end;
		return *this;
	}

	print& sep(std::string const& a_sep)
	{
		m_sep = a_sep;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, print const& c)
	{

		auto it_begin	= c.m_begin_it;
		auto it_end	= c.m_end_it;

		if(std::distance(it_begin, it_end)){
			auto last = it_end - 1;

			out << c.m_begin;

			for(auto&& i : ranges::subrange(it_begin, last))
				out << i << c.m_sep;

			out << *last <<  c.m_end;
		}

		return out;
	}

protected:
	I m_begin_it;
	I m_end_it;

	std::string m_begin = "";
	std::string m_sep = " ";
	std::string m_end = "";
};

}
