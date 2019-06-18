#include <sstream>

#include "graphics/app.hpp"

namespace graphics{

/*
 * auxiliar structs and class implementation
 */

void operator*=(rect2d& r, float scalar)
{

	r.height *= scalar;
	r.width *= scalar;
}

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

static void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action, int mods)
{
	app* a = static_cast<app*>(glfwGetWindowUserPointer(window));
	a->on_input(key_input{ key, scancode, action, mods });
}

static void scroll_callback(
	GLFWwindow* window,
	double xoffset,
	double yoffset)
{
	app* a = static_cast<app*>(glfwGetWindowUserPointer(window));
	a->on_input(scroll_input{ xoffset, yoffset });
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

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

/*
 * app constructors and destructors
 * ================================
 */
app::context::context()
{
	if (!glfwInit()){
		throw std::runtime_error(
			"Could not inicialize the glfw context"
		);
	}

	// some arbitrarily preferences
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

app::context::~context()
{
	glfwTerminate();
}

app::app(int32_t width, int32_t height, char const* window_title)
	: m_window(create_window(width, height, window_title))
{
	// associate this aplication with the glfw window
	glfwSetWindowUserPointer(window(), this);
	glfwSetKeyCallback(window(), key_callback);
	glfwSetScrollCallback(window(), scroll_callback);

	glfwMakeContextCurrent(window());

	if(!gladLoadGL())
		throw std::runtime_error("glad could not be loaded");

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSwapInterval(1);

	glfwSetInputMode(window(), GLFW_STICKY_KEYS, GLFW_TRUE);

	GL(glEnable(GL_BLEND));
	GL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
}

app::~app()
{}

bool app::should_close() const
{
	return glfwWindowShouldClose(window());
}

void app::should_close(bool b) const
{
	glfwSetWindowShouldClose(window(), b);
}

std::tuple<int32_t, int32_t> app::get_framebuffer_size() const
{
	int width, height;
	glfwGetFramebufferSize(window(), &width, &height);

	return { width, height };
}

std::tuple<float, float> app::get_cursor_position() const
{
	double x, y;
	glfwGetCursorPos(window(), &x, &y);

	return { x, y };
}

glm::vec2 app::get_cursor_vector() const
{
	auto [ x, y ] = get_cursor_position();

	return { x, y };
}

window_type* app::window()
{
	return m_window.get();
}

window_type* app::window() const
{
	return m_window.get();
}

void app::clear() const
{
	GL(glClear(GL_COLOR_BUFFER_BIT));
}

void app::poll_events() const
{
	glfwPollEvents();
}

float app::get_time() const
{
	return static_cast<float>(glfwGetTime());
}

void app::swap_buffers() const
{
	GL(glfwSwapBuffers(window()));
}

void app::set_viewport(rect2d const& r) const
{
	auto to_int = [](auto x){ return static_cast<int32_t>(x); };

	GL(glViewport(
		to_int(r.position.x),
		to_int(r.position.y),
		to_int(r.width),
		to_int(r.height)
	));
}

bool app::is_key_pressed(int32_t key) const
{
	return glfwGetKey(window(), key);
}

void app::on_input(key_input const&)
{}

void app::on_input(scroll_input const&)
{}

}
