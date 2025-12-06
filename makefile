CFLAGS := -Wall

SRC_DIR := ./src
BUILD_DIR := ./build

clean:
	rm -rf $(BUILD_DIR)

# Help target
help:
	@printf "\n\033[1;34mAvailable Test Commands:\033[0m\n\n"
	@printf "  \033[1;32mmake test\033[0m           - Run all unit tests (compact output)\n"
	@printf "  \033[1;32mmake test-verbose\033[0m   - Run all unit tests (detailed output)\n"
	@printf "  \033[1;32mmake test-single\033[0m    - Run single test (e.g., make test:single TEST=array_test)\n"
	@printf "  \033[1;32mmake clean\033[0m          - Clean build artifacts\n\n"

# Ckester - This includes some testing targets for your makefile
include /Users/emi/Desktop/sws/ckastal/vendor/ckester/include/makefile
