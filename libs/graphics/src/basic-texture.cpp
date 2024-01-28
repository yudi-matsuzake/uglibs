#include "ug/graphics/basic-texture.hpp"

#include <utility>

namespace ug::graphics{

static uint32_t generate_basic_texture()
{
	uint32_t texture_id;
	GL(glGenTextures(1, &texture_id));
	return texture_id;
}

basic_texture::basic_texture(uint32_t target)
	: m_id(generate_basic_texture()), m_target(target)
{}

basic_texture::basic_texture(basic_texture&& other)
{
	(*this) = std::move(other);
}

basic_texture& basic_texture::operator=(basic_texture&& other)
{
	if(&other != this){
		this->m_id = std::exchange(other.m_id, std::nullopt);
		this->m_target = std::exchange(other.m_target, 0);
	}
	return *this;
}

basic_texture::~basic_texture()
{
	if(m_id.has_value())
		GL(glDeleteTextures(1, &m_id.value()));
}

uint32_t basic_texture::id() const
{
	return m_id.value();
}

void basic_texture::bind() const
{
	GL(glBindTexture(m_target, id()));
}

void basic_texture::set_texture_wrap_s(wrap_type wrap)
{
	bind();
	GL(glTexParameteri(
		m_target,
		GL_TEXTURE_WRAP_S,
		static_cast<int>(wrap)
	));
}

void basic_texture::set_texture_wrap_t(wrap_type wrap)
{
	bind();
	GL(glTexParameteri(
		m_target,
		GL_TEXTURE_WRAP_T,
		static_cast<int>(wrap)
	));
}

void basic_texture::set_texture_magnifier_filter(filter_type filter)
{
	bind();
	GL(glTexParameteri(
		m_target,
		GL_TEXTURE_MAG_FILTER,
		static_cast<int>(filter)
	));
}

void basic_texture::set_texture_minifier_filter(filter_type filter)
{
	bind();
	GL(glTexParameteri(
		m_target,
		GL_TEXTURE_MIN_FILTER,
		static_cast<int>(filter)
	));
}

void basic_texture::set_border_color(color4 const& border_color) const
{
	bind();
	GL(glTexParameterfv(
		m_target,
		GL_TEXTURE_BORDER_COLOR,
		glm::value_ptr(border_color)
	));
}

void basic_texture::generate_mipmap()
{
	bind();
	GL(glGenerateMipmap(m_target));
}

void basic_texture::activate(uint32_t texture_unit) const
{
	bind();
	GL(glActiveTexture(GL_TEXTURE0 + texture_unit));
}

} // end of namespace ug::graphics
