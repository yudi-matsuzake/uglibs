#include <iterator>

#include "graphics/ball2d-render.hpp"

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

uniform vec2  u_center;
uniform float u_radius;
uniform vec4 u_color;

uniform bool u_draw_boundary;
uniform vec4 u_boundary_color;

uniform vec2 u_proj_size;
uniform vec2 u_resolution;

out vec4 fragment_color;

in vec2 p;

void main()
{

	fragment_color = vec4(0., 0., 0., 0.);

	vec2 v = p - u_center;
	float dist = sqrt(dot(v, v));

	if(dist <= u_radius)
		fragment_color = u_color;

	if(u_draw_boundary && u_boundary_color.a > 0.0){
		vec2 thick2 = u_proj_size/u_resolution;
		float thick = max(0.0, max(thick2.x, thick2.y));

		float b = abs(dist - u_radius);
		b = smoothstep(0.0, thick, b);
		fragment_color = fragment_color*b + (1.0-b)*u_boundary_color;
	}
}
)__";

static constexpr auto m_vscreen_indices = std::array{
	0u, 1u, 2u, 0u, 2u, 3u
};

/*
 * methods implementation
 * ======================
 */

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

	m_vscr_vao.bind();

	m_vscr_ebo.set_data(m_vscreen_indices.data(), m_vscreen_indices.size());
}


void ball2d_render::operator()(
	glm::mat4 const& v,
	glm::mat4 const& p,
	ball2d const& b,
	color const& c,
	rect2d const& rect)
{
	/*
	 * arbitrarily add 2 to the radius because of
	 * the ball boundary antialiasing
	 */
	auto left	= b.c.x - (b.r + 2);
	auto right	= b.c.x + (b.r + 2);
	auto top	= b.c.y + (b.r + 2);
	auto bottom	= b.c.y - (b.r + 2);
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
	m_program.set_uniform("u_center", b.c.x, b.c.y);
	m_program.set_uniform("u_radius", b.r);
	m_program.set_uniform("u_projection", p);
	m_program.set_uniform("u_view", v);
	m_program.set_uniform("u_color", c.r, c.g, c.b, c.a);

	m_program.set_uniform(
		"u_proj_size",
		glm::vec2{ rect.width, rect.height }
	);
	m_program.set_uniform("u_resolution", m_resolution);
	m_program.set_uniform("u_boundary_color", m_boundary_color);
	m_program.set_uniform("u_draw_boundary", m_draw_boundary);

	// draw
	GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void ball2d_render::set_resolution(float w, float h)
{
	m_resolution.x = w;
	m_resolution.y = h;
}

void ball2d_render::set_boundary_color(glm::vec4 const& v)
{
	m_boundary_color = v;
}

void ball2d_render::set_draw_boundary(bool b)
{
	m_draw_boundary = b;
}

} // end of namespace graphics
