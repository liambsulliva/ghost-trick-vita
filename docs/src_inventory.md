# src/ code inventory

A read-only inventory of the reconstructed C/C++ in `src/`, taken 2026-11-05. `src/` holds the byte-matched decompilation of the Ghost Trick DS game: almost every file is one DS function (or data object), reconstructed in plain C/C++ with no comments (per the HANDOFF rule) and verified byte-for-byte against the reference binary. This describes code structure only; no game data is included.

## Totals

- 8,572 source files (7,691 `.c`, 881 `.cpp`) plus one stray `.DS_Store`, about 5.27 MB of source, across 10 top-level modules.
- The organization is overwhelmingly **one function per file**. Most filenames are `<prefix>_func_XXXXXXXX.c[pp]`, where `XXXXXXXX` is the function's DS load address and `<prefix>` records the matching batch or heuristic that produced the file (for example `parallel_escalate_m`, `parallel_escalate_x`, `recovered_thumb`, `call1`, `const`, `six`). The prefixes are provenance tags, not hand-made module boundaries.
- Three modules break that pattern with semantic, hand-chosen filenames: `base/`, `vm/` and `motion/`.
- The code reads as decompiler-shaped (Ghidra-style `uVarN`/`param_N` names, raw pointer-offset access, per-file `extern` prototypes, ad-hoc local structs with padding arrays to reach field offsets). A minority has light manual cleanup.

| Module | Files | C / C++ | ~Bytes | Address range | Role |
|---|---|---|---|---|---|
| `thumb` | 7,426 | 6,557 / 869 | 4.60 MB | 0x0200xxxx–0x0208xxxx (main, Thumb) | Bulk of the game: logic, object management, resource/heap, `__sinit` |
| `arm` | 924 | 913 / 11 | 622 KB | 0x01ff8000 (ITCM)–0x020e43f4 | SDK/OS, IRQ/IPC, BIOS SWI stubs, C++ EH runtime, ov004 ARM |
| `base` | 90 | 89 / 1 | 19 KB | ~0x020083xx–0x02009cxx | Base object/RTTI, alloc+construct, flag & vector helpers |
| `ov004` | 47 | 47 / 0 | 9 KB | 0x020d3a40+ | Overlay 4: wrappers, field accessors, dispatch stubs |
| `ov006` | 39 | 39 / 0 | 11 KB | 0x020d3a40+ | Overlay 6: UI/scene controller over a large state object |
| `vm` | 20 | 20 / 0 | 4 KB | ~0x020265c4–0x02028160 | Script VM / LMG bytecode: machine state, operands, play-motion |
| `ov003` | 12 | 12 / 0 | 4 KB | 0x020d3a40+ | Overlay 3: scene state-machine (partial) |
| `ov005` | 11 | 11 / 0 | 1 KB | 0x020d3a40+ | Overlay 5: leaf setters, destructor-like wrappers (partial) |
| `ov001` | 2 | 2 / 0 | 0.5 KB | 0x020d3a40+ | Overlay 1: one setter plus a wrapper (partial) |
| `motion` | 1 | 1 / 0 | 0.8 KB | — | One hand-named `queue_motion` setter |

Overlays 1/3/4/5/6 all load at 0x020d3a40 and only one is resident at a time. Overlay 0 is DS Protect (anti-piracy) and is out of scope. Overlay 2 has no reconstructed code here.

## Modules

### thumb/ — main-binary Thumb bodies (the bulk)
The whole main-binary Thumb corpus, 7,426 files at 0x0200xxxx–0x0208xxxx. Categories: trivial leaf/empty/const-return functions, call wrappers, allocation helpers, bitfield/abs utilities, loop and table-search routines, and `__sinit` static initializers. The 869 `.cpp` files wrap the body in `extern "C"` and declare placeholder classes (e.g. `Object02023ef4_0`) with virtual slots so virtual dispatch compiles. Only about 23 files contain inline asm. A few files are named by role rather than address (`byte_0200ba78.c`, `half_02009df0.c`, `jump_02006ed4.c`).

### arm/ — low-level, runtime and SDK/OS code
924 files spanning ITCM (0x01ff8000) through 0x020e43f4, including 55 overlay-4 ARM functions. Content is hardware register access (IME 0x04000208, IPC 0x04000180), ITCM startup/autoload, roughly 43 files with BIOS SWI stubs, and around 129 `asm void` routines. The 11 `.cpp` files implement the Metrowerks C++ exception runtime (unwind contexts, catch info, destructor calls). Named entries include `Entry`, `__FindExceptionTable`, `AutoloadCallback`, `__sinit_0207e6f8`.

### base/ — base object system and small helpers
90 files, ~19 KB, from the early engine code around 0x020083xx–0x02009cxx, with descriptive filenames (`is_kind_of.c`, `alloc_38.c`, `vector_add.c`, `flag_e_set.c`). Covers object constructors that set vtable pointers (`data_020a2b64`) and fields, `operator new`-style alloc+construct wrappers (`func_0202f624(size)` then ctor), virtual type-check / RTTI-like helpers, type-descriptor getters, flag set/clear/test, small math/vector helpers, forwarding thunks and empty stubs. One `.cpp` (`setup_step.cpp`) uses a virtual destructor to emit `delete`.

### vm/ — script virtual machine (LMG bytecode)
20 files, ~4 KB, around 0x020265c4–0x02028160. Hand-named (`play_motion.c`, `current_object.c`, `load_strings.c`, `string_at.c`, `read_u32.c`). Reconstructs the `ScriptMachine` struct (methods vtable, u16 code pointer, pc, wait, current object), the 0x34-byte `LmgHeader`, unaligned little-endian operand reads, the play-motion opcode handler, LMG string-table registration (into `data_020ca00c`), and bitfield/field accessors with `+0x14`/`+0x28` sub-object offset chains. This is the game-logic spine for scene scripts.

### motion/ — animation queue (hand-named fragment)
One file, `queue_motion.c` (795 B). Defines `struct MotionQueue` (padding + state/flags bitfields, `motion_id`, `motion_address`) and `queue_motion(...)`, which stores a motion address, ID and two flag bits and clears a state bit. The hand-chosen name suggests it was identified manually; content is still a single matched leaf.

### ov001 / ov003 / ov004 / ov005 / ov006 — code overlays
Partial-to-small decompilations of DS code overlays, all based at 0x020d3a40, named `func_ovNNN_XXXXXXXX.c`. `ov004` (47) and `ov006` (39) are the most covered: wrappers, field accessors and dispatch stubs over large state objects (ov006 reaches offsets up to ~0xee4 and builds sprite/graphic elements via sibling helpers; ov004 has runs of near-identical stubs that call `func_0202f25c` then `func_ov004_020d7050` with different `0x020e47xx` data pointers). `ov003` (12) is a scene state-machine (state byte +0x168, counter +0x174, index +0x178) that calls a shared print/graphics routine `func_02022f34`. `ov005` (11) holds leaf setters and destructor-like wrapper pairs (C++ complete/deleting dtor shape expressed in C). `ov001` (2) is just a setter plus a constant-argument wrapper. None of the overlay dirs contain data, asm or SWI stubs.

## What this means for the Vita port

- `src/` is a corpus of **matched function bodies**, not yet a runnable engine. It proves the DS code reconstructs byte-for-byte; it does not by itself run on Vita, because the DS hardware boundaries still need host implementations.
- The `srcVita/` layer supplies those boundaries (renderer, input, timing, file, memory, audio) on vitaGL/VitaSDK. It is independent of the matched bodies for now.
- When the port wires DS logic in, the spine is `src/vm` (LMG script interpreter) plus `src/motion` (motion requests) and the `src/base` object system; the `ovNNN` overlays carry the per-scene UI/state controllers. These are the natural hook points for the Chapter 1 target.
- The flat, address-named layout and batch prefixes are matching-campaign provenance; they are not a linkable module structure and will need a real build/link step (already prototyped by the phase-3 linker tooling) before whole-game integration.
