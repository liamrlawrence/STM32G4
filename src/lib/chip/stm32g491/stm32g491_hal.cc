//------------------------------------------------------------------------------
// File Name    : stm32g491_hal.cc
// Authors      : Liam Lawrence
// Created      : February 23, 2023
// Project      : STM32G4 Module Library
// License      : MIT
//
// Updated      : May 26, 2026
//------------------------------------------------------------------------------

#include "stm32g491_hal.hh"



u32 Chip::HAL::read_register(const vu32* const reg) { return *reg; }


void Chip::HAL::set_register(vu32* const reg, u32 val) { *reg |= val; }


void Chip::HAL::clear_register(vu32* const reg, u32 val) { *reg &= ~(val); }


void Chip::HAL::update_register(vu32* const reg, u32 mask, u32 val)
{
	u32 reg_tmp = *reg;
	reg_tmp &= ~(mask);
	reg_tmp |= val;
	*reg = reg_tmp;
}


u32 Chip::HAL::read_field(volatile const u32* const reg, u16 position, u16 width)
{
	u32 reg_val = Chip::HAL::read_register(reg);
	u32 shift   = position * width;

	u32 mask = Chip::HAL::generate_bitmask(width) << shift;
	return (reg_val & mask) >> shift;
}


// void Chip::HAL::set_field(vu32 *const reg, u16 position, u16 width, u32 val)
// {
// 	Chip::HAL::set_register(reg, val << (position * width));
// }
//
//
// void Chip::HAL::clear_field(vu32 *const reg, u16 position, u16 width)
// {
// 	u32 mask = Chip::HAL::generate_bitmask(width) << (position * width);
// 	Chip::HAL::clear_register(reg, mask);
// }

