#!/bin/bash
# Generates lcov code coverage
# Run from ../scripts/

BUILD_DIR='cmake-build-test'
PROJECT_NAME='STM32G4_Module_Library'

lcov --capture --directory ${BUILD_DIR}/CMakeFiles/${PROJECT_NAME}-run_tests.out.dir --output-file ${BUILD_DIR}/coverage.info
lcov --remove ${BUILD_DIR}/coverage.info '/usr/include/*' '*/src/catch2/*' -o ${BUILD_DIR}/coverage.info
genhtml ${BUILD_DIR}/coverage.info --output-directory ${BUILD_DIR}/coverage
