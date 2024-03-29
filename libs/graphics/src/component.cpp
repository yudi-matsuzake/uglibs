#include "ug/graphics/component.hpp"

namespace ug::graphics{


component::component(app* app_ptr)
	: m_app(app_ptr)
{}

app* component::get_app()
{
	return m_app;
}

app const* component::get_app() const
{
	return m_app;
}

void component::update()
{}

void component::draw()
{}

void component::draw_ui()
{}

void component::finally()
{}

void component::on_key_input(key_input const&)
{}

void component::on_scroll_input(scroll_input const&)
{}

void component::on_drop_path(path_input const&)
{}

} // end of namespace graphics
