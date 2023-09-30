#include "catch2/catch_test_macros.hpp"

#include <limits>
#include <iomanip>

#include "ia/interval.hpp"
#include "ia/operations.hpp"

template<class T>
static constexpr void fullness_test()
{
	using namespace ia;

	constexpr auto R = interval<T>::R();
	STATIC_REQUIRE(R.is_full() == true);
	STATIC_REQUIRE(R.lo() == interval<T>::minus_infinity);
	STATIC_REQUIRE(R.hi() == interval<T>::infinity);
}

TEST_CASE(
	"interval simple tests",
	"[interval]")
{

	using namespace ia;
	constexpr interval empty{ 1.0, -1.0 };

	SECTION("emptyness"){
		STATIC_REQUIRE(empty.is_empty() == true);
		STATIC_REQUIRE(interval<float>::empty().is_empty() == true);
		STATIC_REQUIRE(interval<double>::empty().is_empty() == true);
		STATIC_REQUIRE(interval<long double>::empty().is_empty() == true);
	}

	SECTION("fullness"){
		fullness_test<float>();
		fullness_test<double>();
		fullness_test<long double>();
	}

	SECTION("common arithmetics"){
		constexpr interval i{ 1.0, 2.0 };

		STATIC_REQUIRE(-i == interval{ -2.0, -1.0 });

		REQUIRE((i + interval{ 1.0, 3.0 }) == interval{ 2.0, 5.0 });
		REQUIRE((i - interval{ -4.0, 3.0 }) == interval{ -2.0, 6.0 });

		REQUIRE(i + 5.0 == interval{ 6.0, 7.0 });

		REQUIRE(interval{ -2.0, 5.0 }*3.0 == interval{ -6.0, 15.0 });

		REQUIRE(i*interval{ -1.5, 2.0 } == interval{ -3.0, 4.0 });

		auto div = i/interval{ 1.0, 2.0 };
		REQUIRE(div == interval{ 0.5, 2.0 });
	}

	SECTION("sqrt"){
		REQUIRE(sqrt(empty) == empty);
		REQUIRE(sqrt(interval{ 4.0, 4.0 }) == std::sqrt(4.0));

		REQUIRE(sqrt(interval{ -1.0, 4.0 })
			== interval{ 0.0, std::sqrt(4.0) }
		);
	}

	SECTION("simple operations"){
		STATIC_REQUIRE(abs(interval{ -2.0, -1.0 }) == interval{1.0, 2.0});
		STATIC_REQUIRE(abs(interval{ -2.0, 1.0 }) == interval{0.0, 2.0});
		STATIC_REQUIRE(abs(interval{ -2.0, 1.0 }) == interval{0.0, 2.0});
		STATIC_REQUIRE(abs(interval{ -0.5, 1.0 }) == interval{0.0, 1.0});
		STATIC_REQUIRE(abs(interval{ 1.0, 10.0 }) == interval{1.0, 10.0});
		STATIC_REQUIRE(abs(interval{ -5.0, 0.0 }) == interval{0.0, 5.0});

		STATIC_REQUIRE(min(interval{2.0}, interval{3.0}) == 2.0);
		STATIC_REQUIRE(
			min(interval{2., 3.}, interval{4., 5.}) == interval{2., 3.}
		);
		STATIC_REQUIRE(
			min(interval{2., 4.}, interval{3., 5.}) == interval{2., 4.}
		);
		STATIC_REQUIRE(
			min(interval{2., 5.}, interval{3., 4.}) == interval{2., 4.}
		);

		STATIC_REQUIRE(max(interval{2.0}, interval{3.0}) == 3.0);
		STATIC_REQUIRE(
			max(interval{2., 3.}, interval{4., 5.}) == interval{4., 5.}
		);
		STATIC_REQUIRE(
			max(interval{2., 4.}, interval{3., 5.}) == interval{3., 5.}
		);
		STATIC_REQUIRE(
			max(interval{2., 5.}, interval{3., 4.}) == interval{3., 5.}
		);

		STATIC_REQUIRE(
			join(interval{2., 5.}, interval{3., 4.}) == interval{2., 5.}
		);
		STATIC_REQUIRE(
			join(interval{4., 5.}, interval{3., 6.}) == interval{3., 6.}
		);

		STATIC_REQUIRE(
			smaller_or_join(interval{2., 3.}, interval{4., 5.})
			== interval{2., 3.}
		);

		STATIC_REQUIRE(
			smaller_or_join(interval{4., 5.}, interval{2., 3.})
			== interval{2., 3.}
		);

		STATIC_REQUIRE(
			smaller_or_join(interval{2., 3.}, interval{3., 4.})
			== interval{2., 4.}
		);

		STATIC_REQUIRE(
			greater_or_join(interval{2., 3.}, interval{4., 5.})
			== interval{4., 5.}
		);

		STATIC_REQUIRE(
			greater_or_join(interval{4., 5.}, interval{2., 3.})
			== interval{4., 5.}
		);

		STATIC_REQUIRE(
			greater_or_join(interval{2., 3.}, interval{3., 4.})
			== interval{2., 4.}
		);

	}

	SECTION("euclidean distance"){
		std::array a{ 4.0, 7.0 };
		std::array b{ 1.0, 3.0 };
		auto d = euclidean_distance(
			begin(a), end(a),
			begin(b),
			interval<double>{ 0.0, 0.0}
		);
		REQUIRE(d == 5.0);
	}

	SECTION("interval comparison"){
		STATIC_REQUIRE(
			compare(interval<float>::empty(), interval<float>::empty())
			== interval_comparison::UNDEFINED
		);

		STATIC_REQUIRE(
			compare(interval{ 0.0, 1.0 }, interval{ 2.0, 3.0 }) 
			== interval_comparison::SMALLER
		);

		STATIC_REQUIRE(
			compare(interval{ 4.0, 5.0 }, interval{ 2.0, 3.0 }) 
			== interval_comparison::GREATER
		);

		STATIC_REQUIRE(
			compare(interval{ 1.0, 5.0 }, interval{ 2.0, 3.0 }) 
			== interval_comparison::INTERSECT
		);

		STATIC_REQUIRE(
			compare(interval{ 1.0, 2.0 }, interval{ 2.0, 3.0 }) 
			== interval_comparison::INTERSECT
		);
	}

	SECTION("interval length"){
		constexpr interval a{  0.,  2. };
		constexpr interval b{ -2.,  2. };
		constexpr interval c{ -6.,  0. };
		constexpr interval d{ -10., -2. };

		REQUIRE(a.half_width() == 1.0);
		REQUIRE(b.half_width() == 2.0);
		REQUIRE(c.half_width() == 3.0);
		REQUIRE(d.half_width() == 4.0);
	}

	SECTION("interval difference"){
		using interval_type = ia::interval<double>;

		STATIC_REQUIRE(
			distance(interval{0., 1.}, interval{0., 2.}) == 0.0
		);

		STATIC_REQUIRE(
			distance(interval{0., 1.}, interval_type::empty())
			== interval_type::empty()
		);

		REQUIRE(
			distance(interval{0., 1.}, interval{2., 2.}) == 1.0
		);

		REQUIRE(
			distance(interval{0., 1.}, interval{-2., -.5}) == .5
		);
	}

	SECTION("interval clamp"){

		STATIC_REQUIRE(
			ia::clamp(
				interval{ 1., 2.},
				interval{-1., 0.},
				interval{ 2., 3.}
			) == interval{ 1., 2.}
		);

		STATIC_REQUIRE(
			ia::clamp(
				interval{ 1., 2.},
				interval{ 2., 2.},
				interval{ 2., 3.}
			) == interval{ 2., 2.}
		);

		STATIC_REQUIRE(
			ia::clamp(
				interval{ -1., 2.},
				interval{ 0., 0.},
				interval{ 1., 1.}
			) == interval{ 0., 1.}
		);

	}
}
