#pragma once

namespace ug::graphics{

struct key_input{
	int key;
	int scancode;
	int action;
	int mods;
};

struct scroll_input{
	double x_offset, y_offset;
};

} // end of namespace ug::graphics
