#include "ug/graphics/mesh3d.hpp"

namespace ug::graphics{

mesh3d::mesh3d(
	std::vector<mesh3d_attributes_layout>&& a_attributes,
	std::vector<uint32_t>&& a_indices)
	: m_attributes(a_attributes), m_indices(a_indices)
{
	vao.bind();
	vbo.set_data(m_attributes.data(), m_attributes.size());
	ebo.set_data(m_indices.data(), m_indices.size());

	vao.set_attribute_layout({
		vao::attr<float>{3},
		vao::attr<float>{3},
		vao::attr<float>{4}
	});
}

void mesh3d::bind() const
{
	vao.bind();
	vbo.bind();
	ebo.bind();
}

glm::mat4 const& mesh3d::model_matrix() const
{
	return model;
}

std::vector<mesh3d_attributes_layout> const& mesh3d::attributes() const
{
	return m_attributes;
}

std::vector<uint32_t> const& mesh3d::indices() const
{
	return m_indices;
}

void mesh3d::transform(glm::mat4 const& t)
{
	model = model*t;
}

} // end of namespace ug::graphics
