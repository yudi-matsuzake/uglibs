#pragma once

#include <cstdint>
#include <utility>

namespace ia{

enum class round_mode : int32_t {
	UNDEFINED = -1,
	TO_NEAREST,
	UPWARD,
	DOWNWARD,
	TOWARD_ZERO
};

/**
  * scoped round mode is a class that implements a RAII round mode
  * conforming to IEEE 854
  */
class scoped_round_mode {
public:
	scoped_round_mode(round_mode mode);

	virtual ~scoped_round_mode();
private:
	int32_t const m_original_round_mode;
};

template<class F, class ... Args>
auto execute_in(round_mode m, F f, Args&& ... args)
{
	scoped_round_mode set_round{ m };
	return f(std::forward<Args>(args)...);
}

template<class F, class ... Args>
auto execute_downward(F f, Args&& ... args)
{
	return execute_in(
		round_mode::DOWNWARD,
		f,
		std::forward<Args>(args)...
	);
}

template<class F, class ... Args>
auto execute_upward(F f, Args&& ... args)
{
	return execute_in(
		round_mode::UPWARD,
		f,
		std::forward<Args>(args)...
	);
}



} // end of namespace ia
