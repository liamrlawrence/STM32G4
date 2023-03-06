//------------------------------------------------------------------------------
// File Name    : stm32g491_chip.hh
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : March 5, 2023
//------------------------------------------------------------------------------

#ifndef STM32G4_MODULE_LIBRARY_CHIP_HH
#define STM32G4_MODULE_LIBRARY_CHIP_HH

#include <array>
#include "../../peripherals/gpio/gpio.hh"



namespace Chip {
	void init();

	namespace HAL {
		uint32_t read_register(volatile const uint32_t *reg);
		void set_register(volatile uint32_t *reg, uint32_t val);
		void clear_register(volatile uint32_t *reg, uint32_t val);

		uint32_t read_field(volatile const uint32_t *reg, uint16_t position, uint16_t width);
		void set_field(volatile uint32_t *reg, uint16_t position, uint16_t width, uint32_t val);
		void clear_field(volatile uint32_t *reg, uint16_t position, uint16_t width);


		const uint_fast8_t MAX_BITMASK_WIDTH = 16;
		constexpr std::array<uint32_t, MAX_BITMASK_WIDTH + 1> bitmasks = []() constexpr {
			std::array<uint32_t, MAX_BITMASK_WIDTH + 1> mask = {0};

			uint32_t m = 0;
			for (uint_fast8_t i = 0; i < MAX_BITMASK_WIDTH + 1; i++) {
				mask[i] = m;
				m = (m << 1) | 1;
			}

			return mask;
		}();
	}

	class GPIO : public GPIO_Class {};
}

#endif //STM32G4_MODULE_LIBRARY_CHIP_HH
