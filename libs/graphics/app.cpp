#include <sstream>

#include "graphics/app.hpp"

namespace graphics{

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
}

app::context::~context()
{
	glfwTerminate();
}

app::app(int32_t width, int32_t height, char const* window_title)
	: m_window(create_window(width, height, window_title))
{
	// associate this aplication with the glfw window
	glfwSetWindowUserPointer(m_window.get(), this);
	glfwSetKeyCallback(window(), key_callback);

	glfwMakeContextCurrent(window());

	if(!gladLoadGL())
		throw std::runtime_error("glad could not be loaded");

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSwapInterval(1);

}

app::~app()
{}


window_type* app::window()
{
	return m_window.get();
}

void app::on_input(key_input)
{}

}
