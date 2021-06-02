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

class app : public window {
public:

	enum class projection_type : int8_t {
		NONE,
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	explicit app(
		int32_t width,
		int32_t height,
		char const* window_title,
		app::projection_type proj_type = app::projection_type::ORTHOGRAPHIC
	);

	virtual ~app();

	void clear() const;
	void set_clear_flags(GLbitfield clear_flags);

	void set_clear_color(graphics::color4 const& clear) const;
	void set_clear_color(
		float r,
		float g,
		float b,
		float a
	) const;

	void add_component(std::shared_ptr<component> ptr);

	rect2d const& get_viewport() const;
	void set_viewport(rect2d const& r);

	ug::graphics::camera const& get_camera() const;
	ug::graphics::camera& get_camera();

	double get_delta() const;

	virtual void on_drop_path(path_input const& path) override;
	virtual void on_key_input(key_input const& input) override;
	virtual void on_scroll_input(scroll_input const& input) override;

	virtual int32_t run();

	virtual void update();
	virtual void draw();
	virtual void finally();

	void update_components() const;
	void draw_components() const;
	void finally_components() const;

	void update_all();
	void draw_all();
	void finally_all();

	virtual void draw_ui();
	virtual void draw_components_ui();
	virtual void draw_all_ui();

	virtual bool is_key_pressed(int32_t key) const override;

	virtual bool is_mouse_button_pressed(int32_t mouse_button) const
		override;

	void on_key_input_components(key_input const& input);
	void on_scroll_input_components(scroll_input const& input);

	rect2d compute_projected_viewport(
		std::optional<glm::mat4> const& pm = std::nullopt,
		std::optional<glm::mat4> const& vm = std::nullopt
	) const;

	glm::mat4 get_view_matrix() const;
	[[nodiscard]] glm::mat4 compute_projection_matrix() const;

	/**
	 * zoom
	 * @param	proportion proportion of the zoom,
	 *	to scale the viewport
	 *	i.e. zoom > 1. would be zoom in
	 *	i.e. zoom < 1. would be zoom out
	 **/
	void zoom(float proportion);
	float get_zoom_proportion() const;

	bool ui_want_capture_mouse() const;
	bool ui_want_capture_keyboard() const;

	cv::Mat get_current_frame_image() const;

protected:

	void set_viewport();
	void update_time();
	void update_cached_data();


	double m_last_time{ get_time() };
	double m_delta{ 0.0 };

	float m_zoom{ 1.0 };

	GLbitfield m_clear_flags = GL_COLOR_BUFFER_BIT;

	component_manager m_component_manager;
	rect2d m_viewport{ {0, 0}, 0, 0 };

	ug::graphics::camera m_camera;
public:
	app::projection_type projection_type = app::projection_type::ORTHOGRAPHIC;
};

}
