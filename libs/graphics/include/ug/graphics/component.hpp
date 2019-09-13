#pragma once

#include "ug/graphics/inputs.hpp"

namespace ug::graphics{

class app;

/**
  * this class represents a app component
  */
class component{
public:
	component(app*);

	virtual ~component() = default;

	app* get_app();
	app const* get_app() const;

	virtual void update();
	virtual void draw();
	virtual void draw_ui();

	virtual void on_key_input(key_input const&);
	virtual void on_scroll_input(scroll_input const&);
	virtual void on_drop_path(path_container const&);

protected:
	app* m_app = nullptr;
};

} // end of namespace graphics
