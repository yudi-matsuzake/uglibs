#pragma once

#include <cstdint>

namespace graphics{

class texture2d{
public:

	texture2d(int32_t width, int32_t height, void* data);
	virtual ~texture2d();

	uint32_t id() const;

	void bind() const;

protected:
	uint32_t m_id;
};

} // end of namespace graphics
