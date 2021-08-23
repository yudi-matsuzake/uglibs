#include <limits>

#pragma once

namespace util{

/**
  * this class simulates an integer with smaller precision than an underline
  * integer type
  */
template<class UnsignedInt, UnsignedInt Bits>
struct unsigned_integer{

	constexpr unsigned_integer() = default;

	constexpr unsigned_integer(UnsignedInt a_n)
		: n{ a_n }
	{}

	constexpr operator UnsignedInt() const
	{
		return n;
	}

	UnsignedInt n;
};

}

/*
 * TODO: fix this horrible thing
 */
namespace std{

template<class UnsignedInt, UnsignedInt Bits>
class numeric_limits<util::unsigned_integer<UnsignedInt, Bits>>{
public:

	using uinteger		= util::unsigned_integer<UnsignedInt, Bits>;
	using underling_type	= UnsignedInt;

	static constexpr auto max()
	{
		constexpr auto n = std::numeric_limits<UnsignedInt>::digits;
		return uinteger{
			~underling_type{ 0 } >> (n - Bits)
		};
	}

	static constexpr auto min()
	{
		return uinteger{ 0 };
	}

	static constexpr auto digits = Bits;
};

} // end of namespace util
