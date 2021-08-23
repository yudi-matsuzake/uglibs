#pragma once

#include <concepts>

namespace util{

template<class T>
concept pointer = requires(T ptr){
	{ *ptr };
	{ ++ptr };
	{ ptr++ };
	{ --ptr };
	{ ptr-- };
	{ ptr <=> T{} };
	{ ptr - int{} } -> std::convertible_to<T>;
	{ ptr + int{} } -> std::convertible_to<T>;
	{ ptr - T{} } -> std::convertible_to<int>;
};


} // end of namespace util
