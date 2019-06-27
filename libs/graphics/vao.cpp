#include "graphics/vao.hpp"

#include "graphics/misc.hpp"

namespace graphics{

static uint32_t gen_vao()
{
	uint32_t id;
	GL(glGenVertexArrays(1, &id));
	return id;
}

vao::vao()
	: m_id(gen_vao())
{}

vao::~vao()
{
	GL(glDeleteVertexArrays(1, &m_id));
}

void vao::bind() const
{
	GL(glBindVertexArray(m_id));
}

} // end of namespace graphics
