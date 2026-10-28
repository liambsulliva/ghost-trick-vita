# Frame mapping: DS animations to remaster frames

Goal: when the recompiled DS code draws a character's animation frame, know which remaster texture to draw instead.

Sources: DS ROM (USA, `BGTE`), remaster Android `re_chunk_000.pak` named with `tools/re_names.py`. As in the inventory, the remaster results come from the Android build and should be rechecked on Steam.

## DS side: how sprites are stored

The corrected `tools/cpac_unpack.py` extracts 24,853 nonempty entries from `cpac_2d.bin`:

- Block 0: 2,905 palettes, with four-byte keys.
- Block 1: 8 entries, palette-like.
- Block 2: 307 background tile and fill entries, 304 compressed.
- Block 3: 21,477 sprite entries, including 2,489 `RNAN` resources, custom frame tables, layouts and graphics.
- Block 4: 156 large background entries.

These replace the original counts, which misread palette keys and discarded some language variants. All 247 loads in the saved runtime trace now have manifest entries. See `docs/cpac_format.md` for the key formats and extraction checks.

Resource IDs map to palettes and sprite entries through tables in ARM9, described below. Object and motion names come from the scene scripts or class setup code.

The scene scripts (`stNN/*_Expand.xml`, `1LMG` bytecode) refer to on-screen objects by name. Those names are the same identifiers the remaster uses for its `charfiles` folders. So stage plus object name is a shared key between the two versions.

## Remaster side: how animations are stored

`charfiles/2d/<group>/stNN/<object>/stNN_<object>_<anim>/`, with one texture per frame plus one `.user` file per animation.

- 415 stage-scoped objects, 2,552 animations, 8,263 frame textures.
- Animation IDs are sparse per object. For example, the stage 1 bicycle has 0 and 4 to 9, which suggests the numbers come from an older scheme.

One `.user` file, stage 1 bicycle animation 6, decoded:

- A resource list of the animation's 5 frame textures, then RSZ instance data.
- 195 keyframe records. Each holds:
  - an array of the 5 texture names
  - an atlas size (2048×2048) and a rectangle (232×348)
  - a time counter that moves in steps of 2 or 4 ticks
  - an x/y position in DS-scale pixels (x 122 to 468, y 61 to 140)
  - a keyframe index
  - several packed 16-bit pairs that look like DS sprite (OAM) attributes

The timing and coordinates look DS-derived. Capcom appears to have converted DS animation behavior into the remaster and drawn new art for each frame.

## The full remaster position track was not found on the DS

I searched every extracted DS file (22,586 files: scene scripts, `cpac` entries, ARM9, overlays) for the bicycle's position track:
- encodings: absolute, relative to the first keyframe, per-frame delta, and fixed point (`x<<12`, `x*16`, `x*256`)
- value sizes of 8, 16 and 32 bits, at every stride up to 64 bytes

The only matches were short coincidences. One relative-x run matched 8 of 193 keyframes, and y didn't match at any offset.

This search did not find the remaster's baked position track. The later motion-table decode below does find DS frame durations and position commands; runtime playback combines them with the object and script state.

The later frame-count comparison rules out a per-frame texture replacement. The DS code owns object world position and game logic, while the renderer plays the corresponding remaster timeline, including its local positions and timing. The reference video is still needed to calibrate that playback.

## Debug strings in the code

`cCellAnim`, `cAnimSequencer`, `mCellAnim`, `mAnimCtrl.currentTime` and `AnimFixed` are each referenced once:

- `cAnimSequencer` and `cCellAnim` are referenced from `.init` static initializers, probably class registration.
- `mCellAnim` is used by `func_0202feb4`, and `mAnimCtrl.currentTime` by `func_0200b864`. Decompiled (`tools/ghidra/DecompileFunctions.java`), both register member fields by name with their offsets, through a shared helper: `mCellAnim` at `+0x28` of its owner, and the anim-control fields at `+0xc` and `+0x10`. They belong to a reflection or serialization system, not animation logic.

That reflection data will help name struct fields later. It does not identify the code that selects an animation.

## Runtime trace

### Code

- **Pad reads:** `func_0202292c` and `func_020316dc` are the SDK pad read. Each wraps a 32-byte reader that combines the key register `0x04000130` with the X/Y bits from shared memory, then masks out opposite D-pad directions. Each has one caller.
- **Pack setup:** `func_0202ef90` opens `cpac_2d.bin` and `cpac_3d.bin` through a stream class. The vtable is at `0x020a4670`, and `+0x10` is `open(path)` at `0x0201efb4`. It wraps each stream in a 0xcc-byte pack object.
- **Entry loader:** `func_0201e664(pack, block, index)` reads one entry from a pack. It looks up the block's key table, which uses 4-byte keys (offset and size in 32-byte units) or 8-byte keys, keeps a small cache keyed by the index's low byte, and returns an entry reader. `func_0201e888`, with 44 call sites, wraps it to allocate and decompress.
- **Unpacker fix:** `tools/cpac_unpack.py` now reads both key formats and masks the language flag out of byte offsets. Block 0 contains 2,905 palettes; the saved trace loads indices up to 2,876.

### Input injection

At `main` (`0x02000bec`), after crt0 has decompressed the ARM9 code and before any pad read, the oracle rewrites both pad-read wrappers in RAM as `ldr r0, [pc, #0]; bx lr` followed by a data word. Writing that word through the GDB stub sets the buttons the game sees.

### `tools/trace_cpac.py`

The tracer runs melonDS with breakpoints on the entry loader and on `open(path)`. It presses Start three times, then A every 1.5 seconds, and logs to `extracted/trace/`.

A 90-second run reaches stage 1:
- 1.7 s: both `cpac` files open.
- 6 s: title screen, sprite sets 7,282 to 7,388.
- 17.1 to 17.6 s: system text, `route`, `st01/st01_root`, then `st01/st01_game000_Expand`.
- 17.6 to 17.8 s: the scene loads 24 `cpac_2d` block 3 sprite sets, in two ranges: 13,999 to 14,202 and 6,912 to 8,433.
- In all: 247 loads, from `cpac_2d` blocks 0, 2, 3 and 4 and `cpac_3d` blocks 0 and 3.

Block 3 sets always load as three consecutive entries, `k+1`, `k`, `k+2`. That confirms the triplet layout, so a set is named by its first index.

## Objects identified at load time

`tools/trace_objects.py` runs the same scripted session. For every `cpac_2d` block 3 load after `st01_game000_Expand` opens, it saves the registers and 0x200 bytes of stack. 1.5 s later it dumps main RAM (`extracted/trace/ram_st01_game000.bin`). Then, offline, it follows register and stack pointers up to two levels deep and looks for strings that are known stage 1 object names.

Two names (`telephone`, `pikapika`) show up at fixed stack slots in every load, so they are context. The owner of each set comes through a stable path: `sp+0x28` then `+0xb8` for a set's first load, and `sp+0x1a0` or `sp+0x1c0` then `+0xe0` for the other two. That names all 20 sprite sets in the scene, for example bicycle 13999, cicel 7513, linne 8248 and killer 7978. Each object loads exactly one sprite set.

Before each object's set, the game loads one `cpac_2d` block 0 entry, with indices 141 to 441, often shared between objects. Block 0 uses 4-byte keys with sizes in 32-byte units, so those entries are palettes.

## DS sprite set layout for stage objects

Stage objects do not use `RNAN` triplets. For the bicycle (set 13999):

- `13999`: frame table, `(offset, size)` pairs into `14001`, 112 slots. The first 9 slots are placeholders pointing at offset 0.
- `14000`: per-frame sprite layouts. A table of 16-bit offsets, then 6-byte records that look like DS OAM attributes.
- `14001`: frame graphics, about 100 KB.

None of the three holds animation sequences, meaning which frames make up animation N and for how long. That data is not in `cpac_2d` blocks 0 or 3, so it is probably in the scene scripts or the code. `cicel`, `linne`, `cbag`, `ltel`, `guitar`, `light` and `gomi` parse as one-slot frame tables, so their sets use a different layout. The main characters in particular need their own look.

## DS frames do not map one to one onto remaster frames

| Object | DS distinct frames | Remaster textures | Remaster animations |
|---|---|---|---|
| bicycle | 94 | 32 | 7 |
| container | 59 | 11 | 3 |
| hasigo | 36 | 5 | 2 |
| hook | 55 | 7 | 3 |
| kasa | 64 | 5 | 2 |
| rope | 116 | 11 | 3 |
| shadanki | 71 | 4 | 1 |

The DS animates with many pre-drawn frames. The remaster uses a few drawings moved by keyframes: bicycle animation 6 plays 5 textures across 195 keyframes, each with its own position.

**What this means for the port:** a per-frame texture swap is not possible. The hook has to sit at the start of an animation: when DS code starts animation N on object X, the port plays the remaster's whole `stNN_X_N` timeline, with its textures, keyframe positions and timing, in place of the DS frames. The DS code still owns the object's world position and game logic.

## Looking for the DS animation ID (two hypotheses ruled out)

Object records: every stage object's name is referenced from two parallel arrays of 0x300-byte records, around `0x02115xxx` and `0x02118xxx`, plus a name table around `0x0214axxx`. Addresses come from the scripted run, which is deterministic.

`tools/sample_objects.py` read `0x02114000`–`0x0211c000` every 0.3 s for 25 s of the first stage 1 scene (65 samples). Each field near each object's name pointer was scored on two counts: does it change, and does it only take values that exist as remaster animation IDs for that object?

1. **Record field `+0xee` (second array) is not the animation ID.** `cicel` goes 0→3 and `linne` 0→6, both valid. But every change happens at the same instant (7.5 s), and `flag` 2→0, `freezer` 0→4, `gomi` 0→5 and `shadanki` 0→1 take values the remaster does not have. It looks like a scene or action state that switches for all objects at once.
2. **Script act numbers are not the animation IDs.** Scene scripts name per-object actions `<object>ActNNN`. Of 27 stage 1 objects found in both, 8 match exactly, all of them objects whose only ID is 0. The rest disagree; for example, bicycle has acts 0, 10, 20, 30, 40 against remaster IDs 0, 4–9. So act numbers are script states.

The later script decode resolves the remaining question: animations have named motion tables, and those names supply the animation key.

## Earlier search routes (resolved by the script decode)

It is now known which sprite set each object in the first stage 1 scene loads. Every load goes through the shared wrapper, so the immediate caller does not identify the object. A raw search of the scene script for the set numbers only found noise, because `1LMG` appears to refer to strings by index.

At that point, the DS animation IDs and their relationship to the remaster's numbers were still unknown.

The two routes considered were:

1. **Decode the scene scripts' `1LMG` format.** The scripts are the most likely home of the DS animation definitions: which frames an object plays, for how long, in which act. Decoding works offline and covers every stage at once, with no emulator run. Each DS sequence can then be matched to a remaster `stNN_<object>_<anim>` by frame count and timing.
2. **Trace the frame player.** Find the code that reads an object's frame table each tick, and log (object, frame index) over a scene. That gives ground truth for one scene at a time, which can check route 1.

## Scene scripts decoded: DS animations are named motion tables

`docs/lmg_format.md` describes the `1LMG` format, and `tools/lmg_decode.py` decodes all 929 scripts into `extracted/lmg/`. The scripts are bytecode. A DS animation is a motion table: a labelled data block named `<stage>_<object>_<NNN>.mtbl.xml`, made of frame words (frame index plus ticks) and offset commands. Opcode `0x117` plays one on the current object.

The names line up with the remaster. A rerun on 2026-10-28 with `tools/map_motions.py` matches 4,007 of 4,070 distinct animation stems in the Android file list: 3,757 exactly and 250 more through variant suffixes, leaving 63 unmatched. These counts cover stage folders under `charfiles/2d` with a same-named `.user` resource; they replace the earlier unverified 3,670/3,852 count. They measure naming coverage in the file list, with Steam and variant timing still to verify. The stage 1 bicycle has tables 000 (split into `_000_000` to `_000_003`) and 004 to 009, matching remaster IDs 0 and 4 to 9.

So the hook for the port is opcode `0x117`, or `func_0205991c`, which it reaches. Drop `.mtbl.xml` and try the complete stem first. If no remaster folder matches, remove one additional `_NNN` or `_NNx` variant suffix after the primary animation number. Preserve that primary number: `st01_bicycle_004` stays unchanged; `st01_bicycle_006_00a` can use `st01_bicycle_006`. An exact split folder takes priority over folding. The earlier negative results stand: act numbers and record field `+0xee` are not animation IDs.

Run `python3 -I tools/map_motions.py` to write the full mapping, unmatched folders and unresolved `0x117` targets under `extracted/motion_map/`. All generated data stays out of git. See `docs/lmg_format.md` for reproduced decoder totals and the limits of the runtime evidence.

## Default poses need an additional lookup

The next gap investigation found object-local labels such as `guitar_wait.mtbl.xml` and `door_wait.mtbl.xml`, plus names without a numeric suffix such as `st06_sofa.mtbl.xml`. The new `motion_uses.tsv` report records their calling objects. `idle_candidates.tsv` proposes an existing `stNN_<object>_000` resource only when the calling object and shorter table name agree. This identifies candidates for 23 of the 63 unmatched stems, leaving 40 without a candidate. The confirmed naming totals remain 4,007/4,070.

These are default-pose candidates, not verified animation aliases. All 339 candidate tables contain just frame 0 for four ticks, and 13 starts in the saved stage 1 trace use them. The remaster's bicycle `_000` resource references four textures, so starting its whole timeline for the DS's one-frame `bicycle_wait` could introduce unwanted movement. The hook needs stage, object and label context, plus a verified way to select and hold the default pose. Check this against the video and Steam before promoting candidates.

All 75 unresolved play-motion targets are zero. The VM turns zero into the script header pointer rather than a null pointer; its later-scene behavior remains unverified. See `docs/lmg_format.md` for the code path. The tool reports these explicitly as `zero_target` and does not turn them into mappings.

## Naming gaps after the broader alias pass

`alias_candidates.tsv` now includes cases where script object names differ from art names, shared initializer calls, one-resource families with nonzero animation IDs, cross-stage resources, and two stage 14 naming patterns. `animation_gaps.tsv` keeps these proposals separate from the confirmed mappings: 23 gaps have the stricter object-name candidate, 37 have broader candidates, and three have no named-table candidate. Cross-stage alternatives remain explicit, including both guitar 000 and 999 for a stage 13 wait table. Confirmed coverage remains 4,007/4,070.

The three remaining names are scenery effects: `st13_objeffect_neona_000`, `st13_objeffect_neonb_000` and `st14_scr_gyorai_000`. They exist in the complete local `re_chunk_002.pak` and each references one texture. The neon class paths are now identified below; the gyorai scenery binding remains open. `tools/re_pak_index.py` verifies resource presence directly in raw packs or their containing ZIP archives; all 4,265 queried 2D `.user` paths were found across the three local pack indexes. See `docs/lmg_format.md` for the candidate rules, archive layout and reproduction command.

## Resource IDs resolve through ARM9 tables

The first argument of a motion table's `begin` command is a cell resource ID, not a CPAC entry index. `func_0200b598` resolves the ID through three tables in the USA ARM9 main segment:

- `0x02086f84`: five language-specific u16 IDs per localized resource. When bit `0x8000` is set, use the low 15 bits as the row and the language byte at `0x020cff60`, minus one, as the column.
- `0x02088910`: one packed u16 per resolved resource ID. Its upper 14 bits select a descriptor; the low two bits select the cell-animation sequence.
- `0x0208c064`: four u16 indices per descriptor: palette, graphics, cell layout and animation/frame table. A zero component means it is absent. Palette indices refer to CPAC 2D block 0; the other three refer to block 3.

The table pointers are literals at `0x0200b750`, `0x0200b754` and `0x0200b758`. The loader's component mask uses bits 1, 2, 4 and 8 in that same order. `func_0200af44` loads palettes, `func_0200af34` loads graphics, and `func_020420e0` loads the cell and animation data from block 3. The last component can hold an `RNAN` resource or the custom stage-object frame table; its index is not a remaster animation number.

`tools/cell_resources.py` reads those pointers and writes metadata for explicit IDs. It is specific to this USA main segment; it does not scan other releases for equivalent tables. Language is a numeric selector from 1 to 5, defaulting to 1.

```
python3 -I tools/cell_resources.py extracted/nds/segments/main.bin extracted/motion_map/cell_resources.tsv 3751 4666 7006 7007
```

Resource 3751 resolves to palette 144, graphics 14001, cell 14000 and frame table 13999, agreeing with the saved bicycle load. Across all 929 decoded scripts, motion `begin` commands use 4,088 distinct IDs. One is zero, which selects no resource; the other 4,087 resolve without lookup errors. Every one of their 16,347 nonzero component references exists in the corrected CPAC manifest. This checks table bounds and entry presence, not the contents of every sprite set.

## Neon scenery uses class setup instead of a named motion table

The class registration at `0x020804bc` names `uObjEffectNeon`, `uObjEffectNeonA` and `uObjEffectNeonB`. Their factory and setup paths identify two hooks outside opcode `0x117`:

- `uObjEffectNeonA`: creator `0x02055784`, object vtable `0x020ad474`, setup `0x020559a8`. Setup calls `func_0200b598(object + 0x28, 7006, 0xf)`. Resource 7006 resolves to palette 58, graphics 6906, cell 6905 and animation 6904, sequence 0.
- `uObjEffectNeonB`: creator `0x020557ac`, object vtable `0x020ad4f4`, setup `0x020559f8`. It loads resource 7007 with the same mask. This resolves to palette 59, graphics 6909, cell 6908 and animation 6907, sequence 0.

Their shared update is `0x02055948`. When its gates allow an update, it advances the fixed-point x position by `0x7000` or `0x9000`, wrapping through `0` to `0xff000`. These are increments of 7 or 9 DS pixels per eligible update, not a measured rate per second. Shared draw `0x020558f8` updates the cell animation, applies object position and camera subtraction, then calls `func_0200ba7c` on the embedded cell renderer at `object + 0x28`.

The matching class and art names support using these setup paths for `st13_objeffect_neona_000` and `st13_objeffect_neonb_000`. This is a static binding proposal: neither the later-stage runtime behavior nor remaster pose, timing and placement has been verified. It does not change the confirmed named-table coverage of 4,007/4,070.

`st14_scr_gyorai_000` still needs a binding. The script's `torpedo` object uses a different named resource, `st14_03gyorai-SV_000.mtbl.xml`, whose `begin` ID 4666 resolves to palette 484 and sprite set 19249. That does not establish an alias for the separate scenery resource. The generic `uScrObj` class draws through `0x0205a5f8` and `0x02058e38`, but its class path alone does not identify which instance, if any, uses the scenery art.
