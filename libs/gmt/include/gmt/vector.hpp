/** @file vector.hpp
  * generic vector definition
  */
#pragma once

#include <cmath>
#include <algorithm>
#include <numeric>

#include "point.hpp"

namespace gmt{

template<class T, uint64_t N, class OperationOutputType = void>
class vector : public point<T, N,
	typename defaulted_type<
		OperationOutputType,
		vector<T, N>>::type> {
public:
	using operation_type = typename defaulted_type<
		OperationOutputType,
		vector<T, N>>::type;

	using point<T, N, operation_type>::point;

	constexpr vector(gmt::point<T, N> const& p)
	{
		std::copy(begin(p), end(p), this->begin());
	}

};

template<class Vector, class T = typename Vector::element_type>
constexpr auto squared_norm(Vector const& v)
{
	auto sum = T{0};
	for(auto&& i : v)
		sum += i*i;
	return sum;
}

template<class Vector>
auto norm(Vector const& v)
{
	return sqrt(squared_norm(v));
}


template<class Vector, class T = typename Vector::element_type>
auto dot(Vector const& a, Vector const& b)
{
	return std::inner_product(std::begin(a), std::end(a), std::begin(b), T{0});
}

template<class Vector>
auto angle(Vector const& a, Vector const& b)
{
	return std::acos(dot(a, b)/(norm(a)*norm(b)));
}

template<class Vector, class T = typename Vector::element_type>
Vector versor(Vector const& v)
{
	auto n = norm(v);
	if(n == 0.0)
		return Vector::all(T{0});

	return v / n;
}

/**
  * point promotion definition
  * @see point_promotion_def
  */
template<class T, uint64_t N, class OpPoint, class OpVec>
struct point_promotion_def<
	point<T, N, OpPoint>,
	vector<T, N, OpVec>>{

	using type = vector<T, N, OpVec>;
	static constexpr bool value = true;
};

} // end of namespace gmt
