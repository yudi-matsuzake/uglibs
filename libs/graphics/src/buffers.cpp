#include "ug/graphics/buffers.hpp"

namespace graphics{

static uint32_t gen_buffer()
{
	uint32_t id;
	GL(glGenBuffers(1, &id));
	return id;
}

buffer::buffer(uint32_t a_target)
	: m_id(gen_buffer()), m_target(a_target)
{}


buffer::~buffer()
{
	GL(glDeleteBuffers(1, &m_id));
}

void buffer::bind() const
{
	GL(glBindBuffer(m_target, m_id));
}

} // end of namespace graphics
