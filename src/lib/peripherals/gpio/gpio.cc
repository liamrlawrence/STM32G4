//------------------------------------------------------------------------------
// File Name    : gpio.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
//
// Updated      : May 26, 2026
//------------------------------------------------------------------------------

// TODO: Add logging
// TODO: DOC: R0440-9.3.15-9.3.16 | Warning logged if user is using pin PB8
// (boot) or PG10 (reset)

#include "gpio.hh"
#include "../../chip/stm32g491/stm32g491_hal.hh"



using HAL  = Chip::HAL;
using GPIO = Chip::GPIO;


u32 GPIO::read(const GPIO_Pin_t GPIO_Pin)
{
	// DOC: RM0440-9.4.1
	vu32* const MODE_REG         = &GPIO_Pin.port->MODER;
	constexpr u32 MODER_WIDTH    = GPIO::Reg::MODER::WIDTH;
	constexpr u32 MODER_MASK     = HAL::generate_bitmask(MODER_WIDTH);
	GPIO::Reg::MODER::Value mode = static_cast<GPIO::Reg::MODER::Value>(
	    (HAL::read_register(MODE_REG) & (MODER_MASK << (GPIO_Pin.number * MODER_WIDTH)))
	    >> (GPIO_Pin.number * MODER_WIDTH)
	);

	// DOC: RM0440-9.4.5
	vu32* REGISTER;
	switch(mode) {
	case GPIO::Reg::MODER::INPUT:
		REGISTER = &GPIO_Pin.port->IDR;
		break;

	case GPIO::Reg::MODER::OUTPUT:
		REGISTER = &GPIO_Pin.port->ODR;
		break;

	case GPIO::Reg::MODER::ANALOG:
		// TODO: return Analog::read_channel();

	case GPIO::Reg::MODER::ALTERNATE:
	default:
		return 0;
	}

	constexpr u32 WIDTH = GPIO::Reg::IDR_ODR::WIDTH;
	constexpr u32 MASK  = HAL::generate_bitmask(WIDTH);
	return (HAL::read_register(REGISTER) & (MASK << (GPIO_Pin.number * WIDTH))) ? 1 : 0;
}


void GPIO::set(const GPIO_Pin_t GPIO_Pin)
{
	// DOC: RM0440-9.4.7 | Setting & clearing bits use the same
	// 32b-register GPIOx->BSRR, [15:0] & [31:16] respectively
	vu32* const REGISTER = &GPIO_Pin.port->BSRR;
	constexpr u32 WIDTH  = GPIO::Reg::BSRR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);

	HAL::set_register(REGISTER, MASK << (GPIO_Pin.number * WIDTH));
}


void GPIO::clear(const GPIO_Pin_t GPIO_Pin)
{
	// DOC: RM0440-9.4.7 | Setting & clearing bits use the same
	// 32b-register GPIOx->BSRR, [15:0] & [31:16] respectively
	vu32* const REGISTER = &GPIO_Pin.port->BSRR;
	constexpr u32 WIDTH  = GPIO::Reg::BSRR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);

	HAL::set_register(REGISTER, MASK << ((GPIO_Pin.number + 16) * WIDTH));
}


void GPIO::set_mode(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::MODER::Value mode_value)
{
	// DOC: RM0440-9.4.1
	vu32* const REGISTER = &GPIO_Pin.port->MODER;
	constexpr u32 WIDTH  = GPIO::Reg::MODER::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	u32 value            = mode_value << (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK, value);
}


void GPIO::set_otype(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::OTYPER::Value otype_value)
{
	// DOC: RM0440-9.4.2
	vu32* const REGISTER = &GPIO_Pin.port->OTYPER;
	constexpr u32 WIDTH  = GPIO::Reg::OTYPER::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	u32 value            = otype_value << (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK, value);
}


void GPIO::set_ospeed(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::OSPEEDR::Value ospeed_value)
{
	// DOC: RM0440-9.4.3
	vu32* const REGISTER = &GPIO_Pin.port->OSPEEDR;
	constexpr u32 WIDTH  = GPIO::Reg::OSPEEDR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	u32 value            = ospeed_value << (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK, value);
}


void GPIO::set_pupd(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::PUPDR::Value pupd_value)
{
	// DOC: RM0440-9.4.4
	vu32* const REGISTER = &GPIO_Pin.port->PUPDR;
	constexpr u32 WIDTH  = GPIO::Reg::PUPDR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	u32 value            = pupd_value << (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK, value);
}


void GPIO::set_alternate_function(GPIO_Pin_t GPIO_Pin, GPIO::Reg::AFR::Value af_value)
{
	// DOC: RM0440-9.4.9
	vu32* const REGISTER = (GPIO_Pin.number < 8) ? &GPIO_Pin.port->AFR[0] : &GPIO_Pin.port->AFR[1];
	constexpr u32 WIDTH  = GPIO::Reg::AFR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	u32 value            = af_value << (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK, value);
}


void GPIO::set_port_clock(GPIO_TypeDef* const port, const GPIO::Clock::Status::Value clock_status)
{
	// DOC: RM0440-7.4.15
	vu32* const REGISTER = &RCC->AHB2ENR;
	constexpr u32 WIDTH  = GPIO::Clock::Status::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	u32 field_position   = 0;

	if(port == GPIOA) {
		field_position = 0;
	} else if(port == GPIOB) {
		field_position = 1;
	} else if(port == GPIOC) {
		field_position = 2;
	} else if(port == GPIOD) {
		field_position = 3;
	} else if(port == GPIOE) {
		field_position = 4;
	} else if(port == GPIOF) {
		field_position = 5;
	} else if(port == GPIOG) {
		field_position = 6;
	} else {
		// TODO: Error message
	}

	switch(clock_status) {
	case Clock::Status::ENABLED:
		HAL::update_register(REGISTER, MASK, Clock::Status::ENABLED << (field_position * WIDTH));
		break;
	case Clock::Status::DISABLED:
		HAL::clear_register(REGISTER, MASK << (field_position * WIDTH));
		break;
	}
}

