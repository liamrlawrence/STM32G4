//------------------------------------------------------------------------------
// File Name    : gpio.hh
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Module Library
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : February 25, 2023
//------------------------------------------------------------------------------

#ifndef STM32G4_MODULE_LIBRARY_GPIO_HH
#define STM32G4_MODULE_LIBRARY_GPIO_HH

#include <cstdint>

#ifdef UNIT_TEST
#include "../../chip/stm32g491/stm32g491_mock.hh"
#else
#include "../../../../include/stm32g491xx.h"
#endif



class GPIO_Class {
public:
	enum class Pin_Mode {
		// Doc: RM0440-9.4.1
		INPUT = 0b00,
		OUTPUT = 0b01,
		ALTERNATE = 0b10,
		ANALOG = 0b11
	};

	enum class Pin_OType {
		// Doc: RM0440-9.4.2
		PUSH_PULL = 0b0,
		OPEN_DRAIN = 0b1
	};

	enum class Pin_OSpeed {
		// Doc: RM0440-9.4.3
		VERY_LOW_SPEED = 0b00,
		LOW_SPEED = 0b01,
		HIGH_SPEED = 0b10,
		VERY_HIGH_SPEED = 0b11
	};

	enum class Pin_PUPD {
		// Doc: RM0440-9.4.4
		NO_PULL_UP_DOWN = 0b00,
		PULL_UP = 0b01,
		PULL_DOWN = 0b10
	};

	enum class Pin_AF {
		// Doc: RM0440-9.4.9
		AF0 = 0b0000,
		AF1 = 0b0001,
		AF2 = 0b0010,
		AF3 = 0b0011,
		AF4 = 0b0100,
		AF5 = 0b0101,
		AF6 = 0b0110,
		AF7 = 0b0111,
		AF8 = 0b1000,
		AF9 = 0b1001,
		AF10 = 0b1010,
		AF11 = 0b1011,
		AF12 = 0b1100,
		AF13 = 0b1101,
		AF14 = 0b1110,
		AF15 = 0b1111
	};

	enum class Clock_Status {
		// Doc: RM0440-7.4.15
		DISABLED = 0b0,
		ENABLED = 0b1,
	};

	typedef struct {
		GPIO_TypeDef *port;     // Doc: RM0440-9.3.4 | A, B, C, D, E, F, & G
		uint16_t number;        // Doc: RM0440-9.3.3 | 0-15
	} GPIO_Pin_t;

	static uint16_t read(GPIO_Pin_t GPIO_Pin);
	static void set(GPIO_Pin_t GPIO_Pin);
	static void clear(GPIO_Pin_t GPIO_Pin);

	static void set_mode(GPIO_Pin_t GPIO_Pin, Pin_Mode mode);
	static void set_otype(GPIO_Pin_t GPIO_Pin, Pin_OType mode);
	static void set_ospeed(GPIO_Pin_t GPIO_Pin, Pin_OSpeed ospeed);
	static void set_pupd(GPIO_Pin_t GPIO_Pin, Pin_PUPD pupd);
	static void set_alternate_function(GPIO_Pin_t GPIO_Pin, Pin_AF af);

	static void set_port_clock(GPIO_TypeDef *port, GPIO_Class::Clock_Status clock_status);

private:
};


#endif //STM32G4_MODULE_LIBRARY_GPIO_HH
