#pragma once

#include "ug/graphics/misc.hpp"

namespace ug::graphics{

class camera{
public:
	camera() = default;

	camera(const glm::vec3&);

	[[nodiscard]] const glm::quat orientation() const;
	[[nodiscard]] glm::mat4 view() const;

	void translate(const glm::vec3& v);

	void pitch(float angle);
	void yall(float angle);
	void roll(float angle);

private:
	glm::vec3 position = glm::vec3(0., 0., 0.);

	float x_angle = 0.f;
	float y_angle = 0.f;
	float z_angle = 0.f;
};

} // end of namespace ug::graphics
