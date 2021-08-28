#include "catch2/catch.hpp"

#include "util/bit.hpp"

TEST_CASE("set bit tests", "[util]")
{
	STATIC_REQUIRE(util::set_bit(0b0000, 0) == 0b0001);
	STATIC_REQUIRE(util::set_bit(0b0000, 1) == 0b0010);
	STATIC_REQUIRE(util::set_bit(0b0000, 2) == 0b0100);
	STATIC_REQUIRE(util::set_bit(0b0000, 3) == 0b1000);

	STATIC_REQUIRE(util::set_bit(0b1111, 0) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 2) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 3) == 0b1111);

	STATIC_REQUIRE(util::set_bit(0b000000000000,  8) == 0b000100000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000,  9) == 0b001000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 10) == 0b010000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 11) == 0b100000000000);

	STATIC_REQUIRE(util::set_bit(0b111111111111, 0) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 2) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 3) == 0b111111111111);
}

TEST_CASE("clear bit tests", "[util]")
{
	STATIC_REQUIRE(util::clear_bit(0b1111, 0) == 0b1110);
	STATIC_REQUIRE(util::clear_bit(0b1111, 1) == 0b1101);
	STATIC_REQUIRE(util::clear_bit(0b1111, 2) == 0b1011);
	STATIC_REQUIRE(util::clear_bit(0b1111, 3) == 0b0111);

	STATIC_REQUIRE(util::clear_bit(0b0000, 0) == 0b0000);
	STATIC_REQUIRE(util::clear_bit(0b0000, 1) == 0b0000);
	STATIC_REQUIRE(util::clear_bit(0b0000, 2) == 0b0000);
	STATIC_REQUIRE(util::clear_bit(0b0000, 3) == 0b0000);

	STATIC_REQUIRE(util::clear_bit(0b111111111111,  8) == 0b111011111111);
	STATIC_REQUIRE(util::clear_bit(0b111111111111,  9) == 0b110111111111);
	STATIC_REQUIRE(util::clear_bit(0b111111111111, 10) == 0b101111111111);
	STATIC_REQUIRE(util::clear_bit(0b111111111111, 11) == 0b011111111111);

	STATIC_REQUIRE(util::clear_bit(0b000000000000, 0) == 0b000000000000);
	STATIC_REQUIRE(util::clear_bit(0b000000000000, 1) == 0b000000000000);
	STATIC_REQUIRE(util::clear_bit(0b000000000000, 2) == 0b000000000000);
	STATIC_REQUIRE(util::clear_bit(0b000000000000, 3) == 0b000000000000);
}

TEST_CASE("set bit to tests", "[util]")
{
	STATIC_REQUIRE(util::set_bit(0b0000, 0, 1) == 0b0001);
	STATIC_REQUIRE(util::set_bit(0b0000, 1, 1) == 0b0010);
	STATIC_REQUIRE(util::set_bit(0b0000, 2, 1) == 0b0100);
	STATIC_REQUIRE(util::set_bit(0b0000, 3, 1) == 0b1000);

	STATIC_REQUIRE(util::set_bit(0b1111, 0, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 1, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 2, 1) == 0b1111);
	STATIC_REQUIRE(util::set_bit(0b1111, 3, 1) == 0b1111);

	STATIC_REQUIRE(util::set_bit(0b000000000000,  8, 1) == 0b000100000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000,  9, 1) == 0b001000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 10, 1) == 0b010000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 11, 1) == 0b100000000000);

	STATIC_REQUIRE(util::set_bit(0b111111111111, 0, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 1, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 2, 1) == 0b111111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 3, 1) == 0b111111111111);

	STATIC_REQUIRE(util::set_bit(0b1111, 0, 0) == 0b1110);
	STATIC_REQUIRE(util::set_bit(0b1111, 1, 0) == 0b1101);
	STATIC_REQUIRE(util::set_bit(0b1111, 2, 0) == 0b1011);
	STATIC_REQUIRE(util::set_bit(0b1111, 3, 0) == 0b0111);

	STATIC_REQUIRE(util::set_bit(0b0000, 0, 0) == 0b0000);
	STATIC_REQUIRE(util::set_bit(0b0000, 1, 0) == 0b0000);
	STATIC_REQUIRE(util::set_bit(0b0000, 2, 0) == 0b0000);
	STATIC_REQUIRE(util::set_bit(0b0000, 3, 0) == 0b0000);

	STATIC_REQUIRE(util::set_bit(0b111111111111,  8, 0) == 0b111011111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111,  9, 0) == 0b110111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 10, 0) == 0b101111111111);
	STATIC_REQUIRE(util::set_bit(0b111111111111, 11, 0) == 0b011111111111);

	STATIC_REQUIRE(util::set_bit(0b000000000000, 0, 0) == 0b000000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 1, 0) == 0b000000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 2, 0) == 0b000000000000);
	STATIC_REQUIRE(util::set_bit(0b000000000000, 3, 0) == 0b000000000000);

}
