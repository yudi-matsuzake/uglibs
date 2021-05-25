#include "ug/graphics/camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"

namespace ug::graphics{

camera::camera(const glm::vec3& pos)
	: m_position(pos)
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

glm::vec3 const& camera::position() const
{
	return m_position;
}

void camera::translate(const glm::vec3& v)
{
	m_position += v*orientation();
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
	auto const view = glm::translate(glm::mat4_cast(orientation()), m_position);
	return glm::inverse(view);
}

} // end of namespace ug::graphics
