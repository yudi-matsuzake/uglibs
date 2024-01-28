#pragma once

#include <ranges>
#include <cstdint>

#include "ug/graphics/basic-texture.hpp"

namespace ug::graphics{

namespace rgs = std::ranges;

class texture1d : public basic_texture {
private:
	texture1d();
public:
	template<rgs::range R>
	explicit texture1d(
		R const& data,
		color_type internal_format,
		color_type pixel_format,
		wrap_type s_wrap,
		filter_type min_filter,
		filter_type mag_filter)

		: texture1d()
	{
		set_texture_wrap_s(s_wrap);

		set_texture_minifier_filter(min_filter);
		set_texture_magnifier_filter(mag_filter);

		set_data(data, internal_format, pixel_format);
		generate_mipmap();
	}

	texture1d(texture1d&&);
	texture1d& operator=(texture1d&&);

	void activate(uint32_t texture_unit) const;

	template<rgs::range R>
	void set_data(
		R const& data,
		color_type internal_format,
		color_type pixel_format)
	{
		bind();
		GL(glTexImage1D(
			m_target,
			0,
			static_cast<int32_t>(internal_format),
			static_cast<int32_t>(rgs::size(data)),
			0,
			static_cast<uint32_t>(pixel_format),
			type_code_v<rgs::range_value_t<R>>,
			rgs::cdata(data)
		));
	}
};

} // end of namespace graphics
