#pragma once

#include <optional>
#include <array>

#include "ug/graphics/misc.hpp"
#include "ug/graphics/component.hpp"

namespace ug::graphics{

class app;

/**
  * this class is used when a user can control a 3d camera with translation and
  * rotation
  */
class camera3d_controller : public component{
public:

	static constexpr struct keymap_type {
		static constexpr auto up = std::array{
			GLFW_KEY_UP
		};

		static constexpr auto down = std::array{
			GLFW_KEY_DOWN
		};

		static constexpr auto left = std::array {
			GLFW_KEY_LEFT,
			GLFW_KEY_A
		};

		static constexpr auto right = std::array {
			GLFW_KEY_RIGHT,
			GLFW_KEY_D
		};

		static constexpr auto forward = std::array {
			GLFW_KEY_W
		};

		static constexpr auto backward = std::array {
			GLFW_KEY_S
		};

	} keymap = {};

	camera3d_controller(app*);
	virtual ~camera3d_controller() = default;

	virtual void update() override;
	virtual void on_scroll_input(scroll_input const& input) override;

	struct intentions_type {
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		bool forward = false;
		bool backward = false;
	};

	struct scroll_type {
		float y_offset = 0.0;
	} m_scroll;

	enum class mouse_mode : int8_t {
		NONE,
		TRANSLATION,
		ROTATION
	};

	struct mouse_type{
		mouse_mode mode = mouse_mode::NONE;
		glm::vec2 last = glm::vec2{ 0.f, 0.f };
		glm::vec2 t = glm::vec2{ 0.f, 0.f };
	} m_mouse = {};

private:
	// speed in pixels per second
	float m_speed = 10.f;

	mouse_type process_mouse_input() const;
	glm::vec3 build_translation_vector(intentions_type const&) const;

};

} // end of namespace ug::graphics
