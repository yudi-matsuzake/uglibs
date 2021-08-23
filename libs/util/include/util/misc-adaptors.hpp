/** @file misc-adaptors.hpp
  * Implementation of adaptors to help common 
  * objects manipulation
  */

#pragma once

#include <optional>
#include <tuple>
#include <utility>

#include "util/misc.hpp"

namespace util{

template<class T>
class range
{
public:
	constexpr explicit range(T a_begin, T a_end)
		: m_begin(a_begin), m_end(a_end)
	{}

	constexpr auto begin() noexcept
	{
		return m_begin;
	}

	constexpr auto end() noexcept
	{
		return m_end;
	}

	constexpr auto begin() const noexcept
	{
		return m_begin;
	}

	constexpr auto end() const noexcept
	{
		return m_end;
	}

protected:
	T m_begin;
	T m_end;
};

template<class T>
constexpr auto make_range(T&& unknown)
{
	return range{ std::begin(unknown), std::end(unknown) };
}

template<class T = uint32_t>
class seq_iterator{
public:
	constexpr seq_iterator()
		: m_infinity(true)
	{}

	constexpr seq_iterator(T b)
		: m_counter(b)
	{}

	constexpr auto& operator++()
	{
		m_counter++;
		return *this;
	}

	constexpr auto operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	constexpr auto& operator--()
	{
		m_counter--;
		return *this;
	}

	constexpr auto operator--(int)
	{
		auto copy = *this;
		--(*this);
		return copy;
	}

	constexpr T operator*()
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
class seq_adaptor : public range<seq_iterator<T>> {
public:

	constexpr seq_adaptor()
		: range<seq_iterator<T>>(
			seq_iterator<T>(0),
			seq_iterator<T>()
		  )
	{}

	constexpr seq_adaptor(T e)
		: range<seq_iterator<T>>(
			seq_iterator<T>(0),
			seq_iterator<T>(e)
		  )
	{}

	constexpr seq_adaptor(T b, T e)
		: range<seq_iterator<T>>(
			seq_iterator<T>(b),
			seq_iterator<T>(e)
		  )
	{}
};

template<class T, class ... Ts>
class zip_iterator{
public:
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
		auto t = tuple_transform(m_iterators, [](auto&& i) { return *i; });
		return t;
	}

	constexpr auto operator==(zip_iterator<T, Ts...> const& other) const
	{
		return std::get<0>(m_iterators) == std::get<0>(other.m_iterators);
	}

	constexpr auto operator!=(zip_iterator<T, Ts...> const& other) const
	{
		return !(*this == other);
	}

protected:
	std::tuple<T, Ts...> m_iterators;
};

template<class T, class ... Ts>
class zip_adaptor{
public:
	using iterator_type = zip_iterator<T, Ts...>;
public:
	constexpr explicit zip_adaptor(range<T>&& t, range<Ts>&& ... ts)
		: m_ranges(std::make_tuple(t, ts...))
	{}

	constexpr auto begin() noexcept
	{
		return std::make_from_tuple<iterator_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::begin(i); }
			)
		);
	}

	constexpr auto end() noexcept
	{
		return std::make_from_tuple<iterator_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::end(i); }
			)
		);
	}

	constexpr auto begin() const noexcept
	{
		return std::make_from_tuple<iterator_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::begin(i); }
			)
		);
	}

	constexpr auto end() const noexcept
	{
		return std::make_from_tuple<iterator_type>(
			tuple_transform(
				m_ranges,
				[](auto&& i){ return std::end(i); }
			)
		);
	}

protected:
	std::tuple<range<T>, range<Ts> ... > m_ranges;
};

/*
 * adaptors function
 */

template<class T, class ... Ts>
constexpr auto zip(T&& a, Ts&& ... as)
{
	return zip_adaptor{
		make_range(std::forward<T>(a)),
		make_range(std::forward<Ts>(as)) ...
	};
}

template<class ... T>
constexpr auto seq(T&& ... a)
{
	return seq_adaptor{ a... };
}

}
