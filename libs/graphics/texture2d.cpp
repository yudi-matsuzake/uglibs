#include "graphics/texture2d.hpp"
#include "graphics/misc.hpp"

namespace graphics{

static uint32_t generate_texture2d()
{
	uint32_t texture_id;
	GL(glGenTextures(1, &texture_id));
	return texture_id;
}

texture2d::texture2d(int32_t width, int32_t height, void* data)
	: m_id(generate_texture2d())
{
	bind();

	GL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_BORDER
	));

	GL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_BORDER
	));

	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GL(glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		width, height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data
	));

	GL(glGenerateMipmap(GL_TEXTURE_2D));
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

} // end of namespace graphics
