#pragma once

#include "component.hpp"

namespace ug::graphics{

class ortographic_zoom : public component{
public:
	static constexpr auto zoom_proportion	= .1f;
	static constexpr auto zoomin_proportion	= 1.f - zoom_proportion;
	static constexpr auto zoomout_proportion= 1.f + zoom_proportion;

	ortographic_zoom(app* app_ptr);

	virtual void on_scroll_input(graphics::scroll_input const& input)
		override;

	virtual void update() override;

protected:
	struct intentions_type {
		bool zoom_in = false;
		bool zoom_out = false;
	}intentions;

	struct scroll{
		bool in = false;
		bool out = false;
	}scroll;
};

} // end of namespace graphics
