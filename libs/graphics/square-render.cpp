#include <iterator>

#include "graphics/square-render.hpp"

namespace graphics{

/*
 * constants
 * =========
 */

static constexpr std::string_view m_vertex_shaders =
R"__(
#version 330 core

uniform mat4 u_view;
uniform mat4 u_projection;

layout (location = 0) in vec3 vscr;

out vec2 p;

void main()
{
	gl_Position = u_projection*u_view*vec4(vscr, 1.f);
	p = vscr.xy;
}
)__";

static constexpr std::string_view m_fragment_shaders =
R"__(
#version 330 core

uniform vec4 u_color;
out vec4 fragment_color;

in vec2 p;

void main()
{
	fragment_color = u_color;
}
)__";

static constexpr auto m_vscreen_indices = std::array{
	0u, 1u, 2u, 0u, 2u, 3u
};

/*
 * methods implementation
 * ======================
 */

square_render::square_render()
{

	m_vertex_shader.set_source(
		std::cbegin(m_vertex_shaders),
		std::cend(m_vertex_shaders)
	);

	m_fragment_shader.set_source(
		std::cbegin(m_fragment_shaders),
		std::cend(m_fragment_shaders)
	);

	// create and link program
	m_vertex_shader.compile();
	m_fragment_shader.compile();

	m_program.attach_shader(m_vertex_shader);
	m_program.attach_shader(m_fragment_shader);
	m_program.link();

	m_vscr_vao.bind();

	m_vscr_ebo.set_data(m_vscreen_indices.data(), m_vscreen_indices.size());
}


void square_render::operator()(
		glm::mat4 const& v,
		glm::mat4 const& p,
		square const& s)
{

	auto hs		= s.side/2.f;
	auto left	= s.c.x - hs;
	auto right	= s.c.x + hs;
	auto top	= s.c.y + hs;
	auto bottom	= s.c.y - hs;
	auto vscreen = std::array{
		right, bottom, .0f,
		right, top, .0f,
		left,  top, .0f,
		left, bottom, .0f
	};

	// bind buffers and program
	m_program.use();
	m_vscr_vao.bind();
	m_vscr_ebo.bind();
	m_vscr_vbo.set_data(vscreen.data(), vscreen.size());

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
	m_program.set_uniform("u_projection", p);
	m_program.set_uniform("u_view", v);
	m_program.set_uniform("u_color", current_color);

	// draw
	GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void square_render::set_current_color(color const& a_color)
{
	current_color = a_color;
}

} // end of namespace graphics
