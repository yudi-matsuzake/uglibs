#include "graphics/shader.hpp"

namespace graphics{

/*
 * check compilation and throw an exception in case
 * failed compilation
 */
static void check_shader_compilation(uint32_t id)
{
	int32_t success = 0;
	GL(glGetShaderiv(id, GL_COMPILE_STATUS, &success));

	if(!success){
		int32_t infolog_length = 0;
		GL(glGetShaderiv(
			id,
			GL_INFO_LOG_LENGTH,
			&infolog_length
		));

		std::vector<char> v(static_cast<unsigned>(infolog_length));

		GL(glGetShaderInfoLog(
			id,
			infolog_length,
			NULL,
			v.data()
		));

		throw std::runtime_error(v.data());
	}
}

static unsigned int create_shader(unsigned int shader_type)
{
	unsigned int id;
	GL(id = glCreateShader(shader_type));
	return id;
}

/*
 * constructors and methods
 * ========================
 */

shader::shader(unsigned int shader_type)
	: m_id(create_shader(shader_type))
{}

vertex_shader::vertex_shader()
	: shader(GL_VERTEX_SHADER)
{}

fragment_shader::fragment_shader()
	: shader(GL_FRAGMENT_SHADER)
{}

shader::~shader()
{
	GL(glDeleteShader(m_id));
}

unsigned int shader::id() const
{
	return m_id;
}

void shader::compile() const
{
	GL(glCompileShader(m_id));
	check_shader_compilation(m_id);
}

}
