#pragma once

#include <array>

#include "graphics/misc.hpp"

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

	uint32_t m_vertex_shader_id;
	uint32_t m_fragment_shader_id;
	uint32_t m_program_id;

	uint32_t m_vscr_vao;
	uint32_t m_vscr_vbo;
	uint32_t m_vscr_ebo;

	/*
	 *
	 * vertex and fragment shaders
	 * ---------------------------
	 */
	static constexpr auto m_vertex_shaders = std::array{R"__(
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
)__"};

	static constexpr auto m_fragment_shaders = std::array{R"__(
#version 330 core

uniform vec2  u_center;
uniform float u_radius;
uniform vec4 u_color;

out vec4 fragment_color;

in vec2 p;

void main()
{

	fragment_color = vec4(0., 0., 0., 0.);

	vec2 v = p - u_center;
	float dist = sqrt(dot(v, v));

	if(dist <= u_radius){
		fragment_color = u_color;
	}
}
)__"};

	static constexpr auto m_vscreen_indices = std::array{
		0u, 1u, 2u, 0u, 2u, 3u
	};

};

}
