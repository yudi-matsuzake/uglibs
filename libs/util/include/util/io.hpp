#pragma once
#include <iterator>
#include <iostream>
#include <optional>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include <utility>
#include <type_traits>

namespace util{

/**
  * simple ofstream that if is not open output to cout
  */
class cout_or_file : protected std::ofstream {
public:

	using std::ofstream::ofstream;
	using std::ofstream::open;
	using std::ofstream::is_open;
	using std::ofstream::close;
	using std::ofstream::bad;

	std::ostream& get_stream();

	template<class T>
	std::ostream& operator<<(T const& value);
	std::ostream& write(char const* ptr, std::streamsize count);
};

/**
  * simple ifstream that if is not open input from cin
  */
class cin_or_file : protected std::ifstream {
public:

	using std::ifstream::ifstream;
	using std::ifstream::open;
	using std::ifstream::is_open;
	using std::ifstream::close;
	using std::ifstream::bad;

	std::istream& get_stream();

	template<class T>
	std::istream& operator>>(T& value);
	std::istream& read( char* s, std::streamsize count);
	std::streamsize gcount() const;
};

template<class T>
std::ostream& cout_or_file::operator<<(T const& value)
{
	if(is_open()){
		std::ofstream& base = *this;
		return base << value;
	}else{
		return std::cout << value;
	}
}

template<class T>
std::istream& cin_or_file::operator>>(T& value)
{
	if(is_open()){
		std::ifstream& base = *this;
		return base >> value;
	}else{
		return std::cin >> value;
	}
}

/**
  * functor to extract a placeholder of type T
  * from a buffer
  */
template<class T>
struct stream_extractor{
	T& ref;

	explicit stream_extractor(T& a_placeholder)
		: ref(a_placeholder)
	{}

	void operator()(char const* buffer)
	{
		std::stringstream ss(buffer);
		ss >> ref;
	}
};

/**
  * functor to extract a placeholder of type T
  * from a buffer
  */
template<class T>
struct stream_extractor<std::optional<T>>{
	std::optional<T>& ref;

	explicit stream_extractor(std::optional<T>& a_placeholder)
		: ref(a_placeholder)
	{}

	void operator()(char const* buffer)
	{
		T tmp;
		std::stringstream ss(buffer);
		ss >> tmp;
		ref = tmp;
	}
};

} // end of namespace util
