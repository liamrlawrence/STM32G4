//------------------------------------------------------------------------------
// File Name    : utest_stm32g491_hal.cc
// Authors      : Liam Lawrence
// Created      : February 23, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : February 25, 2023
//------------------------------------------------------------------------------

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include "../stm32g491_chip.hh"



TEST_CASE("Chip HAL", "[Chip][HAL][Registers]")
{
	auto rand_reg = GENERATE(take(1000, random(INT32_MIN, INT32_MAX)));


	SECTION("Chip Initialization") {
		Chip::init();
	}


	SECTION("Read/Set/Clear Registers") {
		// Doc: RM0440-7.4.15
		auto rand_width = GENERATE(take(1, random(1, 8)));
		auto rand_position = GENERATE(take(1, random(0, UINT16_MAX)));
		auto rand_value = GENERATE(take(1, random(0, UINT16_MAX)));

		uint32_t reg = static_cast<uint32_t>(rand_reg);
		uint16_t width = static_cast<uint16_t>(rand_width);
		uint16_t position = static_cast<uint16_t>(rand_position) % static_cast<uint16_t>(32 / width);
		uint32_t value = static_cast<uint32_t>(rand_value) & Chip::HAL::generate_bitmask(width);

		Chip::HAL::clear_field(&reg, position, width);
		REQUIRE(Chip::HAL::read_field(&reg, position, width) == 0);

		Chip::HAL::set_field(&reg, position, width, value);
		REQUIRE(Chip::HAL::read_field(&reg, position, width) == value);
	}
}
