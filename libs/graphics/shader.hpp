#pragma once

#include <vector>
#include <algorithm>

#include "graphics/misc.hpp"

namespace graphics{

using shader_source = std::vector<char>;

class shader{
public:
	shader(unsigned int shader_type);

	virtual ~shader();

	unsigned int id() const;

	template<class I>
	void set_source(I begin_it, I end_it)
	{
		m_source.resize(std::distance(begin_it, end_it));
		std::copy(begin_it, end_it, begin(m_source));

		char* data = m_source.data();
		auto lenghts = static_cast<int>(m_source.size());
		GL(glShaderSource(m_id, 1, &data, &lenghts));
	}

	void compile() const;

protected:

	unsigned int m_id;
	shader_source m_source;
};

class vertex_shader : public shader{
public:
	vertex_shader();
};

class fragment_shader : public shader{
public:
	fragment_shader();
};

}
