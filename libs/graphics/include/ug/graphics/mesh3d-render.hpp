#pragma once

#include "ug/graphics/mesh3d.hpp"
#include "ug/graphics/app.hpp"
#include "ug/graphics/render.hpp"
#include "ug/graphics/scene.hpp"
#include "ug/graphics/program.hpp"

namespace ug::graphics{

class mesh3d_render : public render{
public:

	mesh3d_render(app*);
	void operator()(scene const& s, mesh3d const& mesh);


	float ambient_light_strength = 0.2f;
	bool specular_lighting = true;
	float specular_strength = 0.5f;
	bool diffuse_lighting = true;

private:
	ug::graphics::program m_program;
};

} // end of namespace ug::graphics
