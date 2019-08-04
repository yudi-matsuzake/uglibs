#pragma once

#include <iostream>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <tuple>

#include "graphics/misc.hpp"

namespace graphics{

using window_type = GLFWwindow;
using window_ptr = std::unique_ptr<window_type, decltype(&glfwDestroyWindow)>;

struct key_input{
	int key;
	int scancode;
	int action;
	int mods;
};

struct scroll_input{
	double x_offset, y_offset;
};

class app{
public:
	explicit app(int32_t width, int32_t height, char const* window_title);

	virtual ~app();

	window_type* window();
	window_type* window() const;

	bool should_close() const;
	void should_close(bool) const;

	std::tuple<int32_t, int32_t> get_framebuffer_size() const;

	std::tuple<float, float> get_cursor_position() const;
	glm::vec2 get_cursor_vector() const;

	void clear() const;

	void set_clear_color(
		float r,
		float g,
		float b,
		float a
	) const;

	void poll_events() const;

	float get_time() const;

	void swap_buffers() const;

	void set_viewport(rect2d const& r) const;

	bool is_key_pressed(int32_t key) const;

	virtual void on_key_input(key_input const& input);
	virtual void on_scroll_input(scroll_input const& input);

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
