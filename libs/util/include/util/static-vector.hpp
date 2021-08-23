#pragma once

#include <array>
#include <utility>

namespace util{

template<class T, uint64_t N>
class static_vector : public std::array<T, N>{
public:

	constexpr void push_back(T const& elem)
	{
		if(m_next == N){
			throw std::runtime_error(
				"attempt push over capacity in static_vector"
			);
		}


		(*this)[m_next++] = elem;
	}

	constexpr auto size() const noexcept
	{
		return m_next;
	}

	constexpr auto capacity() const noexcept
	{
		return N;
	}

	constexpr auto clear() noexcept
	{
		m_next = 0;
	}


	constexpr auto end() noexcept
	{
		return begin(*this) + m_next;
	}

	constexpr auto end() const noexcept
	{
		return cbegin(*this) + m_next;
	}

private:
	uint64_t m_next = 0;
};

} // end of namespace util

