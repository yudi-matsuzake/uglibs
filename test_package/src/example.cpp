#include <ranges>
#include <cassert>

#include "gmt/point.hpp"
#include "gmt/vector.hpp"

int main()
{
	using point2d = gmt::point<double, 2>;
	point2d p{ 1., 2. };
	point2d q{ 2., 2. };

	assert(((p + q)*2. == point2d{ 6., 8. }));
}
