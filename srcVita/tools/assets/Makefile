SRC_VITA_FOLDER := ../..
TOOLS_BUILD_FOLDER := $(SRC_VITA_FOLDER)/build/tools
TESTS_FOLDER := $(SRC_VITA_FOLDER)/tests/tools

CC := clang
CFLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -O2 -Wall -Wextra
SANITIZER_FLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -g -O1 -Wall -Wextra -fsanitize=address,undefined -fno-sanitize-recover=all

DECODER_SOURCES := astc_decode.c etc_decode.c
DECODER_HEADERS := astc_decode.h etc_decode.h

APP_FOLDER := $(SRC_VITA_FOLDER)/src/app
INCLUDE_FOLDER := $(SRC_VITA_FOLDER)/include
TEST_FAKES_FOLDER := $(SRC_VITA_FOLDER)/tests/fakes
STB_VORBIS_SOURCE := $(SRC_VITA_FOLDER)/src/third_party/stb_vorbis.c
STB_VORBIS_FLAGS := -std=c99 -w -O2 -ffp-contract=off -include alloca.h -DSTB_VORBIS_NO_STDIO -DSTB_VORBIS_NO_PUSHDATA_API
STB_VORBIS_OBJECT := $(TOOLS_BUILD_FOLDER)/stb_vorbis.o
MUSIC_SOURCES := $(APP_FOLDER)/crc32.c $(APP_FOLDER)/pcm_ramp.c $(APP_FOLDER)/ogg_pages.c $(APP_FOLDER)/vorbis_stream.c $(APP_FOLDER)/music_source.c
MUSIC_REFERENCE_FLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -O2 -Wall -Wextra -ffp-contract=off -I$(INCLUDE_FOLDER)
LOG_WRITER_SOURCES := $(TESTS_FOLDER)/music_log_writer.c $(MUSIC_SOURCES) $(APP_FOLDER)/music_engine.c $(APP_FOLDER)/sound_engine.c $(APP_FOLDER)/playback_speed.c $(APP_FOLDER)/pcm_mix.c $(APP_FOLDER)/file_path.c $(APP_FOLDER)/run_log.c $(TEST_FAKES_FOLDER)/fake_audio.c $(TEST_FAKES_FOLDER)/fake_platform.c

TEXTURE_TOOL := $(TOOLS_BUILD_FOLDER)/texture_tool
MUSIC_REFERENCE := $(TOOLS_BUILD_FOLDER)/music_reference
MUSIC_LOG_WRITER := $(TOOLS_BUILD_FOLDER)/music_log_writer
TEXTURE_TOOL_CHECKED := $(TOOLS_BUILD_FOLDER)/texture_tool_checked
DECODER_SELFTEST := $(TOOLS_BUILD_FOLDER)/decoder_selftest

.PHONY: all test clean

all: $(TEXTURE_TOOL) $(MUSIC_REFERENCE)

$(TEXTURE_TOOL): texture_tool.c $(DECODER_SOURCES) $(DECODER_HEADERS)
	@mkdir -p $(TOOLS_BUILD_FOLDER)
	$(CC) $(CFLAGS) texture_tool.c $(DECODER_SOURCES) -o $@

$(TEXTURE_TOOL_CHECKED): texture_tool.c $(DECODER_SOURCES) $(DECODER_HEADERS)
	@mkdir -p $(TOOLS_BUILD_FOLDER)
	$(CC) $(SANITIZER_FLAGS) texture_tool.c $(DECODER_SOURCES) -o $@

$(DECODER_SELFTEST): $(TESTS_FOLDER)/decoder_selftest.c $(DECODER_SOURCES) $(DECODER_HEADERS)
	@mkdir -p $(TOOLS_BUILD_FOLDER)
	$(CC) $(SANITIZER_FLAGS) -I. $(TESTS_FOLDER)/decoder_selftest.c $(DECODER_SOURCES) -o $@

$(STB_VORBIS_OBJECT): $(STB_VORBIS_SOURCE)
	@mkdir -p $(TOOLS_BUILD_FOLDER)
	$(CC) $(STB_VORBIS_FLAGS) -c $< -o $@

$(MUSIC_REFERENCE): music_reference.c $(MUSIC_SOURCES) $(STB_VORBIS_OBJECT) $(wildcard $(INCLUDE_FOLDER)/*.h)
	$(CC) $(MUSIC_REFERENCE_FLAGS) music_reference.c $(MUSIC_SOURCES) $(STB_VORBIS_OBJECT) -o $@

$(MUSIC_LOG_WRITER): $(LOG_WRITER_SOURCES) $(STB_VORBIS_OBJECT) $(wildcard $(INCLUDE_FOLDER)/*.h) $(wildcard $(TEST_FAKES_FOLDER)/*.h)
	$(CC) $(MUSIC_REFERENCE_FLAGS) -I$(SRC_VITA_FOLDER)/tests $(LOG_WRITER_SOURCES) $(STB_VORBIS_OBJECT) -o $@

test: $(TEXTURE_TOOL) $(TEXTURE_TOOL_CHECKED) $(DECODER_SELFTEST) $(MUSIC_REFERENCE) $(MUSIC_LOG_WRITER)
	$(DECODER_SELFTEST) 20000
	python3 -I $(TESTS_FOLDER)/run_tests.py

clean:
	rm -f $(TEXTURE_TOOL) $(TEXTURE_TOOL_CHECKED) $(DECODER_SELFTEST) $(MUSIC_REFERENCE) $(MUSIC_LOG_WRITER) $(STB_VORBIS_OBJECT)
	rm -rf $(TEXTURE_TOOL_CHECKED).dSYM $(DECODER_SELFTEST).dSYM
