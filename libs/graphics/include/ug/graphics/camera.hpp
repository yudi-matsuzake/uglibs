#pragma once

#include "ug/graphics/misc.hpp"

namespace ug::graphics{

class camera{
public:
	virtual ~camera() = default;

	virtual glm::mat4 view_matrix() const = 0;
	virtual glm::mat4 projection_matrix() const = 0;
	virtual void translate(glm::vec3 const& v) = 0;

	/**
	  * zoom
	  * @param	proportion proportion of the zoom,
	  *		i.e. < 1.0 zoom in > 1 zoom out
	  */
	virtual void zoom(float proportion) = 0;
};

class orthographic_camera : public camera{
public:

	explicit orthographic_camera(rect2d const& r);

	explicit orthographic_camera(
		float left,
		float right,
		float bottom,
		float top);

	virtual glm::mat4 view_matrix() const override;
	virtual glm::mat4 projection_matrix() const override;

	void translate(glm::vec3 const& v) override;
	void rotate(float r);
	void rotate(float r, glm::vec3 const& v);

	virtual void zoom(float proportion) override;

protected:
	glm::mat4 m_inverse_view_matrix{1.f};
	rect2d m_near_plane{ {.0f, .0f}, .0f, .0f};
};

} // end of namespace ug::graphics
