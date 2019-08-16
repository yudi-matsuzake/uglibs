#pragma once

#include <array>
#include <string_view>

#include "graphics/misc.hpp"
#include "graphics/shader.hpp"
#include "graphics/program.hpp"
#include "graphics/vao.hpp"
#include "graphics/buffers.hpp"
#include "graphics/render.hpp"

namespace graphics{

class grid2d_render : public render{
public:

	grid2d_render(app* app_ptr);

	/**
	  * renders a grid in the rect `rect`
	  */
	void operator()();

	void set_grid_color(color const& c);

	void set_resolution(float w, float h);

protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	vao m_vscr_vao;
	vbo m_vscr_vbo;
	ebo m_vscr_ebo;

	color m_grid_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec2 m_resolution;
};

}
