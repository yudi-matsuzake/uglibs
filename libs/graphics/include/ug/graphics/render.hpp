#pragma once

#include "ug/graphics/app.hpp"

namespace ug::graphics{

class render{
public:
	render(app* app_ptr);

	app const* get_app() const;
	app* get_app();

protected:
	app* m_app;
};

} // end of namespace graphics
