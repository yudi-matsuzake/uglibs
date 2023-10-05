#include "catch2/catch_test_macros.hpp"

#include "containers/ring-vector.hpp"

TEST_CASE("basic ring vector consistency", "[containers][ring-vector]")
{
	constexpr auto max = 10;
	containers::ring_vector<int> v(max);

	for(auto i=1; i<=10'000; ++i) {
		v.push_back(i);
		if(i % v.max_size() == 0) {
			auto gt = i - max + 1;
			for(auto vi : v.view())
				REQUIRE(vi == gt++);
		}
	}
}

TEST_CASE("basic ring optional from push_back", "[containers][ring-vector]")
{
	constexpr auto max = 100;
	containers::ring_vector<int> v(max);

	for(auto i=1; i<=10'000; ++i) {
		auto opt = v.push_back(i);
		if(opt.has_value())
			REQUIRE(opt.value() == (i-max));
		else
			REQUIRE(v.size() <= v.max_size());
	}
}

TEST_CASE("ring vector with \"big\" objects", "[containers][ring-vector]")
{
	auto const values = std::array{
		std::string{ "this is a big object, right?" },
		std::string{ "not using LaTeX is pseudoscience" },
		std::string{ "this object is bigger, but in this moment" },
	};

	containers::ring_vector<std::string> a(1), b(1), c(1);

	for(auto v : values) {
		if(auto oa = a.push_back(std::move(v)); oa.has_value()) {
			if(auto ob = b.push_back(std::move(oa.value()));
				ob.has_value()) {
				c.push_back(std::move(ob.value()));
			}
		}
	}

	REQUIRE(a.front() == values[2]);
	REQUIRE(b.front() == values[1]);
	REQUIRE(c.front() == values[0]);
}
