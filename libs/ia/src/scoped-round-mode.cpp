#include <cfenv>

#include "ia/scoped-round-mode.hpp"

namespace ia{

static int to_int(round_mode mode)
{
	switch(mode){
	case round_mode::TO_NEAREST:	return FE_TONEAREST;
	case round_mode::UPWARD:	return FE_UPWARD;
	case round_mode::DOWNWARD:	return FE_DOWNWARD;
	case round_mode::TOWARD_ZERO:	return FE_TOWARDZERO;
	default:
	case round_mode::UNDEFINED:	return -1;
	}
}

scoped_round_mode::scoped_round_mode(round_mode mode)
	: m_original_round_mode(fegetround())
{
	std::fesetround(to_int(mode));
}

scoped_round_mode::~scoped_round_mode()
{
	std::fesetround(m_original_round_mode);
}

} // end of namespace ia
