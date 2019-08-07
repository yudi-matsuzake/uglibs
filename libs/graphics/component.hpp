#pragma once

namespace graphics{

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
	virtual ~component() = default;

	virtual void update();
	virtual void draw();

	virtual void on_key_input(key_input const&);
	virtual void on_scroll_input(scroll_input const&);
};

} // end of namespace graphics
