#include "ug/graphics/mesh3d.hpp"

#include <numeric>

namespace ug::graphics{

void mesh3d::initialize_ebos()
{
	std::vector<uint32_t> indices;
	auto const n_lines = 3UL*m_attributes.size();
	indices.reserve(n_lines);
	for(auto t = 0UL; t<m_attributes.size(); t += 3UL){
		auto const a = t, b = t + 1, c = t + 2;
		indices.emplace_back(a);
		indices.emplace_back(b);
		indices.emplace_back(b);
		indices.emplace_back(c);
		indices.emplace_back(c);
		indices.emplace_back(a);
	}

	ebo_lines.set_data(indices.data(), indices.size());
}

mesh3d::mesh3d(
	std::vector<mesh3d_attributes_layout>&& a_attributes)
	: m_attributes(a_attributes)
{
	vao.bind();
	vbo.set_data(m_attributes.data(), m_attributes.size());

	vao.set_attribute_layout({
		vao::attr<float>{3},
		vao::attr<float>{3},
		vao::attr<float>{4}
	});

	initialize_ebos();
}

void mesh3d::bind() const
{
	vao.bind();
	vbo.bind();
}

void mesh3d::draw_triangles() const
{
	bind();
	auto const n_vertex = static_cast<int32_t>(m_attributes.size());
	GL(glDrawArrays(GL_TRIANGLES, 0, n_vertex));
}

void mesh3d::draw_lines() const
{
	bind();
	ebo_lines.bind();
	auto const n_lines = static_cast<int32_t>(m_attributes.size() * 3UL);
	GL(glDrawElements(GL_LINES, n_lines, GL_UNSIGNED_INT, 0));
}

glm::mat4 const& mesh3d::model_matrix() const
{
	return model;
}

std::vector<mesh3d_attributes_layout> const& mesh3d::attributes() const
{
	return m_attributes;
}

void mesh3d::transform(glm::mat4 const& t)
{
	model = model*t;
}

} // end of namespace ug::graphics
