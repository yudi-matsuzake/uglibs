#pragma once

#include <array>
#include <vector>
#include <algorithm>

#include "ug/graphics/opengl-error.hpp"

namespace ug::graphics{

template<uint64_t N>
using static_shader_source = std::array<char const*, N>;

using shader_source = std::vector<std::string>;

class shader{
public:
	shader(unsigned int shader_type);

	shader(shader const&) = delete;

	virtual ~shader();

	uint32_t id() const;

	template<uint64_t N>
	void set_source(static_shader_source<N> const& source)
	{
		GL(glShaderSource(m_id, N, source.data(), NULL));
	}

	void set_source(shader_source const& source);

	void compile() const;

protected:
	uint32_t m_id;
};

class vertex_shader : public shader{
public:
	vertex_shader();
};

class fragment_shader : public shader{
public:
	fragment_shader();
};

} // end of namespace ug::graphics
