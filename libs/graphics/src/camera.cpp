#include "ug/graphics/camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"

namespace ug::graphics{

camera::camera(const glm::vec3& pos)
	: position(pos)
{}

[[nodiscard]] const glm::quat camera::orientation() const
{
	glm::quat q = glm::angleAxis(
		x_angle,
		glm::vec3(1., 0., 0.)
	);

	q *= glm::angleAxis(y_angle, glm::vec3(.0, 1., 0.));
	q *= glm::angleAxis(z_angle, glm::vec3(0., 0., 1.));

	return q;
}

void camera::translate(const glm::vec3& v)
{
	position += v*orientation();
}

void camera::pitch(float angle)
{
	x_angle += angle;
}

void camera::yall(float angle)
{
	y_angle += angle;
}

void camera::roll(float angle)
{
	z_angle += angle;
}

[[nodiscard]] glm::mat4 camera::view() const
{
	return glm::inverse(glm::translate(glm::mat4_cast(orientation()), position));
}

} // end of namespace ug::graphics
