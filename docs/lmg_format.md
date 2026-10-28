# The `1LMG` scene script format

The DS scene scripts (`stNN/*_Expand.xml.lz`, `system/*`, `route.xml`, `chapter.xml` and so on) are not binary XML. They are compiled bytecode for a 16-bit virtual machine, plus a string table and a label table. The XML-like nesting comes from block opcodes that each carry a jump to the end of their block.

`tools/lmg_decode.py` decodes every `.dec` file under `extracted/nds_plain` into an indented text listing under `extracted/lmg/`. Message text and inline data blocks are left out of the listing and shown only as byte counts.

Confidence: **code** means read from the disassembly, **trace** means confirmed with melonDS, **data** means inferred from the files only.

## File layout

All values are little-endian.

| Offset | Size | Contents | Confidence |
|---|---|---|---|
| 0x00 | 4 | magic `1LMG` | data |
| 0x04 | 4 | unknown. Nonzero in scene scripts (stage 1 `game000`: 0x2fc), zero in language files | not decoded |
| 0x08 | 4 | code size in bytes | code (`func_02027bec`, `func_02027c98`) |
| 0x0c | 4 | string table size | code |
| 0x10 | 4 | label table size | code |
| 0x14 to 0x33 | | zero | data |
| 0x34 | code size | bytecode, 16-bit words | code |
| after code | string size | NUL-terminated strings | code |
| after strings | label size | label table | code |

The sizes add up exactly to the file size in every one of the 929 files.

**Strings** (code). An operand that names a string is a u16 byte offset into the string table: `func_02027c28` returns `table_base + value`. Offset 0 is always `*`. The earlier attempt to read strings from 0x740a missed the table start (in stage 1 `game000` the table begins at 0x7408) and also read operands at the wrong boundaries.

**Labels** (code, `func_02027c98`, lookup in `func_02027d4c`). A u32 count, then that many `(name_offset, target)` pairs, then a block of names. `name_offset` is relative to the start of the name block. `target` is a file offset into the code. The names are compiler labels (`_L$0004`), block names (`st01_game000`, `game`, `gameAct000`, `bicycle`, `bicycleAct010`), message IDs (`m01_0000`) and motion tables (`st01_bicycle_004.mtbl.xml`). Code looks labels up by name at runtime, for example to start an act.

**Language files** (`*.en.xml`, `*.fr.xml` and so on) use the same container. Their code section holds only message blocks, labelled `mNN_NNNN`. The loader (`func_02051cd4`) loads the base script first and then the language file.

## The virtual machine

The interpreter is `func_02023fdc` (Thumb, 9.5 KB). Its machine object holds `+4` code base (file + 0x34), `+8` pc in halfwords, `+0xc` wait counter, `+0x1c/+0x20` a 4-deep call stack. It reads the u16 at `code[pc]`, rejects values above 0x16a, and dispatches through a 363-entry halfword jump table at `0x02024020`. Each handler advances pc by a fixed amount (code). Some handlers call virtual methods: the base vtable is at `0x020a4cb4`, and two subclasses at `0x020abe14` and `0x020abec4` change a few lengths. The table in the decoder uses the `0x020abe14` subclass, which is the one that runs scene scripts.

**Opcode lengths** come from symbolically executing every handler in the disassembly and recording how much each path adds to pc. They were checked in three ways:
1. A melonDS trace of 32,180 VM steps in stage 1 (`tools/trace_lmg.py`): every executed offset lands on an instruction boundary.
2. Every jump target in all 929 files lands on an instruction boundary.
3. Every file decodes to exactly the end of its code section.

**Jumps** (code). `0x22` and `0x23` are goto, `0x2d` is call and `0x2e` is return. Goto and call take a u32 file offset; return has no operand. The jump handler computes `pc = (target - 0x34) / 2`. 104 jumps (goto or call) have target 0, presumably unresolved; the decoder does not treat them as errors.

**Conditional skips** (code, trace). `0x33` and `0x34` are one word long. They either fall through to the next instruction, which is always a `0x22` goto, or skip over it (pc + 4). `0x15` is the same: it always skips the goto that follows it.

**Block opcodes** (code, trace). Each one is followed by a goto to the end of its block, so a pass that walks the blocks executes only the headers.

| Opcode | Layout (16-bit words) | Meaning |
|---|---|---|
| `0x15` | `15, 22, end:u32` | script root, named by the first label (`st01_game000`) |
| `0x16` | `16, 22, end:u32, name:str` | object (`game`, `bicycle`, `cicel`) |
| `0x19` | `19, 22, end:u32, name:str` | act (`bicycleAct010`). Registered by vtable `+0x20`, body at +5 words |
| `0x1a` | `1a, 22, end:u32, sender:str, event:str` | event handler (`SYSTEM` `GAME START`, `core1` `action`, `EVENT` `shot`). Registered by `func_020511c0`, body at +6 words |

Blocks end with `0x113` (handler), `0x112` (act) and `0x0000`. These ops do not advance and wait (code); what ends a block is not fully worked out.

**Other opcodes worth knowing:**
- `0x1d` `target:str, verb:str, arg:str` sends a message (`game EVENT next`).
- `0x2f n` waits n frames.
- `0x117` `mtbl:u32, a, b` plays a motion table on the current object. Its handler at `0x020254f0` calls `func_020280c8`, which calls `func_0205991c`, which stores the table pointer at object `+0x148`.
- `0x12e` is followed by a goto over an inline data block. The block looks like Shift-JIS text and is probably a developer comment.

String operand positions in the decoder come from tracking operands that flow into `func_02027c28` (35 opcodes). U32 operand positions come from tracking operands passed to the unaligned u32 reader `func_020265c4` (29 opcodes). For `0xb0` this probably picks up a pair of s16 values, not an offset. All other operands are shown as signed 16-bit numbers. Opcode names other than those above are not assigned.

## Motion tables (`*.mtbl.xml`)

Motion tables are data blocks inside the code section, each starting at a label named `<anim>.mtbl.xml`. The player is `func_02007cd4` (code): a record whose u16 is 0xfff0 or above is a command, and anything else is a frame word.

- **Frame word:** `frame = v & 0x3ff`, `ticks = v >> 10`. One word long. The frame indexes the object's sprite set frame table.
- **Commands** (length in words, including the tag):

| Tag | Length | Use in the player |
|---|---|---|
| -3 | 3 | begin, `(a, b)`, via `func_02008734`. For example, `a` is 0xea7 to 0xead across the bicycle's tables |
| -4 | 5 | box or two points; the start scan jumps here |
| -12 | 3 | set object offset `(x, y)`, in pixels shifted by 12 |
| -11 | 4 | set offset of layer `n` `(n, x, y)` |
| -16 | 4 | `func_02059170(obj, a, b, c)` |
| -5 | 3 | frame plus explicit duration |
| -13, -10, -6 | 2 | set a single value |
| -9 | 3 | sound or effect id |
| -8 | 11 | spawn an effect, with a string at word 6 |
| -7 | 4 | send a message (three strings) |
| -2 | 1 | end |
| -1 | 1 | loop back to word 3 |

The tag meanings beyond those lengths are a first reading of the player code. The lengths parse all 15,914 tables exactly.

## Coverage

`python3 -I tools/lmg_decode.py` reads all 929 `1LMG` files without errors: 282,425 instructions, 15,914 motion tables, 19,813 message blocks and 2,147 inline blocks. 35 of the files hold only messages.

These totals were rerun on 2026-10-28. The decoder checks the header sizes and instruction bounds and exits nonzero for decode errors or an empty input corpus. The saved stage 1 trace has 32,180 VM steps, all at decoded instruction boundaries with matching opcodes. Another 45 steps belong to earlier scripts whose saved memory headers were overwritten; they are outside that check. All 28 saved motion starts resolve to stage 1 motion tables. The old trace's auxiliary watch-hit rows included scheduled interrupts and are not evidence of memory accesses.

## Animation mapping

The DS names its animations exactly the way the remaster names its folders. Stage 1's script holds `st01_bicycle_004.mtbl.xml` through `st01_bicycle_009.mtbl.xml`, and the remaster has `st01_bicycle_004` through `_009`.

`python3 -I tools/map_motions.py` reproduces the mapping from decoded DS labels and the local Android community file list. It considers stage animation folders under `charfiles/2d` with a same-named `.user` resource. There are 4,070 distinct folder stems: 3,757 match DS labels exactly, and 250 more match after removing one additional `_NNN` or `_NNx` variant suffix (x is a lowercase letter). That gives 4,007 matched stems and 63 unmatched. The earlier 3,670/3,852 count was not reproducible and is superseded by this explicit scope. This is file-list naming coverage; it does not verify that all listed assets exist in the scanned pack or prove the timing and visual equivalence of every variant. Steam still needs checking.

Match the full stem first. Only if it has no remaster folder, try removing an additional variant after the primary animation number. For example, `st01_bicycle_004` stays unchanged and `st01_bicycle_006_00a` can match `st01_bicycle_006`. A split with its own exact remaster folder keeps that folder.

The tool writes the full mapping, unmatched remaster folders and unresolved play-motion targets under gitignored `extracted/motion_map/`. Of the 21,570 `0x117` play-motion instructions, 21,495 point at a decoded motion table; 75 remain unresolved. The 104 zero jump targets were also reproduced.

The two runtime traces back this up: the 28 motion starts traced in stage 1 land on these tables, among them `bicycle_wait`, `st01_cbag_000` and `st01_linne_004`.

### Default poses and shorter motion names

The naming pass above does not cover every motion start. Many DS tables use an object-local name such as `guitar_wait.mtbl.xml`, `Light_wait.mtbl.xml` or `door_wait.mtbl.xml`. Another form omits the animation number, such as `st06_sofa.mtbl.xml`.

`tools/map_motions.py` now writes `motion_uses.tsv`, which records the object enclosing each `0x117` call, its target and motion labels. Object scope ends at the block's end offset; a shared function outside an object block has no inferred owner. It also writes `idle_candidates.tsv`. A candidate requires a call on the matching object, either `<object>_wait` or `stNN_<object>` as the table name, and an existing `stNN_<object>_000` remaster folder. The report includes the call sites, DS frame words and loop flag. These candidates do not change `motions.tsv` or the confirmed naming totals.

The 2026-10-28 gap investigation found 339 such tables across 79 remaster animation stems. Twenty-three of the 63 unmatched stems have candidates; 40 have none under this rule. All 339 tables have one frame word, frame 0 for four ticks; one also has a loop command. Thirteen starts in the saved stage 1 trace use these tables, including the bicycle, guitar, light and telephone. This confirms that the shorter names occur at runtime, while the association with remaster `_000` remains an inference from names and object ownership.

The static pose needs separate treatment from timeline playback. In the locally scanned Android pack, the guitar, light and telephone `_000` resources each reference one texture, but bicycle `_000` references four. A one-frame DS default table therefore does not justify playing a whole `_000` remaster timeline. The renderer needs a verified default pose or timeline segment. Keep stage and object context with the motion label when resolving these aliases, and check the pose against the reference video and Steam assets before promoting a candidate.

### Broader alias candidates

The next pass after commit `1f7b210` traces the remaining naming gaps without requiring the script object and art resource to have the same name. For example, `power_box` starts `02dbox_wait`, and `remote_stand` starts `r-stand_wait`. Other calls live in shared initializers: `microscope_wait` is started in `func_microscope_init`, which the microscope object calls with `0x2d`. A blank enclosing object in `motion_uses.tsv` does not mean that the table is unused.

`alias_candidates.tsv` records broader proposals for currently unmatched animation stems. Every row keeps the source table, object scope where available, call offsets, frame words, reason and alternative animation names. The reasons are:

- `wait_name`: `<asset>_wait` matches an existing `stNN_<asset>_000`, even if the script object has a different name.
- `single_animation_wait`: the same family has one numbered resource, such as stage 1 `patolamp` animation 010 or stage 3 `02rench` animation 001.
- `unnumbered_default`: a name such as `st06_sofa` has an existing `_000` folder.
- `stage_prefix_spelling`: the DS label `sr14_02lamp_000` has the same remainder as remaster `st14_02lamp_000`.
- `primary_letter_variant`: DS `st14_levlamp_00a`, `_00b` and `_00c` have a possible common remaster `_000` resource. The latter two DS tables contain six-frame sequences, so segment selection still needs verification. The rule is limited to `_00x`; other lettered numeric schemes are not inferred.
- `cross_stage_wait`: the same asset name appears in another stage's resource folders. These can have multiple alternatives. For example, a stage 13 `guitar_wait` has both stage 1 guitar 000 and 999 as name candidates; the report preserves both and does not choose one.

`animation_gaps.tsv` summarizes all 63 confirmed naming gaps. Twenty-three retain the stricter object-name candidate, 37 have only broader alias candidates, and three have none: `st13_objeffect_neona_000`, `st13_objeffect_neonb_000` and `st14_scr_gyorai_000`. The confirmed naming coverage remains 4,007/4,070. These candidate reports do not promote aliases into `motions.tsv`.

All three effect resources are present in `re_chunk_002.pak` and each references one texture. Their relation to DS scenery or procedural rendering is still unknown; a single texture does not establish that the effect is static. Guitar 999 is present in `re_chunk_001.pak` and references three textures, which is another reason not to equate its whole timeline with a one-frame wait table.

`tools/re_pak_index.py` checks named-resource hashes against raw `.pak` files and complete `.pak` members inside ZIP archives. It reads headers and entry tables, validates data bounds, and skips split `.pak.0`/`.pak.1` copies. The local `data01` archive contains a complete 794,538,139-byte `re_chunk_002.pak` as well as its partial `.0` copy; `data02` contains complete `re_chunk_001.pak` and the `.1` tail. No concatenation or additional large extraction was needed. A check of all three packs found all 4,265 queried `charfiles/2d` `.user` paths from the Android file list in 22,595 total pack entries. These 4,265 paths include names outside the narrower 4,070 numeric stage-animation stems. This verifies index presence, not the contents or visual correctness of every timeline.

Reproduce the presence check with:

```
python3 -I tools/re_pak_index.py extracted/remaster_apk/GTPD_ANDROID_Release.list extracted/motion_map/pak_presence.tsv extracted/remaster_apk/re_chunk_000.pak extracted/remaster_apk/data01 extracted/remaster_apk/data02
```

### Zero motion targets

All 75 unresolved `0x117` targets are zero; there are no unresolved nonzero motion targets in this corpus. `unresolved_plays.tsv` now includes the enclosing object and a reason, `zero_target` or `unresolved`. This is classification, not resolution.

The handler at `0x020254f0` computes `code_base + ((target - 0x34) & ~1)` at `0x0202550c`–`0x02025518`. Because `code_base` is file base plus 0x34, target zero becomes the file header address. If the current object passes the motion-capability check, `func_020280c8` passes `target >> 1` as the motion ID and `func_0205991c` queues the pointer at object `+0x148`. There is no zero-target check along that path. Object update `func_0205964c` consumes that queued pointer and then clears it. Zero cannot be classified as a stop or null-motion sentinel from this code. Whether these instructions execute in their scenes, and how they behave if reached, remains unverified. The port should not infer an animation folder from them.

The 929-file coverage figures refer to the catalog's decompressed `.dec` corpus. Eighteen uncompressed `1LMG` system files in the ROM were checked separately: they decode without errors and add no new unique motion-table names. They do not explain the remaining stage animation gaps.

See `docs/frame_mapping.md` for what this means for the port.
