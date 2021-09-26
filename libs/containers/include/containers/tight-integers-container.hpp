#pragma once

#include <ranges>
#include <cstdint>
#include <vector>
#include <type_traits>
#include <limits>

#include "util/bit.hpp"
#include "util/integers.hpp"
#include "containers/bit-container-adaptor.hpp"

namespace containers{

namespace rgs = std::ranges;
namespace vws = std::views;

namespace detail{

namespace rgs = std::ranges;
namespace vws = std::views;

/**
  * given a bitsize N, this struct determines which std integers must me used,
  * for instance,
  * underlying_integer<8>::type = int8_t
  * underlying_integer<32>::type = int32_t
  * underlying_integer<32>::type = int32_t
  * underlying_integer<64, unsigned_flag>::type = uint64_t
  */
template<
	uint8_t N,
	util::signess S = util::signed_flag,
	util::mutability M = util::mutable_flag>
struct underlying_integer
{

	static constexpr auto is_signed = std::is_same_v<S, util::signed_flag>;
	static constexpr auto is_unsigned = std::is_same_v<
		S, util::unsigned_flag>;

	static constexpr auto is_const = std::is_same_v<M, util::const_flag>;
	static constexpr auto is_mutable = std::is_same_v<
		M, util::mutable_flag>;

	static_assert(
		N > 0 && N <= util::number_of_bits<uint64_t>(),
		"underlying_integer integer only supports 1 to 64 bits");

	static_assert(
		N > 1 || is_unsigned,
		"underlying_integer does not support binary signed integer");

	static_assert(is_signed xor is_unsigned);
	static_assert(is_const xor is_mutable);

	using unsigned_type = std::conditional_t<
		(N <= util::number_of_bits<uint8_t>()),
		uint8_t,
		std::conditional_t<
			(N <= util::number_of_bits<uint16_t>()),
			uint16_t,
			std::conditional_t<
				(N <= util::number_of_bits<uint32_t>()),
				uint32_t,
				uint64_t
			>
		>
	>;

	using type_with_signess = std::conditional_t<
		is_signed,
		std::make_signed_t<unsigned_type>,
		unsigned_type
	>;

	using type = std::conditional_t<
		is_const,
		std::add_const_t<unsigned_type>,
		type_with_signess
	>;
};

template<uint8_t N, util::signess S, util::mutability M>
using underint_t = typename underlying_integer<N, S, M>::type;

template<uint8_t N, util::signess S, util::mutability M>
using default_allocator_t = std::allocator<underint_t<N, S, M>>;

template<
	uint8_t N,
	util::signess S,
	util::mutability M,
	class A = default_allocator_t<N, S, M>>
using default_container_t = std::vector<underint_t<N, S, M>, A>;

template<
	uint8_t N,
	class S = util::signed_flag,
	class M = util::mutable_flag,
	class A = default_allocator_t<N, S, M>,
	class Container = default_container_t<N, S, M, A>>
struct tight_integer_container_common{

	using container_t = Container;
	using underlying_integer_t = typename container_t::value_type;
	using underlying_integer_info = underlying_integer<N, S, M>;

	static constexpr auto number_of_bits_per_element = N;
	static constexpr bool is_signed = underlying_integer_info::is_signed;
	static constexpr bool is_unsigned = !is_signed;

	static constexpr auto underlying_integer_size = sizeof(
			underlying_integer_t);

	static constexpr auto bits_per_underlying_integer = std::numeric_limits<
		std::make_unsigned_t<underlying_integer_t>
		>::digits;

	static constexpr underlying_integer_t min_value()
		requires is_unsigned
	{
		return 0;
	}

	static constexpr underlying_integer_t max_value()
		requires is_unsigned
	{
		auto n = bits_per_underlying_integer - N;
		underlying_integer_t x = ~underlying_integer_t{0};
		x <<= n;
		x >>= n;
		return x;
	}

	static constexpr underlying_integer_t max_value()
		requires is_signed
	{
		using tighter_container = tight_integer_container_common<
			N - 1, util::unsigned_flag>;

		return tighter_container::max_value();
	}

	static constexpr underlying_integer_t min_value()
		requires is_signed
	{
		using tighter_container = tight_integer_container_common<
			N - 1, util::unsigned_flag>;
		return -(tighter_container::max_value() + 1);
	}

};

template<
	uint8_t N,
	class S = util::signed_flag,
	class M = util::mutable_flag,
	class A = default_allocator_t<N, S, M>,
	class Container = default_container_t<N, S, M, A>>
class tight_integer_container :
	public tight_integer_container_common<N, S, M, A, Container>{
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

	using reference = tight_element_reference<tight_integer_container>;
	using iterator = tight_iterator<tight_integer_container>;
	using const_iterator = tight_iterator<const tight_integer_container>;

	using container_t = Container;
	using underlying_integer_t = typename container_t::value_type;

	using underlying_integer_info = underlying_integer<N, S, M>;

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
		static constexpr bool is_underlying_const = std::is_const_v<
			underlying_integer_t>;

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
			real_assigment(v);
		}

		constexpr
		tight_element_reference(tight_element_reference&& v) noexcept
		{
			real_assigment(std::forward<tight_element_reference>(v));
		}

		constexpr
		auto& operator=(underlying_integer_t v) noexcept
			requires (not is_underlying_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference const& v) noexcept
			requires (not is_underlying_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference&& v) noexcept
			requires (not is_underlying_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(underlying_integer_t v) const noexcept
			requires (not is_underlying_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference const& v) const noexcept
			requires (not is_underlying_const)
		{
			return from_underlying_integer(v);
		}

		constexpr
		auto& operator=(tight_element_reference&& v) const noexcept
			requires (not is_underlying_const)
		{
			return from_underlying_integer(v);
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
			auto const both_are_end =
				[siz = m_container_ptr->size(),
				i = tight_index,
				o = other.tight_index]
			{
				return  i >= siz && o >= siz;
			};

			auto const both_before_begin =
				[i = tight_index, o = other.tight_index]
			{
				return  i < 0 && o < 0;
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

		auto real_assigment(tight_element_reference const& other)
		{
			m_container_ptr = other.m_container_ptr;
			tight_index = other.tight_index;
		}

		auto real_assigment(tight_element_reference&& other)
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
		using reference = tight_element_reference<tight_integer_container>;
		using pointer = tight_iterator;

		constexpr tight_iterator() noexcept = default;

		explicit constexpr
		tight_iterator(TightContainer* a_ptr, uint64_t index)
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
			m_reference.real_assigment(other.m_reference);
			return *this;
		}

		constexpr tight_iterator& operator=(tight_iterator&& other)
		{
			m_reference.real_assigment(other.m_reference);
			return *this;
		}

		constexpr reference operator*() const
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

		constexpr tight_iterator operator++(int) const
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

		constexpr auto operator[](int64_t index) const noexcept
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

		friend constexpr bool operator==(tight_iterator a, tight_iterator b)
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

	constexpr tight_integer_container() = default;

	/** 
	  * creates a tight container with `n_elements` tight elements
	  */
	constexpr explicit tight_integer_container(uint64_t n_elements)
		: m_data(compute_number_of_underint(n_elements)),
		  m_size(n_elements)
	{}

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
	  * access element at index `index`
	  */
	constexpr auto operator[](int64_t index)
	{
		return tight_element_reference{ this, index };
	}

	constexpr auto operator[](int64_t index) const
	{
		return tight_element_reference{ this, index };
	}

	constexpr auto begin()
		noexcept
	{
		return tight_iterator(this, 0);
	}

	constexpr auto end()
		noexcept
	{
		return tight_iterator{ this, size() };
	}

	constexpr auto begin() const
		noexcept
	{
		return tight_iterator(this, 0);
	}

	constexpr auto end() const
		noexcept
	{
		return tight_iterator(this, size());
	}

	constexpr auto rbegin()
		noexcept
	{
		return tight_iterator{ this, size() - 1 };
	}

	constexpr auto rend()
		noexcept
	{
		return tight_iterator{ this, -1 };
	}

	constexpr auto rbegin() const
		noexcept
	{
		return tight_iterator{ this, size() - 1 };
	}

	constexpr auto rend() const
		noexcept
	{
		return tight_iterator{ this, -1 };
	}

private:
	container_t m_data;
	size_t m_size = 0;
};

template<class S, class M, class A, class Container>
class tight_integer_container<8, S, M, A, Container> :
	public tight_integer_container_common<8, S, M, A, Container>,
	public Container
{
public:
	using Container::Container;
};

template<class S, class M, class A, class Container>
class tight_integer_container<16, S, M, A, Container> :
	public tight_integer_container_common<16, S, M, Container>,
	public Container
{
public:
	using Container::Container;
};

template<class S, class M, class A, class Container>
class tight_integer_container<32, S, M, A, Container> :
	public tight_integer_container_common<32, S, M, Container>,
	public Container
{
public:
	using Container::Container;
};

template<class S, class M, class A, class Container>
class tight_integer_container<64, S, M, A, Container> :
	public tight_integer_container_common<64, S, M, Container>,
	public Container
{
public:
	using Container::Container;
};

} // end of namespace detail

using detail::underlying_integer;

template<util::arbitrary_integer_or_integral T>
struct integer_info;

template<util::arbitrary_integer T>
struct integer_info<T>{
	static auto constexpr n_bits = T::n_bits;
	using mutability = util::to_mutability_t<T>;
	using signess = T::signess;
};

template<std::integral T>
struct integer_info<T>{
	static auto constexpr n_bits = util::number_of_bits<T>();
	using mutability = util::to_mutability_t<T>;
	using signess = util::to_signess_t<T>;
};

template<class T>
using tight_allocator_t = typename detail::default_allocator_t<
	integer_info<T>::n_bits,
	typename integer_info<T>::signess,
	typename integer_info<T>::mutability
>;

template<class T>
using tight_container_t = typename detail::default_container_t<
	integer_info<T>::n_bits,
	typename integer_info<T>::signess,
	typename integer_info<T>::mutability
>;

template<
	util::arbitrary_integer_or_integral T,
	class A = tight_allocator_t<T>,
	class C = tight_container_t<T>>
class tight_integer_container : public detail::tight_integer_container<
	integer_info<T>::n_bits,
	typename integer_info<T>::signess,
	typename integer_info<T>::mutability,
	A,
	C>{
public:
	using base_t = detail::tight_integer_container<
		integer_info<T>::n_bits,
		typename integer_info<T>::signess,
		typename integer_info<T>::mutability,
		A, 
		C>;

	using base_t::base_t;
	using base_t::operator=;
};

} // end of namespace containers
