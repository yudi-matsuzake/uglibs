#pragma once

#include <cstdint>

namespace graphics{

class vao{
public:
	vao();
	virtual ~vao();

	void bind() const;

	uint32_t id() const;

protected:
	uint32_t m_id;
};

} // end of namespace graphics
