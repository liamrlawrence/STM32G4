//------------------------------------------------------------------------------
// File Name    : gpio.hh
// Authors      : Liam Lawrence
// Created      : January 19, 2023
// Project      : STM32G4 Embedded Modules
// License      : MIT
// Copyright    : (C) 2023, Liam Lawrence
//
// Updated      : January 19, 2023
//------------------------------------------------------------------------------

#ifndef EMBED_GPIO_HH
#define EMBED_GPIO_HH

#include <cstdint>
#include "../../../../include/stm32g491xx.h"



class GPIO_Class {
public:
	enum class Pin_Mode {
		// Doc: RM0440-9.4.1
		INPUT = 0b00,
		OUTPUT = 0b01,
		ALTERNATE = 0b10,
		ANALOG = 0b11
	};

	enum class Pin_Type {
		// Doc: RM0440-9.4.2
		PUSH_PULL = 0b0,
		OPEN_DRAIN = 0b1
	};

	enum class Pin_Speed {
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

	enum class Ports {
		// Doc: RM0440-9.3.4
		GPIO_A = 'A',
		GPIO_B = 'B',
		GPIO_C = 'C',
		GPIO_D = 'D',
		GPIO_E = 'E',
		GPIO_F = 'F',
		GPIO_G = 'G'
	};

	virtual void enable_port_clock(const GPIO_Class::Ports port);
	virtual void disable_port_clock(const GPIO_Class::Ports port);

	virtual void set_mode(const GPIO_Class::Ports port, const uint8_t pin, GPIO_Class::Pin_Mode mode);
	virtual void set_otype(const GPIO_Class::Ports port, const uint8_t pin, GPIO_Class::Pin_Type type);
	virtual void set_ospeed(const GPIO_Class::Ports port, const uint8_t pin, GPIO_Class::Pin_Speed speed);
	virtual void set_pullup_pulldown(const GPIO_Class::Ports port, const uint8_t pin, GPIO_Class::Pin_PUPD pupd);
	virtual void set_alternate_function(const GPIO_Class::Ports port, const uint8_t pin, GPIO_Class::Pin_AF af);

	virtual uint8_t read(const GPIO_Class::Ports port, const uint8_t pin);
	virtual void set(const GPIO_Class::Ports port, const uint8_t pin);
	virtual void clear(const GPIO_Class::Ports port, const uint8_t pin);

private:
	virtual GPIO_TypeDef *port_enum2Register(const GPIO_Class::Ports port);
	virtual void update_port_clock(const GPIO_Class::Ports port, const bool enable_port);
};


#endif //EMBED_GPIO_HH
