//------------------------------------------------------------------------------
// File Name    : stm32g491_chip.hh
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : February 23, 2023
//------------------------------------------------------------------------------

#ifndef STM32G4_MODULE_LIBRARY_CHIP_HH
#define STM32G4_MODULE_LIBRARY_CHIP_HH


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


		constexpr uint32_t generate_bitmask(const uint32_t n)
		{
			uint32_t mask = 1;
			for (uint_fast8_t i = 1; i < n; i++) {
				mask <<= 1;
				mask |= 1;
			}
			return mask;
		}
	};

	class GPIO : public GPIO_Class {};
}

#endif //STM32G4_MODULE_LIBRARY_CHIP_HH
