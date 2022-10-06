#pragma once

#include <array>
#include <ranges>

#include "containers/bit-container-adaptor.hpp"

namespace containers{

namespace rgs = std::ranges;
namespace vws = std::views;

template<uint64_t N>
class bitset{
public:
	using underint_t = uint8_t;
	static constexpr auto n_underbits = util::number_of_bits<underint_t>();
	static constexpr auto n_bytes = (N%n_underbits == 0)
		? (N/n_underbits)
		: (N/n_underbits + 1UL);

	using adaptor_t = bit_container_adaptor<uint8_t>;
	using underlying_array_t = std::array<uint8_t, n_bytes>;

	constexpr auto size() const
	{
		return N;
	}

	constexpr auto operator[](auto index)
	{
		return m_adaptor[index];
	}

	constexpr auto begin() noexcept
	{
		return m_adaptor.begin();
	}

	constexpr auto end() noexcept
	{
		return m_adaptor.begin() + N;
	}

	constexpr auto begin() const noexcept
	{
		return m_adaptor.begin();
	}

	constexpr auto end() const noexcept
	{
		return m_adaptor.begin() + N;
	}

	constexpr auto rbegin() noexcept
	{
		return m_adaptor.rbegin() + (m_adaptor.size() - N);
	}

	constexpr auto rend() noexcept
	{
		return m_adaptor.rend();
	}

	constexpr auto rbegin() const noexcept
	{
		return m_adaptor.rbegin() + (m_adaptor.size() - N);
	}

	constexpr auto rend() const noexcept
	{
		return m_adaptor.rend();
	}

private:
	underlying_array_t m_data;
	adaptor_t m_adaptor = adaptor_t{m_data};
};

template<uint64_t N>
constexpr void add_one(bitset<N>& bs)
{
	for(auto&& bi : bs | vws::reverse)
	{
		if(bi == 0){
			bi = 1;
			break;
		}else{
			bi = 0;
		}
	}
}

} // end of namespace containers
