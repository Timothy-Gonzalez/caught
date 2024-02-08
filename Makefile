# Constants
CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter -Wmissing-declarations -Wmissing-variable-declarations
CFLAGS_COMMON = $(WARNINGS) -std=c99
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -gdwarf-4 -DDEBUG

SRC_DIR = src
TEST_DIR = tests
AMALGAMATE_DIRS = ./amalgamate/test/src/ ./amalgamate/test/tests/
OBJ_DIR = objs
BIN_DIR = bin

C_FILES := $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c')
H_FILES := $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.h')

.PHONY: all tests clean amalgamate bump-version

all: tests debug amalgamate
debug: bin/tests-debug bin/tests-amalgamate-debug
tests: bin/tests bin/tests-amalgamate

amalgamate:
	(which python > /dev/null && python tools/amalgamate.py) || (which python3 > /dev/null && python3 tools/amalgamate.py)
	rm -rf ./amalgamate/test/src/ ./amalgamate/test/tests/
	mkdir -p ./amalgamate/test/src/
	cp ./amalgamate/caught.h ./amalgamate/caught.c ./amalgamate/test/src/
	cp -r ./tests ./amalgamate/test/

bump-version:
	(which python > /dev/null && python tools/bump-version.py) || (which python3 > /dev/null && python3 tools/bump-version.py)

$(BIN_DIR)/tests-debug: $(C_FILES) $(H_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS_DEBUG) $(C_FILES) -o $@

$(BIN_DIR)/tests: $(C_FILES) $(H_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS_RELEASE) $(C_FILES) -o $@

$(BIN_DIR)/tests-amalgamate-debug: amalgamate $(C_FILES) $(H_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS_DEBUG) $$(find $(AMALGAMATE_DIRS) -name '*.c') -o $@

$(BIN_DIR)/tests-amalgamate: amalgamate $(C_FILES) $(H_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS_RELEASE) $$(find $(AMALGAMATE_DIRS) -name '*.c') -o $@

clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/* amalgamate/*

.DEFAULT_GOAL = all
