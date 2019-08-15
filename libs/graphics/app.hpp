#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <tuple>

#include "graphics/misc.hpp"
#include "graphics/component-manager.hpp"

namespace graphics{

using window_type = GLFWwindow;
using window_ptr = std::shared_ptr<window_type>;

class app {
public:
	explicit app(int32_t width, int32_t height, char const* window_title);

	virtual ~app();

	window_ptr window();
	window_ptr window() const;

	bool should_close() const;
	void should_close(bool) const;

	std::tuple<int32_t, int32_t> get_framebuffer_size() const;

	std::tuple<double, double> get_cursor_position() const;
	glm::vec2 get_cursor_vector() const;

	void clear() const;

	void set_clear_color(
		float r,
		float g,
		float b,
		float a
	) const;

	void add_component(std::shared_ptr<component> ptr);

	void poll_events() const;
	double get_time() const;
	double get_delta() const;
	void swap_buffers() const;
	bool is_key_pressed(int32_t key) const;

	rect2d const& get_viewport() const;
	void set_viewport(rect2d const& r);

	virtual void on_key_input(key_input const& input);
	virtual void on_scroll_input(scroll_input const& input);

	virtual int32_t run();

	void update_cached_data();
	void update_components() const;
	void update_all();

	virtual void update();
	virtual void draw();

	void draw_components() const;
	void draw_all();

	void on_key_input_components(key_input const& input);
	void on_scroll_input_components(scroll_input const& input);
protected:

	/**
	  * context is a glfw context wrapper, the constructor will run
	  * initialization code and must be the first member of this class
	  */
	struct context{
		context();
		~context();
	} m_context;

	window_ptr m_window;
	component_manager m_component_manager;
	rect2d m_viewport;

	/**
	  * this struct holds all cached data for one loop
	  */
	struct{
		double time = 0.0;
		double delta = 0.0;

		struct{
			int32_t width = 0;
			int32_t height = 0;
		} framebuffer_size;

		struct{
			double x = 0.0;
			double y = 0.0;
		} cursor_position;

	} m_cached_data;
};

}
