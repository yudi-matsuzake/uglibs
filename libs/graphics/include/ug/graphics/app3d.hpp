#pragma once

#include "ug/graphics/app.hpp"

namespace ug::graphics{


class app3d : public app{
public:
	
	using projection_type = typename app::projection_type;

	explicit app3d(
		int32_t width,
		int32_t height,
		char const* window_title,
		projection_type proj_type = projection_type::PERSPECTIVE
	);

	virtual ~app3d() = default;

};


} // end of namespace ug::graphics
