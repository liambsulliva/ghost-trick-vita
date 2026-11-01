# Handoff: Ghost Trick PS Vita port

Updated 2026-11-01 for the next agent. This is a compacted copy of the 2026-10-31 handoff, which was about 168 KB. The full earlier text is in git: `git show 18622cc:HANDOFF.md`. Per-function history lives in `config/decomp_progress.json`, each group's `extracted/decomp/parallel_*/HANDOFF.md`, and `git log`. Paths are relative to the repo root. Find a section with `grep -n '^## ' HANDOFF.md`.

## Start here

**Active task:** byte-match every in-scope function in the DS inventory. Denominator: 8,573 (DS Protect overlay excluded). The 7,104 milestone is passed, and there is no stopping point short of full coverage. Chapter 1 playable delivery is the later goal (see Port project).

**State at HEAD `18622cc` (branch `asset-inventory`):**
- Ledger: 7,749 matched, 824 remaining. By module: main 7,422; ov004 141; ov006 121; ov003 33; ov005 20; ov001 6; itcm 6.
- Deferred: 6 functions in `extracted/decomp/escalation_deferred.json`.
- Nothing pending: `python3 -I extracted/decomp/integrate_pending.py --dry-run` prints "nothing pending".
- Working tree is clean except `context_pending_sources/parallel_escalate_m_func_0206b168.c`, which is untracked user work. Leave it in place. It is not a pass.

**Usage stop rule (latest user instruction):** stop all agents when weekly usage reaches 98% consumed (2% remaining), then leave a clean handoff. The last recorded stop (2026-10-31) was 98% weekly and 51% five-hour. Before resuming, check both windows. `usedPercent` is consumed, so remaining is `100 - usedPercent`. Do not restart automatically at a five-hour reset.

**First steps:** check usage; run `git status -sb`; run the integrator dry-run; read `extracted/decomp/WORKER_BRIEF.md` ("Read first" and "Method") and the group handoff you will work in; then run the loop in "Coordinator loop".

## Rules and authorization

**Git**
- Commit verified checkpoints locally on `asset-inventory`. This is authorized. Use the repo's Git identity (Liam).
- Do not push. The user pushes, and each push rebuilds the Pages viewer. Push only after fresh authorization in chat.
- No assistant attribution, `Co-Authored-By`, or trailer lines in commits. This is the user's rule for this project. Do not rewrite earlier commits.
- `integrate_pending.py` commits with `commit-tree` and `reset --soft`, which adds no trailer. Use it rather than a plain `git commit`.

**Code**
- No comments in code. Plain junior-developer C/C++: plain loops and functions, no clever pointer arithmetic.
- Use dsd names only (`func_XXXXXXXX`, `data_XXXXXXXX`). One function per `.c` or `.cpp`, in `src/thumb/` or `src/arm/`, named `<prefix><name>`.
- Match the style of the existing `tools/*.py`.

**Game data and legal**
- Game-derived data stays out of git and chat: no dialogue or story text, no art, no rendering of game characters. `tools/rom_audit.py` enforces the git side. `extracted/` and `toolchain/` are gitignored.
- DS overlay 0 (DS Protect, anti-piracy) is out of scope.
- The Android APK in `~/Developer/` is a modified third-party repack. Use it for format research only, and recheck results on the user's Steam copy.

**Hardware and network**
- The Vita is off to save battery. Ask the user to turn it on before any device test.
- The user runs a VPN that drops connections. When the user says "VPN connected, continue", retry the same command.

## How to match

Use verified calling context to write variants for mapped targets. Use byte-matched functions as reference points. Do not invent functions.

1. Pick an unmatched target that exists in the configured inventory and in a successful Ghidra export. Confirm symbol, module, entry address, size and ARM/Thumb mode. Keep that identity for every variant.
2. Find anchors in `config/decomp_progress.json`: verified callers, callees, constructors, accessors, or siblings on the same object fields. Check each anchor's source and verification hashes. A Ghidra draft, a nearby address or a plausible name is not an anchor. Confirm each direct call edge in the reference instructions. Do not infer a target from an unresolved indirect call.
3. Read the Ghidra draft together with the anchor sources. A verified forwarding wrapper can be restored when the reference calls through it, because Ghidra may name the wrapper's destination instead. Follow argument order and widths, pointer versus scalar use, return-value use, field offsets, table strides, flag updates and virtual slots. A matching body proves only that compiled body. It does not prove the original header or class layout.
4. Keep observations separate from inference. Write a context packet under ignored `extracted/decomp/` with the target identity, the Ghidra export path and hash, anchor symbols with source and report hashes, observed relationships, the tentative role, and the reason for each change.
5. Generate a small set of deliberate variants, each explained by an anchor: a callee's return width, caller-supplied argument positions, an accessor's field type, a constructor's initialization. Do not add speculative helpers, addresses, call targets or inventory entries. Skip ambiguous relationships. Do not repurpose another function to fit.
6. Compile with the pinned compiler and the correct mode. Require the exact entry, size, mode and every linked body byte. Prune unchanged sources first. Promote only a passing snapshot, then integrate.
7. Feed newly verified signatures and fields back into the context. Revisit connected targets when new evidence supports a changed source. Keep rejected variants out of public sources.

### Matching rules

- **Byte-exact only.** Entry, size, mode and every linked body byte. Relocations do not count. Plausible C that does not match is not a match.
- **Identity is fixed.** Never change a function's mode to inflate coverage. Never guess addresses or call targets. Declarations must resolve to existing mapped or reference symbols.
- **Compiler: `mwccarm dsi/1.2p1`.** Thumb game code: `-proc arm946e -thumb -O4,s -fp soft -interworking -sym on`. Main-module ARM at 0x02067000–0x0207ffff: `-O4,p`, placed in the batch folder's `opt.txt`. Other ARM keeps its verified setting.
- **Per-function flags are allowed** when the function byte-matches (user rule, 2026-10-30). Keep such targets in their own batch folder. Mwcc optimization pragmas count the same way, for example `#pragma optimization_level 0` in the source. The ledger records the flags.
- **Anchors are required.** Each pass needs at least one verified anchor in its packet's `anchors` or `evidence`. Reference instructions alone do not count. `integrate_pending.py` refuses an anchorless pass.
- **`volatile`** only on 0x04xxxxxx hardware registers, or where NitroSDK declares it. Volatile on ordinary RAM stays out of `passed.txt` until the user reviews it.
- **`asm`** only where the SDK writes the routine in asm: NitroSDK hand-asm, CPSR/CP15 code, runtime code, BIOS SWI stubs.
- **Variant names are unique per target.** Failures are counted per (tier, function, variant name), so a reused name hides a real try.

## Parallel matching

- Divide targets into groups by subsystem and observed call relationships. Independence is a hypothesis. Give each group disjoint ownership, and check shared callers and callees before changing a hypothesis.
- Each group writes only its own `extracted/decomp/parallel_<group>/` folder and its `src/<mode>/<prefix>*` files.
- Groups exchange context through their `HANDOFF.md` files (ignored). Only recorded byte matches become anchors for other groups, and only after the integrator records them.
- One integration owner handles the ledger, treemap, tests and commits. Workers never edit `config/decomp_progress.json`, the docs, `tools/`, or another group's folder.
- Compiles go through the shared lock: `extracted/decomp/with_compiler_lock.py`.

Groups (ranges as last set on 2026-10-30; each folder's `anchored_targets.json` and `HANDOFF.md` are the current source):

| Folder | Owns |
|---|---|
| `parallel_logic_low` | Thumb below 0x02018000 |
| `parallel_logic_low_b` | Thumb 0x02018000–0x02020e5f |
| `parallel_logic_low_c` | Thumb 0x02020e60–0x0202ffff |
| `parallel_objects_mid` | Thumb 0x02030000–0x02043cb3 |
| `parallel_objects_mid_b` | Thumb 0x02043cb4–0x0205627b |
| `parallel_objects_mid_c` | Thumb 0x0205627c–0x0205e35f |
| `parallel_objects_mid_d` | Thumb 0x0205e360–0x02066fff |
| `parallel_arm_library` | Main ARM below 0x02072ac0; ITCM and overlay ARM; overlay 4 Thumb; main Thumb from 0x02067000 |
| `parallel_arm_high` | Main ARM at or above 0x02072ac0 |
| `parallel_overlay_b` | Overlays 1, 3, 5; overlay 6 below 0x020d6000 |
| `parallel_overlay_c` | Overlay 6 Thumb at or above 0x020d6000 |
| `parallel_small`, `parallel_twins` | Small Thumb helpers; sibling templates from verified anchors |
| `parallel_interpreter` | Analysis only, for `func_02023fdc` |
| `parallel_near_misses` | Paused (0 of 8) |
| `parallel_escalate_m`, `parallel_escalate_x` | Escalation queues for the opus_medium and opus_xhigh tiers |

## Coordinator loop

1. **Pick targets.** Read `parallel_<group>/anchored_targets.json`: untried targets first, then by size. To list the smallest open targets per group (read-only):

```bash
cd /Users/liambsulliva/Developer/ghost-trick-vita/extracted/decomp && python3 -I -c "
import sys,json,glob; sys.path.insert(0,'.'); import escalation
f=escalation.failures(); done=set(m['name'] for m in json.load(open('../../config/decomp_progress.json'))['matched'])
for a in sorted(glob.glob('parallel_*/anchored_targets.json')):
    d=json.load(open(a)); d=d if isinstance(d,list) else d.get('targets',d)
    r=sorted((t.get('size',0),t['name'],f['haiku'].get(t['name'],0)) for t in (d if isinstance(d,list) else d.values()) if t['name'] not in done and f['haiku'].get(t['name'],0)<3)
    print(a.split('/')[0],len(r),r[:6])"
```

2. **Write variants** into a new batch folder, `extracted/decomp/parallel_<group>/<batch>/`, with names the target has never used. Prune unchanged candidates with `extracted/decomp/prune_variants.py <work>`. Use about 10–15 targets per batch. Put per-function flags in that folder's `opt.txt`.

3. **Compile once per folder.**

```bash
python3 extracted/decomp/with_compiler_lock.py python3 extracted/decomp/run_variants.py <full folder path> <thumb|arm> <prefix>
```

Run it exactly once per folder. Never rerun in place, and never hand-edit `compile.log`, `passed.txt` or `build_meta.json`: in-place reruns have lost COMPILE_OK lines. A follow-up goes in `<batch>_round2`. Thumb runs must pass both duplicate-symbol controls. Copy each passing snapshot byte-for-byte to `src/<mode>/<prefix><name>.c` or `.cpp`, and confirm it with `cmp`.

4. **Scan** every new source before recording: `grep -nE "volatile|asm|__restrict|pragma" <file>`. Hold back any volatile-on-RAM, unexplained asm or unexplained pragma.

5. **Record and commit:** `python3 extracted/decomp/integrate_pending.py "<message>"`. It records every batch whose `passed.txt` names are not in the ledger, runs `record_checkpoint.py`, regenerates and checks the treemap, runs the tests and the ROM audit, and commits locally. Extra arguments limit it to groups or `group/folder`. Scan every pending batch first, because the integrator records all pending passes. Every pass needs a COMPILE_OK line in `compile.log`. If one is missing, run `extracted/decomp/recheck_batch.py`.

6. **Route failures.** A target with 3 failures in its tier moves up:

```bash
python3 extracted/decomp/escalation.py extracted/decomp/anchor_all.json 1 1 --append
```

This writes `parallel_escalate_m/queue_shardN.json` and `parallel_escalate_x/queue_shardN.json`. Refresh `anchor_all.json` from a full anchor audit after large recording runs.

**Tiers.** `haiku` runs first, then `opus_medium`, then `opus_xhigh`. These are logical names that count failures, not a claim about which model ran them. Keep the names so the counts stay valid. Each function gets `LIMIT = 3` failed variants per tier, and a 4th failure is never compiled. After 3 opus_xhigh failures the function is deferred to `escalation_deferred.json`, ordered farthest-by-address from the last function attempted. When every function has used its last try, restart from the top of that list. `escalation.failures()` gives the counts.

**Current ladder (2026-11-01, user's choice).** Two tiers: `haiku`, then `opus_xhigh`; `escalation.py` no longer routes to `opus_medium`, and targets already there were moved to the xhigh queue. Haiku workers (subagents) only take functions under 100 bytes (user's rule, 2026-11-01): `escalation.py` sends anything of 100 bytes or more straight to `opus_xhigh` (`HAIKU_MAX = 100`). Haiku matched 1 of 59 targets in its last 10 batches (the last three on 216-428-byte functions). A fresh anchor audit on 2026-11-01 (`anchor_all.json` updated in place; the 2026-10-31 copy is `anchor_all_2026-10-31.json`) found anchors for 136 more functions under 100 bytes. 123 of them are in the Haiku bucket, and `write_anchored_targets.py` now lists only Haiku-tier targets in each group's `anchored_targets.json`. 266 functions under 100 bytes are still unanchored. 151 of them only touch unmatched functions: matching func_02023fdc (9,560 bytes, already anchored) would anchor 13. The other 115 have no code edge at all. The coordinator works the xhigh queue in its own chat, writing each target from the reference disassembly (`sh extracted/decomp/parallel_escalate_x/x41_src/ref.sh <hex> <size>`, which also resolves literal-pool symbols) and building one-off batch folders with `parallel_escalate_x/x41_src/mk.py <batch> <sources>`. Notes are in `parallel_escalate_x/HANDOFF.md` from "## x41". mwcc's soft-float helpers (`_dadd/_dsub/_dmul/_dflt/_dfltu/_dgr/_dfix`) are mapped in RUNTIME_ALIASES, so double math compiles and links.

**Batch hygiene.** A batch that crashes before writing `variant_status.tsv` gets an `abandoned.txt`, or its targets look busy forever. Do the same for a folder with a manifest, no `passed.txt`, and no live process. The tools deduplicate failures copied into record folders.

**Worker lifecycle.** A worker stops after three consecutive zero-match batches, or when its context gets large. Before stopping, it writes a status section to its group handoff, with leads, confirms it has no running processes, and reports. Do not message a stopped worker, because that resumes it. Start a replacement instead.

**Pace.** Six or more workers burned about 30% of usage per hour. Two to four kept it near 12–16% per hour. Scale the worker count to the window you have left. Compiles share one lock, so two workers can each wait 15–20 minutes. That is waiting, not a stall.

## Usage and stalled work

- Read the account limits before starting and at each checkpoint. Stop at 98% weekly consumed, per the latest instruction. Before stopping, signal every worker to stop new experiments, keep their results, and write a handoff that separates integrated matches from untested work.
- A worker waiting for the compiler lock is not stalled. Check for meaningful progress before calling anything stalled. Kill only a confirmed stalled process you own, keep its logs, and record where it stopped.
- Do not leave compiler, Wine, runner or Ghidra jobs running at shutdown. The long-lived `winedevice.exe` bottle service may stay.

## Held, withheld and parked work

- **Anchorless matches.** On 2026-10-29, 61 byte-exact passes with no verified anchor were withdrawn. 23 were restored later with post-match anchors (`811cc5f`). The remaining sources are in each batch folder's ignored `context_pending_sources/`, with the original pass list in `passed_byte_only.txt`. Restore one only when `anchor_audit.py <names>` finds a verified edge, then run `restore_anchored.py <audit.json>`. Never recompile blind. A restored entry keeps its saved compiler flags.
- **Withheld, volatile on RAM:** `func_0206b168`. Its source is the untracked file in `context_pending_sources/`.
- **Deferred:** the 6 functions in `escalation_deferred.json`. Revisit them when the rest is matched or a new lead appears.
- **Interpreter:** `func_02023fdc` (9,560 bytes, `add pc` switch, 363-entry jump table at 0x02024020) is analysis only. A full match needs the whole body. Do not invent labels for its interior blocks. `extraout_r1` is the high word of `func_0207b8a0` after opcode 0x66's BLX at 0x02024604: the unsigned quotient is in r0 and the remainder in r1.
- **BIOS SWI stubs with no verified call edge:** RLUnCompReadNormalWrite8bit, LZ77UnCompReadNormalWrite8bit, HuffUnCompReadByCallback, Halt, LZ77UnCompReadByCallbackWrite16bit, RLUnCompReadByCallbackWrite16bit, SoftReset, Div. Nothing calls them with bl or blx yet. `CpuSet` is recorded, anchored by func_02079b6c.
- **Frame-shape targets:** functions that allocate their frame with a separate `sub sp` after the push. The pinned compiler folds it into the push. In 0x02067000–0x0207ffff, 10 of 10 matched under `-O4,p` in the frame sweep. Outside that range, sweep the shape before spending model tries.
- **ARM jump tables:** `addls pc` jump tables and `bhi` compare trees (about 17 unmatched reference functions) do not match yet.
- **Parked forms:** CPSR/CP15 wrappers and overlay jump-table stubs did not match in plain C. func_020276fc has a prologue store-order problem. Do not retry failed forms unchanged. Each group handoff has its own do-not-retry list, and the pruning tools skip identical sources.

## Decisions waiting for the user

Do not decide these yourself.

1. **C++ destructors:** func_02064cfc, func_02064dec, func_02019dc4, func_02019f04. These need tooling to rename mangled destructor symbols to `func_` names and link one section per function. The same tooling, aliasing a compiler-generated `__vt__` symbol to its address, would cover constructors whose vtable store mwcc schedules itself: func_02016c4c and func_02016d30.
2. **mwcc 2.0 for ov004 ARM.** func_ov004_020d4e20, 4e90, 4f00 and 4da4 match the reference shape of mwcc 2.0 output: base in r3, pool reload into r0 after the call. dsi/1.2p1 keeps the base in r4 at every -O level. func_ov004_020d44d8, 49c4, 4fc0, 500c and 507c show a zero-in-callee-saved pattern. Changing the compiler for this region needs a sweep and approval. Nothing is recorded with 2.0.
3. **C++ exception handling (binding-13 link blocker):** func_0201d3a4, func_0207dd08 and func_0207dc50. The last two are catch handlers reached through exception tables, part of CodeWarrior's `__construct_array` helpers. func_0200fbd4 joins them: it keeps a local object whose destructor (func_0201ef74) runs on each of its three exits, unmerged, after func_0201ef58 constructs it. An inline C++ wrapper (`Archive() { func_0201ef58(this); } ~Archive() { func_0201ef74(this); }`) reproduces that, but the object then carries an out-of-line `~Archive` with binding 13 and ld.lld refuses it (x45_thumb13).
4. **`main`:** mwcc inserts `bl __main` into a C `main`.
5. **Review list.** These match and are accepted for now. The user has not reviewed them.
   - SDK routines written as `asm void`, as NitroSDK does: func_01ff83a8 (OSi_DoBoot), func_0207303c (MI_CpuCopy8), func_0206e784 and func_0206eab4 (MTX_RotZ), func_0206e6a8 (MTX_Scale33_), func_02072f08 (CpuClearFast), func_02072f8c (MI_Copy36B), func_02073174 (LZ8 with swpb), func_020748a0 (CP15), func_0207e04c (runtime throw context save), func_02075234 and func_020751b4 (OS exception mode switches), func_020009fc (crt0 autoload with CP15), and CpuSet (BIOS SWI).
   - MSL routines: func_0207a3c0 (`exit`) and func_0207a6e0 (`raise`).
   - Volatile on ordinary RAM: func_0206b958 (`*(volatile unsigned int *)(param_1 + 3)`), func_ov001_020d3b60 (volatile halfword stores into data_020c9e88 and data_020c9e48), func_020315ac (volatile `busy` spin-wait, probably faithful), func_0206a120 and func_0206a270 (volatile CARDiCommon flag), func_0206c138 (volatile-const inline in FS), func_020699b4 (spin-wait on `data_020d0220[1]`) and func_02077fcc (TP_WaitBusy-style spin on the touch-panel `command_flg`; NitroSDK probably declares it volatile). Both spin-waits matched at -O4,p and can only re-read memory in a loop with volatile. These may be fakematches.
   - SDK-declared volatile: func_02074dd8 (OS_AllocFromHeap, `OSHeapInfo.currentHeap`) and func_020753bc (OS_GetTick volatile locals and tick counter). Hardware volatile is genuine: IRQ and IME at 0x04000208, and func_0206a97c at 0x040001a4.
   - Per-function flags and explicit calls: func_ov004_020d6f34 (`#pragma optimization_level 0`), func_020768d0 (-O2), func_ov004_020d633c (matched at -O2 only), and func_0200c220, func_0200d668 and func_0205fe04, which call `func_0207b674` explicitly where `*` was possible.
6. **Vita device test:** ask first, every time.
7. **Anchors through data tables.** 102 unanchored functions under 100 bytes are reached only through function tables in .data, and a matched function loads each table's base as a literal. For example, func_0200a8d8 is the word at data_020a2be8+0x14, and matched func_020097b4 loads data_020a2be8. `anchor_audit.py` does not count this edge, so counting it would change the anchor rule.

## Tools and environment

**Compiler**
- Wine: `/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine`, bottle `toolchain/wine-mwcc-run`, `WINEDEBUG=-all`, `LM_LICENSE_FILE='Z:\Users\liambsulliva\Developer\ghost-trick-vita\toolchain\mwccarm\license.dat'`. Run one Wine process at a time with `python3 -u`. The host permission level must be `all`. Without the license the compiler hangs. If that happens, kill any stuck `winewrapper.exe`.
- The lock kills a compile after 20 minutes. `batch_tail.py` kills a crashed `mwccarm` and its `winedbg`.
- Runtime names map to reference routines through `RUNTIME_ALIASES`: `_s32_div_f` → func_0207b694 (signed `/`), `_u32_div_f` → func_0207b8a0 (unsigned `/`), `_ll_udiv` → func_0207b62c (u64 `/` and `%`), `_ll_mul` → func_0207b674 (u64 `*`). Write divisions as C operators.

**Link and match**
- `ld.lld`, `llvm-mc` and `llvm-objdump` come from Homebrew llvm and lld. Reference ELF: `extracted/lld/linked.elf`, with `.main.text` at 0x2000000.
- `tools/elf_fixups.py` must run before linking dsd objects. It also relaxes Thumb `.text` alignment to 2.
- Check one variant: `python3 tools/match_function.py <source.c> <reference_symbol> <candidate_symbol> [--compiler dsi/x.y]`. It writes `extracted/decomp/<symbol>/<compiler>/<candidate>_check.json`. Use dsd names for externals so they resolve.

**Ghidra**
- Read-only only, with `-readOnly -noanalysis`. Headless: `$(brew --prefix ghidra)/libexec/support/analyzeHeadless`, with `JAVA_HOME="$(brew --prefix openjdk@25)/libexec/openjdk.jdk/Contents/Home"`. Project: `extracted/ghidra/GhostTrick`.
- `symbols/database.txt` is the source of truth. Auto-analysis is not repeatable.
- Exporters: `DecompileBatch.java` (modes include `params` and `narrow`) and `tools/ghidra/DecompileFunctions.java` (`thumb:` and `arm:` address prefixes).

**Ledger, treemap, tests**
- `config/decomp_progress.json`, key `matched`. Fields: `module`, `address`, `name`, `size`, `source`, `source_sha256`, `compiler`, `flags` (no `-sym on`), `verification_sha256`. Only the integrator writes it.
- Treemap: `python3 -I tools/progress_treemap.py`, then `--check`. Output: `docs/progress-treemap.svg`, with a README preview.
- Tests: `tests/`, 58 at the last checkpoint. ROM audit: `tools/rom_audit.py`. Its `--self-test` must catch every planted file.
- Pages viewer: `.github/workflows/progress-pages.yml` rebuilds on pushes to `asset-inventory` and publishes to https://liambsulliva.github.io/ghost-trick-vita/. See `docs/progress-viewer.md`.

**Compile tips** (the full list is in `extracted/decomp/WORKER_BRIEF.md`, "Compile tips"):
- `char` is unsigned in these builds. Write `signed char` where LDRSB is needed.
- A single-bit test matches only as a shift, such as `(((unsigned int)x) << 26) >> 31 == 1`. A mask like `x & 0x20` compiles differently.
- Declaration order moves registers. Join branches so a loaded global stays live.
- Use `extern const` for .rodata tables. Declare dsd-named C externs inside `extern "C" { }` in `.cpp` files, and put a C++ target's own function inside `extern "C"`, or link_one fails on the mangled name.
- A ghidra draft's function-pointer type is `code`. Declare `typedef int code();` before using its result.

## Port project (later goal)

**Goal and scope.** A PS Vita port of Ghost Trick: Phantom Detective. It runs the DS game's ARM9 logic, recompiled for the Vita, with the 2023 remaster's art and UI. The current delivery target is Chapter 1 only: a playable on-device build and a feedback cycle. See `docs/chapter1.md` for the delivery plan. Keep prepared assets and tooling, and keep new work narrow. The whole `st01` folder is a conservative inventory, not a proven Chapter 1 boundary, because it includes later scenes.

**Settled direction**
- The remaster's own binaries cannot run on the Vita. They are 64-bit RE Engine builds, about 3 GB.
- Recompile the DS ARM9 code for the Vita. ARMv5TE code runs on ARMv7. Replace the renderer, UI and I/O. Draw the remaster assets, converted offline from the user's own Steam copy, in a single-screen UI that follows the Android remaster.
- Neither version uses skeletal animation. Art runs on keyframe timelines.

**Phase ladder**

| Phase | Status |
|---|---|
| 0 Governance | Done: `tools/rom_audit.py`, `tools/audit_rules.txt`, `.github/workflows/audit.yml` |
| 1 Extraction | Done: `tools/nds_unpack.py`, `nds_catalog.py`, `cpac_unpack.py`, `lz.py` |
| 2 Oracles and load map | Done: `nds_loadmap.py`, `nds_prove_bases.py`, `ghidra_build.sh`, `runtime_oracle.py` |
| 3 All-assembly baseline | Done: `config/arm9/`, `lld_link.py`, `elf_fixups.py`, `fix_header.py`, `phase3_check.sh` |
| 4 Compiler | `dsi/1.2p1` is pinned. The exact dsi version is not proven, because probes gave identical output across the dsi builds. |
| 5–8 Matching | In progress: 7,749 of 8,573 |

**Inputs**
- DS ROM: `~/Developer/Ghost Trick - Phantom Detective (USA) (En,Fr,De,Es,It).nds`, SHA-1 `bcfe74e5592eef3255fd23758b772226960a1b0b`, game code BGTE.
- Remaster: `extracted/remaster_apk/` holds the packs and indexes. `GTPD_ANDROID_Release.list` is a community file list with an unclear license. Never commit it.
- Disk: about 23 GiB free at 2026-10-29. `extracted/` takes several GB, so avoid large outputs.

**Toolchain**

| Tool | Where | Notes |
|---|---|---|
| Ghidra 12.2 | `brew install ghidra` | Headless, as above |
| dsd 0.12.1, patched | `toolchain/dsd` | Patch `tools/patches/dsd-0.12.1-thumb-retry.patch`; source `toolchain/src/ds-decomp` |
| mwccarm, 24 versions | `toolchain/mwccarm/` | Hashes in `toolchain/mwccarm_manifest.json` |
| LLVM and lld | Homebrew | `ld.lld`, `llvm-objcopy`, `llvm-readelf`, `llvm-mc` |
| melonDS 1.1 | `extracted/melonds/`, portable mode | Oracle on the Mac only. GPL, never part of the build. |
| yt-dlp | `toolchain/yt-dlp-venv/bin/yt-dlp` | Leave the global `/opt/homebrew/bin/yt-dlp` alone. It is old and gets HTTP 403. |
| ffmpeg 9.0.2 | Homebrew | Decodes AV1 through dav1d. Its TLS to googlevideo fails. |

**Runtime tracing (melonDS oracle).** `tools/melonds.py` launches melonDS and writes its GDB settings. `tools/gdb_remote.py` is a minimal RSP client. The trace tools are `trace_cpac.py`, `trace_objects.py`, `sample_objects.py`, `trace_lmg.py` and `trace_mtbl.py`. Quirks: send `+` first after connecting; memory reads cap at about 512 bytes; Thumb breakpoints use kind 2; run one melonDS instance at a time; only buttons are injected, not touch.

**Reference video (comparison only).** `extracted/reference/pc_longplay_720p60.mp4` is a PC longplay: AV1, 1280×720, 60 fps, 33,598 s. It was downloaded on 2026-10-28 and is local only. Never commit it, and never reproduce its frames in outputs. If the file is lost, resume it with `toolchain/yt-dlp-venv/bin/yt-dlp --no-playlist -f 398 --newline -o "extracted/reference/pc_longplay_720p60.%(ext)s" "https://www.youtube.com/watch?v=K6Z8QvgObFA" >> extracted/reference/download.log 2>&1`. Do not add JS runtimes, cookies or PO-token workarounds. If YouTube refuses, stop and tell the user.

**Assets and formats**
- Remaster names are MurmurHash3 (seed 0xFFFFFFFF) of UTF-16LE `natives/android/<path>.<ext>.<version>`. `tools/re_names.py` names 82% of `re_chunk_000.pak`. Versions are in `tools/re_versions_android.txt`.
- Characters live under `charfiles/2d/<group>/stNN/<object>/…`: per-frame textures plus one `.user` RSZ timeline. DS frames do not map one to one onto remaster frames. Bicycle has 94 DS frames and 32 remaster textures. When the DS starts an animation, the port plays the whole remaster timeline.
- DS graphics: `cpac_2d.bin` and `cpac_3d.bin` (Capcom pack, KEY/DAT blocks). `extracted/cpac_checked/` has 24,853 2D entries. See `docs/cpac_format.md`.
- Motion tables are labelled blocks named `stNN_<object>_<NNN>.mtbl.xml`. Frame words are `frame = v & 0x3ff` and `ticks = v >> 10`; commands use tags −1 to −16. The table player is `func_02007cd4`. Opcode 0x117 leads to func_020280c8 and func_0205991c, which store the table pointer at object `+0x148`.
- Scene scripts are `1LMG` bytecode for a 16-bit VM, not XML. All 929 decode with `tools/lmg_decode.py`. See `docs/lmg_format.md`.
- Mapping: `tools/map_motions.py` matches 4,007 of 4,070 stage animation stems to a remaster `.user` file: 3,757 exactly, 250 through variants. For stage 01 alone, 352 of 366 match. Output is under `extracted/motion_map/`. The 63 unmatched stems are 23 with stricter pose candidates, 37 with broader alias candidates, and 3 with none (`st13_objeffect_neona_000`, `st13_objeffect_neonb_000`, `st14_scr_gyorai_000`).
- Port hook: at 0x117 or func_0205991c, try the full stem first. If there is no remaster folder, remove one `_NNN` or `_NNx` variant suffix. Keep the animation ID.
- Neon classes: `uObjEffectNeonA` and `uObjEffectNeonB` load resources 7006 and 7007. This is a static binding proposal only, and it is deferred for Chapter 1.

**Open items, not solved**
- 75 motion calls target zero, and 104 jumps go to 0. The VM turns zero into a pointer to the script header, with no zero check. Do not label these as stop commands. Their reachability is unknown.
- The meaning of header field +0x04, most opcode names, and motion-command meanings beyond their lengths.
- Cross-stage naming is ambiguous. For example, a stage 13 guitar wait has both guitar 000 and 999 as candidates.

**Next steps for the port** (`docs/chapter1.md`)
1. Trace Chapter 1's entry, its completion transition and its resource dependencies, starting at the `st01_game000` oracle path.
2. Set up VitaSDK and build an installable viewer with a small converted asset set. Measure frame time and free memory. Ask the user to power on the Vita when the build is ready. The repo has no Vita application yet.
3. Bring up the Chapter 1 DS logic with Vita replacements for file, memory, timing, input, renderer and audio. Leave DS Protect out.
4. Implement only the chapter's required art, motions, default poses and single-screen UI. Calibrate timing and placement against the reference video.
5. Package an opening-to-completion build with working retry and checkpoints, then collect the user's on-device feedback.

## Gotchas

- zsh: a glob with no matches aborts `&&` chains. Use explicit paths. macOS has no `timeout`.
- python.org's Python 3.12 has no CA bundle. Do not disable certificate checks. Use `curl` or yt-dlp's own HTTP stack.
- `tail -n +8` drops the first instruction. Use `+7`, or the match tool's diff.
- Ghidra helper names can clash with GhidraScript methods, such as `parseAddress`.
- dsd objects need `tools/elf_fixups.py` before lld can link them: reorder symbols, set the Thumb bit on FUNC symbols, retype `R_ARM_PC24` to `CALL`/`JUMP24`, and add an ARMv5TE attributes object.
- A ROM rebuild needs `tools/fix_header.py`. It restores the secure-area CRC at 0x6C and recomputes the header CRC, because dsd cannot do that without an ARM7 BIOS.
- A stray `build.bat` in the repo root once made every compile run it. Keep scratch files in your group's scratchpad subfolder, never in the repo root.
- Cut command output with `head`, `tail`, `grep -c` or `wc -l`. Do not cat large files whole.
