#pragma once
#include <cstdint>
#include <cstddef>
#include <ranges>

#include "ug/graphics/opengl-error.hpp"

namespace ug::graphics{

namespace rgs = std::ranges;

class buffer {
public:

	buffer(std::uint32_t);

	virtual ~buffer();
	virtual void bind() const;

	buffer(buffer const&) = delete;
	buffer& operator=(buffer const&) = delete;

	buffer(buffer&&);
	buffer& operator=(buffer&&);

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

	template<rgs::range R>
	void set_data(R const& r, uint32_t usage = GL_STATIC_DRAW)
	{
		set_data(rgs::cdata(r), rgs::size(r), usage);
	}

	uint32_t id() const;

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

class tdb : public buffer {
public:
	tdb() : buffer(GL_TEXTURE_BUFFER)
	{} 
};

} // end of namespace ug::graphics
