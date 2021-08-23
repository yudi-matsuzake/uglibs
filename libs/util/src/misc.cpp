#include "util/misc.hpp"

namespace util{

bool ends_with(std::wstring const& str, std::string const& ext)
{
	return ends_with(begin(str), end(str), begin(ext), end(ext));
}

bool ends_with(std::string const& str, std::string const& ext)
{
	return ends_with(begin(str), end(str), begin(ext), end(ext));
}


} // end of namespace util
