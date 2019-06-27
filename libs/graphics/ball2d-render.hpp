#pragma once

#include <array>
#include <string_view>

#include "graphics/misc.hpp"
#include "graphics/shader.hpp"
#include "graphics/program.hpp"

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
		color const& c
	);

protected:

	vertex_shader m_vertex_shader;
	fragment_shader m_fragment_shader;

	program m_program;

	uint32_t m_vscr_vao;
	uint32_t m_vscr_vbo;
	uint32_t m_vscr_ebo;

	/*
	 *
	 * vertex and fragment shaders
	 * ---------------------------
	 */
	static const std::string_view m_vertex_shaders;
	static const std::string_view m_fragment_shaders;

	static constexpr auto m_vscreen_indices = std::array{
		0u, 1u, 2u, 0u, 2u, 3u
	};

};

}
