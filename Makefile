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
debug: bin/tests-debug objs/src/main-debug.o

main: objs/src/main.o
tests: bin/tests

amalgamate:
	(which python > /dev/null && python tools/amalgamate.py) || (which python3 > /dev/null && python3 tools/amalgamate.py)

$(BIN_DIR)/tests-debug: $(DEBUG_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $^ -o $@

$(BIN_DIR)/tests: $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%-debug.o: ./%.c
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

$(OBJ_DIR)/%.o: ./%.c
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/* amalgamate/*

.DEFAULT_GOAL = main
