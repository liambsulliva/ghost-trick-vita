# Inventory and blockers

What was examined:

- DS ROM, USA release (`BGTE`), through `tools/nds_unpack.py`, `tools/nds_catalog.py` and `tools/cpac_unpack.py`.
- Remaster, Android v1.0.0: only `assets/re_chunk_000.pak`, through `tools/re_pak_scan.py`. The two Play Asset Delivery archives (`data01`, `data02`, about 2 GB of further `.pak` and `.dat` files) were listed but not scanned.
- The Steam copy has not been examined. Everything said about the remaster below must be rechecked against it.

All extracted output goes to `extracted/`, which is gitignored.

## Are the animations riggable?

No. Neither version uses skeletal animation for characters, so there is nothing to rig or retarget.

**DS.** Characters are 2D cell (sprite) animation.

- The ARM9 code contains the classes `cCellAnim`, `cAnimSequencer` and `mAnimCtrl.currentTime`, and no bone, joint or skeleton names.
- `cpac_2d.bin` (81.8 MB) holds 1,977 `RNAN` cell-animation files and about 18,000 entries of frame layouts and tile pixels.
- `cpac_3d.bin` (6.4 MB) is mostly textures (961 `sdt` entries) plus 310 `bws` structures that reference them. No skeleton data was found, but `bws` has not been decoded.

**Remaster.** Characters are high-resolution 2D, not rigged models.

- The main pack has 15 meshes (0.4 MB in total). None of them contain bone names, and the pack has no motion or skeleton files.
- Character art lives under `charfiles/2d/`, one folder per stage, then per character, then per animation. Each animation folder holds one texture per frame (`stNN_<character>_<anim>_<frame>_imlN.tex`) and one `.user` file, which probably holds the frame timing. The `.user` file has not been decoded.
- `charfiles/3d/` is not 3D models. It holds face cut-ins, backlog, profiles, chapter cards, ending and effect images.
- `SDF` (348 entries, 424 MB) is not animation. In the file list, `.sdf` and `.vsdf` live under `systems/`, so it is most likely shader data.

**What this means for the port.** Swapping assets means swapping frames. When the DS code asks `cCellAnim` for animation A, frame F, the new renderer draws the remaster's frame for the same animation. The remaster's layout (stage, character, animation, frame) suggests the key. The DS ROM also groups content by stage. Whether the DS animation numbers match the remaster's is still unproven. There are no high-poly models to bring over. What the remaster adds is higher-resolution 2D art, Ogg audio and new UI layouts.

## Blockers

| Blocker | Evidence | Status |
|---|---|---|
| Recompiling the DS code | ARM9 is 728,888 bytes decompressed. The 7 overlays add 122 KB, and all of them load at `0x020d3a40`, so only one is resident at a time. The code is ARMv5TE, which the Vita's ARMv7 CPU mostly runs natively. | Feasible. Small for a DS game. |
| Game logic | About 1,500 scene scripts in a binary format with the magic `1LMG`, one set per stage (`st01` to `st15`), plus 5 translations each. The interpreter is in ARM9. | Good sign: the logic is mostly data. |
| Matching remaster files to DS files | The remaster references `.xml` files under stage folders named like the DS ones (902 references), so the scene structure looks shared. | Promising. Not proven. |
| Remaster file names are hashed | Verified scheme: MurmurHash3 (seed `0xFFFFFFFF`) of the UTF-16LE path `natives/android/<path>.<ext>.<version>`, plus an optional language or platform suffix. The lowercase path gives one half of the hash and the uppercase path the other. Paths found inside the game data name 4,268 of the 6,478 entries in `re_chunk_000.pak`. Adding the community list `GTPD_ANDROID_Release.list` (REE.PAK.Tool) names 5,293. | Solved for the Android build. A Steam list (`GTPD_STM_Release.list`) also exists. |
| Character frame mapping | The DS has 2,004 cell sequences. The remaster has a folder per animation with one texture per frame and one `.user` file. | The key looks like stage, character, animation, frame. The next step is decoding a remaster `.user` file and the DS animation index. This is the biggest open question. |
| Texture memory | 2,644 textures, 1.76 GB. Common sizes are 4096², 2048², 1920×1080 and 1440×1080. The format codes (1038, 1086, 1087) look like mobile ASTC. | The converter must re-encode to formats the Vita's GPU reads, at a size chosen per chapter. The Steam formats are not checked yet. |
| UI | The remaster has 25 GUI layouts with numeric IDs (table below). The DS UI is drawn by code using `cpac_2d` graphics. | Each layout has to be identified, and its DS draw site found. |
| Video | 17 Mobiclip `.mods` cutscenes on the DS. The remaster references 10 `.mov` files under `@GUI`. | FFmpeg includes a Mobiclip decoder. Re-encode offline for the Vita's hardware video decoder. |
| Audio | DS: one 15 MB `SDAT`. Remaster: 600 Ogg streams at 44.1 kHz (362 MB). | Play the remaster audio. Map DS sound IDs to remaster tracks. |

## Sequencing

The order follows the phase ladder in the BFM-decomp wiki ("Start a new decomp project"). That guide targets a PlayStation project built with gcc. DS games are built with Metrowerks CodeWarrior (`mwccarm`), so the platform and compiler steps are swapped for DS ones.

| Phase | Milestone from the guide | DS version | Status |
|---|---|---|---|
| 0 Governance and firewall | ROM audit fails on a planted file and passes on the tree; CI green | `.gitignore` blocks ROM, APK, pak and bin files and `extracted/`. `tools/rom_audit.py` reads its forbidden extensions and magic bytes from `tools/audit_rules.txt`. When a local manifest exists, it also flags any file whose hash matches a ROM file. `--self-test` must catch every planted file before the tree check is trusted. `.github/workflows/audit.yml` runs both on every push. | Done locally. CI not yet seen green. |
| 1 Deterministic extraction | One command extracts; a rerun reproduces the manifest | `nds_unpack.py` writes `manifest.txt` with SHA-1 hashes. Two runs produced identical manifests. | Done |
| 2 Oracles and load map | Disassembly database exported as text; runtime oracle; load addresses proven | **Load map:** `tools/nds_loadmap.py` reads the SDK module params and autoload list. Segments are main `0x02000000` (0xb18a0), ITCM `0x01ff8000` (0x620), DTCM `0x027e0000` (0x60), static BSS to `0x020d3a40`, and 7 overlays at `0x020d3a40`. The sizes add up exactly to the autoload list and the decompressed file size. **Load addresses:** `tools/nds_prove_bases.py` counts absolute pointers to the segment's own strings and function prologues. Main scores 7,108 at its base against at most 1,945 at any shifted base. Six overlays pass the same 2× margin. ITCM, DTCM and overlay 2 are too small to measure, so their addresses rest on the SDK tables. **Database:** Ghidra 12.2 headless (`tools/ghidra_build.sh`). `symbols/database.txt` is the source of truth: 15 blocks with SHA-1 hashes and 6,284 functions with entry, ARM/Thumb mode, name and exact body. Rebuilding with no analysis gives an identical export, checked on two runs. Fresh auto-analysis is not repeatable (6,288 functions one run, 6,284 the next), which is why the text is the truth. **Runtime oracle:** `tools/runtime_oracle.py` runs melonDS 1.1 (official macOS release, kept in gitignored `extracted/melonds/` in portable mode) through its built-in GDB stub. `tools/gdb_remote.py` is a small GDB remote-protocol client, so no separate debugger is needed. The oracle boots halted, checks the program counter against the header entry, runs the game for 10 seconds, interrupts it and compares live memory with the extracted segments. Results: entry `0x2000800` matches. Main and ITCM match 100%, so ITCM is now proven at runtime. DTCM matches 87.5%, which is expected because it holds data the game changes. The overlay region matches overlay 5 exactly (next best 16%). Overlay 2 and the other overlays were not resident at that moment, so they are not runtime-checked. | Done |
| 3 All-assembly baseline | Every binary rebuilds byte-identical from assembly | `dsd` 0.12.1 (ds-decomp, macOS arm64 release, kept in gitignored `toolchain/`). **Unchanged round trip:** `dsd rom extract` then `dsd rom build` differs from the base ROM in only 4 header bytes. They are the secure-area checksum at `0x6C`, which `dsd` can only compute with an ARM7 BIOS, and the header CRC at `0x15E`. `tools/fix_header.py` copies the checksum from the user's base ROM and recomputes the header CRC after checking its CRC code against the base ROM's stored value. After that the rebuilt ROM is byte-identical (SHA-1 `bcfe74e5…`). **`dsd init`:** fails on an upstream bug, ds-decomp issue #81, still open. The Thumb function at `0x02020cfc` starts at a 4-aligned address and its second instruction is a Thumb `b`, so `dsd` reads it as ARM, the parse fails, and the function is never found. The caller at `0x02021bae` uses a Thumb `BL`, so the target is Thumb (Ghidra agrees). **Local `dsd` patch** (`tools/patches/dsd-0.12.1-thumb-retry.patch`, applied to the v0.12.1 source, built with Homebrew Rust into `toolchain/dsd`; the official binary is kept as `toolchain/dsd-0.12.1-official`): (1) when an ARM parse fails on a word whose top byte is `0xe0`–`0xe7`, retry the same address as Thumb, which fixes #81; (2) skip the "illegal function start" heuristic for known call targets and for functions already in `symbols.txt`. The function at `0x020267c8` starts with `lsrs r2, r1, #8`, which that heuristic treats as a byte table. **`dsd init`** then succeeds. One large function (`0x02023fdc`, about 9.5 KB, ending in a Thumb `add pc` switch) is real. Ghidra had cut it short at 68 bytes. **`dsd delink`** succeeds after one hand-added symbol: `data_ov000_020d59e0` in `ov000/symbols.txt`, the boundary of an encrypted blob. **Overlay 0 is DS Protect** (anti-piracy, encrypted code). The port should leave it out. It produces 9 objects, one per module, and a Metrowerks-syntax LCF. A second hand edit removes `func_ov000_020d59ec` and its relocation. Those bytes are part of DS Protect's encrypted blob and only happen to decode as an ARM `bne`. **Linking:** LLVM `ld.lld` 23 (Homebrew `lld`) through `tools/lld_link.py`. The script places every input section at the exact address from `delinks.txt` and links all modules in one pass with `--no-check-sections`, since overlays share `0x020d3a40`. It defines `<MODULE>_<SECTION>_START/_END` and `__exception_table_start__/_end__`, then cuts each module out with `llvm-objcopy`. Three object fix-ups (`tools/elf_fixups.py`) bridge the gap between `dsd`'s Metrowerks-style output and lld. (1) `llvm-objcopy` rewrites the symbol table so locals come first. (2) Thumb `FUNC` symbols get bit 0 set; `dsd` only marks Thumb with `$t` mapping symbols. (3) `R_ARM_PC24` relocations are retyped to `R_ARM_CALL` for `BL`/`BLX` and `R_ARM_JUMP24` for other branches. This lets lld emit `BLX` instead of veneers. An extra empty object declares ARMv5TE in `.ARM.attributes`. **Result:** all 10 modules (main, ITCM, DTCM, overlays 0 to 6) match `dsd`'s extract byte for byte. `tools/phase3_check.sh <rom>` delinks, links, swaps the linked binaries into a copy of the extract, rebuilds the ROM with `dsd` and applies the header fix-up. The result is byte-identical to the base ROM (SHA-1 `bcfe74e5…`). **Config:** the `dsd` config with both hand edits now lives in the repo at `config/arm9/`. | Done |
| 4 Compiler pinned | Probe functions byte-identical under one compiler | Find the `mwccarm` version and flags | Not started |
| 5 to 8 Census, multipliers, permuter, campaign | Matching C, measured coverage | Matching-decomp tooling for DS | Not started |
| 9 Publish | A fresh clone rebuilds from the user's own dump | Same | Not started |
| 10 Readability | Clean, still byte-identical | Same | Not started |

**Open decision.** A recompilation can start from the split assembly in phase 3. Phases 4 to 8 produce matching C source, which makes the hooks easier to write and read but isn't needed for the game to run. The guide says to do the phases in order.

## UI inventory

The target is the remaster's single-screen layout. The remaster's layouts carry numeric IDs only. Identifying them needs either a GUI-format decoder or screenshots from a running remaster.

| Remaster layout | Screen | DS draw site | Vita buttons and touch |
|---|---|---|---|
| `GUI/Fade/MainFade.gui` | screen fade | | |
| `systems/gui/ImagePlane_1920x1080.gui` | full-screen image | | |
| `ui000000` | | | |
| `ui000100` | | | |
| `ui000200` | | | |
| `ui000201` | | | |
| `ui000400` | | | |
| `ui000500` | | | |
| `ui000600` | | | |
| `ui010000` | | | |
| `ui010100` | | | |
| `ui010300` | | | |
| `ui010400` | | | |
| `ui010700` | | | |
| `ui010701` | | | |
| `ui020000` | | | |
| `ui020100` | | | |
| `ui020300` | | | |
| `ui020400` | | | |
| `ui030000` | | | |
| `ui030100` | | | |
| `ui030200` | | | |
| `ui040000` | | | |
| `ui040100` | | | |
| `ui040200` | | | |
| `ui040300` | | | |

DS UI-related files outside the stage folders: `system/language_select`, `system/database_0000_Expand` (likely the in-game profiles), `system/system_0000.msg` (system text), the fonts (`*.imb`, `font8x8.bin`, `CharWidth*.bin`, `PixLine*.bin`) and the 247 compressed entries in `cpac_2d` block 2.

## Running the tools

```
python3 -I tools/nds_unpack.py "<your rom>.nds" extracted/nds
python3 -I tools/nds_catalog.py extracted/nds/files extracted/nds_plain
python3 -I tools/cpac_unpack.py extracted/nds/files/cpac_2d.bin extracted/cpac
python3 -I tools/cpac_unpack.py extracted/nds/files/cpac_3d.bin extracted/cpac
python3 -I tools/re_pak_scan.py <remaster>/re_chunk_000.pak extracted/remaster/pak_entries.tsv
python3 -I tools/re_names.py <remaster>/re_chunk_000.pak tools/re_versions_android.txt extracted/remaster/names.tsv [optional .list file]
python3 -I tools/rom_audit.py --self-test
python3 -I tools/rom_audit.py
python3 -I tools/nds_loadmap.py "<your rom>.nds" extracted/nds
python3 -I tools/nds_prove_bases.py extracted/nds
./tools/ghidra_build.sh
```

`python3 -I tools/runtime_oracle.py "<your rom>.nds" [seconds]` needs melonDS 1.1 unzipped to `extracted/melonds/melonDS.app` with an empty `extracted/melonds/portable/` folder next to it. On the first run it lets melonDS write its default config, then switches on the GDB stub and switches off the JIT, editing values in place. melonDS crashes if its config is written by hand. The oracle opens a melonDS window while it runs.

ROM round trip with `dsd`:

```
./toolchain/dsd rom extract --rom "<your rom>.nds" --output-path extracted/dsd/extract
./toolchain/dsd rom build --config extracted/dsd/extract/config.yaml --rom extracted/dsd/roundtrip.nds
python3 -I tools/fix_header.py "<your rom>.nds" extracted/dsd/roundtrip.nds
shasum "<your rom>.nds" extracted/dsd/roundtrip.nds
```

Phase 3 baseline, from linked objects to the ROM:

```
./tools/phase3_check.sh "<your rom>.nds"
```

It needs the extract above in `extracted/dsd/extract`, the patched `dsd` in `toolchain/dsd`, and `brew install llvm lld`. To rebuild the patched `dsd`: clone ds-decomp at tag `v0.12.1` into `toolchain/src/ds-decomp`, `git apply` `tools/patches/dsd-0.12.1-thumb-retry.patch`, run `CARGO_NET_GIT_FETCH_WITH_CLI=true cargo build --release -p ds-decomp-cli`, and copy `target/release/dsd` to `toolchain/dsd`.

`ghidra_build.sh` needs `brew install ghidra`. It rebuilds the Ghidra project from `symbols/database.txt` and fails if the re-export differs. It only runs a full auto-analysis when that file is missing. The Ghidra project itself lives in `extracted/ghidra/` and is never committed.

`re_pak_scan.py` and `re_names.py` need the `zstd` command on the PATH. `tools/re_versions_android.txt` holds the path prefix and the version number for each file extension in the Android build. The Steam build will need its own table. Community `.list` files stay in `extracted/` and never go into the repo.

## Not examined yet

- `re_chunk_001.pak` and `re_chunk_002.pak`, the rest of the remaster, stored in the Play Asset Delivery archives `data01` and `data02` (about 2 GB)
- The Steam copy
- The remaster's per-animation `.user` files, `FBFO` (high entropy, maybe video or encrypted), the DS `bws` structures and the `cpac` frame-layout records
