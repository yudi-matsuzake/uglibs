#include "ug/graphics/texture1d.hpp"

#include <utility>

namespace ug::graphics{

texture1d::texture1d()
	: basic_texture(GL_TEXTURE_1D)
{}

texture1d::texture1d(texture1d&& other)
	: basic_texture(std::forward<texture1d>(other))
{
	if(this != &other){
		m_width = std::exchange(other.m_width, -1);
	}
}

texture1d& texture1d::operator=(texture1d&& other)
{
	if(this != &other){
		basic_texture& base = *this;
		base = std::forward<texture1d>(other);
		m_width = std::exchange(other.m_width, -1);
	}
	return *this;
}

void texture1d::activate(uint32_t texture_unit) const
{
	GL(glActiveTexture(GL_TEXTURE0 + texture_unit));
	bind();
}

} // end of namespace graphics
