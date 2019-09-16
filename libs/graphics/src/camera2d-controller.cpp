#include "ug/graphics/camera2d-controller.hpp"

#include "ug/graphics/app.hpp"

namespace ug::graphics{

template<class T>
static bool is_at_least_one_pressed(app const* a, T const& array)
{
	for(auto&& k : array)
		if(a->is_key_pressed(k))
			return true;
	return false;
}

camera2d_controller::camera2d_controller(app* app_ptr)
	: component(app_ptr)
{}

void camera2d_controller::on_scroll_input(scroll_input const& input)
{
	if(input.y_offset < 0.)
		scroll.out = true;
	else if(input.y_offset > 0.)
		scroll.in = true;
}

void camera2d_controller::update()
{

	auto cam = get_app()->camera();

	// zoom
	{
		auto is_zoom_in_pressed = is_at_least_one_pressed(
			get_app(),
			zoom_in_keys
		);

		auto is_zoom_out_pressed = is_at_least_one_pressed(
			get_app(),
			zoom_out_keys
		);

		intentions.zoom_in	= is_zoom_in_pressed || scroll.in;
		intentions.zoom_out	= is_zoom_out_pressed || scroll.out;

		scroll.in = scroll.out = false;

		if(intentions.zoom_in){
			intentions.zoom_in = false;
			cam->zoom(zoom_in_proportion);
		}

		if(intentions.zoom_out){
			intentions.zoom_out = false;
			cam->zoom(zoom_out_proportion);
		}
	}

	// translation
	{
		m_speed = std::max(
			get_app()->get_projected_viewport().width,
			get_app()->get_projected_viewport().height
		);

		glm::vec3 translation = build_translation_vector();

		cam->translate(translation);
	}
}


bool camera2d_controller::up() const
{
	return is_at_least_one_pressed(get_app(), up_keys);
}

bool camera2d_controller::down() const
{
	return is_at_least_one_pressed(get_app(), down_keys);
}

bool camera2d_controller::left() const
{
	return is_at_least_one_pressed(get_app(), left_keys);
}

bool camera2d_controller::right() const
{
	return is_at_least_one_pressed(get_app(), right_keys);
}

glm::vec3 camera2d_controller::build_translation_vector() const
{
	/*
	 * button translation
	 * ==================
	 */
	glm::vec3 button_translation{ .0f, .0f, .0f };
	if(up())	button_translation.y += 1.f;
	if(down())	button_translation.y -= 1.f;
	if(right())	button_translation.x += 1.f;
	if(left())	button_translation.x -= 1.f;

	auto l = glm::length(button_translation);
	if(l > 0.f){
		auto fd = static_cast<float>(get_app()->get_delta());
		auto length = m_speed * fd;
		button_translation = glm::normalize(button_translation)*length;
	}

	/*
	 * mouse translation
	 */
	glm::vec3 mouse_translation{ .0f, .0f, .0f };

	static glm::vec2 last_cursor_position{ .0f, .0f };

	auto mv = get_app()->get_cursor_vector();

	if(get_app()->is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)){
		auto mov = mv - last_cursor_position;
		auto const& vp = get_app()->get_viewport();
		auto const& np = get_app()->get_projected_viewport();

		auto s = glm::vec2(
			-1.f*np.width/vp.width,
			np.height/vp.height
		);
		mouse_translation = glm::vec3(mov*s, 0.f);

		last_cursor_position = mv;
	}else{
		last_cursor_position = mv;
	}


	return button_translation + mouse_translation;
}

} // end of namespace ug::graphics
