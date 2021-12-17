#pragma once

/* #include <vector> */

#include "util/bit-adaptors.hpp"
#include "util/misc.hpp"

namespace containers{

/**
  * this adaptor adapts a contiguously memory location (abstracted as a
  * std::span) as a bit container
  */
template<std::integral T>
class bit_container_adaptor{
public:

	template<std::integral Q>
	class iterator{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Q;
		using difference_type = int64_t;
		using reference = util::element_bit_reference<Q>;
		using pointer = iterator;

		constexpr iterator() noexcept = default;
		constexpr iterator(iterator const&) noexcept = default;
		constexpr iterator(iterator&&) noexcept = default;

		template<uint64_t N>
		constexpr explicit iterator(
			std::span<Q, N> s,
			int64_t bit_index) noexcept
			: m_reference(s, bit_index)
		{}

		constexpr iterator& operator=(iterator const& other) noexcept
		{
			m_reference.real_assignment(other.m_reference);
			return *this;
		}

		constexpr iterator& operator=(iterator&& other) noexcept
		{
			m_reference.real_assignment(std::move(other.m_reference));
			return *this;
		}

		constexpr auto operator*() const
		{
			return m_reference;
		}

		constexpr iterator& operator++()
		{
			return (*this) += 1;
		}

		constexpr iterator& operator--()
		{
			return (*this) += -1;
		}

		constexpr iterator operator++(int)
		{
			auto it = *this;
			++(*this);
			return it;
		}

		constexpr iterator operator--(int)
		{
			auto it = *this;
			--(*this);
			return it;
		}

		friend constexpr iterator operator+(iterator a, int64_t scalar)
		{
			return a += scalar;
		}

		friend constexpr iterator operator-(iterator a, int64_t scalar)
		{
			return a += (-scalar);
		}

		constexpr iterator& operator+=(int64_t scalar)
		{
			this->m_reference.bit_index += scalar;
			return *this;
		}

		constexpr auto operator<=>(iterator const& other)
		{
			return (*this) - other;
		}

		friend constexpr int64_t operator-(iterator a, iterator b)
		{
			return a.m_reference.bit_index - b.m_reference.bit_index;
		}

		friend constexpr bool operator==(iterator a, iterator b)
		{
			auto to_sig = util::make_static_cast<int64_t>();
			auto const max = to_sig(a.max_index());
			if(max != to_sig(b.max_index()))
				return false;

			if(!a.m_reference.is_same_reference(b.m_reference))
				return false;

			return (a.m_reference.bit_index
				== a.m_reference.bit_index)
				|| (a.m_reference.bit_index < 0 &&
				b.m_reference.bit_index < 0)
				|| (a.m_reference.bit_index >= max &&
				b.m_reference.bit_index >= max);
		}

		friend constexpr bool operator!=(iterator a, iterator b)
		{
			return !(a == b);
		}
	private:
		constexpr auto max_index() const
		{
			return m_reference.get_data().size() * n_bits_per_element;
		}

		util::element_bit_reference<Q> m_reference;
	};

	static constexpr auto n_bits_per_element =
		util::element_bit_reference<T>::n_bits_per_element;

	template<uint64_t N>
	constexpr explicit
		bit_container_adaptor(std::span<T, N> a_data) noexcept
		: m_data(a_data)
	{}

	template<uint64_t N>
	constexpr explicit
	bit_container_adaptor(std::array<T, N> const& a_data) noexcept
		: m_data(a_data)
	{}

	template<uint64_t N>
	constexpr explicit
	bit_container_adaptor(std::array<T, N>& a_data) noexcept
		: m_data(a_data)
	{}

	constexpr explicit
	bit_container_adaptor(T& a_data) noexcept
		: m_data(&a_data, 1)
	{}

	constexpr auto size() const
	{
		return m_data.size() * n_bits_per_element;
	}

	constexpr auto operator[](auto index)
	{
		return *(begin() + index);
	}

	constexpr auto begin() noexcept
	{
		return iterator{ m_data, 0 };
	}

	constexpr auto end() noexcept
	{
		return iterator{ m_data, static_cast<int64_t>(size()) };
	}

	constexpr auto begin() const noexcept
	{
		return iterator{ m_data, 0 };
	}

	constexpr auto end() const noexcept
	{
		return iterator{ m_data, static_cast<int64_t>(size()) };
	}

	constexpr auto rbegin() noexcept
	{
		return iterator{ m_data, size() };
	}

	constexpr auto rend() noexcept
	{
		return iterator{ m_data, -1 };
	}

	constexpr auto rbegin() const noexcept
	{
		return iterator{ m_data, static_cast<int64_t>(size()) };
	}

	constexpr auto rend() const noexcept
	{
		return iterator{ m_data, -1 };
	}

private:
	std::span<T> m_data = nullptr;
};

} // end of namespace containers
