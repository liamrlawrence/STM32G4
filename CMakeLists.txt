CMAKE_MINIMUM_REQUIRED(VERSION 3.23)
PROJECT(STM32G4_Module_Library)
OPTION(UNIT_TEST "Run unit tests" OFF)


IF (UNIT_TEST MATCHES ON)
	MESSAGE(STATUS "Compiling unit tests")
	SET(CMAKE_CXX_STANDARD 17)
	SET(CMAKE_CXX_COMPILER "g++" CACHE STRING "" FORCE)

	INCLUDE(FetchContent)
	FETCHCONTENT_DECLARE(
			Catch2
			GIT_REPOSITORY https://github.com/catchorg/Catch2.git
			GIT_TAG v3.0.1 # or a later release
	)
	FETCHCONTENT_MAKEAVAILABLE(Catch2)

	SET(EXECUTABLE ${PROJECT_NAME}-run_tests.out)
	SET(OPT_FLAG "-Og")
	SET(DEBUG_FLAG "-g")

	LIST(APPEND WARNING_FLAGS
			-Wall -Wextra -Werror
			-Wundef -Wconversion                                # Warnings for Undefined symbols, Implicit conversions
			-Wshadow -fno-common                                # Warnings for Shadowing names, Duplicate global symbols
			-Wdouble-promotion                                  # Warnings for Using Doubles instead of Floats
			-Wformat-overflow -Wformat-truncation -Wformat=2    # Warnings for String buffers & Print formatting
			)

	ADD_EXECUTABLE(${EXECUTABLE}
			# Chip
			src/lib/chip/stm32g491/stm32g491_mock.hh
			src/lib/chip/stm32g491/stm32g491_mock.cc
			src/lib/chip/stm32g491/stm32g491_chip.hh
			src/lib/chip/stm32g491/stm32g491_chip.cc
			src/lib/chip/stm32g491/stm32g491_hal.cc
			src/lib/chip/stm32g491/test/utest_stm32g491_hal.cc

			# Peripherals
			src/lib/peripherals/gpio/gpio.hh
			src/lib/peripherals/gpio/gpio.cc
			src/lib/peripherals/gpio/test/utest_gpio.cc
			)

	TARGET_COMPILE_OPTIONS(${EXECUTABLE} PRIVATE
			# Enable the UNIT_TEST flag
			-DUNIT_TEST

			# Compiler options
			${OPT_FLAG} ${DEBUG_FLAG}

			# Warning flags
			${WARNING_FLAGS}

			# Code coverage flags
			-fprofile-arcs -ftest-coverage
			)

	TARGET_LINK_LIBRARIES(${EXECUTABLE} PRIVATE Catch2::Catch2WithMain)
	TARGET_LINK_LIBRARIES(${EXECUTABLE} PRIVATE gcov)


ELSE ()
	ENABLE_LANGUAGE(ASM C CXX)
	MESSAGE(STATUS "Compiling for embedded")
	SET(CMAKE_CXX_STANDARD 17)
	SET(CMAKE_CXX_COMPILER "/usr/bin/arm-none-eabi-g++")
	SET(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
	SET(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-gcc")
	SET(OBJCOPY "arm-none-eabi-objcopy")


	# Configurable parameters
	SET(EXECUTABLE ${PROJECT_NAME}.elf)
	SET(OPT_FLAG "-Os")
	SET(DEBUG_FLAG "-g")
	SET(STACK_MAX_BYTES "512")
	SET(MCU "STM32G491xx")


	LIST(APPEND WARNING_FLAGS
			-Wall -Wextra -Werror
			-Wundef -Wconversion                                # Warnings for Undefined symbols, Implicit conversions
			-Wshadow -fno-common                                # Warnings for Shadowing names, Duplicate global symbols
			-Wdouble-promotion                                  # Warnings for Using Doubles instead of Floats
			-Wformat-overflow -Wformat-truncation -Wformat=2    # Warnings for String buffers & Print formatting
			-fstack-usage -Wstack-usage=${STACK_MAX_BYTES}      # Warnings for stack overflows
			)

	LIST(APPEND MCU_FLAGS
			-D${MCU}
			-mcpu=cortex-m4
			-mthumb
			-mfloat-abi=hard
			)

	LIST(APPEND COMPILER_LINKER_FLAGS
			-ffunction-sections -fdata-sections                 # Dead code elimination and verbose output
			)

	LIST(APPEND LINKER_FLAGS
			-T${CMAKE_SOURCE_DIR}/include/stm32_flash.ld
			-Wl,--gc-sections -Wl,--print-gc-sections           # Dead code elimination and verbose output
			--specs=nosys.specs
			)

	ADD_EXECUTABLE(${EXECUTABLE}
			# Drivers
			include/startup_stm32g491xx.s
			include/core_cm4.h
			include/stm32g4xx.h
			include/stm32g491xx.h
			include/system_stm32g4xx.h
			include/system_stm32g4xx.c

			# Chip
			src/lib/chip/stm32g491/stm32g491_chip.hh
			src/lib/chip/stm32g491/stm32g491_chip.cc
			src/lib/chip/stm32g491/stm32g491_hal.cc

			# Peripherals
			src/lib/peripherals/gpio/gpio.hh
			src/lib/peripherals/gpio/gpio.cc

			# Source
			src/main.cc)

	TARGET_COMPILE_OPTIONS(${EXECUTABLE} PRIVATE
			# Compiler options
			${OPT_FLAG} ${DEBUG_FLAG}

			# Linker options
			${COMPILER_LINKER_FLAGS}

			# Warning flags
			${WARNING_FLAGS}

			# MCU flags
			${MCU_FLAGS}
			)


	TARGET_LINK_OPTIONS(${EXECUTABLE} PRIVATE
			${DEBUG_FLAG}
			${MCU_FLAGS}
			${LINKER_FLAGS}
			)

	SET(ELF_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf)
	SET(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
	SET(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
	ADD_CUSTOM_COMMAND(TARGET "${EXECUTABLE}" POST_BUILD
			COMMAND ${OBJCOPY} -Oihex ${ELF_FILE} ${HEX_FILE}
			COMMAND ${OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
			)
ENDIF ()
