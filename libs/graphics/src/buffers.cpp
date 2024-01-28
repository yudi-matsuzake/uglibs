#include <utility>

#include "ug/graphics/buffers.hpp"

namespace ug::graphics{

static uint32_t gen_buffer()
{
	uint32_t id;
	GL(glGenBuffers(1, &id));
	return id;
}

buffer::buffer(uint32_t a_target)
	: m_id(gen_buffer()), m_target(a_target)
{}

buffer& buffer::operator=(buffer&& other)
{
	if(this != &other){
		this->m_id = std::exchange(other.m_id, 0u);
		this->m_target = std::exchange(other.m_target, 0u);
	}
	return *this;
}

buffer::buffer(buffer&& other)
{
	(*this) = std::forward<buffer>(other);
}

buffer::~buffer()
{
	GL(glDeleteBuffers(1, &m_id));
}

void buffer::bind() const
{
	GL(glBindBuffer(m_target, m_id));
}

uint32_t buffer::id() const
{
	return m_id;
}

} // end of namespace ug::graphics
