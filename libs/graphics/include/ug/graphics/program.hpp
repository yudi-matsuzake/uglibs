#pragma once

#include <ranges>
#include "ug/graphics/misc.hpp"
#include "ug/graphics/shader.hpp"

namespace ug::graphics{

namespace rgs = std::ranges;

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
	void set_uniform(char const* name, glm::vec3 const& v) const;
	void set_uniform(char const* name, glm::vec4 const& v) const;
	void set_uniform(char const* name, bool b) const;

	void set_uniform(char const* name, int32_t n) const;

	template<rgs::range R>
		requires (std::is_same_v<rgs::range_value_t<R>, float>)
	void set_uniform(char const* name, R const& r) const
	{
		int32_t id;
		GL(id = glGetUniformLocation(m_id, name));
		GL(glUniform1fv(
			id,
			static_cast<int32_t>(rgs::size(r)),
			rgs::cdata(r))
		);
	}

	template<rgs::range R>
		requires (std::is_same_v<rgs::range_value_t<R>, int32_t>)
	void set_uniform(char const* name, R const& r) const
	{
		int32_t id;
		GL(id = glGetUniformLocation(m_id, name));
		GL(glUniform1iv(
			id,
			static_cast<int32_t>(rgs::size(r)),
			rgs::cdata(r))
		);
	}

protected:
	uint32_t m_id;

};

} // end of namespace ug::graphics
