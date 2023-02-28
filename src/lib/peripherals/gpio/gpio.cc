//------------------------------------------------------------------------------
// File Name    : gpio.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Embedded Modules
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : January 19, 2023
//------------------------------------------------------------------------------

// TODO: Add logging
// TODO: Doc: R0440-9.3.15-9.3.16 | Warning logged if user is using pin PB8 (boot) or PG10 (reset)

#include "gpio.hh"



// These are the 5 main registers for each of the GPIO pins (mode, otype, speed, pupd, af)
void GPIO_Class::set_mode(const Ports port, const uint8_t pin, const Pin_Mode mode)
{
	// Doc: RM0440-9.4.1
	GPIO_TypeDef *prt = port_enum2Register(port);
	prt->MODER &= ~(0b11 << (pin * 2));
	prt->MODER |= ((uint8_t) mode << (pin * 2));
}


void GPIO_Class::set_otype(const Ports port, const uint8_t pin, const Pin_Type type)
{
	// Doc: RM0440-9.4.2
	GPIO_TypeDef *prt = port_enum2Register(port);
	prt->OTYPER &= ~(0b1 << pin);
	prt->OTYPER |= ((uint8_t) type << pin);
}


void GPIO_Class::set_ospeed(const Ports port, const uint8_t pin, const Pin_Speed speed)
{
	// Doc: RM0440-9.4.3
	GPIO_TypeDef *prt = port_enum2Register(port);
	prt->OSPEEDR &= ~(0b11 << (pin * 2));
	prt->OSPEEDR |= ((uint8_t) speed << (pin * 2));
}


void GPIO_Class::set_pullup_pulldown(const Ports port, const uint8_t pin, const Pin_PUPD pupd)
{
	// Doc: RM0440-9.4.4
	GPIO_TypeDef *prt = port_enum2Register(port);
	prt->PUPDR &= ~(0b11 << (pin * 2));
	prt->PUPDR |= ((uint8_t) pupd << (pin * 2));
}


void GPIO_Class::set_alternate_function(const Ports port, const uint8_t pin, const Pin_AF af)
{
	// Doc: RM0440-9.4.9-9.4.10
	GPIO_TypeDef *prt = port_enum2Register(port);
	if (pin < 8) {
		prt->AFR[0] &= ~(0b1111 << (pin * 4));
		prt->AFR[0] |= ((uint8_t) af << (pin * 4));
	} else {
		prt->AFR[1] &= ~(0b1111 << ((pin - 8) * 4));
		prt->AFR[1] |= ((uint8_t) af << ((pin - 8) * 4));
	}
}


/////////////////////////////
// GPIO register functions //
/////////////////////////////
void GPIO_Class::enable_port_clock(const Ports port)
{
	update_port_clock(port, true);
}


void GPIO_Class::disable_port_clock(const Ports port)
{
	update_port_clock(port, false);
}


uint8_t GPIO_Class::read(const Ports port, const uint8_t pin)
{
	// Doc: RM0440-9.4.5
	GPIO_TypeDef *prt = port_enum2Register(port);
	return (prt->IDR & (1 << pin)) ? 1 : 0;
}


void GPIO_Class::set(const Ports port, const uint8_t pin)
{
	// Doc: RM0440-9.4.7
	GPIO_TypeDef *prt = port_enum2Register(port);
	prt->BSRR = (1 << pin);
}


void GPIO_Class::clear(const GPIO_Class::Ports port, const uint8_t pin)
{
	// Doc: RM0440-9.4.7 | Setting & clearing bits use the same 32b-register GPIOx->BSRR, [15:0] & [31:16] respectively
	GPIO_Class::set(port, pin + 16);
}


//////////////////////
// HELPER FUNCTIONS //
//////////////////////
GPIO_TypeDef *GPIO_Class::port_enum2Register(const Ports port)
{
	switch (port) {
		case Ports::GPIO_A:
			return GPIOA;
		case Ports::GPIO_B:
			return GPIOB;
		case Ports::GPIO_C:
			return GPIOC;
		case Ports::GPIO_D:
			return GPIOD;
		case Ports::GPIO_E:
			return GPIOE;
		case Ports::GPIO_F:
			return GPIOF;
		case Ports::GPIO_G:
			return GPIOG;
	}
}


void GPIO_Class::update_port_clock(const Ports port, const bool enable_port)
{
	// Doc: RM0440-7.4.15
	uint8_t register_position;
	switch (port) {
		case Ports::GPIO_A:
			register_position = 0;
			break;
		case Ports::GPIO_B:
			register_position = 1;
			break;
		case Ports::GPIO_C:
			register_position = 2;
			break;
		case Ports::GPIO_D:
			register_position = 3;
			break;
		case Ports::GPIO_E:
			register_position = 4;
			break;
		case Ports::GPIO_F:
			register_position = 5;
			break;
		case Ports::GPIO_G:
			register_position = 6;
			break;
	}

	if (enable_port) {
		RCC->AHB2ENR |= (1 << register_position);
	} else {
		RCC->AHB2ENR &= ~(1 << register_position);
	}
}
