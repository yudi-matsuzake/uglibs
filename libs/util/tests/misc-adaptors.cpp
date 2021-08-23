#include <catch2/catch.hpp>

#include <vector>
#include <list>

#include "util/misc-adaptors.hpp"

TEST_CASE(
	"misc adaptors",
	"[util]")
{

	using namespace util;

	constexpr std::array a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	constexpr std::array b{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

	SECTION("range"){

		auto m = a.size()/2;
		auto mid = a[m];

		for(auto&& i : range(a.begin() + m, a.end()))
			REQUIRE(i == mid++);

	}

	SECTION("zip iterator"){
		auto ra = make_range(a);
		auto rb = make_range(b);
		auto it0 = zip_iterator(ra.begin(), rb.begin());
		auto it1 = it0;
		REQUIRE(it0 == it1);
		it0++;
		++it0;
		it0++;
		--it0;
		it0--;
		REQUIRE(it0 != it1);

		auto [va, vb]  = *it0;

		REQUIRE(va == 1);
		REQUIRE(vb == 8);

		std::tie(va, vb) = *it1;

		REQUIRE(va == 0);
		REQUIRE(vb == 9);
	}

	SECTION("zip adaptor"){
		std::vector<int> vsum(a.size());
		std::list<int> lsum;

		for(auto i=0u; i<a.size(); i++){
			auto sum = a[i] + b[i];
			vsum[i] = sum;
			lsum.push_back(sum);
		}

		for(auto&& [ av, bv, v, l ] : zip(a, b, vsum, lsum)){
			REQUIRE(v == av+bv);
			REQUIRE(l == av+bv);
		}

		constexpr auto n = 10;
		constexpr auto generate = [n](auto step)
		{
			std::array<int, n> tmp{};
			int last = -step;
			for(auto&& i : tmp){
				i = last + step;
				last = i;
			}

			return tmp;
		};

		constexpr std::array aa = generate(1);
		constexpr std::array ba = generate(2);

		constexpr auto sum_of_diff = [&aa, &ba]
		{
			auto diff = 0;
			for(auto&& [ aav, bav ] : zip(make_range(aa), make_range(ba)))
				diff += bav - aav;
			return diff;
		}();


		REQUIRE(sum_of_diff == (n*(n-1))/2);
	}

	/* SECTION("zip set"){ */

	/* 	std::array<int32_t, a.size()> tmp; */

	/* 	for(auto& [ ai, ti ] : zip(a, tmp)) */
	/* 		ti = ai*ai; */

	/* 	for(auto&& [ ai, ti ] : zip(a, tmp)) */
	/* 		REQUIRE(ti == ai*ai); */

	/* } */

	SECTION("seq adaptor"){
		constexpr int n = 10;
		constexpr auto sum_formula = (n*(n-1))/2;
		constexpr auto sum_iter = []
		{
			auto s = 0;
			for(auto&& c : seq(10))
				s += c;
			return s;
		}();

		STATIC_REQUIRE(sum_formula == sum_iter);

		std::vector<int> v{15};
		std::iota(begin(v), end(v), 0);

		for(auto&& [i, value] : zip(seq(v.size()), make_range(v)))
			REQUIRE(i == static_cast<uint64_t>(value));
	}

}
