# Handoff: Ghost Trick PS Vita port

For the next agent. Updated 2026-10-29. **Active task: continue autonomously through the full port function inventory, making local commits along the way.** The user extended the task beyond 7104 to all functions; 7104 is now an intermediate milestone, not a stopping point. Include ARM functions after the current Thumb work. Chapter 1 remains the later on-device delivery goal. The user authorizes local commits they can adjust later.

## 0. Active task: match the full function inventory

User instruction: "I already committed it. Keep going til 7104." The 3552 snapshot is `d996db0` on `asset-inventory` ("3552 total functions! Mostly six-arg constructors, alloc-init wrappers, and vtable stores"). Do not rewrite it.

Newer instruction on 2026-10-29: "you can continue past 7104 and decompile all functions without my input, as long as you make commits along the way." This supersedes the old stopping point. Continue with verified local checkpoint commits, keep the README treemap current at those checkpoints, and do not ask for permission merely to continue. DS Protect remains outside the port inventory.

### Status (2026-10-29)

| Item | Value |
|---|---|
| Previous user snapshot | 3552 (`d996db0`, preserved) |
| Verified ledger / treemap | 4455 |
| Needed for 7104 milestone | 2649 more |
| Remaining in full inventory | 4118 |
| Current compilation | Remaining ARM proposals, followed by larger Thumb batches; one Wine process |
| dsd inventory (treemap denominator) | 8573 (DS Protect overlay excluded) |
| Untracked new sources | `src/thumb/*.c` (one function per file) |
| Batch scripts | gitignored under `extracted/decomp/batch_more*.py` |

Brace count can overcount: `generate` may rewrite a failed body, and `defined_names()` then treats it as done. A function counts toward 7104 only after `link_one` / `match_function.py` reports pass. Delete C that fails.

Fresh continuation check: all 414 pending sources passed a new pinned compile and linked-byte comparison in `extracted/decomp/pending_audit_3969/`, committed locally as `37509c7`. The first 1200-function Ghidra proposal batch yielded 443 exact matches (37 in the sample and 406 in the remainder). All four older source bodies outside the ledger also passed: `func_02067ea0`, `func_02067e94`, `func_02067ce0` and `func_020082e8`. Normalized sources and those four were freshly compiled and checked in `extracted/decomp/checkpoint_audit/` (209/209 passed). The first ARM sample added 42 exact matches in ARM mode, bringing the ledger to 4455. The 4413 checkpoint is committed as `3b0b705`.

Next proposals are prepared under ignored `extracted/decomp/`: `batch_ghidra_arm.py` has 817 ARM candidates and `batch_ghidra_thumb.py` has 2037 larger Thumb candidates. Both use `ghidra_remaining/`; the ARM script omits `-thumb`. `ghidra_other/` contains the 403 ITCM and in-scope overlay candidates. The remaining ARM proposals are compiling; sample pass names are preserved in `batch_ghidra_arm/sample_passed.txt`. ARM sources go under `src/arm/`. Only passing sources are promoted into `src/`. Do not regenerate failed proposals without a deliberate source change. `record_checkpoint.py` verifies the compiled source copy and linked function again, writes reports, and appends ledger rows while preserving old row order.

Use verified names, not a brace census: a loose definition regex can mistake a function call in an `if` condition for a definition. Anchor definitions at an unindented declaration line and reject braces/semicolons inside the parameter match. The source census also omits the differently named motion queue entry. Update the ledger and treemap at verified local checkpoints.

### Rules

- On 2026-10-29 the user authorized local commits while continuing autonomously, with the expectation they may adjust them later. Use the repository's configured Git identity. Do not add assistant attribution or co-author lines. Do not push or rewrite the user's earlier commits.
- One function per `.c`/`.cpp`. Junior style: plain loops, no comments, dsd names (`func_XXXXXXXX`, `data_XXXXXXXX`).
- Pinned compiler: `mwccarm dsi/1.2p1` with `-proc arm946e -thumb -O4,s -fp soft -interworking -sym on` for Thumb. Omit `-thumb` for ARM. Ledger `flags` omit `-sym on`.
- Compare address, size, thumb, and body. Relocs do not count.
- The current batch uses `-thumb`. ARM functions require separate ARM compiler settings and verification; do not apply Thumb flags to them or change their mode to inflate coverage. They are in scope under the user's newer full-inventory instruction.
- Wine: `/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine`, bottle `toolchain/wine-mwcc-run`, `WINEDEBUG=-all`, `LM_LICENSE_FILE=Z:\Users\liambsulliva\Developer\ghost-trick-vita\toolchain\mwccarm\license.dat`. One wine process. `python3 -u`. Needs host permissions (`all`).
- Tools: `ld.lld`, `llvm-mc`, `llvm-objdump` from Homebrew llvm/lld. Reference ELF `extracted/lld/linked.elf`, `.main.text` at `0x2000000`. `elf_fixups.fix_object` before link. Thumb `BL` is `F000`/`F800`; `BLX` second half is `E800` (word-align target with `&~3`). Literal pool `PC=(insn+4)&~3 + imm8*4`.
- Defined names: brace regex `\bfunc_...\([^;]*\)\s*\{` plus `__sinit_`.
- `extracted/` and `toolchain/` stay gitignored. No game dialogue or art in git or chat.

### How to match

Reuse `extracted/decomp/batch_tail.py` (`compile_items`, `match_items`, `defined_names`, `halfs`, `literal_word`). Pattern:

1. Classify remaining unmatched Thumb by size and first opcodes.
2. Write a `batch_moreNN.py` that emits one C file per hit.
3. `python3 -u extracted/decomp/batch_moreNN.py generate && python3 -u extracted/decomp/batch_moreNN.py sample`
4. Sample is one function per kind. Keep passing kinds, delete failing kinds, then `... all`.
5. `match_items` deletes failing sources. If you rerun `generate`, it can recreate them: delete those again.

Duplicate small bodies are mostly gone (remaining size≤32 dups are known-fail hw pack and virt `15db0`). Yield is unique functions plus constrained-register wrappers.

Pass-through unused parameters keep live registers. Extra `0` in r1 forces vtable into r2 (`virt0`, `virt_c0`). Originals that use r2 with r1 free will not match; skip them. Compiler tails (`ldr r3; bx r3`) where the original `push`/`bl`/`pop`: skip (`wrap`, `add_wrap`, `wrap_blx`).

### C that has matched

Empty `bx lr`; imm return; field `ldr` word/half/byte via `char*` plus byte offset; const pool; tail `bx`; alloc-if; six-arg ctor; destructor duplication; CRC tails; virtual delete in `.cpp`; `(short)x<<12`; overlapping stores via `char*`; unused third param to keep r2; `func_0206a7f0` fillers; singleton guard; return 1 vs 2; `unsigned char` `lsl#24`/`lsr#24`; return-this after 1/2 calls; call then store vtable; alloc-if with shift; swap tail; `tail3`; `zero_third`; `add_tail` / `add_tail2` / `add_tail3`; `imm_tail`; `big_add_tail`; `index_load`; `nonzero`; `shift_store`; `null_call`; `call_zero`; `two_call`; `two_this` as `return second(first(object), object)` with no extra `return object`; `ldrsh` as `*(short *)(object + imm)`; `virt0` as `object->methods[n](object, 0)`; `idx_add` as `object += index; return object[CONST]`; `ctor_far` parent then vtable then field zeros; `ctor_vt` vtable-first then calls/stores; `ctor_x` parent then vtable then r0 stores and `(imm << shift)` stores; `sinitn` with numeric 4th arg; `flag_word` as `if ((*(int*)(object+off) & mask) != 0) return 1; return 0`; `call_offstr` stores the second param, not the call result; `call2_str` with unused second param; `ubfx` / `ubfx8` as `(value << lsl) >> lsr` (some widths become AND-with-pool: drop those); `call_strb`; `zero_inc`; `inc_far`; `copy_far2`; `bicw` as `&= ~mask`; `child1`; `relink`; `setb2`; `setb_z`; `call_bool`; `and_pool`; `strb_tail`; `child_imm`; `add_r2`; `imm_shift`; `ifsetb`.

### Do not retry the same C

`flag_bit` / `flag_bit8` (compiler `tst`/`and`, not `lsl`/`lsr`+`beq`); `wrap_blx`; copies `copy8`/`copy12`/`copy2`/`copy2h`/`copy2b`/`copy3` (store order); hw pack `func_02029370` / `func_02029390`; `ldm` r3 vs r2; `089c8` struct-init; virt `func_02015db0` / `func_02015e38` register allocation; SVC `dfxx` size 4; `slot_virt` / `virt_s` / `virt_c` / `virt_e` / `virt_cf` (r1 vs r2 or extra ldr); `signbit` (`cmp`/`bge` vs `lsrs #31`); clamp `>` vs unsigned `bls`; `u8` double `lsl`/`lsr` (compiler emits one pair); `zero_2call` (compiler saves r4, original does not); `loadb_far` unused `cmp` dropped.

`skip_sinit` (do not regenerate): `__sinit_0207e3f4`, `0207e58c`, `0207e934`, `0207ec0c`, `0207ef1c`, `0207f60c`, `0207f87c`, `0207f8bc`, `0207fa3c`, `0207fc8c`, `0207fefc`, `020800f8`, `02080964`, `02080c80`.

### Checkpoint verification, including the 7104 milestone

1. Verify checkpoint names actually pass (`link_one` or `tools/match_function.py`). Delete extras that fail.
2. Write check JSONs (`indent=2` plus newline) under `extracted/decomp/<name>/dsi_1.2p1/<name>_check.json`. Helpers: `extracted/decomp/snapshot_ready.py`, `extracted/decomp/build_pending.py` (point them at the new batch work dirs / `passed.txt` lists, or write an equivalent).
3. Append verified ledger rows at each checkpoint; 7104 is an intermediate milestone. Fields: `module`, `address`, `name`, `size`, `source`, `source_sha256`, `compiler`, `flags` (no `-sym on`), `verification_sha256`. Sort each appended batch by module/address while preserving older row order. Do not count unverified remainder sources. Check that the compiled source copy equals the promoted source before recording a match.
4. `python3 -I tools/progress_treemap.py` then `python3 -I tools/progress_treemap.py --check`.
5. Update this file's status and phase table (5–8) to the verified count of 8,573 and add a snapshot bullet. Preserve earlier snapshot bullets.
6. Commit each verified checkpoint locally under the user's latest authorization, then continue. Report meaningful progress. Do not push.

### Where to look next

Remaining Thumb is mostly unique. Highest leftover piles: `10b5 041c` save-this (~650), `08b5` wrappers (~160 small), `38b5`/`b570`/`b5f8` frames, ~120 ctor-like that are not `ctor_far`/`ctor_x` (pool load is not a vtable store, or extra bit ops / far calls). Leaf `bx lr` size 8–16 still has table/index/and/or forms. `extracted/decomp/batch_more20.py` `ctor_x` can grow if it learns `1c20`+BL after vtable and extra pool pointer stores; `batch_more22.py` is the latest unique-leaf classifier.

## 1. What the project is

The goal is a PS Vita port of Ghost Trick: Phantom Detective, using the **2023 remaster's art and UI** on top of the **original DS game logic**.

**Current delivery scope: Chapter 1 only.** The user wants a playable on-device build and a feedback cycle before expanding. The rest of the game is a stretch goal if that works well. Keep all prepared assets, logic, extracted data, tools and research for later use; narrow new implementation and verification work rather than removing existing preparation. `docs/chapter1.md` is the current delivery plan. The whole `st01` folder is a conservative inventory, not a proven Chapter 1 boundary, because it also contains later scenes.

Decided direction, after earlier feasibility work:
- **The remaster's own binaries can't be ported.** The Android APK, Windows and Steam builds are RE Engine, 64-bit (arm64 or x86-64), Vulkan or Direct3D, and need about 3 GB. The Vita is 32-bit ARMv7 with 512 MB RAM and vitaGL (GLES-like).
- **The plan:** recompile the DS game's ARM9 code natively for the Vita (ARMv5TE code runs mostly unchanged on ARMv7), replace its renderer and UI, and draw remaster assets converted offline from the user's own Steam copy. The UI follows the single-screen Android remaster, not the DS's two screens.
- **The asset finding:** neither version uses skeletal animation. The remaster's characters are high-resolution 2D art driven by keyframe timelines. Details are in section 6.

The `BFM-decomp` wiki phase ladder (https://github.com/Druthulu/BFM-decomp/wiki/Start-a-new-decomp-project) supplied the earlier phase sequence. It was written for PlayStation and gcc, so the DS equivalents are used. Phases 0–3 are done (section 5). The user now prioritizes matching the full function inventory autonomously (section 0), with local commits along the way. The playable release target remains Chapter 1.

## 2. How the user works (follow this)

- **Commits:** the newer 2026-10-29 instruction authorizes autonomous local commits, which the user expects to adjust later. Use the configured Git identity, with no assistant attribution or co-author lines. Do not push or rewrite their previous commits. Branch: `asset-inventory`. Remote: `git@github.com:liambsulliva/ghost-trick-vita.git` (private).
- **Code style:** write like a junior developer learning as they go. Plain loops and functions, no clever pointer arithmetic, no unusual data structures, **no comments in code**. Match the existing `tools/*.py`.
- **Writing style:** plain and direct, no filler. The user had asked for an "unslop" skill that wasn't installed, so write plainly.
- **Decisions:** continue the matching campaign autonomously under the user's newer instruction. Do not stop at checkpoints to request continuation approval.
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
- **Disk:** about 23 GiB free at the 2026-10-29 checkpoint. `extracted/` takes several GB. Be careful with big outputs.

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
| 5–8 Matching decomp | In progress through the full inventory | 4455 of 8,573 functions have source-level compiler and linked-byte verification (`docs/progress-treemap.svg`, `config/decomp_progress.json`). Full-game C link integration and Vita runtime verification remain separate milestones. The user authorized continuation beyond 7104 and local commits along the way. |

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
- **Batch after `0x02008900` (54 functions, 856 bytes in all):** matched `func_020089fc`, `func_02008a00`, `func_02008ad0`, `func_02008ad4`, `func_02008ad8`, `func_02008ae0`, `func_02008aec`, `func_02008b0c`, `func_02008b20`. These are empty functions, a forward, flag writes at `+0x12`/`+0x13`, return-1, a static type pointer, release-and-return, a 4-byte allocate-then-construct (`func_02031254(4)`, likely `operator new`) and a constructor that sets vtable `&data_020a2b74`. Misses moved to drafts: `func_02008abc`.
- **Next stretch, `0x02008b34` through `0x02009c0c` (111 functions, 2,118 bytes in all).** 57 new matches, 1,262 bytes. The large bodies in between were left: `func_02008ca4` (512 bytes), `func_02008ef4`, `func_02009120`, `func_02009388`, `func_0200946c`, `func_020094b8`, `func_0200958c` through `func_0200965c`, `func_020097b4`, `func_02009858` through `func_02009938`, `func_02009968` and `func_02009a28`.
  - Second 4-byte allocate/construct pair (`func_02008b34`, `func_02008b48`, vtable `&data_020a2b90`). `func_02008b68` passes `0xedb88320` to `func_0207250c`; `func_02008b5c` drops its first argument and tails into that; `func_02008b78` forwards the other three arguments to `func_02072588`.
  - `func_02008b88` stores vtable `&data_020a2b64`, halfword `0xfffd`, three zero bytes and a zero pointer, then calls `func_02009630(object, 0, object + 0x14e0)`. `func_02008bb8` is the matching teardown (release the pointer at `+0xc` through `func_02007bd8` / `func_02007b70` and virtual slot `+0x10`). `func_02008bf0` is that same body plus `func_02031270`.
  - `func_02008c2c` (`src/base/setup_step.cpp`) is the first C++ match. The virtual call is `delete` on a type with a virtual destructor. It has to sit in its own `if (created != 0)` inside the outer null check. One check compiles to a single `beq`; the original has two.
  - Also matched: base-then-free and base-then-return copies, calls that pass one function's result straight into `func_02073fc4`, the one-time init `func_02008eb0` (guard `data_020b4250`, object `data_020b4254`), the `func_0206a7f0` fillers (`6, 1, 0` and swapped `8, 10, 2`), mark/clear of the byte at `+0x18`, 0x40 and 0x38 allocate/construct pairs, `func_02009830`'s field clear (the word at `+4` and the bytes at `+6/+7` are separate stores), and `func_02009bdc`, which sign-extends two ints to 16 bits and shifts them left by 12 before `func_02008398`.
- **Snapshot at 222 functions (2,710 bytes).** Added 111 matched functions: empty bodies, small constant returns, single field loads, address constants, and tail calls.
- **Snapshot at 444 functions (3,598 bytes).** Added 222 matched functions: empty bodies, small constant returns, single field loads, address constants, and tail calls.
- **Snapshot at 888 functions (6,416 bytes).** Added 444 matched functions: empty bodies, small constant returns, single field loads, address constants, and tail calls.
- **Snapshot at 1776 functions (16,852 bytes).** Added 888 matched functions: wrappers that call one or two functions and return the object, plus field reads, field writes, and shifts.
- **Snapshot at 3552 functions (49,270 bytes).** Added 1776 matched functions: six-argument constructors, allocate-then-init wrappers, vtable stores, and the remaining small call and field helpers. User committed this as `d996db0`.
- **Snapshot at 4413 functions (72,386 bytes).** Freshly verified the 414 pending helpers, added 443 matches from the first Ghidra proposal batch and verified four older source bodies. Committed as `3b0b705`. The README treemap and ledger now update at each local checkpoint. The 7104 count is an intermediate milestone under the newer full-inventory instruction in section 0.
- **Snapshot at 4455 functions.** Added the first 42 ARM-mode matches using the pinned compiler without `-thumb`. ARM and Thumb modes are checked separately.
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

## 10. Next steps

**Now:** section 0, continue through all in-scope Thumb and ARM functions, with verified local commits along the way. Do not stop at 7104 or ask for continuation approval.

**For the playable delivery:** follow `docs/chapter1.md`. Keep all prepared work. Defer later-stage effect tracing, whole-game alias cleanup and full-pack payload surveys until Chapter 1 is on device.

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
