#include <vector>

#include "ug/graphics/misc.hpp"

namespace ug::graphics{

/*
 * structs
 * =======
 */

void operator*=(rect2d& r, float s)
{

	r.height *= s;
	r.width *= s;
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
		position.y - height/2.f
	};
}

glm::vec2 rect2d::size() const
{
	return { width, height };
}

float rect2d::left() const
{
	return position.x;
}

float rect2d::right() const
{
	return position.x + width;
}

float rect2d::bottom() const
{
	return position.y - height;
}

float rect2d::top() const
{
	return position.y;
}

} // end of namespace graphics
