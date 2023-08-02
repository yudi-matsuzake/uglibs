#pragma once

#include <span>

#include "ug/graphics/mesh3d.hpp"
#include "ug/graphics/light3d.hpp"
#include "ug/graphics/app.hpp"
#include "ug/graphics/render.hpp"
#include "ug/graphics/program.hpp"

namespace ug::graphics{

class mesh3d_render : public render{
public:

	mesh3d_render(app*);
	void operator()(
		camera const& cam,
		std::span<light3d> lights,
		mesh3d const& mesh);

	float ambient_light_strength = 0.2f;
	bool specular_lighting = true;
	float specular_strength = 0.5f;
	bool diffuse_lighting = true;

	struct{
		bool draw = false;
		bool use_attr_color = false;
		float line_width = 1.f;
		color4 color = { .8, 0., 0., 1. };
	}lines;

	struct{
		bool draw = true;
		bool use_attr_color = true;
		color4 color = { .0, .7, 1., 1. };
	}triangles;


private:
	void common_program_setup(
		camera const& cam,
		std::span<light3d> lights,
		mesh3d const& mesh);

	ug::graphics::program m_program;
};

} // end of namespace ug::graphics
