# Convenience wrapper around the CMake build.
#
# This Makefile only forwards to CMake, so Linux/macOS terminals (and MSYS/MinGW
# make on Windows) all build identically. Visual Studio and VS Code use
# CMakePresets.json instead - see README.md.
#
#   make            configure + build (Release)
#   make test       build + run the automated tests
#   make run        build + launch the application
#   make clean      remove the build directory
#   make CONFIG=Debug   pick a different build type

CMAKE     ?= cmake
BUILD_DIR ?= build
CONFIG    ?= Release

.PHONY: all configure build test run clean rebuild help

all: build

configure:
	$(CMAKE) -B $(BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=$(CONFIG)

build: configure
	$(CMAKE) --build $(BUILD_DIR)

test: build
	$(CMAKE) -E chdir $(BUILD_DIR) ctest --output-on-failure

run: build
	./$(BUILD_DIR)/UASM-Tool

clean:
	$(CMAKE) -E rm -rf $(BUILD_DIR)

rebuild: clean build

help:
	@echo "Available targets:"
	@echo "  all/build - Configure and build (default, CONFIG=$(CONFIG))"
	@echo "  test      - Build and run the automated tests"
	@echo "  run       - Build and launch the application"
	@echo "  clean     - Remove the build directory"
	@echo "  rebuild   - Clean and build again"
	@echo "  help      - Display this help message"
