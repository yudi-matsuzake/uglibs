#pragma once

#include <memory>
#include <vector>

#include "ug/graphics/camera.hpp"
#include "ug/graphics/mesh3d.hpp"

namespace ug::graphics{

struct light3d{
	glm::vec3 position;
	ug::graphics::color3 color;
};

/**
  * struct that represents a 3d scene to be rendered
	*
	* TODO: change mesh3d for object with some kind of inheritance or
	* polymorphism
	*/
struct scene{
	ug::graphics::camera camera;
	std::vector<std::unique_ptr<mesh3d>> meshes;
	std::vector<light3d> lights;
};

} // end of namespace ug::graphics
