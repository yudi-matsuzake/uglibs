
#include "catch2/catch_all.hpp"

#include <list>

#include "util/variant.hpp"

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

TEST_CASE("templated_variant", "[util]")
{
	STATIC_REQUIRE(
		std::is_same_v<
			util::templated_variant<
				std::variant<int, double>,
				std::vector
			>::type,
			std::variant<std::vector<int>, std::vector<double>>
		>
	);

	STATIC_REQUIRE(
		std::is_same_v<
			util::templated_variant_t<
				std::variant<std::string, double>,
				std::vector,
				std::list
			>,
			std::variant<
				std::vector<std::string>,
				std::vector<double>,
				std::list<std::string>,
				std::list<double>
			>
		>
	);
}
