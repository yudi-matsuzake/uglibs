#include <iterator>

#include "ug/graphics/segment2d-render.hpp"

namespace ug::graphics{

/*
 * constants
 * =========
 */

static constexpr auto m_vertex_shaders = std::array{
R"__(
#version 330 core

uniform mat4 u_view;
uniform mat4 u_projection;

layout (location = 0) in vec2 vscr;

void main()
{
	gl_Position = u_projection*u_view*vec4(vscr, 0.0f, 1.f);
}
)__"};

static constexpr auto m_fragment_shaders = std::array{
R"__(
#version 330 core

uniform vec4 u_color;

out vec4 fragment_color;

void main()
{

	fragment_color = u_color;
}
)__"};

/*
 * methods implementation
 * ======================
 */

segment2d_render::segment2d_render(app* app_ptr)
	: render(app_ptr)
{

	m_vertex_shader.set_source(m_vertex_shaders);
	m_fragment_shader.set_source(m_fragment_shaders);

	// create and link program
	m_vertex_shader.compile();
	m_fragment_shader.compile();

	m_program.attach_shader(m_vertex_shader);
	m_program.attach_shader(m_fragment_shader);
	m_program.link();
}


void segment2d_render::operator()(segment2d const& s)
{
	/*
	 * arbitrarily add 2 to the radius because of
	 * the ball boundary antialiasing
	 */
	auto line = std::array{s.from.x, s.from.y, s.to.x, s.to.y};

	// bind buffers and program
	m_program.use();
	m_vscr_vao.bind();
	m_vscr_vbo.set_data(line.data(), line.size());

	m_vscr_vao.set_attribute_layout({ graphics::vao::attr<float>(2) });

	// set uniforms
	m_program.set_uniform("u_projection", get_app()->compute_projection_matrix());
	m_program.set_uniform("u_view", get_app()->get_view_matrix());
	m_program.set_uniform("u_color", m_color);

	/* auto const& pv = get_app()->get_projected_viewport(); */
	/* auto [ w, h ] = get_app()->get_framebuffer_size(); */
	/* m_program.set_uniform("u_proj_size", pv.width, pv.height); */
	/* m_program.set_uniform("u_resolution", glm::vec<2, float>{ w, h }); */

	// draw
	GL(glDrawArrays(GL_LINES, 0, 2));
}

void segment2d_render::set_color(graphics::color4 const& c)
{
	m_color = c;
}

} // end of namespace graphics
