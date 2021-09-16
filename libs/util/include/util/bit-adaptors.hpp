#pragma once

#include "util/bit.hpp"

namespace util{

/**
  * this class points to a bit in memory and wraps it to handle
  * like a int reference
  */
template<std::integral T>
class element_bit_reference{
public:
	static constexpr auto n_bits_per_element = util::number_of_bits<T>();

	constexpr element_bit_reference() noexcept = default;
	constexpr element_bit_reference(element_bit_reference&&)
		noexcept = default;
	constexpr element_bit_reference(element_bit_reference const&)
		noexcept = default;

	template<uint64_t N>
	constexpr explicit 
	element_bit_reference(std::span<T, N> s, int64_t index = 0)
		noexcept
		: m_data(s), bit_index{index}
	{}

	constexpr explicit
		element_bit_reference(T& s, int64_t index = 0) noexcept
		: m_data(&s, 1), bit_index{index}
	{}

	constexpr auto n_bit_indices() const
	{
		return m_data.size() * n_bits_per_element;
	}

	constexpr auto& operator=(T e) const noexcept
		requires (not std::is_const_v<T>)
	{
		auto&& [ v, b ] = compute_indices();
		m_data[v] = set_bit(m_data[v], b, e, bit_order::leftmost{});
		return *this;
	}

	constexpr auto& operator=(element_bit_reference const& e) const noexcept
		requires (not std::is_const_v<T>)
	{
		return *this = e.get_bit_value();
	}

	constexpr auto& operator=(element_bit_reference&& e) const noexcept
		requires (not std::is_const_v<T>)
	{
		return *this = e.get_bit_value();
	}

	constexpr auto get_bit_value() const
	{
		auto&& [ v, b ] = compute_indices();
		return get_bit(m_data[v], b, bit_order::leftmost{});
	}

	constexpr auto get_data() const
	{
		return m_data;
	}

	constexpr bool is_same_reference(element_bit_reference const& o) const
	{
		return (this == &o)
			|| (m_data.data() == o.m_data.data()
			    && bit_index == o.bit_index);
	}

	template<std::integral Q>
	constexpr bool operator==(Q rhs)
	{
		return get_bit_value() == rhs;
	}

	template<std::integral Q>
	constexpr bool operator==(element_bit_reference<Q> const& rhs)
	{
		return get_bit_value() == rhs.get_bit_value();
	}

	template<std::integral Q>
	constexpr bool operator!=(element_bit_reference<Q> const& rhs)
	{
		return !(get_bit_value() == rhs.get_bit_value());
	}

	operator T() const
	{
		return get_bit_value();
	}

	constexpr void real_assignment(element_bit_reference const& other)
		noexcept
	{
		m_data = other.m_data;
		bit_index = other.bit_index;
	}

	constexpr void real_assignment(element_bit_reference&& other)
		noexcept
	{
		m_data = other.m_data;
		bit_index = other.bit_index;
	}

private:
	constexpr auto compute_indices() const
	{
		auto const vector_index = bit_index / n_bits_per_element;
		auto const index = bit_index % n_bits_per_element;
		return std::tuple{ vector_index, index };
	}

	std::span<T> m_data;
public:
	int64_t bit_index = 0;
};

} // end of namespace util

template<class T, template<class> class TQ, template<class> class TU>
struct std::basic_common_reference<
	util::element_bit_reference<T>,
	T,
	TQ,
	TU>{
	using type = util::element_bit_reference<T>;
};

template<class T, template<class> class TQ, template<class> class TU>
struct std::basic_common_reference<
	T,
	util::element_bit_reference<T>,
	TQ,
	TU>{
	using type = util::element_bit_reference<T>;
};

