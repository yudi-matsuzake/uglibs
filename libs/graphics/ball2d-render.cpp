#include <iterator>

#include "graphics/ball2d-render.hpp"

namespace graphics{

constexpr std::string_view ball2d_render::m_vertex_shaders =
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

constexpr std::string_view ball2d_render::m_fragment_shaders =
R"__(
#version 330 core

uniform vec2  u_center;
uniform float u_radius;
uniform vec4 u_color;

out vec4 fragment_color;

in vec2 p;

void main()
{

	fragment_color = vec4(0., 0., 0., 0.);

	vec2 v = p - u_center;
	float dist = sqrt(dot(v, v));

	if(dist <= u_radius){
		fragment_color = u_color;
	}
}
)__";

ball2d_render::ball2d_render()
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
	m_program.use();
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
	m_program.set_uniform("u_center", b.c.x, b.c.y);
	m_program.set_uniform("u_radius", b.r);
	m_program.set_uniform("u_projection", p);
	m_program.set_uniform("u_view", v);
	m_program.set_uniform("u_color", c.r, c.g, c.b, c.a);

	// draw
	GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

} // end of namespace graphics
