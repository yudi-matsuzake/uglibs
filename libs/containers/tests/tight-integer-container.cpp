#include "catch2/catch.hpp"

#include "containers/tight-integers-container.hpp"

TEST_CASE("simple tests", "[tight-integers-container]")
{
	STATIC_REQUIRE(sizeof(containers::underlying_integer<1>::type) == 1);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<8>::type) == 1);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<9>::type) == 2);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<16>::type) == 2);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<17>::type) == 4);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<32>::type) == 4);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<33>::type) == 8);
	STATIC_REQUIRE(sizeof(containers::underlying_integer<64>::type) == 8);

	constexpr auto n_bits = 9;
	containers::tight_integer_container<n_bits> v;
	assert(v.empty() == true);

	v.resize(10);
	v.clear();
	REQUIRE(v.size() == 0);
	REQUIRE(v.capacity() >= 0);

	for(auto i=0UL; i<30; ++i){
		INFO(i);
		v.reserve(i);
		REQUIRE(v.size() == 0);
		REQUIRE(v.capacity() >= i);
	}

	for(auto i=0UL; i<30; ++i){
		INFO(i);
		v.resize(i);
		REQUIRE(v.size() == i);
		REQUIRE(v.capacity() >= i);
		REQUIRE(v.empty() == (i == 0UL));
	}

}

