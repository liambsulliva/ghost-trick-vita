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

### 14. Memory went to padding, and grayscale copies skipped the plan

The Vita pads each texture to powers of two, so a 1300 px page took the memory of a 2048 px one. A scene's pages then outgrew the cache, which evicted pages still on screen and read them back dozens of times a minute. Grayscale copies were neither tiled nor read ahead, so every flashback loaded whole 16 MB pages on first draw and froze for seconds.

Guard: the converter splits any page whose padding wastes a quarter of its memory, and the tile table gives every tiled page a grayscale twin. The loading step follows the screen filter and grey-world effects through the paced replay and plans the grayscale copy of everything on screen when grayscale starts. The log counts tiles the table cannot hold.

### 15. A chapter spans several stages and unported ops hide in the next one

Chapter 2 runs through `chapter.xml`, two stages and `phone.xml`, so a set tied to one stage folder could not bake it, and the parlor stalled on op 0x69, which Chapter 1's chain never used. The new stages also brought a pageless timeline, an 18 kHz sound and an empty action label.

Guard: a set names its stages, entry, chapter mark and any extra scripts to follow. Replay each new chapter on the Mac before the device, compare stalls against native handlers, and make each converter refusal a rule for its whole class.

### 16. Limits and tables were sized for one chapter

The machine held 32 scripts and the talk book 1024 messages, so the whole game's 163 scripts and 1979 messages were dropped. The talk book then failed to parse at all and every talk row went missing. Op 0x74 was filed as store-only in the op table, so its handler never ran, the chapter mark never advanced and chapter.xml sent every chapter back to the first stage; Chapter 2 only worked because its set lacked st01 and the wrong jump fell through.

Guard: replay the whole-game set from Chapter 1 on the Mac; a row the size check drops stops it. `tests/tools/check_op_table.py` lists any op whose table entry hides its handler.

The animation library had the same limit: 500 animations and no way to let one go. The whole-game set reached it 82 s into the device run, and after that every new animation logged `library_full` and never drew. A cache that lives for the whole game evicts the least recently used entry that nothing on screen plays, as the texture and sound caches already did.

### 17. Per-set data drifted between chapters

Each set carried its own copy of shared data: the DS music map, the rewind movies' sound, the first core. Chapter 2's copies were partial, so its rooms played the arranged music, Missile's rewind had no clock sound, and the whole-game set lost Chapter 1's tutorial core.

Guard: derive what native code decides instead of listing it; a DS track is now the sequence at its music id's index. Next, move the remaining shared rows into one file every set inherits.

### 18. Ops with no family entry ran as silence

The whole game's scripts use 89 opcodes that the op table routes nowhere, so each one logs `op_not_ported` once and does nothing. Chapters 1 and 2 got through because most of those are visual. Among them were the register ops:
- 0x127 copies a register into `reg_a`. 0x12d and 0x12c then write `reg_a` into the script's own code, as the value of the compare two ops later.
- 0x128 and 0x12a set and decrement registers.
- Registers 0x0c to 0x10 are per-actor byte counters, and register 3 is the actor's flip bits. The port read all of them as 0 and dropped every write.

So in the four-minute puzzle the check that compares the girl's X with the caster's X compared with 0, the girl never got her move event, and the puzzle stalled. Native drops register writes, including to `reg_a`, when the VM has no current actor (`func_02028410`).

Guard: `tests/tools/check_unrouted_ops.py` lists every opcode in a baked set with no family entry, and every use of the counter registers. Read its list before routing a new chapter, and rank the unrouted ops by what their native case writes: anything that sets a register, a flag or script code goes first.

### 19. A failed load became a skipped asset

After Chapter 1 the rooms had no backgrounds. Four rules let that happen:
- **A small fixed heap.** The app heap was 48 MB, and the whole-game set filled it with scripts, talk, sound and animations. Every texture read needs a heap buffer for the file and its unpacked pixels, so those reads failed. Meanwhile vitaGL held 160 MB of RAM it never used.
- **Failures treated as final.** A failed read was dropped until something asked for the texture again. A failed GPU upload was marked missing for good.
- **A misdiagnosis.** A heap shortage was read as texture memory running out. The texture budget was cut to its 32 MB floor, which freed no heap at all and made rooms evict their own pages.
- **An art gate that let go.** It never held in fast forward and gave up after 4 s, so the story moved on with nothing drawn.

Guard:
- The heap is 112 MB, and the `memory` log line shows `heap_used`, `heap_size` and the live GPU texture count, so a filling heap or a leak shows before it fails.
- A read that runs out of heap, a full read queue and an upload with no GPU memory all keep the texture loading and retry it. The upload case frees an old texture first.
- The art gate holds in fast forward too and gives up only after 30 s, with an `art_wait_gave_up` line.
- Fast forward is a double gate: it runs only the ticks that fit an 8 ms update budget, drops to normal speed while any texture is still loading, and tops out at 8 ticks a frame. Slower logic is fine; skipped art and story are not.

### 20. One camera write skipped the room clamp

Native clamps the camera to the room on every write, to `[0, w-256]` and `[0, h-192]` (`func_0200f57c`, applied by `func_0200f1e0`). The port clamped camera cuts and pans but not the camera a room load sets. Chapter 2's first room is 256x192 and its script asks for y 192, so the port drew it one screen below its only plate: a black screen for the whole opening.

Guard: every write to `camera_x` and `camera_y` in `script_ops_view.c` goes through `clamped_x` and `clamped_y`.

### 21. A name that is unique in one stage was used as a key for the whole game

A motion table's animation was named from the table alone, such as `t_door_wait`, and the converter kept one table-to-art map for the whole set. Every stage has its own `door_wait`, `kasa_wait` or `gaku_wait` with its own art, so the whole-game set drew stage 4's door in Lynne's apartment and stage 1's umbrella in stage 6. 175 such generic tables were exposed. Chapter sets never showed it, because each held one or two stages.

Guard: tables resolve per script. When one table name means different art in different scripts, its animation name carries the art folder. A set that spans stages must never key game data by a name that is only unique inside one stage; check sounds, effects and talk entries the same way when a new kind of data joins a multi-stage set.

### 22. State the port kept but never read back, or never applied

Three native rules were half ported:
- **Fate level.** Ops 0x102 to 0x105 set the 3-bit fate field, which the port kept as `fate_level` for the hourglass. Register 0x16, which 85 script branches read, still returned 0. The writers use C bitfields, so a search for masks missed them.
- **Camera shake.** Op 0x2a stored a shake that nothing applied.
- **Balloon slots.** Balloons used one pool of six that overwrote slot 0 when full. Native keeps slots 0 and 1 for tap balloons and evicts the timed balloon with the least time left from slots 2 to 7.

Guard: when an op stores state, find its native reader too and port both together. When looking for writers of a field, search for the bitfield struct as well as the mask. `tests/tools/check_unrouted_ops.py` lists counter-register use. Registers 0x11 and 0x17 still read 0: 0x17 needs the entry list op 0xd8 keeps (`func_020101fc`), and 0x11 needs the writer of a target's mode bits at +0x14.

### 23. Pictures paired with the footage by the wrong clock

A route step's `footage_s` is when its action fires in the footage, but bursts stamped time from when the step began waiting. A step that waits through a long talk began on the device right after the previous action, so its pictures were paired with footage up to 100 s later: the watcher reported missing balloons, missing characters and a lagging device in Chapter 1 steps 20 and 23 that were only pairing errors. Footage talk also runs at the player's reading speed, which the device does not match (step 20's talk is 100 s in the footage and 66 s on the device).

Guard: bursts (`GTB3`) stamp the last route step whose action fired and the ticks since then. The device log's mark lines carry `tick=`, and `tools/burst_reference.py --log` stretches each window between two actions to the footage's length, so both ends line up. Before reporting a mismatch inside a long talk, compare the order of talk lines, not the time.

### 24. Read-ahead evicted what was on screen

Read-ahead marked every texture in its window as used last frame, the same age as textures drawn last frame, and its loads evicted the least recently used texture. Once fast forward multiplied the window, the window alone filled the 159 MB budget (one close-up animation is 9 MB a frame), and read-ahead loads evicted textures on screen, which were read again at once: in Chapter 1 steps 41 to 54 the same five textures reloaded every 0.2 s and the frame rate fell to 13 to 36.

Guard: read-ahead only uses spare room. It never evicts a texture drawn in the last 60 frames or another texture it still wants, it starts no read without room, and a read-ahead upload that finds no room is dropped (`read_ahead_dropped` in the textures log line). Drawing may still evict anything not drawn this frame, read-ahead textures first. When a log shows the same texture loaded again within a second, look for a loader that evicts by a clock it shares with drawing.

### 25. A native class keyed by one of its effect ids

The soul sent the phone border's `ready` and `end` events only for effect id 49. Native maps effect ids to kinds (`data_02086624`) and kinds to class factories (`data_020ac194`): ids 48 and 49 both build the phone border (factory 0x020ce9c4), and Chapter 2's first call spawns 48, so the telephone waited for events that never came and the run sat in the split screen until the stuck detector pushed its handlers a minute apart. Id 48 is also spawned in stages 1, 3, 9 and 11.

Guard: when port behaviour belongs to an effect class, key it by every id whose kind uses that factory. Before keying a rule by effect id, list the ids sharing its factory from the two tables. The pill buttons (99 to 102, factory 0x020ce820) share their class with ids 242 and 243, which stage 13 spawns and the port does not handle yet.

## Still open

- Which verb an object's action line shows. `st01_id_st01a_acn_*` holds state variants and native selection is not traced, so objects with an empty first variant show no action.
- Telephone, Lynne and the dead core have no name entry in the tables read so far.
- Action selection for player mark layouts: 14 shapes exist and the port uses 4 by a position rule.
