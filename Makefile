.PHONY: all clean docs build configure run

BUILD_DIR := build
TOOLCHAIN_FILE := arm-toolchain.cmake
JLINK_DEVICE := STM32G491RE
JLINK_SCRIPT := flash.jlink

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


run: $(ELF_FILE) build
	JLinkExe -device $(JLINK_DEVICE) -if SWD -speed 4000 -autoconnect 1 -commanderscript $(JLINK_SCRIPT)

