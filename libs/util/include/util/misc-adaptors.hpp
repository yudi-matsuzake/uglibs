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

#include "range/v3/view/zip.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/algorithm/min.hpp"

#include "util/meta.hpp"


namespace util{

template<ranges::range ... Ts>
auto enumerate(Ts&& ... a)
{
	auto const sizes = std::array{ (ranges::size(std::forward<Ts>(a)), ...) };
	using sizes_t = decltype(sizes)::size_type;

	return ranges::views::zip(
		ranges::views::iota(sizes_t{0}, ranges::min(sizes)),
		std::forward<Ts>(a) ...
	);
}

} // end of namespace util
