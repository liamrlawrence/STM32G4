//------------------------------------------------------------------------------
// File Name    : utest_gpio.cc
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
#include "../gpio.hh"
#include "../../../chip/stm32g491/stm32g491_chip.hh"



TEST_CASE("GPIO functions", "[GPIO][PERIPHERAL]")
{
	using GPIO = Chip::GPIO;
	auto gpio_ports = GENERATE(GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG);
	auto pin_numbers = GENERATE(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	uint32_t field;


	SECTION("GPIO Set Port Clocks") {
		GPIO::GPIO_Pin_t test_pin = {.port=gpio_ports, .number=static_cast<uint16_t>(pin_numbers)};
		uint32_t *const REGISTER = &RCC->AHB2ENR;
		const uint_fast8_t FIELD_WIDTH = 1;

		GPIO::set_port_clock(test_pin.port, GPIO::Clock_Status::ENABLED);
		field = Chip::HAL::read_field(REGISTER, port2int(test_pin.port), FIELD_WIDTH);
		REQUIRE(field == static_cast<uint32_t>(GPIO::Clock_Status::ENABLED));

		GPIO::set_port_clock(test_pin.port, GPIO::Clock_Status::DISABLED);
		field = Chip::HAL::read_field(REGISTER, port2int(test_pin.port), FIELD_WIDTH);
		REQUIRE(field == static_cast<uint32_t>(GPIO::Clock_Status::DISABLED));
	}


	SECTION("GPIO Set Pin Mode") {
		// Doc: RM0440-9.4.1
		auto pin_mode = GENERATE(GPIO::Pin_Mode::INPUT, GPIO::Pin_Mode::OUTPUT, GPIO::Pin_Mode::ALTERNATE,
		                         GPIO::Pin_Mode::ANALOG);

		GPIO::GPIO_Pin_t test_pin = {.port=gpio_ports, .number=static_cast<uint16_t>(pin_numbers)};
		uint32_t *const REGISTER = &test_pin.port->MODER;
		const uint_fast8_t FIELD_WIDTH = 2;

		GPIO::set_mode(test_pin, pin_mode);
		field = Chip::HAL::read_field(REGISTER, test_pin.number, FIELD_WIDTH);
		REQUIRE(field == static_cast<uint32_t>(pin_mode));
	}


	SECTION("GPIO Set Pin Output Type") {
		// Doc: RM0440-9.4.2
		auto pin_otype = GENERATE(GPIO::Pin_OType::OPEN_DRAIN, GPIO::Pin_OType::PUSH_PULL);

		GPIO::GPIO_Pin_t test_pin = {.port=gpio_ports, .number=static_cast<uint16_t>(pin_numbers)};
		uint32_t *const REGISTER = &test_pin.port->OTYPER;
		const uint_fast8_t FIELD_WIDTH = 1;

		GPIO::set_otype(test_pin, pin_otype);
		field = Chip::HAL::read_field(REGISTER, test_pin.number, FIELD_WIDTH);
		REQUIRE(field == static_cast<uint32_t>(pin_otype));
	}


	SECTION("GPIO Set Pin Output Speed") {
		// Doc: RM0440-9.4.3
		auto pin_ospeed = GENERATE(GPIO::Pin_OSpeed::VERY_LOW_SPEED, GPIO::Pin_OSpeed::LOW_SPEED,
		                           GPIO::Pin_OSpeed::HIGH_SPEED, GPIO::Pin_OSpeed::VERY_HIGH_SPEED);

		GPIO::GPIO_Pin_t test_pin = {.port=gpio_ports, .number=static_cast<uint16_t>(pin_numbers)};
		uint32_t *const REGISTER = &test_pin.port->OSPEEDR;
		const uint_fast8_t FIELD_WIDTH = 2;

		GPIO::set_ospeed(test_pin, pin_ospeed);
		field = Chip::HAL::read_field(REGISTER, test_pin.number, FIELD_WIDTH);
		REQUIRE(field == static_cast<uint32_t>(pin_ospeed));
	}


	SECTION("GPIO Set Pin Pull-up Pull-down Resistor") {
		// Doc: RM0440-9.4.4
		auto pin_pupd = GENERATE(GPIO::Pin_PUPD::NO_PULL_UP_DOWN, GPIO::Pin_PUPD::PULL_UP, GPIO::Pin_PUPD::PULL_DOWN);

		GPIO::GPIO_Pin_t test_pin = {.port=gpio_ports, .number=static_cast<uint16_t>(pin_numbers)};
		uint32_t *const REGISTER = &test_pin.port->PUPDR;
		const uint_fast8_t FIELD_WIDTH = 2;

		GPIO::set_pupd(test_pin, pin_pupd);
		field = Chip::HAL::read_field(REGISTER, test_pin.number, FIELD_WIDTH);
		REQUIRE(field == static_cast<uint32_t>(pin_pupd));
	}


	SECTION("GPIO Set Alternate Function") {
		// Doc: RM0440-9.4.9
		auto pin_af = GENERATE(GPIO::Pin_AF::AF0, GPIO::Pin_AF::AF1, GPIO::Pin_AF::AF2, GPIO::Pin_AF::AF3,
		                       GPIO::Pin_AF::AF4, GPIO::Pin_AF::AF5, GPIO::Pin_AF::AF6, GPIO::Pin_AF::AF7,
		                       GPIO::Pin_AF::AF8, GPIO::Pin_AF::AF9, GPIO::Pin_AF::AF10, GPIO::Pin_AF::AF11,
		                       GPIO::Pin_AF::AF12, GPIO::Pin_AF::AF13, GPIO::Pin_AF::AF14, GPIO::Pin_AF::AF15);

		GPIO::GPIO_Pin_t test_pin = {.port=gpio_ports, .number=static_cast<uint16_t>(pin_numbers)};
		uint32_t *const REGISTER = (test_pin.number < 8) ? &test_pin.port->AFR[0] : &test_pin.port->AFR[1];
		const uint_fast8_t FIELD_WIDTH = 4;

		GPIO::set_alternate_function(test_pin, pin_af);
		field = Chip::HAL::read_field(REGISTER, test_pin.number, FIELD_WIDTH);
		REQUIRE(field == static_cast<uint32_t>(pin_af));
	}


	SECTION("GPIO Read/Set/Clear") {
		// Doc: RM0440-9.4.5-9.4.7
		auto pin_mode = GENERATE(GPIO::Pin_Mode::INPUT, GPIO::Pin_Mode::OUTPUT, GPIO::Pin_Mode::ALTERNATE,
		                         GPIO::Pin_Mode::ANALOG);

		GPIO::GPIO_Pin_t test_pin = {.port=gpio_ports, .number=static_cast<uint16_t>(pin_numbers)};
		GPIO::set_mode(test_pin, pin_mode);

		switch (pin_mode) {
			case GPIO_Class::Pin_Mode::OUTPUT:
				GPIO::set(test_pin);
				update_GPIO_BSRR();
				REQUIRE(GPIO::read(test_pin) == 1);

				GPIO::clear(test_pin);
				update_GPIO_BSRR();
				REQUIRE(GPIO::read(test_pin) == 0);
				break;

			case GPIO_Class::Pin_Mode::INPUT:
			case GPIO_Class::Pin_Mode::ALTERNATE:
			case GPIO_Class::Pin_Mode::ANALOG:
				REQUIRE(GPIO::read(test_pin) == 0);
				return;
		}
	}
}
