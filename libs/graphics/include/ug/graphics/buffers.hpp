#pragma once
#include <cstdint>
#include <cstddef>

#include "ug/graphics/opengl-error.hpp"

namespace ug::graphics{

class buffer {
public:

	buffer(std::uint32_t);

	virtual ~buffer();
	void bind() const;

	buffer(buffer const&) = delete;
	buffer(buffer&&) = default;

	/**
	  * sets the buffer data
	  * 
	  * @param ptr	the pointer to `size` elements of  `T`
	  * @param size	is the number of `T`-elements pointed by `ptr`
	  * @param usage the usage of the buffer object
	  */
	template<class T>
	void set_data(
		T const* ptr,
		size_t size,
		uint32_t usage = GL_STATIC_DRAW) const
	{
		bind();
		GL(glBufferData(
			m_target,
			static_cast<int64_t>(size*sizeof(T)),
			ptr,
			usage
		));
	}


protected:
	uint32_t m_id;
	uint32_t m_target;
};


class vbo : public buffer {
public:
	vbo() : buffer(GL_ARRAY_BUFFER)
	{} 
};

class ebo : public buffer {
public:
	ebo() : buffer(GL_ELEMENT_ARRAY_BUFFER)
	{} 
};

} // end of namespace ug::graphics
