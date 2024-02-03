# Constants
CC = clang
CFLAGS = -Wall -std=c99
CFLAGS_DEBUG = -g

SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = objs
BIN_DIR = bin

C_FILES := $(shell find . -name '*.c')
H_FILES := $(shell find . -name '*.h')

OBJ_FILES := $(C_FILES:%.c=$(OBJ_DIR)/%.o)
DEBUG_OBJ_FILES := $(OBJ_FILES:%.o=%-debug.o)

.PHONY: all tests clean amalgamate

all: tests main amalgamate
debug: bin/tests-debug

lib: echo TODO
tests: bin/tests

amalgamate:
	(which python > /dev/null && python tools/amalgamate.py) || (which python3 > /dev/null && python3 tools/amalgamate.py)

$(BIN_DIR)/tests-debug: $(C_FILES) $(H_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(C_FILES) -o $@

$(BIN_DIR)/tests: $(C_FILES) $(H_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(C_FILES) -o $@

clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/* amalgamate/*

.DEFAULT_GOAL = lib
