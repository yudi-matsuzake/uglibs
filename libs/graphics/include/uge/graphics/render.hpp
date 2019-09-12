#pragma once

#include "graphics/app.hpp"

namespace graphics{

class render{
public:
	render(app* app_ptr);

	app* get_app();

protected:
	app* m_app;
};

} // end of namespace graphics
