# Frame mapping: DS animations to remaster frames

Goal: when the recompiled DS code draws a character's animation frame, know which remaster texture to draw instead.

Sources: DS ROM (USA, `BGTE`), remaster Android `re_chunk_000.pak` named with `tools/re_names.py`. As in the inventory, the remaster results come from the Android build and should be rechecked on Steam.

## DS side: how sprites are stored

`cpac_2d.bin`, as initially unpacked by `tools/cpac_unpack.py` (some counts and interpretations below are superseded by the runtime findings because the unpacker assumes 8-byte keys):

| Block | Entries | Contents |
|---|---|---|
| 0 | 1 | 4 KB table of 15-bit colors in a smooth ramp, probably a color lookup |
| 1 | 8 | 3 KB entries, palette-like |
| 2 | 251 | background tile and fill data |
| 3 | 19,941 | character sprites as repeating triplets: an `RNAN` cell animation (usually one frame), a small cell-layout record, an LZ-compressed tile block. About 6,650 frame sets. |
| 4 | 156 | large background bitmaps |

There is no index in the pack that maps objects or animations to entries. That mapping lives in the code or the scene scripts.

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
- **Unpacker bug:** `tools/cpac_unpack.py` assumes 8-byte keys everywhere. `cpac_2d` block 0 uses 4-byte keys, so the unpacker shows one entry there, but the game loads indices up to 2,876.

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
