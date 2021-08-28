#pragma once

#include <cstdint>
#include <vector>
#include <type_traits>
#include <limits>

#include "util/bit.hpp"

namespace containers{

template<class T>
static constexpr auto number_of_bits = std::numeric_limits<T>::digits;

struct signed_flag{};
struct unsigned_flag{};

/**
  * given a bitsize N, this struct determines which std integers must me used,
  * for instance,
  * underlying_integer<8>::type = int8_t
  * underlying_integer<32>::type = int32_t
  * underlying_integer<32>::type = int32_t
  * underlying_integer<64, unsigned_flag>::type = uint64_t
  */
template<uint8_t N, class S = signed_flag>
struct underlying_integer
{

	static constexpr auto is_signed =
		(std::is_same_v<S, signed_flag> or std::is_same_v<S, void>);

	static constexpr auto is_unsigned = std::is_same_v<S, unsigned_flag>;

	static_assert(
		N > 0 && N <= number_of_bits<uint64_t>,
		"underlying_integer integer only supports 1 to 64 bits");

	static_assert(is_signed xor is_unsigned);

	using unsigned_type = std::conditional_t<
		(N <= number_of_bits<uint8_t>),
		uint8_t,
		std::conditional_t<
			(N <= number_of_bits<uint16_t>),
			uint16_t,
			std::conditional_t<
				(N <= number_of_bits<uint32_t>),
				uint32_t,
				uint64_t
			>
		>
	>;

	using type = std::conditional_t<
		is_signed,
		std::make_signed_t<unsigned_type>,
		unsigned_type
	>;
};

template<
	uint8_t N,
	class S = signed_flag,
	class Container = std::vector<typename underlying_integer<N, S>::type>>
class tight_integer_container {
private:
	/**
	  * compute the number of underlying integers to hold `n_elements` of 
	  * integers with `N` bits
	  */
	[[nodiscard]]
	constexpr auto compute_number_of_underint(size_t n_elements) const
		noexcept
	{
		size_t const n_bits = n_elements*N;
		size_t const n_ints = n_bits/N + ((n_bits % N) > 0);
		return n_ints;
	}

	/**
	  * computes the number of tight ints that could be placed in
	  * a memory allocated for `n_elements` of underlying integers
	  */
	[[nodiscard]]
	constexpr auto compute_number_of_tight_ints(size_t n_elements) const
		noexcept
	{
		size_t const n_bits = n_elements*bits_per_underlying_integer;
		size_t const n_ints = n_bits / N;
		return n_ints;
	}

public:
	using container_t = Container;
	using underlying_integer_t = typename container_t::value_type;

	static constexpr auto number_of_bits_per_element = N;
	static constexpr bool is_signed = std::is_same_v<S, signed_flag>;

	template<class TightContainer>
	class element_wrapper{
	private:
		constexpr auto compute_bit_index()
		{
			return number_of_bits_per_element * m_tight_index;
		}

		constexpr auto compute_underlying_index(auto bit_index)
		{
		}

		constexpr void set_value(underlying_integer_t x)
		{
			auto const bi = compute_bit_index();
		}

	public:
		auto& operator=(element_wrapper&& e)
		{
			return *this;
		}

	private:
		TightContainer* m_container_ptr = nullptr;
		uint64_t m_tight_index = 0;
	};

	static constexpr auto underlying_integer_size = sizeof(
		underlying_integer_t);

	static constexpr auto bits_per_underlying_integer = std::numeric_limits<
		std::make_unsigned_t<underlying_integer_t>
	>::digits;

	/**
	  * resize the container to hold n_elements with N bits
	  */
	constexpr auto resize(size_t n_elements)
		noexcept(noexcept(m_data.resize(size_t{})))
	{
		size_t const n = compute_number_of_underint(n_elements);
		m_size = n_elements;
		return m_data.resize(n);
	}

	/**
	  * @return number of elements of N bits the container can hold
	  */
	constexpr auto size() const
		noexcept(noexcept(m_data.size()))
	{
		return m_size;
	}

	/**
	  * @return the number of elements that can be held in currently
	  * allocated storage
	  */
	[[nodiscard]]
	constexpr auto capacity() const noexcept
	{
		return compute_number_of_tight_ints(m_data.capacity());
	}

	/**
	  * clears the contents
	  */
	constexpr auto clear()
		noexcept(noexcept(m_data.clear()))
	{
		m_size = 0;
		return m_data.clear();
	}

	/**
	  *  reserves storage for `n_elements` with `N` bits
	  */
	constexpr auto reserve(size_t n_elements)
		noexcept(noexcept(m_data.reserve(n_elements)))
	{
		m_data.reserve(compute_number_of_underint(n_elements));
	}

	/**
	  * checks whether the container is empty 
	  */
	constexpr auto empty() const
	{
		return m_size == 0;
	}

private:
	container_t m_data;
	size_t m_size = 0;
};


} // end of namespace containers
