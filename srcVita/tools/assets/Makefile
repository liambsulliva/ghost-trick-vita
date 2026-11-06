SRC_VITA_FOLDER := ../..
TOOLS_BUILD_FOLDER := $(SRC_VITA_FOLDER)/build/tools
TESTS_FOLDER := $(SRC_VITA_FOLDER)/tests/tools

CC := clang
CFLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -O2 -Wall -Wextra
SANITIZER_FLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -g -O1 -Wall -Wextra -fsanitize=address,undefined -fno-sanitize-recover=all

DECODER_SOURCES := astc_decode.c etc_decode.c
DECODER_HEADERS := astc_decode.h etc_decode.h

TEXTURE_TOOL := $(TOOLS_BUILD_FOLDER)/texture_tool
TEXTURE_TOOL_CHECKED := $(TOOLS_BUILD_FOLDER)/texture_tool_checked
DECODER_SELFTEST := $(TOOLS_BUILD_FOLDER)/decoder_selftest

.PHONY: all test clean

all: $(TEXTURE_TOOL)

$(TEXTURE_TOOL): texture_tool.c $(DECODER_SOURCES) $(DECODER_HEADERS)
	@mkdir -p $(TOOLS_BUILD_FOLDER)
	$(CC) $(CFLAGS) texture_tool.c $(DECODER_SOURCES) -o $@

$(TEXTURE_TOOL_CHECKED): texture_tool.c $(DECODER_SOURCES) $(DECODER_HEADERS)
	@mkdir -p $(TOOLS_BUILD_FOLDER)
	$(CC) $(SANITIZER_FLAGS) texture_tool.c $(DECODER_SOURCES) -o $@

$(DECODER_SELFTEST): $(TESTS_FOLDER)/decoder_selftest.c $(DECODER_SOURCES) $(DECODER_HEADERS)
	@mkdir -p $(TOOLS_BUILD_FOLDER)
	$(CC) $(SANITIZER_FLAGS) -I. $(TESTS_FOLDER)/decoder_selftest.c $(DECODER_SOURCES) -o $@

test: $(TEXTURE_TOOL) $(TEXTURE_TOOL_CHECKED) $(DECODER_SELFTEST)
	$(DECODER_SELFTEST) 20000
	python3 -I $(TESTS_FOLDER)/run_tests.py

clean:
	rm -f $(TEXTURE_TOOL) $(TEXTURE_TOOL_CHECKED) $(DECODER_SELFTEST)
	rm -rf $(TEXTURE_TOOL_CHECKED).dSYM $(DECODER_SELFTEST).dSYM
