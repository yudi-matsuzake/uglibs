#include "util/io.hpp"


namespace util{

std::ostream& cout_or_file::write(char const* ptr, std::streamsize count)
{
	if(is_open())
		return std::ofstream::write(ptr, count);
	else
		return std::cout.write(ptr, count);
}

std::ostream& cout_or_file::get_stream()
{
	if(is_open())
		return *this;
	else
		return std::cout;
}

std::istream& cin_or_file::read(char* s, std::streamsize count)
{
	if(is_open())
		return std::ifstream::read(s, count);
	else
		return std::cin.read(s, count);
}

std::streamsize cin_or_file::gcount() const
{
	if(is_open())
		return std::ifstream::gcount();
	else
		return std::cin.gcount();
}

std::istream& cin_or_file::get_stream()
{
	if(is_open())
		return *this;
	else
		return std::cin;
}

} // end of namespace util
