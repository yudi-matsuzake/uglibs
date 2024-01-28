#pragma once

#include <ranges>
#include <cstdint>

#include "ug/graphics/buffers.hpp"
#include "ug/graphics/basic-texture.hpp"

namespace ug::graphics{

namespace rgs = std::ranges;

class buffer_texture : public basic_texture {
private:
	buffer_texture();
public:
	template<rgs::range R>
	explicit buffer_texture(R const& data)
		: buffer_texture()
	{
		set_data(data);
	}

	buffer_texture(buffer_texture&&);
	buffer_texture& operator=(buffer_texture&&);

	void activate(uint32_t texture_unit) const;

	void bind() const override;

	template<rgs::range R>
	void set_data(R const& data)
	{
		bind();
		m_buffer.set_data(data);
		GL(glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, m_buffer.id()));
	}

private:
	ug::graphics::tdb m_buffer;
};

} // end of namespace graphics
