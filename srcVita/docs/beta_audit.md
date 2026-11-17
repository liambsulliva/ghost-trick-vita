# Beta audit (v1.0.0-beta.1)

An audit of the port at `4ae19e47`, before the first public beta. It finds and ranks problems; it fixes none. Each entry gives its severity, the set it affects, the evidence, the native function and a rough effort (S under a day, M a few days, L a week or more).

Severity: **blocks beta** (a player cannot go on, or loses progress), **visible** (a player will notice), **minor** (polish, log noise, test gaps).

## What ran and what did not

- Liam's Mac could not be reached for the whole session (`mac-run --check`: ssh to the Mac timed out). So nothing heavy ran: no smoke test (quick or full, either set), no contact sheets, no `pause_render.py` or `scene_render` pictures, no `route-check`, no `script_replay --marks`, and no `tests/tools/check_unrouted_ops.py` over the baked scripts.
- Everything below is static: the port's VM against the decompiled interpreter `func_02023fdc` (`src/thumb/parallel_escalate_x_func_02023fdc.cpp`), the smoke lists, the README, `root_causes.md` and `game.json`.
- Because the bake was out of reach, op use counts come only from what `root_causes.md` already records. Ops it does not mention have no count; run `python3 -I srcVita/tests/tools/check_unrouted_ops.py` on the Mac to get one.
- Part 2 of the brief (looking at every screenshot, DS mode especially) has not been done. The DS entries below come from code and `game.json`, and each needs a look on screen.

## Ranked findings

### 1. Most talks with the dead cannot be started by a player — blocks beta (both sets)

- Evidence: `src/app/script_soul.c:1471-1486` starts a dead talk from player input only when the scene handles both `SYSTEM|SV_DEADCORE` and `SVTUTO|onClick` (the tutorial kind). Every other dead talk starts only from the route's `dead_talk` step (`script_soul.c:2770`), which logs `route_no_player_input` (`tests/smoke/log_kinds.txt`, `docs/routing.md:18`). Root causes 43 and 92 (`root_causes.md:313`, `:637`) keep "the yes/no prompt for every other dead core" open. `game.json` has 21 `dead_talk` route steps, from st06_game004 (Chapter 2) to st14_game061 (the last chapters): Jowd (`j_core`, st03_game012 and 020), Kamila (`deadtalk`, st07_game000), `linne_core` (st02_game023, st04_game010), `cicel_d` (st14_game061) and more.
- Why it blocks: the smoke test and the whole-game replay finish because auto-play stands in for the player. A person playing on the Vita who reaches a non-tutorial dead core sees no prompt and has no button that starts the talk. If any of those talks gates the story (Jowd's and Kamila's talks look like they do), the game cannot be finished by hand. Confirm on the device with a save at st03_game012 before anything else.
- Native: `func_02032914` (ghost-view update) → `func_02033408`: yes/no prompt on pills 10 and 11 (trigger bits 0x1000 and 0x2000), on yes push `pGameMainDeadTalk` (state 0x15), then `func_0203bd84` / `func_0203bfc8` send DEADTALK or CORETALK.
- Effort: M. The talk itself, its targeting, Finish, Abort and Restart are already ported; what is missing is the two pills, their art in both sets, and the confirm path into `start_dead_talk`. The route's `dead_talk` step should then press the yes pill through `script_route_pad` like every other action, so the smoke test proves the player path.

### 2. Op 0xfd (rollback checkpoint) is not ported, so a game over can restart an earlier scene — visible, possibly blocks (both)

- Evidence: `src/app/script_opcodes.c` has a length for 0xfd and no family, so `run_op` logs `op_not_ported` (`script_machine.c:1537`). `root_causes.md:1245`: "Without op 0xfd a game over in the torpedo restarts at `st14_game031_Expand`". Hit in st01_game012, st01_game021, st02_game022, st09_game003, st13, st14_game021, st14_game040 and others (`root_causes.md:2317`, 949, 1120, 2334).
- Native: case 0xfd → `func_02011b04` copies the scene name (`game+0x1c`) to `game+0x1058` and builds the checkpoint at `game+0x1098`; the rollback menu's second entry reads it through `func_02011c80`.
- Effort: M (the store is S; the retry path that uses it, and the rollback menu in item 9, are the rest).

### 3. Unported ops that write the script's condition or hold it — visible, may break branches (both)

Same class as root cause 18 and the op 0x6b entry (`root_causes.md:387`): an op with a length and no family logs `op_not_ported` and leaves `vm->condition` or the pc as it was, so the next test branches on a stale value or the script runs on past a wait.

| Op | Native case in `func_02023fdc` | What the port loses |
|---|---|---|
| 0x6c | waits while `func_0202657c(game) != 0` (a scene change is pending) | the wait; the script runs on into the change. 0x6b, its twin, is ported (`script_machine.c:1212`) |
| 0x78 | `s->b10 = func_0201fa5c(game+0xe4c[w1], w2, w3)`: whether entry `w2` of list `w1` is in range and either empty or passes `func_0201f810(entry, w3)` | the condition write. The list at `game+0xe4c` is the one op 0x75 (filed stores-only) writes with `func_0201fcd0`; trace both together |
| 0xa8 | `s->b10 = 0` | the condition write |
| 0xe4 | returns 0 without advancing the pc, so that VM never runs again, as ops 0, 1, 0x112 and 0x113 do | the stop: the port steps over it and runs whatever follows |
| 0xc2 | waits until `game+0xee0` exists and `func_0200a5dc` is true | the wait (0xc3, `func_0200eee0`, stops the same object) |
| 0xf2 | waits while `func_02022fa4(sequencer, data_020850b0)` (a state op 0xf1 pushed with `w1` in `data_020cff60`) | the wait and the state 0xf1 pushes |
| 0x38 | starts the message whose label is `w1` in the message object at `game+0xf04` (`func_02015a54` → `func_02014a8c`), sets bit 2 of `game+8`, then yields | a talk that never shows |

- Effort: S each for 0x6c, 0xa8 and 0xe4 (see quick wins); M for 0x78/0x75 and 0x38 (need tracing); M for 0xc2/0xc3 and 0xf1/0xf2 (identify `game+0xee0` and `data_020850b0` first).

### 4. Other ops with no family entry — visible or minor (both)

`script_opcodes.c` gives these ops a length and no family, so each logs `op_not_ported` once and does nothing. Grouped by what the native case does.

- Mode requests: 0xda, 0x100 and 0x101 call `func_02011080(game, 9 / 10 / 11)`, the same request byte (`game+8` byte 2) that code 12 uses for the ghost view; `pGameMainNormal`'s update (`func_02032488`, lines 99-105) pushes `data_02099e74` with argument 0, 1 or 2 for them. So they enter a ghost-view-like sub-state from the script. Visible; M.
- Actor and motion: 0x9a (`func_0205902c` on a human, a facing or pose call; st03_game003), 0xaf (stops an actor through vtable slot 15 after `func_02059020`), 0x119 (links two actors' motion controllers, `func_02059894`; st03_game031, `root_causes.md:2319`), 0x8c and 0x8d (`func_02027f74`, `func_0202810c`: write a byte or a property on the current actor). Visible; M.
- Camera: 0xd1 and 0xd2 save and restore the camera (9 and 8 sites from st04_game001), 0xd3 stores a point at `game+0xdac` (52 sites in 14 scripts), 0xc5, 0xc6 and 0xd4 move the top state-1 object of type `data_020ce1e8` (st15_demo110). `root_causes.md:2328-2334`. Visible; M.
- Balloons: 0x47 (tap balloon on a human, `func_020104b4`), 0x48 (`+0x124` of balloon slot 0, st01_game020), 0x49 (`+0x11c` of balloon slot 0, st02_game000, st03_game003, st09_game004), 0x4f (`func_0201062c`). `root_causes.md:1408` notes the tap rules skip 0x47 and 0x4f. Visible; S-M.
- Command objects by name: 0x14, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8 and 0xeb each make a 4-byte command (`func_0202697c`, `func_02026a64` … `func_02026ab4`, `func_02026af0`) and run its `c2(script, name)`; 0xb5 and 0xb6 are hit in st05_game001, 0xb6 and 0xb7 in st14. The command classes are not identified. Visible; M.
- Objects and stills: 0xdd and 0xde make and remove an object of type `data_020cfcf0` (0xde also calls `func_0200c82c`); 0xf4 is op 0xf3's still plus `func_0202681c` (byte `+0xc9` = 1) on the `uSpriteCaptureNormal`. The port routes 0xf3 and 0xf5 but not 0xf4. Whether `+0xc9` keeps the still grey, which would explain the Known issue of the film still after "(Unconscious, eh?)", is a guess to check. Visible; S-M.
- Audio and globals: 0x10f (`func_0202a90c` on the sound object, `func_0200847c`), 0x169 and 0x16a (`data_020abf38`, `data_020abf34`; the second is read by `func_020521d4`), 0x8 and 0x9 (`game+0x103e` = 1 and `+0x1040` = 1 or 0; st15_demo110). Minor until a reader is found; S each.
- No-ops natively: 0x44, 0x7d, 0xab (calls the empty `func_020280c4`, `src/vm/empty_0c4.c`) and 0xdc (advance, and 0xdc yields). They only add `op_not_ported` noise. Minor; S.

### 5. Routed ops that do nothing although native does something — visible (both)

- 0x7f and 0x80 (`script_machine.c:1268`) return at once. Native calls vtable slots 28 and 27 of the object `func_0201003c` returns (`func_02015748`, `func_02015418`), the rollback controller that `func_02015428` drives. They belong with the rollback menu (item 9). M.
- 0x43 (choice family) has no `case` in `script_choice.c`. Native calls `func_02015bdc` on the child at `+0xc` of `func_0201003c()` (`func_02015bcc`). S once the child is known.
- 0x75, 0x7c and 0xee are filed stores-only. 0xee is fine (`root_causes.md:873`). 0x7c's string is the Minutes Before Death card's time (`root_causes.md:2325`), so the card's middle line is not drawn. 0x75 writes `func_020265ec(game, w1)` and `func_0201fcd0` on entries 0 and 2 of `game+0xe4c`, the list 0x78 reads (item 3). M.

### 6. Modes inside ported ops (GameApp commands, eyecatch kinds, motion words) — not yet audited

The comparison against native is in progress and will be added here.

### 7. DS mode draws nothing in slots the cartridge art is not decoded for — visible (DS)

`game.json` `ds_absent_textures` lists the slots the DS set leaves empty, each logged `texture absent`. A player in DS mode sees none of these:

- `chapter_card_00` ("the DS card is not decoded"), `staff_*` (the staff roll pages: DS-mode credits show no pages), `wipe_time_passing`.
- `film_corner`, `film_strip` (the film frame around stills and the split screen), `phone_cap`, `phone_strip` (the phone book), `ui_mark` (the player mark, `script_view.c:65`), `ui_hourglass` (the hourglass pill, `script_ui_view.c:70`; "the DS has three whole frames, the app wants parts"), `core_ring`, `ui_wave_quadrant`, `side_label`, `card_hand_seconds`, and the death count (`digit_0` to `digit_3`, `digit_label`).
- Native: each comes from `cpac_2d.bin` / `cpac_3d.bin` art in the ROM; `game.json` names the block where known.
- Effort: M to L in all; the player mark, the film frame and the hourglass are the ones a player meets in every chapter. Check each on screen before ranking further.

### 8. DS mode has six smoke flows; the remastered set has 25 — minor (DS)

`tests/smoke/flows.txt` runs only `opening_normal_speed`, `choice_panels`, `whole_game` (full), `pause_and_resume`, `visual_filter_switch` and `talk_boxes` on the DS set. The title, Continue, Save and Continue, Chapters, the Record, the retry after a failure, every route action, Options, touch, hints and the nine languages run on the remastered set only. Most recent device reports (root causes 290 to 298) were DS-only. Effort: S per flow (add `ds` to `sets` and fix what fails).

### 9. Rollback menu after a failed rescue or the hourglass is not drawn — visible (both)

The port retries at once with the native default and draws no menu, so an hourglass press cannot be cancelled and the checkpoint entry (item 2) is missing. Native: `func_0203b72c` from `func_02015428`; `SYSTEM|RESTART ROLLBACK` (`pGameMainMovieRollbackShort`, `pGameMainRollback`). `root_causes.md` Still open. Effort: L.

### 10. Known lines and log kinds hide new problems — minor (both)

- `tests/smoke/log_kinds.txt:83` lists `known script *_not_ported`, a wildcard over `op_not_ported`, `gameapp_not_ported` and `eyecatch_kind_not_ported`. A newly reached unported op never fails a flow. `tests/tools/check_unrouted_ops.py`, the guard root cause 18 names, is not run by any `make` target (only `check_undrawn_effects.py` is, `Makefile:243`).
- Stale reasons: `known title needs_art` says "the DS set bakes no title art, so it starts in the opening", but `ds_pause.py` now bakes the DS title buttons and `visual_filter_switch` reaches the title in DS mode. While the kind stays `known`, a DS set that lost its title would boot into the opening and pass. Line 83 cites "README, Future updates" and several lines cite "Still open"; the README has neither section (it is `root_causes.md`'s).
- `tests/smoke/known_gaps.txt` is empty and no flow has a `known` line, so none hides a bug today.
- Effort: S.

### 11. `texture missing name=hud_button_art/hud_sheet` on the device — minor, probably a stale set (DS)

Both names went into `ds_absent_textures` in `5f20d0c7` (2026-11-15), the day before the device log. A DS set baked before that commit has no `absent_textures` entry for them, so the cache logs `missing` instead of `absent`. `viewer_app.c:554-557` reads the absent list at every set start, the in-process switch included, so the code path looks right. Confirm by reading the device set's `absent_textures` file, or rebake and reinstall the DS set (with Liam's OK). Effort: S.

### 12. Known issues and Still open entries that look stale — minor

- `root_causes.md` Still open says op 0x9f "is not ported"; `script_motion.c:1057` now runs it (`set_core_info`). Its Chapter 18 entry lists 0xf6 as unported; `script_machine.c:1244` runs it.
- README Known issues ends with a paragraph about the VM's timing source ("The VM's timing comes from the script…") that is not an issue, and the `## Debug controls` section opens with two paragraphs about the loading screen.
- The README's Known issues name no DS-only differences at all, although item 7 lists many. A beta's notes should say what DS mode lacks.
- Effort: S.

### 13. Other still-open items worth a beta note — visible (both)

From `root_causes.md` Still open, unchanged by this audit: telephone lines and neon are tracked and not drawn; effect classes with no port (`uTimeOverChainEffect` 50, `uPhoneBorderEffect` 59, `uBlackDarkness` 151/228/229, `uLanternLightBlackSt02_01` 119 with 57 sites in stage 2, `uNightVisionGoggle`, `uElectricTorch`); the ripple (effect 6, 50 sites); one light circle at a time; the title screen plays no music and shows no boot logos; 49 talk and balloon labels the remaster's text lacks; the Minutes Before Death card's middle line. Each is already documented there with its native function.

## Quick wins

Small, low risk, fixed in the shared op table or the VM rather than per asset, as AGENTS.md asks. **Not applied: waiting for Liam's approval.** Each needs a unit test that runs the op in `script_machine` and, for the class, a check that fails on the next op of the same kind.

1. **Op 0x6c**: route it to `SCRIPT_OP_CONTROL` and hold (`SCRIPT_STEP_MOVED_THEN_YIELD`) while `script_change_is_pending`, next to 0x6b. Native case 0x6c in `func_02023fdc`, `func_0202657c`.
2. **Op 0xa8**: route it and set `vm->condition = 0`. Native case 0xa8.
3. **Op 0xe4**: route it as a stop, as ops 0, 1, 0x112 and 0x113 are (`script_machine.c:1104`). Native case 0xe4 returns 0 without moving the pc.
4. **Ops 0x44, 0x7d, 0xab, 0xdc**: route them as the no-ops they are natively (0xdc yields), so `op_not_ported` only names ops that do something. Native cases 0x44, 0x7d, 0xdc; `func_020280c4` is empty.
5. **Op 0xf4**: share 0xf3's case (`script_machine.c:1240`), as native does (`case 0xf3: case 0xf4:`). The extra `+0xc9` byte can stay unported until its reader is found.
6. **Make unported ops fail the smoke test**: replace `known script *_not_ported` with `known` lines per op for the ones left, and run `check_unrouted_ops.py` from `make test` against a checked-in list of the ops still unrouted, so the list can only shrink. The class guard root cause 18 asked for.
7. **`title needs_art` → `fail`** now that both sets bake title art, and fix the `log_kinds.txt` reasons that cite README sections that do not exist.
8. **Stale docs**: drop 0x9f and 0xf6 from Still open (item 12) and move the README's misplaced paragraphs.

## Next steps once the Mac is back

1. Play st03_game012 (Jowd) and st07_game000 (Kamila) by hand on the Vita, or in `smoke_host` without auto-play's `dead_talk`, to confirm item 1.
2. `make -C srcVita smoke SMOKE_QUICK=1`, then the full run, both sets. Bring back `build/smoke/report.txt` and `build/smoke/pictures/` with `--get`, and look at every DS sheet against item 7.
3. `python3 -I srcVita/tests/tools/check_unrouted_ops.py` on the game set, to put use counts against items 3 and 4.
4. `make -C srcVita route-check` and `script_replay --marks` for `route_step_failed`.
