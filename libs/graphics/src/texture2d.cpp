#include "ug/graphics/texture2d.hpp"

#include <utility>

namespace ug::graphics{

texture2d::texture2d()
	: basic_texture(GL_TEXTURE_2D)
{}

texture2d::texture2d(texture2d&& other)
	: basic_texture(std::forward<texture2d>(other))
{
	if(this != &other){
		m_width = std::exchange(other.m_width, -1);
		m_height = std::exchange(other.m_height, -1);
	}
}

texture2d& texture2d::operator=(texture2d&& other)
{
	if(this != &other){
		basic_texture& base = *this;
		base = std::forward<texture2d>(other);
		m_width = std::exchange(other.m_width, -1);
		m_height = std::exchange(other.m_height, -1);
	}
	return *this;
}

void texture2d::activate(uint32_t texture_unit) const
{
	GL(glActiveTexture(GL_TEXTURE0 + texture_unit));
	bind();
}

} // end of namespace graphics
