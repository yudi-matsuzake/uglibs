#pragma once

#include <iostream>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <tuple>

// WARN: include glad.h before glfw3.h
#include "glad/glad.h"
#include <GLFW/glfw3.h>


namespace graphics{

using window_type = GLFWwindow;
using window_ptr = std::unique_ptr<window_type, decltype(&glfwDestroyWindow)>;

struct key_input{
	int key;
	int scancode;
	int action;
	int mods;
};

class app{
public:
	explicit app(int32_t width, int32_t height, char const* window_title);

	virtual ~app();

	window_type* window();

	window_type* window() const;

	virtual void on_input(key_input k);

	bool should_close() const;
	void should_close(bool) const;

	std::tuple<int32_t, int32_t> get_framebuffer_size() const;

	void poll_events() const;

	float get_time() const;

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
