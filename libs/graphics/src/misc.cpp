#include <vector>

#include "graphics/misc.hpp"

namespace graphics{

/*
 * structs
 * =======
 */

void operator*=(rect2d& r, float scalar)
{

	r.height *= scalar;
	r.width *= scalar;
}

void operator+=(rect2d& r, glm::vec2 const& v)
{
	r.position += v;
}

void operator-=(rect2d& r, glm::vec2 const& v)
{
	r.position -= v;
}

glm::vec2 rect2d::center() const
{
	return {
		position.x + width/2.f,
		position.y + height/2.f
	};
}

glm::vec2 rect2d::size() const
{
	return { width, height };
}

} // end of namespace graphics
