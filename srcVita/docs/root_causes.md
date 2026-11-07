# Why Chapter 1's visual bugs happened

Written after the Chapter 1 audit of 2026-11-06. Evidence is the matched C in `src/thumb`, replay marks, burst captures and the reference footage. The audit notes are in `extracted/audit_notes_2026-11-06/` and stay out of git, because they quote game data.

## The shared causes

### 1. A look was chosen before anyone looked

Sizes, colours, timings and layouts were guessed, then corrected by trial when they looked wrong. The answers already existed in native code, the remaster's data or the footage.

Examples:
- Radiation speed went 4, 8, then 16 ticks by trial. The native table and the footage agree on one value.
- The talk box rule was "no face means top box". The real rule is in the message codes: `0x08` picks the bottom box and `0x1b` the top box.
- The small speaker heads were called art of unknown source, but `cmn_do_10_*` holds them and fits the footage with an error of about 6 of 255.
- The tooltip ring size was invented. `ui010207` has every key.

Guard: every new visual cites its native function or clip and a footage time. Numbers come from a fit, not an estimate. Decode footage with `in_color_matrix=bt601`, because the video is BT.601 tagged as BT.709 and plain decodes skew colour.

### 2. The gap list came from the code, not the game

Anything native-only has no script op, so nothing logged it: the HUD pills, Ghost mode entry and exit, the player mark, possession lightning. The "what is missing" list could only name what the port knew it skipped.

Guard: `extracted/audit_notes_2026-11-06/footage_inventory.md` lists what the footage shows, row by row. Work from that list. Burst watchers compare against the footage, not against the code's idea of what exists.

### 3. Readers dropped content quietly

A strict reader or a one-time log line turned missing content into silence:
- `text skipped` hid 7 narrations with no position tag and 9 shout bubbles with tags the reader did not know. Native ignores those tags.
- `op_not_ported`, `gameapp_not_ported` and `effect_not_drawn` log once and carry on, as the Chapter 1 goal requires, and nobody read them.
- The opcode table's family decides dispatch. A `case` in `run_control_op` does nothing until the op is also listed as `SCRIPT_OP_CONTROL`.

- Reader tables had fixed sizes and dropped rows past the limit without a word. Room plates stopped at 64 of 79 rows, so the split-screen call and a later room drew black while their actors and rain still showed.
- A font tool flag that did nothing (`-alpha copy`) made every button glyph a solid block. Only a look at the pixels could tell.

Guard: read `text skipped` and the not-ported marks in every replay. `make coverage` prints them after `make route-check`. A table that overflows now counts `rows_dropped`, and the log and the replay both report it.

### 4. The view kept its own copy of game state

Ring, label and soul view were flags set once by events, so they drifted from native state: the ring and label showed during ghost-world dialogue and were absent at living-world trick moments.

Guard: derive what is on screen from machine state every tick, as the pills do (`script_pills.c`) and as native does with its board and groups.

### 5. Lazy loading fought a game whose art is resident

Native art is in memory. The port read 8 MB sheets at 9.5 MB/s against a change every 0.75 s, then held old poses to hide the wait, which caused the teleports. Holding a pose is out of step with game state, so it cannot be fixed by tuning.

Guard: packed textures, bounded read-ahead on the script's own timeline, and the art gate rechecked after ticks. `rca/artsim` replays the real cache code against a chapter's log and should report 0 absent and 0 held.

A first open in the texture folder can also cost 120 to 150 ms, against about 1 ms for a repeat. It hits the groups added most recently, such as movie and effect frames, which then read at 5 per second at best. Frames that are read in sequence get a folder per group on the device, so the first open stays short. Each "texture loaded" line now reports `open_us`, `read_us` and `unpack_us`.

### 6. Checks proved logic, and a test harness was shown as the game

The golden compare passes on VM rows and cannot see a soul. Auto play was built to reach the end, then became the demo: nearest core, instant hops, fast forward. A 59.9 fps reading once looked fine while the picture was a slideshow.

Guard: `make route-check` holds the route to the real solution with no forced pushes. Bursts prove the picture. Nothing is called working without a device capture compared with footage.

### 7. One sample was stretched to cover everything

One bubble fit, so its rule became the rule for 49 sites. One tooltip shape stood in for 14 leaders. One burst page stood in for the soul.

Guard: count the sites before writing a rule, and check a rule on more than the sample that suggested it.

### 8. Native coordinates were read without their frame

Objects ignored the world origin that plates do not use, which put models in the wrong place only in some scripts. Camera height ignored the floor bands, which let talk boxes cover corpses. Label sizes mixed DS units and Vita pixels in fields that look alike.

Guard: name the frame in the variable, as `object_y` and `screen_x` do, and convert in one place.

### 9. Loading assumed the memory card was fast

The Vita pads compressed textures to powers of two, so 2880 px character pages took 16 MB each and big animations overflowed the cache, evicting pages still in use. A scene's textures were read only when first drawn, its animations only when first played, and grayscale copies were made on the main thread. Animations froze on old poses, effects flickered as their pages reloaded, and single frames ran 60 to 440 ms at motion starts and scene changes.

Guard: plan what each scene needs at bake time, for every chapter. Atlas pages over 2048 px bake as overlapping tiles, and the loading step writes each scene's read-ahead list and one bundle of its animations. The main thread never reads, unpacks or converts a file, and what a chain loads up front is never cleared at a stage change: clearing the animation library there once threw away every scene's bundle after the first. The log names any animation still read from the card during play. Every device run ends with a stutter sweep over its whole log.

### 10. Native behaviour lived outside the scripts

The corpse light's sound and music, the rewind clip's sound and the second core markers come from native code, not from the scene scripts, so running the scripts alone missed them. Effect movies carry a placement for each frame, which the port first threw away, so the corpse sparks drifted.

Guard: when a sound, cue or marker is missing, find the native state that drives it and port that state's rule for every case it covers, with its data in the set description (such as a sound on each movie row). Check it with a cue expectation over the paced replay.

### 11. Build choices that make did not notice

Switching `FAST_FORWARD` or `DS_MUSIC` changed compile flags but not what make compared, so the package could keep the old program. macOS make compares times to the second, which also hid a changed LiveArea choice.

Guard: keep each build choice in its own output folder, relink the program and repack the package on every build, and confirm a switch from what the package holds.

### 12. Auto-play acted as soon as it could

Each route step waited for its condition plus one fixed dwell, while the footage player often took longer. Script content timed against the player's pace then never ran: a speech balloon chain was cut short because the route entered ghost view and tricked before its second line was due.

Guard: route steps carry their footage time. The loading step replays the chapter, compares each step's start with the previous step's in the footage, and writes `route_pacing.txt`, which stretches any step that acts sooner than the footage did, up to 5 s more. A stretch that makes a step miss its window is dropped. Gaps beyond the cap are listed as drift, which points at content the port does not build yet.

### 13. A sibling op's meaning was assumed

Op 0x4d closes only native balloon slot 1, the Cicel tap balloon, but the port read it as the close-all of op 0x57. Every 0x4d in a chapter could shut an actor's speech balloon that the footage keeps open. Likewise op 0x45 with label 0 closes slot 0, which the port ignored.

Guard: read each op's row in `extracted/trace/vm_golden/opcodes_media.md` (from the matched interpreter) before sharing a case between ops. Tap balloons keep their native slot as their tap group, and closing ops close by group.

## Still open

- Which verb an object's action line shows. `st01_id_st01a_acn_*` holds state variants and native selection is not traced, so objects with an empty first variant show no action.
- Telephone, Lynne and the dead core have no name entry in the tables read so far.
- Action selection for player mark layouts: 14 shapes exist and the port uses 4 by a position rule.
