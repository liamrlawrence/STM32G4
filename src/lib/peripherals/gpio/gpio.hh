//------------------------------------------------------------------------------
// File Name    : gpio.hh
// Authors	    : Liam Lawrence
// Created	    : January 19, 2023
// Project	    : STM32G4 Module Library
// License	    : MIT
//
// Updated	    : May 27, 2026
//------------------------------------------------------------------------------


// ─── Group declarations ──────────────────────────────────────────────────────
/**
 * @defgroup GPIO_IO Pin I/O
 * @brief Read and write GPIO pin state.
 */

/**
 * @defgroup GPIO_Config Pin Configuration
 * @brief Configure mode, speed, pull resistors, and alternate functions.
 */

/**
 * @defgroup GPIO_Port Port Control
 * @brief Enable and disable GPIO port clocks.
 */
// ─────────────────────────────────────────────────────────────────────────────


#pragma once

#include "types.hh"

#ifdef UNIT_TEST
// #include "../../chip/stm32g491/stm32g491_mock.hh"
#else
#include "stm32g491xx.h"
// #include "../../chip/stm32g491/reg.hh"
#endif



namespace Chip {
/**
 * @brief GPIO peripheral driver for STM32G4.
 *
 * Provides static methods for reading and writing pin state, configuring
 * pin mode, output type, speed, pull resistors, and alternate functions,
 * and enabling or disabling GPIO port clocks.
 */
class GPIO {
public:
	/// Represents a single GPIO pin by port and pin number.
	typedef struct {
		GPIO_TypeDef* port;  ///< GPIO port (GPIOA–GPIOG). See RM0440-9.3.4
		uint16_t number;     ///< Pin number 0–15. See RM0440-9.3.3
	} GPIO_Pin_t;


	struct Reg {
		struct IDR_ODR {
			static constexpr u32 WIDTH = 1;
		};

		struct BSRR {
			static constexpr u32 WIDTH = 1;
		};

		struct MODER {
			static constexpr u32 WIDTH = 2;

			/// Pin mode selection. See RM0440-9.4.1
			enum Value : u32 {
				INPUT     = 0b00,  ///< Digital input mode.
				OUTPUT    = 0b01,  ///< General-purpose output mode.
				ALTERNATE = 0b10,  ///< Alternate function mode.
				ANALOG    = 0b11,  ///< Analog mode (ADC/DAC).
			};
		};

		struct OTYPER {
			static constexpr u32 WIDTH = 1;
			/// See RM0440-9.4.2
			enum Value : u32 {
				PUSH_PULL  = 0b0,
				OPEN_DRAIN = 0b1,
			};
		};

		struct OSPEEDR {
			static constexpr u32 WIDTH = 2;

			/// Output slew rate.
			/// Frequencies are STM32G474 maximums at C=~50 pF, 2.7 V ≤ VDD ≤ 3.6 V.
			/// See RM0440-9.4.3. Values change depending on specific chip.
			enum Value : u32 {
				VERY_LOW_SPEED  = 0b00,  ///< ~5 MHz (C=50 pf) - Lowest slew rate & EMI
				LOW_SPEED       = 0b01,  ///< ~25 MHz (C=50 pf)
				HIGH_SPEED      = 0b10,  ///< ~50 MHz (C=50 pf)
				VERY_HIGH_SPEED = 0b11,  ///< ~120 MHz (C=30 pf)
			};
		};

		struct PUPDR {
			static constexpr u32 WIDTH = 2;
			/// See RM0440-9.4.4
			enum Value : u32 {
				NO_PULL_UP_DOWN = 0b00,
				PULL_UP         = 0b01,
				PULL_DOWN       = 0b10,
			};
		};

		struct AFR {
			static constexpr u32 WIDTH = 4;
			/// See RM0440-9.4.9
			enum Value : u32 {
				AF0  = 0b0000,
				AF1  = 0b0001,
				AF2  = 0b0010,
				AF3  = 0b0011,
				AF4  = 0b0100,
				AF5  = 0b0101,
				AF6  = 0b0110,
				AF7  = 0b0111,
				AF8  = 0b1000,
				AF9  = 0b1001,
				AF10 = 0b1010,
				AF11 = 0b1011,
				AF12 = 0b1100,
				AF13 = 0b1101,
				AF14 = 0b1110,
				AF15 = 0b1111,
			};
		};
	};  // GPIO::Reg


	struct Clock {
		struct Status {
			static constexpr u32 WIDTH = 1;
			/// See RM0440-7.4.15
			enum Value : u32 {
				DISABLED = 0b0,
				ENABLED  = 0b1,
			};
		};
	};


	/// @addtogroup GPIO_IO
	/// @{

	/**
	 * @brief Read the current value of a GPIO pin.
	 *
	 * Reads from IDR if the pin is configured as input, or ODR if output.
	 * Returns 0 for ANALOG and ALTERNATE modes.
	 *
	 * @param GPIO_Pin	The pin to read, specified by port and pin number.
	 * @return		1 if the pin is high, 0 if low or mode is unsupported.
	 *
	 * @note See RM0440-9.4.1 (MODER) and RM0440-9.4.5 (IDR/ODR).
	 */
	static u32 read(GPIO_Pin_t GPIO_Pin);

	/**
	 * @brief Drive a GPIO pin high.
	 *
	 * Writes to the BSRR set bits [15:0]. The hardware atomically sets the
	 * corresponding ODR bit without a read-modify-write.
	 *
	 * @param GPIO_Pin	The pin to set.
	 *
	 * @note See RM0440-9.4.7
	 */
	static void set(GPIO_Pin_t GPIO_Pin);

	/**
	 * @brief Drive a GPIO pin low.
	 *
	 * Writes to BSRR reset bits [31:16]. Atomic, no read-modify-write needed.
	 *
	 * @param GPIO_Pin	The pin to clear.
	 *
	 * @note See RM0440-9.4.7
	 */
	static void clear(GPIO_Pin_t GPIO_Pin);

	/// @}	endgroup GPIO_IO


	/// @addtogroup GPIO_Config
	/// @{

	/**
	 * @brief Set the mode of a GPIO pin.
	 *
	 * @param GPIO_Pin	  The target pin.
	 * @param mode_value  One of INPUT, OUTPUT, ALTERNATE, or ANALOG.
	 *
	 * @note See RM0440-9.4.1
	 */
	static void set_mode(GPIO_Pin_t GPIO_Pin, GPIO::Reg::MODER::Value mode);

	/**
	 * @brief Set the output type of a GPIO pin.
	 *
	 * @param GPIO_Pin    The target pin.
	 * @param otype       PUSH_PULL or OPEN_DRAIN.
	 *
	 * @note See RM0440-9.4.2
	 */
	static void set_otype(GPIO_Pin_t GPIO_Pin, GPIO::Reg::OTYPER::Value mode);

	/**
	 * @brief Set the output slew rate of a GPIO pin.
	 *
	 * @param GPIO_Pin    The target pin.
	 * @param ospeed      One of VERY_LOW_SPEED, LOW_SPEED, HIGH_SPEED, or
	 * VERY_HIGH_SPEED.
	 *
	 * @note See RM0440-9.4.3
	 */
	static void set_ospeed(GPIO_Pin_t GPIO_Pin, GPIO::Reg::OSPEEDR::Value ospeed);

	/**
	 * @brief Set the pull-up/pull-down configuration of a GPIO pin.
	 *
	 * @param GPIO_Pin    The target pin.
	 * @param pupd        One of NO_PULL_UP_DOWN, PULL_UP, or PULL_DOWN.
	 *
	 * @note See RM0440-9.4.4
	 */
	static void set_pupd(GPIO_Pin_t GPIO_Pin, GPIO::Reg::PUPDR::Value pupd);

	/**
	 * @brief Set the alternate function mapping of a GPIO pin.
	 *
	 * @param GPIO_Pin    The target pin.
	 * @param af          Alternate function selection (AF0–AF15).
	 *
	 * @note See RM0440-9.4.9 Pins 0–7 map to AFR[0], pins 8–15 to AFR[1].
	 */
	static void set_alternate_function(GPIO_Pin_t GPIO_Pin, GPIO::Reg::AFR::Value af);

	/// @}	endgroup GPIO_Config


	/// @addtogroup GPIO_Port
	/// @{

	/**
	 * @brief Enable or disable the clock for a GPIO port.
	 *
	 * Controls the corresponding bit in RCC->AHB2ENR. The port clock must be
	 * enabled before any register on that port can be accessed.
	 *
	 * @param port          GPIO port base pointer (GPIOA–GPIOG).
	 * @param clock_status  Set the port clock to ENABLED or DISABLED.
	 *
	 * @note See RM0440-7.4.15.
	 */
	static void set_port_clock(GPIO_TypeDef* port, GPIO::Clock::Status::Value clock_status);

	/// @}	endgroup GPIO_Port


private:
};
}  // namespace Chip

