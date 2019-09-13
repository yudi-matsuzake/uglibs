#pragma once

#include <vector>
#include <filesystem>

namespace ug::graphics{

using path_container = std::vector<std::filesystem::path>;

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
