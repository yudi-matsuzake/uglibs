#pragma once

#include <cinttypes>
#include <vector>
#include "util/ranges.hpp"

namespace containers {

/**
  * @brief ring_vector is a rotation vector with constant max size
  */
template<class T,
	class Allocator = std::allocator<T>,
	class VectorType = std::vector<T, Allocator>>
class ring_vector {
private:
	template<class RingVector>
	static auto make_view(RingVector* ptr)
	{
		auto& vec = ptr->m_vector;
		if(vec.size() != ptr->m_max_size) {
			return rg::v3::concat_view(
				rg::subrange(vec.begin(), vec.end()),
				rg::subrange(vec.begin(), vec.begin())
			);
		}

		return rg::v3::concat_view(rg::subrange(
				vec.begin() + ptr->m_next,
				vec.end()
			),
			rg::subrange(
				vec.begin(),
				vec.begin() + ptr->m_next
			)
		);
	}

public:
	constexpr explicit ring_vector(uint64_t max_size)
		: m_max_size(max_size)
	{
		m_vector.reserve(max_size);
	}

	[[nodiscard]] constexpr auto size() const
	{
		return m_vector.size();
	}

	[[nodiscard]] constexpr auto max_size() const
	{
		return m_max_size;
	}

	[[nodiscard]] constexpr auto is_full() const
	{
		return size() == max_size();
	}


	/**
	 * push_back function is similar to the `std::vector::pushback`
	 * the difference is that this function returns a optional `opt`,
	 * and the value of `opt` is the value of the element of the vector
	 * that was overwritten, if none value was overwritten, the opt is
	 * std::optnull
	 *
	 * @see push_back(T&& value)
	 */
	constexpr auto push_back(T const& value)
	{
		std::optional<T> opt = std::nullopt;
		if(m_vector.size() == m_max_size) {
			opt = m_vector[m_next];
			m_vector[m_next] = value;
		} else {
			m_vector.push_back(value);
		}

		m_next = (m_next + 1) % m_max_size;

		return opt;
	}

	constexpr auto push_back(T&& value)
	{
		std::optional<T> opt = std::nullopt;
		if(m_vector.size() == m_max_size) {
			opt = std::exchange(
				m_vector[m_next],
				std::move(value)
			);
		} else {
			m_vector.push_back(std::move(value));
		}

		m_next = (m_next + 1) % m_max_size;
		return opt;
	}

	[[nodiscard]] constexpr auto& front()
	{
		if(m_vector.size() == m_max_size)
			return m_vector[m_next];
		return m_vector.front();
	}

	[[nodiscard]] constexpr auto const& front() const
	{
		if(m_vector.size() == m_max_size)
			return m_vector[m_next];
		return m_vector.front();
	}

	[[nodiscard]] auto view()
	{
		return make_view(this);
	}

	[[nodiscard]] auto view() const
	{
		return make_view(this);
	}

private:
	VectorType m_vector;
	uint64_t m_max_size;
	uint64_t m_next = 0UL;
};

} // end of namespace containers
