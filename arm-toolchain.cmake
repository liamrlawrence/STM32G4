# arm-toolchain.cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(APPLE)
    execute_process(
        COMMAND brew --prefix llvm
        OUTPUT_VARIABLE LLVM_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
elseif(UNIX)
    set(LLVM_PREFIX "/usr")
elseif(WIN32)
    message(FATAL_ERROR "Windows builds are not supported")
endif()

set(CMAKE_C_COMPILER   "${LLVM_PREFIX}/bin/clang")
set(CMAKE_CXX_COMPILER "${LLVM_PREFIX}/bin/clang++")
set(CMAKE_ASM_COMPILER "${LLVM_PREFIX}/bin/clang")

# Prevent CMake from failing to link a test executable during configuration
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Sysroot and include paths from arm-none-eabi-gcc installation
execute_process(
    COMMAND arm-none-eabi-gcc -print-sysroot
    OUTPUT_VARIABLE ARM_SYSROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND arm-none-eabi-gcc -print-file-name=include
    OUTPUT_VARIABLE ARM_GCC_INCLUDE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND arm-none-eabi-gcc -dumpversion
    OUTPUT_VARIABLE ARM_GCC_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# C++ headers live in a versioned subdir under the sysroot
set(ARM_CXX_INCLUDE        "${ARM_SYSROOT}/include/c++/${ARM_GCC_VERSION}")
# Target-specific C++ headers (e.g. <bits/c++config.h>)
set(ARM_CXX_INCLUDE_TARGET "${ARM_CXX_INCLUDE}/arm-none-eabi")

set(CMAKE_SYSROOT ${ARM_SYSROOT})

# Use arm-none-eabi-gcc as the linker driver instead of clang++
# This avoids Clang's incorrect runtime library assumptions (libgcc_eh, libgcc_s, etc.)
execute_process(
    COMMAND which arm-none-eabi-gcc
    OUTPUT_VARIABLE ARM_GCC
    OUTPUT_STRIP_TRAILING_WHITESPACE
)


set(CMAKE_CXX_LINK_EXECUTABLE
    "${ARM_GCC} <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_C_FLAGS_INIT
    "--target=arm-none-eabi --sysroot=${ARM_SYSROOT}")
set(CMAKE_CXX_FLAGS_INIT
    "--target=arm-none-eabi --sysroot=${ARM_SYSROOT} -isystem ${ARM_GCC_INCLUDE} -isystem ${ARM_CXX_INCLUDE} -isystem ${ARM_CXX_INCLUDE_TARGET} -stdlib=libstdc++ -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables")
set(CMAKE_ASM_FLAGS_INIT
    "--target=arm-none-eabi")

