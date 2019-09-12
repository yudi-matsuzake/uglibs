#pragma once

#include "ug/graphics/shader.hpp"

namespace ug::graphics{

class program{
public:
	program();
	virtual ~program();

	void attach_shader(shader const& s) const;
	void use() const;
	void link() const;
	uint32_t id() const; 

	void set_uniform(char const* name, float f1) const;
	void set_uniform(char const* name, float f1, float f2) const;
	void set_uniform(char const* name, float f1, float f2, float f3) const;
	void set_uniform(
		char const* name,
		float f1,
		float f2,
		float f3,
		float f4
	) const;
	void set_uniform(char const* name, glm::mat4 const& m) const;
	void set_uniform(char const* name, glm::vec2 const& v) const;
	void set_uniform(char const* name, glm::vec4 const& v) const;
	void set_uniform(char const* name, bool b) const;

	void set_uniform(char const* name, int32_t n) const;

protected:
	uint32_t m_id;

};

} // end of namespace graphics
