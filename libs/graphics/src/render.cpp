#include "graphics/render.hpp"

namespace graphics{

render::render(app* app_ptr)
	: m_app(app_ptr)
{}

app* render::get_app()
{
	return m_app;
}

} // end of namespace graphics
