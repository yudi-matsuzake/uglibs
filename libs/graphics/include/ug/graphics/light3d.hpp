#pragma once

#include "ug/graphics/misc.hpp"

namespace ug::graphics {

struct light3d {
	glm::vec3 position;
	ug::graphics::color3 color;
};

}
