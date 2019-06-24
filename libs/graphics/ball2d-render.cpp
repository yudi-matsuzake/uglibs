#include "graphics/ball2d-render.hpp"

namespace graphics{

ball2d_render::ball2d_render()
{
	// create and compile shaders
	GL(m_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	GL(m_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));

	GL(glShaderSource(
		m_vertex_shader_id,
		m_vertex_shaders.size(),
		m_vertex_shaders.data(),
		nullptr
	));
	GL(glShaderSource(
		m_fragment_shader_id,
		m_fragment_shaders.size(),
		m_fragment_shaders.data(),
		nullptr
	));

	// create and link program
	GL(glCompileShader(m_vertex_shader_id));
	check_shader_compilation(m_vertex_shader_id);

	GL(glCompileShader(m_fragment_shader_id));
	check_shader_compilation(m_fragment_shader_id);

	GL(m_program_id = glCreateProgram());
	GL(glAttachShader(m_program_id, m_vertex_shader_id));
	GL(glAttachShader(m_program_id, m_fragment_shader_id));
	GL(glLinkProgram(m_program_id));

	// create buffers
	GL(glGenVertexArrays(1, &m_vscr_vao));
	GL(glGenBuffers(1, &m_vscr_vbo));
	GL(glGenBuffers(1, &m_vscr_ebo));

	GL(glBindVertexArray(m_vscr_vao));

	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vscr_ebo));
	GL(glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof m_vscreen_indices,
		m_vscreen_indices.data(),
		GL_STATIC_DRAW
	));

}


void ball2d_render::operator()(
		glm::mat4 const& v,
		glm::mat4 const& p,
		ball2d const& b,
		color const& c)
{

	auto left	= b.c.x - b.r;
	auto right	= b.c.x + b.r;
	auto top	= b.c.y + b.r;
	auto bottom	= b.c.y - b.r;
	auto vscreen = std::array{
		right, bottom, .0f,
		right, top, .0f,
		left,  top, .0f,
		left, bottom, .0f
	};

	// bind buffers and program
	GL(glUseProgram(m_program_id));
	GL(glBindVertexArray(m_vscr_vao));
	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vscr_ebo));
	GL(glBindBuffer(GL_ARRAY_BUFFER, m_vscr_vbo));

	// set the vertex buffer data
	GL(glBufferData(
		GL_ARRAY_BUFFER,
		sizeof vscreen,
		vscreen.data(),
		GL_DYNAMIC_DRAW
	));

	GL(glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				vscreen.size(),
				0
				));
	GL(glEnableVertexAttribArray(0));

	// set uniforms
	int32_t id;
	GL(id = glGetUniformLocation(m_program_id, "u_center"));
	GL(glUniform2f(id, b.c.x, b.c.y));

	GL(id = glGetUniformLocation(m_program_id, "u_radius"));
	GL(glUniform1f(id, b.r));

	GL(id = glGetUniformLocation(m_program_id, "u_projection"));
	GL(glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(p)));

	GL(id = glGetUniformLocation(m_program_id, "u_view"));
	GL(glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(v)));

	GL(id = glGetUniformLocation(m_program_id, "u_color"));
	GL(glUniform4f(
		id, c.r, c.g, c.b, c.a
	));

	// draw
	GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

} // end of namespace graphics
