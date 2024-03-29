#include <utility>
#include <vector>

/* #include "ug/graphics/misc.hpp" */
#include "ug/graphics/program.hpp"

namespace ug::graphics{

/*
 * check linkage status and throw an exception in case
 * failed linkage
 */
static void check_program_linkage(uint32_t id)
{
	int32_t success = 0;
	GL(glGetProgramiv(id, GL_LINK_STATUS, &success));

	if(!success){
		int32_t infolog_length = 0;
		GL(glGetProgramiv(
			id,
			GL_INFO_LOG_LENGTH,
			&infolog_length
		));

		std::vector<char> v(static_cast<unsigned>(infolog_length));

		GL(glGetProgramInfoLog(
			id,
			infolog_length,
			NULL,
			v.data()
		));

		throw std::runtime_error(v.data());
	}
}


program::program()
{
	GL(m_id = glCreateProgram());
}

program::program(program&& other)
{
	if(this != &other) {
		m_id = std::exchange(other.m_id, 0);
	}
}

program::~program()
{
	if(m_id != 0)
		GL(glDeleteProgram(m_id));
}

void program::attach_shader(shader const& s) const
{
	GL(glAttachShader(m_id, s.id()));
}

void program::link() const
{
	GL(glLinkProgram(m_id));
	check_program_linkage(m_id);
}

uint32_t program::id() const
{
	return m_id;
}

void program::use() const
{
	GL(glUseProgram(m_id));
}

void program::set_uniform(char const* name, float f1) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform1f(id, f1));
}

void program::set_uniform(char const* name, float f1, float f2) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform2f(id, f1, f2));
}

void program::set_uniform(char const* name, float f1, float f2, float f3) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform3f(id, f1, f2, f3));
}

void program::set_uniform(
	char const* name,
	float f1,
	float f2,
	float f3,
	float f4) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform4f(id, f1, f2, f3, f4));
}

void program::set_uniform(char const* name, glm::mat4 const& m) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(m)));
}

void program::set_uniform(char const* name, glm::vec2 const& v) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform2fv(id, 1, glm::value_ptr(v)));
}

void program::set_uniform(char const* name, glm::vec3 const& v) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform3fv(id, 1, glm::value_ptr(v)));
}

void program::set_uniform(char const* name, glm::vec4 const& v) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform4fv(id, 1, glm::value_ptr(v)));
}

void program::set_uniform(char const* name, bool b) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform1i(id, b));
}

void program::set_uniform(char const* name, int32_t n) const
{
	int32_t id;
	GL(id = glGetUniformLocation(m_id, name));
	GL(glUniform1i(id, n));
}



} // end of namespace ug::graphics
