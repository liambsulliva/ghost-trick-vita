# Handoff: Ghost Trick PS Vita port

For the next agent picking up this project. Updated 2026-10-28 after the CPAC fix and the user's Chapter 1 scope decision. Read this file, then `docs/chapter1.md`, `docs/inventory.md` and `docs/frame_mapping.md`.

## 1. What the project is

The goal is a PS Vita port of Ghost Trick: Phantom Detective, using the **2023 remaster's art and UI** on top of the **original DS game logic**.

**Current delivery scope: Chapter 1 only.** The user wants a playable on-device build and a feedback cycle before expanding. The rest of the game is a stretch goal if that works well. Keep all prepared assets, logic, extracted data, tools and research for later use; narrow new implementation and verification work rather than removing existing preparation. `docs/chapter1.md` is the current delivery plan. The whole `st01` folder is a conservative inventory, not a proven Chapter 1 boundary, because it also contains later scenes.

Decided direction, after earlier feasibility work:
- **The remaster's own binaries can't be ported.** The Android APK, Windows and Steam builds are RE Engine, 64-bit (arm64 or x86-64), Vulkan or Direct3D, and need about 3 GB. The Vita is 32-bit ARMv7 with 512 MB RAM and vitaGL (GLES-like).
- **The plan:** recompile the DS game's ARM9 code natively for the Vita (ARMv5TE code runs mostly unchanged on ARMv7), replace its renderer and UI, and draw remaster assets converted offline from the user's own Steam copy. The UI follows the single-screen Android remaster, not the DS's two screens.
- **The asset finding:** neither version uses skeletal animation. The remaster's characters are high-resolution 2D art driven by keyframe timelines. Details are in section 6.

The `BFM-decomp` wiki phase ladder (https://github.com/Druthulu/BFM-decomp/wiki/Start-a-new-decomp-project) supplied the earlier phase sequence. It was written for PlayStation and gcc, so the DS equivalents are used. Phases 0–3 are done (section 5). The user's newer priority is the Chapter 1 device test; a whole-game matching-C campaign is deferred.

## 2. How the user works (follow this)

- **Commits:** the user reviews, commits and pushes with their own messages. The user reaffirmed this during the continuation: "Don't commit. Let me do it myself." Leave changes ready for their review and commit. Do not commit, push, amend or add co-author lines. Branch: `asset-inventory`. Remote: `git@github.com:liambsulliva/ghost-trick-vita.git` (private).
- **Code style:** write like a junior developer learning as they go. Plain loops and functions, no clever pointer arithmetic, no unusual data structures, **no comments in code**. Match the existing `tools/*.py`.
- **Writing style:** plain and direct, no filler. The user had asked for an "unslop" skill that wasn't installed, so write plainly.
- **Decisions:** at each checkpoint, offer the user a short choice of the next step rather than charging ahead into big new areas.
- **Hardware:** the Vita (`ps-vita-mcp`, FTP on volume `/Untitled`, DevLoop and Control MCP servers) is **turned off to save battery**. Ask the user to re-enable it before any on-device test. The first on-device step is planned to be a small model or frame viewer that reports frame time and free memory.
- **Network:** the user runs a VPN that sometimes drops connections. Some network commands get rejected or interrupted. When the user says "VPN connected, continue", retry the same command.
- **Legal and ethical guardrails:**
  - Game data comes only from the user's own dumps and copies. Nothing game-derived goes in git; `tools/rom_audit.py` enforces this.
  - Do not reproduce the game's dialogue or story text in files or chat.
  - Do not render game characters for display.
  - The APK `~/Developer/Ghost-Trick-v1.0.0-full-apkvision.apk` is a **modified third-party repack**. It was only read for format research. Do not patch its licensing and don't treat it as authoritative. Results from it should be rechecked on the Steam copy, which isn't on this Mac yet.
  - DS overlay 0 is **DS Protect** (anti-piracy). The port should leave it out.

## 3. Inputs on this machine

- **DS ROM:** `~/Developer/Ghost Trick - Phantom Detective (USA) (En,Fr,De,Es,It).nds` (SHA-1 `bcfe74e5592eef3255fd23758b772226960a1b0b`, game code `BGTE`).
- **Remaster (Android repack):** the APK above. `re_chunk_000.pak` and the `data01`/`data02` Play Asset Delivery ZIP archives are under `extracted/remaster_apk/`. Pack 000 has a broad payload inventory. All three pack indexes and selected other animation payloads have now been checked. `data01` contains a complete 794,538,139-byte `re_chunk_002.pak` plus its partial `.pak.0` copy; `data02` contains complete `re_chunk_001.pak` and the `.pak.1` tail. Use the complete files inside the ZIP archives directly; there is no need to join the split copies or extract another large copy.
- **Community file list:** `extracted/remaster_apk/GTPD_ANDROID_Release.list`, from Ekey/REE.PAK.Tool. Its license is unclear, so never commit it. A `GTPD_STM_Release.list` for Steam also exists upstream.
- **PC reference video:** downloaded and checked; see section 8.
- **Disk:** about 40 GiB free after the corrected CPAC extraction and video download. `extracted/` takes several GB. Be careful with big outputs.

## 4. Toolchain installed

| Tool | Where | Notes |
|---|---|---|
| Ghidra 12.2 | `brew install ghidra` | Headless: `$(brew --prefix ghidra)/libexec/support/analyzeHeadless`. Needs `JAVA_HOME="$(brew --prefix openjdk@25)/libexec/openjdk.jdk/Contents/Home"`. Project: `extracted/ghidra/GhostTrick` (`main.bin`). |
| dsd (ds-decomp) 0.12.1, **patched** | `toolchain/dsd` (official binary kept as `toolchain/dsd-0.12.1-official`) | Patch: `tools/patches/dsd-0.12.1-thumb-retry.patch`. Source in `toolchain/src/ds-decomp`. Rebuild steps are in `docs/inventory.md`. |
| Rust (Homebrew) | `cargo`, `rustc` | Fetch with `CARGO_NET_GIT_FETCH_WITH_CLI=true`; libgit2 TLS breaks behind the VPN. |
| LLVM 23 + lld | `brew --prefix llvm`, `brew --prefix lld` | `ld.lld`, `llvm-objcopy`, `llvm-readelf`, `llvm-mc`. |
| melonDS 1.1 (official macOS zip, SHA-256 checked) | `extracted/melonds/melonDS.app`, **portable mode** in `extracted/melonds/portable/` | Reference/oracle on the Mac only; never part of the Vita build (GPL). |
| yt-dlp 2026.10.26 | `toolchain/yt-dlp-venv/bin/yt-dlp` | The global `/opt/homebrew/bin/yt-dlp` is old (2026.03.17) and gets HTTP 403. Leave the global one alone. |
| ffmpeg/ffprobe 9.0.2 | Homebrew | Has dav1d for AV1. Its TLS to googlevideo fails, so it can't download sections. |

`toolchain/` and `extracted/` are gitignored.

## 5. Phase status (BFM ladder, DS version)

| Phase | Status | Key files |
|---|---|---|
| 0 Governance and firewall | Done | `.gitignore`; `tools/rom_audit.py` with `tools/audit_rules.txt` (forbidden extensions and magic bytes, plus a hash check against the local ROM manifest); `--self-test` must catch every planted file; CI workflow `.github/workflows/audit.yml` |
| 1 Deterministic extraction | Done | `tools/nds_unpack.py` (manifest reproducible), `tools/nds_catalog.py`, `tools/cpac_unpack.py`, `tools/lz.py` |
| 2 Oracles and load map | Done | `tools/nds_loadmap.py`, `tools/nds_prove_bases.py`, `tools/ghidra_build.sh` (rebuilds the Ghidra DB from `symbols/database.txt` and checks the re-export is identical), `tools/runtime_oracle.py` (melonDS GDB stub; entry, main, ITCM and overlay checks) |
| 3 All-assembly baseline | Done | `config/arm9/` (dsd config with two hand edits), `tools/lld_link.py`, `tools/elf_fixups.py`, `tools/fix_header.py`, `tools/phase3_check.sh <rom>` (ROM rebuilds byte-identical from linked objects) |
| 4 Compiler pinned | Flags pinned and the dsi family pinned (8 matches); exact dsi version not yet | See "Phase 4 compiler probe" below. |
| 5–8 Matching decomp | Started | 45 of 8,573 dsd functions are C-matched (`README.md` treemap via `tools/progress_treemap.py`, `config/decomp_progress.json`). A whole-game matching campaign is deferred behind the Chapter 1 target. |

### Phase 4 compiler probe (2026-10-28)

- The user approved the compiler download. `toolchain/mwccarm.zip` comes from decomp.me's `compilers` release; 24 versions are unpacked in `toolchain/mwccarm/`, with hashes in `toolchain/mwccarm_manifest.json`. They run under the user's installed CrossOver with bottle `toolchain/wine-mwcc-run`, and need `LM_LICENSE_FILE='Z:\Users\liambsulliva\Developer\ghost-trick-vita\toolchain\mwccarm\license.dat'`. Without it the compiler hangs; kill any stuck `winewrapper.exe`. All of this is gitignored.
- The probe function is `func_0205991c`, the motion-table queue step: 80 bytes of Thumb. Drafts and sweeps are in `extracted/decomp/motion_start/` (`queue_motion_bits.c`, `queue_motion_bool.cpp`, `sweep.py`, `sweep2.py`, results in `sweep*.json` and `sweep2.log`). `tools/check_function.py` compares the linked bytes. Note that `-interworking` takes no argument; `-interworking off` makes every compile fail.
- **Results, all failures.**
  - The 1.2 family gives 84 bytes with 78 differing. The 2.0 family gives 88 bytes with 46 differing.
  - The dsi family at `-O4,p` gives exactly 80 bytes but 69 differ. At `-O2`, `-O3` and `-O4,s` it gives 78 bytes with 67 differing, in both C and C++ `bool` variants.
  - The dsi family is the likely one, since the game's SDK version is `0x05047530`, from the DSi era.
- **Remaining differences from the original.**
  - The original saves `{r3, r4, r5, lr}` and returns with `pop {..., pc}` even though it is a leaf. Every candidate saves only `{r4, r5}`, and the original's 5th-argument offset (`sp+0x10`) follows from that.
  - The original truncates the 4th parameter to a byte on entry.
  - The scheduling differs slightly.
- **First byte-identical match.** `func_0205991c` is reproduced exactly (80 bytes, 0 differing) by `src/motion/queue_motion.c`, built with `mwccarm dsi/1.2p1` or `dsi/1.6sp2` using `-proc arm946e -thumb -O4,p -fp soft -sym on` and **no `-interworking`**. Two things fixed it:
  1. The 4th parameter is an `int` that the source casts to `unsigned char`, which produces the entry truncation. A `char` or `bool` parameter does not.
  2. Leaving interworking off gives the original's `push {r3, r4, r5, lr}` / `pop {..., pc}` frame. With `-interworking` you get `push {r4, r5}` / `bx lr`.
  The verify step is in `extracted/decomp/verify/` (`queue_motion_check.json`, `status: pass`). The ledger entry in `config/decomp_progress.json` pins the source and report hashes, and the README treemap shows 1/8,573.
- **Sweeps** (`extracted/decomp/motion_start/sweep3.py` and `sweep4.py`): `-inline off`, `-g`, `-opt nopeephole` and `-opt noschedule` changed nothing; `-O0` is much worse. Families 1.2 and 2.0 still fail with the cast variant (88 bytes). The C++ member-function variant was never measured because of a wrong mangled-name guess.
- **Second match, and the pinned flags.** `func_020280c8` (68 bytes), the `0x117` play-motion helper, matches from `src/vm/play_motion.c`. It needs `-O4,s`: `-O4,p` builds `push {r3-r7, lr}; sub sp, #8` where the original allocates stack by pushing `{r1-r7, lr}`. `func_0205991c` also matches at `-O4,s`, so `-O4,s` is pinned. The interworking setting is corrected below.
- **`tools/match_function.py <source.c> <reference_symbol> <candidate_symbol> [--compiler dsi/x.y]`** does all of this in one step: compile with the pinned compiler and flags, resolve external symbols from `extracted/lld/linked.elf` (Thumb bits included), link at the original address, compare, and write `extracted/decomp/<symbol>/<compiler>/<candidate>_check.json`. On a failure it prints a side-by-side instruction diff. Use dsd names (`func_XXXXXXXX`, `data_XXXXXXXX`) for externals so they resolve. `src/vm/play_motion.c` therefore calls `func_0205991c`; the name `queue_motion` is local to its own file.
- **Three more matches on the same path, all first-try.** `func_02028148` (20 bytes, `src/vm/current_object.c`) returns the script machine's cached object at `+0x14`, or calls its virtual slot `0x4c`. `func_02008828` (18 bytes, `src/base/is_kind_of.c`) calls the object's virtual slot `0x10` for its type, then `func_02008314(type, kind)`. `func_020265c4` (22 bytes, `src/vm/read_u32.c`) reads a little-endian u32 one byte at a time.
- **Correction: one setting matches everything.** At `-O4,s`, **all eight matches pass with `-interworking`**. The earlier "no interworking" and "set per source file" readings were artifacts of testing at `-O4,p`. The pinned setting is now `mwccarm dsi/1.2p1 -proc arm946e -thumb -O4,s -fp soft -interworking -sym on`, which is the default in `tools/match_function.py`. `--no-interworking` exists for experiments only. Every ledger entry was re-verified with this one setting.
- **Matched so far (8 functions, 238 bytes), all on the Chapter 1 play-motion path or next to it:** `func_0205991c` (queue a motion table), `func_020280c8` (the `0x117` helper), `func_02028148` (current object), `func_0202815c` (no object; returns 0), `func_02028160` (string-table pointer: `code + header code_size + offset`), `func_020265c4` (read u32), `func_02008828` (is-kind-of) and `func_02008314` (type-check tail call to `func_02008320`). Sources are under `src/motion/`, `src/vm/` and `src/base/`. Each C file declares only the struct fields it uses, so a shared header is still to come.
- **Eight more first-try matches (16 functions, 340 bytes in all):** `func_02008348` (returns `&data_020bbc18`, a static type descriptor), `func_02008370` (2-bit field at `+0x1c`), `func_02008364` (tail call into it at `+0x14`), `func_02008350` (bit 0 of that at `+0x28`), `func_02008398` (store a 3-int vector), `func_02008378` (vector add into `out`), `func_020083a0` (`object + 0xb0`) and `func_020083a4` (tail call `func_020083b0(object + 0xb0, source)`). Sources are in `src/base/`, one function per file. That matters: a call to a function in the same object would resolve to the local copy, not the original address.
- **Known miss: `func_02008320`** (type-chain check, 38 bytes). The original reloads `kind->id` on every comparison. Both drafts, a plain while loop and an inline helper, keep it in a register and come out 34 bytes. The drafts are in `extracted/decomp/func_02008320/drafts/`. Things to try: a `for` loop over `type` itself, a C++ member call, or a different optimization level for that file.
- **Eleven more matches (27 functions, 502 bytes in all):** `func_020083c0` (vector subtract), `func_020083e0` (byte bitfield at `+0xdf` from an `int` cast to `unsigned char`), `func_020083fc` (byte at `+0xc2`), `func_02027b08` (word at `+0xf8`), `func_02027bec` (hands the script string table, `file + 0x34 + code_size` with size `string_size`, to `func_020223b8(&data_020ca00c, ...)`), `func_02027c28` (string table base + offset), `func_0202809c` and `func_020280a8` (forwarding at `+0x28` and `+0x14`), `func_020280b4` (low nibble at `+0x1f`, written as a manual mask; a bitfield adds a byte truncation the original doesn't have), `func_02027eb4` (byte at `+0x11`) and `func_020280c4` (empty).
- **More known misses**, with drafts in `extracted/decomp/<symbol>/drafts/`:
  - `func_020083b0` (vector copy): the original loads `y` and `z` before storing `y`. Neither field-by-field copies nor locals reproduce that; next try a struct assignment.
  - `func_02027c08` (string index lookup): the original truncates to `u16` in the found branch and again where the branches meet, then has one `pop`. Early returns and a single result variable both come out 4 bytes short.
- **Seven more matches (34 functions, 604 bytes in all):** `func_02027ea4` and `func_02027eb8` (bit 18 of the word at `+0x10` and bit 0 of the word at `+8`, as 0 or 1), `func_02027ec8` (1 if the ARM function `func_020790c8(holder->item)` returns 0), `func_02027edc` (store two values and clear a count), `func_02027f9c` and `func_02027fa8` (forwarding at `+0x28` and `+0x14`) and `func_02027fb4` (2-bit manual mask at `+0x1c`).
- **Two match-tool fixes for calls into ARM code** (`func_02027ec8`): `tools/match_function.py` now links `extracted/lld/objects/armv5te.o`, so lld may emit `BLX`. External **functions** are now defined in a generated `externals.s` with `.type %function`, assembled by `llvm-mc`. `--defsym` makes untyped absolute symbols, and lld won't interwork with those, so it is now used only for data. Every ledger report was regenerated or re-hashed after this change, and all 34 pass.
- **Eleven more first-try matches (45 functions, 780 bytes in all), from the `0x02008400`-`0x020088b4` stretch of `src/base/`:** absolute value; clear and set bits of the byte at `+0xe` (written as `& 0xfe` and `| 2`); bit 27 of the word at `+0xc`; a second copy of the byte-wise u32 reader (`func_0200842c`); `func_020084f0`, a type-checked forward of three `u16` command fields to `func_02059170`; `func_02008560`, which sets bit 0 and clears bit 1 of the byte at `+0x12` unless bit 2 is set; a slot-8 virtual call with `(0, &data_02083a30)`; first word `>> 8`; first word; and `func_02008898`, which calls two member functions at `+0x1568` and `+0x152c` and returns the object (it looks like a constructor or destructor).
- **Method note:** dumping disassembly with `tail -n +8` drops the first instruction. Use `+7` or the match tool's diff. `func_02027eb4` looked empty because of that.
- **Still open for phase 4:** the dsi versions have produced identical output on every probe so far, including the non-leaf `func_020280c8`. Pinning "exactly one compiler" needs probe functions that tell `dsi/1.1`–`dsi/1.6sp2` apart. Until then, treat the dsi family plus the flags above as the working baseline for new matches.

Commands: see "Running the tools" in `docs/inventory.md`.

## 6. What is known about the assets (summary)

Full detail is in `docs/inventory.md` and `docs/frame_mapping.md`.

**DS ROM**
- ARM9 is 728,888 bytes decompressed, mostly Thumb. There are 7 overlays, all loaded at `0x020d3a40`.
- Scene scripts are `1LMG` VM bytecode, under `stNN/*_Expand.xml`. Videos are Mobiclip `.mods`. Sound is `sound_data.sdat`.
- Graphics are in `cpac_2d.bin` and `cpac_3d.bin` (Capcom pack format, KEY/DAT blocks). The extractor now handles both 4-byte palette keys and 8-byte keys, masking language flags out of byte offsets. Fresh output in `extracted/cpac_checked/` has 24,853 2D entries, including 2,905 palettes, and 1,481 3D entries. Existing `extracted/cpac/` remains for comparison. All old byte-key files are unchanged and all 247 saved trace loads are present in the corrected manifest. See `docs/cpac_format.md`.
- Stage objects' sprite sets are three consecutive block 3 entries: a frame table of (offset, size) pairs, per-frame OAM-style layouts, and graphics. Each is preceded by a block 0 palette.
- The title and UI use `RNAN` triplets.
- The main characters (`cicel`, `linne`) and some small objects use a different, undecoded set layout.

**Remaster (RE Engine, `natives/android/`)**
- File names are MurmurHash3 (seed `0xFFFFFFFF`) of UTF-16LE `natives/android/<path>.<ext>.<version>`; the lowercase path gives one half of the hash, the uppercase path the other. Versions are in `tools/re_versions_android.txt`. `tools/re_names.py` names 82% of `re_chunk_000.pak`.
- Characters live at `charfiles/2d/<group>/stNN/<object>/stNN_<object>_<anim>/`: per-frame textures plus one `.user` RSZ timeline. For example, stage 1 bicycle animation 6 is 5 textures across 195 keyframes, with a time step of 2 or 4, DS-scale x/y positions, and OAM-like packed fields.
- `charfiles/3d` is UI and cut-in images, not models. `SDF` is shader data. Audio is Ogg in `srcd`. UI layouts are 25 `.gui` files `ui0x0000`.

**Mapping findings**
- Stage plus object name is a shared key: DS scripts and remaster folders use the same object identifiers.
- DS frames do **not** map one to one onto remaster frames. The DS has many more drawings; for example, bicycle has 94 distinct DS frames against 32 remaster textures. The port must play a whole remaster timeline when the DS starts an animation.
- DS motion tables contain frame durations and position commands. The full baked remaster position track was not found as a matching array; runtime playback combines the tables with object and script state.
- Ruled out as the DS animation ID: object record field `+0xee`, and script `ActNNN` numbers.
- DS animation sequences are named motion tables. Their labels provide the remaster animation key; the exact-first matching rule and remaining gaps are in section 9.

## 7. Runtime tracing kit (melonDS oracle)

- `tools/melonds.py`: launch and stop melonDS in portable mode. It writes the GDB settings into melonDS's own generated config, because melonDS crashes if its config is hand-written.
- `tools/gdb_remote.py`: a minimal GDB RSP client (read and write memory, registers, breakpoints, interrupt).
- `tools/trace_cpac.py`:
  - Breaks at `main` (`0x02000bec`) and patches both pad readers (`0x0202292c`, `0x020316dc`) to `ldr r0,[pc]; bx lr` plus a data word, so the oracle sets the buttons.
  - Presses Start, then A every 1.5 s.
  - Breaks on the cpac loader `0x0201e664 (pack, block, index)` and on stream `open(path)` at `0x0201efb4`.
  - Reaches `st01/st01_game000_Expand.xml.lz` at about 17.6 s.
- `tools/trace_objects.py`: on each load, captures the stack, dumps RAM, and resolves the owning object name (path `sp+0x28 → +0xb8`).
- `tools/sample_objects.py`: samples a RAM window every 0.3 s.
- Object records are two parallel 0x300-byte arrays near `0x02115xxx` and `0x02118xxx`.
- Quirks:
  - The stub needs a `+` sent first after connecting.
  - Memory reads are capped at about 512 bytes.
  - Breakpoints use kind 2 for Thumb.
  - Only one melonDS instance at a time.
  - Buttons only, no touch injection yet.

## 8. PC reference video: downloaded and verified

Source of truth for verifying animation reproductions, as the user put it: https://www.youtube.com/watch?v=K6Z8QvgObFA. It's "Ghost Trick: Phantom Detective Full Walkthrough Gameplay - No Commentary (PC Longplay)" by Lacry, 9:19:58. Local reference only; never commit it. The audit now blocks `.mp4`, `.webm`, `.mkv` and `.part`.

- **Format:** 398 (1280×720, 60 fps, AV1, about 1.45 GiB).
- **Destination:** `extracted/reference/pc_longplay_720p60.mp4`. It's `.mp4.part` while downloading.
- **Log:** `extracted/reference/download.log`. Download completed on 2026-10-28 in 2:10:30. No downloader needs to be restarted.
- **Recovery command only if the local file is later lost or incomplete:** yt-dlp can continue a `.part` file:
  ```
  toolchain/yt-dlp-venv/bin/yt-dlp --no-playlist -f 398 --newline -o "extracted/reference/pc_longplay_720p60.%(ext)s" "https://www.youtube.com/watch?v=K6Z8QvgObFA" >> extracted/reference/download.log 2>&1
  ```
  A warning about no JavaScript runtime is harmless for this format. Do not install a JS runtime or use cookies, PO tokens or other workarounds without asking. If YouTube refuses, stop and tell the user.
- **Verification command:**
  ```
  ffprobe -v error -show_entries stream=codec_name,width,height,r_frame_rate -show_entries format=duration -of compact extracted/reference/pc_longplay_720p60.mp4
  ```
  Confirmed av1, 1280×720, 60/1, duration 33,598.083333 s. `ffmpeg -v error -xerror -i extracted/reference/pc_longplay_720p60.mp4 -t 180 -an -f null -` completed with exit code 0. Its empty error log is `extracted/reference/decode_check.log`. This checks the opening three minutes, not the whole nine-hour file.
- **Planned use:**
  1. Now: check the decoded remaster timeline units, the tick rate and coordinate scale, against the footage. Example: stage 1 bicycle animation 6, early in chapter 1.
  2. Later: the reference for the Vita renderer's timing and placement. Use it for comparison only; don't reproduce frames in outputs.

## 9. 1LMG decoding: reviewed and re-verified

A background agent was stopped before its final report. The continuation review reran `python3 -I tools/lmg_decode.py`, independently checked header sizes and play-motion targets, and checked the saved runtime traces. All 929 scripts decode cleanly. The mapping denominator changed after making its input scope and matching rule explicit; use the tool and counts below.

What it found (full detail in `docs/lmg_format.md`, summary in `docs/frame_mapping.md`, section "Scene scripts decoded"):
- **Container format.** `1LMG` files are bytecode for a 16-bit VM, not binary XML. Header: magic, unknown, code size, string table size, label table size. Code starts at 0x34. Strings are referenced by u16 offset into the string table. A label table maps names to code offsets.
- **Interpreter.** `func_02023fdc`, the 9.5 KB function with the `add pc` switch, with a 363-entry jump table at `0x02024020`. Opcode lengths came from symbolic execution of the handlers in the earlier session. The review verified all 32,180 saved stage 1 VM steps against decoded boundaries and opcodes, and the decoder checks every nonzero goto/call target. Another 45 trace steps have overwritten saved headers and were outside the stage 1 check.
- **Animations are motion tables.** Each is a labelled data block named `stNN_<object>_<NNN>.mtbl.xml`, made of frame words (`frame = v & 0x3ff`, `ticks = v >> 10`) and commands (tags -1 to -16, lengths listed in the doc). The table player is `func_02007cd4`. Opcode `0x117` plays a table: handler `0x020254f0` → `func_020280c8` → `func_0205991c`, which stores the table pointer at object `+0x148`.
- **Mapping to the remaster.** `python3 -I tools/map_motions.py` finds 4,070 distinct stage animation folder stems under `charfiles/2d` with a same-named `.user` resource in the Android community file list. It matches 3,757 exactly and 250 more through variants: 4,007 matched, 63 unmatched. The earlier 3,670/3,852 count did not reproduce and is superseded. These are file-list naming matches; Steam assets and variant timing remain to verify. The saved 28 motion starts all resolve to decoded stage 1 tables.
- **Port hook.** At `0x117` or `func_0205991c`, drop `.mtbl.xml` and try the full stem first. If it has no remaster folder, try removing one additional `_NNN` or `_NNx` variant suffix after the primary animation number. Preserve the animation ID; an exact split folder takes priority.
- **Verified coverage.** 929 files, 282,425 instructions, 15,914 motion tables, 19,813 message blocks, 2,147 inline blocks, 35 message-only files, and 21,495 of 21,570 `0x117` targets resolved. Header sizes match all files. The zero jump count is 104.
- **Review fixes.** The decoder now rejects bad header sizes, truncated records and empty/failed CLI decodes. Tracers share a checked main-breakpoint/input-patch setup. Motion tracing excludes scheduled interrupts from watch-hit rows. The old auxiliary watch-hit rows are not evidence of memory accesses. The Ghidra helper recognizes unpadded mode-prefixed addresses. Twelve synthetic parser/mapping tests run locally and in CI. The refactored 10-second runtime oracle passed with 100% main/ITCM and overlay 5 matches.
- **Fresh runtime check.** A new 40-second `trace_mtbl.py` run produced 28 starts, all resolved to decoded stage 1 tables, and zero watch hits. The previous trace is preserved locally as `extracted/trace/mtbl_starts.previous.tsv`. `DecompileFunctions.java` compiled and ran in the read-only Ghidra project with `thumb:2023fdc`. The audit passed and its self-test caught all 30 planted files. Live checks need access to the localhost GDB port; the sandbox blocks that connection.
- **Gap investigation after commit `1452430`.** Motion calls now carry their enclosing object in `motion_uses.tsv`. `idle_candidates.tsv` proposes `_000` resources for matching `<object>_wait` or `stNN_<object>` labels. There are 339 candidate tables across 79 stems, covering 23 of the 63 naming gaps; 40 have no candidate under this rule. All candidate tables contain only frame 0 for four ticks, with one loop. Thirteen saved stage 1 starts use them. Candidates remain outside the confirmed 4,007/4,070 naming totals. The bicycle remaster `_000` resource has four textures, so the port needs a verified default pose instead of automatically playing the full timeline for a one-frame wait table. See `docs/lmg_format.md` for the evidence and code path.
- **Zero targets classified, not solved.** All 75 unresolved motion targets are zero; none is an unknown nonzero offset. The VM converts zero into a pointer to the script header, not a null pointer, with no zero check before queueing it. Do not label these stop commands. Reachability and behavior in later scenes still need verification. Eighteen raw system `1LMG` files were also checked; they decode and add no new unique motion names. Seventeen synthetic tests pass after the report changes.
- **Broader alias pass after commit `1f7b210`.** `alias_candidates.tsv` handles different script/art names, one-resource wait families, cross-stage waits, the `sr14` spelling and `_00a`/`_00b`/`_00c` primary variants. It keeps source calls, frame evidence and alternative animation names; it does not add confirmed mappings. `animation_gaps.tsv` classifies the 63 naming gaps as 23 stricter object-name candidates, 37 broader candidates and three without a candidate. The three are `st13_objeffect_neona_000`, `st13_objeffect_neonb_000` and `st14_scr_gyorai_000`. Each exists in pack 002 and references one texture; its DS draw path remains unknown. Cross-stage choices are ambiguous: a stage 13 guitar wait has both remaster guitar 000 and 999 as candidates, and guitar 999 has three textures. Confirmed coverage stays 4,007/4,070.
- **Pack presence check.** `tools/re_pak_index.py` reads metadata from raw packs and complete `.pak` members of ZIP archives, checks entry bounds and ignores split copies. The three local indexes contain 22,595 entries and all 4,265 queried 2D `.user` paths from the Android file list. This is index presence, not full payload or visual validation. Output is `extracted/motion_map/pak_presence.tsv`; the command is in `docs/lmg_format.md`. Twenty-six synthetic tests pass after the alias and archive-index changes.
- **CPAC and cell resource lookup after commit `191b13f`.** The extractor fix above has nine synthetic regression tests. `tools/cell_resources.py` follows the literal pointers in `func_0200b598` to the language, packed-index and four-component descriptor tables. Resource 3751 reproduces the known bicycle palette 144 and sprite set 13999. All 4,088 distinct `begin` IDs from the decoded corpus were checked: one zero/no-resource ID and 4,087 resolved IDs, with every one of their 16,347 nonzero component references present in the corrected manifest. Details and the explicit-ID command are in `docs/frame_mapping.md`.
- **Neon class paths found before scope narrowing.** `uObjEffectNeonA` setup `0x020559a8` loads resource 7006 (palette 58, sprite set 6904); `uObjEffectNeonB` setup `0x020559f8` loads 7007 (palette 59, set 6907). Shared native update/draw moves and renders them without a named motion-table start. This is a static binding proposal, not later-stage runtime or remaster verification. `st14_scr_gyorai_000` remains unidentified. Keep these findings, but defer further work on them for the Chapter 1 target.
- **Scoped stage inventory.** `python3 -I tools/map_motions.py --stage st01` filters scripts and the remaster denominator and writes separate output under `extracted/motion_map/st01/`. It preserves the full-game reports. The first result is 82 files, 1,454 motion tables, 352/366 naming matches and 2,298/2,298 resolved motion calls, with no zero targets or jumps. Fourteen art stems are unmatched; six have stricter pose candidates and one more has a broader candidate. Seven have no stage-local candidate, which does not prove they are used in Chapter 1. The stage folder includes later scenes; trace reachability before promoting these to blockers. Forty-three synthetic tests pass after all current changes, and the audit/self-test pass.

New and changed files:
- `docs/lmg_format.md`: the format write-up.
- `docs/frame_mapping.md`: a new section on the mapping.
- `tools/lmg_decode.py`: decodes every `.dec` into a text listing under `extracted/lmg/`. Message text and data blocks are omitted; only their byte counts appear.
- `tools/map_motions.py`: writes `motions.tsv`, `unmatched_remaster.tsv`, `unresolved_plays.tsv`, `motion_uses.tsv`, `idle_candidates.tsv`, `alias_candidates.tsv` and `animation_gaps.tsv` under `extracted/motion_map/`. Generated data remains gitignored. Mapping rows can outnumber motion tables because a label can match multiple full folder paths.
- `tools/re_pak_index.py`: checks named-resource presence without extracting pack contents. `tools/re_names.py` now has a main guard so the existing hash functions can be imported. `tests/test_re_pak_index.py` covers index bounds and ZIP-contained packs with split duplicates.
- `tests/test_lmg_tools.py` and `.github/workflows/audit.yml`: synthetic regression checks, with no game data.
- `tools/cpac_unpack.py`, `tests/test_cpac_unpack.py` and `docs/cpac_format.md`: corrected key formats, language flags, bounds and manifest quoting.
- `tools/cell_resources.py` and `tests/test_cell_resources.py`: explicit cell resource lookup from the USA main segment, with seven synthetic tests.
- `docs/chapter1.md`: the current delivery scope, acceptance conditions, dependencies and work order.
- `tools/trace_lmg.py`: melonDS trace of the VM's pc, used to check opcode lengths.
- `tools/trace_mtbl.py`: melonDS trace of motion-table starts.
- `tools/ghidra/DecompileFunctions.java`: now takes `thumb:`/`arm:` address prefixes and disassembles and creates a function when none exists.

Still open:
- the 63 remaster animation stems without a confirmed name match: 23 have stricter default-pose candidates, 37 have broader alias candidates, and three have no named-table candidate
- the runtime behavior and reachability of the 75 zero `0x117` targets, plus the 104 jumps to 0
- the meaning of header field +0x04
- full names for most opcodes
- confirming the motion-command meanings beyond their lengths

## 10. Next steps: deliver the Chapter 1 test

Follow `docs/chapter1.md`. Keep all prepared work, but defer later-stage effect tracing, whole-game alias cleanup, full-pack payload surveys and a whole-game matching-C campaign.

1. Trace the first chapter's entry, completion transition and actual scene/resource dependencies. Start at the existing `st01_game000` oracle path. The whole `st01` folder includes later scenes and is only a conservative inventory.
2. Locate/configure VitaSDK for this repository and make an installable viewer with a small converted asset set. No Vita application exists in this repository yet. Measure frame time and free memory; ask the user to power on the Vita when the build is ready.
3. Bring up the Chapter 1 DS logic with Vita file, memory, timing, input, renderer and audio replacements. The byte-identical ARM build is a reference; DS hardware calls still require replacement. Omit DS Protect. The user's owned Steam copy still needs to be available for authoritative remaster conversion.
4. Implement and verify only the required chapter art, motions, default poses and single-screen UI. Use the downloaded reference to calibrate tick rate, placement and transitions. Keep existing broader data and tooling intact.
5. Package a playable opening-to-chapter-completion build, verify retry/checkpoint behavior, then collect the user's on-device feedback. The remaining game is a stretch goal if that test works well and the user wants to proceed.

## 11. Gotchas learned the hard way

- **zsh:** a glob with no matches aborts `&&` chains. Use explicit paths. macOS has no `timeout` command.
- **Python:** the python.org 3.12 build has no CA bundle (`CERTIFICATE_VERIFY_FAILED`). Don't disable verification; use `curl` or yt-dlp's own HTTP code.
- **Ghidra:** auto-analysis isn't repeatable (6,288 vs 6,284 functions), which is why `symbols/database.txt` is the source of truth. Script helper names can clash with `GhidraScript` methods (`parseAddress`).
- **dsd and lld:** `dsd`'s objects need `tools/elf_fixups.py` before lld will link them: reorder the symbol table, set the Thumb bit on FUNC symbols, retype `R_ARM_PC24` to `CALL`/`JUMP24`, and add an ARMv5TE attributes object. `dsd`'s extract stores ARM9 with the compressed-end field zeroed and DS Protect decrypted; `dsd rom build` restores both.
- **ROM rebuild:** needs `tools/fix_header.py`. It copies the secure-area CRC at `0x6C` from the base ROM and recomputes the header CRC, because `dsd` can't compute the first without an ARM7 BIOS.
