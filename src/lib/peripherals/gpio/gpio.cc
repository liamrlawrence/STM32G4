//------------------------------------------------------------------------------
// File Name    : gpio.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
//
// Updated      : May 27, 2026
//------------------------------------------------------------------------------

// TODO: DOC: R0440-9.3.15-9.3.16 | Warning logged if user is using pin PB8
// (boot) or PG10 (reset)

#include "gpio.hh"
#include "../../chip/stm32g491/stm32g491_hal.hh"

#include "log.hh"



using HAL  = Chip::HAL;
using GPIO = Chip::GPIO;


static char gpio_port_letter(const GPIO_TypeDef* const port)
{
	if(port == GPIOA) return 'A';
	if(port == GPIOB) return 'B';
	if(port == GPIOC) return 'C';
	if(port == GPIOD) return 'D';
	if(port == GPIOE) return 'E';
	if(port == GPIOF) return 'F';
	if(port == GPIOG) return 'G';
	return '?';
}


// TODO: Test saving read-port as a value in GPIO_Pin_t and comparing read speeds
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
	const u32 read      = (HAL::read_register(REGISTER) & (MASK << (GPIO_Pin.number * WIDTH))) ? 1 : 0;
	LOG_DEBUG("GPIO::read  P%c%u=%u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number, read);
	return read;
}


void GPIO::set(const GPIO_Pin_t GPIO_Pin)
{
	// DOC: RM0440-9.4.7 | Setting & clearing bits use the same
	// 32b-register GPIOx->BSRR, [15:0] & [31:16] respectively
	vu32* const REGISTER = &GPIO_Pin.port->BSRR;
	constexpr u32 WIDTH  = GPIO::Reg::BSRR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);

	HAL::set_register(REGISTER, MASK << (GPIO_Pin.number * WIDTH));
	LOG_DEBUG("GPIO::set   P%c%u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number);
}


void GPIO::clear(const GPIO_Pin_t GPIO_Pin)
{
	// DOC: RM0440-9.4.7 | Setting & clearing bits use the same
	// 32b-register GPIOx->BSRR, [15:0] & [31:16] respectively
	vu32* const REGISTER = &GPIO_Pin.port->BSRR;
	constexpr u32 WIDTH  = GPIO::Reg::BSRR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);

	HAL::set_register(REGISTER, MASK << ((GPIO_Pin.number + 16) * WIDTH));
	LOG_DEBUG("GPIO::clear P%c%u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number);
}


void GPIO::set_mode(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::MODER::Value mode_value)
{
	// DOC: RM0440-9.4.1
	vu32* const REGISTER = &GPIO_Pin.port->MODER;
	constexpr u32 WIDTH  = GPIO::Reg::MODER::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	const u32 shift      = (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK << shift, mode_value << shift);
	LOG_DEBUG(
	    "GPIO::set_mode   P%c%u = %u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number, mode_value
	);
}


void GPIO::set_otype(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::OTYPER::Value otype_value)
{
	// DOC: RM0440-9.4.2
	vu32* const REGISTER = &GPIO_Pin.port->OTYPER;
	constexpr u32 WIDTH  = GPIO::Reg::OTYPER::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	const u32 shift      = (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK << shift, otype_value << shift);
	LOG_DEBUG(
	    "GPIO::set_otype  P%c%u = %u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number, otype_value
	);
}


void GPIO::set_ospeed(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::OSPEEDR::Value ospeed_value)
{
	// DOC: RM0440-9.4.3
	vu32* const REGISTER = &GPIO_Pin.port->OSPEEDR;
	constexpr u32 WIDTH  = GPIO::Reg::OSPEEDR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	const u32 shift      = (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK << shift, ospeed_value << shift);
	LOG_DEBUG(
	    "GPIO::set_ospeed P%c%u = %u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number, ospeed_value
	);
}


void GPIO::set_pupd(const GPIO_Pin_t GPIO_Pin, const GPIO::Reg::PUPDR::Value pupd_value)
{
	// DOC: RM0440-9.4.4
	vu32* const REGISTER = &GPIO_Pin.port->PUPDR;
	constexpr u32 WIDTH  = GPIO::Reg::PUPDR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	const u32 shift      = (GPIO_Pin.number * WIDTH);

	HAL::update_register(REGISTER, MASK << shift, pupd_value << shift);
	LOG_DEBUG(
	    "GPIO::set_pupd   P%c%u = %u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number, pupd_value
	);
}


void GPIO::set_alternate_function(GPIO_Pin_t GPIO_Pin, GPIO::Reg::AFR::Value af_value)
{
	// DOC: RM0440-9.4.9
	vu32* const REGISTER = (GPIO_Pin.number < 8) ? &GPIO_Pin.port->AFR[0] : &GPIO_Pin.port->AFR[1];
	constexpr u32 WIDTH  = GPIO::Reg::AFR::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);
	const u32 pin        = (GPIO_Pin.number < 8) ? GPIO_Pin.number : GPIO_Pin.number - 8;
	const u32 shift      = (pin * WIDTH);

	HAL::update_register(REGISTER, MASK << shift, af_value << shift);
	LOG_DEBUG("GPIO::set_af     P%c%u = %u", gpio_port_letter(GPIO_Pin.port), GPIO_Pin.number, af_value);
}


void GPIO::set_port_clock(GPIO_TypeDef* const port, const GPIO::Clock::Status::Value clock_status)
{
	// DOC: RM0440-7.4.15
	vu32* const REGISTER = &RCC->AHB2ENR;
	constexpr u32 WIDTH  = GPIO::Clock::Status::WIDTH;
	constexpr u32 MASK   = HAL::generate_bitmask(WIDTH);

	u32 field_position;
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
		LOG_ERROR("GPIO::set_port_clock P%c unknown port", gpio_port_letter(port));
		return;
	}
	const u32 shift = (field_position * WIDTH);

	switch(clock_status) {
	case Clock::Status::ENABLED:
		HAL::update_register(REGISTER, MASK << shift, Clock::Status::ENABLED << shift);
		(void)HAL::read_register(REGISTER);  // TODO: Look up if required to add 1 clock cycle delay
		LOG_DEBUG("GPIO::set_port_clock P%c ENABLE", gpio_port_letter(port));
		break;
	case Clock::Status::DISABLED:
		HAL::clear_register(REGISTER, MASK << shift);
		LOG_DEBUG("GPIO::set_port_clock P%c DISABLE", gpio_port_letter(port));
		break;
	}
}

