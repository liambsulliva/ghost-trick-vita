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
- Scene and data files hold 59,906 references to textures under `charfiles/`, which looks like per-frame character art.
- The largest unknown format is `SDF` (348 files, 424 MB). It is the most likely home of the animation timing, but it has not been decoded.

**What this means for the port.** Swapping assets means swapping frames. When the DS code asks `cCellAnim` for animation A, frame F, the new renderer draws the remaster's frame for the same animation. There are no high-poly models to bring over. What the remaster adds is higher-resolution 2D art, Ogg audio and new UI layouts.

## Blockers

| Blocker | Evidence | Status |
|---|---|---|
| Recompiling the DS code | ARM9 is 728,888 bytes decompressed. The 7 overlays add 122 KB, and all of them load at `0x020d3a40`, so only one is resident at a time. The code is ARMv5TE, which the Vita's ARMv7 CPU mostly runs natively. | Feasible. Small for a DS game. |
| Game logic | About 1,500 scene scripts in a binary format with the magic `1LMG`, one set per stage (`st01` to `st15`), plus 5 translations each. The interpreter is in ARM9. | Good sign: the logic is mostly data. |
| Matching remaster files to DS files | The remaster references `.xml` files under stage folders named like the DS ones (902 references), so the scene structure looks shared. | Promising. Not proven. |
| Remaster file names are hashed | The pack's table of contents has hashes only. 1,892 data files contain readable path strings, which can be hashed to recover the names. | Next step. The hash scheme has not been verified on this game. |
| Character frame mapping | The DS has 2,004 cell sequences; the remaster has `charfiles` textures plus `SDF`. | Unknown until `SDF` is decoded. This is the biggest open question. |
| Texture memory | 2,644 textures, 1.76 GB. Common sizes are 4096², 2048², 1920×1080 and 1440×1080. The format codes (1038, 1086, 1087) look like mobile ASTC. | The converter must re-encode to formats the Vita's GPU reads, at a size chosen per chapter. The Steam formats are not checked yet. |
| UI | The remaster has 25 GUI layouts with numeric IDs (table below). The DS UI is drawn by code using `cpac_2d` graphics. | Each layout has to be identified, and its DS draw site found. |
| Video | 17 Mobiclip `.mods` cutscenes on the DS. The remaster references 10 `.mov` files under `@GUI`. | FFmpeg includes a Mobiclip decoder. Re-encode offline for the Vita's hardware video decoder. |
| Audio | DS: one 15 MB `SDAT`. Remaster: 600 Ogg streams at 44.1 kHz (362 MB). | Play the remaster audio. Map DS sound IDs to remaster tracks. |

## Sequencing

The order follows the phase ladder in the BFM-decomp wiki ("Start a new decomp project"). That guide targets a PlayStation project built with gcc. DS games are built with Metrowerks CodeWarrior (`mwccarm`), so the platform and compiler steps are swapped for DS ones.

| Phase | Milestone from the guide | DS version | Status |
|---|---|---|---|
| 0 Governance and firewall | ROM audit fails on a planted file and passes on the tree; CI green | `.gitignore` blocks ROM, APK, pak and bin files and `extracted/`. The audit script and CI are not written yet. | Partly done |
| 1 Deterministic extraction | One command extracts; a rerun reproduces the manifest | `nds_unpack.py` writes `manifest.txt` with SHA-1 hashes. Two runs produced identical manifests. | Done |
| 2 Oracles and load map | Disassembly database exported as text; runtime oracle; load addresses proven | Ghidra (not installed) for the database. A DS emulator with a debugger as the runtime oracle. ARM9 loads at `0x02000000`, overlays at `0x020d3a40`. | Not started |
| 3 All-assembly baseline | Every binary rebuilds byte-identical from assembly | ds-decomp toolkit to split and rebuild the ROM | Not started |
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
```

`re_pak_scan.py` needs the `zstd` command on the PATH.

## Not examined yet

- `data01` and `data02` (Play Asset Delivery packs, about 2 GB)
- The Steam copy
- The formats `SDF`, `FBFO` (high entropy, maybe video or encrypted), `bws` and the `cpac` frame-layout records
