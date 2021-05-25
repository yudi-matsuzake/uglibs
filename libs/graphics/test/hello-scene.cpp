#include <chrono>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <memory>

#include "ug/graphics/mesh3d-render.hpp"
#include "ug/graphics/scene.hpp"
#include "ug/graphics/inputs.hpp"
#include "ug/graphics/window.hpp"
#include "ug/graphics/buffers.hpp"
#include "ug/graphics/vao.hpp"
#include "ug/graphics/shader.hpp"
#include "ug/graphics/program.hpp"
#include "ug/graphics/misc.hpp"

namespace chrono = std::chrono;

auto make_cube_mesh()
{
	return std::make_unique<ug::graphics::mesh3d>(
		std::vector<ug::graphics::mesh3d_attributes_layout>{
			{ { -.5f, -.5f, -.5f }, { .0f,  .0f, -1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ {  .5f, -.5f, -.5f }, { .0f,  .0f, -1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ {  .5f,  .5f, -.5f }, { .0f,  .0f, -1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ {  .5f,  .5f, -.5f }, { .0f,  .0f, -1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ { -.5f,  .5f, -.5f }, { .0f,  .0f, -1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ { -.5f, -.5f, -.5f }, { .0f,  .0f, -1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ { -.5f, -.5f,  .5f }, { .0f,  .0f,  1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ {  .5f, -.5f,  .5f }, { .0f,  .0f,  1.0f }, { .5f, 1.f, .25f, 1.f } },
			{ {  .5f,  .5f,  .5f }, { .0f,  .0f,  1.0f }, { 1.f, .5f, .25f, 1.f } },
			{ {  .5f,  .5f,  .5f }, { .0f,  .0f,  1.0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f,  .5f,  .5f }, { .0f,  .0f,  1.0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f, -.5f,  .5f }, { .0f,  .0f,  1.0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f,  .5f,  .5f }, { -1.0f,  .0f, .0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f,  .5f, -.5f }, { -1.0f,  .0f, .0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f, -.5f, -.5f }, { -1.0f,  .0f, .0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f, -.5f, -.5f }, { -1.0f,  .0f, .0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f, -.5f,  .5f }, { -1.0f,  .0f, .0f }, { 1.f, .5f, .25f, 1.f } },
			{ { -.5f,  .5f,  .5f }, { -1.0f,  .0f, .0f }, { 1.f, .5f, .25f, 1.f } },
			{ {  .5f,  .5f,  .5f }, { 1.0f,  .0f,  .0f }, { 1.f, .5f, .25f, 1.f } },
			{ {  .5f,  .5f, -.5f }, { 1.0f,  .0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f, -.5f, -.5f }, { 1.0f,  .0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f, -.5f, -.5f }, { 1.0f,  .0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f, -.5f,  .5f }, { 1.0f,  .0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f,  .5f,  .5f }, { 1.0f,  .0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ { -.5f, -.5f, -.5f }, { .0f, -1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f, -.5f, -.5f }, { .0f, -1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f, -.5f,  .5f }, { .0f, -1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f, -.5f,  .5f }, { .0f, -1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ { -.5f, -.5f,  .5f }, { .0f, -1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ { -.5f, -.5f, -.5f }, { .0f, -1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ { -.5f,  .5f, -.5f }, { .0f,  1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f,  .5f, -.5f }, { .0f,  1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f,  .5f,  .5f }, { .0f,  1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ {  .5f,  .5f,  .5f }, { .0f,  1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ { -.5f,  .5f,  .5f }, { .0f,  1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } },
			{ { -.5f,  .5f, -.5f }, { .0f,  1.0f,  .0f }, { .5f, .25f, 1.f, 1.f } }
		},
		[]{
			std::vector<uint32_t> v(36);
			std::iota(v.begin(), v.end(), 0U);
			return v;
		}()
	);
}

class hello_scene : public ug::graphics::app{
private:

	struct input{
		struct{
			chrono::milliseconds cooldown{500};
			bool value = true;

			void set_status(bool v)
			{
				static auto last = chrono::steady_clock::now();
				static auto first = true;

				auto now = chrono::steady_clock::now();

				if(first){
					first = false;
					value = v;
					last = now;
				}else{
					auto const d = now - last;
					if(d >= cooldown){
						value = v;
						last = now;
					}
				}
			}
		}camera_mov;

		bool left	= false;
		bool right	= false;
		bool up		= false;
		bool down	= false;
		bool back	= false;
		bool front	= false;

		struct{
			bool left	= false;
			bool right	= false;
			bool up		= false;
			bool down	= false;
			bool back	= false;
			bool front	= false;
		}translate;
	};

	auto process_inputs(input const& last)
	{
		input in{};
		in.camera_mov = last.camera_mov;

		if(this->is_key_pressed(GLFW_KEY_ESCAPE))
			this->should_close(true);

		if(this->is_key_pressed(GLFW_KEY_C))
			in.camera_mov.set_status(!in.camera_mov.value);

		if(in.camera_mov.value){
			if(this->is_key_pressed(GLFW_KEY_LEFT))
				in.left = true;

			if(this->is_key_pressed(GLFW_KEY_RIGHT))
				in.right = true;

			if(this->is_key_pressed(GLFW_KEY_UP)){
				if(this->is_key_pressed(GLFW_KEY_LEFT_CONTROL))
					in.front = true;
				else
					in.up = true;
			}

			if(this->is_key_pressed(GLFW_KEY_DOWN)){
				if(this->is_key_pressed(GLFW_KEY_LEFT_CONTROL))
					in.back = true;
				else
					in.down = true;
			}
		}else{
			if(this->is_key_pressed(GLFW_KEY_LEFT))
				in.translate.left = true;

			if(this->is_key_pressed(GLFW_KEY_RIGHT))
				in.translate.right = true;

			if(this->is_key_pressed(GLFW_KEY_UP))
				in.translate.up = true;

			if(this->is_key_pressed(GLFW_KEY_DOWN))
				in.translate.down = true;

			if(this->is_key_pressed(GLFW_KEY_PERIOD))
				in.translate.front = true;

			if(this->is_key_pressed(GLFW_KEY_COMMA))
				in.translate.back = true;
		}

		return in;
	}

	void update_camera(input const& in)
	{
		// update camera
		glm::vec3 mov(0.f, 0.f, 0.f);
		if(in.left)
			mov += glm::vec3(-1.f, 0.f, 0.f);
		if(in.right)
			mov += glm::vec3(1.f, 0.f, 0.f);
		if(in.up)
			mov += glm::vec3(0.f, 1.f, 0.f);
		if(in.down)
			mov += glm::vec3(0.f, -1.f, 0.f);
		if(in.front)
			mov += glm::vec3(0.f, 0.f, -1.f);
		if(in.back)
			mov += glm::vec3(0.f, 0.f, 1.f);

		scene.camera.translate(5.f*static_cast<float>(this->get_delta())*mov);
	}

public:
	hello_scene()
		: ug::graphics::app(1280, 720, "Hello scene/mesh render")
	{
		scene.meshes.emplace_back(make_cube_mesh());

		glfwSetFramebufferSizeCallback(this->ptr(), framebuffer_size_callback);

		GL(glEnable(GL_DEPTH_TEST));
		GL(glEnable(GL_BLEND));
		GL(glViewport(0, 0, 1280, 720));
		GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void update() override
	{
		input in = process_inputs(in);
		update_camera(in);
	}

	void draw() override
	{
		GL(glClearColor(0.f, 0.f, 0.f, 1.f));
		GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		scene.meshes[0]->transform(
			glm::rotate(
				static_cast<float>(this->get_delta()*M_PI),
				glm::vec3{ 1.f, 1.f, 1.f }
			)
		);

		mesh_render(scene, *scene.meshes[0]);
	}

private:
	ug::graphics::scene scene{
		ug::graphics::camera(glm::vec3{ 0.f, 0.f, 6.f }),
		{},
		std::vector{
			ug::graphics::light3d{
				glm::vec3{ 1.f, 1.f, 1.f },
				ug::graphics::color3{ 1.f, 1.f, 1.f }
			}
		}
	};

	ug::graphics::mesh3d_render mesh_render = ug::graphics::mesh3d_render(this);

	static void framebuffer_size_callback(GLFWwindow* w, int width, int height)
	{
		GL(glViewport(0, 0, width, height));
		hello_scene* app = static_cast<hello_scene*>(glfwGetWindowUserPointer(w));
		auto vp = app->get_viewport();
		vp.width = static_cast<float>(width);
		vp.height = static_cast<float>(height);
		app->set_viewport(vp);
	}
};

int main(int const, char const* const* const)
{
	hello_scene app;
	return app.run();
}
