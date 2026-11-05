TARGET   := ghosttrick
TITLE    := GHOSTTRICK
TITLE_ID := GHTR00001
APP_VER  := 01.00

ifeq ($(strip $(VITASDK)),)
$(error VITASDK is not set; point it at the VitaSDK install)
endif

PREFIX  := arm-vita-eabi
CC      := $(PREFIX)-gcc
CFLAGS  := -Wall -O2 -Iinclude -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard
LDFLAGS := -Wl,-q
LIBS    := -lvitaGL -lstdc++ -lSceLibKernel_stub -lSceAppMgr_stub -lSceAppUtil_stub -lmathneon \
           -lc -lSceCommonDialog_stub -lm -lSceGxm_stub -lSceDisplay_stub -lSceSysmodule_stub \
           -lvitashark -lSceShaccCgExt -ltaihen_stub -lSceShaccCg_stub \
           -lSceKernelDmacMgr_stub -lSceCtrl_stub -lSceIofilemgr_stub -lSceProcessmgr_stub

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,build/obj/%.o,$(SRCS))

.PHONY: all clean

all: build/$(TARGET).vpk

build/obj:
	mkdir -p $@

build/obj/%.o: src/%.c | build/obj
	$(CC) $(CFLAGS) -c $< -o $@

build/$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

build/$(TARGET).velf: build/$(TARGET).elf
	vita-elf-create $< $@

build/eboot.bin: build/$(TARGET).velf
	vita-make-fself -s $< $@

build/param.sfo: | build/obj
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) -s APP_VER=$(APP_VER) -s VERSION=$(APP_VER) "$(TITLE)" $@

build/$(TARGET).vpk: build/eboot.bin build/param.sfo
	vita-pack-vpk -s build/param.sfo -b build/eboot.bin $@

clean:
	rm -rf build
