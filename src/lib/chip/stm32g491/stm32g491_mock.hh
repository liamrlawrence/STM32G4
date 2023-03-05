//------------------------------------------------------------------------------
// File Name    : stm32g491_mock.hh
// Authors      : Liam Lawrence
// Created      : February 23, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : February 25, 2023
//------------------------------------------------------------------------------

#ifndef STM32G4_MODULE_LIBRARY_MOCK_HH
#define STM32G4_MODULE_LIBRARY_MOCK_HH

#include <cstdint>



/**
  * @brief General Purpose I/O
  */

typedef struct {
	uint32_t MODER;        /*!< GPIO port mode register,               Address offset: 0x00      */
	uint32_t OTYPER;       /*!< GPIO port output type register,        Address offset: 0x04      */
	uint32_t OSPEEDR;      /*!< GPIO port output speed register,       Address offset: 0x08      */
	uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
	uint32_t IDR;          /*!< GPIO port input data register,         Address offset: 0x10      */
	uint32_t ODR;          /*!< GPIO port output data register,        Address offset: 0x14      */
	uint32_t BSRR;         /*!< GPIO port bit set/reset  register,     Address offset: 0x18      */
	uint32_t LCKR;         /*!< GPIO port configuration lock register, Address offset: 0x1C      */
	uint32_t AFR[2];       /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
	uint32_t BRR;          /*!< GPIO Bit Reset register,               Address offset: 0x28      */
} GPIO_TypeDef;

extern GPIO_TypeDef *GPIOA;
extern GPIO_TypeDef *GPIOB;
extern GPIO_TypeDef *GPIOC;
extern GPIO_TypeDef *GPIOD;
extern GPIO_TypeDef *GPIOE;
extern GPIO_TypeDef *GPIOF;
extern GPIO_TypeDef *GPIOG;

void update_GPIO_BSRR();
uint16_t port2int(GPIO_TypeDef *port);


/**
  * @brief Reset and Clock Control
  */

typedef struct {
	uint32_t CR;          /*!< RCC clock control register,                                              Address offset: 0x00 */
	uint32_t ICSCR;       /*!< RCC internal clock sources calibration register,                         Address offset: 0x04 */
	uint32_t CFGR;        /*!< RCC clock configuration register,                                        Address offset: 0x08 */
	uint32_t PLLCFGR;     /*!< RCC system PLL configuration register,                                   Address offset: 0x0C */
	uint32_t RESERVED0;   /*!< Reserved,                                                                Address offset: 0x10 */
	uint32_t RESERVED1;   /*!< Reserved,                                                                Address offset: 0x14 */
	uint32_t CIER;        /*!< RCC clock interrupt enable register,                                     Address offset: 0x18 */
	uint32_t CIFR;        /*!< RCC clock interrupt flag register,                                       Address offset: 0x1C */
	uint32_t CICR;        /*!< RCC clock interrupt clear register,                                      Address offset: 0x20 */
	uint32_t RESERVED2;   /*!< Reserved,                                                                Address offset: 0x24 */
	uint32_t AHB1RSTR;    /*!< RCC AHB1 peripheral reset register,                                      Address offset: 0x28 */
	uint32_t AHB2RSTR;    /*!< RCC AHB2 peripheral reset register,                                      Address offset: 0x2C */
	uint32_t AHB3RSTR;    /*!< RCC AHB3 peripheral reset register,                                      Address offset: 0x30 */
	uint32_t RESERVED3;   /*!< Reserved,                                                                Address offset: 0x34 */
	uint32_t APB1RSTR1;   /*!< RCC APB1 peripheral reset register 1,                                    Address offset: 0x38 */
	uint32_t APB1RSTR2;   /*!< RCC APB1 peripheral reset register 2,                                    Address offset: 0x3C */
	uint32_t APB2RSTR;    /*!< RCC APB2 peripheral reset register,                                      Address offset: 0x40 */
	uint32_t RESERVED4;   /*!< Reserved,                                                                Address offset: 0x44 */
	uint32_t AHB1ENR;     /*!< RCC AHB1 peripheral clocks enable register,                              Address offset: 0x48 */
	uint32_t AHB2ENR;     /*!< RCC AHB2 peripheral clocks enable register,                              Address offset: 0x4C */
	uint32_t AHB3ENR;     /*!< RCC AHB3 peripheral clocks enable register,                              Address offset: 0x50 */
	uint32_t RESERVED5;   /*!< Reserved,                                                                Address offset: 0x54 */
	uint32_t APB1ENR1;    /*!< RCC APB1 peripheral clocks enable register 1,                            Address offset: 0x58 */
	uint32_t APB1ENR2;    /*!< RCC APB1 peripheral clocks enable register 2,                            Address offset: 0x5C */
	uint32_t APB2ENR;     /*!< RCC APB2 peripheral clocks enable register,                              Address offset: 0x60 */
	uint32_t RESERVED6;   /*!< Reserved,                                                                Address offset: 0x64 */
	uint32_t AHB1SMENR;   /*!< RCC AHB1 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x68 */
	uint32_t AHB2SMENR;   /*!< RCC AHB2 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x6C */
	uint32_t AHB3SMENR;   /*!< RCC AHB3 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x70 */
	uint32_t RESERVED7;   /*!< Reserved,                                                                Address offset: 0x74 */
	uint32_t APB1SMENR1;  /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 1, Address offset: 0x78 */
	uint32_t APB1SMENR2;  /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 2, Address offset: 0x7C */
	uint32_t APB2SMENR;   /*!< RCC APB2 peripheral clocks enable in sleep mode and stop modes register, Address offset: 0x80 */
	uint32_t RESERVED8;   /*!< Reserved,                                                                Address offset: 0x84 */
	uint32_t CCIPR;       /*!< RCC peripherals independent clock configuration register,                Address offset: 0x88 */
	uint32_t RESERVED9;   /*!< Reserved,                                                                Address offset: 0x8C */
	uint32_t BDCR;        /*!< RCC backup domain control register,                                      Address offset: 0x90 */
	uint32_t CSR;         /*!< RCC clock control & status register,                                     Address offset: 0x94 */
	uint32_t CRRCR;       /*!< RCC clock recovery RC register,                                          Address offset: 0x98 */
	uint32_t CCIPR2;      /*!< RCC peripherals independent clock configuration register 2,              Address offset: 0x9C */
} RCC_TypeDef;

extern RCC_TypeDef *RCC;


#endif //STM32G4_MODULE_LIBRARY_MOCK_HH
