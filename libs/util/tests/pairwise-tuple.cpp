#include "catch2/catch_all.hpp"

#include "util/meta.hpp"

TEST_CASE("pairwise tuple tests", "[util]")
{
	std::tuple<double, int> t;
	std::tuple<> empty;

	using t_type = std::remove_reference_t<decltype(t)>;
	using empty_type = std::remove_reference_t<decltype(empty)>;

	using concat_type = util::tuple_cat_invoke_result_t<t_type, empty_type>;

	STATIC_REQUIRE(std::is_same_v<concat_type, t_type>);

	STATIC_REQUIRE(
		std::is_same_v<
			util::pairwise_tuple<
				std::pair,
				int, double,
				std::string, float>,

			std::tuple<
				std::pair<int, double>,
				std::pair<std::string, float>>
		>
	);

	STATIC_REQUIRE(
		std::is_same_v<
			util::pairwise_tuple<
				std::pair,
				std::string, double,
				std::string, float>,

			std::tuple<
				std::pair<std::string, double>,
				std::pair<std::string, float>>
		>
	);


	STATIC_REQUIRE(
		std::is_same_v<
			util::pairwise_tuple<
				std::tuple,
				int, double,
				std::string, float>,

			std::tuple<
				std::tuple<int, double>,
				std::tuple<std::string, float>>
		>
	);

	STATIC_REQUIRE(
		std::is_same_v<
			util::pairwise_tuple<
				std::tuple,
				std::string, double,
				std::string, float>,

			std::tuple<
				std::tuple<std::string, double>,
				std::tuple<std::string, float>>
		>
	);

}
