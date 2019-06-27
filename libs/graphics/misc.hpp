#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/opengl-error.hpp"

namespace graphics{

/*
 * structs and aliases
 * ===================
 */
using color = glm::vec4;

struct rect2d{
	glm::vec2 position;
	float width, height;

	friend void operator*=(rect2d& r, float scalar);
	friend void operator+=(rect2d& r, glm::vec2 const& v);
	friend void operator-=(rect2d& r, glm::vec2 const& v);

	glm::vec2 center() const;
};

} // end of namespace graphics
