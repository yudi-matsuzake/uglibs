#pragma once

#include <cmath>
#include <deque>
#include <numbers>
#include <type_traits>
#include <concepts>
#include <functional>

namespace util {

/**
  * holds the golden section state; the current extremes to be checked
  * the field names are minemonics to the description o the algorithm
  * a -> extreme left value
  * b -> extreme right value
  * c -> inner left value
  * d -> inner right value
  * fa -> f of extreme left value
  * fb -> f of extreme right value
  * fc -> f of inner left value
  * fd -> f of inner right value
  */
template<std::floating_point Float>
struct golden_section_state {
	Float a, b, c, d;
	Float fc, fd;
	std::optional<Float> fa, fb;
};

/**
  * golden selection search: finds the extreme point defined by `extreme`
  * in the domain [a, b] with tolerance defined by the `tolerance` function.
  * notice that `extreme` and `f` must be consistent. for instance,
  * if extreme = std::less<>{}, then `f` must be unimodal and have a minimum value.
  *
  * @param f float -> float unimodal function
  * @param a first element of the domain
  * @param b last element of the domain
  * @param torerance tolerance function of the algorithm; this function receives a
  *        `golden_section_state`; and returns true if the algorithm should halt
  * @param extreme function thar receives (x, y) and defines whether x
  *        is closer to the extreme than y; notice that for minimum extreme
  *        you could pass the std::less<>{} function. similarly, for the maximum
  *        extreme, std::greater will lead to the maximum search.
  *
  * @return the extreme point x, where f(x) is the extreme value
  */
template<
	class Fn,
	std::floating_point Float,
	class FnTolerance,
	class FnExtreme = std::less<Float>>
	requires (
		std::is_invocable_r_v<Float, Fn, Float>
		&& std::is_invocable_r_v<
			bool,
			FnTolerance,
			golden_section_state<Float> const&>)
[[nodiscard]] Float golden_section_search(
	Fn&& f,
	Float const a, Float const b,
	FnTolerance tolerance,
	FnExtreme extreme = FnExtreme{})
{
	static constexpr auto phi = std::numbers::phi_v<Float>;

	auto s = [&,
		  c = b - (b - a) / phi,
		  d = a + (b - a) / phi]{
		return golden_section_state<Float>{
			.a  = a,
			.b  = b,
			.c  = c,
			.d  = d,
			.fc = f(c),
			.fd = f(d),
			.fa = std::nullopt,
			.fb = std::nullopt,
		};
	}();

	do {
		if(extreme(s.fc, s.fd)){
			s.b = s.d;
			s.d = s.c;
			s.c = s.b - (s.b - s.a)/phi;

			s.fb = s.fd;

			s.fd = s.fc;
			s.fc = f(s.c);
		}else{
			s.a = s.c;
			s.c = s.d;
			s.d = s.a + (s.b - s.a)/phi;

			s.fa = s.fc;

			s.fc = s.fd;
			s.fd = f(s.d);
		}

	} while(tolerance(s));

	return extreme(s.fc, s.fd) ? s.c : s.d;
}

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

	auto tolerance = [tol](golden_section_state<Float> const& s) -> bool
	{
		return std::fabs(s.b - s.a) > tol;
	};

	return golden_section_search(
		std::forward<Fn>(f),
		a, b,
		tolerance,
		extreme
	);
}

}
