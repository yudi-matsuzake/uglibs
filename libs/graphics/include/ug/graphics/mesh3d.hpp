#pragma once
#include <vector>
#include "ug/graphics/misc.hpp"
#include "ug/graphics/buffers.hpp"
#include "ug/graphics/vao.hpp"

namespace ug::graphics{

struct mesh3d_attributes_layout{
	glm::vec3 vertex;
	glm::vec3 normal;
	ug::graphics::color4 color;
};

class mesh3d{
public:

	explicit mesh3d(
		std::vector<mesh3d_attributes_layout> const&& attributes,
		std::vector<uint32_t> const&& indices);

	void bind() const;

	glm::mat4 const& model_matrix() const;

	std::vector<mesh3d_attributes_layout> const& attributes() const;

	std::vector<uint32_t> const& indices() const;

	void transform(glm::mat4 const&);

private:
	ug::graphics::vao vao;
	ug::graphics::vbo vbo;
	ug::graphics::ebo ebo;

	std::vector<mesh3d_attributes_layout>	m_attributes;
	std::vector<uint32_t>	m_indices;
	glm::mat4 model = glm::mat4(1.);
};


} // end of namespace ug::graphics
