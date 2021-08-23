#pragma once

#include "gmt/sphere.hpp"
#include "util/container-operations.hpp"

namespace gmt{

using std::begin;
using std::end;
namespace cop = util::cop;

/**
  * computes the squared distance from a point to another point
  */
template<class T, uint64_t N>
T squared_distance(point<T, N> const& p, point<T, N> const& q)
{
	return cop::squared_distance(begin(p), end(p), begin(q));
}

/**
  * computes the euclidean distance from a point to another point
  */
template<class T, uint64_t N>
T euclidean_distance(point<T, N> const& p, point<T, N> const& q)
{
	return cop::euclidean_distance(begin(p), end(p), begin(q));
}

/**
  * computes the signed euclidean distance from a point to a sphere
  */
template<class T, uint64_t N>
T signed_euclidean_distance(point<T, N> const& p, sphere<T, N> const& s)
{
	return euclidean_distance(p, s.center()) - s.radius();
}

/**
  * computes the clipped distance from a point to a sphere
  */
template<class T, uint64_t N>
T clipped_distance(point<T, N> const& p, sphere<T, N> const& s, T delta)
{
	return std::clamp(signed_euclidean_distance(p, s), -delta, delta);
}

} // end of namespace gmt

