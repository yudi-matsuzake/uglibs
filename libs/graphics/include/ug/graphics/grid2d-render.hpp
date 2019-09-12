#pragma once

#include <array>
#include <string_view>

#include "ug/graphics/misc.hpp"
#include "ug/graphics/shader.hpp"
#include "ug/graphics/program.hpp"
#include "ug/graphics/vao.hpp"
#include "ug/graphics/buffers.hpp"
#include "ug/graphics/render.hpp"

namespace graphics{

class grid2d_render : public render{
public:

	grid2d_render(app* app_ptr);

	/**
	  * renders a grid in the rect `rect`
	  */
	void operator()();

	void set_grid_color(color const& c);

protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	vao m_vscr_vao;
	vbo m_vscr_vbo;
	ebo m_vscr_ebo;

	color m_grid_color = { 1.0f, 1.0f, 1.0f, 1.0f };
};

}
