#include "graphics/app.hpp"
#include "graphics/misc.hpp"
#include "graphics/orthographic-zoom.hpp"

namespace graphics{

ortographic_zoom::ortographic_zoom(app* app_ptr)
	: component(app_ptr)
{}

void ortographic_zoom::on_scroll_input(scroll_input const& input)
{
	if(input.y_offset < 0.)
		scroll.out = true;
	else if(input.y_offset > 0.)
		scroll.in = true;
}

void ortographic_zoom::update()
{

	auto is_equal_pressed = get_app()->is_key_pressed(GLFW_KEY_EQUAL);
	auto is_minus_pressed = get_app()->is_key_pressed(GLFW_KEY_MINUS);

	intentions.zoom_in =	is_equal_pressed || scroll.in;
	intentions.zoom_out =	is_minus_pressed || scroll.out;

	scroll.in = scroll.out = false;

	auto proj_frustrum = get_app()->get_near_plane();

	if(intentions.zoom_in){
		glm::vec2 v = {
			proj_frustrum.width,
			proj_frustrum.height
		};

		v *= 1.f - zoomin_proportion;
		v *= 1.f/2.f;

		proj_frustrum *= zoomin_proportion;

		proj_frustrum += v;
	}

	if(intentions.zoom_out){
		intentions.zoom_out = false;

		glm::vec2 v = {
			proj_frustrum.width,
			proj_frustrum.height
		};

		v *= zoomout_proportion - 1.f;
		v *= 1.f/2.f;

		proj_frustrum *= zoomout_proportion;

		proj_frustrum -= v;
	}

	get_app()->set_projection_matrix(
		glm::ortho(
		proj_frustrum.position.x,
		proj_frustrum.position.x + proj_frustrum.width,
		proj_frustrum.position.y,
		proj_frustrum.position.y + proj_frustrum.height,
		1.f, -1.f
	));

	get_app()->set_near_plane(proj_frustrum);

}

} // end of namespace graphics
