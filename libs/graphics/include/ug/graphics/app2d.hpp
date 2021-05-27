#pragma once

#include "ug/graphics/app.hpp"
#include "ug/graphics/camera2d-controller.hpp"
#include "ug/graphics/grid2d-render.hpp"

namespace ug::graphics{

class app2d : public app{
public:
	static constexpr auto default_width = 1280;
	static constexpr auto default_height = 720;
	static constexpr auto default_fwidth =  static_cast<float>(default_width);
	static constexpr auto default_fheight = static_cast<float>(default_height);

	static constexpr std::array exit_keys{
		GLFW_KEY_ESCAPE,
		GLFW_KEY_Q
	};

	app2d(int width, int height, char const* title);
	app2d(char const* title);

	virtual ~app2d() = default;

	virtual void on_key_input(ug::graphics::key_input const& k) override;

	virtual void update() override;

	virtual void draw() override;
	virtual void draw_ui() override;

	std::tuple<int32_t, int32_t> compute_cell_index_of_cursor_position(
			std::optional<glm::vec2> const& space_point_cursor = std::nullopt
	);

	glm::vec2 compute_space_point_of_cursor_position() const;

	constexpr bool is_exit_key(int key) const;

protected:
	ug::graphics::grid2d_render m_grid;

	std::shared_ptr<ug::graphics::camera2d_controller> m_camera_controller
		= std::make_shared<ug::graphics::camera2d_controller>(this);

	bool m_show_grid = true;

	// TODO: background with alpha value makes any sense at all?
	ug::graphics::color4 m_background_color{.0f, .0f, .0f, 1.0f};
	ug::graphics::color4 m_grid_color{.5f, .5f, .5f, 1.0f};

};

} // end of namespace ug::graphics
