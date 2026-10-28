# Handoff: Ghost Trick PS Vita port

For the next agent picking up this project. Updated 2026-10-28 after the decoder and mapping review. Read this file, then `docs/inventory.md` and `docs/frame_mapping.md`.

## 1. What the project is

The goal is a PS Vita port of Ghost Trick: Phantom Detective, using the **2023 remaster's art and UI** on top of the **original DS game logic**.

Decided direction, after earlier feasibility work:
- **The remaster's own binaries can't be ported.** The Android APK, Windows and Steam builds are RE Engine, 64-bit (arm64 or x86-64), Vulkan or Direct3D, and need about 3 GB. The Vita is 32-bit ARMv7 with 512 MB RAM and vitaGL (GLES-like).
- **The plan:** recompile the DS game's ARM9 code natively for the Vita (ARMv5TE code runs mostly unchanged on ARMv7), replace its renderer and UI, and draw remaster assets converted offline from the user's own Steam copy. The UI follows the single-screen Android remaster, not the DS's two screens.
- **The asset finding:** neither version uses skeletal animation. The remaster's characters are high-resolution 2D art driven by keyframe timelines. Details are in section 6.

The `BFM-decomp` wiki phase ladder (https://github.com/Druthulu/BFM-decomp/wiki/Start-a-new-decomp-project) is the user's chosen source of truth for sequencing. It was written for PlayStation and gcc, so the DS equivalents are used. Phases 0–3 are done (section 5).

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
- **Remaster (Android repack):** the APK above. Its `assets/re_chunk_000.pak` and the `data01`/`data02` Play Asset Delivery packs (`re_chunk_001.pak`, split `re_chunk_002.pak`) are extracted to `extracted/remaster_apk/`. Only `re_chunk_000.pak` has been scanned.
- **Community file list:** `extracted/remaster_apk/GTPD_ANDROID_Release.list`, from Ekey/REE.PAK.Tool. Its license is unclear, so never commit it. A `GTPD_STM_Release.list` for Steam also exists upstream.
- **PC reference video:** see section 8. It is downloading now.
- **Disk:** about 43 GiB free at the continuation check. `extracted/` takes several GB. Be careful with big outputs.

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
| 4–8 Matching decomp | Not started | Open decision for the user: recompilation can start from the phase 3 objects. Matching C needs proprietary Metrowerks `mwccarm`, and the user must approve any download of it. |

Commands: see "Running the tools" in `docs/inventory.md`.

## 6. What is known about the assets (summary)

Full detail is in `docs/inventory.md` and `docs/frame_mapping.md`.

**DS ROM**
- ARM9 is 728,888 bytes decompressed, mostly Thumb. There are 7 overlays, all loaded at `0x020d3a40`.
- Scene scripts are `1LMG` VM bytecode, under `stNN/*_Expand.xml`. Videos are Mobiclip `.mods`. Sound is `sound_data.sdat`.
- Graphics are in `cpac_2d.bin` and `cpac_3d.bin` (Capcom pack format, KEY/DAT blocks). Some blocks use 4-byte keys; `tools/cpac_unpack.py` wrongly assumes 8-byte keys everywhere. That's a known bug: `cpac_2d` block 0 really has about 2,900 palette entries.
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

## 8. In flight: the PC reference video (keep tabs on this)

Source of truth for verifying animation reproductions, as the user put it: https://www.youtube.com/watch?v=K6Z8QvgObFA. It's "Ghost Trick: Phantom Detective Full Walkthrough Gameplay - No Commentary (PC Longplay)" by Lacry, 9:19:58. Local reference only; never commit it. The audit now blocks `.mp4`, `.webm`, `.mkv` and `.part`.

- **Format:** 398 (1280×720, 60 fps, AV1, about 1.45 GiB).
- **Destination:** `extracted/reference/pc_longplay_720p60.mp4`. It's `.mp4.part` while downloading.
- **Log:** `extracted/reference/download.log`. Progress: `rg '^\[download\]' extracted/reference/download.log | tail -1`. During the gap investigation on 2026-10-28 it was still advancing at about 62.5%, around 237 KiB/s, with roughly 40 minutes remaining. Check the live log rather than relying on that snapshot, and verify progress before starting another downloader.
- **If it stopped** (the earlier session's background shell may die with that session), resume with the same command; yt-dlp continues the `.part` file:
  ```
  toolchain/yt-dlp-venv/bin/yt-dlp --no-playlist -f 398 --newline -o "extracted/reference/pc_longplay_720p60.%(ext)s" "https://www.youtube.com/watch?v=K6Z8QvgObFA" >> extracted/reference/download.log 2>&1
  ```
  A warning about no JavaScript runtime is harmless for this format. Do not install a JS runtime or use cookies, PO tokens or other workarounds without asking. If YouTube refuses, stop and tell the user.
- **When done, verify:**
  ```
  ffprobe -v error -show_entries stream=codec_name,width,height,r_frame_rate -show_entries format=duration -of compact extracted/reference/pc_longplay_720p60.mp4
  ```
  Expect av1, 1280×720, 60/1, duration about 33,598 s. Then decode-check the first minutes.
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

New and changed files:
- `docs/lmg_format.md`: the format write-up.
- `docs/frame_mapping.md`: a new section on the mapping.
- `tools/lmg_decode.py`: decodes every `.dec` into a text listing under `extracted/lmg/`. Message text and data blocks are omitted; only their byte counts appear.
- `tools/map_motions.py`: writes `motions.tsv`, `unmatched_remaster.tsv`, `unresolved_plays.tsv`, `motion_uses.tsv` and `idle_candidates.tsv` under `extracted/motion_map/`. Generated data remains gitignored. Mapping rows can outnumber motion tables because a label can match multiple full folder paths.
- `tests/test_lmg_tools.py` and `.github/workflows/audit.yml`: synthetic regression checks, with no game data.
- `tools/trace_lmg.py`: melonDS trace of the VM's pc, used to check opcode lengths.
- `tools/trace_mtbl.py`: melonDS trace of motion-table starts.
- `tools/ghidra/DecompileFunctions.java`: now takes `thumb:`/`arm:` address prefixes and disassembles and creates a function when none exists.

Still open:
- the 63 remaster animation stems without a confirmed name match: 23 have default-pose candidates, 40 do not
- the runtime behavior and reachability of the 75 zero `0x117` targets, plus the 104 jumps to 0
- the meaning of header field +0x04
- full names for most opcodes
- confirming the motion-command meanings beyond their lengths

## 10. Suggested next steps (offer these to the user as choices)

1. Verify the 23 default-pose candidates and investigate the 40 other naming gaps. Keep default poses separate from full animation playback; `idle_candidates.tsv` gives source calls and frame evidence. The 75 zero motion targets need later-scene reachability checks before their semantics can be assigned.
2. Finish verifying the video (section 8). Then calibrate the remaster timeline decode (tick rate, coordinate scale) against it and against the DS motion table's ticks and offsets for the same animation.
3. Decode the main characters' sprite-set layout (`cicel`, `linne`).
4. Fix `tools/cpac_unpack.py` for 4-byte-key blocks.
5. Scan `re_chunk_001.pak` and `re_chunk_002.pak`, and get the Steam copy onto the Mac to recheck the remaster results against `GTPD_STM_Release.list` (prefix `natives/stm/`, with its own version table).
6. Start the recompilation (phase 3 → recomp) or phase 4. That's the user's decision; phase 4 needs Metrowerks tools, so ask first.
7. When the user re-enables the Vita: a first on-device viewer (vitaGL) that loads converted remaster frames and reports frame time and free memory.

## 11. Gotchas learned the hard way

- **zsh:** a glob with no matches aborts `&&` chains. Use explicit paths. macOS has no `timeout` command.
- **Python:** the python.org 3.12 build has no CA bundle (`CERTIFICATE_VERIFY_FAILED`). Don't disable verification; use `curl` or yt-dlp's own HTTP code.
- **Ghidra:** auto-analysis isn't repeatable (6,288 vs 6,284 functions), which is why `symbols/database.txt` is the source of truth. Script helper names can clash with `GhidraScript` methods (`parseAddress`).
- **dsd and lld:** `dsd`'s objects need `tools/elf_fixups.py` before lld will link them: reorder the symbol table, set the Thumb bit on FUNC symbols, retype `R_ARM_PC24` to `CALL`/`JUMP24`, and add an ARMv5TE attributes object. `dsd`'s extract stores ARM9 with the compressed-end field zeroed and DS Protect decrypted; `dsd rom build` restores both.
- **ROM rebuild:** needs `tools/fix_header.py`. It copies the secure-area CRC at `0x6C` from the base ROM and recomputes the header CRC, because `dsd` can't compute the first without an ARM7 BIOS.
