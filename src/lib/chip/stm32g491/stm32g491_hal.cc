//------------------------------------------------------------------------------
// File Name    : stm32g491_hal.cc
// Authors      : Liam Lawrence
// Created      : February 23, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : March 5, 2023
//------------------------------------------------------------------------------

#include "stm32g491_chip.hh"



uint32_t Chip::HAL::read_register(volatile const uint32_t *const reg)
{
	return *reg;
}


void Chip::HAL::set_register(volatile uint32_t *const reg, uint32_t val)
{
	*reg |= val;
}


void Chip::HAL::clear_register(volatile uint32_t *const reg, uint32_t val)
{
	*reg &= ~(val);
}


uint32_t Chip::HAL::read_field(volatile const uint32_t *const reg, uint16_t position, uint16_t width)
{
	uint32_t reg_val = Chip::HAL::read_register(reg);
	uint32_t shift = position * width;

	uint32_t mask = Chip::HAL::bitmasks[width] << shift;
	return (reg_val & mask) >> shift;
}


void Chip::HAL::set_field(volatile uint32_t *const reg, uint16_t position, uint16_t width, uint32_t val)
{
	Chip::HAL::set_register(reg, val << (position * width));
}


void Chip::HAL::clear_field(volatile uint32_t *const reg, uint16_t position, uint16_t width)
{
	uint32_t mask = Chip::HAL::bitmasks[width] << (position * width);
	Chip::HAL::clear_register(reg, mask);
}
