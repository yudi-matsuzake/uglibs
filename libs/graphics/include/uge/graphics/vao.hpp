#pragma once

#include <cstdint>
#include <vector>
#include <initializer_list>

namespace graphics{

class vao{
protected:
	struct attribute{
		attribute(
			uint32_t a_index,
			uint32_t a_type_size,
			uint32_t a_n_member,
			uint32_t a_type,
			uint32_t a_offset,
			bool a_normalized)
			: index(a_index),
			  type_size(a_type_size),
			  n_member(a_n_member),
			  type(a_type),
			  offset(a_offset),
			  normalized(a_normalized)
		{}

		virtual ~attribute() = default;
		uint32_t index;
		uint32_t type_size;
		uint32_t n_member;
		uint32_t type;
		uint64_t offset;
		bool normalized;
	};

public:

	template<class T>
	struct attr;

	class attribute_layout : public std::vector<attribute> {
	protected:
		uint32_t m_stride = 0;
	public:

		using iterator_type = std::vector<attribute>::iterator;
		using const_iterator_type = std::vector<attribute>::const_iterator;

		attribute_layout(const std::initializer_list<attribute>& l)
		{
			std::uint32_t index = 0;
			for(auto i : l){
				i.index = index++;
				i.offset = m_stride;
				m_stride += i.type_size*i.n_member;
				push_back(i);
			}
		}

		virtual ~attribute_layout() = default;

		std::uint32_t stride() const
		{
			return m_stride;
		}

		iterator_type begin()
		{
			return std::vector<attribute>::begin();
		}

		iterator_type end()
		{
			return std::vector<attribute>::end();
		}

		const_iterator_type begin() const
		{
			return std::vector<attribute>::cbegin();
		}

		const_iterator_type end() const
		{
			return std::vector<attribute>::cend();
		}

	};

	vao();
	virtual ~vao();

	void bind() const;

	uint32_t id() const;

	void set_attribute_layout(const attribute_layout& attributes) const;

protected:
	uint32_t m_id;
};

template<>
struct vao::attr<float> : public vao::attribute{
	attr(uint32_t n_member, bool normalized = false);
};

template<>
struct vao::attr<double> : public vao::attribute {
	attr(std::uint32_t n_member, bool normalized = false);
};



} // end of namespace graphics
