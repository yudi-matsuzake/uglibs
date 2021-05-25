#pragma once

#include <array>
#include <string_view>

#include "ug/graphics/misc.hpp"
#include "ug/graphics/shader.hpp"
#include "ug/graphics/program.hpp"
#include "ug/graphics/vao.hpp"
#include "ug/graphics/buffers.hpp"
#include "ug/graphics/render.hpp"

namespace ug::graphics{

struct ball2d {
	glm::vec2 c{ 0.f, 0.f };
	float r = 0.f;
};

class ball2d_render : public render{
public:

	ball2d_render(app* app_ptr);

	/**
	  * renders a 2d ball `b` with the view matrix `v` and
	  * the projection matrix `p`
	  */
	void operator()(ball2d const& b);

	void set_boundary_color(graphics::color4 const& c);
	void set_fill_color(graphics::color4 const& c);
	void set_draw_boundary(bool b);
	void set_draw_fill(bool b);
	void set_thick(float t);
protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	vao m_vscr_vao;
	vbo m_vscr_vbo;
	ebo m_vscr_ebo;

	color4 m_fill_color = { 1.0, 1.0, 1.0, 1.0 };

	graphics::color4 m_boundary_color = graphics::color4{
		1.0, 1.0, 1.0, 1.0
	};

	bool m_draw_boundary	= true;
	bool m_draw_fill	= true;

	float m_thick = 1.0f;
};

}
