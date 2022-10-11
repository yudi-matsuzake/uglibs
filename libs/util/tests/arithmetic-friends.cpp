#include "catch2/catch_all.hpp"

#include "util/arithmetic-friends.hpp"

template<class T>
struct derived : public util::arithmetic_friends<derived<T>> {

	constexpr derived() = default;

	constexpr derived(T a_value) : value{ a_value }
	{}

	constexpr friend derived operator-(derived const& other)
	{
		derived negated{ -other.value };
		return negated;
	}

	constexpr derived& operator-=(derived const& other)
	{
		value -= other.value;
		return *this;
	}

	constexpr derived& operator+=(derived const& other)
	{
		value += other.value;
		return *this;
	}

	constexpr derived& operator*=(derived const& other)
	{
		value *= other.value;
		return *this;
	}

	constexpr derived& operator/=(derived const& other)
	{
		value /= other.value;
		return *this;
	}

	constexpr auto operator==(derived const& other) const noexcept
	{
		return value == other.value;
	}

	T value;
};


TEST_CASE("arithmetic friends", "[util]")
{
	constexpr derived<int> a{1}, b{2};

	constexpr auto c = a+b+1;
	STATIC_REQUIRE(c == 4);

	constexpr auto d = a*b;
	STATIC_REQUIRE((d + 1) == 3);

	constexpr auto e = a-b;
	STATIC_REQUIRE(e == -1);

	STATIC_REQUIRE((derived{ 1.f } / derived{ 2.f }) == 0.5f);
}
