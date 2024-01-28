#pragma once

#include <cstdint>
#include <ranges>

#include "ug/graphics/basic-texture.hpp"

namespace ug::graphics{

namespace rgs = std::ranges;

class texture2d : public basic_texture {
private:
	texture2d();
public:

	template<rgs::range R>
	texture2d(
		R const& data,
		color_type internal_format,
		color_type pixel_format,
		int32_t width,
		int32_t height,
		wrap_type s_wrap,
		wrap_type t_wrap,
		filter_type min_filter,
		filter_type mag_filter)

		: texture2d()
	{
		m_width = width;
		m_height = height;

		set_texture_wrap_s(s_wrap);
		set_texture_wrap_t(t_wrap);

		set_texture_minifier_filter(min_filter);
		set_texture_magnifier_filter(mag_filter);

		set_data(data, internal_format, pixel_format);
		generate_mipmap();
	}

	texture2d(texture2d&&);
	texture2d& operator=(texture2d&&);

	template<rgs::range R>
	void set_data(
		R const& data,
		color_type internal_format,
		color_type pixel_format)
	{
		bind();
		GL(glTexImage2D(
			m_target,
			0,
			static_cast<int32_t>(internal_format),
			m_width, m_height,
			0,
			static_cast<uint32_t>(pixel_format),
			type_code_v<rgs::range_value_t<R>>,
			rgs::cdata(data)
		));
	}

	void activate(uint32_t texture_unit) const;

private:
	int32_t m_width = -1;
	int32_t m_height = -1;
};

} // end of namespace graphics
