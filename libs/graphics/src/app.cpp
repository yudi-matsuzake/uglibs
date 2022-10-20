#include <sstream>
#include <stdexcept>

#include "ug/graphics/app.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ug::graphics{

static void framebuffer_size_callback(GLFWwindow* w, int width, int height)
{
	GL(glViewport(0, 0, width, height));
	app* a = static_cast<app*>(glfwGetWindowUserPointer(w));
	auto vp = a->get_viewport();
	vp.width = static_cast<float>(width);
	vp.height = static_cast<float>(height);
	a->set_viewport(vp);
}

/*
 * app constructors and destructors
 * ================================
 */
app::app(
	int32_t width, int32_t height,
	char const* window_title,
	enum ug::graphics::app::projection_type proj_type)

	: window(width, height, window_title),
	  m_viewport{
			{0.0, 0.0},
			static_cast<float>(width),
			static_cast<float>(height)
	  },
		projection_type(proj_type)
{
	glfwSetFramebufferSizeCallback(this->ptr(), framebuffer_size_callback);

	if(!gladLoadGL())
		throw std::runtime_error("glad could not be loaded");

	if(!gladLoadGLLoader(
		reinterpret_cast<GLADloadproc>(glfwGetProcAddress))){
		throw std::runtime_error("Failed to initialize Opengl context");
	}

	GL(glEnable(GL_BLEND));
	GL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));


	/*
	 * imgui context
	 * =============
	 */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |=
		ImGuiConfigFlags_NavEnableKeyboard
		| ImGuiConfigFlags_NavEnableGamepad;
	// style
	ImGui::StyleColorsDark();
	// render
	ImGui_ImplGlfw_InitForOpenGL(ptr(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

app::~app()
{

	/*
	 * imgui context destruction
	 * =========================
	 */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void app::clear() const
{
	GL(glClear(m_clear_flags));
}

void app::set_clear_flags(GLbitfield clear_flags)
{
	m_clear_flags = clear_flags;
}

void app::set_clear_color(
	float r,
	float g,
	float b,
	float a) const
{
	GL(glClearColor(r, g, b, a));
}

void app::set_clear_color(graphics::color4 const& clear) const
{
	set_clear_color(clear.r, clear.g, clear.b, clear.a);
}

void app::add_component(std::shared_ptr<component> ptr)
{
	m_component_manager.add_component(ptr);
}

rect2d const& app::get_viewport() const
{
	return m_viewport;
}

void app::set_viewport(rect2d const& r)
{
	m_viewport = r;
}

void app::set_viewport()
{
	auto to_int = [](auto x){ return static_cast<int32_t>(x); };

	GL(glViewport(
		to_int(m_viewport.position.x),
		to_int(m_viewport.position.y),
		to_int(m_viewport.width),
		to_int(m_viewport.height)
	));
}

ug::graphics::camera const& app::get_camera() const
{
	return m_camera;
}

ug::graphics::camera& app::get_camera()
{
	return m_camera;
}

void app::update_time()
{
	auto now = get_time();
	m_delta = now - m_last_time;
	m_last_time = now;
}

double app::get_delta() const
{
	return m_delta;
}

bool app::is_key_pressed(int32_t key) const
{
	return !ui_want_capture_keyboard() &&
		window::is_key_pressed(key);
}

bool app::is_mouse_button_pressed(int32_t mouse_button) const
{
	return !ui_want_capture_keyboard()
		&& window::is_mouse_button_pressed(mouse_button);
}

void app::on_drop_path(path_input const& input)
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->on_drop_path(input);
	}
}

void app::on_key_input(key_input const& input)
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->on_key_input(input);
	}
}

void app::on_scroll_input(scroll_input const& input)
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->on_scroll_input(input);
	}
}

void app::update()
{}

void app::finally()
{}

void app::update_components() const
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->update();
	}
}

void app::finally_components() const
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->finally();
	}
}

void app::update_all()
{
	update();
	update_components();
}

void app::finally_all()
{
	finally();
	finally_components();
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

void app::draw_ui()
{}

void app::draw_components_ui()
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->draw_ui();
	}
}

void app::draw_all_ui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	draw_ui();
	draw_components_ui();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int app::run()
{
	while(!should_close()){
		update_time();
		update_all();

		set_viewport();
		clear();

		draw_all();

		draw_all_ui();

		swap_buffers();
		poll_events();

		finally_all();
	}

	return EXIT_SUCCESS;
}

[[nodiscard]] glm::mat4 app::compute_projection_matrix() const
{
	auto const [ sw, sh ] = this->get_framebuffer_size();
	auto const screen_width = static_cast<float>(sw);
	auto const screen_height = static_cast<float>(sh);

	auto vp = get_viewport();

	vp *= 1.f / m_zoom;

	if(projection_type == projection_type::ORTHOGRAPHIC){
		return glm::ortho(0.f, vp.width, 0.f, vp.height, -1.f, 1.f);
	}else if(projection_type == projection_type::PERSPECTIVE){
		return glm::perspective(
			glm::radians(45.f),
			screen_width / screen_height,
			.1f, 100.f
		);
	}
	throw std::runtime_error("Unknown projection type");
}

void app::zoom(float proportion)
{
	if(proportion <= .0f){
		throw std::runtime_error(
			"zoom proportion cannot be equal or smaller than zero"
		);
	}

	m_zoom *= proportion;
}

float app::get_zoom_proportion() const
{
	return m_zoom;
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

rect2d app::compute_projected_viewport(
	std::optional<glm::mat4> const& pm,
	std::optional<glm::mat4> const& vm) const
{
	auto const p = pm.has_value() ? pm.value() : compute_projection_matrix();
	auto const v = vm.has_value() ? vm.value() : get_view_matrix();

	glm::mat4 vp = glm::inverse(p*v);

	auto ul = vp*glm::vec4{ -1.f,  1.f, 0.f, 1.f };
	auto dr = vp*glm::vec4{  1.f, -1.f, 0.f, 1.f };

	return rect2d{
		{ ul.x, ul.y },
		dr.x - ul.x,
		ul.y - dr.y 
	};
}

glm::mat4 app::get_view_matrix() const
{
	return m_camera.view();
}

bool app::ui_want_capture_mouse() const
{
	return ImGui::GetIO().WantCaptureMouse;
}

bool app::ui_want_capture_keyboard() const
{
	return ImGui::GetIO().WantCaptureKeyboard;
}

cv::Mat app::get_current_frame_image() const
{
	auto& vp = get_viewport();

	auto w = static_cast<int>(vp.width);
	auto h = static_cast<int>(vp.height);

	static cv::Mat img;
	img.create(h, w, CV_8UC4);

	//use fast 4-byte alignment (default anyway) if possible
	GL(glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4));

	//set length of one complete row in destination data (doesn't need to equal img.cols)
	GL(glPixelStorei(
		GL_PACK_ROW_LENGTH,
		static_cast<int32_t>(img.step/img.elemSize())
	));

	GL(glReadPixels(
		0, 0,
		img.cols, img.rows,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		img.data
	));

	cv::Mat flipped(img.size(), img.type());
	cv::flip(img, flipped, 0);
	return flipped;
}


}
