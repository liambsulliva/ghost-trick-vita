TARGET   := ghosttrick
TITLE    := Ghost Trick Vita
TITLE_ID := GHTR00001
APP_VER  := 01.00
BUILD_ID := $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)$(shell git diff --quiet HEAD -- . 2>/dev/null || echo -dirty)-$(shell date +%Y%m%d%H%M%S)

ifeq ($(DEBUG),1)
BUILD_ID := $(BUILD_ID)-debug
endif

PREFIX  := arm-vita-eabi
CC      := $(PREFIX)-gcc
CFLAGS  := -Wall -Wextra -O2 -Iinclude -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -DBUILD_ID=\"$(BUILD_ID)\"
DATA_SEGMENT_CLEAR_OF_SCE_TABLES := 0x81200000
TEXTURE_FOLDER_BUCKETS := 64
DEVICE_DEFINES := -DTEXTURE_FOLDER_BUCKETS=$(TEXTURE_FOLDER_BUCKETS)u
CFLAGS += $(DEVICE_DEFINES)
LDFLAGS := -Wl,-q -Wl,--section-start=.init_array=$(DATA_SEGMENT_CLEAR_OF_SCE_TABLES)
LIBS    := -lvitaGL -lstdc++ -lSceAppMgr_stub -lSceAppUtil_stub -lmathneon \
           -lSceCommonDialog_stub -lm -lSceGxm_stub -lSceDisplay_stub \
           -lvitashark -lSceShaccCgExt -ltaihen_stub -lSceShaccCg_stub \
           -lSceKernelDmacMgr_stub -lSceCtrl_stub -lSceTouch_stub \
           -lSceAudio_stub -lScePower_stub

STB_VORBIS_SOURCE := src/third_party/stb_vorbis.c
STB_VORBIS_DEFINES := -DSTB_VORBIS_NO_STDIO -DSTB_VORBIS_NO_PUSHDATA_API
OBJECT_FOLDER := build/obj
ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG_BUILD
OBJECT_FOLDER := build/obj_debug
endif

DEBUG_ONLY_NAMES := bmp_image command_file command_reader fps_overlay log_sink screen_burst screen_capture screen_changes screen_watch smoke_run
DEBUG_ONLY_SRCS := $(foreach name,$(DEBUG_ONLY_NAMES),$(wildcard src/app/$(name).c src/vita/$(name).c))
SRCS := src/main.c $(wildcard src/app/*.c) $(wildcard src/vita/*.c) $(STB_VORBIS_SOURCE)
ifneq ($(DEBUG),1)
SRCS := $(filter-out $(DEBUG_ONLY_SRCS),$(SRCS))
endif
OBJS := $(patsubst src/%.c,$(OBJECT_FOLDER)/%.o,$(SRCS))

LIVEAREA_FOLDER := build/sce_sys
RELEASE ?= 0
LIVEAREA_ASSETS_PRESENT := $(and $(wildcard ../extracted/remaster_apk/GTPD_ANDROID_Release.list),$(wildcard build/tools/texture_tool))
LIVEAREA_DS_PRESENT := $(wildcard ../extracted/ds_rom/game.nds)
LIVEAREA_STYLE := $(if $(filter 1,$(RELEASE)),monogram,$(if $(LIVEAREA_ASSETS_PRESENT),logo,$(if $(LIVEAREA_DS_PRESENT),ds_logo,monogram)))
LIVEAREA_SOURCE := build/livearea_$(LIVEAREA_STYLE)
LIVEAREA_IMAGE_NAMES := icon0.png livearea/contents/bg.png livearea/contents/startup.png

PLAN_SET ?= game
PLAN_DESCRIPTION ?= $(PLAN_SET)
PLAN_STOP_TICK ?= 4200000
PLAN_SCRIPTS := ../extracted/vita_assets/$(PLAN_SET)/scripts
STEPS_THE_STORY_PASSES_OVER := 1
HOST_CC     := clang
HOST_STANDARD_CFLAGS := -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -g -O1 -Iinclude -Itests
HOST_CFLAGS := $(HOST_STANDARD_CFLAGS) -DDEBUG_BUILD -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer
APP_SRCS    := $(wildcard src/app/*.c)
FAKE_SRCS   := $(wildcard tests/fakes/*.c)
TEST_SRCS   := $(wildcard tests/unit/*.c) $(wildcard tests/integration/*.c)
TEST_BINS   := $(patsubst tests/%.c,build/tests/%,$(TEST_SRCS))
STB_VORBIS_TEST_OBJECT := build/test_objects/stb_vorbis.o
TEST_HEADERS := $(wildcard include/*.h) $(wildcard tests/*.h) $(wildcard tests/fakes/*.h) $(wildcard tests/fakes/psp2/*.h) $(wildcard tests/fakes/psp2/kernel/*.h) $(wildcard tests/fixtures/*.h)
APP_TEST_OBJECTS := $(patsubst src/app/%.c,build/test_objects/app/%.o,$(APP_SRCS))
FAKE_TEST_OBJECTS := $(patsubst tests/fakes/%.c,build/test_objects/fakes/%.o,$(FAKE_SRCS))
TEST_RUNS := $(addprefix run-,$(TEST_BINS))
# A test that hangs fails by name after this many seconds instead of stalling the run with no output (root cause 299).
TEST_SECONDS_MOST ?= 300
RUN_TEST_WITH_LIMIT = perl -e '$$s = shift; $$SIG{ALRM} = sub { print STDERR "FAIL $$ARGV[0]: still running after $$s seconds\n"; kill "KILL", $$pid; exit 124 }; $$pid = fork; if (!$$pid) { exec @ARGV; exit 127 } alarm $$s; waitpid $$pid, 0; exit($$? & 127 ? 128 + ($$? & 127) : $$? >> 8)' $(TEST_SECONDS_MOST)

.PHONY: all release clean check-sdk check-libs test test-binaries test-parallel test-all golden coverage always-rebuild $(TEST_RUNS)

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

build/$(TARGET).elf: $(OBJS) FORCE
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

build/$(TARGET).velf: build/$(TARGET).elf FORCE
	vita-elf-create $< $@

build/eboot.bin: build/$(TARGET).velf FORCE
	vita-make-fself -s $< $@

build/param.sfo: Makefile
	@mkdir -p build
	vita-mksfoex -d ATTRIBUTE2=12 -s TITLE_ID=$(TITLE_ID) -s APP_VER=$(APP_VER) -s VERSION=$(APP_VER) "$(TITLE)" $@

$(LIVEAREA_SOURCE)/icon0.png: tools/make_livearea.py
	python3 -I tools/make_livearea.py $(LIVEAREA_SOURCE) $(if $(filter monogram,$(LIVEAREA_STYLE)),--monogram,)

release:
	$(MAKE) RELEASE=1 build/$(TARGET).vpk

release-debug:
	$(MAKE) DEBUG=1 RELEASE=1 build/$(TARGET).vpk

.PHONY: FORCE
FORCE:

build/$(TARGET).vpk: build/eboot.bin build/param.sfo $(LIVEAREA_SOURCE)/icon0.png sce_sys/livearea/contents/template.xml FORCE
	@mkdir -p $(LIVEAREA_FOLDER)/livearea/contents
	@for name in $(LIVEAREA_IMAGE_NAMES); do cp $(LIVEAREA_SOURCE)/$$name $(LIVEAREA_FOLDER)/$$name; done
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
build/tests/unit/test_packed_file: | build/packed_vectors/manifest.txt

build/packed_vectors/manifest.txt: tests/tools/make_packed_vectors.py tools/assets/packed_file.py
	python3 -I tests/tools/make_packed_vectors.py build/packed_vectors

build/tests/%: build/test_objects/tests/%.o $(APP_TEST_OBJECTS) $(FAKE_TEST_OBJECTS) $(STB_VORBIS_TEST_OBJECT)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) $^ -o $@

SMOKE_HOST := build/smoke_host/smoke_host
SMOKE_OBJECT_FOLDER := build/smoke_host/objects
SMOKE_FAKE_NAMES := fake_command_reader fake_reader fake_system_state fake_task fake_work_queue
SMOKE_SRCS := tests/smoke/smoke_host.c tests/smoke/smoke_renderer.c tests/smoke/smoke_audio.c $(APP_SRCS) $(foreach name,$(SMOKE_FAKE_NAMES),tests/fakes/$(name).c)
SMOKE_OBJECTS := $(patsubst %.c,$(SMOKE_OBJECT_FOLDER)/%.o,$(SMOKE_SRCS))

$(SMOKE_OBJECT_FOLDER)/%.o: %.c $(TEST_HEADERS) $(wildcard tests/smoke/*.h)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) $(DEVICE_DEFINES) -Itests/fakes -Itests/smoke -c $< -o $@

$(SMOKE_OBJECT_FOLDER)/tests/smoke/smoke_host.o: src/main.c

$(SMOKE_OBJECT_FOLDER)/stb_vorbis.o: $(STB_VORBIS_SOURCE)
	@mkdir -p $(dir $@)
	$(HOST_CC) -std=c99 -w -O2 -ffp-contract=off -include alloca.h $(STB_VORBIS_DEFINES) -c $< -o $@

$(SMOKE_HOST): $(SMOKE_OBJECTS) $(SMOKE_OBJECT_FOLDER)/stb_vorbis.o
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) $^ -o $@

SMOKE_PACKAGE_FOLDER := build/smoke/package

.PHONY: smoke smoke-package smoke-mutations

smoke:
	python3 -I tools/smoke_test.py $(if $(filter 1,$(SMOKE_QUICK)),--quick,) $(SMOKE_ARGUMENTS)

smoke-package: check-sdk build/$(TARGET).vpk
	@test "$(DEBUG)" = 1 || { echo "smoke-package links the debug build; pass DEBUG=1"; exit 2; }
	@mkdir -p $(SMOKE_PACKAGE_FOLDER)
	cp build/$(TARGET).elf $(SMOKE_PACKAGE_FOLDER)/$(TARGET)-debug.elf
	cp build/$(TARGET).vpk $(SMOKE_PACKAGE_FOLDER)/$(TARGET)-debug.vpk

smoke-mutations:
	python3 -I tools/smoke_mutations.py $(SMOKE_ARGUMENTS)

.SECONDARY:

test-binaries: $(TEST_BINS)

PLAYER_OBJECT_FOLDER := build/player_objects
PLAYER_SRCS := src/main.c $(filter-out $(DEBUG_ONLY_SRCS),$(APP_SRCS)) $(filter-out tests/fakes/fake_command_reader.c,$(FAKE_SRCS)) tests/player_link/platform.c
PLAYER_OBJECTS := $(patsubst %.c,$(PLAYER_OBJECT_FOLDER)/%.o,$(PLAYER_SRCS))
PLAYER_DEVICE_SRCS := $(filter-out $(DEBUG_ONLY_SRCS),$(wildcard src/vita/*.c))
PLAYER_LINK_MAP := $(PLAYER_OBJECT_FOLDER)/player.map
PLAYER_LINK_CHECK := $(PLAYER_OBJECT_FOLDER)/player_link_checked

$(PLAYER_OBJECT_FOLDER)/%.o: %.c $(TEST_HEADERS)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_STANDARD_CFLAGS) -fno-inline -Itests/fakes -c $< -o $@

$(PLAYER_OBJECT_FOLDER)/stb_vorbis.o: $(STB_VORBIS_SOURCE)
	@mkdir -p $(dir $@)
	$(HOST_CC) -std=c99 -w -O1 -include alloca.h $(STB_VORBIS_DEFINES) -c $< -o $@

$(PLAYER_LINK_MAP): $(PLAYER_OBJECTS) $(PLAYER_OBJECT_FOLDER)/stb_vorbis.o
	$(HOST_CC) $^ -Wl,-dead_strip -Wl,-map,$@ -o $(PLAYER_OBJECT_FOLDER)/player

$(PLAYER_LINK_CHECK): $(PLAYER_LINK_MAP) tests/player_link/unreached.txt tests/tools/check_player_link.py $(PLAYER_DEVICE_SRCS)
	python3 -I tests/tools/check_player_link.py $(PLAYER_LINK_MAP) tests/player_link/unreached.txt --game-objects $(PLAYER_OBJECT_FOLDER)/src/ --never-linked log_sink_start log_sink_write log_sink_flush --device-sources $(PLAYER_DEVICE_SRCS)
	@touch $@

test: $(PLAYER_LINK_CHECK) $(TEST_BINS)
	@failed=0; for test_binary in $(TEST_BINS); do $(RUN_TEST_WITH_LIMIT) $$test_binary || failed=1; done; exit $$failed

$(TEST_RUNS): run-%: %
	@$(RUN_TEST_WITH_LIMIT) $*

test-parallel: $(TEST_RUNS)

test-all: test
	$(MAKE) -C tools/assets test

golden: build/tests/golden/script_replay
	@mkdir -p build/golden
	@test -f ../extracted/trace/vm_golden/golden_trace.tsv || { echo "no golden trace in extracted/trace/vm_golden"; exit 2; }
	build/tests/golden/script_replay --scripts $(PLAN_SCRIPTS) --timing ds --rows build/golden/rows_ds.tsv --cues build/golden/cues_ds.tsv --stop-tick 6056 --close-talk-at 5643 --close-talk-at 5823 --close-talk-at 6002
	python3 -I tools/script_golden_check.py build/golden/rows_ds.tsv --stop-tick 7095

coverage: route-check
	python3 -I tools/coverage_report.py build/golden/route_marks.txt

read-ahead: build/tests/golden/script_replay
	@mkdir -p build/golden
	python3 -I tools/assets/route_pacing.py build/tests/golden/script_replay tools/assets/$(PLAN_DESCRIPTION).json ../extracted/vita_assets/$(PLAN_SET)/scripts build/golden/cues_paced_$(PLAN_SET).tsv --stop-tick $(PLAN_STOP_TICK) --kept tools/assets/$(PLAN_SET)_pacing.txt
	python3 -I tools/assets/read_ahead.py build/golden/cues_paced_$(PLAN_SET).tsv ../extracted/vita_assets/$(PLAN_SET)/scripts ../extracted/vita_assets/$(PLAN_SET)/animations

route-check: build/tests/golden/script_replay
	@mkdir -p build/golden
	build/tests/golden/script_replay --scripts $(PLAN_SCRIPTS) --timing remaster --auto-paced --marks --stop-tick $(PLAN_STOP_TICK) --cues build/golden/route_cues.txt --chapter-starts build/golden/chapter_starts.txt > build/golden/route_marks.txt
	python3 -I tools/route_check.py build/golden/route_marks.txt $(PLAN_SCRIPTS)/route_steps.txt --most-passed-over $(STEPS_THE_STORY_PASSES_OVER) --must-reach GOTO
	build/tests/golden/script_replay --scripts $(PLAN_SCRIPTS) --timing remaster --auto-paced --reads-at-once --marks --stop-tick $(PLAN_STOP_TICK) > build/golden/route_marks_fast.txt
	python3 -I tools/route_check.py build/golden/route_marks_fast.txt $(PLAN_SCRIPTS)/route_steps.txt --most-passed-over $(STEPS_THE_STORY_PASSES_OVER) --must-reach GOTO
	$(if $(filter game,$(PLAN_SET)),python3 -I tests/tools/check_undrawn_effects.py $(PLAN_SCRIPTS))
	python3 -I tools/chapter_starts_check.py build/tests/golden/script_replay $(PLAN_SCRIPTS) build/golden/chapter_starts.txt build/golden/route_cues.txt

HOST_RENDER_FAKES := tests/fakes/fake_audio.c tests/fakes/fake_command_reader.c tests/fakes/fake_reader.c tests/fakes/fake_system_state.c tests/fakes/fake_task.c tests/fakes/fake_work_queue.c
HOST_RENDER_SRCS := $(APP_SRCS) $(HOST_RENDER_FAKES) $(wildcard tests/host/*.c) tests/golden/scene_render.c

build/host/stb_vorbis.o: $(STB_VORBIS_SOURCE)
	@mkdir -p $(dir $@)
	$(HOST_CC) -std=c99 -w -O2 -ffp-contract=off -include alloca.h -DNDEBUG $(STB_VORBIS_DEFINES) -c $< -o $@

build/host/scene_render: $(HOST_RENDER_SRCS) build/host/stb_vorbis.o $(TEST_HEADERS) $(wildcard tests/host/*.h)
	@mkdir -p $(dir $@)
	$(HOST_CC) -std=c99 -D_POSIX_C_SOURCE=200809L -D_DARWIN_C_SOURCE -Wall -Wextra -Werror -g -O2 -Iinclude -Itests -Itests/fakes -DDEBUG_BUILD -DTEXTURE_FOLDER_BUCKETS=$(TEXTURE_FOLDER_BUCKETS)u $(HOST_RENDER_SRCS) build/host/stb_vorbis.o -lm -o $@

scene-render: build/host/scene_render

check-libs: check-sdk $(OBJS)
	@$(foreach library,$(LIBS),if $(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(filter-out $(library),$(LIBS)) -o build/without_one_library.elf 2>/dev/null; then echo "the link does not need $(library)"; exit 1; fi;)
	@echo "every linked library is needed"

clean:
	rm -rf build
