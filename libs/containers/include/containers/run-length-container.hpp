#pragma once

#include <bits/stdint-uintn.h>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <optional>
#include <compare>
#include <algorithm>

namespace containers{

/**
  * represents a run length item
	*/
template<class T>
struct run_length_item{

	constexpr run_length_item() noexcept = default;
	constexpr run_length_item(T const& v, uint64_t c) noexcept
		: value(v), count(c)
	{}


	T value;
	uint64_t count;
};

template<class T>
constexpr bool operator==(
	run_length_item<T> const& a,
	run_length_item<T> const& b)
{
	return a.value == b.value && a.count == b.count;
}

template<class T, class A>
using rl_default_underlying_container_t = std::vector<run_length_item<T>, A>;

/**
  * represents a run-length container for sparse arrays, with the same interface
  * as possible as the std::vector
  */
template<
	class T,
	class Alloc = std::allocator<run_length_item<T>>,
	class UnderlyingContainer = rl_default_underlying_container_t<T, Alloc>>
class run_length_container : private UnderlyingContainer {
public:
	using underling_container = UnderlyingContainer;
private:

	template<class It>
	static constexpr auto find_rl_iterator(It first, It last, uint64_t index)
	{
		constexpr auto comp = [](auto&& a, auto&& b){ return a.count <= b; };
		auto it = std::lower_bound(first, last, index, comp);
		return it;
	}

	constexpr auto find_rl_iterator(uint64_t index) const
	{
		return find_rl_iterator(base().cbegin(), base().cend(), index);
	}

	/**
	  * encapsulates the logic for run length consistent types with arrays
		*/
	template<class RLC>
	class run_length_consistent_type
	{
	public:
		constexpr auto index() const
		{
			return m_index;
		}

		[[nodiscard]] constexpr auto rl_index() const
		{
			if(m_rl_index_cache.has_value()){
				auto const rl_idx = m_rl_index_cache.value();
				auto const rl_sidx = static_cast<int64_t>(rl_idx);

				auto const b = std::cbegin(m_rlc->base());
				auto const e = std::cend(m_rlc->base());

				if(m_index >= m_rlc->base()[rl_idx].count)
					m_rl_index_cache = m_rlc->find_rl_index(b + rl_sidx + 1, e, m_index);

				else if(rl_idx > 0 && rl_idx < m_rlc->base()[rl_idx - 1].count)
					m_rl_index_cache = m_rlc->find_rl_index(b, b + rl_sidx, m_index);

			}else{
				m_rl_index_cache = m_rlc->find_rl_index(m_index);
			}

			return m_rl_index_cache.value();
		}

		constexpr run_length_consistent_type()
		{}

		constexpr run_length_consistent_type(
				RLC* a_rlc,
				uint64_t a_index)
			: m_rlc(a_rlc), m_index(a_index)
		{}

		constexpr auto rlc() const
		{
			return m_rlc;
		}

	protected:
		RLC* m_rlc = nullptr;
		uint64_t m_index = 0;
		std::optional<uint64_t> mutable m_rl_index_cache = std::nullopt;
	};

	template<class RLC> 
	class element_wrapper : public run_length_consistent_type<RLC> {
	private:
		[[nodiscard]] constexpr auto value() const
		{
			return this->rlc()->base()[this->rl_index()].value;
		}

	public:
		constexpr explicit element_wrapper(
				RLC* a_rlc,
				uint64_t a_index)
			: run_length_consistent_type<RLC>(a_rlc, a_index)
		{}

		constexpr explicit element_wrapper(element_wrapper const& e)
			: element_wrapper(e.rlc(), e.index())
		{}

		constexpr explicit element_wrapper(element_wrapper&& e)
			: element_wrapper(e.rlc(), e.index())
		{}

		constexpr explicit element_wrapper(element_wrapper& e)
			: element_wrapper(e.rlc(), e.index())
		{}

		constexpr element_wrapper& operator=(T const& e)
			requires (!std::is_const_v<RLC>)
		{
			this->rlc()->set(this->index(), e);
			return *this;
		}

		constexpr element_wrapper& operator=(T&& e)
			requires (!std::is_const_v<RLC> && !std::is_const_v<T>)
		{
			this->rlc()->set(this->index(), std::forward<T>(e));
			return *this;
		}

		constexpr element_wrapper const& operator=(T const& e) const
			requires (!std::is_const_v<RLC>)
		{
			this->rlc()->set(this->index(), e);
			return *this;
		}

		constexpr element_wrapper const& operator=(T&& e) const
			requires (!std::is_const_v<RLC> && !std::is_const_v<T>)
		{
			this->rlc()->set(this->index(), std::forward<T>(e));
			return *this;
		}

		constexpr element_wrapper& operator=(element_wrapper const& e)
			requires (!std::is_const_v<RLC>)
		{
			*this = e.value();
			return *this;
		}

		constexpr element_wrapper& operator+=(T const& e)
			requires (!std::is_const_v<RLC>)
		{
			*this = (value() + e);
			return *this;
		}

		constexpr element_wrapper& operator-=(T const& e)
			requires (!std::is_const_v<RLC>)
		{
			*this += -e;
			return *this;
		}

		constexpr element_wrapper& operator+=(element_wrapper const& e)
			requires (!std::is_const_v<RLC>)
		{
			*this += e.value();
			return *this;
		}

		constexpr element_wrapper& operator-=(element_wrapper const& e)
			requires (!std::is_const_v<RLC>)
		{
			*this -= e.value();
			return *this;
		}

		/* [[nodiscard]] constexpr operator T() const */
		/* { */
		/* 	return this->rlc()->base()[this->rl_index()].value; */
		/* } */

		[[nodiscard]] constexpr operator T const&() const
		{
			return this->rlc()->base()[this->rl_index()].value;
		}

		friend constexpr void swap(element_wrapper a, element_wrapper b)
		{
			auto cup = a.value();
			a = b.value();
			b = cup;
		}

	};

	template<class RLC>
	element_wrapper(RLC* a_rlc, uint64_t a_index) -> element_wrapper<RLC>;

public:

	template<class RLC>
	class rl_iterator : public run_length_consistent_type<RLC> {
		public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = int64_t;
		using reference = element_wrapper<RLC>;
		using pointer = rl_iterator<RLC>;

		static_assert(uint64_t{0} - 1 == std::numeric_limits<uint64_t>::max(),
			"For the sentinel iterator work properly, we must have the maximum value"
			" of uint64_t be the same value as uint64_t{0} - 1");

		static constexpr auto sentinel_value = std::numeric_limits<uint64_t>::max();

		public:
			constexpr rl_iterator()
			{}

			constexpr explicit rl_iterator(
					RLC* a_rlc,
					uint64_t a_index)
				: run_length_consistent_type<RLC>(a_rlc, a_index)
			{}

			constexpr auto operator*()
			{
				return element_wrapper(this->rlc(), this->index());
			}

			constexpr auto operator*() const
			{
				return element_wrapper(this->rlc(), this->index());
			}

			constexpr rl_iterator& operator++()
			{
				return (*this) += 1;
			}

			constexpr rl_iterator& operator--()
			{
				return (*this) += -1;
			}

			constexpr rl_iterator operator++(int)
			{
				auto it = *this;
				++(*this);
				return it;
			}

			constexpr rl_iterator operator--(int)
			{
				auto it = *this;
				--(*this);
				return it;
			}

			friend constexpr rl_iterator operator+(rl_iterator a, int64_t scalar)
			{
				return a += scalar;
			}

			friend constexpr rl_iterator operator-(rl_iterator a, int64_t scalar)
			{
				return a += (-scalar);
			}

			constexpr rl_iterator& operator+=(int64_t scalar)
			{
				if(scalar < 0){
					if(auto const abs = static_cast<uint64_t>(-scalar); abs > this->m_index)
						this->m_index = sentinel_value;
					else
						this->m_index -= abs;
				}else{
					this->m_index += static_cast<uint64_t>(scalar);
				}

				return *this;
			}

			constexpr auto operator<=>(rl_iterator const& other)
			{
				return (*this) - other;
			}

			friend constexpr int64_t operator-(rl_iterator a, rl_iterator b)
			{
				return static_cast<int64_t>(a.index()) - static_cast<int64_t>(b.index());
			}

			friend constexpr bool operator==(rl_iterator a, rl_iterator b)
			{
				if(a.rlc() != b.rlc())
					return false;

				auto const max = a.rlc()->base().back().count;
				bool a_is_out = a.index() >= max || a.index() == sentinel_value;
				bool b_is_out = b.index() >= max || b.index() == sentinel_value;

				if(a_is_out && b_is_out)
					return true;

				return a.m_index == b.m_index;
			}

			friend constexpr bool operator!=(rl_iterator a, rl_iterator b)
			{
				return !(a == b);
			}
	};

	template<class RLC>
	rl_iterator(RLC* a_rlc, uint64_t a_index) -> rl_iterator<RLC>;

	static constexpr auto sentinel_value = rl_iterator<
		run_length_container>::sentinel_value;

	template<class It>
	constexpr auto find_rl_index(It first, It last, uint64_t index) const
	{
		auto const idx = std::distance(
				std::begin(this->base()),
				this->find_rl_iterator(first, last, index)
		);

		return idx;
	}

	constexpr auto find_rl_index(uint64_t index) const
	{
		if(auto idx = find_rl_index(base().begin(), base().end(), index);
			idx < 0 || static_cast<uint64_t>(idx) >= m_size){
			throw std::out_of_range("index out of range in rl container");
		}else{
			return static_cast<uint64_t>(idx);
		}
	}

	template<class ForwardIt>
	constexpr static ForwardIt remove(ForwardIt to_remove, ForwardIt last)
	{
		auto first = to_remove;
		if (first != last)
			for(ForwardIt i = first; ++i != last; )
				*first++ = std::move(*i);
		return first;
	}

	template<class ForwardIt>
	constexpr static ForwardIt erase(
		UnderlyingContainer& v,
		ForwardIt to_erase)
	{
		auto it = remove(to_erase, v.end());
		return v.erase(it, v.end());
	}

	constexpr auto rl_item_size(uint64_t rl_index)
	{
		if(rl_index == 0)
			return base()[rl_index].count;
		return base()[rl_index].count - base()[rl_index - 1].count;
	}

	constexpr auto rl_item_first_index(uint64_t rl_index)
	{
		if(rl_index == 0)
			return rl_index;

		return base()[rl_index-1].count;
	}

	enum class set_case : int8_t {
		BEGINNING,
		ENDING,
		CHANGE,
		ADD_LEFT,
		ADD_RIGHT,
		SPLIT
	};

	template<class It>
	constexpr auto rotate(It first, It last)
	{
		std::rotate(first, first+1, last);
	}

	/**
	  * add one rl item in the base vector and puts it in the position
		* `index`
		*/
	constexpr auto add_rl_item_at(
		uint64_t index,
		containers::run_length_item<T> const& rl_item)
	{
			base().emplace_back(rl_item);
			rotate(base().rbegin(), base().rend() - static_cast<int64_t>(index));
	}

	constexpr auto remove_rl_item_at(uint64_t index)
	{
		erase(base(), base().begin() + static_cast<int64_t>(index));
	}

	/**
	  * sets the element at index `index` to `e`
	  *
	  * beginning case
	  * 	the element is in the begining of the rl item
	  * 	and it is not the first rl item
	  * 	and `e` is equal to the value of previous rl item
	  *
	  * ending case
	  * 	the element is in the ending of the rl item
	  * 	and it is not the last rl item
	  * 	and `e` is equal to the value of next rl item
	  *
	  * add case
	  *
	  * split
	  */
	constexpr void set(uint64_t index, T const& e)
	{
		auto const rli = find_rl_index(index);

		if(e == base()[rli].value) return;

		auto const first_index = rl_item_first_index(rli);
		auto const last_index  = base()[rli].count - 1;
		bool const begin_flag = index == first_index; 
		bool const end_flag   = index == last_index; 

		auto const is_beginning_case = [&]
		{
			return rli > 0 && begin_flag && e == base()[rli-1].value;
		};

		auto const is_ending_case = [&]
		{
			return base().size() > 0 &&
				rli < base().size() - 1 &&
				end_flag &&
				e == base()[rli+1].value;
		};

		auto const compute_case = [&]
		{
			// beginning case
			if(is_beginning_case()){
				return set_case::BEGINNING;
			// ending case
			}else if(is_ending_case()){
				return set_case::ENDING;
			}else if(rl_item_size(rli) == 1){
				return set_case::CHANGE;
			}else if(begin_flag){
				return set_case::ADD_LEFT;
			}else if(end_flag){
				return set_case::ADD_RIGHT;
			}else{
				return set_case::SPLIT;
			}
		};

		switch(compute_case()){
		case set_case::BEGINNING:
			base()[rli-1].count += 1;
			break;
		case set_case::ENDING:
			base()[rli].count -= 1;
			break;
		case set_case::CHANGE:
			base()[rli].value = e;
			break;
		case set_case::ADD_LEFT:
			add_rl_item_at(rli, { e, first_index + 1 });
			break;
		case set_case::ADD_RIGHT:
			add_rl_item_at(rli + 1, { e, last_index + 1 });
			base()[rli].count -= 1;
			break;
		case set_case::SPLIT:
			add_rl_item_at(rli+1, base()[rli]);
			add_rl_item_at(rli+1, { e, index + 1 });
			base()[rli].count = index;
			break;
		}

		// removing a item if needed
		if(rl_item_size(rli) == 0){
			remove_rl_item_at(rli);

			// merging two items if needed
			if(base().size() >= 2 &&
					rli > 0 &&
					base()[rli].value == base()[rli - 1].value){
				remove_rl_item_at(rli - 1);
			}
		}

	}

public:

	using const_reference	= element_wrapper<run_length_container const>;
	using reference		= element_wrapper<run_length_container>;
	using value_type	= T;
	using size_type		= uint64_t;
	using difference_type	= int64_t;
	using pointer 		= rl_iterator<run_length_container>;
	using const_pointer	= rl_iterator<run_length_container const>;
	using iterator		= rl_iterator<run_length_container>;
	using const_iterator	= rl_iterator<run_length_container const>;
	using reverse_iterator	= rl_iterator<run_length_container>;
	using const_reverse_iterator= rl_iterator<run_length_container const>;
	using allocator_type	= Alloc;

	/*
	 * constructors
	 */

	constexpr run_length_container() noexcept(noexcept(underling_container{}))
	{}

	constexpr explicit run_length_container(const Alloc& alloc )
		noexcept(noexcept(underling_container(alloc)))
		: underling_container(alloc)
	{}

	constexpr explicit run_length_container(
			size_type count,
			const T& value,
			const Alloc& alloc = Alloc())
			noexcept(noexcept(underling_container(1, {value, count}, alloc)))
			: underling_container(1, {value, count}, alloc)
	{}

	constexpr explicit run_length_container(
			size_type count,
			const Alloc& alloc = Alloc())
			noexcept(noexcept(underling_container(1, {{}, count}, alloc)))
			: underling_container(1, {{}, count}, alloc)
	{}

	// element access
	constexpr auto at(uint64_t index)
	{
		return (*this)[index];
	}

	constexpr auto at(uint64_t index) const
	{
		return (*this)[index];
	}

	constexpr auto operator[](uint64_t index)
	{
		return element_wrapper(this, index);
	}

	constexpr auto operator[](uint64_t index) const
	{
		return element_wrapper(this, index);
	}

	// front
	// back
	// data


	// iterators
	constexpr auto begin()
	{
		return rl_iterator(this, 0);
	}

	constexpr auto end()
	{
		return rl_iterator(this, m_size);
	}

	constexpr auto cbegin() const
	{
		return rl_iterator(this, 0);
	}

	constexpr auto cend() const
	{
		return rl_iterator(this, m_size);
	}

	constexpr auto rbegin()
	{
		return rl_iterator(this, m_size - 1);
	}

	constexpr auto rend()
	{
		return rl_iterator(this, sentinel_value);
	}

	constexpr auto crbegin() const
	{
		return rl_iterator(this, m_size - 1);
	}

	constexpr auto crend() const
	{
		return rl_iterator(this, sentinel_value);
	}

	// capacity
	
	constexpr auto size() const
	{
		return m_size;
	}

	/**
	  * returns the number of run length items (item, count)
		*/
	constexpr underling_container& base()
	{
		return *this;
	}

	constexpr underling_container const& base() const
	{
		return *this;
	}


	// max_size

	// modifiers

	// resize
	constexpr void resize(uint64_t count)
	{
		if(count == 0) clear();

		if(m_size < count){
			base().emplace_back();
			base().back().count = count;
			m_size = count;
		}else{
			auto const rli = find_rl_index(count - 1);
			base().resize(rli + 1);
			base()[rli].count = count;
			m_size = count;
		}
	}

	// clear
	constexpr void clear()
	{
		base().clear();
		m_size = 0;
	}

	// insert
	// emplace
	// erase
	// push_back
	constexpr void push_back(T const& value)
	{
		if(base().empty()){
			base().emplace_back(value, 1UL);
		}else{
			auto& b = base().back();

			if(b.value == value)
				b.count++;
			else
				base().emplace_back(value, b.count + 1);
		}

		++m_size;
	}

	template<class F>
	constexpr auto visit_underlying_container(F f)
	{
		f(base());
		m_size = base().back().count;
	}

	// emplace_back
	// pop_back
	// swap

private:
	uint64_t m_size = uint64_t{0};
};

// operator==

} // end of namespace util
