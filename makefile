CFLAGS := -Wall

SRC_DIR := ./src
BUILD_DIR := ./build
TEST_UNIT_DIR := ./test/suite/unit

TEST_SRCS := $(wildcard $(TEST_UNIT_DIR)/*.c)
UNIT_TEST_BINS := $(patsubst $(TEST_UNIT_DIR)/%.c,$(BUILD_DIR)/$(TEST_UNIT_DIR)/%,$(TEST_SRCS))

.PHONY: test clean

$(BUILD_DIR)/$(TEST_UNIT_DIR)/%: $(TEST_UNIT_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

test: $(UNIT_TEST_BINS)
	@printf "\n\033[1;34m============================\n"
	@printf "   Running Unit Tests\n"
	@printf "============================\033[0m\n\n"

	@fail=0; \
	for bin in $(UNIT_TEST_BINS); do \
		printf "\033[1;36m[INFO]\033[0m Running test: \033[1m$$bin\033[0m\n"; \
		if ./$$bin 2>&1 | sed 's/^/      [TEST_OUTPUT]: /'; then \
			printf "      → \033[1;32mPASS\033[0m\n\n"; \
		else \
			printf "      → \033[1;31mFAIL\033[0m\n\n"; \
			fail=1; \
		fi; \
	done; \
	if [ $$fail -eq 0 ]; then \
		printf "\033[1;32mAll tests passed ✔\033[0m\n"; \
	else \
		printf "\033[1;31mSome tests failed ✖\033[0m\n"; \
		exit 1; \
	fi

clean:
	rm -rf $(BUILD_DIR)
