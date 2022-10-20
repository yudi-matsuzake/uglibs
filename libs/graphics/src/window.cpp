#include <sstream>

#include "ug/graphics/window.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ug::graphics{

/*
 * callbacks
 * =========
 */

static void error_callback(int code, const char *description)
{
	std::stringstream ss;

	ss	<< "Error ("
		<< code
		<< "): "
		<< description
		<< '\n';

	throw std::runtime_error(ss.str());
}

static void drop_callback(
	GLFWwindow* w,
	int count,
	char const** paths)
{
	window* a = static_cast<window*>(glfwGetWindowUserPointer(w));
	path_input p(paths, paths + count);
	a->on_drop_path(p);
}

static void key_callback(
	GLFWwindow* w,
	int key,
	int scancode,
	int action, int mods)
{

	window* a = static_cast<window*>(glfwGetWindowUserPointer(w));
	key_input input{ key, scancode, action, mods };
	a->on_key_input(input);
}

static void scroll_callback(
	GLFWwindow* w,
	double xoffset,
	double yoffset)
{
	window* a = static_cast<window*>(glfwGetWindowUserPointer(w));

	scroll_input input{ xoffset, yoffset };
	a->on_scroll_input(input);
}

static void mouse_button_callback(
	GLFWwindow* w,
	int button,
	int action,
	int mods)
{
	window* a = static_cast<window*>(glfwGetWindowUserPointer(w));
	a->on_mouse_button({button, action, mods});
}

/*
 * auxiliar functions
 * ==================
 */

static void terminate_and_throw(char const* msg)
{
	glfwTerminate();
	throw std::runtime_error(msg);
}

static window_ptr create_window(
	int32_t width,
	int32_t height,
	char const* window_title)
{

	glfwSetErrorCallback(error_callback);

	auto w = window_ptr(
		glfwCreateWindow(
			width,
			height,
			window_title,
			nullptr,
			nullptr),
		&glfwDestroyWindow
	);

	if(!w) terminate_and_throw("Could not create a glfw window");

	return w;
}

/*
 * context wrapper
 */
window::context::context()
{
	if (!glfwInit()){
		throw std::runtime_error(
			"Could not inicialize the glfw context"
		);
	}

	// opengl hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// some arbitrarily preferences
	/* glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); */
}

window::context::~context()
{
	glfwTerminate();
}

/*
 * window constructors and destructors
 * ================================
 */
window::window(int32_t width, int32_t height, char const* window_title)
	: m_window(create_window(width, height, window_title))
{
	// associate this aplication with the glfw window
	glfwSetWindowUserPointer(ptr(), this);
	glfwSetKeyCallback(ptr(), key_callback);
	glfwSetScrollCallback(ptr(), scroll_callback);
	glfwSetDropCallback(ptr(), drop_callback);
	glfwSetMouseButtonCallback(ptr(), mouse_button_callback);

	glfwMakeContextCurrent(ptr());

	glfwSwapInterval(1);
	glfwSetInputMode(ptr(), GLFW_STICKY_KEYS, GLFW_TRUE);

	if(!gladLoadGL())
		throw std::runtime_error("glad could not be loaded");

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

}

bool window::should_close() const
{
	return glfwWindowShouldClose(ptr());
}

void window::should_close(bool b) const
{
	glfwSetWindowShouldClose(ptr(), b);
}

std::tuple<int32_t, int32_t> window::get_framebuffer_size() const
{
	int width, height;
	glfwGetFramebufferSize(ptr(), &width, &height);

	return { width, height };
}

std::tuple<double, double> window::get_cursor_position() const
{
	double x, y;
	glfwGetCursorPos(ptr(), &x, &y);
	return {x, y};
}

glm::vec2 window::get_cursor_vector() const
{
	auto [ x, y ] = get_cursor_position();

	return { x, y };
}

window_type* window::ptr()
{
	return m_window.get();
}

window_type* window::ptr() const
{
	return m_window.get();
}

void window::poll_events() const
{
	glfwPollEvents();
}

double window::get_time() const
{
	return glfwGetTime();
}

void window::swap_buffers() const
{
	GL(glfwSwapBuffers(ptr()));
}

bool window::is_key_pressed(int32_t key) const
{
	return glfwGetKey(ptr(), key) == GLFW_PRESS;
}

bool window::is_mouse_button_pressed(int32_t mouse_button) const
{
	return glfwGetMouseButton(ptr(), mouse_button) == GLFW_PRESS;
}

void window::on_drop_path(path_input const&)
{}

void window::on_key_input(key_input const&)
{}

void window::on_scroll_input(scroll_input const&)
{}

void window::on_mouse_button(mouse_button_input const&)
{}

} // end of namespace ug::graphics
