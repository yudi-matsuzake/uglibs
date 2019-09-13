#include <sstream>

#include "ug/graphics/app.hpp"

namespace ug::graphics{

/*
 * app constructors and destructors
 * ================================
 */
app::app(int32_t width, int32_t height, char const* window_title)
	: window(width, height, window_title),
	  m_viewport{
		{0.0, 0.0},
		static_cast<float>(width),
		static_cast<float>(height)
	  },
	  m_near_plane{ m_viewport }
{
	if(!gladLoadGL())
		throw std::runtime_error("glad could not be loaded");

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	GL(glEnable(GL_BLEND));
	GL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

	m_projection_matrix = glm::ortho(
		m_near_plane.position.x,
		m_near_plane.position.x + m_near_plane.width,
		m_near_plane.position.y,
		m_near_plane.position.y + m_near_plane.height,
		1.f, -1.f
	);

	/*
	 * imgui context
	 * =============
	 */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |=
		ImGuiConfigFlags_NavEnableKeyboard;
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

void app::set_clear_color(graphics::color const& clear) const
{
	set_clear_color(clear.r, clear.g, clear.b, clear.a);
}

glm::mat4 const& app::projection_matrix() const
{
	return m_projection_matrix;
}

void app::set_projection_matrix(glm::mat4 const& m)
{
	m_projection_matrix = m;
	update_projected_viewport();
}

glm::mat4 const& app::view_matrix() const
{
	return m_view_matrix;
}

void app::set_view_matrix(glm::mat4 const& m)
{
	m_view_matrix = m;
	update_projected_viewport();
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

rect2d const& app::get_near_plane() const
{
	return m_near_plane;
}

void app::set_near_plane(rect2d const& r)
{
	m_near_plane = r;
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

void app::update_components() const
{
	for(auto&& [ id, ptr ] : m_component_manager){
		if(auto c = ptr.lock())
			c->update();
	}
}

void app::update_all()
{

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
	}

	return EXIT_SUCCESS;
}

void app::update_projected_viewport()
{
	glm::mat4 vp = glm::inverse(projection_matrix()*view_matrix());

	auto ul = vp*glm::vec4{ -1.f,  1.f, 0.f, 1.f };
	auto dr = vp*glm::vec4{  1.f, -1.f, 0.f, 1.f };

	m_projected_viewport = rect2d{
		{ ul.x, ul.y },
		dr.x - ul.x,
		ul.y - dr.y 
	};
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

rect2d const& app::get_projected_viewport() const
{
	return m_projected_viewport;
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
	img.create(h, w, CV_8UC3);

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
		GL_BGR,
		GL_UNSIGNED_BYTE,
		img.data
	));

	cv::Mat flipped(img.size(), img.type());
	cv::flip(img, flipped, 0);
	return flipped;
}


}
