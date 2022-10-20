#pragma once

#include <filesystem>
#include <vector>
#include <iostream>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <tuple>

#include "imgui.h"

#include "ug/graphics/misc.hpp"
#include "ug/graphics/opengl-error.hpp"
#include "ug/graphics/inputs.hpp"

namespace ug::graphics{

using window_type = GLFWwindow;
using window_ptr = std::unique_ptr<window_type, decltype(&glfwDestroyWindow)>;

class window {
public:
	explicit window(
		int32_t width, int32_t height,
		char const* window_title
	);

	virtual ~window() = default;

	window_type* ptr();
	window_type* ptr() const;

	bool should_close() const;
	void should_close(bool) const;

	std::tuple<int32_t, int32_t> get_framebuffer_size() const;

	std::tuple<double, double> get_cursor_position() const;
	glm::vec2 get_cursor_vector() const;

	void poll_events() const;

	double get_time() const;

	virtual bool is_key_pressed(int32_t key) const;
	virtual bool is_mouse_button_pressed(int32_t mouse_button) const;

	virtual void on_drop_path(path_input const& path);
	virtual void on_key_input(key_input const& input);
	virtual void on_scroll_input(scroll_input const& input);
	virtual void on_mouse_button(mouse_button_input const& input);

	void swap_buffers() const;

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
};

}
