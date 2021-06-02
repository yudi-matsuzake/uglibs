#pragma once

#include <optional>

#include "ug/graphics/misc.hpp"

namespace ug::graphics{

class camera{
public:
	camera() = default;

	explicit camera(glm::vec3 const&);

	explicit camera(glm::vec3 const&, glm::vec3 const&);

	glm::vec3 const& position() const;
	glm::vec3 const& direction() const;

	[[nodiscard]] glm::mat4 view() const;
	void translate(glm::vec3 const& v);
	void aim_to(glm::vec3 const& p);

private:
	[[nodiscard]] glm::vec3 compute_right_axis() const;
	[[nodiscard]] glm::vec3 compute_up_axis(
		std::optional<glm::vec3> const& right = std::nullopt) const;

	glm::vec3 m_position = glm::vec3{0.f, 0.f, 1.f};
	glm::vec3 m_direction = glm::vec3{0.f, 0.f, -1.f};
};

} // end of namespace ug::graphics
