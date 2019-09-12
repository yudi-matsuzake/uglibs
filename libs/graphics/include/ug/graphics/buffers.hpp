#pragma once
#include <cstdint>
#include <cstddef>

#include "ug/graphics/misc.hpp"

namespace graphics{

class buffer {
public:

	buffer(std::uint32_t);

	virtual ~buffer();
	void bind() const;

	template<class T>
	void set_data(
		T* ptr,
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

} // end of namespace graphics
