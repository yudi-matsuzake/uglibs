#include <cstdio>
#include <ranges>
#include <cassert>

#include "util/static-string.hpp"
#include "gmt/point.hpp"
#include "gmt/vector.hpp"
#include "ug/graphics/app.hpp"
#include "ia/interval.hpp"
#include "ia/operations.hpp"
#include "containers/ring-vector.hpp"

int main()
{
	{ // util
		constexpr auto a = util::to_static_string<42>();
		static_assert(a.size() == 3);
		static_assert(a == util::static_string("42"));
		constexpr auto concat_64 = util::static_string("uint")
			+ util::to_static_string<64>()
			+ "_t";
		static_assert(concat_64 == "uint64_t");

		std::puts("util done");
	}

	{ // graphics
		ug::graphics::app app(512, 512, "Hello, graphics");
		std::puts("graphics done");
	}

	{ // gmt
		using point2d = gmt::point<double, 2>;
		point2d p{ 1., 2. };
		point2d q{ 2., 2. };

		assert(((p + q)*2. == point2d{ 6., 8. }));
		std::puts("gmt done");
	}

	{ // ia
		using namespace ia;
		constexpr interval empty{ 1.0, -1.0 };
		static_assert(empty.is_empty() == true);
		static_assert(interval<float>::empty().is_empty() == true);
		static_assert(interval<double>::empty().is_empty() == true);
		static_assert(interval<long double>::empty().is_empty() == true);

		constexpr interval i{ 1.0, 2.0 };
		static_assert(-i == interval{ -2.0, -1.0 });
		assert(((i + interval{ 1.0, 3.0 }) == interval{ 2.0, 5.0 }));
		assert(((i - interval{ -4.0, 3.0 }) == interval{ -2.0, 6.0 }));
		assert((i + 5.0 == interval{ 6.0, 7.0 }));
		assert((interval{ -2.0, 5.0 }*3.0 == interval{ -6.0, 15.0 }));
		assert((i*interval{ -1.5, 2.0 } == interval{ -3.0, 4.0 }));

		auto const div = i/interval{ 1.0, 2.0 };
		assert((div == interval{ 0.5, 2.0 }));

		std::puts("ia done");
	}

	{ // containers
		constexpr auto max = 10;
		containers::ring_vector<int> v(max);

		for(auto i=1; i<=10'000; ++i) {
			v.push_back(i);
			if(i % v.max_size() == 0) {
				auto gt = i - max + 1;
				for(auto vi : v.view())
					assert(vi == gt++);
			}
		}
		std::puts("containers done");
	}
}
