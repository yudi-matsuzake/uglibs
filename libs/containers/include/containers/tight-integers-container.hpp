#pragma once

#include <ranges>
#include <cstdint>
#include <vector>
#include <type_traits>
#include <limits>

#include "util/bit.hpp"
#include "util/integers.hpp"
#include "util/underlying-integer.hpp"

#include "containers/bit-container-adaptor.hpp"

namespace containers{

namespace detail{

namespace rgs = std::ranges;
namespace vws = std::views;

template<uint8_t N, util::signess S, util::mutability M>
using underint_t = typename util::underlying_integer<N, S, M>::type;

template<uint8_t N, util::signess S, util::mutability M>
using default_allocator_t = std::allocator<underint_t<N, S, M>>;

template<
	uint8_t N,
	util::signess S,
	util::mutability M,
	class Alloc>
using default_container_t = std::vector<underint_t<N, S, M>, Alloc>;

template<
	uint8_t N,
	class S = util::signed_flag,
	class M = util::mutable_flag,
	class Alloc = default_allocator_t<N, S, M>,
	class Container = default_container_t<N, S, M, Alloc>>
struct tight_integer_container_common{

	using container_t = Container;
	using underlying_integer_t = typename container_t::value_type;
	using underlying_integer_info = util::underlying_integer<N, S, M>;

	static constexpr auto number_of_bits_per_element = N;
	static constexpr bool is_signed = underlying_integer_info::is_signed;
	static constexpr bool is_unsigned = !is_signed;

	static constexpr auto underlying_integer_size = sizeof(
			underlying_integer_t);

	static constexpr auto bits_per_underlying_integer = std::numeric_limits<
		std::make_unsigned_t<underlying_integer_t>
		>::digits;

	static constexpr underlying_integer_t min_value()
	{
		return std::numeric_limits<util::integer<N, S>>::min();
	}

	static constexpr underlying_integer_t max_value()
	{
		return std::numeric_limits<util::integer<N, S>>::max();
	}

};

template<
	uint8_t N,
	class S = util::signed_flag,
	class M = util::mutable_flag,
	class Alloc = default_allocator_t<N, S, M>,
	class Container = default_container_t<N, S, M, Alloc>>
class tight_integer_container :
	public tight_integer_container_common<N, S, M, Alloc, Container>{
private:
	/**
	  * compute the number of underlying integers to hold `n_elements` of 
	  * integers with `N` bits
	  */
	[[nodiscard]]
	constexpr auto compute_number_of_underint(size_t n_elements) const
		noexcept
	{
		size_t const n_bits = n_elements*number_of_bits_per_element;
		auto const n = bits_per_underlying_integer;
		size_t const n_ints = n_bits/n + ((n_bits % n) > 0);
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

	template<class TightContainer>
	class tight_element_reference;
	template<class TightContainer>
	class tight_iterator;

	using type = tight_integer_container;
	using container_t = Container;
	using underlying_integer_t = underint_t<N, S, M>;
	using underlying_integer_info = util::underlying_integer<N, S, M>;

	using reference = tight_element_reference<tight_integer_container>;
	using const_reference = tight_element_reference<const tight_integer_container>;
	using iterator = tight_iterator<tight_integer_container>;
	using const_iterator = tight_iterator<const tight_integer_container>;
	using value_type	= underlying_integer_t;
	using size_type		= uint64_t;
	using difference_type	= int64_t;

	using pointer		= iterator;
	using const_pointer	= const_iterator;

	using reverse_iterator	= iterator;
	using const_reverse_iterator= const_iterator;
	using allocator_type	= Alloc;

	static constexpr auto number_of_bits_per_element = N;
	static constexpr bool is_signed = underlying_integer_info::is_signed;
	static constexpr bool is_unsigned = !is_signed;

	static constexpr auto underlying_integer_size = sizeof(
		underlying_integer_t);

	static constexpr auto bits_per_underlying_integer = std::numeric_limits<
		std::make_unsigned_t<underlying_integer_t>
	>::digits;

	template<class TightContainer>
	class tight_element_reference{
	private:
		constexpr auto compute_bit_index() const
		{
			return number_of_bits_per_element * tight_index;
		}

		constexpr auto tight_container_bits() const noexcept
		{
			return bit_container_adaptor(std::span(
				m_container_ptr->m_data.data(),
				m_container_ptr->m_data.size()
			));
		}

		constexpr auto get_element_bit_range() const noexcept
		{
			auto tcb = tight_container_bits();
			auto b = tcb.begin() + compute_bit_index();
			return rgs::subrange(b, b + number_of_bits_per_element);
		}

		/**
		  * our number region from a element
		  * is the N bits of this element if the element is unsigned
		  * is the N-1 last bits of this element if the element is signed
		  *
		  */
		constexpr auto get_element_number_region_bits() const noexcept
		{
			return get_element_bit_range() | vws::drop(int{is_signed});
		}

		/**
		  * our number region from a underlying_integer type
		  * is the last N bits for unsigned and the last
		  * N-1 bits for signed
		  */
		constexpr auto get_number_region_from_underlying_type(
			underlying_integer_t& x) const noexcept
		{
			auto c = bit_container_adaptor(x);
			auto const n =
				bits_per_underlying_integer
				- (N - int{is_signed});

			return rgs::subrange(rgs::begin(c) + n, rgs::end(c));
		}

		constexpr auto get_sign_iterator() const noexcept
			requires is_signed
		{
			return get_element_bit_range().begin();
		}

		constexpr void set_sign_bit(underlying_integer_t v) const noexcept
		{
			v = static_cast<underlying_integer_t>(v&1);
			*get_sign_iterator() = v;
		}

		constexpr underlying_integer_t get_sign_bit() const noexcept
		{
			return *get_sign_iterator();
		}

		constexpr void impl_from_underlying_integer(underlying_integer_t x) const
		{
			auto e_bits = get_element_number_region_bits();
			auto v_bits = get_number_region_from_underlying_type(x);
			rgs::copy(v_bits, e_bits.begin());
			if constexpr (is_signed){
				if(x < underlying_integer_t{0})
					set_sign_bit(1);
				else
					set_sign_bit(0);
			}
		}

		constexpr auto& from_underlying_integer(underlying_integer_t x)
		{
			impl_from_underlying_integer(x);
			return *this;
		}

		constexpr auto& from_underlying_integer(underlying_integer_t x) const
		{
			impl_from_underlying_integer(x);
			return *this;
		}

		[[nodiscard]] constexpr
		underlying_integer_t to_undelying_integer() const
		{
			underlying_integer_t v = 0;
			if constexpr (is_signed){
				if(get_sign_bit())
					v = ~0;
			}

			auto v_bits = get_number_region_from_underlying_type(v);
			auto e_bits = get_element_number_region_bits();
			rgs::copy(e_bits, v_bits.begin());
			return v;
		}

	public:

		static constexpr bool is_container_const = std::is_const_v<
			TightContainer>;

		constexpr tight_element_reference() = default;

		constexpr tight_element_reference(
			TightContainer* a_ptr,
			int64_t index) noexcept
			: m_container_ptr(a_ptr),
			  tight_index(index)
		{}

		constexpr
		tight_element_reference(tight_element_reference const& v) noexcept
		{
			real_assignment(v);
		}

		constexpr
		tight_element_reference(tight_element_reference&& v) noexcept
		{
			real_assignment(std::forward<tight_element_reference>(v));
		}

		constexpr
		auto& operator=(underlying_integer_t v) noexcept
			requires (not is_container_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference const& v) noexcept
			requires (not is_container_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference&& v) noexcept
			requires (not is_container_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(underlying_integer_t v) const noexcept
			requires (not is_container_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference const& v) const noexcept
			requires (not is_container_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference&& v) const noexcept
			requires (not is_container_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference const& v) noexcept
			requires (is_container_const)
		{
			this->real_assignment(v);
			return *this;
		}

		constexpr
		auto& operator=(tight_element_reference&& v) noexcept
			requires (is_container_const)
		{
			this->real_assignment(v);
			return *this;
		}

		[[nodiscard]] constexpr
		underlying_integer_t get_value() const noexcept
		{
			return to_undelying_integer();
		}

		constexpr auto& get_container() const
		{
			return *m_container_ptr;
		}

		constexpr auto& get_container()
		{
			return *m_container_ptr;
		}

		constexpr operator underlying_integer_t() const noexcept
		{
			return get_value();
		}

		constexpr operator underlying_integer_t() noexcept
		{
			return get_value();
		}

		constexpr
		bool operator==(tight_element_reference const& other) const
			noexcept
		{
			return get_value() == other.get_value();
		}

		constexpr
		bool compare_real_reference(
			tight_element_reference const& other) const
		{
			auto const both_before_begin =
				[i = tight_index, o = other.tight_index]
			{
				return  i < 0 && o < 0;
			};

			auto const both_are_end =
				[siz = m_container_ptr->size(),
				i = static_cast<uint64_t>(tight_index),
				o = static_cast<uint64_t>(other.tight_index)]
			{
				return i >= siz && o >= siz;
			};

			return m_container_ptr == other.m_container_ptr
				&& (tight_index == other.tight_index
				|| both_before_begin()
				|| both_are_end());
		}

		constexpr
		bool operator!=(tight_element_reference const& other) const
			noexcept
		{
			return !(*this == other);
		}

		constexpr
		auto operator<=>(underlying_integer_t o) const
		{
			return this->get_value() <=> o;
		}

		constexpr
		auto operator<=>(tight_element_reference const& other) const
		{
			return *this <=> other.get_value();
		}

		auto real_assignment(tight_element_reference const& other)
		{
			m_container_ptr = other.m_container_ptr;
			tight_index = other.tight_index;
		}

		auto real_assignment(tight_element_reference&& other)
		{
			m_container_ptr = std::exchange(other.m_container_ptr, nullptr);
			tight_index = std::exchange(other.tight_index, 0);
		}

		TightContainer* get_container_ptr() const
		{
			return m_container_ptr;
		}

		TightContainer* get_container_ptr()
			requires (not std::is_const_v<TightContainer>)
		{
			return m_container_ptr;
		}

		friend constexpr void swap(
			tight_element_reference a,
			tight_element_reference b)
		{
			underlying_integer_t cup = a.get_value();
			a = b.get_value();
			b = cup;
		}

	private:
		TightContainer* m_container_ptr = nullptr;
	public:
		int64_t tight_index = 0;
	};

	template<class TightContainer>
	class tight_iterator{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = underlying_integer_t;
		using difference_type = int64_t;
		using reference = tight_element_reference<TightContainer>;
		using const_reference = tight_element_reference<TightContainer>;
		using pointer = tight_iterator;

		constexpr tight_iterator() noexcept = default;

		explicit constexpr
		tight_iterator(TightContainer* a_ptr, int64_t index)
			noexcept
			: m_reference(a_ptr, index)
		{}

		constexpr tight_iterator(tight_iterator const& other)
			noexcept
			: tight_iterator(
				other.m_reference.get_container_ptr(),
				other.m_reference.tight_index)
		{}

		constexpr tight_iterator(tight_iterator&& other)
			noexcept
			: tight_iterator(
				other.m_reference.get_container_ptr(),
				other.m_reference.tight_index)
		{}

		constexpr tight_iterator& operator=(tight_iterator const& other)
		{
			m_reference.real_assignment(other.m_reference);
			return *this;
		}

		constexpr tight_iterator& operator=(tight_iterator&& other)
		{
			m_reference.real_assignment(other.m_reference);
			return *this;
		}

		constexpr const_reference operator*() const
		{
			return m_reference;
		}

		constexpr reference operator*()
		{
			return m_reference;
		}

		constexpr tight_iterator& operator++()
		{
			return (*this) += 1;
		}

		constexpr tight_iterator& operator--()
		{
			return (*this) += -1;
		}

		constexpr tight_iterator operator++(int)
		{
			auto it = *this;
			++(*this);
			return it;
		}

		constexpr tight_iterator operator--(int)
		{
			auto it = *this;
			--(*this);
			return it;
		}

		friend constexpr tight_iterator operator+(tight_iterator a, int64_t scalar)
		{
			return a += scalar;
		}

		friend constexpr tight_iterator operator-(tight_iterator a, int64_t scalar)
		{
			return a += (-scalar);
		}

		friend constexpr tight_iterator operator+(int64_t scalar, tight_iterator a)
		{
			return a += scalar;
		}

		friend constexpr tight_iterator operator-(int64_t scalar, tight_iterator a)
		{
			return a += (-scalar);
		}

		constexpr tight_iterator& operator+=(int64_t scalar)
		{
			m_reference.tight_index += scalar;
			return *this;
		}

		constexpr tight_iterator& operator-=(int64_t scalar)
		{
			return *this += -scalar;
		}

		constexpr const_reference operator[](int64_t index) const noexcept
		{
			return m_reference.get_container()[
				m_reference.tight_index + index];
		}

		friend constexpr int64_t operator-(tight_iterator a, tight_iterator b)
		{
			auto const a_index = a.m_reference.tight_index;
			auto const b_index = b.m_reference.tight_index;
			return a_index - b_index;
		}

		friend constexpr bool operator==(
			tight_iterator const& a,
			tight_iterator const& b)
		{
			return a.m_reference.compare_real_reference(b.m_reference);
		}

		friend constexpr bool operator!=(tight_iterator a, tight_iterator b)
		{
			return !(a == b);
		}

		friend constexpr auto operator<=>(tight_iterator a, tight_iterator b)
		{
			return a.m_reference.tight_index <=> b.m_reference.tight_index;
		}

	private:
		tight_element_reference<TightContainer> m_reference;
	};

	template<class TightContainer>
	tight_iterator(TightContainer* a_ptr, int64_t index)
		-> tight_iterator<TightContainer>;

	constexpr tight_integer_container() = default;

	/** 
	  * creates a tight container with `n_elements` tight elements
	  */
	constexpr explicit tight_integer_container(const Alloc& alloc )
		noexcept(noexcept(container_t(alloc)))
		: m_data(alloc)
	{}

	constexpr explicit tight_integer_container(
		size_type count,
		const Alloc& alloc = Alloc())
		: m_data(compute_number_of_underint(count), alloc),
		  m_size(count)
	{}

	constexpr explicit tight_integer_container(
		size_type count,
		const value_type& value,
		const Alloc& alloc = Alloc())
		: tight_integer_container(count, alloc)
	{
		rgs::fill(*this, value);
	}


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

	constexpr auto push_back(underlying_integer_t e)
	{
		if(compute_number_of_underint(m_size+1) < m_data.size())
			m_data.emplace_back();
		(*this)[m_size] = e;
		++m_size;
	}

	/**
	  * access element at index `index`, throws std::out_of_range
	  */
	constexpr const_reference at(uint64_t index) const
	{
		if(index < m_size)
			return (*this)[index];

		throw std::out_of_range(
			"index out of range: " + std::to_string(index));
	}

	constexpr reference at(uint64_t index)
	{
		if(index < m_size)
			return (*this)[index];

		throw std::out_of_range(
			"index out of range: " + std::to_string(index));
	}

	/**
	  * access element at index `index`
	  */
	constexpr auto operator[](int64_t index) noexcept
	{
		return reference{ this, index };
	}

	constexpr auto operator[](int64_t index) const noexcept
	{
		return const_reference{ this, index };
	}

	constexpr auto begin()
		noexcept
	{
		return tight_iterator(this, 0);
	}

	constexpr auto end()
		noexcept
	{
		return tight_iterator{ this, static_cast<int64_t>(size()) };
	}

	constexpr auto begin() const
		noexcept
	{
		return const_iterator(this, 0);
	}

	constexpr auto end() const
		noexcept
	{
		return const_iterator(this, static_cast<int64_t>(size()));
	}

	constexpr auto cbegin() const
		noexcept
	{
		return const_iterator(this, 0);
	}

	constexpr auto cend() const
		noexcept
	{
		return const_iterator(this, static_cast<int64_t>(size()));
	}

	constexpr auto rbegin()
		noexcept
	{
		return iterator{ this, size() - 1 };
	}

	constexpr auto rend()
		noexcept
	{
		return tight_iterator{ this, -1 };
	}

	constexpr auto crbegin() const
		noexcept
	{
		return const_iterator{ this, size() - 1 };
	}

	constexpr auto crend() const
		noexcept
	{
		return const_iterator{ this, -1 };
	}

	template<
		uint8_t OtherN,
		class OtherS,
		class OtherM,
		class OtherAlloc,
		class OtherContainer>
	friend bool operator==(
		tight_integer_container const& t,
		tight_integer_container<
			OtherN,
			OtherS,
			OtherM,
			OtherAlloc,
			OtherContainer> const& other)
	{
		return rgs::equal(t, other);
	}

	template<
		uint8_t OtherN,
		class OtherS,
		class OtherM,
		class OtherAlloc,
		class OtherContainer>
	friend bool operator!=(
		tight_integer_container const& t,
		tight_integer_container<
			OtherN,
			OtherS,
			OtherM,
			OtherAlloc,
			OtherContainer> const& other)
	{
		return !(t == other);
	}

private:
	container_t m_data;
	size_t m_size = 0;
};

template<class M, class Alloc, class Container>
class tight_integer_container<1, util::unsigned_flag, M, Alloc, Container> :
	public tight_integer_container_common<
		1, util::unsigned_flag, M, Alloc, Container>,
	public Container
{
public:
	using Container::Container;
};

template<class S, class M, class Alloc, class Container>
class tight_integer_container<8, S, M, Alloc, Container> :
	public tight_integer_container_common<8, S, M, Alloc, Container>,
	public Container
{
public:
	using Container::Container;
};

template<class S, class M, class Alloc, class Container>
class tight_integer_container<16, S, M, Alloc, Container> :
	public tight_integer_container_common<16, S, M, Alloc, Container>,
	public Container
{
public:
	using Container::Container;
};

template<class S, class M, class Alloc, class Container>
class tight_integer_container<32, S, M, Alloc, Container> :
	public tight_integer_container_common<32, S, M, Alloc, Container>,
	public Container
{
public:
	using Container::Container;
};

template<class S, class M, class Alloc, class Container>
class tight_integer_container<64, S, M, Alloc, Container> :
	public tight_integer_container_common<64, S, M, Alloc, Container>,
	public Container
{
public:
	using Container::Container;
};

} // end of namespace detail

template<class T>
using tight_allocator_t = typename detail::default_allocator_t<
	util::integer_info<T>::n_bits,
	typename util::integer_info<T>::signess,
	typename util::integer_info<T>::mutability
>;

template<class T, class Alloc>
using tight_container_t = typename detail::default_container_t<
	util::integer_info<T>::n_bits,
	typename util::integer_info<T>::signess,
	typename util::integer_info<T>::mutability,
	Alloc
>;

template<
	util::arbitrary_integer_or_integral T,
	class Alloc = tight_allocator_t<T>,
	class C = tight_container_t<T, Alloc>>
class tight_integer_container : public detail::tight_integer_container<
	util::integer_info<T>::n_bits,
	typename util::integer_info<T>::signess,
	typename util::integer_info<T>::mutability,
	Alloc,
	C>{
public:
	using base_t = detail::tight_integer_container<
		util::integer_info<T>::n_bits,
		typename util::integer_info<T>::signess,
		typename util::integer_info<T>::mutability,
		Alloc, 
		C>;

	using base_t::base_t;
	using base_t::operator=;
	using integer_type = T;

	base_t& base()
	{
		return *this;
	}

	base_t const& base() const
	{
		return *this;
	}
};

template<class T>
using underint_t = typename detail::underint_t<
	util::integer_info<T>::n_bits,
	typename util::integer_info<T>::signess,
	typename util::integer_info<T>::mutability>;

} // end of namespace containers
