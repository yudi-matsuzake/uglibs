#pragma once

#include <cstdint>

#include "ug/graphics/basic-texture.hpp"

namespace ug::graphics{

class texture2d : public basic_texture {
private:
	texture2d();
public:
	explicit texture2d(
		unsigned char const* data,
		int32_t width,
		int32_t height,
		wrap_type s_wrap = wrap_type::REPEAT,
		wrap_type t_wrap = wrap_type::REPEAT,
		filter_type min_filter = filter_type::LINEAR,
		filter_type mag_filter = filter_type::LINEAR
	);

	texture2d(texture2d&&);
	texture2d& operator=(texture2d&&);

	void set_data(unsigned char const* data) override;
	void activate(uint32_t texture_unit) const;

private:
	int32_t m_width = -1;
	int32_t m_height = -1;
};

} // end of namespace graphics
