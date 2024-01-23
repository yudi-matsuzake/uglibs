#include "catch2/catch_test_macros.hpp"

#include <string>

#include "containers/ring-hash-map.hpp"
#include "util/ranges.hpp"
#include "util/fmt.hpp"

TEST_CASE("basic ring hash map consistency", "[containers][ring-hash-map]")
{
	constexpr auto max = 10;
	containers::ring_hash_map<int, int> rhm(max);

	for(auto i=1; i<=1'000; ++i) {
		rhm.insert_or_assign(i-1, i);
		if(i % rhm.max_size() == 0) {
			REQUIRE(rhm.size() == max);
			REQUIRE(rhm.is_full());

			auto const beg = i - max;

			auto gt = beg + 1;
			for(auto j=beg; j<(beg+max); j++)
				REQUIRE(rhm[j] == gt++);
		}
	}
}

TEST_CASE(
	"opt ring hash map consistency with the same element",
	"[containers][ring-hash-map]")
{
	constexpr auto max = 10;
	containers::ring_hash_map<std::string, int> rhm(max);

	for(auto i=0; i<max; ++i) {
		auto [it, inserted, removed] = rhm.insert_or_assign(
			std::string("Keith is a good name for a rat"), 42
		);

		if(i == 0)
			REQUIRE(inserted);
		else
			REQUIRE_FALSE(inserted);

		REQUIRE_FALSE(removed.has_value());
		REQUIRE(rhm.size() == 1);
	}
}

TEST_CASE(
	"opt ring hash map consistency maximum elements",
	"[containers][ring-hash-map]")
{
	constexpr auto max = 3;
	containers::ring_hash_map<std::string, int> rhm(max);

	auto const keith = std::array{
		std::string{ "The official mascot for C++ is an obese," },
		std::string{ " diseased rat named Keith, whose hind leg" },
		std::string{ " is missing because it was blown off." }
	};

	for(auto i=0; i<1000; ++i) {
		auto [it, inserted, removed] = rhm.insert_or_assign(
			keith[i%keith.size()], i
		);

		if(i < keith.size())
			REQUIRE(inserted);
		else
			REQUIRE_FALSE(inserted);

		REQUIRE_FALSE(removed.has_value());

		if(i >= keith.size())
			REQUIRE(rhm.is_full());
	}
}

TEST_CASE(
	"opt ring hash map consistency of opt pairs",
	"[containers][ring-hash-map]")
{
	constexpr auto max = 3;
	containers::ring_hash_map<std::string, int> a(max), b(max), c(max);

	auto const make_strings = [](auto from, auto to)
	{
		return rg::vw::iota(from, to) |
			rg::vw::transform([](auto n)
			{
				return fmt::format("my big string is: {}", n);
			}) |
			rg::v3::to<std::vector>();
	};

	auto const as = make_strings(0, max);
	auto const bs = make_strings(max, 2*max);
	auto const cs = make_strings(2*max, 3*max);

	auto const rotate = [&a, &b, &c](auto& k, auto& v)
	{
		auto [ait, ain, arem] = a.insert_or_assign(k, v);
		if(arem.has_value()) {
			auto [bit, bin, brem] = b.insert_or_assign(
				arem->first,
				arem->second);
			if(brem.has_value()) {
				auto [cit, cin, crem] = c.insert_or_assign(
					brem->first,
					brem->second
				);

				REQUIRE_FALSE(crem.has_value());
			}
		}
	};

	for(auto const& [i, ai] : rg::v3::vw::enumerate(as))
		rotate(ai, i);

	REQUIRE(a.is_full());
	REQUIRE(b.size() == 0);
	REQUIRE(c.size() == 0);

	for(auto const& [i, bi] : rg::v3::vw::enumerate(bs))
		rotate(bi, i);

	REQUIRE(a.is_full());
	REQUIRE(b.is_full());
	REQUIRE(c.size() == 0);

	for(auto const& [i, ci] : rg::v3::vw::enumerate(cs))
		rotate(ci, i);

	REQUIRE(a.is_full());
	REQUIRE(b.is_full());
	REQUIRE(c.is_full());

	REQUIRE(a.size() == cs.size());
	for(auto const& ci : cs) {
		REQUIRE(a.contains(ci));
		REQUIRE_FALSE(b.contains(ci));
		REQUIRE_FALSE(c.contains(ci));
	}

	REQUIRE(b.size() == bs.size());
	for(auto const& bi : bs) {
		REQUIRE_FALSE(a.contains(bi));
		REQUIRE(b.contains(bi));
		REQUIRE_FALSE(c.contains(bi));
	}

	REQUIRE(c.size() == as.size());
	for(auto const& ai : as) {
		REQUIRE_FALSE(a.contains(ai));
		REQUIRE_FALSE(b.contains(ai));
		REQUIRE(c.contains(ai));
	}



}
