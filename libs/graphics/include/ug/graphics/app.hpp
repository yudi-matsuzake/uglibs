#pragma once

#include <filesystem>
#include <vector>
#include <iostream>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <tuple>

#include <opencv2/core/core.hpp>

#include "ug/graphics/misc.hpp"
#include "ug/graphics/window.hpp"
#include "ug/graphics/component-manager.hpp"
#include "ug/graphics/camera.hpp"

namespace ug::graphics{

using camera_ptr = std::unique_ptr<camera>;

class app : public window {
public:
	explicit app(
		int32_t width,
		int32_t height,
		char const* window_title
	);

	virtual ~app();

	void clear() const;

	void set_clear_color(graphics::color const& clear) const;
	void set_clear_color(
		float r,
		float g,
		float b,
		float a
	) const;

	ug::graphics::camera const* camera() const;
	ug::graphics::camera* camera();

	void add_component(std::shared_ptr<component> ptr);

	rect2d const& get_viewport() const;
	void set_viewport(rect2d const& r);

	double get_delta() const;

	virtual void on_drop_path(path_input const& path) override;
	virtual void on_key_input(key_input const& input) override;
	virtual void on_scroll_input(scroll_input const& input) override;

	virtual int32_t run();

	void update_components() const;
	void update_all();

	virtual void update();
	virtual void draw();

	void draw_components() const;
	void draw_all();

	virtual void draw_ui();
	virtual void draw_components_ui();
	virtual void draw_all_ui();

	virtual bool is_key_pressed(int32_t key) const override;

	virtual bool is_mouse_button_pressed(int32_t mouse_button) const
		override;

	void on_key_input_components(key_input const& input);
	void on_scroll_input_components(scroll_input const& input);

	rect2d const& get_projected_viewport() const;
	glm::mat4 const& view_matrix() const;
	glm::mat4 const& projection_matrix() const;

	bool ui_want_capture_mouse() const;
	bool ui_want_capture_keyboard() const;

	cv::Mat get_current_frame_image() const;

protected:

	void set_viewport();
	void update_time();
	void update_cached_data();

	double m_last_time{ get_time() };
	double m_delta{ 0.0 };

	component_manager m_component_manager;
	rect2d m_viewport{ {0, 0}, 0, 0 };

	camera_ptr m_camera = nullptr;

	struct{
		rect2d projected_viewport{ {0.0, 0.0}, 0.0, 0.0};
		glm::mat4 projection_matrix{1.f};
		glm::mat4 view_matrix{1.f};
	}m_cached_data;
};

}
