
#include "catch2/catch_test_macros.hpp"

#include <list>
#include <variant>

#include "util/variant.hpp"

using namespace std::string_literals;

TEST_CASE("merged_variant", "[util]")
{
	using v0 = std::variant<int, float, std::string>;
	using v1 = std::variant<long, double, char>;
	using v2 = std::variant<int16_t, uint64_t>;

	STATIC_REQUIRE(
		std::is_same_v<
			util::merged_variant<v0, v1>::type,
			std::variant<
				int,
				float,
				std::string,
				long,
				double,
				char
			>
		>
	);

	STATIC_REQUIRE(
		std::is_same_v<
			util::merged_variant_t<v0, v1, v2>,
			std::variant<int,
				float,
				std::string,
				long,
				double,
				char,
				int16_t,
				uint64_t
			>
		>
	);
}

TEST_CASE("generate_variant_with_t_t", "[util][variant]")
{
	STATIC_REQUIRE(
		std::is_same_v<
			util::generate_variant_with_t_t<
				std::vector,
				util::list_of_types<int, double>
			>,
			std::variant<std::vector<int>, std::vector<double>>
		>
	);

	STATIC_REQUIRE(
		std::is_same_v<
			util::generate_variant_with_t_t<
				std::list,
				util::list_of_types<std::string, double>
			>,
			std::variant<
				std::list<std::string>,
				std::list<double>
			>
		>
	);
}

template<std::size_t N>
using integer_array = std::array<int32_t, N>;
using various_size_array_t = util::generate_variant_with_v_t<
	integer_array,
	util::to_value_list_t<std::make_integer_sequence<std::size_t, 6>>>;

template<class T>
using triple_with_one_type = std::array<T, 3>;
using various_type_array_t = util::generate_variant_with_t_t<
	triple_with_one_type,
	util::list_of_types<int, double, char, std::string>>;

template<class Variant, class Type>
constexpr auto variant_test(Type const& value)
{
	Variant v = value;
	return util::match(
		v,
		[&value](Type const& a){ return a == value; },
		[](auto const& a){ return false; }
	);
};

TEST_CASE("arrays", "[util][variant]")
{

	{
		STATIC_REQUIRE(variant_test<various_size_array_t>(std::array{ 0 }));
		STATIC_REQUIRE(variant_test<various_size_array_t>(std::array{ 0, 1 }));
		STATIC_REQUIRE(variant_test<various_size_array_t>(std::array{ 0, 1, 2 }));
		STATIC_REQUIRE(variant_test<various_size_array_t>(std::array{ 0, 1, 2, 3 }));
		STATIC_REQUIRE(variant_test<various_size_array_t>(std::array{ 0, 1, 2, 3, 4 }));
	}

	{
		REQUIRE(variant_test<various_type_array_t>(std::array{ 1, 2, 3 }));
		REQUIRE(variant_test<various_type_array_t>(std::array{ 2.13, 3.14, 0.1 }));
		REQUIRE(variant_test<various_type_array_t>(std::array{ 'a', 'b', 'c' }));
		REQUIRE(variant_test<various_type_array_t>(std::array{ "presunto"s, "queijo"s, "oi"s }));
	}

	{
		using arrays = util::generate_variant_with_tv_t<
			std::array,
			util::list_of_types<int, char, double>,
			util::list_of_values<3, 4, 5>>;

		STATIC_REQUIRE(variant_test<arrays>(std::array{ 1, 2, 3 }));
		STATIC_REQUIRE(variant_test<arrays>(std::array{ 2.13, 3.14, 0.1 }));
		STATIC_REQUIRE(variant_test<arrays>(std::array{ 'a', 'b', 'c' }));
		STATIC_REQUIRE(variant_test<arrays>(std::array{ 1, 2, 3, 4 }));
		STATIC_REQUIRE(variant_test<arrays>(std::array{ 'a', 'b', 'c', 'd' }));
	}

}
