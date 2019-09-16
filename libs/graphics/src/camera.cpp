#include "ug/graphics/camera.hpp"

namespace ug::graphics{

orthographic_camera::orthographic_camera(rect2d const& r)
	: m_inverse_view_matrix{1.f},
	  m_near_plane(r)
{}

orthographic_camera::orthographic_camera(
	float left,
	float right,
	float bottom,
	float top)
	: orthographic_camera({ {left, top}, right - left, top - bottom})
{}

void orthographic_camera::translate(glm::vec3 const& v)
{
	m_inverse_view_matrix = glm::translate(m_inverse_view_matrix, v);
	m_near_plane.position += glm::vec2{ v.x, v.y };
}

void orthographic_camera::rotate(float r, glm::vec3 const& v)
{
	m_inverse_view_matrix = glm::rotate(
		m_inverse_view_matrix,
		r,
		v
	);
}

void orthographic_camera::rotate(float r)
{

	rotate(r, glm::vec3(0.f, 0.f, 1.f));
}

void orthographic_camera::zoom(float proportion)
{
	if(proportion <= .0f){
		throw std::runtime_error(
			"zoom proportion cannot be equal or greater than zero"
		);
	}

	glm::vec3 v = { m_near_plane.width, m_near_plane.height, 0.f };

	v *= 1.f - proportion;

	v *= 1.f/2.f;

	m_near_plane *= proportion;

	translate(v);
}

glm::mat4 orthographic_camera::view_matrix() const
{
	return glm::inverse(m_inverse_view_matrix);
}

glm::mat4 orthographic_camera::projection_matrix() const
{
	return glm::ortho(
		0.f,
		m_near_plane.width,
		0.f,
		m_near_plane.height,
		-1.f,
		1.f
	);
}

} // end of namespace ug::graphics
