TARGET   := ghosttrick
TITLE    := Ghost Trick Vita
TITLE_ID := GHTR00001
APP_VER  := 01.00
BUILD_ID := $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)$(shell git diff --quiet HEAD -- . 2>/dev/null || echo -dirty)-$(shell date +%Y%m%d%H%M%S)

PREFIX  := arm-vita-eabi
CC      := $(PREFIX)-gcc
CFLAGS  := -Wall -Wextra -O2 -Iinclude -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -DBUILD_ID=\"$(BUILD_ID)\"
LDFLAGS := -Wl,-q
LIBS    := -lvitaGL -lstdc++ -lSceLibKernel_stub -lSceAppMgr_stub -lSceAppUtil_stub -lmathneon \
           -lc -lSceCommonDialog_stub -lm -lSceGxm_stub -lSceDisplay_stub -lSceSysmodule_stub \
           -lvitashark -lSceShaccCgExt -ltaihen_stub -lSceShaccCg_stub \
           -lSceKernelDmacMgr_stub -lSceCtrl_stub -lSceIofilemgr_stub -lSceProcessmgr_stub

SRCS := src/main.c $(wildcard src/app/*.c) $(wildcard src/vita/*.c)
OBJS := $(patsubst src/%.c,build/obj/%.o,$(SRCS))

LIVEAREA_FOLDER := build/sce_sys
LIVEAREA_IMAGES := $(LIVEAREA_FOLDER)/icon0.png $(LIVEAREA_FOLDER)/livearea/contents/bg.png $(LIVEAREA_FOLDER)/livearea/contents/startup.png

HOST_CC     := clang
HOST_CFLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -g -O1 -Iinclude -Itests -fsanitize=address,undefined -fno-omit-frame-pointer
APP_SRCS    := $(wildcard src/app/*.c)
FAKE_SRCS   := $(wildcard tests/fakes/*.c)
TEST_SRCS   := $(wildcard tests/unit/*.c) $(wildcard tests/widget/*.c) $(wildcard tests/integration/*.c)
TEST_BINS   := $(patsubst tests/%.c,build/tests/%,$(TEST_SRCS))

.PHONY: all clean check-sdk test always-rebuild

all: check-sdk build/$(TARGET).vpk

build/obj/vita/build_info.o: always-rebuild

check-sdk:
ifeq ($(strip $(VITASDK)),)
	$(error VITASDK is not set; point it at the VitaSDK install)
endif

build/obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

build/$(TARGET).velf: build/$(TARGET).elf
	vita-elf-create $< $@

build/eboot.bin: build/$(TARGET).velf
	vita-make-fself -s $< $@

build/param.sfo:
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

build/tests/%: tests/%.c $(APP_SRCS) $(FAKE_SRCS) $(wildcard include/*.h) $(wildcard tests/*.h)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) $< $(APP_SRCS) $(FAKE_SRCS) -o $@

test: $(TEST_BINS)
	@failed=0; for test_binary in $(TEST_BINS); do $$test_binary || failed=1; done; exit $$failed

clean:
	rm -rf build
