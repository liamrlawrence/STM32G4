//------------------------------------------------------------------------------
// File Name    : main.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : March 5, 2023
//------------------------------------------------------------------------------

#include "lib/chip/stm32g491/stm32g491_chip.hh"



int main()
{
	Chip::init();
	using GPIO = Chip::GPIO;

	// Define pins
	GPIO::GPIO_Pin_t led_pin = {.port=GPIOA, .number=5};
	GPIO::GPIO_Pin_t btn_pin = {.port=GPIOC, .number=13};

	// Enable GPIO port clocks
	GPIO::set_port_clock(led_pin.port, GPIO::Clock_Status::ENABLED);
	GPIO::set_port_clock(btn_pin.port, GPIO::Clock_Status::ENABLED);

	// Configure pins
	GPIO::set_mode(led_pin, GPIO::Pin_Mode::OUTPUT);
	GPIO::set_ospeed(led_pin, GPIO::Pin_OSpeed::VERY_HIGH_SPEED);
	GPIO::set_pupd(led_pin, GPIO::Pin_PUPD::PULL_DOWN);
	GPIO::set_mode(btn_pin, GPIO::Pin_Mode::INPUT);

	GPIO::clear(led_pin);

	while (!GPIO::read(btn_pin)) {}

	for (;;) {
		if (GPIO::read(led_pin)) {
			GPIO::clear(led_pin);
		} else {
			GPIO::set(led_pin);
		}
	}
}
