#include "ug/graphics/buffer-texture.hpp"

#include <utility>

namespace ug::graphics{

buffer_texture::buffer_texture()
	: basic_texture(GL_TEXTURE_BUFFER)
{}

buffer_texture::buffer_texture(buffer_texture&& other)
	: basic_texture(std::forward<buffer_texture>(other))
{}

buffer_texture& buffer_texture::operator=(buffer_texture&& other)
{
	if(this != &other){
		basic_texture& base = *this;
		base = std::forward<buffer_texture>(other);
		m_buffer = std::move(other.m_buffer);
	}
	return *this;
}

void buffer_texture::activate(uint32_t texture_unit) const
{
	GL(glActiveTexture(GL_TEXTURE0 + texture_unit));
	bind();
}

void buffer_texture::bind() const
{
	basic_texture::bind();
	m_buffer.bind();
}

} // end of namespace graphics
