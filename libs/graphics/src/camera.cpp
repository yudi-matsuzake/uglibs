#include "ug/graphics/camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>

namespace ug::graphics{

[[nodiscard]] glm::vec3 camera::compute_right_axis() const
{
	static constexpr auto up = glm::vec3{ 0.f, 1.f, 0.f };
	return glm::normalize(glm::cross(direction, up));
}

[[nodiscard]] glm::vec3 camera::compute_up_axis(
	std::optional<glm::vec3> const& right) const
{
	auto const r = right.has_value() ? right.value() : compute_right_axis();
	return glm::normalize(glm::cross(r, direction));
}

[[nodiscard]] glm::mat4 camera::view() const
{
	return glm::lookAt(position, position + direction, compute_up_axis());
}

void camera::translate(glm::vec3 const& offset)
{
	auto const right = compute_right_axis();
	auto const up = compute_up_axis(right);
	position += offset.x * right + offset.y * up + offset.z * -direction;
}

void camera::aim_to(glm::vec3 const& p)
{
	direction = glm::normalize(p - position);
}

} // end of namespace ug::graphics
