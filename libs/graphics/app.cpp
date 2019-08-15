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

	key_input input{ key, scancode, action, mods };
	a->on_key_input(input);
	a->on_key_input_components(input);
}

static void scroll_callback(
	GLFWwindow* window,
	double xoffset,
	double yoffset)
{
	app* a = static_cast<app*>(glfwGetWindowUserPointer(window));

	scroll_input input{ xoffset, yoffset };
	a->on_scroll_input(input);
	a->on_scroll_input_components(input);
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
		glfwDestroyWindow
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

	// opengl hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// some arbitrarily preferences
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

app::context::~context()
{
	glfwTerminate();
}

app::app(int32_t width, int32_t height, char const* window_title)
	: m_window(create_window(width, height, window_title)),
	  m_viewport{
		{0.0, 0.0},
		static_cast<float>(width),
		static_cast<float>(height)
	  }
{
	// associate this aplication with the glfw window
	glfwSetWindowUserPointer(window().get(), this);
	glfwSetKeyCallback(window().get(), key_callback);
	glfwSetScrollCallback(window().get(), scroll_callback);

	glfwMakeContextCurrent(window().get());

	if(!gladLoadGL())
		throw std::runtime_error("glad could not be loaded");

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSwapInterval(1);

	glfwSetInputMode(window().get(), GLFW_STICKY_KEYS, GLFW_TRUE);

	GL(glEnable(GL_BLEND));
	GL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
}

app::~app()
{}

bool app::should_close() const
{
	return glfwWindowShouldClose(window().get());
}

void app::should_close(bool b) const
{
	glfwSetWindowShouldClose(window().get(), b);
}

std::tuple<int32_t, int32_t> app::get_framebuffer_size() const
{
	int width, height;
	glfwGetFramebufferSize(window().get(), &width, &height);

	return { width, height };
}

std::tuple<double, double> app::get_cursor_position() const
{
	return {
		m_cached_data.cursor_position.x,
		m_cached_data.cursor_position.y
	};
}

glm::vec2 app::get_cursor_vector() const
{
	auto [ x, y ] = get_cursor_position();

	return { x, y };
}

window_ptr app::window()
{
	return m_window;
}

window_ptr app::window() const
{
	return m_window;
}

void app::clear() const
{
	GL(glClear(GL_COLOR_BUFFER_BIT));
}

void app::set_clear_color(
	float r,
	float g,
	float b,
	float a) const
{
	GL(glClearColor(r, g, b, a));
}

void app::add_component(std::shared_ptr<component> ptr)
{
	m_component_manager.add_component(ptr);
}

void app::poll_events() const
{
	glfwPollEvents();
}

double app::get_time() const
{
	return m_cached_data.time;
}

double app::get_delta() const
{
	return m_cached_data.delta;
}

void app::swap_buffers() const
{
	GL(glfwSwapBuffers(window().get()));
}

rect2d const& app::get_viewport() const
{
	return m_viewport;
}

void app::set_viewport(rect2d const& r)
{
	auto to_int = [](auto x){ return static_cast<int32_t>(x); };

	m_viewport = r;

	GL(glViewport(
		to_int(r.position.x),
		to_int(r.position.y),
		to_int(r.width),
		to_int(r.height)
	));
}

bool app::is_key_pressed(int32_t key) const
{
	return glfwGetKey(window().get(), key);
}

void app::on_key_input(key_input const&)
{}

void app::on_scroll_input(scroll_input const&)
{}

void app::update()
{}

void app::update_components() const
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->update();
	}
}

void app::update_all()
{
	update_cached_data();
	update();
	update_components();
}

void app::draw()
{}

void app::draw_components() const
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->draw();
	}
}

void app::draw_all()
{
	draw();
	draw_components();
}

int app::run()
{
	while(!should_close()){
		update_all();

		set_viewport(m_viewport);
		clear();

		draw_all();

		swap_buffers();
		poll_events();
	}

	return EXIT_SUCCESS;
}

void app::update_cached_data()
{
	auto now = glfwGetTime();
	m_cached_data.delta = now - m_cached_data.time;
	m_cached_data.time = now;

	glfwGetFramebufferSize(
		window().get(),
		&m_cached_data.framebuffer_size.width,
		&m_cached_data.framebuffer_size.height
	);

	glfwGetCursorPos(
		window().get(),
		&m_cached_data.cursor_position.x,
		&m_cached_data.cursor_position.y
	);
}

void app::on_key_input_components(key_input const& input)
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->on_key_input(input);
	}
}

void app::on_scroll_input_components(scroll_input const& input)
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->on_scroll_input(input);
	}
}

}
