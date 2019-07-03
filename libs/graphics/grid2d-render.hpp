#pragma once

#include <array>
#include <string_view>

#include "graphics/misc.hpp"
#include "graphics/shader.hpp"
#include "graphics/program.hpp"
#include "graphics/vao.hpp"
#include "graphics/buffers.hpp"

namespace graphics{

class grid2d_render {
public:

	grid2d_render();

	/**
	  * renders a grid in the rect `rect`
	  */
	void operator()(
		glm::mat4 const& p,
		glm::mat4 const& v,
		rect2d const& rect
	);

	void set_grid_color(color const& c);
	void set_grid_width(float width);

protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	vao m_vscr_vao;
	vbo m_vscr_vbo;
	ebo m_vscr_ebo;

	color m_grid_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_grid_width = 0.1f;
};

}
