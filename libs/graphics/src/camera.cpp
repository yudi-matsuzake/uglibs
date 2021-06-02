#include "ug/graphics/camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>

namespace ug::graphics{

camera::camera(const glm::vec3& pos)
	: m_position(pos)
{}

camera::camera(glm::vec3 const& pos, glm::vec3 const& dir)
	: m_position(pos), m_direction(dir)
{}

[[nodiscard]] glm::vec3 camera::compute_right_axis() const
{
	static constexpr auto up = glm::vec3{ 0.f, 1.f, 0.f };
	return glm::normalize(glm::cross(m_direction, up));
}

[[nodiscard]] glm::vec3 camera::compute_up_axis(
	std::optional<glm::vec3> const& right) const
{
	auto const r = right.has_value() ? right.value() : compute_right_axis();
	return glm::normalize(glm::cross(r, m_direction));
}

glm::vec3 const& camera::position() const
{
	return m_position;
}

[[nodiscard]] glm::mat4 camera::view() const
{
	return glm::lookAt(m_position, m_position + m_direction, compute_up_axis());
}

void camera::translate(glm::vec3 const& offset)
{
	auto const right = compute_right_axis();
	auto const up = compute_up_axis(right);
	m_position += offset.x * right + offset.y * up + offset.z * -m_direction;
}

void camera::aim_to(glm::vec3 const& p)
{
	m_direction = glm::normalize(p - m_position);
}

} // end of namespace ug::graphics
