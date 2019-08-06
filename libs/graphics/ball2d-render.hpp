#pragma once

#include <array>
#include <string_view>

#include "graphics/misc.hpp"
#include "graphics/shader.hpp"
#include "graphics/program.hpp"
#include "graphics/vao.hpp"
#include "graphics/buffers.hpp"

namespace graphics{

struct ball2d {
	glm::vec2 c{ 0.f, 0.f };
	float r = 0.f;
};

class ball2d_render {
public:

	ball2d_render();

	/**
	  * renders a 2d ball `b` with the view matrix `v` and
	  * the projection matrix `p`
	  */
	void operator()(
		glm::mat4 const& v,
		glm::mat4 const& p,
		ball2d const& b,
		color const& c,
		rect2d const& rect
	);

	void set_resolution(float w, float h);
	void set_boundary_color(graphics::color const& c);
	void set_draw_boundary(bool b);
protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	vao m_vscr_vao;
	vbo m_vscr_vbo;
	ebo m_vscr_ebo;

	glm::vec2 m_resolution;

	graphics::color m_boundary_color = graphics::color{
		1.0, 1.0, 1.0, 1.0
	};

	bool m_draw_boundary = true;
};

}
