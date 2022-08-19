#pragma once

#include <array>

#include "ug/graphics/misc.hpp"
#include "ug/graphics/component.hpp"

namespace ug::graphics{

class app2d;

/**
  * this class is used when a user can control a 2d camera with translation and
  * zoom
  */
class camera2d_controller : public component{
public:

	static constexpr auto zoom_proportion		= .1f;
	static constexpr auto zoom_in_proportion	= 1.f + zoom_proportion;
	static constexpr auto zoom_out_proportion	= 1.f - zoom_proportion;

	static constexpr auto up_keys = std::array{
		GLFW_KEY_UP,
		GLFW_KEY_W
	};

	static constexpr auto down_keys = std::array{
		GLFW_KEY_DOWN,
		GLFW_KEY_S
	};

	static constexpr auto left_keys = std::array {
		GLFW_KEY_LEFT,
		GLFW_KEY_A
	};

	static constexpr auto right_keys = std::array {
		GLFW_KEY_RIGHT,
		GLFW_KEY_D
	};

	static constexpr auto zoom_in_keys = std::array{
		GLFW_KEY_EQUAL
	};

	static constexpr auto zoom_out_keys = std::array{
		GLFW_KEY_MINUS
	};

	camera2d_controller(app2d* app_ptr);
	virtual ~camera2d_controller() = default;

	app2d const* get_app2d() const;
	app2d* get_app2d();

	virtual void on_scroll_input(graphics::scroll_input const& input)
		override;

	virtual void update() override;

protected:
	struct intentions_type {
		bool zoom_in = false;
		bool zoom_out = false;

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
	}intentions;

	struct scroll{
		bool in = false;
		bool out = false;
	}scroll;

	// speed in pixels per second
	float m_speed = 100.f;

	void update_zoom();
	void update_translation();

	glm::vec3 build_translation_vector() const;

	bool up() const;
	bool down() const;
	bool left() const;
	bool right() const;

	ug::graphics::app2d* m_app2d = nullptr;
};

} // end of namespace ug::graphics
