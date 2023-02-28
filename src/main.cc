//------------------------------------------------------------------------------
// File Name    : main.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Embedded Modules
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : January 19, 2023
//------------------------------------------------------------------------------

#include <iostream>
#include "lib/chip/stm32g491/stm32g491_chip.hh"



int main()
{
	Chip::init();
	auto *gpio = new Chip::GPIO();

	gpio->enable_port_clock(Chip::GPIO::Ports::GPIO_A);
	gpio->set(Chip::GPIO::Ports::GPIO_A, 5);

	for (;;) {

	}
}
