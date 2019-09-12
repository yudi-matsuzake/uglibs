#pragma once

#include <array>
#include <string_view>

#include "ug/graphics/misc.hpp"
#include "ug/graphics/shader.hpp"
#include "ug/graphics/program.hpp"
#include "ug/graphics/vao.hpp"
#include "ug/graphics/buffers.hpp"

namespace ug::graphics{

struct square {
	glm::vec2 c{ 0.f, 0.f };
	float side = 0.f;
};

class square_render {
public:

	square_render();

	/**
	  * renders a square `s` with the view matrix `v` and
	  * the projection matrix `p`
	  */
	void operator()(
		glm::mat4 const& v,
		glm::mat4 const& p,
		square const& s
	);

	void set_current_color(color const& color);

protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	vao m_vscr_vao;
	vbo m_vscr_vbo;
	ebo m_vscr_ebo;

	color current_color = { 1.f, 1.f, 1.f, 1.f };
};

}
