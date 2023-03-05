//------------------------------------------------------------------------------
// File Name    : stm32g491_mock.cc
// Authors      : Liam Lawrence
// Created      : February 23, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : February 25, 2023
//------------------------------------------------------------------------------

#include "stm32g491_mock.hh"



/*
 * GPIO
 */
const uint16_t NUM_GPIO_PORTS = 7;
GPIO_TypeDef mock_GPIO_ports[NUM_GPIO_PORTS];
GPIO_TypeDef *GPIOA = &mock_GPIO_ports[0];
GPIO_TypeDef *GPIOB = &mock_GPIO_ports[1];
GPIO_TypeDef *GPIOC = &mock_GPIO_ports[2];
GPIO_TypeDef *GPIOD = &mock_GPIO_ports[3];
GPIO_TypeDef *GPIOE = &mock_GPIO_ports[4];
GPIO_TypeDef *GPIOF = &mock_GPIO_ports[5];
GPIO_TypeDef *GPIOG = &mock_GPIO_ports[6];


// Updates all the GPIO Port ODR registers based on the BSRR registers
void update_GPIO_BSRR()
{
	uint32_t reg;
	for (auto &port: mock_GPIO_ports) {
		reg = port.BSRR;
		for (uint16_t j = 0; j < 32; j++) {
			if (j < 16) {
				// Set Register
				if (reg & 1) {
					port.ODR |= (1 << j);
					port.BSRR &= ~(1 << j);
				}
			} else {
				// Reset Register
				if (reg & 1) {
					port.ODR &= ~(1 << (j - 16));
					port.BSRR &= ~(1 << j);
				}
			}
			reg >>= 1;
		}
	}
}


// Returns an int based off the GPIO Port
uint16_t port2int(GPIO_TypeDef *port)
{
	uint16_t num = 0;
	if (port == GPIOA) {
		num = 0;
	} else if (port == GPIOB) {
		num = 1;
	} else if (port == GPIOC) {
		num = 2;
	} else if (port == GPIOD) {
		num = 3;
	} else if (port == GPIOE) {
		num = 4;
	} else if (port == GPIOF) {
		num = 5;
	} else if (port == GPIOG) {
		num = 6;
	}
	return num;
}



/*
 * RCC
 */
RCC_TypeDef mock_RCC;
RCC_TypeDef *RCC = &mock_RCC;
