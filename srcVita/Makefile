TARGET   := ghosttrick
TITLE    := Ghost Trick Vita
TITLE_ID := GHTR00001
APP_VER  := 01.00
BUILD_ID := $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)$(shell git diff --quiet HEAD -- . 2>/dev/null || echo -dirty)-$(shell date +%Y%m%d%H%M%S)

ifeq ($(FAST_FORWARD),1)
BUILD_ID := $(BUILD_ID)-ff
endif

PREFIX  := arm-vita-eabi
CC      := $(PREFIX)-gcc
CFLAGS  := -Wall -Wextra -O2 -Iinclude -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -DBUILD_ID=\"$(BUILD_ID)\"
LDFLAGS := -Wl,-q
LIBS    := -lvitaGL -lstdc++ -lSceLibKernel_stub -lSceAppMgr_stub -lSceAppUtil_stub -lmathneon \
           -lc -lSceCommonDialog_stub -lm -lSceGxm_stub -lSceDisplay_stub -lSceSysmodule_stub \
           -lvitashark -lSceShaccCgExt -ltaihen_stub -lSceShaccCg_stub \
           -lSceKernelDmacMgr_stub -lSceCtrl_stub -lSceIofilemgr_stub -lSceProcessmgr_stub \
           -lSceAudio_stub -lScePower_stub

STB_VORBIS_SOURCE := src/third_party/stb_vorbis.c
STB_VORBIS_DEFINES := -DSTB_VORBIS_NO_STDIO -DSTB_VORBIS_NO_PUSHDATA_API
OBJECT_FOLDER := build/obj
ifeq ($(FAST_FORWARD),1)
CFLAGS += -DFAST_FORWARD_BUILD
OBJECT_FOLDER := build/obj_fast_forward
endif

SRCS := src/main.c $(wildcard src/app/*.c) $(wildcard src/vita/*.c) $(STB_VORBIS_SOURCE)
OBJS := $(patsubst src/%.c,$(OBJECT_FOLDER)/%.o,$(SRCS))

LIVEAREA_FOLDER := build/sce_sys
LIVEAREA_IMAGES := $(LIVEAREA_FOLDER)/icon0.png $(LIVEAREA_FOLDER)/livearea/contents/bg.png $(LIVEAREA_FOLDER)/livearea/contents/startup.png

HOST_CC     := clang
HOST_CFLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -g -O1 -Iinclude -Itests -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer
APP_SRCS    := $(wildcard src/app/*.c)
FAKE_SRCS   := $(wildcard tests/fakes/*.c)
TEST_SRCS   := $(wildcard tests/unit/*.c) $(wildcard tests/integration/*.c)
TEST_BINS   := $(patsubst tests/%.c,build/tests/%,$(TEST_SRCS))
STB_VORBIS_TEST_OBJECT := build/test_objects/stb_vorbis.o
TEST_HEADERS := $(wildcard include/*.h) $(wildcard tests/*.h) $(wildcard tests/fakes/*.h) $(wildcard tests/fakes/psp2/*.h) $(wildcard tests/fakes/psp2/kernel/*.h) $(wildcard tests/fixtures/*.h)
APP_TEST_OBJECTS := $(patsubst src/app/%.c,build/test_objects/app/%.o,$(APP_SRCS))
FAKE_TEST_OBJECTS := $(patsubst tests/fakes/%.c,build/test_objects/fakes/%.o,$(FAKE_SRCS))
TEST_RUNS := $(addprefix run-,$(TEST_BINS))

.PHONY: all clean check-sdk test test-binaries test-parallel test-all golden always-rebuild $(TEST_RUNS)

all: check-sdk build/$(TARGET).vpk

$(OBJECT_FOLDER)/vita/build_info.o: always-rebuild

$(OBJECT_FOLDER)/third_party/stb_vorbis.o: CFLAGS += -w -DNDEBUG $(STB_VORBIS_DEFINES)

check-sdk:
ifeq ($(strip $(VITASDK)),)
	$(error VITASDK is not set; point it at the VitaSDK install)
endif

$(OBJECT_FOLDER)/%.o: src/%.c $(wildcard include/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

build/$(TARGET).velf: build/$(TARGET).elf
	vita-elf-create $< $@

build/eboot.bin: build/$(TARGET).velf
	vita-make-fself -s $< $@

build/param.sfo: Makefile
	@mkdir -p build
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) -s APP_VER=$(APP_VER) -s VERSION=$(APP_VER) "$(TITLE)" $@

$(LIVEAREA_IMAGES): tools/make_livearea.py
	python3 -I tools/make_livearea.py $(LIVEAREA_FOLDER)

build/$(TARGET).vpk: build/eboot.bin build/param.sfo $(LIVEAREA_IMAGES) sce_sys/livearea/contents/template.xml
	vita-pack-vpk -s build/param.sfo -b build/eboot.bin \
		-a $(LIVEAREA_FOLDER)/icon0.png=sce_sys/icon0.png \
		-a $(LIVEAREA_FOLDER)/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		-a $(LIVEAREA_FOLDER)/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		-a sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
		$@

$(STB_VORBIS_TEST_OBJECT): $(STB_VORBIS_SOURCE)
	@mkdir -p $(dir $@)
	$(HOST_CC) -std=c99 -w -g -O0 -ffp-contract=off -include alloca.h $(STB_VORBIS_DEFINES) -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer -c $< -o $@

build/test_objects/app/%.o: src/app/%.c $(TEST_HEADERS)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) -c $< -o $@

build/test_objects/fakes/%.o: tests/fakes/%.c $(TEST_HEADERS)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) -c $< -o $@

build/test_objects/tests/%.o: tests/%.c $(TEST_HEADERS)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) -Itests/fakes -c $< -o $@

build/test_objects/tests/integration/test_main_loop.o: src/main.c
build/test_objects/tests/unit/test_vita_calls.o: src/vita/keep_awake.c src/vita/system_state.c
build/test_objects/tests/unit/test_vita_renderer.o: src/vita/renderer.c

build/tests/%: build/test_objects/tests/%.o $(APP_TEST_OBJECTS) $(FAKE_TEST_OBJECTS) $(STB_VORBIS_TEST_OBJECT)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) $^ -o $@

.SECONDARY:

test-binaries: $(TEST_BINS)

test: $(TEST_BINS)
	@failed=0; for test_binary in $(TEST_BINS); do $$test_binary || failed=1; done; exit $$failed

$(TEST_RUNS): run-%: %
	@$*

test-parallel: $(TEST_RUNS)

test-all: test
	$(MAKE) -C tools/assets test

golden: build/tests/golden/script_replay
	@mkdir -p build/golden
	@test -f ../extracted/trace/vm_golden/golden_trace.tsv || { echo "no golden trace in extracted/trace/vm_golden"; exit 2; }
	build/tests/golden/script_replay --scripts ../extracted/vita_assets/chapter1/scripts --timing ds --rows build/golden/rows_ds.tsv --cues build/golden/cues_ds.tsv --stop-tick 6056 --close-talk-at 5643 --close-talk-at 5823 --close-talk-at 6002
	python3 -I tools/script_golden_check.py build/golden/rows_ds.tsv --stop-tick 7095

route-check: build/tests/golden/script_replay
	@mkdir -p build/golden
	build/tests/golden/script_replay --scripts ../extracted/vita_assets/chapter1/scripts --timing remaster --auto-paced --marks --stop-tick 140000 > build/golden/route_marks.txt
	python3 -I tools/route_check.py build/golden/route_marks.txt ../extracted/vita_assets/chapter1/scripts/route_steps.txt

clean:
	rm -rf build
