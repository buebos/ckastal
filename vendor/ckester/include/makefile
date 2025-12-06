# Makefile wrapper for ckester test runner

.PHONY: test test-all test-single test-clean help

CKESTER_INCLUDE_MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
CKESTER_BIN := $(CKESTER_INCLUDE_MAKEFILE_DIR)../build/ckester

include $(CKESTER_INCLUDE_MAKEFILE_DIR)../makefile

# Build directory (can be overridden: make test CKESTER_BUILD_DIR=./custom_build)
CKESTER_BUILD_DIR := $(CKESTER_INCLUDE_MAKEFILE_DIR)../build

# CKESTER_MODE normalization
CKESTER_MODE ?= $(M)
CKESTER_MODE := $(CKESTER_MODE)

ifeq ($(CKESTER_MODE),)
  CKESTER_MODE_FLAG = --default
endif
ifeq ($(CKESTER_MODE),V)
  CKESTER_MODE_FLAG = --verbose
endif
ifeq ($(CKESTER_MODE),D)
  CKESTER_MODE_FLAG = --default
endif
ifeq ($(CKESTER_MODE),S)
  CKESTER_MODE_FLAG = --silent
endif

# Default test target - runs all tests
test: ckester
	@$(CKESTER_BIN) unit $(CKESTER_MODE_FLAG)

# Run all tests
test-all: ckester
	@$(CKESTER_BIN) all $(CKESTER_MODE_FLAG)

test-unit: ckester
	@$(CKESTER_BIN) unit $(CKESTER_MODE_FLAG)
test-integration: ckester
	@$(CKESTER_BIN) integration $(CKESTER_MODE_FLAG)
test-system: ckester
	@$(CKESTER_BIN) system $(CKESTER_MODE_FLAG)

# Run a specific test (usage: make test-single NAME=test_name)
test-single: ckester
	@if [ -z "$(NAME)" ]; then \
		echo "Error: Please specify NAME=<test_name>"; \
		echo "Example: make test-single NAME=array_test"; \
		exit 1; \
	fi
	@$(CKESTER_BIN) single $(NAME) $(CKESTER_MODE_FLAG)

# Clean build artifacts
test-clean: ckester
	@$(CKESTER_BIN) clean

# Show help
test-help: ckester
	@$(CKESTER_BIN) help
	@echo ""
	@echo "Makefile Usage:"
	@echo "  make test                    Run all tests (default mode)"
	@echo "  make test CKESTER_MODE=verbose       Run all tests with verbose output"
	@echo "  make test CKESTER_MODE=silent        Run all tests in silent mode"
	@echo "  make test-single NAME=<name> Run a specific test"
	@echo "  make test-single NAME=<name> CKESTER_MODE=verbose"
	@echo "  make test CKESTER_BUILD_DIR=./path   Use custom build directory"
	@echo "  make test-clean              Clean build artifacts"
