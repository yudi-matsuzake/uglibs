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

private:
	ug::graphics::program m_program;
};

} // end of namespace ug::graphics
