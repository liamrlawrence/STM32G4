//------------------------------------------------------------------------------
// File Name    : stm32g491_chip.cc
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : May 27, 2026
//------------------------------------------------------------------------------

#include "stm32g491_chip.hh"
#include "log.hh"



void Chip::init()
{
	SEGGER_RTT_Init();
	LOG_INFO("RTT is alive");
}

