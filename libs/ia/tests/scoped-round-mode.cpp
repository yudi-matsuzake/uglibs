#include "catch2/catch_all.hpp"

#include "ia/scoped-round-mode.hpp"

auto compute_pi()
{
	auto pi = 1.0;
	auto denominator = 3.0;
	auto m = -1.0;

	auto iterations = 1'000'000;

	for(auto i=0; i<iterations; i++){
		pi += m*1.0/denominator;
		denominator += 2.0;
	}

	return pi;
}

TEST_CASE(
	"testing scoped round mode",
	"[scoped-round-mode]")
{

	using namespace ia;

	auto downward	= execute_downward(compute_pi);
	auto upward	= execute_upward(compute_pi);

	REQUIRE(downward != upward);
}
