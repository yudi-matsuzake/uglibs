#include "ug/graphics/app2d.hpp"

namespace ug::graphics{

app2d::app2d(int width, int height, char const* title)
	: app(width, height, title),
	  m_grid(this)
{
	add_component(m_camera_controller);
	m_grid.set_grid_color({ 1.0, 1.0, 1.0, 0.25 });
}

app2d::app2d(char const* title)
	: app2d(default_width, default_height, title)
{}

void app2d::on_key_input(ug::graphics::key_input const& k)
{
	app::on_key_input(k);
	if(k.action == GLFW_RELEASE && is_exit_key(k.key))
		this->should_close(true);
}

glm::vec2 app2d::compute_space_point_of_cursor_position() const
{
	auto mv = get_cursor_vector();

	auto [ w, h ] = get_framebuffer_size();

	mv.x /= static_cast<float>(w);
	mv.y /= static_cast<float>(h);

	auto const np = compute_projected_viewport();

	mv.x = np.left() + np.width*mv.x;
	mv.y = np.top() - np.height*mv.y;

	return mv;
}

void app2d::update()
{
	app::update();
	this->set_clear_color(m_background_color);
	m_grid.set_grid_color(m_grid_color);
}

void app2d::draw_ui()
{
	app::draw_ui();

	if(auto view = this->ui_window_view("app2d options")){
		auto fr = static_cast<double>(ImGui::GetIO().Framerate);
		ImGui::Text("%3.2f FPS		", fr);

		auto [ cx, cy ] = compute_cell_index_of_cursor_position();

		ImGui::Text("cell index: (%5d, %5d)		", cx, cy);

		ImGui::Checkbox("show grid		", &m_show_grid);

		ImGui::ColorEdit3(
			"background color		",
			glm::value_ptr(m_background_color),
			ImGuiColorEditFlags_NoInputs
		);

		ImGui::ColorEdit3(
			"grid color",
			glm::value_ptr(m_grid_color),
			ImGuiColorEditFlags_NoInputs
		);
	}
}

void app2d::draw()
{
	app::draw();
	if(m_show_grid)
		m_grid();
}

std::tuple<int32_t, int32_t> app2d::compute_cell_index_of_cursor_position(
	std::optional<glm::vec2> const& space_point_cursor)
{
	auto to_cell_index = [](auto x)
	{
		return static_cast<int>(
			(x <= .0f) ? std::floor(x) : std::ceil(x)
		);
	};

	auto const mv = space_point_cursor.has_value()
		? space_point_cursor.value()
		: compute_space_point_of_cursor_position();

	return { to_cell_index(mv.x), to_cell_index(mv.y) };
}

constexpr bool app2d::is_exit_key(int key) const
{
	for(auto&& k : exit_keys)
		if(k == key)
			return true;

	return false;
}

} // end of namespace ug::graphics
