#include <array>

#include "ug/graphics/misc.hpp"

#include "component.hpp"

namespace ug::graphics{

class translation2d : public component{
public:

	static constexpr std::array up_keys{
		GLFW_KEY_UP,
		GLFW_KEY_W
	};

	static constexpr std::array down_keys{
		GLFW_KEY_DOWN,
		GLFW_KEY_S
	};

	static constexpr std::array left_keys{
		GLFW_KEY_LEFT,
		GLFW_KEY_A
	};

	static constexpr std::array right_keys{
		GLFW_KEY_RIGHT,
		GLFW_KEY_D
	};

	explicit translation2d(app* app_ptr);
	virtual ~translation2d() = default;

	virtual void update() override;

protected:
	struct intentions_type {
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
	}m_intentions;

	// speed in pixels per second
	float m_speed = 100.f;

	glm::vec3 build_translation_vector() const;

	bool up() const;
	bool down() const;
	bool left() const;
	bool right() const;

};

} // end of namespace graphics
