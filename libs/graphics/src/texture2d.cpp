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

texture2d::texture2d(
	unsigned char const* data,
	int32_t width,
	int32_t height,
	wrap_type s_wrap,
	wrap_type t_wrap,
	filter_type min_filter,
	filter_type mag_filter)

	: texture2d()
{
	m_width = width;
	m_height = height;

	set_texture_wrap_s(s_wrap);
	set_texture_wrap_t(t_wrap);

	set_texture_minifier_filter(min_filter);
	set_texture_magnifier_filter(mag_filter);

	set_data(data);
	generate_mipmap();
}

void texture2d::set_data(unsigned char const* data)
{
	bind();
	GL(glTexImage2D(
		m_target,
		0,
		GL_RGB,
		m_width, m_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data
	));
}

void texture2d::activate(uint32_t texture_unit) const
{
	GL(glActiveTexture(GL_TEXTURE0 + texture_unit));
	bind();
}

} // end of namespace graphics
