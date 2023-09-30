#pragma once
#include <fmt/format.h>
#include <fmt/ostream.h>

// namespace util {

// template<class T>
// concept fmt_formattable = fmt::is_formattable<T>::value;

// template<class T>
// concept streamable = requires(std::ostream& out, T const& a) {
// 	{ out << a } -> std::convertible_to<std::ostream&>;
// };

// template<class T>
// concept only_streamable = (streamable<T> && fmt_formattable<T>);

// }

// template <util::only_streamable T>
// struct fmt::formatter<T> : fmt::ostream_formatter {};
