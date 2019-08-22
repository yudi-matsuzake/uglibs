#pragma once

#include <cstdint>

#include "graphics/misc.hpp"

namespace graphics{

enum class wrap_type : int32_t {
	clamp_to_edge		= GL_CLAMP_TO_EDGE,
	clamp_to_border		= GL_CLAMP_TO_BORDER,
	mirrored_repeat		= GL_MIRRORED_REPEAT,
	repeat			= GL_REPEAT,
	mirror_clamp_to_edge	= GL_MIRROR_CLAMP_TO_EDGE
};

enum class filter_type : int32_t {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR
};

class texture2d{
public:

	texture2d();
	texture2d(
		uint8_t* data,
		int32_t width,
		int32_t height,
		wrap_type s_wrap = wrap_type::repeat,
		wrap_type t_wrap = wrap_type::repeat,
		filter_type min_filter = filter_type::LINEAR,
		filter_type mag_filter = filter_type::LINEAR
	);

	virtual ~texture2d();

	uint32_t id() const;
	void bind(uint32_t texture_unit = 0) const;

	void set_texture_wrap_s(wrap_type wrap);
	void set_texture_wrap_t(wrap_type wrap);

	void set_texture_magnifier_filter(filter_type filter);
	void set_texture_minifier_filter(filter_type filter);

	void generate_mipmap();

	void set_data(uint8_t* data, int32_t width, int32_t height);

protected:
	uint32_t m_id;
};

} // end of namespace graphics
