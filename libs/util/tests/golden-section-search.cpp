#include "catch2/catch_test_macros.hpp"

#include <numbers>
#include "util/numeric-computing.hpp"

TEST_CASE("golden search minimum", "[util][numeric-computing]")
{
	static constexpr auto phi = std::numbers::phi_v<double>;

	auto c = uint64_t{0};
	auto f = [&c](double x) -> double { ++c; return x*x; };
	auto tol = 1e-10;

	auto const a = -4.0;
	auto const b = +4.0;
	auto x = util::golden_section_search(f, a, b, tol);
	REQUIRE(std::fabs(x) <= tol);

	auto const max_iterations = static_cast<uint64_t>(
		std::log((b-a)/tol) / std::log(phi) + 1.0);

	auto const max_calls = max_iterations + 2;

	REQUIRE(c <= max_calls);
}

TEST_CASE("golden search maximum", "[util][numeric-computing]")
{
	static constexpr auto phi = std::numbers::phi_v<double>;

	auto c = uint64_t{0};
	auto f = [&c](double x) -> double { ++c; return -(x*x); };
	auto tol = 1e-10;

	auto const a = -4.0;
	auto const b = +4.0;
	auto x = util::golden_section_search(f, a, b, tol, std::greater<double>{});
	REQUIRE(std::fabs(x) <= tol);

	auto const max_iterations = static_cast<uint64_t>(
		std::log((b-a)/tol) / std::log(phi) + 1.0);

	auto const max_calls = max_iterations + 2;

	REQUIRE(c <= max_calls);
}
