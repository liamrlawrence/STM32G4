//------------------------------------------------------------------------------
// File Name    : stm32g491_chip.hh
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Embedded Modules
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : January 19, 2023
//------------------------------------------------------------------------------

#ifndef EMBED_STM32G491_CHIP_HH
#define EMBED_STM32G491_CHIP_HH

#include "../../peripherals/serial/spi/spi.hh"
#include "../../peripherals/gpio/gpio.hh"



namespace Chip {
	void init();

	class GPIO : public GPIO_Class {};
}

#endif //EMBED_STM32G491_CHIP_HH
