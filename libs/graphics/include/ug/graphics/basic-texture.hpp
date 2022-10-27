#pragma once

#include <optional>
#include <cstdint>
#include "ug/graphics/misc.hpp"

namespace ug::graphics{

enum class wrap_type : int32_t {
	CLAMP_TO_EDGE		= GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER		= GL_CLAMP_TO_BORDER,
	MIRRORED_REPEAT		= GL_MIRRORED_REPEAT,
	REPEAT			= GL_REPEAT,
	MIRROR_CLAMP_TO_EDGE	= GL_MIRROR_CLAMP_TO_EDGE
};

enum class filter_type : int32_t {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR
};

class basic_texture {
public:

	basic_texture(uint32_t target);
	virtual ~basic_texture();

	basic_texture(basic_texture const&) = delete;
	basic_texture& operator=(basic_texture const&) = delete;

	basic_texture(basic_texture&&);
	basic_texture& operator=(basic_texture&&);


	uint32_t id() const;
	void bind() const;

	void set_texture_wrap_s(wrap_type wrap);
	void set_texture_wrap_t(wrap_type wrap);

	void set_texture_magnifier_filter(filter_type filter);
	void set_texture_minifier_filter(filter_type filter);

	void set_border_color(color4 const& border_color) const;

	void generate_mipmap();

	void activate(uint32_t texture_unit) const;

protected:
	std::optional<uint32_t> m_id;
	uint32_t m_target;
};


} // end of namespace ug::graphics
