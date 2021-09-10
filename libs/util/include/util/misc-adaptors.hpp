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

template<class T = uint32_t>
class seq_iterator{
public:
	using difference_type = int64_t;

	constexpr seq_iterator() : m_infinity(true)
	{}

	constexpr seq_iterator(T b) : m_counter(b)
	{}

	constexpr auto& operator+=(seq_iterator rhs)
	{
		m_infinity = rhs.m_infinity;
		m_counter += rhs.m_counter;
		return *this;
	}

	constexpr auto& operator+=(T rhs)
	{
		return *this += seq_iterator(rhs);
	}

	constexpr auto& operator++()
	{
		return *this += 1;
	}

	constexpr auto operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	constexpr auto& operator--()
	{
		m_counter += -1;
		return *this;
	}

	constexpr auto operator--(int)
	{
		auto copy = *this;
		--(*this);
		return copy;
	}

	friend constexpr difference_type operator-(
		seq_iterator const& lhs,
		seq_iterator const& rhs)
	{
		if(lhs.m_infinity)
			return std::numeric_limits<difference_type>::max();
		if(rhs.m_infinity)
			return std::numeric_limits<difference_type>::min();
		return lhs.m_counter - rhs.m_counter;
	}

	constexpr T const& operator*()
	{
		return m_counter;
	}

	constexpr bool operator==(seq_iterator const& other) const
	{
		return !m_infinity &&
			!other.m_infinity &&
			m_counter == other.m_counter;
	}

	constexpr bool operator!=(seq_iterator const& other) const
	{
		return !(*this == other);
	}

protected:
	T m_counter = T{0};
	bool m_infinity = false;
};

template<class T = uint32_t>
struct seq_adaptor{
	constexpr seq_adaptor() = default;

	constexpr seq_adaptor(T e)
		: first(seq_iterator<T>(0)), last(seq_iterator<T>(e))
	{}

	constexpr seq_adaptor(T b, T e)
		: first(seq_iterator<T>(b)), last(seq_iterator<T>(e))
	{}

	constexpr auto begin() noexcept
	{
		return first;
	}

	constexpr auto end() noexcept
	{
		return last;
	}

	seq_iterator<T> first = seq_iterator<T>(0);
	seq_iterator<T> last = seq_iterator<T>();
};

template<std::input_or_output_iterator T, std::input_or_output_iterator ... Ts>
class zip_iterator{
private:

	template<uint64_t index, uint64_t ... indices>
	constexpr auto impl_equal(
		zip_iterator<T, Ts...> const& other,
		std::index_sequence<index, indices...>) const
	{
		if(std::get<index>(m_iterators)
			!= std::get<index>(other.m_iterators)){
			return false;
		}

		if constexpr(sizeof...(indices) > 0){
			return impl_equal(
				other,
				std::index_sequence<indices...>{});
		}else{
			return true;
		}
	}
public:

	using difference_type = int64_t;
	using reference = std::tuple<
		std::iter_reference_t<T>,
		std::iter_reference_t<Ts> ...
	>;

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

	constexpr auto operator==(zip_iterator<T, Ts...> const& other) const
	{
		return impl_equal(
			other,
			std::make_index_sequence<sizeof...(Ts) + 1>{}
		);
	}

	constexpr auto operator!=(zip_iterator<T, Ts...> const& other) const
	{
		return !(*this == other);
	}

	friend constexpr difference_type operator-(
		zip_iterator const& lhs,
		zip_iterator const& rhs)
	{
		// TODO: get the minimal for ALL iterators in the tuple
		return std::get<0>(lhs.m_iterators) - std::get<0>(rhs.m_iterators);
	}

protected:
	std::tuple<T, Ts...> m_iterators;
};

template<ranges::range T, ranges::range ... Ts>
class zip_adaptor{
public:
	using iterator_type = zip_iterator<
		ranges::iterator_t<T>,
		ranges::iterator_t<Ts>...>;

	using sentinel_type = zip_iterator<
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
		return std::make_from_tuple<iterator_type>(
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
		return std::make_from_tuple<iterator_type>(
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

template<class ... T>
constexpr auto seq(T&& ... a)
{
	return seq_adaptor{ a... };
}

/* template<ranges::range ... Ts> */
/* constexpr auto enumerate(Ts&& ... a) */
/* { */
/* 	return zip(ranges::views::iota(0), a ...); */
/* } */

} // end of namespace util
