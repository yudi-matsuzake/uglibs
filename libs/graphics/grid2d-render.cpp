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
	p = vscr.xy;
}
)__";

static constexpr std::string_view m_fragment_shaders =
R"__(
#version 330 core

uniform vec4 u_color;
uniform vec2 u_proj_size;
uniform vec2 u_resolution;

out vec4 fragment_color;

in vec2 p;

vec2 smoothdist(vec2 v, float thick)
{
	float tmp = 1.0f - thick;

	vec2 d = abs(v - 0.5f)*2.0f;
	return step(tmp, d);
}

bool in_range(float a, float b, float x)
{
	return x >= a && x <= b;
}

void main()
{

	vec2 thick2 = u_proj_size/u_resolution;
	float thick = max(thick2.x, thick2.y);
	float a_thick = 1.0f - clamp(thick*5.0f, 0.0f, 1.0f);

	vec4 axis_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	vec2 m = mod(p, 1.0f);
	vec2 s = smoothdist(m, thick);

	float a = min(max(s.x, s.y), a_thick);

	vec4 color = u_color;

	float ht = thick/2.0f; // half thick
	if(in_range(-ht, ht, p.x) || in_range(-ht, ht, p.y))
		color = axis_color;

	fragment_color = vec4(color.rgb, min(a, color.a));
}
)__";

static constexpr auto m_vscreen_indices = std::array{
	0u, 1u, 2u, 0u, 2u, 3u
};

/*
 * methods implementation
 * ======================
 */

grid2d_render::grid2d_render(app* app_ptr)
	: render(app_ptr)
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

void grid2d_render::operator()()
{

	auto pv = get_app()->get_projected_viewport();
	auto left	= pv.position.x;
	auto right	= pv.position.x + pv.width;
	auto top	= pv.position.y;
	auto bottom	= pv.position.y - pv.height;

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
		0,
		0
	));
	GL(glEnableVertexAttribArray(0));

	auto [ w, h ] = get_app()->get_framebuffer_size();

	// set uniforms
	m_program.set_uniform("u_projection", get_app()->projection_matrix());
	m_program.set_uniform("u_view", get_app()->view_matrix());
	m_program.set_uniform("u_color", m_grid_color);
	m_program.set_uniform("u_proj_size", pv.width, pv.height);
	m_program.set_uniform("u_resolution", glm::vec<2, float>{ w, h });

	// draw
	GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void grid2d_render::set_grid_color(color const& c)
{
	m_grid_color = c;
}

} // end of namespace graphics
