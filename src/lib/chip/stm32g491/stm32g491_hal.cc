//------------------------------------------------------------------------------
// File Name    : stm32g491_hal.cc
// Authors      : Liam Lawrence
// Created      : February 23, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : February 25, 2023
//------------------------------------------------------------------------------

#include "stm32g491_hal.hh"
#include "../../../types.hh"



u32 Chip::HAL::read_register(volatile const u32 *const reg)
{
	return *reg;
}


void Chip::HAL::set_register(volatile u32 *const reg, u32 val)
{
	*reg |= val;
}


void Chip::HAL::clear_register(volatile u32 *const reg, u32 val)
{
	*reg &= ~(val);
}


u32 Chip::HAL::read_field(volatile const u32 *const reg, uint16_t position, uint16_t width)
{
	u32 reg_val = Chip::HAL::read_register(reg);
	u32 shift = position * width;

	u32 mask = Chip::HAL::generate_bitmask(width) << shift;
	return (reg_val & mask) >> shift;
}


void Chip::HAL::set_field(volatile u32 *const reg, uint16_t position, uint16_t width, u32 val)
{
	Chip::HAL::set_register(reg, val << (position * width));
}


void Chip::HAL::clear_field(volatile u32 *const reg, uint16_t position, uint16_t width)
{
	u32 mask = Chip::HAL::generate_bitmask(width) << (position * width);
	Chip::HAL::clear_register(reg, mask);
}

