#include "ug/graphics/camera3d-controller.hpp"
#include "ug/graphics/app.hpp"

namespace ug::graphics{

template<class T>
static bool is_at_least_one_pressed(ug::graphics::app const* a, T const& array)
{
	for(auto&& k : array)
		if(a->is_key_pressed(k))
			return true;
	return false;
}

camera3d_controller::camera3d_controller(app* app)
	: component(app)
{}

camera3d_controller::mouse_type camera3d_controller::process_mouse_input() const
{
	auto const m = get_app()->get_cursor_vector();
	if(get_app()->is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)){
		switch(m_mouse.mode){
			case mouse_mode::NONE:
			case mouse_mode::ROTATION:
				return mouse_type{ mouse_mode::TRANSLATION, m };
			case mouse_mode::TRANSLATION:
				return mouse_type{
					mouse_mode::TRANSLATION,
					m,
					m - m_mouse.last
				};
		}
	}else if(get_app()->is_mouse_button_pressed(GLFW_MOUSE_BUTTON_MIDDLE)){
		switch(m_mouse.mode){
			case mouse_mode::NONE:
			case mouse_mode::TRANSLATION:
				return mouse_type{ mouse_mode::ROTATION, m };
			case mouse_mode::ROTATION:
				return mouse_type{
					mouse_mode::ROTATION,
					m,
					m - m_mouse.last
				};
		}
	}
	return mouse_type{};
}

void camera3d_controller::update()
{
	auto const intentions = intentions_type{
		.up = is_at_least_one_pressed(get_app(), keymap.up),
		.down = is_at_least_one_pressed(get_app(), keymap.down),
		.left = is_at_least_one_pressed(get_app(), keymap.left),
		.right = is_at_least_one_pressed(get_app(), keymap.right),
		.forward = is_at_least_one_pressed(get_app(), keymap.forward)
			|| (m_scroll.y_offset > 0.f),
		.backward = is_at_least_one_pressed(get_app(), keymap.backward)
			|| (m_scroll.y_offset < 0.f)
	};

	auto const v = build_translation_vector(intentions);
	get_app()->get_camera().translate(v);

	m_scroll = scroll_type{};
	m_mouse = process_mouse_input();
}

void camera3d_controller::on_scroll_input(scroll_input const& input)
{
	if(!get_app()->ui_want_capture_mouse())
		m_scroll = { static_cast<float>(input.y_offset) };
}

static glm::vec3 make_button_translation(
	ug::graphics::app const* app,
	camera3d_controller::intentions_type const& intetions,
	float speed)
{
	glm::vec3 button_translation{ .0f, .0f, .0f };
	if(intetions.up)	button_translation.y += 1.f;
	if(intetions.down)	button_translation.y -= 1.f;
	if(intetions.left)	button_translation.x -= 1.f;
	if(intetions.right)	button_translation.x += 1.f;
	if(intetions.forward)	button_translation.z -= 1.f;
	if(intetions.backward)	button_translation.z += 1.f;

	auto l = glm::length(button_translation);
	if(l > 0.f){
		auto fd = static_cast<float>(app->get_delta());
		auto length = speed * fd;
		button_translation = glm::normalize(button_translation)*length;
	}

	return button_translation;
}

glm::vec3 camera3d_controller::build_translation_vector(
	camera3d_controller::intentions_type const& intetions) const
{
	auto const button_t = make_button_translation(
		get_app(),
		intetions, m_speed
	);

	auto const mouse_t = glm::vec3{
		(m_mouse.mode == mouse_mode::TRANSLATION)
		? (m_mouse.t * glm::vec2{ -1.f, 1.f })
		: glm::vec2(0.f),
		0.f
	} * static_cast<float>(get_app()->get_delta());

	return button_t + mouse_t;
}

} // end of namespace ug::graphics
