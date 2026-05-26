.PHONY: all clean docs build configure

BUILD_DIR := build
TOOLCHAIN_FILE := arm-toolchain.cmake

configure:
	@if [ -d "$(BUILD_DIR)" ]; then \
		cmake -B $(BUILD_DIR); \
	else \
		cmake -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_FILE); \
	fi

build: configure
	cmake --build $(BUILD_DIR)

docs:
	doxygen Doxyfile
	sphinx-build -a -E -b html docs docs/_build
	@echo "Documentation generated at: docs/_build/index.html"

clean:
	rm -rf $(BUILD_DIR) docs/doxygen docs/_build
	@echo "Cleaned."

