#pragma once

#include <ranges>
#include "range/v3/all.hpp"
#include <algorithm>

namespace rg {
	using namespace std::ranges;

	namespace vw {
		using namespace std::ranges::views;
	}

	namespace v3 {
		using namespace ranges;

		namespace vw {
			using namespace ranges::views;
		}

		namespace ac {
			using namespace ranges::actions;
		}
	}
}
