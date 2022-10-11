#pragma once

#include <utility>
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

	friend class element_bit_reference<std::remove_const_t<T> const>;

	template<uint64_t N>
	constexpr explicit 
	element_bit_reference(std::span<T, N> s, int64_t index)
		noexcept
		: m_data(s), bit_index{index}
	{}

	constexpr explicit
		element_bit_reference(T& s, int64_t index) noexcept
		: m_data(&s, 1), bit_index{index}
	{}

	template<uint64_t N>
	constexpr element_bit_reference(std::span<T, N> s) noexcept
		: element_bit_reference(s, 0L)
	{}

	constexpr element_bit_reference(T& s) noexcept
		: element_bit_reference(&s, 1L)
	{}

	constexpr auto n_bit_indices() const
	{
		return m_data.size() * n_bits_per_element;
	}

	template<std::integral U>
	constexpr auto& operator=(U const& e) const
		requires (not std::is_const_v<T>)
	{
		auto&& [ v, b ] = compute_indices();
		m_data[v] = set_bit(m_data[v], b, e, bit_order::leftmost{});
		return *this;
	}

	constexpr auto& operator=(T const& e) const
		requires (not std::is_const_v<T>)
	{
		auto&& [ v, b ] = compute_indices();
		m_data[v] = set_bit(m_data[v], b, e, bit_order::leftmost{});
		return *this;
	}

	constexpr auto& operator=(element_bit_reference const& e) const
		requires (not std::is_const_v<T>)
	{
		return *this = e.get_bit_value();
	}

	constexpr auto& operator=(element_bit_reference&& e) const
		requires (not std::is_const_v<T>)
	{
		return *this = e.get_bit_value();
	}

	constexpr auto& operator=(
		element_bit_reference<std::remove_const_t<T>> const& e)
		requires (std::is_const_v<T>)
	{
		m_data = e.m_data;
		bit_index = e.bit_index;
		return *this;
	}

	constexpr auto& operator=(
		element_bit_reference<std::remove_const_t<T>>&& e)
		requires (std::is_const_v<T>)
	{
		m_data = std::move(e.m_data);
		bit_index = std::exchange(e.bit_index, 0LL);
		return *this;
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
		if(bit_index < 0)
			throw std::logic_error("bit index cannot be negative");
		auto const vector_index = bit_index / n_bits_per_element;
		auto const index = bit_index % n_bits_per_element;
		return std::tuple{
			to_unsigned(vector_index),
			to_unsigned(index)
		};
	}

	std::span<T> m_data;
public:
	int64_t bit_index = 0;
};

} // end of namespace util

template<
	std::integral T,
	std::integral U,
	template<class> class TQ,
	template<class> class TU>
struct std::basic_common_reference<util::element_bit_reference<T>, U, TQ, TU>{
	using type = std::common_reference_t<T, U>;
};

template<
	std::integral T,
	std::integral U,
	template<class> class TQ,
	template<class> class TU>
struct std::basic_common_reference<U, util::element_bit_reference<T>, TQ, TU>{
	using type = typename std::basic_common_reference<
		util::element_bit_reference<T>, U, TQ, TU
	>::type;
};
