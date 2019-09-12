#pragma once

#include <cstdint>

#include "graphics/misc.hpp"

namespace graphics{

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

class texture2d{
public:

	texture2d();
	texture2d(
		unsigned char const* data,
		int32_t width,
		int32_t height,
		wrap_type s_wrap = wrap_type::REPEAT,
		wrap_type t_wrap = wrap_type::REPEAT,
		filter_type min_filter = filter_type::LINEAR,
		filter_type mag_filter = filter_type::LINEAR
	);

	virtual ~texture2d();

	uint32_t id() const;
	void bind() const;

	void set_texture_wrap_s(wrap_type wrap);
	void set_texture_wrap_t(wrap_type wrap);

	void set_texture_magnifier_filter(filter_type filter);
	void set_texture_minifier_filter(filter_type filter);

	void set_border_color(color const& border_color) const;

	void generate_mipmap();

	void set_data(unsigned char const* data, int32_t width, int32_t height);

	void activate(uint32_t texture_unit) const;

protected:
	uint32_t m_id;
};

} // end of namespace graphics
