#pragma once

#include "gmt/point.hpp"
#include "gmt/vector.hpp"

namespace gmt{

/**
  * representation of a hyperplane
  * the hyperplane is the orthogonal hyperplane with the vector
  * versor(`direction`) with distance h to the origin
  */
template<class T, uint64_t N>
struct hyperplane{
	vector<T, N> direction;
	T distance;
};

/**
  * representation of a point with N-dimension embedded in a `M`-dimension space
  */
template<class T, uint64_t N, uint64_t M>
class embedded_point{
public:
	using point_type = point<T, N>;
	using embedded_point_type = point<T, M>;
	using plane_type = hyperplane<T, M>;

	[[nodiscard]] constexpr embedded_point_type to_embedded() const noexcept
	{
	}

	/* [[nodiscard]] constexpr operator embedded_point_type() const noexcept */
	/* { */
	/* 	return to_embedded(); */
	/* } */

	point_type p;
	plane_type plane;
};

} // end of namespace gmt
