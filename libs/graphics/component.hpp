#pragma once

namespace graphics{

class app;

struct key_input{
	int key;
	int scancode;
	int action;
	int mods;
};

struct scroll_input{
	double x_offset, y_offset;
};

/**
  * this class represents a app component
  */
class component{
public:
	component(app*);

	virtual ~component() = default;

	app* get_app();

	virtual void update();
	virtual void draw();
	virtual void draw_ui();

	virtual void on_key_input(key_input const&);
	virtual void on_scroll_input(scroll_input const&);

protected:
	app* m_app = nullptr;
};

} // end of namespace graphics
