#include "ug/graphics/app3d.hpp"

namespace ug::graphics{

app3d::app3d(
	int32_t width,
	int32_t height,
	char const* window_title,
	projection_type proj_type)
	: app(width, height, window_title, proj_type)
{
	GL(glEnable(GL_DEPTH_TEST));
	GL(glEnable(GL_BLEND));
	GL(glEnable(GL_LINE_SMOOTH));
	GL(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
	GL(glViewport(0, 0, 1280, 720));
	GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	set_clear_flags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


} // end of namespace ug::graphics
