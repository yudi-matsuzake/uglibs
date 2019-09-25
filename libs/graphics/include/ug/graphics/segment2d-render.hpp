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

struct segment2d {
	glm::vec2 from{ 0.f, 0.f };
	glm::vec2 to{ 0.f, 0.f };
};

class segment2d_render : public render{
public:

	segment2d_render(app* app_ptr);

	/**
	  * renders a 2d segment `b` with the view matrix `v` and
	  * the projection matrix `p`
	  */
	void operator()(segment2d const& b);

	void set_color(graphics::color const& c);
protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	vao m_vscr_vao;
	vbo m_vscr_vbo;

	color m_color = { 1.0, 1.0, 1.0, 1.0 };
};

}
