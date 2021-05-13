#pragma once

#include <vector>
#include <filesystem>

namespace ug::graphics{

using path_input = std::vector<std::filesystem::path>;

struct key_input{
	int key;
	int scancode;
	int action;
	int mods;
};

struct scroll_input{
	double x_offset, y_offset;
};

struct mouse_button_input{
	int button;
	int action;
	int mods;
};


} // end of namespace ug::graphics
