//------------------------------------------------------------------------------
// File Name    : gpio.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : March 5, 2023
//------------------------------------------------------------------------------

// TODO: Add logging
// TODO: Doc: R0440-9.3.15-9.3.16 | Warning logged if user is using pin PB8 (boot) or PG10 (reset)

#include "gpio.hh"
#include "../../chip/stm32g491/stm32g491_chip.hh"



/*
 * GPIO pin I/O functions
 */
uint16_t GPIO_Class::read(const GPIO_Pin_t GPIO_Pin)
{
	// Doc: RM0440-9.4.1
	volatile uint32_t *const MODE_REG = &GPIO_Pin.port->MODER;
	const uint_fast8_t MODER_FIELD_WIDTH = 2;
	constexpr uint32_t MODER_FIELD_MASK = Chip::HAL::bitmasks[MODER_FIELD_WIDTH];
	Pin_Mode mode = static_cast<Pin_Mode>(
		(Chip::HAL::read_register(MODE_REG) & (MODER_FIELD_MASK << (GPIO_Pin.number * MODER_FIELD_WIDTH)))
			>> (GPIO_Pin.number * MODER_FIELD_WIDTH));

	// Doc: RM0440-9.4.5
	volatile uint32_t *REGISTER;
	const uint_fast8_t FIELD_WIDTH = 1;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	switch (mode) {
		case Pin_Mode::INPUT:
			REGISTER = &GPIO_Pin.port->IDR;
			break;

		case Pin_Mode::OUTPUT:
			REGISTER = &GPIO_Pin.port->ODR;
			break;

		case Pin_Mode::ALTERNATE:
		case Pin_Mode::ANALOG:
		default:
			return 0;
	}

	return (Chip::HAL::read_register(REGISTER) & (FIELD_MASK << (GPIO_Pin.number * FIELD_WIDTH))) ? 1 : 0;
}


void GPIO_Class::set(const GPIO_Pin_t GPIO_Pin)
{
	// Doc: RM0440-9.4.7 | Setting & clearing bits use the same 32b-register GPIOx->BSRR, [15:0] & [31:16] respectively
	volatile uint32_t *const REGISTER = &GPIO_Pin.port->BSRR;
	const uint_fast8_t FIELD_WIDTH = 1;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	Chip::HAL::set_register(REGISTER, FIELD_MASK << (GPIO_Pin.number * FIELD_WIDTH));
}


void GPIO_Class::clear(const GPIO_Pin_t GPIO_Pin)
{
	// Doc: RM0440-9.4.7 | Setting & clearing bits use the same 32b-register GPIOx->BSRR, [15:0] & [31:16] respectively
	volatile uint32_t *const REGISTER = &GPIO_Pin.port->BSRR;
	const uint_fast8_t FIELD_WIDTH = 1;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	Chip::HAL::set_register(REGISTER, FIELD_MASK << ((GPIO_Pin.number + 16) * FIELD_WIDTH));
}



/*
 * GPIO register functions
 */
void GPIO_Class::set_mode(const GPIO_Pin_t GPIO_Pin, const Pin_Mode mode)
{
	// Doc: RM0440-9.4.1
	volatile uint32_t *const REGISTER = &GPIO_Pin.port->MODER;
	const uint_fast8_t FIELD_WIDTH = 2;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	Chip::HAL::clear_register(REGISTER, FIELD_MASK << (GPIO_Pin.number * FIELD_WIDTH));
	Chip::HAL::set_register(REGISTER, static_cast<uint32_t>(mode) << (GPIO_Pin.number * FIELD_WIDTH));
}


void GPIO_Class::set_otype(const GPIO_Pin_t GPIO_Pin, const Pin_OType otype)
{
	// Doc: RM0440-9.4.2
	volatile uint32_t *const REGISTER = &GPIO_Pin.port->OTYPER;
	const uint_fast8_t FIELD_WIDTH = 1;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	Chip::HAL::clear_register(REGISTER, FIELD_MASK << (GPIO_Pin.number * FIELD_WIDTH));
	Chip::HAL::set_register(REGISTER, static_cast<uint32_t>(otype) << (GPIO_Pin.number * FIELD_WIDTH));
}


void GPIO_Class::set_ospeed(const GPIO_Pin_t GPIO_Pin, const Pin_OSpeed ospeed)
{
	// Doc: RM0440-9.4.3
	volatile uint32_t *const REGISTER = &GPIO_Pin.port->OSPEEDR;
	const uint_fast8_t FIELD_WIDTH = 2;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	Chip::HAL::clear_register(REGISTER, FIELD_MASK << (GPIO_Pin.number * FIELD_WIDTH));
	Chip::HAL::set_register(REGISTER, static_cast<uint32_t>(ospeed) << (GPIO_Pin.number * FIELD_WIDTH));
}


void GPIO_Class::set_pupd(const GPIO_Pin_t GPIO_Pin, const Pin_PUPD pupd)
{
	// Doc: RM0440-9.4.4
	volatile uint32_t *const REGISTER = &GPIO_Pin.port->PUPDR;
	const uint_fast8_t FIELD_WIDTH = 2;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	Chip::HAL::clear_register(REGISTER, FIELD_MASK << (GPIO_Pin.number * FIELD_WIDTH));
	Chip::HAL::set_register(REGISTER, static_cast<uint32_t>(pupd) << (GPIO_Pin.number * FIELD_WIDTH));
}


void GPIO_Class::set_alternate_function(GPIO_Pin_t GPIO_Pin, Pin_AF af)
{
	// Doc: RM0440-9.4.9
	volatile uint32_t *const REGISTER = (GPIO_Pin.number < 8) ? &GPIO_Pin.port->AFR[0] : &GPIO_Pin.port->AFR[1];
	const uint_fast8_t FIELD_WIDTH = 4;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];

	Chip::HAL::clear_register(REGISTER, FIELD_MASK << (GPIO_Pin.number * FIELD_WIDTH));
	Chip::HAL::set_register(REGISTER, static_cast<uint32_t>(af) << (GPIO_Pin.number * FIELD_WIDTH));
}



/*
 * GPIO port register functions
 */
void GPIO_Class::set_port_clock(GPIO_TypeDef *const port, const GPIO_Class::Clock_Status clock_status)
{
	// Doc: RM0440-7.4.15
	volatile uint32_t *const REGISTER = &RCC->AHB2ENR;
	const uint_fast8_t FIELD_WIDTH = 1;
	constexpr uint32_t FIELD_MASK = Chip::HAL::bitmasks[FIELD_WIDTH];
	uint16_t field_position = 0;

	if (port == GPIOA) {
		field_position = 0;
	} else if (port == GPIOB) {
		field_position = 1;
	} else if (port == GPIOC) {
		field_position = 2;
	} else if (port == GPIOD) {
		field_position = 3;
	} else if (port == GPIOE) {
		field_position = 4;
	} else if (port == GPIOF) {
		field_position = 5;
	} else if (port == GPIOG) {
		field_position = 6;
	}

	switch (clock_status) {
		case Clock_Status::ENABLED:
			Chip::HAL::set_register(REGISTER, FIELD_MASK << (field_position * FIELD_WIDTH));
			break;
		case Clock_Status::DISABLED:
			Chip::HAL::clear_register(REGISTER, FIELD_MASK << (field_position * FIELD_WIDTH));
			break;
	}
}
