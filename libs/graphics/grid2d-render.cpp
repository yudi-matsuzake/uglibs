#include <iterator>

#include "graphics/grid2d-render.hpp"

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
	/* gl_Position = u_projection*vec4(vscr, 1.f); */
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

	fragment_color = vec4(1., 0., 0., 1.);

	float dist = sqrt(dot(p, p));

	if(dist <= 100.0)
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

grid2d_render::grid2d_render()
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

void grid2d_render::operator()(
		glm::mat4 const& p,
		glm::mat4 const& v,
		rect2d const& rect,
		color const& c)
{

	glm::vec4 pos{ rect.position, 0.f, 1.f };
	pos = glm::inverse(v)*pos;

	auto left	= pos.x;
	auto right	= pos.x + rect.width;
	auto top	= pos.y + rect.height;
	auto bottom	= pos.y;

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
	m_program.set_uniform("u_color", c.r, c.g, c.b, c.a);

	// draw
	GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

} // end of namespace graphics
