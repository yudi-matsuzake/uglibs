#include "catch2/catch_test_macros.hpp"

#include "util/meta.hpp"


TEST_CASE("integer queue", "[util][meta]")
{
	STATIC_REQUIRE(
		std::is_same_v<
			util::integer_queue_empty<int>,
			std::integer_sequence<int>>);

	using empty_t = util::integer_queue_empty<int>;
	STATIC_REQUIRE(
		std::is_same_v<
			util::integer_queue_type_t<empty_t>, int>);

	STATIC_REQUIRE(
		std::is_same_v<
		util::integer_queue_push_back_t<empty_t, 3>,
		std::integer_sequence<int, 3>>);

	STATIC_REQUIRE(
		std::is_same_v<
		util::integer_queue_push_back_t<
			util::integer_queue_push_back_t<empty_t, 3>,
			4>,
		std::integer_sequence<int, 3, 4>>);

	STATIC_REQUIRE(
		std::is_same_v<
		util::integer_queue_push_front_t<
			util::integer_queue_push_front_t<empty_t, 3>,
			4>,
		std::integer_sequence<int, 4, 3>>);
}

TEST_CASE("tuple_reverse", "[util][meta]")
{
	constexpr std::tuple<int, double, float> t{ 1, 2.3, 1.3f };
	constexpr auto r = util::tuple_reverse(t);
	STATIC_REQUIRE(
		std::is_same_v<
			decltype(r),
			std::tuple<float, double, int> const
		>
	);

	STATIC_REQUIRE(std::get<0>(t) == std::get<2>(r));
	STATIC_REQUIRE(std::get<1>(t) == std::get<1>(r));
	STATIC_REQUIRE(std::get<2>(t) == std::get<0>(r));
}
