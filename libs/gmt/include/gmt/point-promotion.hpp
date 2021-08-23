/**
  * @brief this files defines promotion rules for point
  * operations
  */
#pragma once

#include "gmt/misc.hpp"
#include "util/misc.hpp"

namespace gmt{

/*
 * promotion definitions
 */
template<class P, class Q>
struct point_promotion_def{
	static constexpr bool value = false;
};

template<class T>
struct point_promotion_def<T, T>{
	using type = T;
	static constexpr bool value = true;
};

/*
 * point promotion logic
 *
 * the ideia is to define only one `point_promotion_def<P, Q>`
 * and the rules of promotion will be applied without the need
 * to declare `point_promotion_def<Q, P>`
 *
 * for instance, if we declare that operation(point, vec) -> vec
 * then operation(vec, point) -> vec also
 *
 * note that this means that point promotions are commutative
 */
template<class P, class Q>
using point_promotion = std::conditional_t<
	point_promotion_def<std::decay_t<P>, std::decay_t<Q>>::value,
	point_promotion_def<std::decay_t<P>, std::decay_t<Q>>,
	point_promotion_def<std::decay_t<Q>, std::decay_t<P>>
>;

/*
 * aliases
 */
template<class P, class Q>
constexpr bool has_promotion_v = point_promotion<P, Q>::value;

template<class P, class Q>
using point_promotion_t = typename point_promotion<P, Q>::type;

} // end of namespace gmt
