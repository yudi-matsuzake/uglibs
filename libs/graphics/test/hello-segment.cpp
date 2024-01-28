#include <ug/graphics/app.hpp>
#include <ug/graphics/segment2d-render.hpp>

using namespace ug;

class segment_test : public graphics::app {
public:
	segment_test () :
		graphics::app(512, 512, "Hello, graphics"),
		segment_render(this)
	{}

	virtual void draw() override
	{
		static auto center = glm::vec2{256.f, 256.f};
		static auto dir = glm::vec2{ 256.f, 0.f };

		graphics::app::draw();

		static auto angle = 0.f;

		auto pif = static_cast<float>(M_PI);
		auto deltaf = static_cast<float>(get_delta());

		angle = fmodf(angle + pif/2.f*deltaf, 2*pif);

		auto rot_dir = glm::vec2{
			dir.x*cosf(angle) - dir.y*sinf(angle),
			dir.x*sinf(angle) + dir.y*cosf(angle)
		};

		auto timef = static_cast<float>(get_time());

		segment_render.set_color({
			sinf(timef),
			1.f - sinf(timef),
			cosf(timef),
			1.f
		});

		segment_render(graphics::segment2d{
			center, 
			center + rot_dir
		});
	}

protected:
	graphics::segment2d_render segment_render;
};

int main()
{
	segment_test app;

	return app.run();
}
