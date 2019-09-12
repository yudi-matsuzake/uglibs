#include "ug/graphics/vao.hpp"

#include "ug/graphics/misc.hpp"

namespace ug::graphics{

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

uint32_t vao::id() const
{
	return m_id;
}

void vao::set_attribute_layout(const vao::attribute_layout& attributes) const
{
	bind();
	for(auto&& atribute : attributes){
		GL(glVertexAttribPointer(
			atribute.index,
			static_cast<int32_t>(atribute.n_member),
			atribute.type,
			atribute.normalized?GL_TRUE:GL_FALSE,
			static_cast<int32_t>(attributes.stride()),
			reinterpret_cast<void*>(atribute.offset)
		));
		GL(glEnableVertexAttribArray(atribute.index));
	}
}

vao::attr<float>::attr(uint32_t a_n_member, bool a_normalized)
	: vao::attribute(
		0,
		sizeof(float),
		a_n_member,
		GL_FLOAT,
		0,
		a_normalized)
{}

vao::attr<double>::attr(std::uint32_t a_n_member, bool a_normalized)
	: vao::attribute(
		0,
		sizeof(double),
		a_n_member,
		GL_DOUBLE,
		0,
		a_normalized)
{}

} // end of namespace graphics
