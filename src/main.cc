//------------------------------------------------------------------------------
// File Name    : main.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
//
// Updated      : May 27, 2026
//------------------------------------------------------------------------------

#include "lib/chip/stm32g491/stm32g491_chip.hh"



int main()
{
	using GPIO = Chip::GPIO;
	Chip::init();

	// Define pins
	GPIO::GPIO_Pin_t led_pin = {.port = GPIOA, .number = 5};
	GPIO::GPIO_Pin_t btn_pin = {.port = GPIOC, .number = 13};

	// Enable GPIO port clocks
	GPIO::set_port_clock(led_pin.port, GPIO::Clock::Status::ENABLED);
	GPIO::set_port_clock(btn_pin.port, GPIO::Clock::Status::ENABLED);

	// Configure pins
	GPIO::set_mode(led_pin, GPIO::Reg::MODER::OUTPUT);
	GPIO::set_otype(led_pin, GPIO::Reg::OTYPER::PUSH_PULL);
	GPIO::set_ospeed(led_pin, GPIO::Reg::OSPEEDR::VERY_HIGH_SPEED);
	GPIO::set_pupd(led_pin, GPIO::Reg::PUPDR::PULL_DOWN);

	GPIO::set_mode(btn_pin, GPIO::Reg::MODER::INPUT);
	GPIO::set_otype(btn_pin, GPIO::Reg::OTYPER::PUSH_PULL);
	GPIO::set_ospeed(btn_pin, GPIO::Reg::OSPEEDR::VERY_HIGH_SPEED);
	GPIO::set_pupd(btn_pin, GPIO::Reg::PUPDR::PULL_DOWN);

	// Main
	GPIO::set(led_pin);

	while(!GPIO::read(btn_pin))
		;

	for(;;) {
		if(GPIO::read(btn_pin)) {
			GPIO::set(led_pin);
		} else {
			GPIO::clear(led_pin);
		}
	}
}

