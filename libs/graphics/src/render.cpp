#include "ug/graphics/render.hpp"

namespace ug::graphics{

render::render(app* app_ptr)
	: m_app(app_ptr)
{}

app* render::get_app()
{
	return m_app;
}

} // end of namespace graphics
