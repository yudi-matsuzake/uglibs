#include "ug/graphics/camera2d-controller.hpp"
#include "ug/graphics/app2d.hpp"

namespace ug::graphics{

template<class T>
static bool is_at_least_one_pressed(ug::graphics::app2d const* a, T const& array)
{
	for(auto&& k : array)
		if(a->is_key_pressed(k))
			return true;
	return false;
}

camera2d_controller::camera2d_controller(ug::graphics::app2d* app_ptr)
	: component(app_ptr),
	  m_app2d(app_ptr)
{}

app2d const* camera2d_controller::get_app2d() const
{
	return m_app2d;
}

app2d* camera2d_controller::get_app2d()
{
	return m_app2d;
}

void camera2d_controller::on_scroll_input(scroll_input const& input)
{
	if(input.y_offset < 0.)
		scroll.out = true;
	else if(input.y_offset > 0.)
		scroll.in = true;
}

void camera2d_controller::update_zoom()
{
	auto is_zoom_in_pressed = is_at_least_one_pressed(
		get_app2d(),
		zoom_in_keys
	);

	auto is_zoom_out_pressed = is_at_least_one_pressed(
		get_app2d(),
		zoom_out_keys
	);

	intentions.zoom_in	= is_zoom_in_pressed || scroll.in;
	intentions.zoom_out	= is_zoom_out_pressed || scroll.out;

	scroll.in = scroll.out = false;

	struct{
		bool enabled;
		glm::vec2 mpos_before;
	} zoom = {
		false,
		get_app2d()->compute_space_point_of_cursor_position()
	};

	if(intentions.zoom_in){
		zoom.enabled = true;

		intentions.zoom_in = false;
		get_app2d()->zoom(zoom_in_proportion);
	}

	if(intentions.zoom_out){
		zoom.enabled = true;

		intentions.zoom_out = false;
		get_app2d()->zoom(zoom_out_proportion);
	}

	if(zoom.enabled){
		auto& cam = get_app2d()->get_camera();
		auto const mpos = get_app2d()->compute_space_point_of_cursor_position();
		auto const t = zoom.mpos_before - mpos;
		cam.translate(glm::vec3{ t, 0.f });
	}
}

void camera2d_controller::update_translation()
{
	auto const proj_vp = get_app()->compute_projected_viewport();
	m_speed = std::max(proj_vp.width, proj_vp.height);

	glm::vec3 translation = build_translation_vector();

	get_app()->get_camera().translate(translation);
}

void camera2d_controller::update()
{
	update_zoom();
	update_translation();
}


bool camera2d_controller::up() const
{
	return is_at_least_one_pressed(get_app2d(), up_keys);
}

bool camera2d_controller::down() const
{
	return is_at_least_one_pressed(get_app2d(), down_keys);
}

bool camera2d_controller::left() const
{
	return is_at_least_one_pressed(get_app2d(), left_keys);
}

bool camera2d_controller::right() const
{
	return is_at_least_one_pressed(get_app2d(), right_keys);
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
		auto const& np = get_app()->compute_projected_viewport();

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
