CFLAGS := -Wall

SRC_DIR := ./src
BUILD_DIR := ./build
TEST_UNIT_DIR := ./test/suite/unit

TEST_SRCS := $(wildcard $(TEST_UNIT_DIR)/*.c)
UNIT_TEST_BINS := $(patsubst $(TEST_UNIT_DIR)/%.c,$(BUILD_DIR)/$(TEST_UNIT_DIR)/%,$(TEST_SRCS))

VERBOSE ?= $(V)

.PHONY: test test\:verbose test\:single/% clean help

# Create build directories
$(BUILD_DIR)/$(TEST_UNIT_DIR):
	@mkdir -p $(BUILD_DIR)/$(TEST_UNIT_DIR)

# Compile test binaries
$(BUILD_DIR)/$(TEST_UNIT_DIR)/%: $(TEST_UNIT_DIR)/%.c | $(BUILD_DIR)/$(TEST_UNIT_DIR)
	@printf "\033[1;35m[BUILD]\033[0m Compiling %s\n" "$<"
	@$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

# Default test target (compact output - shows bin/src but not individual test statements)
test: $(UNIT_TEST_BINS)
	@printf "\n\033[1;34m============================\n"
	@printf "   Running Unit Tests\n"
	@printf "============================\033[0m\n\n"
	@fail=0; \
	passed=0; \
	total=0; \
	for bin in $(UNIT_TEST_BINS); do \
		total=$$((total + 1)); \
		test_src=$$(echo $$bin | sed 's|$(BUILD_DIR)/||'); \
		printf "\033[1;36m[BIN]\033[0m $$(realpath "$$bin")\n"; \
		printf "\033[1;36m[SRC]\033[0m $$(realpath "$$test_src.c")\n"; \
		if ./$$bin 2>&1 | grep -q "All tests passed"; then \
			passed=$$((passed + 1)); \
			printf "\033[1;32m  ✓ Passed\033[0m\n\n"; \
		else \
			fail=1; \
			printf "\033[1;31m  ✗ Failed\033[0m\n\n"; \
		fi; \
	done; \
	if [ $$fail -eq 0 ]; then \
		printf "\033[1;32m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
		printf "\033[1;32m  ✓ All $$total test suites passed\033[0m\n"; \
		printf "\033[1;32m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
	else \
		printf "\033[1;31m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
		printf "\033[1;31m  ✗ $$((total - passed))/$$total test suites failed\033[0m\n"; \
		printf "\033[1;31m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
		exit 1; \
	fi

# Verbose test target (detailed output - shows bin/src and all test statements)
test\:verbose: $(UNIT_TEST_BINS)
	@printf "\n\033[1;34m============================\n"
	@printf "   Running Unit Tests\n"
	@printf "   (Verbose Mode)\n"
	@printf "============================\033[0m\n\n"
	@fail=0; \
	passed=0; \
	total=0; \
	for bin in $(UNIT_TEST_BINS); do \
		total=$$((total + 1)); \
		test_src=$$(echo $$bin | sed 's|$(BUILD_DIR)/||'); \
		printf "\033[1;36m[BIN]\033[0m $$(realpath "$$bin")\n"; \
		printf "\033[1;36m[SRC]\033[0m $$(realpath "$$test_src").c\n"; \
		if ./$$bin --verbose 2>&1 | sed 's/^/  /'; then \
			passed=$$((passed + 1)); \
			printf "\n"; \
		else \
			fail=1; \
			printf "\n"; \
		fi; \
	done; \
	if [ $$fail -eq 0 ]; then \
		printf "\033[1;32m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
		printf "\033[1;32m  ✓ All $$total test suites passed\033[0m\n"; \
		printf "\033[1;32m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
	else \
		printf "\033[1;31m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
		printf "\033[1;31m  ✗ $$((total - passed))/$$total test suites failed\033[0m\n"; \
		printf "\033[1;31m━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n"; \
		exit 1; \
	fi

test\:single:
	@TEST_NAME=$$(basename $(TEST)); \
	TEST_SRC=$(TEST_UNIT_DIR)/$$TEST_NAME.c; \
	TEST_BIN=$(BUILD_DIR)/$(TEST_UNIT_DIR)/$$TEST_NAME; \
	mkdir -p $(BUILD_DIR)/$(TEST_UNIT_DIR); \
	if [ ! -f $$TEST_SRC ]; then \
		echo "\033[1;31m[ERROR]\033[0m Test file $$TEST_SRC not found"; \
		exit 1; \
	fi; \
	if [ ! -f $$TEST_BIN ] || [ $$TEST_SRC -nt $$TEST_BIN ]; then \
		printf "\033[1;35m[BUILD]\033[0m Compiling $$TEST_SRC\n"; \
		$(CC) $(CFLAGS) $(CPPFLAGS) $$TEST_SRC -o $$TEST_BIN; \
	fi; \
	bin_clean=$(echo $$TEST_BIN | sed 's|^\./||'); \
	src_clean=$(echo $$TEST_SRC | sed 's|^\./||'); \
	printf "\n\033[1;34m============================\n"; \
	printf "   Running Single Test\n"; \
	printf "============================\033[0m\n\n"; \
	printf "\033[1;36m[BIN]\033[0m $$(realpath "$$TEST_BIN")\n"; \
	printf "\033[1;36m[SRC]\033[0m $$(realpath "$$TEST_SRC")\n"; \
	if [ -n "${VERBOSE}" ]; then \
		./$$TEST_BIN --verbose 2>&1 | sed 's/^/  /'; \
	else \
		./$$TEST_BIN 2>&1 | sed 's/^/  /'; \
	fi; \
	printf "\n"

clean:
	rm -rf $(BUILD_DIR)

# Help target
help:
	@printf "\n\033[1;34mAvailable Test Commands:\033[0m\n\n"
	@printf "  \033[1;32mmake test\033[0m           - Run all tests (compact output)\n"
	@printf "  \033[1;32mmake test:verbose\033[0m   - Run all tests (detailed output)\n"
	@printf "  \033[1;32mmake test:single\033[0m    - Run single test (e.g., make test:single TEST=array_test)\n"
	@printf "  \033[1;32mmake test:watch\033[0m     - Run tests on file changes (requires entr)\n"
	@printf "  \033[1;32mmake clean\033[0m          - Clean build artifacts\n\n"
