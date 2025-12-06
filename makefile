CFLAGS := -Wall

SRC_DIR := ./src
BUILD_DIR := ./build
TEST_UNIT_DIR := ./test/suite/unit

TEST_SRCS := $(wildcard $(TEST_UNIT_DIR)/*.c)
UNIT_TEST_BINS := $(patsubst $(TEST_UNIT_DIR)/%.c,$(BUILD_DIR)/$(TEST_UNIT_DIR)/%,$(TEST_SRCS))

# MODE normalization
MODE ?= $(M)
MODE := $(MODE)

ifeq ($(MODE),)
  MODE := DEFAULT
endif
ifeq ($(MODE),V)
  MODE := VERBOSE
endif
ifeq ($(MODE),D)
  MODE := DEFAULT
endif
ifeq ($(MODE),S)
  MODE := SILENT
endif

# Allowed modes: VERBOSE DEFAULT SILENT
VALID_MODES := VERBOSE DEFAULT SILENT

# Error if MODE is invalid
ifeq (,$(filter $(MODE),$(VALID_MODES)))
$(error Invalid MODE '$(MODE)'. Valid values are: V, VERBOSE, D, DEFAULT, S, SILENT)
endif

# Mode booleans
ifeq ($(MODE),VERBOSE)
  MODE_VERBOSE := 1
endif
ifeq ($(MODE),SILENT)
  MODE_SILENT := 1
endif

.PHONY: test test\:single clean help

$(BUILD_DIR)/$(TEST_UNIT_DIR):
	@mkdir -p $(BUILD_DIR)/$(TEST_UNIT_DIR)

$(BUILD_DIR)/$(TEST_UNIT_DIR)/%: $(TEST_UNIT_DIR)/%.c | $(BUILD_DIR)/$(TEST_UNIT_DIR)
	@printf "\033[1;35m[BUILD]\033[0m Compiling %s\n" "$<"
	@$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

test: $(UNIT_TEST_BINS)
	@printf "\n\033[1;34m========================================\n"
	@printf "   Ckastal Test Runner — Mode: $(MODE)\n";
	@printf "========================================\033[0m\n\n"
	@fail=0; \
	passed=0; \
	total=0; \
	for bin in $(UNIT_TEST_BINS); do \
		total=$$((total + 1)); \
		test_src=$$(echo "$$bin" | sed 's|^$(BUILD_DIR)/||'); \
		if [ -z "$(MODE_SILENT)" ]; then \
			printf "\033[1;36m[BIN]\033[0m %s\n" "$$(realpath "$$bin")"; \
			printf "\033[1;36m[SRC]\033[0m %s\n" "$$(realpath "$$test_src.c")"; \
		fi; \
		if [ -n "$(MODE_VERBOSE)" ]; then \
			out="$$(./$$bin --verbose 2>&1)"; \
		else \
			out="$$(./$$bin 2>&1)"; \
		fi; \
		if [ -z "$(MODE_SILENT)" ]; then \
			printf "%s\n" "$$out" | sed 's/^/  /'; \
		fi; \
		if printf "%s" "$$out" | grep -q "All tests passed"; then \
			passed=$$((passed + 1)); \
			if [ -z "$(MODE_SILENT)" ]; then \
				printf "\033[1;32m  ✓ Passed\033[0m\n\n"; \
			fi; \
		else \
			fail=1; \
			if [ -z "$(MODE_SILENT)" ]; then \
				printf "\033[1;31m  ✗ Failed\033[0m\n\n"; \
			fi; \
		fi; \
	done; \
	printf "\n"; \
	if [ $$fail -eq 0 ]; then \
		printf "\033[1;32m  ✓ Passed $$passed/$$total test suites\033[0m\n"; \
	else \
		printf "\033[1;31m  ✗ Failed $$((total - passed))/$$total test suites\033[0m\n"; \
		exit 1; \
	fi

test\:single:
	@if [ -z "$(TEST)" ]; then \
		echo "\033[1;31m[ERROR]\033[0m Please specify TEST variable"; \
		echo "Usage: make test:single TEST=lenstr_test"; \
		exit 1; \
	fi
	@TEST_NAME=$$(basename $(TEST)); \
	TEST_SRC=$(TEST_UNIT_DIR)/$$TEST_NAME.c; \
	TEST_BIN=$(BUILD_DIR)/$(TEST_UNIT_DIR)/$$TEST_NAME; \
	mkdir -p $(BUILD_DIR)/$(TEST_UNIT_DIR); \
	\
	if [ ! -f $$TEST_SRC ]; then \
		echo "\033[1;31m[ERROR]\033[0m Test file $$TEST_SRC not found"; \
		exit 1; \
	fi; \
	\
	if [ ! -f $$TEST_BIN ] || [ $$TEST_SRC -nt $$TEST_BIN ]; then \
		printf "\033[1;35m[BUILD]\033[0m Compiling $$TEST_SRC\n"; \
		$(CC) $(CFLAGS) $(CPPFLAGS) $$TEST_SRC -o $$TEST_BIN; \
	fi; \
	printf "\n\033[1;34m========================================\n"; \
	printf "   Ckastal Test Runner — Mode: $(MODE)\n"; \
	printf "========================================\033[0m\n\n"; \
	if [ -z "$(MODE_SILENT)" ]; then \
		printf "\033[1;36m[BIN]\033[0m $$(realpath "$$TEST_BIN")\n"; \
		printf "\033[1;36m[SRC]\033[0m $$(realpath "$$TEST_SRC")\n"; \
	fi; \
	if [ -n "$(MODE_VERBOSE)" ]; then \
		out="$$(./$$TEST_BIN --verbose 2>&1)"; \
	else \
		out="$$(./$$TEST_BIN 2>&1)"; \
	fi; \
	if [ -z "$(MODE_SILENT)" ]; then \
		printf "%s\n" "$$out" | sed 's/^/  /'; \
	fi; \
	if printf "%s" "$$out" | grep -q "All tests passed"; then \
		if [ -z "$(MODE_SILENT)" ]; then \
			printf "\033[1;32m\n  ✓ Passed\033[0m\n\n"; \
		else \
			printf "\033[1;32m✓ Passed\033[0m\n"; \
		fi; \
	else \
		if [ -z "$(MODE_SILENT)" ]; then \
			printf "\033[1;31m\n  ✗ Failed\033[0m\n\n"; \
		else \
			printf "\033[1;31m✗ Failed\033[0m\n"; \
		fi; \
		exit 1; \
	fi

clean:
	rm -rf $(BUILD_DIR)

# Help target
help:
	@printf "\n\033[1;34mAvailable Test Commands:\033[0m\n\n"
	@printf "  \033[1;32mmake test\033[0m           - Run all unit tests (compact output)\n"
	@printf "  \033[1;32mmake test:verbose\033[0m   - Run all unit tests (detailed output)\n"
	@printf "  \033[1;32mmake test:single\033[0m    - Run single test (e.g., make test:single TEST=array_test)\n"
	@printf "  \033[1;32mmake test:watch\033[0m     - Run tests on file changes (requires entr)\n"
	@printf "  \033[1;32mmake clean\033[0m          - Clean build artifacts\n\n"
