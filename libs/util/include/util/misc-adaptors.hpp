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

#include "util/meta.hpp"

namespace util{

namespace ranges = std::ranges;

template<
	bool sentinel_flag,
	std::weakly_incrementable T,
	std::weakly_incrementable ... Ts>
class zip_iterator{
private:

	template<uint64_t index, uint64_t ... indices>
	friend constexpr auto impl_equal(
		zip_iterator<sentinel_flag, T, Ts...> const& lhs,
		zip_iterator<true, T, Ts...> const& rhs,
		std::index_sequence<index, indices...>)
	{
		if(std::get<index>(lhs.m_iterators)
			== std::get<index>(rhs.get_iterators())){
			return true;
		}

		if constexpr(sizeof...(indices) > 0){
			return impl_equal(
				lhs,
				rhs,
				std::index_sequence<indices...>{});
		}else{
			return false;
		}
	}

	template<uint64_t index, uint64_t ... indices>
	friend constexpr auto impl_equal(
		zip_iterator<sentinel_flag, T, Ts...> const& lhs,
		zip_iterator<false, T, Ts...> const& rhs,
		std::index_sequence<index, indices...>)
	{
		if(std::get<index>(lhs.m_iterators)
			!= std::get<index>(rhs.get_iterators())){
			return false;
		}

		if constexpr(sizeof...(indices) > 0){
			return impl_equal(
				lhs,
				rhs,
				std::index_sequence<indices...>{});
		}else{
			return true;
		}
	}

	template<
		uint64_t index,
		bool r_sentinel,
		uint64_t ... indices>
	friend constexpr auto impl_difference(
		zip_iterator<sentinel_flag, T, Ts...> const& lhs,
		zip_iterator<r_sentinel, T, Ts...> const& rhs,
		std::index_sequence<index, indices...>)
	{
		// TODO: return the std::common type of the differences

		auto const d = std::get<index>(lhs.m_iterators)
			- std::get<index>(rhs.get_iterators());

		if constexpr(sizeof...(indices) > 0){
			auto const min = impl_difference(
				lhs,
				rhs,
				std::index_sequence<indices...>{}
			);

			return d < min ? d : min;
		}else{
			return d;
		}
	}
public:

	using difference_type = int64_t;
	using reference = std::tuple<
		std::iter_reference_t<T>,
		std::iter_reference_t<Ts> ...
	>;
	
	static constexpr auto n_iterators = sizeof...(Ts) + 1;
	static constexpr auto iterators_sequence = std::make_index_sequence<
		n_iterators>{};

	constexpr explicit zip_iterator()
	{}

	constexpr explicit zip_iterator(T&& t, Ts&& ... ts)
		: m_iterators(
			std::make_tuple<T, Ts...>(
				std::forward<T>(t),
				std::forward<Ts>(ts)...
			)
		)
	{}

	constexpr auto operator++(int)
	{
		auto copy = *this;
		tuple_foreach(m_iterators, [](auto&& i){ ++i; });
		return copy;
	}

	constexpr auto& operator++()
	{
		tuple_foreach(m_iterators, [](auto&& i){ ++i; });
		return *this;
	}

	constexpr auto operator--(int)
	{
		auto copy = *this;
		tuple_foreach(m_iterators, [](auto&& i) { --i; });
		return copy;
	}

	constexpr auto& operator--()
	{
		tuple_foreach(m_iterators, [](auto&& i) { --i; });
		return *this;
	}

	constexpr auto operator*()
	{
		return tuple_transform(
			m_iterators,
			[](auto&& i) -> std::iter_reference_t<decltype(i)>
			{ return *i; }
		);
	}

	template<bool other_sentinel_flag>
	constexpr auto operator==(
		zip_iterator<other_sentinel_flag, T, Ts...> const& other) const
	{
		return impl_equal(*this, other, iterators_sequence);
	}

	template<bool other_sentinel_flag>
	constexpr auto operator!=(
		zip_iterator<other_sentinel_flag, T, Ts...> const& other) const
	{
		return !(*this == other);
	}

	constexpr auto operator==(std::unreachable_sentinel_t) const
	{
		return false;
	}

	constexpr auto operator!=(std::unreachable_sentinel_t) const
	{
		return true;
	}

	template<bool r_sentinel>
	friend constexpr difference_type operator-(
		zip_iterator<sentinel_flag, T, Ts ...> const& lhs,
		zip_iterator<r_sentinel, T, Ts ...> const& rhs)
	{
		return impl_difference(
			lhs,
			rhs,
			iterators_sequence
		);
	}

	constexpr std::tuple<T, Ts...> const& get_iterators() const
	{
		return m_iterators;
	}

protected:
	std::tuple<T, Ts...> m_iterators;
};

template<ranges::range T, ranges::range ... Ts>
class zip_adaptor{
public:
	using iterator_type = zip_iterator<
		false,
		ranges::iterator_t<T>,
		ranges::iterator_t<Ts>...>;

	using sentinel_type = zip_iterator<
		true,
		ranges::sentinel_t<T>,
		ranges::sentinel_t<Ts> ...>;

	constexpr explicit zip_adaptor(T&& t, Ts&& ... ts)
		: m_ranges{ t, ts ... }
	{}

	constexpr iterator_type begin() noexcept
	{
		return std::make_from_tuple<iterator_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::begin(i); }
			)
		);
	}

	constexpr sentinel_type end() noexcept
	{
		return std::make_from_tuple<sentinel_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::end(i); }
			)
		);
	}

	constexpr iterator_type begin() const noexcept
	{
		return std::make_from_tuple<iterator_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::begin(i); }
			)
		);
	}

	constexpr sentinel_type end() const noexcept
	{
		return std::make_from_tuple<sentinel_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::end(i); }
			)
		);
	}

protected:
	using range_tuple = std::tuple<T, Ts...>;

	range_tuple m_ranges;
};

/*
 * adaptors function
 */

template<ranges::range T, ranges::range ... Ts>
constexpr auto zip(T&& a, Ts&& ... as)
{
	return zip_adaptor<T, Ts...>{ std::forward<T>(a), std::forward<Ts>(as) ...  };
}

template<ranges::range ... Ts>
constexpr auto enumerate(Ts&& ... a)
{
	auto const sizes = std::array{ ( ranges::size(a), ...) };
	using sizes_t = decltype(sizes)::size_type;

	return zip(
		ranges::views::iota(sizes_t{0}, ranges::min(sizes)),
		a ...
	);
}

} // end of namespace util
