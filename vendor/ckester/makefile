CKESTER_MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ckester: $(CKESTER_MAKEFILE_DIR)src/ckester_cli.c
	@mkdir -p $(CKESTER_MAKEFILE_DIR)build
	@$(CC) $(CCFLAGS) $(CKESTER_MAKEFILE_DIR)src/ckester_cli.c -o $(CKESTER_MAKEFILE_DIR)build/ckester
	@touch makefile
	@if ! grep -q "include $$(realpath $(CKESTER_MAKEFILE_DIR)include/makefile)" makefile; then \
		echo "\n# Ckester - This includes some testing targets for your makefile" >> makefile; \
		echo "include $$(realpath $(CKESTER_MAKEFILE_DIR)include/makefile)" >> makefile; \
		echo "Added ckester include to makefile"; \
	else \
		echo "Ckester include already exists in makefile"; \
	fi
