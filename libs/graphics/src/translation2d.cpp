#include "ug/graphics/app.hpp"
#include "ug/graphics/translation2d.hpp"

namespace graphics{

translation2d::translation2d(app* app_ptr)
	: component(app_ptr)
{}

template<class T>
static bool is_at_least_one_pressed(app const* a, T const& array)
{
	for(auto&& k : array)
		if(a->is_key_pressed(k))
			return true;
	return false;
}

bool translation2d::up() const
{
	return is_at_least_one_pressed(get_app(), up_keys);
}

bool translation2d::down() const
{
	return is_at_least_one_pressed(get_app(), down_keys);
}

bool translation2d::left() const
{
	return is_at_least_one_pressed(get_app(), left_keys);
}

bool translation2d::right() const
{
	return is_at_least_one_pressed(get_app(), right_keys);
}

glm::vec3 translation2d::build_translation_vector() const
{
	/*
	 * button translation
	 * ==================
	 */
	glm::vec3 button_translation{ .0f, .0f, .0f };
	if(up())	button_translation.y -= 1.f;
	if(down())	button_translation.y += 1.f;
	if(right())	button_translation.x -= 1.f;
	if(left())	button_translation.x += 1.f;


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
		auto const& np = get_app()->get_near_plane();

		auto s = glm::vec2(np.width/vp.width, -1.f*np.height/vp.height);
		mouse_translation = glm::vec3(mov*s, 0.f);
		last_cursor_position = mv;
	}else{
		last_cursor_position = mv;
	}

	return button_translation + mouse_translation;
}

void translation2d::update()
{
	m_speed = std::max(
		this->get_app()->get_near_plane().width,
		this->get_app()->get_near_plane().height
	);

	glm::vec3 translation = build_translation_vector();

	get_app()->set_view_matrix(glm::translate(
		get_app()->view_matrix(),
		translation
	));

}

} // end of namespace graphics
