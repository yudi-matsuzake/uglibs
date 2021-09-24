
#include <catch2/catch.hpp>

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
