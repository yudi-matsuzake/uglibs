#include "catch2/catch_all.hpp"

#include "gmt/cross-product.hpp"
#include "gmt/vector.hpp"
#include <algorithm>
#include <numeric>

TEST_CASE("Cross product", "[cross-product]")
{
	SECTION("n = 2"){
		using vec2 = gmt::vector<double, 2>;

		for(auto&& v : std::array{
				vec2{ 1., 0. },
				vec2{ 0., 1. },
				vec2{ 1., 1. }}){

			auto o = gmt::compute_orthogonal_vector(v);
			INFO("v : " << v[0] << ' ' << v[1]);
			INFO("o : " << v[0] << ' ' << v[1]);
			REQUIRE(std::inner_product(
				begin(v), end(v),
				begin(o),
				0.0) == 0);
		}
	}

	SECTION("n = 3"){
		using vec3 = gmt::vector<double, 3>;

		for(auto&& vs : std::array{
			std::array{ vec3{ 1., 0., 0. }, vec3{ 0., 1., 0. } },
			std::array{ vec3{ 0., 1., 0. }, vec3{ 0., 0., 1. } },
			std::array{ vec3{ 1., 0., 1. }, vec3{ 0., 1., 0. } }}){

			auto o = gmt::compute_orthogonal_vector(vs);
			for(auto&& v : vs){
				REQUIRE(std::inner_product(
					begin(v), end(v),
					begin(o),
					0.0) == 0
				);
			}
		}

	}

	SECTION("n = 4"){
		using vec = gmt::vector<double, 4>;
		vec v0{ 1., 0., 0., 0. };
		vec v1{ 0., 0., 1., 0. };
		vec v2{ 0., 0., 0., 1. };
		
		std::array a{ v0, v1, v2 };
		REQUIRE(gmt::cross_product(a) == vec{ 0., -1., 0., 0. });
	}

}

TEST_CASE("parallelotope_signed_area", "[cross-product]")
{
	SECTION("n = 2"){
		using vec2 = gmt::vector<double, 2>;

		REQUIRE(gmt::parallelotope_signed_area(
			std::array{
				vec2{ 1.0, 0.0 },
				vec2{ 1.0, 1.0 }
			}
		) > 0.0);

		REQUIRE(gmt::parallelotope_signed_area(
			std::array{ vec2{ 0.0, 1.0 }, vec2{ 1.0, 0.0 } }
		) < 0.0);

		REQUIRE(gmt::parallelotope_signed_area(
			std::array{ vec2{ 0.0, 1.0 }, vec2{ 1.0, 1.0 } }
			) < 0.0
		);

		REQUIRE(gmt::parallelotope_signed_area(
			std::array{ vec2{ 0.0, 1.0 }, vec2{ -1.0, 1.0 } }
		) > 0.0);

		REQUIRE(gmt::parallelotope_signed_area(
			std::array{ vec2{ 0.0, 1.0 }, vec2{ -1.0, 0.0 } }
		) > 0.0);

		REQUIRE(gmt::parallelotope_signed_area(
			std::array{ vec2{ 0.0, 1.0 }, vec2{ -1.0, -1.0 } }
		) > 0.0);

	}
}
