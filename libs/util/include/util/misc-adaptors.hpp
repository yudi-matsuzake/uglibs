/** @file misc-adaptors.hpp
  * Implementation of adaptors to help common 
  * objects manipulation
  */

#pragma once

#include <optional>
#include <tuple>
#include <utility>
#include <limits>
#include <cstdint>
#include <iterator>
#include <ranges>

#include "range/v3/all.hpp"

#include "util/meta.hpp"


namespace util{

namespace rgs = std::ranges;

template<rgs::range ... Ts>
constexpr auto enumerate(Ts&& ... a)
{
	auto const sizes = std::array{ ( rgs::size(a), ...) };
	using sizes_t = decltype(sizes)::size_type;

	return ranges::views::zip(
		ranges::views::iota(sizes_t{0}, ranges::min(sizes)),
		a ...
	);
}

} // end of namespace util
