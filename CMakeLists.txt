cmake_minimum_required(VERSION 3.23)
project(STM32G4_Module_Library C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Compiling for embedded")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Error if C++17 unavailable, don't silently fall back
set(CMAKE_CXX_EXTENSIONS OFF)           # Use -std=c++17, not -std=gnu++17

find_program(OBJCOPY arm-none-eabi-objcopy REQUIRED)

# Configurable parameters
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(OPT_FLAG -Os)
set(DEBUG_FLAG -g -DLOG_LEVEL=5)
set(STACK_MAX_BYTES 512)
set(MCU STM32G491xx)

# Warning flags
list(APPEND WARNING_FLAGS
        -Wall -Wextra -Werror
        -Wundef -Wconversion                                 # Warnings for undefined symbols, implicit conversions
        -Wshadow -fno-common                                 # Warnings for shadowing names, duplicate global symbols
        -Wdouble-promotion                                   # Warnings for using doubles instead of floats
        -Wformat=2                                           # Warnings for print formatting
        )

# GCC-only warning flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND WARNING_FLAGS
            -Wformat-overflow -Wformat-truncation           # Warnings for string buffer formatting
            -fstack-usage -Wstack-usage=${STACK_MAX_BYTES}  # Warnings for stack overflows
            )
endif()

list(APPEND MCU_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16                                   # Required for hard-float ABI on Cortex-M4
        )

list(APPEND COMPILER_LINKER_FLAGS
        -ffunction-sections -fdata-sections                 # Dead code elimination
        -fshort-enums
        )

list(APPEND LINKER_FLAGS
        -T${CMAKE_SOURCE_DIR}/include/stm32/stm32_flash.ld
        -Wl,--gc-sections
        -Wl,-z,noexecstack                                  # Suppress executable stack warning
        -Wl,--no-warn-rwx-segments                          # Suppress rwx segment warning
        # -Wl,--print-gc-sections                           # Dead code elimination and verbose output
        )

# GCC/newlib-specific linker flag
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND LINKER_FLAGS --specs=nosys.specs)
endif()

add_executable(${EXECUTABLE}
        # Debug
        include/segger/SEGGER_RTT.c
        include/segger/SEGGER_RTT_printf.c
        # Drivers
        include/stm32/startup_stm32g491xx.s
        include/stm32/system_stm32g4xx.c
        # Chip
        src/lib/chip/stm32g491/stm32g491_chip.cc
        src/lib/chip/stm32g491/stm32g491_hal.cc
        # Peripherals
        src/lib/peripherals/gpio/gpio.cc
        # Source
        src/main.cc
        )

target_include_directories(${EXECUTABLE} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/stm32
        ${CMAKE_SOURCE_DIR}/include/segger
        )

target_compile_definitions(${EXECUTABLE} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX>:${MCU}>
        )

target_compile_options(${EXECUTABLE} PRIVATE
        # MCU flags apply to all languages including ASM
        ${MCU_FLAGS}
        # Everything else only applies to C and C++
        $<$<COMPILE_LANGUAGE:C,CXX>:
            ${OPT_FLAG}
            ${DEBUG_FLAG}
            ${COMPILER_LINKER_FLAGS}
            ${WARNING_FLAGS}
        >
        )

target_link_options(${EXECUTABLE} PRIVATE
        ${DEBUG_FLAG}
        ${MCU_FLAGS}
        ${LINKER_FLAGS}
        )

set(ELF_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET "${EXECUTABLE}" POST_BUILD
        COMMAND ${OBJCOPY} -Oihex   ${ELF_FILE} ${HEX_FILE}
        COMMAND ${OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
        )

