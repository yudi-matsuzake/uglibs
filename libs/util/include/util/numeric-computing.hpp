#pragma once

#include <cmath>
#include <deque>
#include <numbers>
#include <type_traits>
#include <concepts>
#include <functional>

namespace util {

/**
  * golden selection search: finds the extreme point defined by `extreme`
  * in the domain [a, b] with tolerance of `tol`. notice that `extreme` and
  * `f` must be consistent. for instance, if extreme = std::less<>{}, then
  * `f` must be unimodal and have a minimum value.
  *
  * @param f float -> float unimodal function
  * @param a first element of the domain
  * @param b last element of the domain
  * @param tol tolerance of the algorithm, where the real extreme will be in the
  *            interval [x-tol, x+tol] where x is the value returned by this function
  * @param extreme function thar receives (x, y) and defines whether x
  *        is closer to the extreme than y; notice that for minimum extreme
  *        you could pass the std::less<>{} function. similarly, for the maximum
  *        extreme, std::greater will lead to the maximum search.
  *
  * @return the extreme point x, where f(x) is the extreme value
  */
template<class Fn, std::floating_point Float, class FnExtreme = std::less<Float>>
	requires (std::is_invocable_r_v<Float, Fn, Float>)
[[nodiscard]] Float golden_section_search(
	Fn&& f,
	Float a, Float b,
	Float tol = Float{1e-10},
	FnExtreme extreme = FnExtreme{})
{
	static constexpr auto phi = std::numbers::phi_v<Float>;
	Float c = b - (b - a) / phi;
	Float d = a + (b - a) / phi;
	Float fc = f(c);
	Float fd = f(d);

	do {
		if(extreme(fc, fd)){
			b = d;
			d = c;
			c = b - (b - a)/phi;

			fd = fc;
			fc = f(c);
		}else{
			a = c;
			c = d;
			d = a + (b - a)/phi;

			fc = fd;
			fd = f(d);
		}

	} while(std::fabs(b - a) > tol);

	if(extreme(fc, fd))
		return c;
	return d;
}

}
