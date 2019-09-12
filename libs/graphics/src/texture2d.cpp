#include "graphics/texture2d.hpp"

namespace graphics{

static uint32_t generate_texture2d()
{
	uint32_t texture_id;
	GL(glGenTextures(1, &texture_id));
	return texture_id;
}

texture2d::texture2d()
	: m_id(generate_texture2d())
{}

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
	set_texture_wrap_s(s_wrap);
	set_texture_wrap_t(t_wrap);

	set_texture_minifier_filter(min_filter);
	set_texture_magnifier_filter(mag_filter);

	set_data(data, width, height);
	generate_mipmap();
}

texture2d::~texture2d()
{
	GL(glDeleteTextures(1, &m_id));
}

uint32_t texture2d::id() const
{
	return m_id;
}

void texture2d::bind() const
{
	GL(glBindTexture(GL_TEXTURE_2D, id()));
}

void texture2d::set_texture_wrap_s(wrap_type wrap)
{
	bind();
	GL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		static_cast<int>(wrap)
	));
}

void texture2d::set_texture_wrap_t(wrap_type wrap)
{
	bind();
	GL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		static_cast<int>(wrap)
	));
}

void texture2d::set_texture_magnifier_filter(filter_type filter)
{
	bind();
	GL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		static_cast<int>(filter)
	));
}

void texture2d::set_texture_minifier_filter(filter_type filter)
{
	bind();
	GL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		static_cast<int>(filter)
	));
}

void texture2d::set_border_color(color const& border_color) const
{
	bind();
	GL(glTexParameterfv(
		GL_TEXTURE_2D,
		GL_TEXTURE_BORDER_COLOR,
		glm::value_ptr(border_color)
	));
}

void texture2d::generate_mipmap()
{
	bind();
	GL(glGenerateMipmap(GL_TEXTURE_2D));
}

void texture2d::set_data(unsigned char const* data, int32_t width, int32_t height)
{
	bind();
	GL(glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		width, height,
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
