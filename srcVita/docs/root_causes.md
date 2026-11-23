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
- The general heap is 112 MB (128 MB with the reserved read block of root cause 194), and the `memory` log line shows `heap_used`, `heap_size` and the live GPU texture count, so a filling heap or a leak shows before it fails.
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

Even anchored and stretched, times drift over a long run, and timing gaps are not what the comparison is for. `burst_reference.py` now searches the footage within 45 s of the expected time for the closest-looking frame (colour, 64 by 48, the talk box area left out) and pairs each picture with that. It scores the pair: same shot (difference up to 0.08), possibly the same shot (up to 0.11), or no close shot, and writes `matches.tsv`. Review what differs inside a same-shot pair; a picture with no close shot is a scene question, not a timing one.

The log also had two clocks: `cue` and `soul_vision` lines used the bridge's tick, which counts on while the script machine's tick holds, and ran 400 ticks ahead of the burst stamps by Chapter 1's middle, so the watcher read the ghost view as off where it was on. Every device log tick, route mark and burst stamp now uses the script machine's tick.

### 24. Read-ahead evicted what was on screen

Read-ahead marked every texture in its window as used last frame, the same age as textures drawn last frame, and its loads evicted the least recently used texture. Once fast forward multiplied the window, the window alone filled the 159 MB budget (one close-up animation is 9 MB a frame), and read-ahead loads evicted textures on screen, which were read again at once: in Chapter 1 steps 41 to 54 the same five textures reloaded every 0.2 s and the frame rate fell to 13 to 36.

Guard: read-ahead only uses spare room. It never evicts a texture drawn in the last 60 frames or another texture it still wants, it starts no read without room, and a read-ahead upload that finds no room is dropped (`read_ahead_dropped` in the textures log line). Drawing may still evict anything not drawn this frame, in this order: textures nobody wants, then read-ahead guesses, and textures drawn in the last 60 frames last. Run 13 had that order the other way for drawing (read-ahead guesses kept ahead of recently drawn pages), and a stage 6 character whose animation alternates between two pages reloaded each of them every 0.15 to 0.6 s. When a log shows the same texture loaded again within a second, look for a loader that evicts by a clock it shares with drawing.

### 25. A native class keyed by one of its effect ids

The soul sent the phone border's `ready` and `end` events only for effect id 49. Native maps effect ids to kinds (`data_02086624`) and kinds to class factories (`data_020ac194`): ids 48 and 49 both build the phone border (factory 0x020ce9c4), and Chapter 2's first call spawns 48, so the telephone waited for events that never came and the run sat in the split screen until the stuck detector pushed its handlers a minute apart. Id 48 is also spawned in stages 1, 3, 9 and 11.

Guard: when port behaviour belongs to an effect class, key it by every id whose kind uses that factory. Before keying a rule by effect id, list the ids sharing its factory from the two tables. The pill buttons (99 to 102, factory 0x020ce820) share their class with ids 242 and 243, which stage 13 spawns and the port does not handle yet.

### 26. Fast forward slowed for art nobody was drawing

Fast forward fell to one tick a frame whenever any texture was loading, and read-ahead keeps textures loading most of the time, so a run asked for 8 ticks a frame averaged 245 ticks a second, about 4 times normal speed, while update used 1.7 ms of each 16.7 ms frame.

Guard: fast forward slows to one tick a frame only while a texture that is being drawn is loading, and to a quarter while read-ahead is behind. Its tick budget fits itself to the frame: it grows while frames make vsync and shrinks when one misses, up to 32 ticks a frame. `script fast_forward_speed` logs the ticks a second it reached, its budget and how many frames art slowed it.

At 22 times speed the next limit was the cache's own bookkeeping: read-ahead looked up every texture name in its window each frame by a linear search of all 2048 cache entries, and names it could not load yet ran two more full scans, so update took 27 to 46 ms with two ticks to run. The cache now finds names through a hash index and checks read-ahead room once a frame. When a per-frame loop calls into a cache, count the work per call, not per frame.

### 27. Interface art that loads when its screen opens

The Rewind Time screen asked for its three rock layers only when it opened, so its first pictures were white with a faded label while the rocks loaded. The pill and tap balloon art did not show this because the UI view touched them every tick.

Guard: every fixed interface texture (hourglass, side label, Rewind Time rocks and button, tap balloon, narration arrow, pill art and sheet) is on one resident list that the UI view touches every tick, so it loads at the start and is never evicted. A new interface screen adds its textures to that list. They total about 680 KB.

Run 20 showed the Minutes Before Death card without its clock and disc for its first quarter second in stage 6: the UI clips (cards, banners, time wipes, death-count digits) were never on the list, so their art was evicted after Chapter 1 and read again when the card opened. The UI view now also keeps every texture any loaded clip names resident, read from the clip table itself, so a new clip needs no list entry. Clip art adds about 1.3 MB.

### 28. A script change that loaded its animations one search at a time

Each script change loads that script's animation bundle on the main thread, and every frame stall over 150 ms in a whole-game run was one of these loads (190 ms to 540 ms). Reading the file took 50 to 130 ms. The rest came from searching: every bundled name was looked up by a linear search of the 500-animation library, and once the library was full every new animation ran its own eviction scan, which asked every scene object whether it plays each of the 500 animations.

Guard: the library finds names through a hash index, and a bundle first counts its new animations and makes room for all of them in one pass. Reading the next script's bundle before the change is still open: the new script's first acts need their animations in the same tick, so the read cannot simply move later.

Run 13 showed the first bundle still taking 556 ms: a 258 ms read and about 3 ms of parsing per animation, because each frame line went through `sscanf` with eight `%f` fields, which is slow in the Vita's C library. Frame lines are now read by a direct decimal reader for the converter's `%.6f` format, with `sscanf` kept as the fallback; it matches `sscanf` on all 609,781 baked frame lines.

Run 14 still logged 114 to 324 ms changes where only a few animations were new (7 in 192 ms for one script): the bundle's name lines also went through `sscanf`, twice per animation, once to count new names and once to add them. Name lines are now read by a direct reader; no `sscanf` remains in the bundle path.

### 29. Effect art listed by hand, one id at a time

Effect rows and effect movies were written by hand for the effects Chapter 1 used, so 152 of the 178 effect ids that the whole game's scripts spawn drew nothing. Stage 6 draws characters such as its sofa man and the dog's poses as effects, so whole scenes played empty.

Native maps an effect id to a kind (`data_02086624`); kinds below 0x8000 are sprites, and the remaster's effect list for the spawning script (`resourcelistuserdata/effect/<script>`, falling back to `cmn`) holds the movie at row kind, or kind + 1 from kind 0x86 up. That rule reproduces all 9 hand-mapped movies.

Guard: the converter reads the kind table and every script from the ROM at bake time and adds a movie row for each spawned id that has none, so a new chapter's effects arrive without hand rows. Hand rows still win, so a hand row can still fix a wrong guess. The bake prints the ids whose art differs by stage (only id 60, two noise variants, keeps the first). Class kinds (0x8000 and up, 48 ids) still need class code. The device tables now hold 512 effect rows, 256 movies and 16384 movie frames.

### 30. Game-wide tables searched row by row

The whole-game set holds 14,169 motion rows, and every actor change looked its row up by a linear search. Rows of later scripts sit later in the table, so stage 6's scenes paid the most: about 2 ms a tick, which held fast forward to 300 ticks a second in the opening demo of the house next door (38 s of wall time) against 1,900 in Chapter 1.

Guard: motion rows are found through a hash index on script and table offset; the first row of a duplicate still wins. Any lookup that runs per tick or per actor over a table that grows with the set must be indexed. A Mac replay cannot show this (it runs the machine without the bridge), so measure per-tick cost on the device with `script fast_forward_speed`.

### 31. Waiting a minute for a native message that never comes

When a script waits for a message only native code sends, the stuck detector pushes the waiting handler after 3000 quiet ticks. In the house next door, the Santa trick leaves the lady at her typewriter looping until the chair's `core|action` arrives, so a routed run sat there 110 s of game time where the footage moves on in 5 s.

Guard: while a route drives auto-play, the detector pushes after 1200 quiet ticks (the Santa wait drops to 35 s of game time, the whole-game replay still finishes all 180 steps with the same 11 pushes); manual play keeps 3000. This only shortens the wait. Each `stuck_pushed_handler` line still names a native sender to port: here, what sends the chair its action after the Santa trick.

### 32. A bake that deleted the step after it

The converter removes every file in its output folders that it did not write, and the read-ahead step (`make read-ahead`) writes `.ahead` lists, `.anims` bundles and `route_pacing.txt` into the same scripts folder afterwards. Re-baking the game set for new effects deleted all 261 of them. The device kept its old copies, so nothing broke on screen, but the next upload would have shipped a set with no bundles or read-ahead.

Guard: the converter keeps the read-ahead step's outputs. After any bake of a set, run `make read-ahead PLAN_SET=<set>` before uploading, and check the scripts folder's `.anims` and `.ahead` counts before an upload.

### 33. A movie that played on while its frames were still loading

Run 14's rewind clock froze for over a second with the numerals half flown in, then jumped to the full dial. Full-screen movies are one texture per frame, and the art gate let game time run during a movie, so the movie's clock moved on while its frames sat in the read queue about a second behind; the screen kept the last frame that had arrived.

Guard: while a movie covers the screen, the art gate holds game time until the frame it has reached is loaded, and before the first frame shows it waits for the first 12, the same "slow over skip" rule the rooms follow. The movie's sound now starts on the tick its first frames are ready, so holding does not leave the picture behind the sound, and it is played once even when the script also asks for it in the same tick (a class movie's op does). A sound of that kind asked for in any later tick plays as usual; the first version dropped every one until the frames arrived. A new script clears the playing movie.

### 34. Effects spawned by motion tables, not by the script

Run 15 logged a dozen stage 6 effects as not drawn (the dog's bark, the music box, the stand, a muzzle flash and others) although the remaster has art for all of them in that script's effect list. The bake found effect ids only in the script's spawn op (0xb0), and these come from the effect command inside motion tables (tag -8), which the port already runs. Sounds were collected from both places; effects were not.

Guard: the converter collects effect ids from spawn ops and from every motion table's effect commands through one helper, so a table-spawned effect gets its row and its read-ahead pages. The re-bake added 34 effect rows across the game. Ids whose kind is a native class still need code.

### 35. One chapter card for every chapter

The whole-game set drew the Chapter 2 card at the start of Chapter 1 and no card at all from Chapter 3 on. Effect ids 66 to 83 all map through the kind table to one native class, `uChapterStart`, one id per chapter, and the remaster has a card texture for each (`cmn_ch_00_00` to `cmn_ch_00_17`). The set listed ids 66 and 67 by hand with one texture, copied from the Chapter 2 set, and the bridge drew the set's single card texture for any card effect.

Guard: the set names the first chapter-start id and the card source pattern; the converter finds every spawned id whose kind uses the same class factory (`data_020ac194`) and bakes a card row and texture for it, numbered by kind. Each card row names its own texture, the bridge draws that texture, and the read-ahead plans schedule card art at the card's effect cue. The set-level card line is gone.

### 36. A pill hold the port kept for one pill only

Ops 0xe7 and 0xe8 logged as not ported in Chapter 2. Their native case clears or sets bit 11 of `game+8` (`func_020100a0`), and `func_020100bc` reads that bit, or the current core's busy bit, when `func_0204f850` decides whether the Trick pill stays out. The port had the busy-core half and the matching Ghost pill hold (ops 0xe5 and 0xe6, `game+0xc` bit 0), but not this bit.

Guard: 0xe7 and 0xe8 clear and set a Trick pill hold beside the Ghost pill hold, reset with the pills. Before porting one of a pair of HUD ops, read the pill rule in `func_0204f850` for every pill, since each pill's wanted state combines several native bits.

### 37. An op whose length depends on its object

Op 0xaa logged as not ported on the stage 6 dog. The port then advanced it by its table length, 3 words, which also skipped the op 0xa9 that follows it in the script. Native 0xaa (case at `0x0202543e`) checks that the current object is a `uScript2dR`; if so it calls `func_02008abc` on the motion controller (count at `+0x127`, counted-loop bit 3 set and done bit 4 cleared at `+0x126`), advances 2 words and runs on; otherwise it advances 3 and yields. The stepper `func_02007c54` decrements the count each time a frame ends just before the loop word and, at zero, sets the done bit instead of looping; 0xa9's end test reads that bit. So the script's 0xaa then 0xa9 lets the current loop finish before the next motion.

Guard: 0xaa sets a counted loop on the actor and moves the pc itself (2 words with an actor, 3 and a yield without), the stepper stops at the loop word when the count runs out, the end test reads the done bit, and a new motion clears both. Before routing an op, check its native case for a pc step that differs from the length table.

### 38. Script changes waiting on plain-text bundles

After the parsing fixes (28), run 21's script changes still stalled 170 to 325 ms, and the bundle read was most of it: 175 to 210 ms for 0.7 to 1 MB of plain text, read on the main thread while textures streamed. The bundles are frame lines that repeat almost word for word, so they compress about 12.8 to 1 with deflate (994 KB to 81 KB for the largest).

Guard: the read-ahead step writes bundles as `GPK1`, the unpacked size and raw deflate; the player's file reader unpacks any file carrying that header (`packed_file.c`, a table-driven inflate written for this port) and still reads plain files as before. The largest bundle unpacks in under 1 ms on the Mac. Any other large text table read on the main thread can use the same header without a loader change.

Run 22 measured it on the Vita: unpacking took 8 to 34 ms, and the slowest script changes fell from 257, 315 and 325 ms to 136, 248 and 210 ms. What remains of each read is per-file latency behind the texture reads (a 10-byte empty bundle took 18 to 84 ms), so the player now skips the bundle for any script with no motion rows instead of opening an empty file.

### 39. A scene the route could not leave

Past the routed chapters, run 23 sat for hours on the Chapter 3 junkyard with two detectives talking. The junkyard leaves only through `phone.xml`, which picks the next place from the phone book (register 0x1a); with no route step picking one, the place stayed 15 from Chapter 2 and the phone sent the game back through stage 6 and route.xml to the same junkyard. The stuck detector reset with every reload of the same script, so the loop never counted as stuck, and its fallback exit search walked ops in a line and stopped at the inline data after a jump (op 0x22) in 53 of the 163 baked scripts.

Guard: a scene timeout in the shared machine. `scene_most_ticks` in the route (51600 ticks: the longest stretch between screen transitions in the footage, 684.5 s, times 1.25; measured with a dark-screen split that put all 12 Chapter 1-2 route scripts in one stretch each) counts ticks since the game last began a script it had not begun before, so a loop through seen scripts keeps counting. When it runs out under auto-play, the game moves to the next scene in `scenes.txt`: every script's first visit in the read-ahead replay (cut scenes included), then every unvisited script from the replay's last chapter on, ordered by the chapter mark each script sets with op 0x74, a chapter's ending scripts last. If a loop has carried the game behind the furthest scene reached, it resumes after that scene, because returning to it alone let a scene whose own exit leads backwards (st03_game000 back to st02_game000) loop forever. Only a scene outside the list takes its own exit. With this the replay crosses all 18 chapters to the game's return to the title, routed or not. The exit search now finds any `0x24` whose operand names an `.xml` script in the string table, so data between ops cannot hide it, and a finished machine keeps counting ticks so the timeout still fires after `stuck_with_nowhere_left_to_go`. Every forced move logs `scene_timeout_left` and `scene_timeout_moved_to`.

### 40. What a scene change has to leave behind

Native's teardown on every script change (`func_0200fe74`, run from the `0x24` chain through `pGameRestartDirect`) kills scripts, UI clips (`uSwfPlayGame`), custom effects, past-death sprites, soul background gimmicks, spot and point lights, phone-book objects, mask fades and soul tentacles, and kills the countdown (`uCountdown`) unless flag 0x1008 is set, clearing that flag every time (`func_02027518` reads it, `func_02027538` clears it). It leaves music alone. The port kept playing UI clips across a change and always dropped the countdown.

Guard: the stage clear stops every playing UI clip, the machine keeps the countdown across a change only while flag 0x1008 is set and clears the flag at each teardown, and each new script checks that no talk, narration or press is still pending from the earlier one (`scene_leftover_cleared` names what it cleared). Music follows native and carries over, except on a forced move (the scene timeout or the stuck detector's own exit, both through one `force_script_change`), which stops it so the earlier scene's music cannot play over a scene native never reached that way. The movie and its waiting sound are left alone, because the phone ride's movie plays across the change.

### 41. A run log with no ceiling

Run 23's log reached 2.5 GB in five hours of the junkyard loop, almost all of it per-frame animation lines, and the Vita's FTP server cannot seek past 2 GB, so the tools could not read it at all.

Guard: the Vita log sink moves `run.log` aside to `run.log.earlier` when it passes 128 MB and starts a fresh file with `log continued_after_bytes=`, so the newest part always stays small enough to pull; if the move fails it deletes the full file instead, and a launch removes a stale earlier part. The route tables follow the same rule from the other side: route steps (4096) and scenes (512) clear themselves on a line they cannot hold instead of keeping half a route, and the bake refuses a field wider than the player reads.

### 42. Fixed caps below what the scripts declare

The Chapter 3 junkyard (st01_game020) logged `too_many_objects`: it declares 69 objects and the port held 64, so the last objects never existed. Across the game, 19 of 176 scripts declare more than 64 objects (105 at most, st03_game011), and one act declares 88 handlers (st02_game036) against a cap of 64. Native creates objects and handlers as heap tasks with no fixed count.

Guard: `SCRIPT_OBJECT_MAX` and the view's `SCENE_OBJECT_MAX` are 128 and `SCRIPT_HANDLER_MAX` is 96, above the largest counts in the decoded dumps (objects per script, handlers per act, since starting an act clears and re-registers its object's handlers). The machine grows to 1.4 MB on the heap. Before raising a limit again, count the worst case across every script, not the chapter at hand.

### 43. The talk with the dead that no input started

22 scripts from Chapter 2 on hold a `SYSTEM|DEADTALK` handler that plays a talk and ends with `GameApp DeadTalk|Finish`. Native starts it from the soul's dead-core state machine (`func_02032914`): `func_02033408` either starts it at once (when scripts handle both `SYSTEM|SV_DEADCORE` and `SVTUTO|onClick`, checked with `func_02012518`) or opens a yes/no prompt that starts it on confirm, pushing `pGameMainDeadTalk` and state 0x15. The scene sends `SYSTEM|DEADTALK` to the dead core's object, or to every object when it has none (`func_0203bfc8`; `CORETALK` instead when its mode byte at +0x49 is 1), and `cGameAppCmdDeadTalk` (`func_0203c03c`) takes `Finish` (back to play), `Abort` (`CustomSoulVision|NoEffectFinish`, state 2) and `Restart`. The port had none of it, so these talks only ran when the stuck detector pushed them, and `DeadTalk|Finish` logged `gameapp_not_ported`.

Guard: a `dead_talk` route action starts the talk with the native targeting (the step's object, or every object), play holds while it runs, and the `DeadTalk` command handles Finish, Abort and Restart. Still open: the player's own way in (the prompt and its confirm), and `CORETALK`.

### 44. Two Missiles, two Lynnes: classes the port drew alike

Run 27 showed two Lynnes, the class behind the two Missiles of Chapter 2. Native draws an object's sprite by its class (slot 7 of each vtable, `opcodes_objects.md` and `object_visibility.md`): humans (`uHumCommon` and `uHuman`) and script objects (`uScrObj`) only in normal view, soul-vision gimmicks only in ghost view, and past-dead bodies (`uHumPastDead`, `func_02053dd0`) never; a past-dead object instead hands its first visible frame to a separate `uPastDeadSprite` (`func_02053dd4`). In ghost view every human is an orange silhouette (`uSoulBGHuman`, built per human at entry). The port knew only `uHumCommon` as human and gated two classes, so the 13 past-dead objects in the game (`carcass_silhouette`, `linne_dead`, `linnedead`, `linne_s2`, `caron_dead` and others) drew as a second live character, and the 41 `uHuman` objects (`linne`, `girl`, `dog`, `madam`, `killerB` and others) kept their colours or a core's tint in ghost view and sat on the gimmick draw level.

Guard: `script_classes.c` holds every class rule in one place, keyed by the class ids read from the native class registry (id at +0x18 of each class info): which classes are human, which views each draws in, the soul style a class takes, and its default level (humans 6, dead gimmicks 4 from `func_02053010`, other gimmicks 9). The bridge and the motion code ask it instead of comparing ids. A new class goes in that file, never in a caller. The `uPastDeadSprite` is ported in root cause 86. Native (class info `0x020ca128`, methods at `0x020a4e24`, update `func_020294f0`, draw `func_020570cc`) watches its source body object: while the source is hidden its level is 0; when the source becomes visible it fades in from 0 to 31 over 60 ticks, then can hold, fall and rise on timers, and it draws at that level. So native shows a body as a fading copy only while the body object is visible. The doubles were most likely body objects visible when native keeps them hidden; the hold-rule divergences in `object_visibility.md` (ops 0x96 to 0x99 showing targets native leaves hidden) are the first suspect. Porting both would bring back the bodies native does show, such as `caron_dead`, which is visible for long stretches in the late chapters.

### 45. Hold ops that showed what native keeps hidden

Ops 0x96 to 0x99 freeze, unfreeze, hard-hide and lift a hard hide on every object a name matches, often `*`. Native guards each one (`func_02059a3c`, `func_02059a80`, `func_02059ad0`, `func_02059b30`): 0x96 acts only on an object neither frozen nor hard-hidden, 0x97 only on a frozen one and 0x99 only on a hard-hidden one, and a hard-hidden object's table sounds are muted (`func_02059b18`). The port applied all four to every match, so a wildcard 0x97 or 0x99 showed objects that were hidden some other way, such as body objects; Missile's carcass and `caron_dead` were visible about twice as often as native allows.

Guard: `change_hold` in `script_motion.c` applies each op's native guard, and a hard-hidden object plays no table sounds. One native rule is kept out on evidence: native leaves the frozen bit alone when a new table starts (`func_0205964c`), but keeping it froze objects the Chapter 2 scripts wait on and sent the replay down another branch at st14_game000 (113 route steps instead of 235), so a table start still clears it until the reason is traced.

### 46. Choice panels the port never offered

Chapter 4's lab, and 40 scripts in all, offer a set of panels the player picks from (ops 0x3a to 0x43). The port had none of them, so a scene that waited on a choice sat until the stuck detector or the scene timeout moved it, and the Chapter 4 route could not be written. Native keeps the panels on the game object: a list of up to four at `game+0xf24` (count at +0xf20, limit at +0xf21, both reset by 0x3e, `func_02011d1c`), and a second list of turned panels at `game+0xfa4`. 0x3c and 0x40 add a panel (`func_02011da4`, `func_02011f34`): a talk panel holds a caption label, the talk to open and a flag; an event panel holds a caption label, a flag and the target, kind and event it sends. 0x3d and 0x41 put a panel at an index (`func_02011e48`, `func_02011fac`): into the turned list when that slot is empty, otherwise over the panel there, otherwise at the end. 0x3f clears both lists (`func_02011d74`). 0x3a creates the select control (`uSelectCtrl`, `func_02015b70`), 0x42 and 0x43 add its cancel panel and extra art, and 0x3b opens it with input locked. The control (`func_0205a9e8`) stores the picked index at `game+0xf22` (`func_0205ad20`), sets the panel's flag (`func_02029910`), opens its talk like op 0x37 (`func_0202991c`) or sends its event (`func_02029930`), and deletes itself; cancel sends `*` `select|cancel`, which 46 scripts handle. Op 0x39 waits while the control exists, as well as for talk.

Guard: `script_choice.c` holds the lists and the pick with the native rules, the machine clears them at every scene change, and play is held while a choice is open. A `choose` route step (wait `choice`) picks a panel by its event, talk label or caption label, or `cancel`; with no such step, auto-play picks the first panel whose flag is not set after a second and logs `choice_picked_without_route`, and a press does the same in manual play. Still open (the panels are drawn since root cause 218): the turned panel's flip (`game+0x120` of the control) is read as the turned panel showing at its index.

### 47. Auto-play that went back to Chapter 1

Run 28 played on past Chapter 12 and then started Chapter 1 again, twice. Stage roots pick their game script from flags (`st05_root` picks `st05_game000` to `st05_game003`), and the scene timeout carries the game on without setting the flags the story would have set, so `st05_root` picked Chapter 3's `st05_game000`. The route then found that script's steps by name and played Chapter 3's block again, whose last step is the phone ride to the junkyard (step 229, place 1), and the junkyard root started Chapter 1.

Guard: two rules in the shared path. Under auto-play, a script that was already visited and whose chapter mark (its op 0x74, the first words of every chapter's scripts) is below the highest mark reached is behind the story: the machine moves on to the scene after the furthest one reached in story order and logs `scene_behind_the_story_left` and `scene_behind_the_story_moved_to`. A rewind inside a chapter keeps its mark and stays, and manual play may go anywhere. And the route never plays a block it has passed: a script's steps are looked up from the block of the furthest step acted on, so a revisit to an earlier block logs `route_block_already_played` and plays nothing. A revisit to the script of the furthest step acted on (a phone ride that returns to the same scene) resumes at the step after it, or plays nothing once that script's block is done; it no longer starts the block again, which had replayed Chapter 5's st02_game100 steps and their phone ride forever. Chapters 1 to 3 replay unchanged (235 steps done), and the whole-game replay no longer returns to `st01_game000`.

### 48. Ghost-view commands the port dropped

41 scripts send `GameApp CustomSoulVision` commands the port ignored: `DemoModeOn` (343 sends), `DemoModeOff` (88), `CloseStatus` (102), `HoseiOff` (32), `ReturnGame` (25) and a few rarer ones. Native `func_02033504` handles them all, and every one but `Start` and `NoEffectStart` only while the soul-vision task exists. `DemoModeOn` takes the soul's input away (clears bit 0x20 of the soul at +0x7c, `func_02061138`), hides every pill (`func_02010ef4`) and sets the task's demo byte (+0x35). `DemoModeOff` gives input back, clears the demo byte and the camera-correction byte (+0x32), and shows pills 1 and 2, the hourglass and the back pill, even in a view the script opened; `DemoModeOffButtonOff` does the same with those pills hidden. `HoseiOff` sets +0x32. `ReturnGame` and `ReturnDemo` leave the view into play (mode 3) or demo (mode 2) through `func_0201192c`. `CloseStatus` and `OpenStatus` gate the status panel. Chapter 6's floor lifts in st02_game024 hold the view in demo mode and give it back with `DemoModeOff`.

Guard: `run_custom_soul_command` follows the native handler: the commands act only while the view is on or leaving; demo mode stops the soul's movement, the route's `view` wait and the back pill; `DemoModeOff` offers the back pill in a script-opened view; `HoseiOff` stops the camera following the core until `DemoModeOff` or the next view; and `ReturnGame` leaves into play. The demo, follow and back flags reset when a view opens and when it finishes closing, as native builds the task on entry and deletes it on leave; until audit 30 they survived a leave, so a `HoseiOff` before `ReturnDemo` kept the camera from following the core in normal play. A view opened without its effect (`NoEffectStart`) leaves the world running too: only an entry with the effect freezes time, so a script that re-opens the view quietly (Chapter 4's motorAct010 in st02_game004) can run its own `SV_START` handler, which sat paused for about 60,000 ticks until the stuck detector pushed it. Demo mode also lets the world run inside the view: `DemoModeOn` clears the pause bit the view holds and `DemoModeOff` sets it back, because the scripts run timed sequences there (Chapter 4's floor in st02_game004 fades and moves under `DemoModeOn`, and its handler froze at the fade wait while the view kept the world paused). Still open: the status panel (`CloseStatus`, `OpenStatus`) and `CancelCameraHosei` (`func_020337f8`).

### 49. A rewind the lab never armed

Chapter 4's lab ends its talk with `GameApp Timeleap RESTART` and no `SELECT`; the port knew only `SELECT` and `BREAK`, so the rewind never armed and the lab looped through `phone.xml`. Native `func_0203e538` creates the time-leap task for `RESTART` and for `SELECT` when none exists (`RESTART FINAL` builds it in its final mode), and with a task, `SELECT` opens the choice (`func_0203e4a4`) and `BREAK` breaks it (`func_0203e508`).

Guard: `RESTART` and `RESTART FINAL` arm the rewind as `SELECT` does when no time leap is up. The lab now clears: its three panel picks, then the rewind, then st02_demo003 and st02_game004.

### 50. Stuck pushes while a native timer was still running

On the device the memo's `core|ready` handler in st01_game014 was pushed 242 ticks after the phone ride began, every run, though the replay never pushed it. The stuck detector pushes a waiting handler after a quiet stretch unless something is "running to a known end", and that list held narration, fades, pans, talk, load steps and object timers but not the port's own native timers: a movie, an eyecatch, a ghost-view change, and the scheduled phone-ride and time-jump events. The device's phone effect waits on its art, so its event comes later than the replay's and the detector took the wait for a stall.

Guard: `something_is_running_to_a_known_end` counts those five timers too, so a push only happens when the game is idle. Whole-game replay unchanged (235 steps done).

### 51. A failed rescue that never offered the retry

Op 0xdb (99 sites in 28 scripts) is the game over: native starts a `uKaiwaGameOver` talk (`uKaiwaGameOverDirect` when its third word is set) through `func_020267d8` and `func_02014b98`, puts the game in mode 0xe and turns the hourglass off (`func_02011cdc`), and the talk leads to the rollback scene, whose retry sends `*` `SYSTEM|RESTART` when a script handles it (13 do) and otherwise reloads the scene. The port opened the talk and stopped there, so a failed rescue (Chapter 4's guard catching the soul in st02_game004) sat until the stuck detector or the scene timeout moved on, and the route had no retry to follow.

Guard: after an 0xdb talk closes, auto-play retries after a second and a press retries in manual play (`game_over_retry restart` or `reload`). A reload starts the scene again and the route resumes at the step it was on, so the second attempt plays the steps after the failure instead of the whole block again. The resume step and the route's furthest acted step live on the machine, not the soul: every script change resets the soul (`tear_down_script` calls `script_soul_reset`), which at first wiped both, so neither the resume nor root cause 47's block rule ever acted until audit 30 caught it. Still open: the rollback menu itself (its choices and art), so the retry is always the native default. Root cause 223 sends the hourglass pill through the same retry.

### 52. An eyecatch that left the game in demo mode, and ghost views the player could not leave

Native eyecatches (`func_02013a88`) switch the game mode with `func_0201192c` to an eyecatch mode (9 to 0xd), which keeps the mode it replaced at +0xb; ReturnGame (3), ReturnDemo (2) and the game over (0xe) go through the same switch. The port set demo mode for the eyecatch and only a later op 0x68 or 0xe3 gave play back, so where a script set normal play before the eyecatch (Chapter 7's cock balloon in st04_game000) the game stayed in demo mode for good. Separately, the port let the player leave only a ghost view the player had opened: script-opened views (Chapters 6 and 7 open them after a talk and wait for `SYSTEM|SV_END`) could never be left, so those scenes waited for the stuck detector.

Guard: an eyecatch remembers the mode it replaced and puts it back when it ends if nothing else changed the mode meanwhile; the back pill and the ghost button leave any ghost view that is not in demo mode and not in a tutorial. Chapters 1 to 4 replay unchanged (253 steps done).

### 53. A death scene that looped under auto-play

Run 30 replayed Chapter 4's office (st02_game004) and later st02_game022 again and again: with no route steps there, every attempt ran the scene's own timeline into its game over, the retry of root cause 51 reloaded the scene, and the scene timeout, which counts ticks since the last new scene, took minutes of reloads to fire on the device.

Guard: under auto-play a game over moves on in story order, like the scene timeout (`game_over_left`, `game_over_moved_to`; one shared `script_machine_move_on_in_story_order`), when the route has no step waiting in the scene or after three retries at the same step; otherwise it retries as native does. A routed failure, such as the footage's first attempt in the office, still retries once its next steps are written.

### 54. Rooms and objects that never dimmed

Op 0xf7 (41 sites from Chapter 3 on) scales palette colours. Native (case at `0x02026176`): for `BG` it stores the value at `game+0xdd4` and runs every one of the room's 16 palette blocks through `func_0200d46c`, which multiplies each 5-bit channel by value/256 and clamps at full (`func_0200f1a8`, `func_0200a2f8`); for other names two visitors (`func_02026b40`, `func_02026d34`) apply it to the objects and their attached sprites that match: `*` every object, `@x` names containing x, `?` the object running the script (`func_02028148`), otherwise the exact name. Values run from 0 (black) through 128 to 240 (fades in steps of 16) to 256 (normal) and 400 (brighter). The port ignored it, so rooms stayed lit where the footage dims them.

Guard: the machine keeps a level for the background and for each actor, the bridge hands both to the stage, and the view multiplies the drawn colour by level/256 in normal view (ghost view keeps its own silhouette colours). Still open: levels above 256 draw at full brightness, because the sprite path only multiplies; native brightens and clamps.

### 55. Black squares on characters in the grey world

Rooms and characters drawn in the grey world (the `-mono` copies of textures, made on the Vita by graying each DXT block's two endpoint colours) showed scattered black 4x4 squares. A DXT5 colour block is meant to be decoded as four colours whatever its endpoint order, but the Vita decodes a block whose first endpoint is not the larger one with the DXT1 rule, where index 3 is black. Graying can reverse or equalise the endpoints (pure red and pure green gray to 6 and 22, so red-over-green blocks flip), so every block that flipped and used index 3 turned black. The DXT1 path already kept its order; the DXT5 path did not, and its unit test had pinned the flipped order.

Guard: graying a DXT5 colour block keeps the first endpoint larger, swapping the endpoints and flipping each index's low bit when they cross, and points every index at the first colour when both ends gray to the same value. The baked colour textures were checked: none of the 3601 DXT5 textures has a block that uses index 3 with its endpoints in DXT1 order, so the fault was only in the runtime grey copies.

### 56. An op that had a size but no family

Op 0x6b (two sites, Chapter 7's st04_game001 `click linne_balloon1` among them) sets the condition to whether a scene change is pending: native returns `game+0xe58 != 0` (`func_0202657c`), the word op 0x24 sets to 0x19 through `func_02010350` and a player mode request sets to 0x18 through `func_020110b8`. The port had the op's length in `script_opcodes.c` but no family, so `run_op` logged `op_not_ported`, left the condition as it was, and the balloon's handler took whichever branch the last test had chosen.

Guard: op 0x6b is a control op whose condition is the port's pending script change (teardown or load scheduled); `tests/unit/test_script_change_pending.c` runs it with and without an op 0x24 first. When porting an op, add it to the family table as well as the switch, or the case is never reached.

### 57. Talks a choice panel opens that were never baked

A picked panel from op 0x3c (talk label in word 2) or op 0x3d (word 3) opens that talk (`script_choice_pick`), but the converter collected talk labels only from operand 0 of ops 0x37, 0x5c and 0xdb, so those talks had no row in `talk.txt` and the replay logged `no_talk_row`. Eleven such talks were missing across the game set.

Guard: `Script.talk_labels_of_choice_panels` adds the panel talks to the baked talk labels; `tests/tools/test_choice_talk_labels.py` covers both panel ops and an event panel that names no talk. Any op that opens a talk by label must feed the same list.

### 58. Demo mode's end paused a view that was opened quietly

Root causes 52 and the quiet-view rule (only a ghost view entered with its effect pauses the world, `func_02033504`) left one path behind: `DemoModeOff` set the pause whenever any view was open. Scripts that open a view with `NoEffectStart` and then run `DemoModeOn` / `DemoModeOff` (Chapter 8's st03_game033 `func_init_sv_core`, and others) therefore froze the world after demo mode, so core-ready handlers never ran and no trick became playable.

Guard: the soul remembers whether the open view was entered with its effect, and `DemoModeOff` pauses only then. `tests/unit/test_quiet_view_pause.c` covers both the quiet and the effect view. The whole-game replay plays 401 route steps (393 before), Chapters 1 to 4 unchanged.

### 59. Scripts that put the soul on a core were skipped

Ops 0x9c, 0xa4 and 0xa7 all go through `func_020101c0` / `func_020101d0`, which stores a core number and a mode (0, 1, 2) on the current object (+0xbf, +0xde bits 6-7). 0xa4 then moves an existing soul onto that core through `func_020611b4` (immediately, unless the soul is in state 5, when the move waits), and 0xa7 through `func_0206115c` (immediately). The port handled 0x9c only, so 0xa4 (244 sites) and 0xa7 (13) were logged as unported and the soul stayed where it was, which left later chapters' scenes (15, 17, 18) waiting on a soul that never arrived.

Guard: 0xa4 and 0xa7 share 0x9c's placement in `script_motion.c` and the op family table; `tests/unit/test_soul_placed_on_core.c` runs all three and checks their family. Still open: 0xa4's delayed move in soul state 5, and 0xa6 (`func_020610ac`, the soul travelling to a core), which stays unported.

### 60. Talks named from another stage were never baked

Scripts often open talks numbered for another stage: st04_game101 opens `m02_12xx`, st13_demo001 `m04_11xx`, st14_game020 `m03_8112`, st15_demo070 `m14_0280`, and st06_game004 the `tel_info` of st06_game003. The converter looked a label up only in its own script's message files, so these 13 talks and balloons had no row and the scene skipped them (`no_talk_row`, `no_balloon_row`).

Guard: `entries_for` in `chapter_assets.py` looks in the script's own files first and then in every message file of the set. The bake prints `talk and balloon labels with no text` with the count, which is 49 now: labels the remaster's text does not have at all (Chapter 2's `st06_m00xx`, Chapter 5's `m02_2480`, and talks in Chapters 5, 9, 13, 14 and 16). Their text is only in the DS ROM's language file for the script.

### 61. Docks onto a core were refused

Native reads an actor's node through `func_0205958c`: nodes 0 to 3 are the four points (12-byte entries from node +0xe4, written by motion word 0xfff5 through `func_02059204`), nodes from 10 are its cores (node minus 10, while that core exists; motion word 0xfff0 places them through `func_02059170`), and anything else reads as zero. Docks 0x85 and 0x86 (`func_020593f8`) store any node. The port refused docks past point 3 as unported, so Chapter 2's susi (own node 10 onto the file), Chapter 14's start_point, Chapter 16's linne_dead_core and Chapter 17's joedo_d_core never moved. A first fix that grew the point table to 16 read node 10 as an empty point, and on the Vita (run 37) it threw Chapter 2's obj core 256 pixels away from the file, so step 95's hop failed.

Guard: docks keep any node and place through `script_machine_node_position`, which follows `func_0205958c` (points 0 to 3, cores at 10 and 11 when the actor has them, zero otherwise); the table holds four points as native does. `tests/unit/test_dock_onto_core.c` reads each kind of node and docks onto a core.

### 62. Scripts that run in the ghost view were frozen with the world

Native decides whether a script runs each frame in `func_02027db8`, the VM's is-paused check (vtable +0x18), which the VM step `func_02023fdc` asks before it counts down a wait or runs an op. It reads the pause class ops 0x1e, 0x1f, 0x20 and 0x21 store (2, 0, 3, 1). Class 0 pauses whenever the world is halted (game+8 bit 1, set by every ghost-view start) or the game is not in normal play. Class 1 pauses while a talk shows and otherwise runs in pGameMainNormal, pGameMainSV (the ghost view), pGameMainExchange and pGameMainReturnNormal. Class 2 pauses only for a talk, and class 3 never. The port treated class 1 like class 0, so every handler meant to react inside the ghost view sat frozen until the player left it: Chapter 7's cock_talk, Chapter 14's denwa_act10 and Chapter 15's sys_act100 deadlocked without the stall breaker, and the telephones and Chapter 6's floor lift (which poll op 0xa0 for the soul's core) only reacted after a ghost-off the original never needs.

Guard: `script_machine_vm_is_paused` runs class 1 while the view is open or leaving, and pauses it for a talk, a dead talk (pGameMainDeadTalk) or a choice panel. `tests/unit/test_pause_classes.c` checks every class. The route drops the three ghost-offs the scripts now do themselves (Chapter 2's telephone, Chapter 5's telephone, Chapter 6's caster); Chapter 6 plays all 42 steps without the breaker. Chapter 4's office and Chapter 7's dead talk were routed around the old freeze and need new routes.

### 63. The other ghosts and Jowd had no controls

Native keeps the active ghost in game+0xc bits 13-14 (`func_0201132c` reads 1 as Missile; `func_0201424c` sets Missile, `func_02014260` Sissel, `func_02014270` the boss). The `GameApp Exception` commands (`func_02013b18`) choose it: `Missile:On`, then `Missile:Off` or `Cicel:On` for Sissel, and `Boss:On`. The `GameApp Button` commands (`func_020135c8`) show HUD buttons by slot: Jawed 5, Missile 6, Cicel 7. In the ghost view the switch (`func_02035078`) toggles Sissel and Missile when both buttons are registered, and the HUD board (`func_0204f850`) sends `* JAWED CALL` when Jowd's button is pressed. Every core change also sends `SV_CHGCORE_MISSILE` or `SV_CHGCORE_CICEL` after `SV_CHGCORE` (`func_0206001c`). The port knew none of these, so the prison escape (st03_game012, 42 `JAWED CALL` handlers) could not call Jowd, and Missile's scenes in Chapters 14, 15, 17 and 18 had no switch.

Guard: the soul keeps the active ghost and each ghost's last core; Jowd's pill and the Missile and Sissel switch pills sit at the bottom centre with the remaster's icons and TRIANGLE (when no balloon takes it), and the commands, the switch and the call follow the native functions above. `tests/unit/test_character_buttons.c` covers Jowd's call, the switch both ways, the exceptions and the per-ghost cores. Missile's ghost view is green and the boss's dark blue, with scanlines and a sweeping line over both (measured from the footage: Missile dark #000d00 to bright #099c04, the boss #000317 to #136ae3, Sissel #0a0000 to #9b0e01). The converter bakes the green and blue room plates only for rooms of scripts that name `Missile:On` or `Boss:On`, the rooms file carries them after the red plate, and the stage picks the plate by the active ghost the HUD view reports (`tests/tools/test_ghost_plates.py`). Still open: Missile's exchange (pGameMainExchange, op 0xa5), the per-ghost object tints, and the route through Jowd's prison escape (st03_game012, flag 0x10a2 set by its own handler at 0xc74).

### 64. A closed balloon left its parts on screen

Native keeps each balloon in a slot (the eight words at `game+0xe1c`; `func_020109c8` closes them), and closing a slot ends the balloon with every part it has. The port draws a balloon's oval and tail as overlays that appear on scheduled ticks, and a close scheduled its hides beside the shows still waiting. A balloon closed before its last part appeared, which fast forward does within a few ticks, left that part on the screen: the stale bubbles over black screens and later scenes that the watchers kept reporting.

Guard: scheduling an action for an overlay drops any action still pending for the same overlay, so the latest open or close wins (`forget_pending_overlay` in `script_bridge.c`). `tests/unit/test_balloon_close_race.c` closes a balloon before its parts appear, shows and hides one, and reopens one in its slot.

### 65. The frame register read the table position

Native register 2 of an actor is its frame number: `func_0200853c` writes the frame of each frame record to owner+0x50. The port returned the index of the frame word in the motion table, so a script that waits for an actor to reach a frame (Chapter 2's loops among them) compared against a number the index never matched.

Guard: frame records and word 0xfffb set the actor's `frame_number`, and register 2 reads it. `tests/unit/test_frame_number_register.c` loops frames 117 to 125 and reads the register on every loop.

### 66. Motion tables never sent their messages

Motion word 0xfff9 sends a message: `func_02007cd4` passes its three strings (target, verb, argument) to `func_0200da04`. The port skipped the word, so a handler waiting for a message from an animation never ran.

Guard: `send_table_message` in `script_motion.c` sends it through `script_machine_send`. `tests/unit/test_motion_message.c` plays a table whose record sends a message to another object.

### 67. Missile's and the boss's views drew Sissel's red

Native gives each ghost its own art for the soul line and the silhouettes: `func_02059cac` and `func_02052a7c` choose one of three classes by the active ghost (`func_0201132c` for Missile, `func_020328e4` for the boss). The port drew Sissel's red soul beam and red object tints in every ghost's view.

Guard: the converter bakes a hue-turned copy of every screen texture marked `ghost_variants` (Missile turned 120 degrees, the boss 240; the beam is not one, root cause 179) and flags every soul-view class whose tint is soul red (red at least twice green and blue); the view turns flagged tints to green for Missile and blue for the boss. `tests/tools/test_ghost_plates.py` checks the flag rule over every class in game.json and the per-ghost jobs, `tests/tools/test_texture_encoder.py` the hue turn, and `tests/unit/test_soul_ghost_turn.c` the table flag.

### 68. Every ghost reached as far as Missile

Native's reach is `func_020615e8`: Missile 112 DS pixels, the boss 48, and Sissel the soul's own value at +0xa4, which only the soul's constructor `func_0205f18c` writes (0x30000, 48 pixels); each becomes 168 while its `MissileHandLong`, `BossHandLong` or `CicelHandLong` exception is on (`func_02013b18` sets game+0xc bit 30 and game+0x10 bits 19 and 17). `func_0206167c` holds the tip within that reach of the core, and `func_0205fa2c` picks a core near the tip. The port held every ghost's tip at 112, so Sissel's drags reached more than twice as far as the game allows and picked cores the scene never offers.

Guard: `script_soul_reach` follows `func_020615e8` and the exceptions set each ghost's long hand; the chain and the route's hop planner use it. `tests/unit/test_soul_touch_rules.c` checks each ghost's reach and the long hand. The replay with the breaker plays 397 steps (440 with the old reach); the 31 hops beyond reach are the next routing work: a player reaches those cores through others, and the route will plan them once it plays by simulated touch.

### 69. One drag hopped twice and the tutorial never heard the touch

Native drops the drag once a pick connects (`func_0205fa2c` clears its drag flag when the chain settles) and picks the drag up again only while the stylus is within 16 pixels of the chain's end. The port kept dragging from wherever the stylus was, so one long drag picked a core and then the next one past it. The touch tutorial (`func_0203de90`, pGameMainSVTutorial) sends `onClick` on the first tick the chain is out or the d-pad is held, `onTouch` on every tick after that, and `onRelease` when it lets go; MoveStart sends `move` on every held tick, and the script's handler checks each time whether the soul now sits in the core it asked for (op 0xa0, `func_02027630`). The port sent `onClick` only for a press, ignored a touch while waiting for it, and sent `onTouch` and `move` once on arrival, so a player's drag through Chapter 1's first tutorial core went on to another and the tutorial never let go of the ghost view.

Guard: a held touch grabs the tip only within 16 pixels of it and an arrival ends the drag; the tutorial events follow `func_0203de90` every tick. The route waits on TouchCheck and drags, as a player does, in both touch tutorials (Chapter 1's st01_game000 and Chapter 6's st06_game002). `tests/unit/test_soul_touch_rules.c` drags past every core, drags on after a new touch, touches away from the tip, and checks each tutorial event. Without the end-of-drag rule the long drag reaches the third core.

### 70. A player could not choose a panel

Native's choice control (`func_0205a9e8`, state 9) keeps a marker on one panel: it starts on the first panel (`func_0205ad48`, unless a newly added panel is flagged), d-pad down moves it to the next and wraps to the first (`func_0205b0b8`), up moves it back and wraps to the last (`func_0205b110`), A confirms the marked panel, a tap on a panel picks it, and B cancels when the choice offers it, sending `select cancel` (state 12). The port had no marker: CROSS always took the first panel not yet chosen and the d-pad and CIRCLE did nothing, so a player could never pick a later panel first, and dead talks whose answer is not the first unseen panel could not be finished by hand. Only the route's `choose` step, which picks by name, got through.

Guard: the choice keeps a cursor that starts on the first panel; d-pad up and down move it with native's wrap, CROSS confirms it, and CIRCLE cancels when offered. `tests/unit/test_choice_input.c` picks the first panel, moves down past the last, moves up from the first, and cancels with and without the offer. Still open: a tap on a panel is not tested (root cause 218 draws the panels).

### 71. Auto-play played through shortcuts no player has

The route ran inside the machine and acted through internal flags. A press or a ghost toggle answered even when its pill was not on screen. The soul's tip followed an internal aim vector. A failed move teleported the soul to its target, and choices were picked by name. Touch input was dropped during auto-play, and fast forward shortened game timing: instant drags, skipped view transitions, no dwell. A player softlocked in Chapter 1's first tutorial where the route had passed.

Guard: the route only decides which step is current and when it is done. `script_route_pad` builds a pad frame each tick from what a player sees, and it goes through `script_input`, the same code as the Vita pad, with the game in player mode. The pad looks for the pills, balloons, pages waiting for a press, and the choice marker. A move pushes the analog stick toward the next core on a chain within the native reach, and nothing teleports. `phone_call` and `dead_talk` still act inside the game and log `route_no_player_input` until their player inputs are ported. The Mac replay and the Vita run the same pad each tick; fast forward only runs more ticks per frame. Honest counts with no stall breaker: Chapter 1 plays steps 0 to 20 and stops at 21. With the breaker the whole replay reaches 66 steps, against 397 with the shortcuts. `tests/unit/test_route_pad.c` covers a press held back until its pill is up and a move made with the stick.

### 72. The ghost view hid its Back pill

Native's Trick button (HUD slot 0, `Button Trick:On` in `func_020135c8`) is also the ghost view's way back: the footage shows a Back pill in Chapter 1's ghost view (frame 659.7 s, the step where the soul has reached the bed), although st01_game001 never sends `BackToMain:On` (slot 2). The port showed Back only after `BackToMain:On`, so a player could not leave that ghost view with CIRCLE, L or a tap. Old auto-play pressed past it, which the run 39 watcher reported as a press before its pill.

Guard: in the ghost view the Back pill shows while the Trick button or BackToMain is registered, and not in demo mode. `tests/unit/test_back_pill.c` checks both a view the player can leave and one the scene holds. With it, Chapter 1's player-input run plays steps 13 to 20.

### 73. The d-pad could not scroll the camera tutorial

The camera tutorial (`FreeCamera TutorialStart`) keeps a talk open while it waits for the player to scroll. `free_camera_is_allowed` already let a touch drag scroll during it. But `move_free_camera` refused the d-pad and stick while any talk held the soul, so a player using buttons was stuck at Chapter 1's step 7. Native's free-camera input is not traced yet.

Guard: the d-pad follows the same tutorial exception as the touch drag. `tests/unit/test_script_input.c` holds right during the tutorial's talk.

### 74. The route pressed Rewind Time only once

Chapter 1's rewind (`Timeleap SELECT`, st01_game001 0xca8) waits for a press on the Rewind Time button; the port starts the time jump 63 ticks after a press lands while the button is up. The route pressed once, as its `touch` step began, and that press could be taken by something earlier in the press order, so the button stayed up for good and the honest run stopped at step 20. A player who sees the button presses it until the clock turns.

Guard: the route pad presses CROSS whenever the Rewind Time button waits and no page is waiting, as it does for the game-over menu. `tests/unit/test_route_pad.c` arms the rewind and checks the time jump is sent. With it the player-input replay plays through Chapter 1 and Chapter 2 to step 161.

### 75. Every dead talk was sent as DEADTALK

Native starts a dead talk in `func_0203bd84`. Its target is the core the soul last connected to: `game+0xef8`, written by `func_020266e8` whenever the chain connects (`func_02061718`). If that core is a ghost's own core (`game+0xe40` for Sissel, `+0xe44` for Missile, the cores `func_02010194` reads, which a ghost holds only in living objects), `func_0203bfc8` sends `* SYSTEM CORETALK`. If it is a dead person, it sends `SYSTEM DEADTALK` to that person, and with no core it sends `* SYSTEM DEADTALK`. The port always sent DEADTALK, to the named object or to everyone, so scenes that wait on CORETALK never started their talk; Chapter 16's st14_game020 is one.

Guard: `start_dead_talk` takes the soul's current core when none is named. It sends CORETALK when that core is living, and DEADTALK to the core's object when it is dead. `tests/unit/test_route_pad.c` starts a dead talk from a living core and from a dead one.

### 76. A carried-over countdown was never announced

Native game start (`func_02032328`) sends `* SYSTEM GAME START`. It then sends `* SYSTEM COUNTDOWN CARRY OVER` when the countdown task (`uCountdown`) is still running from the scene before. It sends `LOAD START` or `CHAPTER SELECT START` only for a game loaded from a save (flag 0x1004) or a chapter select (0x1005), never in normal play. The port kept the countdown across a script change (flag 0x1008) but never sent the carry-over, so a scene that handles it never ran that handler. Chapter 8's hub st03_game033 is one.

Guard: game start sends the carry-over whenever the countdown survived the change, and logs `countdown_carried_over`. `tests/unit/test_countdown_carry_over.c` starts a scene with and without a running countdown.

### 77. The stall breaker derailed the stories it was meant to carry

Superseded by root cause 173: the stall breaker is removed.

The breaker was built for the old instant auto-play. It pushed after 1200 quiet ticks (about 20 s) while a route drove, then closed talks, pushed waiting handlers, forced acts and scripts, and finally moved to the next scene on a list built from an old replay. At a player's pace that fired where nothing was stuck. A long conversation counted as quiet because the script just waits on the talk. On the Vita, run 40's pushes skipped Chapter 1's steps 73 to 89. Its scene timeouts then jumped from st01_game016 to st14_game040 (a Chapter 16/17 scene), so the device played chapters out of order. With the breaker on, the Mac replay played 67 steps, against 160 without it.

Guard:
- The breaker acts only while auto-play drives; a person playing is never pushed.
- It waits the player's limits: 3000 quiet ticks, or 18000 while the player can act.
- A talk or narration page turned by a press counts as progress.
- When a scene is stuck it skips the whole scene to the next scene in route order, logging `stall_left` and `stall_moved_to`. There are no pushes inside a scene, and scene timeouts and game-over move-ons use the same route order.
- The route pad never pushes the stick toward a core no chain reaches, so a failed move cannot grab a core by accident.
- It presses Rewind Time before reading pages.
- The app command `stall_breaker on|off` switches the breaker on the device.

With the breaker on, the Mac replay plays 284 steps in story order. The first skip is at Chapter 2's restaurant, where a route beat is missing. `tests/unit/test_stall_skip.c` covers a stuck routed scene being skipped to the next routed scene and a player without auto-play never being skipped. `tests/unit/test_route_pad.c` covers a move with no chain pushing nothing.

### 78. A free unit's position landed on the script's own actor

Op 0x82 (handler at `0x02024f0a`) makes a unit of any class from the factory registry (`func_0201ee6c` on `0x020bbbb4`), adds it at priority 6, parents it to the `uEvtScript`, and stores it in the VM's actor register `+0x14` (`mpFreeUnit`). Ops 0x83 and 0x116 overwrite and clear that same register. The scripts use it for seven classes: `uTelephoneLine` (152 sites), `uTelephoneCore` (56), `uTelephoneLineLeft`, `uTelephoneLineVertical`, `uTelephoneLineVerticalLeft` and the neon effects `uObjEffectNeonA` and `uObjEffectNeonB`. 209 of the 217 sites set a position with op 0x87 straight away. These units only draw. `uTelephoneCore`'s setup `func_02049df4` and update `func_02049f40` slide a line in, fade it and delete it; the line's update `func_0204a228` draws a glowing line from the phone to the screen edge. Op 0x84 (`0x02024f6c`, `func_0202fad4`) deletes every task of a class.

The port ignored 0x82, so the actor register still held the script's owner, and 0x87 moved the owner's actor to the line's position. That harmed any owner with a visible actor or a core. Chapter 10's denwa_003 was moved, harmlessly, because its actor is hidden.

Guard: the VM keeps a free unit in its actor register (`holds_free_unit`, `free_unit`). `script_machine_current_actor` returns the unit, and `script_machine_current_object` returns nothing, so no object op lands on the owner. 0x83 and 0x116 replace or clear the unit, 0x84 ends every unit of its class, and a new script clears them all. Each unit logs `free_unit_not_drawn` with its class until the lines and neon are drawn. `tests/unit/test_free_unit.c` places a unit and checks the owner stays put, covers 0x116, 0x83 and 0x84, and checks a full set of units still keeps the owner in place.

### 79. A direction in the living world reopened the ghost view

Native enters the ghost view from the living world only through a request code (code 12 in byte 2 of `game+8`, set by `func_02011080`), which `pGameMainNormal`'s update `func_02032488` reads. Every caller that sets a code takes it from a button object, never from the d-pad. The port added its own rule: the first push of the d-pad or stick opened the ghost view whenever the world was playable. On the Vita, Chapter 1's tutorial left the ghost view when the soul reached the shadanki (gameAct030). The player's next stick push opened it again, with no Trick or Back pill, so the trick could never be pressed: a softlock.

Guard: directions in the living world only scroll the free camera (`move_free_camera`, unless a script forbids it with op 0x6e). The ghost view opens only through the Ghost pill or the L button. `tests/unit/test_script_input.c` holds the d-pad and pushes the stick in the living world, then checks that the camera panned and the ghost view stayed shut.

### 80. The stall breaker went back to a finished scene

Superseded by root cause 173: the stall breaker is removed.

The breaker's search for the next scene in route order started at the furthest step already acted on, inclusive. From a scene with no route steps (Chapter 6's st03_game000, after the phone call at step 348 in st02_game024), the first step it looked at was that finished step. Its scene differs from the stuck one, so the breaker returned to st02_game024.

Guard: the search starts after the furthest acted step (or at the current route step, if that is later), as `first_step_not_yet_played` does. `tests/unit/test_stall_skip.c` starts in an unrouted scene after a finished block and checks that the breaker moves on to the next block, never back.

### 81. The soul jumped between cores while a scripted event held the ghost view

Native gives the player the chain only from the ghost view's own state. Scripted beats inside a ghost view leave the player without it:
- The tutorial view that `GameApp SoulVision On` opens (`func_0203de90`, pGameMainSVTutorial) does nothing in its idle state 99. It reads the chain only after `SVTUTO TouchCheck` (state 1) or `MoveStart` (states 20 to 22). All four `SoulVision On` sites, in Chapters 1 and 2, are followed by one of them.
- `CustomSoulVision DemoModeOn` (343 sites) runs a scripted beat inside the view.
- Op 0xe2 opens the view for a demo (`pGameMainSVforDemo`).
- `Button Input:Off`, a dead talk, narration, a movie, a choice and an open talk all hold it too.

The port held the chain only for an open talk or a choice. On the Vita, the player moved between cores between Chapter 1's ghost view introduction and its first talk.

Guard: one check, `a_scripted_event_holds_the_soul`, gates every chain input (touch, stick and d-pad): the view entering or leaving, view demo mode, the demo soul view, the tutorial hold until its check begins, Input:Off, a dead talk, narration, a movie, a choice, and a talk. The Back button is held during the tutorial hold as well. `tests/unit/test_soul_touch_rules.c` drags and holds the stick during the tutorial hold, DemoModeOn, Input:Off, the demo soul view and a movie, then checks that the soul stays put and moves again once the event ends.

### 82. Op 0x118 never set a table's start frame

Op 0x118 (case in `func_02023fdc`) sets byte `+0x143` and bit 7 of `+0x142` on a uScript2d (`func_02026744`). Its 33 sites all follow an op 0x117 that queues a table. The next queued start in `func_0205964c` passes that frame to `func_02007c14`/`func_02007cd4`, which pre-walks the table from its start. On the way it applies the point words (0xfff4, held for the box when the table keeps point zero; 0xfff5) and the core place word (0xfff0), skips the other commands, and stops at the first frame word whose frame number matches. When the table's box word (0xfffc) runs, playback jumps to the record after the last frame passed. The port logged `op_not_ported`, so these motions started from their first frame. Chapter 16's gun in st14_game031 and Chapter 17's jairo in st14_game040 run it.

Guard: 0x118 marks the current object's actor, and the queued start pre-walks the table the native way (`find_the_start_frame`); the box word resumes at the found word. `tests/unit/test_motion_start_frame.c` starts a table on its first frame by default, and on a later frame after 0x118 with the point word before it applied.

### 83. The free camera panned while a scripted event held the player

Directions in the living world pan the free camera (root cause 79). The port allowed it whenever the soul view was off and no talk paused the world, so a player could pan during demo-mode cutscenes, Input:Off stretches, narration and movies, where native takes the controls away.

Guard: the free camera (stick, d-pad and touch drag) follows the same rule as the chain. It moves only in a playable world (`script_soul_world_is_playable`: normal play mode, no pause, input on, no dead talk or choice) with no scripted event holding the player (`a_scripted_event_holds_the_soul`). Camera:Off and op 0x6e still forbid it, and the camera tutorial still allows it. `tests/unit/test_script_input.c` holds the d-pad in demo mode and under Input:Off and checks that the camera stays put.

### 84. The player kept the chain through film-strip cutscenes and trick animations

Two more scripted stretches take control away natively, and the port missed both:
- The film-strip cutscenes (op 0x6f film frame on, op 0x70 off), such as the "4 Minutes Before Death" leap.
- A trick's own animation. Scripts mark their core busy with op 0xea at the start of the core-action handler and clear it with op 0xe9 at the end. The port already hid the Trick pill for a busy core, but it let the player open the ghost view, move the chain and pan the camera. On the Vita, Chapter 1's player moved the chain while the blender rolled down the slope.

The player's reading: the chain is usually held exactly when the touch targets (the pills) leave the screen.

Guard: `a_scripted_event_holds_the_soul` also holds for the film frame and for a busy current core, which covers the chain and the free camera. The Ghost pill hides and the Ghost button (L) does nothing while the current core is busy. As a detector, any player move of the chain while no pill is on screen logs `chain_moved_without_touch_targets`; the whole-game replay logs it only at Chapter 1's three tutorial moves, where the tutorial grants the chain without pills. Tests:
- `tests/unit/test_soul_touch_rules.c` holds the chain for a busy core and the film frame, and checks the Ghost button waits for the trick to finish.
- `tests/unit/test_script_input.c` holds the camera in the film frame.
- `tests/unit/test_chain_without_touch_targets.c` checks the detector.

With this hold, the breaker replay's Chapter 3 mouse scene (st05_game000) loses steps 221 to 224. The old route reopened the ghost view during fuusya's trick animation, which a player cannot do.

Root cause 88 corrects the busy-core part of this guard: native holds the Ghost pill and the chain through ops 0xe6 and 0xe5, and the busy bit hides only the Trick pill.

### 85. The "Minutes Before Death" side label stayed lit on a black screen

The port draws the side HUD (the "Minutes Before Death" label and the hourglass) in the cover pass, after the screen fade. A fade to black left it bright on an empty screen. On the DS the whole screen fades through master brightness, so nothing stays lit.

Guard: the side HUD takes the screen's fade amount: it fades with the screen and is not drawn once the screen is black. `tests/unit/test_script_ui_view.c` draws it with no fade and with a full fade.

### 86. The lavender body at the start of a Minutes Before Death puzzle was never drawn

When a "Minutes Before Death" puzzle starts, the soul sits on a core (`souldead`, class `uGmkSoulDead`; `souldead2` is Missile's `uGmkSoulDeadMissile`) where the body landed in the cutscene, beside a past-dead body (`uHumPastDead` or `uHumPastDeadMissile`: `linnedead`, `linne_eft`, `carcass_silhouette`, `caron_dead` and 13 more across nine stages). Native never draws a past-dead body itself (empty draw `func_02053dd0`). On its first visible frame, `func_02053dd4` hands that frame to a `uPastDeadSprite` (class info `0x020ca128`, methods `0x020a4e24`). The sprite draws the frame as a flat lavender silhouette (DS colour `0x7e10`) at an alpha taken from its level, 0 to 31. The alpha is half the level in the ghost view and three quarters of it in the living world (`func_0202966c`, game+8 bit 5). Its update `func_020294f0` sets the level to 0 while the body is hidden and fades it in over 60 ticks when the body shows again. Slot 0xac (`func_020297b0`) holds the level for 60 ticks, fades it out over 100 and removes the sprite. Slot 0xb0 (`func_02029800`) shows it at full for 45 ticks, then fades it to 0 over 100. `uGmkSoulDead`'s update (`func_020478b0`) calls slot 0xac on its two core polygons the first tick the soul is no longer on its core (`func_020478f8`). The footage of st01_game011 (1084 to 1093 s) shows the lavender body and the core glow holding for about a second after the soul leaves for the desk lamp, then fading out together. The port gave past-dead bodies no drawing at all, so the lavender body never showed, and the start core's mark stayed lit after the soul left.

Guard: `script_ghost_copy.c` runs the sprite's level program for every past-dead body. It makes the copy at full on the body's first visible frame (what makes the native level full at creation is not traced; the footage shows it full), fades it in when the body shows again, and gives every start core (`script_class_is_soul_start`) the same program. The first tick the soul has left a start core it held, that core and every past-dead copy take the hold-60, fade-100 program. The stage draws a copy flat lavender at the native alpha in both views, and the start core's mark fades with its level. `tests/unit/test_ghost_copy.c` checks the full copy, the fade-in after a hide, the hold and fade when the soul leaves, and a start core the soul never held. Still open: native draws the frozen first frame where the body was, while the port draws the body's current frame. The trigger that fades the sprite itself (rather than the start core's polygons) when the soul leaves is not found, so the port ties it to the start core. The start core's glow in the living world is not drawn.

### 87. A follow onto "*" kept the actor docked

Op 0x85 (handler `0x02024f8a`) makes an actor follow a named object. When the name starts with `*`, native calls `func_020593f8(actor, 0, 0, 0)`, which ends the follow and leaves the actor where it stands. An unknown name leaves the dock as it was. Op 0x86 (`0x02024fe6`, a single dock) does nothing for `*`. The scripts use `*` (string offset 0) with op 0x85 at 131 sites in 29 scripts to let go of a carried object. The port looked `*` up as an object name, found none, and left the actor following, so a carried object stayed on its carrier. With the fix, the breaker replay plays steps 221 to 223 of Chapter 3's st05_game000 again.

Guard: the shared dock op treats a name starting with `*` as "every object": a follow ends, and a single dock changes nothing. `tests/unit/test_dock_onto_core.c` lets go of a follow with `*` and checks that a single dock onto `*` keeps the earlier dock.

### 88. The Ghost pill hid for a core the script never marked idle

Native holds the Ghost pill with ops 0xe6 and 0xe5 (game+0xc bit 0, the port's `ghost_pill_is_held_out`). Chapter 1's trick handlers bracket each trick animation with op 0xe6 and op 0xea at the start and op 0xe9 and op 0xe5 at the end. The busy bit that ops 0xea and 0xe9 set and clear hides only the Trick pill (`func_0204f850`, `func_020100bc`). Root cause 84 also hid the Ghost pill, ignored the Ghost button and held the chain for a busy core. Chapter 8's w_bell1 trick handler (st03_game011, 0x32fa) sets the busy bit and never clears it, so the Ghost pill never came back, and the player could not reach the lower column.

Guard: the Ghost pill, the Ghost button and the chain hold follow `ghost_pill_is_held_out`, and the busy bit stays on the Trick pill and the Trick button only. Chapter 1's blender is still held, through its op 0xe6. `tests/unit/test_soul_touch_rules.c` holds the chain and the Ghost button for the Ghost hold, and opens the ghost view for a core left busy without it. From the same st03_game011 entry, the honest replay now plays steps 400 to 411, where it stopped at step 410, and the breaker replay plays st05_game000's step 224 again, the last of the four steps root cause 84 cost.

### 89. The route hopped to moving cores on a timer

Some hops only open while a core moves: Chapter 2's santa ornament swings around its wreath, and Chapter 3's paper bounces up out of the opened wastebasket. A player watches the core and opens the ghost view when the path is open, which freezes it there. The route opened the view after a fixed dwell, so the hop worked or failed with the timing of the run. Native time stops the tick the Ghost button opens the view (the port pauses on the same tick), but the route checked its waits before the tick's motion update and the pad's press landed one update later, so a core about to change frame was already out of reach when the view froze.

Guard: the wait word `reach:<object>[:slot]` holds while the soul can reach that core through any chain of cores within reach, and the pad presses Ghost only while every reach in the step's wait still holds after the tick's motion, which is the picture the press freezes. A ghost_on whose moves end on a moving core waits on it, and when the path opens only at the far end of a swing the route closes the view and opens it again later, as the footage does. `tests/unit/test_route_pad.c` holds the press while the gate is out of reach and opens the view once it comes within reach. With it, Chapter 2 plays steps 90 to 167 and Chapter 3 its whole block with no stall breaker.

### 90. Route pacing kept the dwells of another route

`route_pacing.txt` stretches each step's dwell so auto-play never acts sooner after the previous step than the footage did, and it names steps by index. It was baked once and not again after the day's splices, so from Chapter 3 on every step carried the dwell of the step that used to have its index (one ghost_on waited 390 ticks for a paper that falls in 60). Rebaking exposed a second fault: the footage gap includes talk the player read at their own pace, which auto-play turns faster, so stretching the step after a talk makes it act later than the game state it was written for (Chapter 3's windmill and dictionary tricks fell after the madam left the shelf). Chapter 3's bird, windmill, dictionary and balloon steps also carried guessed footage times that the footage contradicts.

Guard: the pacing bake writes `route <crc32>` of the route_steps.txt it paced, and the player and the replay ignore a pacing file baked for another route (the replay prints it and the player logs `route_pacing_stale`), so a splice without a rebake falls back to each step's pace instead of misplacing every dwell. The bake never stretches a step that waits on the game (`reach:` or `act:`), and Chapter 3's footage times were read again from the footage. `tests/unit/test_script_route.c` reads a matching and a mismatched stamp; `tests/tools/test_route_pacing.py` checks the stamp and both exemptions. Rebake pacing and read-ahead (`make read-ahead PLAN_SET=game PLAN_STOP_TICK=4200000`) after every route change. A pass never shortens a dwell the previous pass baked for the same route (`never_shorter_than_before`), so two steps whose stretches move each other's windows settle instead of trading 150 ticks back and forth forever, which had stopped `make read-ahead` before it wrote the scene order and read-ahead files.

### 91. Demo mode shut the ghost view that native leaves open

Op 0x68 asks for demo mode (request word `game+0xe58` = 2). `func_02011110` applies it: byte 2 of `game+8` becomes 2 (`func_02011080`) and the main state machine enters `pGameMainDemoMode` (type `data_020cd92c`); op 0x67 puts it back in `pGameMainNormalMode` (3). The port's mode byte matches, but it showed the Ghost pill and took the Ghost button only in mode 3. Native `pGameMainDemoMode`'s update (`func_02034260`) shows pill 1, the Ghost pill, when the Trick buttons are enabled (`game+8` bits 13 and 8) and nothing halts the world, and a press on it opens the ghost view through `pGameMainRBMforDemo` (code 0x13). Scripts run timed beats in demo mode that the player catches in the ghost view: Chapter 2's santa ornament swings (op 0x68 in `santa_act00`, op 0x67 only when the swing ends), and the footage opens the view mid-swing, about 1.5 s after the trick, to hop to the bowl. Chapter 2's cart rolls the same way. The route stalled there in every run.

Guard: `script_soul_demo_mode_offers_the_ghost_view` holds in mode 2 while the world runs, no talk, dead talk, choice, narration or movie is up and input is on. The pill board then shows the Ghost pill (`GROUP_DEMO_MODE_PLAY`) and `take_ghost_button` opens the view, which leaves the mode byte at 2 until the script's own op 0x67. The Trick and notebook pills stay hidden, as native shows only pill 1 there. `tests/unit/test_route_pad.c` opens the view in demo mode and checks it stays shut while input is off. Chapter 2 now plays its whole block with no stall breaker.

### 92. The dead talk a tutorial scene waits for never started

Root cause 43 left the player's way into a dead talk open. Native's ghost-view update (`func_02032914`) checks, once the soul settles on a dead core, whether the scene handles both `SYSTEM|SV_DEADCORE` and `SVTUTO|onClick` (`func_02012518`). If so, it pushes `pGameMainDeadTalk` at once (code 0x15). Otherwise it opens a yes/no prompt (pills 10 and 11, trigger bits 0x1000 and 0x2000) that starts the talk on yes. Chapter 2's first dead talk is the tutorial kind (`sv_demo_02` in st06_game002 handles both), so arriving on the dog's core should start it. The port started dead talks only from a route `dead_talk` step, so the soul sat on the core and the route hopped on with no talk.

Guard: arriving on a dead core in the view marks it (`dead_core_was_just_reached`), and the soul step starts the dead talk there when the scene handles both keys. `tests/unit/test_route_pad.c` walks onto a dead core with and without the onClick handler. Still open: the yes/no prompt for every other dead core, which the route's `dead_talk` step stands in for.

### 93. The stall breaker cut a scripted sequence that runs in an idle handler

The breaker counts the game as quiet when no new script site has run for 240 ticks and nothing is running to a known end. It ignored timers in idle handlers, because an idle loop waits forever. But some idle handlers are a one-pass sequence: Chapter 3's madam answers the telephone in `madam_act06`'s idle handler, with waits of 274 and 76 ticks before she sends `tel_stop`. Meanwhile the telephone's ring loop repeats its old sites. The breaker skipped st05_game000 460 ticks after the hop to the telephone, on the Mac and on the Vita, so Chapter 3's last scene was cut short.

Guard: op 0x2f records whether it ran from a site visited for the first time (`wait_began_on_a_new_site`), and an idle handler's wait counts as running to a known end only then. A repeating loop's wait never counts. The 3000-tick limit still applies. `tests/unit/test_stall_skip.c` checks that a one-pass idle wait holds the breaker and a repeating one does not.

### 94. A retry put the soul back on the scene's first core

A failed rescue's retry reloads the scene when no script handles `SYSTEM|RESTART` (root cause 51), and the reloaded scene's GAME START names its first core again (op 0x9c). The port put the soul there. The reference footage starts the second attempt with the soul on the core it held when the attempt failed: Chapter 4's office (st02_game004) restarts with the soul on the step ladder, where the first attempt left it, and the ladder, notebook and desk lamp chain plays from there. Native's rollback (`func_0203b72c`, retry state 0x84) turns the hourglass off and sets the reload bit (`func_0202682c`) without touching the soul's core slots at game+0xe40 to +0xe48; only the quit path (`func_0200fe74`) clears them. Op 0x9c only stages a core on its object (+0xbf, +0xde, through `func_020101c0`). The port, with no native core slots, rebuilt the soul from the first core it was given, so the second attempt began 145 px from the ladder and every hop after it failed.

Guard: a retry that reloads keeps the soul's core (object name and slot) and the scene it belongs to on the machine, because every script change resets the soul. The first core the reloaded scene names is replaced by the kept one when that object exists, once. A different scene beginning drops it, so a fresh entry starts on its own first core. `tests/unit/test_scene_timeout.c` fails a scene with the soul on a second core and checks the retry starts there, that a later fresh start uses the scene's first core, and that another scene forgets the kept core. Still open: native also resumes the retried scene's timeline much later than its start (the footage's second attempt gets control 8 s after the retry, the port's about 130 s); the reload bit's reader is not found.

### 95. The route rang a telephone the game had not opened

Every telephone ride in the game passes through op 0x77, which native runs from the telephone's own handler: after its Dial trick (`core` `action`, in most scenes), after the soul leaves the ghost view on it (`phone_effect` `end`, st01_game014, st02_game004, st02_game022), or after a line choice (st03_game000's `gaisen`). Native's op 0x77 (case at `0x020246fa`) stores the current place (`words[1]`) in byte 1 of game+0x10 (`func_02026608`) and switches to the phone book scene (`func_020123a0(game, words[0])`, "pGameMainPhoneBook"). The book shows the dial wheel and the Go There / Stay Here panel; Go There sends `phone` `movie`, Stay Here sends `phone` `cancel`. In the port op 0x77 only turned the phone view on, and the route's `phone_call` sent `phone movie` the moment its wait passed, whether or not the book was open. So four routed rides (now steps 241, 310, 325 and 364, Chapters 3 and 5) skipped the Dial trick the footage shows, and a scene whose book had not opened rode anyway.

Guard: op 0x77 marks the phone book open (`phone_book_is_open`, cleared when a scene begins). The route's `phone_call` (Go There, with the destination place) and the new `phone_stay` (Stay Here, `phone cancel`) wait until the book is open and close it. The four routes now trick the telephone first, as the footage does at 4554, 7297.5, 7637.5 and 8752. `tests/unit/test_phone_book.c` checks that both picks wait for op 0x77 and that only Go There sets the place. Still open: the book itself is not drawn, and three steps outside the routed chapters ring a telephone whose book never opens (415 in st03_game011, which has no op 0x77; 492 and 542, whose Dial is not routed).

### 96. The second attempt's steps waited on a clock the retry restarted

Chapter 5's basement (st02_game022) fails by design on the first attempt: sys_act010's `event` `timer` handler waits 300 ticks and, when scene flag 0x2 is still clear, sets 0x7 and jumps to sys_act030, Lynne's death and op 0xdb. The scene has no `SYSTEM|RESTART` handler, so native's retry takes `pGameRestartShort` (`func_0203b72c` state 0x84; its `s11`, `func_0203e998`, sets game+8 bit 14 through `func_0203e9bc` and plays `pGameMainMovieRollbackShort`), tears down and returns to `pGameMain`, whose start (`func_02032018`) sees bit 14, restores the flag copy (`func_02011478`) and starts the script object again. The scene then runs from GAME START, and the footage's second attempt shows demo000's own Trick Time banner, so the port's reload is right. The route was not: it held the soul through the failure with `no_talk & ticks:3600`, and the `ticks` clock restarts at every script begin, so the hold ran 3600 ticks into the second attempt, past the 300-tick timer, and the soul never reached the telephone. Its next step then began a `phone_call` as soon as `no_talk` held, while the scene opens the book only after demo010 ends (b1_tel_022 act010's ACT_JUMP sends its own `SV_END` when the soul is on core 888, the line effect plays, and act020 runs op 0x77 0 0), and the route's 1800-tick action limit gave up first. The block also tricked the telephone from inside the ghost view: a trick fires from the living world, and this Dial only plays tel_svdemo's talk (`CustomSoulVision Start`, `DemoModeOn`, talk, `ReturnGame`).

Guard: the route has a `retried` wait (optionally `retried:N`) that holds once the scene has been retried since it was entered. The count survives the reload and an in-place `SYSTEM|RESTART`, and a fresh start or another scene clears it. `phone_call` and `phone_stay` are ready only while the book is open. `tests/unit/test_scene_timeout.c` holds a step on `retried` through a failed attempt and checks a fresh start and another scene forget the count; `tests/unit/test_phone_book.c` answers a book that opens 2500 ticks after the step. The honest replay from st02_game022's entry now plays the second attempt to the ride upstairs (sys_act040); the kettle rescue after it is not routed yet.

### 97. Balloons stayed up and tappable through the scenes that hide them

Ops 0x58, 0x59 and 0x4a were not ported, so they logged `op_not_ported` and changed nothing. Op 0x58 has seven sites in st02_game004, st02_game010, st02_game022 and st13_game000 (st02_game004's tel_svdemo_act010 runs it in its `SV_END` handler, 0x4b58); ops 0x59 and 0x4a are in st02_game004 and st02_game010, where `0x59` and `0x4a 1` come together and the later `0x58` runs before `0x4a 0`. Native op 0x59 calls each of the eight balloon slots' (`game+0xdfc`) vtable `+0xd8` (`func_02044e20`), which clears bit 0 of the slot's `+0x0e` and its `+0xf2`; op 0x58 is `func_02010974(game, 1)`, which calls `+0xd4` (`func_02010430`) to set them again, skipped while `game+0xc` bit 15 is set. The tap test (`func_020109c8`) and the timer pass (`func_02010b54`) both need bit 0, so a hidden balloon stays open but cannot be tapped and its timer stands still. Op 0x4a (`func_020106b8`) sets or clears `game+0xc` bit 15 and, when it sets it, hides slots 0 and 1 (`func_020108c0`). The balloon openers do not read that bit (`func_0201052c` reads bits 13 and 14), so a new balloon opens shown, and balloons hidden under the hold stay hidden after `0x58` then `0x4a 0`, as native leaves them. Op 0x4d (`func_020106a0`) closes the right tap balloon and also calls `func_020106b8(game, 0)`, so it lifts the hold too. In the port a balloon a demo had hidden still answered taps and the route's `balloon` waits, and its timer ran out behind the demo.

Guard: each balloon has `is_hidden`, and the machine keeps `balloons_stay_hidden`. Op 0x59 hides every open balloon and closes its drawing, op 0x58 shows them again (they redraw through `open_is_pending`) unless op 0x4a has set the hold, and op 0x4a hides the two tap slots when it sets it. Hidden balloons are skipped by taps, timers and the route's balloon waits; ops 0x4b and 0x50 still see them as open, as native's slot words do. `tests/unit/test_balloon_show_hide.c` checks a hidden balloon is neither tapped nor timed out, that showing restores both, that hiding twice closes the drawing once, that op 0x4a's hold refuses op 0x58 until it is cleared, and that op 0x4d clears it.

### 98. Route pacing stretched steps across gaps the game sets

`route_pacing.py` stretches each step so auto-play never acts sooner after the step before than the footage did. Across a ride or a scene change that gap is not the player's: the game plays the line effect, the arrival or a whole scene's opening, and the port's length differs from the footage's. In Chapter 5 the Go There ride upstairs (st02_game022, `phone_call` at footage 8062) runs about 320 ticks shorter in the port than in the footage, so the next step, the ghost view on the upstairs telephone, was stretched by 390 ticks and the kettle trick landed after keikan_act010's intro had set scene flag 0xa (tick 15032); sys_act040's `f1_yakan` `core_action` handler (0x325a) skips its jump to sys_act050 once 0xa is set, so the party room never began. The footage has the trick about 11 s after arrival, inside the intro.

Guard: the pacing tool never stretches a step that follows a `phone_call` or whose script differs from the step before; such a step still waits for the game as its `wait` says. `tests/tools/test_route_pacing.py` checks both. Eleven dwells dropped from the bake; the honest and scene-skip whole replays play the same steps as before, and from st02_game022's entry the kettle trick now starts sys_act050 and the soul hops down to the basement ball.

### 99. A turned topic panel stayed under the panel it replaced

Topic panels turn over to a new topic once the old one is cleared: Chapter 17's submarine talk (st14_game050 `stage_01`, idle handler at 0x44e) rebuilds its list with ops 0x3e and 0x40 each time, then op 0x41 puts `mujun` at index 1 once the time topic's flag 0x11f1 is set, and `target` at index 2 once 0x11f2 is set. Native's op 0x41 (`func_02011fac`) puts the new panel into the turned list at `game+0xfa4` when that slot is empty, and the footage (8:00:15 and 8:05:45) shows the turned topics in those slots, with the old ones gone. The port kept both lists but showed the panel in the main list first, so the time topic came back in slot 1, the contradictory-existence topic never appeared, and auto-play picked another panel. Once all three topics are cleared, op 0x42 adds Back, which leads on to `stage_04`.

Guard: a panel in the turned list shows over the panel at its index (`panel_at` in `script_choice.c`). `tests/unit/test_choice_panels.c` checks that a turned panel replaces the panel at its index. From st14_game050's entry the five topics and Back now play in footage order and the scene goes on to the telephone.

### 100. A finished message that stays on screen froze the scripts behind it

Native op 0x39 (`func_02023fdc`, case 0x39) waits while the message object at `game+0xf08` exists (`func_02011244`, made by `func_02014a8c`) or the select control does, and native's pause check (`func_02027db8`) uses the same test for classes 1 and 2. A message whose last page ends with the `auto_end` tag needs no Forward: in Chapter 12's talk with the dead (st09_game010 `deadtalkJodoAct021`, ACT_JUMP at 0x3f7c) the short line that closes the first topic stays on screen while op 0x39 lets the script go on to the flashback (footage 5:17:14). The port held op 0x39 and the class 1 and 2 scripts while any talk box was up, and kept the world halted, so the scene froze on that line until the replay's limit. What releases native's message object at that point is not traced; the footage fixes when.

Guard: `script_talk_holds_the_script` holds the script only until a page that ends the message on a forward is fully shown; op 0x39 and `script_machine_vm_is_paused` use it, and that page lets the world run while it stays on screen until the next talk or a Forward. `tests/unit/test_talk_wait.c` checks both endings. In the scene-skip replay Chapter 12 goes from 7 to 14 of 18 steps; the honest replay plays the same 387 steps.

### 101. Objects a script deleted lived on and answered later events

Op 0x17 deletes objects, and scripts use it 860 times in 50 scripts, mostly to clear a flashback's or a demo's objects when it ends. Native (`func_02023fdc` case 0x17) runs a by-name command (`func_02026968`, whose target loop `func_02026c1c` takes `?` for the caller, `*` for every script object, `@part` for every object whose name holds the part, or one exact name) and calls each target's virtual slot 8 (`func_020270b0`). The port had no op 0x17, so the objects stayed. In Chapter 17's submarine (st14_game050) `demo_121_end` deletes `st13_121_game` and the flashback's actors, but in the port `st13_121_game` lived on, answered the player's next ghost view with its SV_START handler (DemoModeOn, then ReturnDemo), and the soul could not move off the Door.

Guard: op 0x17 marks its targets deleted: they lose their actor (no drawing, no core), their queued messages and their updates, and name lookups and sends pass them over. `tests/unit/test_object_delete.c` checks the partial and exact names and the lookup. From st14_game050's entry the Control Panel, arm and telephone beats now play; the honest whole replay plays the same 387 steps.

### 102. A ghost view left with Back kept demo mode, so the Trick pill never came back

Native leaves the ghost view with Back in the view's update (`func_02032914`): when the Back pill is pressed (pad bits 0xa000, pill 2 enabled) it switches the main state to `pGameMainReturnNormal` (`data_020cd7d0`) and sets the mode byte to 3 with `func_0201192c`, whatever mode opened the view. Normal play's update (`func_0203696c`, the `pGameMainNormalMode` state) then offers the Trick pill. The port's Back press only closed the view, so a view a script opened after op 0x68 (demo mode) left the game in mode 2, where the pill board shows only the Ghost pill. In Chapter 6's floor lift (st02_game024) motorAct010 asks for demo mode, opens the view, and the player hops from the Step Ladder through the Kettle to the Telephone and presses Back; the footage then shows the Ghost and Trick pills and the Dial at 2:25:52, but the port never offered the Trick pill.

Guard: the Back press sets the mode byte to normal play before it closes the view. `tests/unit/test_back_pill.c` leaves a view opened in demo mode with Back and checks the game is back in mode 3. From st02_game024's entry the lift block now plays the Dial and the phone call and rides on to st03_root.

### 103. Ops 0xe9 and 0xea turned the trick off and on for the script's own object, not the selected one

Native's ops 0xe9 and 0xea (`func_02023fdc` case 0xe9/0xea) take their target from `func_02028148`: the VM's word +0x14, which op 0x83 sets, when it is not zero, and otherwise the owner (vtable slot 19). They then call `func_0202680c(core, op == 0xea)`, which writes bit 1 of the core's +0x14 word. The Trick press in normal play's update (`func_0203696c`) returns without a sound when bit 0 (`func_02036f3c`, ops 0x9d/0x9e) or bit 1 (`func_020100e4`) is set, and plays the refusal sound when the core's trick info (`func_02058ab8`, the top half of the same word, op 0x9f) names no trick. The port's two ops always marked the script's own object. Chapter 11's fan could not hand the trick back to the motor (`op_083 motor; op_0e9 0` in its halt handler), so the motor trick never fired. In Chapter 4's junkyard (st01_game021) the flag's airfan_action handler holds the air fan's trick while the fan runs (`op_083 dn_airfan; op_0ea 0`); the port marked the flag instead, and the route had a Trick step on the flag at 1:46:28 that only passed because that wrong mark held it until the flag was up. The flag has no core handler in any act, and the footage shows no trick there: the player waits on the flag while the fan, started 256 living ticks after its trick (the handler is pause class 0, so the ghost view from 1:46:23.5 to 1:46:26 holds the count, `func_02027db8`), lifts it, then opens the view at 1:46:34.

Guard: both ops act on `script_machine_current_object`, the op 0x83 selection or the owner. `tests/unit/test_core_trick_toggle.c` selects another object, turns its trick off and on, and checks the script's own object keeps its state. The junkyard's flag step is now a `wait` on the flag's risen act (`core:md_flag & act:md_flag:flagAct020`). A route Trick step belongs only where the footage shows the Trick pill pressed and the object has a trick; the port does not yet read the trick info id, so it still accepts a press on a core with no trick, where native refuses it.

### 104. Aborting a talk with the dead left the ghost view open, so the flashback it aborts for never ran

A talk topic that plays a flashback sends `GameApp DeadTalk|Abort`, starts the flashback from handlers of pause class 0, and sends `DeadTalk|Restart` when it ends (12 scripts, from Chapter 5's st02_game023 to Chapter 16's st14_game060). Native's `cGameAppCmdDeadTalk` (`func_0203c03c`) does more on Abort than send `CustomSoulVision|NoEffectFinish`: it switches the main state to `pGameMainReturnNormal` (`data_020cd7d0`, `func_02022f2c`) and sets the mode byte to 2 with `func_0201192c`, so the view closes without its effect and the world runs in demo mode. Restart sets the view's quiet-entry bit (`func_020135b8`, the bit `CustomSoulVision|NoEffectStart` sets in `func_02033504`), pushes the dead-talk state again with mode 0x15 and sets the view bit (`func_02010384`). The port only marked the next leaving as quiet, so the view and its pause stayed and the class 0 flashback handlers never ran: the honest run sat in st02_game023 after the second topic.

Guard: `DeadTalk|Abort` leaves the view for demo mode and `Restart` reopens it quietly; `tests/unit/test_quiet_view_pause.c` checks both. The fix is in the shared command, so the other 11 scripts inherit it.

### 105. A step gated on a script flag still got an idle dwell, and a view that must open right after a talk opened seconds late

Chapter 5's party room (st02_game022) runs on fixed timelines of op 0x2f waits that only count in living time. After the kettle trick the guard's talk (demo080) and Lynne's walk in (demo110: 323, 287 and 107 ticks, then `* linne on`) start together; native holds class 0 handlers while a talk is up, because opening a message sets game+8 bit 1 (`func_02014a8c` calls `func_02010384(game, 1)` and pushes `pGameMainTalk`) and the pause check reads that bit (`func_02027db8` through `func_0200f330`); the port holds them too (`pause_bit`). In the footage the player opens the ghost view 0.75 s after the talk and its Fate Changed banner end (8124.25), so demo110 has about 250 ticks left when the view closes at 8132 and the ball leaves the fan after 8145. The route's step before that view waited on `no_talk` alone, which already held before the talk opened, so the pacing pass measured the 36 s footage gap from the wrong moment and gave the view its longest dwell: the view opened 433 living ticks after the talk, Lynne was in and the ball fell 211 ticks after the next close, and by the hop at 8165 the ball had rolled 81 from the Toy Cake (61 in the footage).

Guard: the step before a view that follows a talk waits on the flag the talk's timeline sets (`flag:24 & playable & no_talk`, footage time of the pills' return), and `route_pacing.py` treats `flag:` and `noflag:` waits as synced to the game like `reach:` and `act:`, so they get no idle stretch; `tests/tools/test_route_pacing.py` checks it. When a hop onto a moving or rolling object is out of reach, compare the living ticks between the scene's last talk or view and the step against the footage before touching the engine. Still unported: op 0xfd and the Fate Changed banner (`GameApp EyeCatch|DestinyUpdate`), and the longer world hold after a view closes.

### 106. The whole-game set still closed on Chapter 3's card, a leftover from the two-chapter set

After the last script of the whole-game run (st15_demo120) the Vita showed the "Chapter 3" card for about 3 s. `game.json` was grown from the Chapter 2 set and kept its `next_chapter_card_texture`, the card a single-chapter set shows when its run finishes (`finish_run` in `script_player.c`) to stand in for the next chapter it does not hold. The whole-game set plays every card from the game's own data (`chapter_cards`, the CHAPTER_START effects native spawns), so a closing card there is always a stale one.

Guard: `refuse_a_closing_card_beside_the_game_s_own_cards` in `chapter_assets.py` stops the bake of any set that has both, and `tests/tools/test_closing_card.py` runs it over every shipped set. When a set is grown from another, check each key it inherited for something that only made sense at the smaller scope.

### 107. Every named sound was played as a loop, and a stop could be applied before the start it followed

On the Vita a mouse squeak or the "!" sound kept repeating until the scene changed. Ops 0x10c and 0x10d start a sound and give it a name (`func_02023fdc` cases 0x10c and 0x10d: `func_0202a0ec` / `func_0202a114` make the sound object, `func_020268e0` names it) so op 0x10e can stop it by that name (`func_0202aba4`). Nothing in those cases asks for a loop: whether a sound repeats is a property of the sound itself. The port looped every sound that had a name, so a one-shot with a name played until an explicit stop or the scene's stop-all. Of the game set's 479 sounds, 41 carry loop points in their source file; the bake read them and threw them away for sounds, keeping them only for music.

Two ordering faults sat beside it. The sound thread applied all of a batch's stops before its starts, so a loop started and stopped between two grains (a few ticks at normal speed, most loops in fast forward) was never stopped. And stops were taken from the mailbox while the port was still closed, when starts are held back, so a stop could be consumed before the start it was meant for.

Guard: the bake writes `sounds/<name>.loop` for a sound whose source has loop points, as it does for music. `start_voice` loops a named voice only when its clip has loop points, between those points; a named clip without them plays once and can still be cut by its name. Each start records how many stops were asked before it, and `stop_and_start_in_the_order_asked` replays the batch in that order; stops leave the mailbox only together with starts. `tests/unit/test_sound_engine.c` covers a named one-shot, a cut by name, start-then-stop and stop-then-start inside one grain, and a bad loop file; `tests/tools/test_end_to_end.py` checks the loop file. When a script op carries a name, check in the native case what the name is for before giving it a meaning.

### 108. Talk pages dropped their flash and shake, so a white screen a script left for the next page stayed white

In Chapter 2's talk with the dog's soul (st06_game000) the swirl turned white and stayed white for the rest of the talk. The script fades to white on the talk's pause (op 0xe 30, then op 0xa), sends `GameApp Message Forward`, and relies on the next page to bring the picture back: that page begins with the message's flash code. Native runs message control codes as small handlers: the flash (`func_02018480`) calls `func_02011d04(game, 4, 0)`, which is `func_020521d4(.., 0, 4, 0x100)`, the same four-frame white-in as script op 0x2b (`func_02023fdc` case 0x2b); the shake (`func_020184c8`) calls `func_020184f4` with the code's level and frames, the routine op 0x2a calls, and `func_0201851c` is the same for the message window (`func_02018510(game, 1)`). In the remaster's text these are the `mesflash` and `mesquake` tags, and the talk bake listed both as ignored: 2,208 flashes and 1,411 shakes in the game never played. In the DS message data the flash is control code 0x0f and the shake is 0x10 with (level, frames): (1, 40) for the bare tag, (1, n) for `LEVEL_M` and (2, n) for `LEVEL_L`, read by pairing each tagged page with its DS page.

Guard: `talk_assets.py` bakes a flash flag and the shake's level and frames into each page row, and stops on a shake level it does not know. `begin_reveal` starts the page's flash through `script_fades_add` (white-in, 4 frames) and its shake through the machine's shake fields, the paths ops 0x2b and 0x2a use. `tests/unit/test_talk_page_effects.c` whites the screen out and checks that a flashing page brings it back, that a plain page does not, the shake's level and length, and that an old page row still reads; `tests/tools/test_talk_page_effects.py` checks the tags. Still open: the window shake (`mesquake_win`, 9 pages), a flash or shake in the middle of a page plays at the page's start (21 flashes), and the `mes_white_in` tag (DS code 0x24 with 60 or 120 frames, 310 pages) is baked as a text fade without its native handler having been read.

### 109. Pills drawn over a chapter card and a black screen

Between chapters (the interlude scripts and the `chapter` card script run op 0x68, demo mode) the Ghost pill sat in the corner of the black screen and of the chapter card, in every chapter. Root cause 91 offers the Ghost pill in demo mode while the world runs; native also asks for `game+8` bits 13 and 8 (`func_02034260` through `func_020322d4` and `func_02013a14`), whose setters are still unread, and on the DS the master brightness (`func_020521d4` on `data_02084fc4`) darkens the pills with the rest of the screen. The port drew pills after the fade and the card.

Guard: `the_picture_is_covered` in `script_ui_view.c` (a card is up, or the fade is full) stops pills from drawing and from answering a tap, as the side label already did. `tests/unit/test_script_ui_view.c` checks a half fade, a full fade and a card. Open: the setters of bits 13 and 8.

### 110. A scripted soul view left people in colour and props white

Op 0xe2 opens the red world for a demo (`pGameMainSVforDemo`, `func_02023fdc` case 0xe2), the same view the player opens. The port styled objects for the view only while the player's own view was on (`soul->is_on`), so a scene that began with op 0xe2 (Chapter 4's junkyard, st01_game021) drew people in full colour and props as flat white shapes.

Guard: `report_soul_view` styles every object while either view is on and marks cores only in the player's. `tests/unit/test_demo_soul_view_styles.c`.

### 111. Demo mode in the ghost view unpaused the world, and a return to demo mode beat the script's own mode

Chapter 4's junkyard went black from the first hop onto the searchlight to the end of the scene. `subsp`'s `SV_CHGCORE` handler runs `DemoModeOn`, waits, fades out (op 0xc 30) and sends `ReturnDemo`; only then should `game`'s polling `check_soul` handler (pause class 0) see the core, jump to `gameAct060` and reopen the view with `NoEffectStart`, whose `SV_START` handler fades in (op 0xb 30). The port cleared the pause bit on `DemoModeOn` (commit c78054c1, no native reading), so the poll ran at once, the fade-in came before the fade-out, and brightness stayed at -16. Native `func_02033504` sets the pause bit only on `Start` and `NoEffectStart` (`func_02010384(window, 1)`); `DemoModeOn` takes the soul's input and hides pills, nothing else, and the pause check (`func_02027db8`) holds class 0 on that bit alone (`func_0200f330`). With the order right, a second fault showed: `ReturnDemo; ...; op 0x67` in one tick ended in demo mode, because GameApp commands run after the scripts. Native op 0x67 and 0x68 write the request word (`game+0xe58`) that `func_02011110` applies later, after `ReturnDemo`'s direct switch (`func_0201192c(window, 2)`).

Guard: `DemoModeOn` leaves the pause bit alone; the mode a script asked for this tick is applied again after the tick's GameApp commands (`mode_a_script_asked_for`). `tests/unit/test_quiet_view_pause.c` checks both. `tests/tools/check_no_step_in_the_dark.py` replays the whole game and fails when a route step begins on a fully black screen (it finds the old fault: 7 steps); `tests/tools/test_no_step_in_the_dark.py` tests its rule. The route step `ghost_off md_searchlightA` is now a no-op, since the script closes that view.

### 112. The move tutorial let the soul hop on after its goal

In Chapter 1's tutorial (st01_game000) the `SVTUTO|move` handler sends `MoveEnd` once the soul sits in the asked core, shows two messages and then sends `ReturnGame`. Native's tutorial view (`func_0203de90`, pGameMainSVTutorial) reads the chain only in the `TouchCheck` and `MoveStart` states and is idle (state 99) otherwise. The port's `MoveEnd` left input open, so during those messages the player could hop back to the crossing gate; the trick then ran on the gate and the scene could not go on.

Guard: `MoveEnd` holds the soul's input again until the view closes (`tutorial_holds_input`). `TouchCheckEnd` does not, because Chapter 2 (st06_game002) sends `DemoModeOff` before it and plays on in the same view. `tests/unit/test_soul_touch_rules.c` drags and steers after `MoveEnd`.

### 113. Asking for demo mode left an open ghost view open, and the world paused

Chapter 6's cell block (st03_game010) hung after the hop into cell 3. A hop onto a core in another cell runs `tocell`, which fades out, sends `CustomSoulVision|NoEffectFinish`, runs op 0x68 and moves the camera. For most cells it then reopens the view with `NoEffectStart`; for cell 3's first visit it does not, and `stage_master` and `joedo` (both pause class 0) start the scene there. The port kept the view open through op 0x68, so the world stayed paused and those handlers never ran.

Native ops 0x67 and 0x68 (`func_02011094`, `func_020110ec`) write the request word (`game+0xe58`) and queue `System|ChangeMode` with `func_02012400`, so the request is applied in order with the tick's other commands (`func_020134d4` calls `func_02011110`), not after them as root cause 111 assumed. For a demo request `func_02011110` does nothing when the mode byte is already 2; otherwise it sets it to 2 and switches the main state to `pGameMainDemoMode`. The ghost view's start (`func_020326a4`) sets the mode byte to 4, so a demo request inside a view replaces `pGameMainSV` (`func_020230dc` ends every state not shared with the new one), and the view's end (`func_02033140`) sends `SV_FINAL`, clears the view bit, and skips the leaving effect when `NoEffectFinish` set its byte +0x30. Not read: what releases class 0 scripts on this path (bit 0 of `game+8` is set each frame by `func_0200fd14` and cleared only by `func_02032488` and `func_02032018`); the scripts' own dependence on it decided that the world runs.

Closing the view there exposed a second fault. A view the script reopens with `NoEffectStart` is the player's to use once `DemoModeOff` arrives, but only a view opened with the Ghost button counted as waiting on the player, so the stall breaker left Chapter 8's st03_game011 during a step's dwell.

Guard: ops 0x67 and 0x68 queue `System|ChangeMode`; a demo request closes an open view, quietly after `NoEffectFinish`. A view that is open with demo mode off and input on waits on the player. `tests/unit/test_quiet_view_pause.c` checks the close, that a view opened after the request stays open, and the wait. Open: native op 0x67 inside a view would replace the view state the same way; no routed scene does it, so the port leaves the view open there.

### 114. A script could not skip or zero the death countdown

`GameApp` `Exception` commands are looked up by name in native's `func_02013b18`. `CountdownSkip` calls `func_02046ec0` on the countdown (count + 1, dirty) and `CountdownForceZero` calls `func_02046ed0` (count = 3, dirty). The port knew neither, so Chapter 7's kitchen (st04_game001) ran its clock on while the giant chicken fell, and the game ended before the fall did.

Guard: `run_exception_command` moves `countdown_step` and fires the step at once. `tests/unit/test_countdown_exceptions.c` sends both commands from a script.

### 115. Op 0xe1 marked the script's own object as the corpse, not the selected one

Eighteen of the 19 sites of op 0xe1 sit in the corpse's own script. The 19th (st04_game001, `func_set_gameover_obj`) selects the driver with op 0x83 first and runs from `sys`, so the port marked `sys` dead and the soul could not enter the driver. This is the class of cause 103: an actor op acts on the op 0x83 selection, or on the owner when there is none (`func_02028148`). Native's case 0xe1 itself was not read (the body of `func_02023fdc` is not decompiled); the site only makes sense this way.

Guard: op 0xe1 uses `script_machine_current_object`. `tests/unit/test_core_trick_toggle.c` selects another object, marks it, and checks the script's own object stays alive.

### 116. The death replay's white wipe (effect 50) did not exist, so 16 scripts waited for ever

Effect 50 (`iEffect::TIMELIMIT_FADEOUT`) is its own class in native (vtable `0x020a90d4`, set by `func_020473ec`; setup `func_0204748c`, update `func_02047574`). It plays a white wipe animation and reads that animation's frame events (`func_020662c4`): event 1 sends `SYSTEM` `TIMELIMIT W/I` to every object, event 2 lets the wipe leave. The script holds the wipe with `GameApp` `Exception` `GameOverWhiteWait` and frees it with `GameOverWhiteProceed` (both in `func_02013b18`), changing the scene under the white in between. When the animation ends native sends `SYSTEM` `TIMELIMIT W/O` and then turns the world grey with the same child as effect 53 (`func_02063b70`). The port drew nothing and sent nothing, so st13_demo000 (Chapter 7's replay of the driver's death) stopped at the crash; st03_demo010, which opens Chapter 8, waits on the same event, as do 14 more scripts.

Guard: `step_the_white_wipe` in `script_soul.c` runs the effect for any script: white through the brightness output, both events, the hold, then effect 53 under the wipe's name so deleting the wipe ends the grey. `tests/unit/test_white_wipe.c` covers the fade, the hold and the delete. Open: the animation's event frames are not decoded, so the ticks (white in 48, `W/I` at 210, leave from 252 over 90, grey 42 later) are measured from the footage at 12270 s, and the ring drawn on the white is not ported.

### 117. Ops 0x71 and 0x72 were not ported, so a flashback inside a talk kept or lost its film frame

Native's cases 0x6f to 0x72 in `func_02023fdc` all set bit 2 of `game+0xc` (`func_0201354c`) and then ask for the film frame to open or close: 0x6f through `func_02010ca4` and 0x70 through `func_02010d18`, 0x71 through `func_02010cdc` and 0x72 through `func_02010d30`. The second pair also sets bit 9, which makes `func_0200fd14` call the panel's sliding open and close (`func_02046b1c`, `func_02046b68`) in place of the plain ones (`func_02046b10`, `func_02046b44`). The port knew only 0x6f and 0x70 and logged `op_not_ported` for the other two, which have 24 sites in 10 scripts from Chapter 7 on (st04_game010, st13_demo100, st13_game012 to 022, st14_game030, st07_game000, st03_demo010).

Guard: all four ops share one case in `script_ops_view.c`. `tests/unit/test_film_frame_ops.c` runs each pair from a script and checks the machine's state and the view's output.

### 118. Lessons from routing Chapter 7's kitchen and restaurant

- A carried core changes height with its carrier's motion table. The chef's bowl (`cock_bow`) is within reach of the pedal only while the chef pours the wine (`cock_act02`); in the other acts it sits about 80 units from it. A hop that depends on such a core waits on `reach:<target>` from the step that opens the ghost view, since the view freezes the world.
- A second core is named by `slot` in the route step. Without it `move pedal` aims at the first core, which is out of reach, and the step fails with `route_hop_beyond_reach`.
- A trick that a watching character undoes is gated on that character's act. The pedal turns the table at any time, but the waitress turns it back unless she is at the fan switch (`act:waitress:wait_act09`).
- A telephone's `op_077` word is its own place, not the destination. The destination is the route step's `place`, matched by `phone.xml` (12 is the restaurant, 13 the kitchen, 21 the van).
- A scene's `sys` acts name the order the player must follow. st04_game010 ends only from `sys_act060`, reached through the bell rung three times, the pot ride, Rindge's talk and a hop to the sign.

Guard: the Chapter 7 route steps carry these waits, and the whole-game replay fails the step if one is dropped.

### 119. A message pause before a page's text was dropped, so the handler it signals never ran

A talk page can hold `<TRCK mes_pause />` before its first character. Jowd's dead talk (`m03_5630` page 16) has two: the first sends `message` `pause in` to `jodo_act00`, which fades the screen white and forwards; the second reaches `jodo_act01`, which forwards and arms the rewind with `GameApp` `Timeleap` `SELECT`. The baker kept only a pause at the end of a section, so the second `pause in` was never sent and the rewind button never came. Four pages in the game set have a pause before their text (`m02_3180`, `m03_5630`, `m13_2220`, `m13_3270`). The native message tag reader has not been found in `src/` yet; the behaviour here comes from those scripts' `pause in` handlers.

Guard: `talk_assets.py` counts the pauses before a section's text into an eleventh `page` field, and `script_talk.c` holds the page in `SCRIPT_TALK_PAUSED_BEFORE_TEXT`, sending `pause in` and waiting for a forward once per pause, before the box opens and the page's flash or quake runs. `tests/tools/test_talk_page_pauses.py` covers the baker and `tests/unit/test_talk_page_pauses.c` the engine. Open: the box is hidden during these pauses, which matches the `meswin disp="off"` around them on all four pages but is not read from native code.

### 120. Sissel's own balloon (ops 0x4c and 0x4e) needed an actor and was baked as a speech bubble

Native's cases 0x4c and 0x4e in `func_02023fdc` call `func_0201052c` and `func_020105d4` with the game object only: unlike 0x45, 0x46, 0x47, 0x51 and 0x52 they never ask for the script's actor (`func_02028148`). Both make the same balloon class (`data_020ce704`) in the right tap slot (`game+0xe00`, timer at `+0xe20`, read as a signed short, so -1 never runs out), after `func_020106a0` closes the one there. 0x4e stores a target, verb and argument (`func_0204649c`); 0x4c stores a talk label at `+0xd0` (`func_02046198`), and a 0x4c naming the label already open only renews the timer. On a tap `func_020463e4` opens that talk (`func_02010718`) or sends the message.

The port asked for an actor in both, so a balloon opened from an object with no sprite (`cicel_balloon`, `stage_master`, `sys`, `game_master`: 16 sites of 0x4e and 35 of 0x4c) never opened. It also drew 0x4c as a speech bubble holding the talk's text above the calling actor, and a tap opened nothing. st03_game032 could not be routed: its `CicelBalloon00` balloon holds off the guard's call while its talk plays.

Guard: `open_soul_balloon` in `script_ops_media.c` serves both ops with no owner, and `script_balloons_tap` opens the talk of a 0x4c balloon. The baker lists 0x4c labels as talks (`SOUL_BALLOON_TALK_OP`) and no longer bakes bubbles for 0x4c, 0x46 or 0x4e, whose first words are not bubble labels; the game set gained 163 talks. `tests/unit/test_soul_balloon.c` and `tests/tools/test_choice_talk_labels.py` cover it. Open: `func_0201052c` skips a 0x4c with mode 1 while bits 13 and 14 of `game+0xc` are set, which the port does not model; op 0x45 stores its label at `game+0xf18` for the same talk-on-tap path (`func_02010394`) but is still drawn as a bubble; op 0x4f (`func_0201062c`) is not ported.

### 121. Lessons from routing Chapter 8

Chapter 8 is 111 route steps in `game.json`: `st03_game003_Expand`, `st03_game031_Expand`, the rewind, `st03_demo010_Expand`, `st03_game032_Expand`, `st03_game011_Expand` and `st03_game033_Expand`, ending with the lever that blacks out the prison (footage 14792 s). They replace the old placeholder steps, whose path and times were guesses.

- A core in reach for less than the dwell of a `normal` step needs pace `instant` and a `reach:<target>` wait. The grounding sphere swings with a period of 120 ticks and is in reach of the bucket or the pool for about 15.
- A falling water drop is a core for a moment and bridges a hop longer than the reach (pool to bucket, wrench to helmet).
- A trick that only counts while another character is in one act waits on that act (`act:repairman:repairman_act03` for the lever).
- Arriving on a telephone can close the ghost view from the script, so the step after it is `ghost_on`, not `ghost_off`.
- In the past a telephone works only during a call. st03_game032 offers the line twice: `phone_stay` on the first call, `phone_call` on the second.
- A step that must beat a scripted timer is paced `quick`. In st03_game032 the guard's call starts about 124 ticks of play after the scene opens, and the balloon tap must come first.
- After a rewind `st03_root` picks the scene from flags 0x1028 to 0x102c, so the order is demo010, game032, game011, game033, game035.
- `script_replay`'s core list skipped nothing, so cores of deleted actors from an earlier stage looked usable; it now lists only actors that exist.
- A trick only counts in the act that handles it. Jowd's wastebasket sends `can_fly`, and the can handles it only in `kcan_00`, before Jowd sets the can down; the step waits on `act:joedo:joedo_02`. Tricked later, the can stays, flag 0xf stays clear, the prisoner reads a blank napkin (`muji_ring`) and the countdown runs out.
- The route's act rule (`script_soul_act_began`) leaves a step that waits on one act of an object when that object begins an act a later step of the scene waits on. A character who loops through his acts (the repairman: 00, 01, then 02 or 03 at random) therefore cannot be waited on in one act and later in an earlier one. The step after the shock waits on the shock's flag (`flag:21`) instead of `act:repairman:repairman_act00`.
- A press in the footage that changes nothing is left out when its outcome depends on a random loop. The player pulls the lever once before the repairman reaches for it (14006 s): sparks and no flag. The lever sends `shock` 32 ticks after the pull, and when the repairman's random act 03 comes up in that time the same pull lands the shock, which the footage's pull did not.
- A scene that opens inside the ghost view (`st03_game033`, entered by the hop from the spoon to `s_core`) starts with `move` steps that wait on `view & core:<start>`; `free` never holds there.

Guard: the whole-game replay plays the chapter's steps with their waits and fails a step when one is dropped: 849 steps done and 0 failed without the stall breaker, up to the start of Chapter 9 (`st03_game034_Expand`).

Open in these scenes: `op_not_ported` for 0x9a and 0x49 (st03_game003), 0xcd, 0xd3, 0x9f and 0x119 (st03_game031) and 0xc2, 0xc3, 0xfd (st03_game011), and the free units of classes `74c365b4` and `19062b66` that op 0x82 makes and nothing draws.

### 122. Op 0xae was not ported, so sped-up motions played at normal speed

Native's case 0xae in `func_02023fdc` calls `func_02059bd8` on the script's actor (`func_02028148`), which stores a speed shift in bits 6 and 7 of the motion controller's flags (`func_02059c0c`, actor `+0x126`): 0 for a word of 0 or 1, 1 for 2, 2 for anything above. The table runner `func_02007cd4` shifts the ticks of every frame word, and of every frame with its own ticks (`0xfffb`), right by that amount when they are above 1, and never below 1. So `0xae 2` plays a motion at twice the speed and `0xae 4` at four times, until `0xae 1` ends it; starting another table does not clear it. The game set uses the op 270 times (hurrying guards and prisoners, the ending's demos), and the port skipped it, so those motions ran slow and everything timed against them was late.

Guard: `speed_shift` on the actor, set by op 0xae and applied in `run_table` to both kinds of frame. `tests/unit/test_motion_speed_shift.c` checks each speed, the floor of one tick, that the speed outlives a table, and the `0xfffb` frame. Route lesson: the curry carrier in st03_game010 now walks at the native speed, so the hop from the bell to his spoon waits on `reach:spoon` at pace `instant` instead of a dwell that only fitted the slow walk.

### 123. Ops 0xa1 to 0xa3 were not ported, so a script could not ask which core Sissel or Missile holds

Native's cases 0xa0 to 0xa3 in `func_02023fdc` all call `func_02027630` with the named object and a mode. The modes pick whose core is compared: 0xa1 reads `game+0xe40` (`func_02027684`, Sissel's core), 0xa2 reads `game+0xe44` (`func_0202768c`, Missile's), and 0xa0 and 0xa3 fall to `func_02010194`, the core of the active ghost (bits 13 and 14 of `game+0xc`). The result goes to the VM's condition, 0 when the object or the core is missing. The port had only 0xa0, so the 38 sites of 0xa1 and 0xa2 in the Missile chapters (st13, st14) left the condition from an earlier op and took the wrong branch.

Guard: `compare_core_of_ghost` in `script_motion.c` reads the soul's per-ghost cores; 0xa3 shares 0xa0's case. `tests/unit/test_ghost_core_checks.c` asks about both ghosts' cores with each one active and with no core yet.

### 124. Missile's exchange (the swap) was not ported

Every Missile scene in st13 and st14 and Chapter 18's st13_game021 needs it. Native, by function:

- A shape is the byte at actor `+0xdf`, set by motion word 0xfffa (`func_02007cd4` calls `func_020083e0`) and read by `func_02058ba4`. The port skipped that word.
- When the chain finds a core (`func_02061718`) with Missile active (`func_0201132c`) and both objects have the same shape, not 0, it sends `EXCHANGE` `CONNECTED` to both and enters soul state 5 instead of hopping.
- `pGameMainExchange` (`func_0203447c`) outlines both cores over 0x18 ticks, scrolls the camera towards their midpoint, hides every HUD button and shows button 8 (`Exchange`) and 9 (`Unlock`). Button 9 sends `EXCHANGE` `CANCEL` to both (`func_0206124c`). Button 8 sends, a tick later, `EXCHANGE` `ROOT` to the first object and `EXCHANGE` `TARGET` to the second (`func_02034a44`), then waits until a script answers with op 0xa4 or 0xa5 (`func_020611b4` clears bit 2 of soul `+0x7d`). The chain then shakes for 0x19 ticks (soul state 7 in `func_0206001c`) and the mode waits 0x20 more before plain ghost view returns.
- The scripts do the visible swap in those handlers and name the new pair with op 0xa4 (root) and op 0xa5 (target). Op 0xa6 (`func_020610ac`) aims the chain at a core; the port's chain follows the target each tick, so it does nothing there.
- `GameApp` `Button` `All:On` sets every button (`func_02010f20`), so it brings back `Exchange` and `Unlock` too; `Exchange:On/Off` and `Unlock:On/Off` set them alone (`func_020135c8`). `GameApp` `Exchange` `TutoStart/TutoEnd` (`func_02034d54`) makes the mode send `*` `Exchange` `onClick` and `onRelease` as the drag starts and stops (`func_02034dcc`).

Guard: `script_exchange.c` holds the link; `tests/unit/test_exchange.c` covers the shape word, who may link, the outline wait, the wait for the scripts, the settle time, unlocking, and that only the two swap pills show while linked. `test_every_pill_has_a_place_and_draws` in `tests/unit/test_script_ui_view.c` draws and taps each pill id, so a new pill without a layout fails there. The route action is `swap` (object: the target core).

Class notes: touching a character pill (Jowd, Missile, Sissel) went to `pill_not_ported`; `press_the_pill` in `script_input.c` now sends it as the character button.

### 125. Ops 0xb3, 0xb9, 0xba, 0xee and the ending's 0x6a, 0xf8, 0xf9, 0xfa were not ported

Chapter 18 (st13) and every st15 demo logged `op_not_ported` for them. Native, by case in `func_02023fdc`:

- 0xb3 calls the named effect's slot 50 (`func_02027184`). For a plain movie that is `func_020670bc`: a movie whose frame scripts read the variable gets it set (`func_0202d700`), any other goes to its frame label `status<n>` and plays. The port played every movie as one loop from its age, so a movie that branches on the status or on a roll could not be played at all. The converter now reads each frame script (`frame_script_rows` in `effect_file.py`) and bakes `fxjump` and `fxstatus` rows; `fx_playhead.c` steps a playhead through them.
- 0xb9 is `func_02066500`: it stores the ticks, and `func_02066928` takes the effect from full to clear over them and deletes it.
- 0xba is `func_020273c4`: it stops the effect's task (`func_02008418`) when the word is not 0 and sets it going again (`func_02010dc4`) when it is; the effect stays drawn.
- 0xee is `func_02026fb0`: it sets bit 0 of `+0x141` on each named object so `func_020599c0` reloads its color tables before a movie. The port has no such cache, so it is filed as stores-only.
- 0x6a is `func_02011288`: it sets `game+0x103c` and switches the sub screen to `pGameSubEnding`. `func_020112b4` clears it when the staff roll leaves.
- 0xf8 starts `pGameMainStaffRoll` (`func_0203dbf0`, mode byte 0x17) on a page. `func_0203dcc0` waits 0x3c ticks, then the page (`uStaffRoll`, `func_02064750`) rests 0x8c ticks on its first screen and 0x78 on each later one, and scrolls one pixel a tick between them (`func_02064b88`). 0xf9 holds the script while the mode byte is 0x17 and the page is not over (`func_02026844`). 0xfa is `func_0203ddc4`: the roll closes and the mode byte under it comes back.
- The remaster keeps one texture per screen (`13_ending/staff_<page>_<screen>`). The bake finds the pages from the 0xf8 sites, counts each page's screens in the packs and writes `staffpage` rows.

Guard: `tests/unit/test_fx_playhead.c` (plain loop, hold and end still play by age; status variable; status label; rolled jump; table rows), `tests/unit/test_effect_control_ops.c` (the four effect ops and their word counts), `tests/unit/test_staff_roll.c` (rests, scroll, the script's wait, the mode byte and the ending mode coming back). `tests/tools/check_effect_scripts.py` sweeps all 166 effect movies in the packs: it fails when a branching frame script could not be described, when a jump or status row points outside its movie, or when the rows outgrow `SCRIPT_FX_JUMP_MAX` or `SCRIPT_FX_STATUS_MAX`.

Class notes: the sweep found seven movies with jump rows, not only the st13 ones (`st02_yakan_00`, `st05_batibati_00`, `st14_gyorai_01`, `mazlflash2` among them); a movie whose script stops before its last frame now holds there, as Flash does, where the port used to run on to the end.

### 126. A picked choice control was deleted in the same tick, so the handler waiting on it ran before the pick's event

In Chapter 9's talk with Jowd (st03_game012, `j_core`), cancelling the topic menu reopened it for ever. Native's choice control (`func_0205a9e8`) sends its event (`select|cancel` or the panel's own) in state 11 or 12, moves to state 13 and is deleted only on the next update pass (`func_02015be4`). The port cleared `control_is_up` inside `script_choice_pick`, so op 0x39 on the handler that opened the menu resumed in the same tick, jumped back to the act that opens the menu and reset the object's handlers before the event's handler could run.

Guard: the pick sets `delete_is_due` and `script_choice_delete_picked_control` clears the control after the tick's object pass. `tests/unit/test_choice_panels.c` (`test_a_picked_control_outlives_the_pick_by_one_object_pass`).

### 127. Pills and tap balloons drew over pictures that hide the world, and demo mode offered the Ghost pill with nobody playing

Reported from the device: the Ghost and Trick pills and the tap balloons stayed up over the swirling backdrop, over narration, over a black screen, and the balloons over the four-minutes-before-death film strip. They also slid in between dialogue boxes during a dramatic pause, and in demo mode whenever the world was merely running. Native keeps the swirl as bit 15 of `game+8` (`func_0200f084`, ops 0xdf and 0xe0), and every world draw asks `func_0203435c` first. The HUD board (`func_0204f850`, `func_02010ef4`) is asked for a group by the mode update: normal play (`func_02036f5c`) only while its mode is not the talk modes, and demo mode (`func_02034260`) only while the Ghost button is enabled and the world is not halted. A player's trick is what puts the world in demo mode (`func_02011110` from op 0x68); a talk (`func_02010ef4` with 0xff) takes the buttons away. The port showed the Ghost pill for the whole of demo mode, so every scripted beat between presses put the pills back up.

Guard: `script_machine_picture_is_hidden` is the swirl, the film strip, running narration or a fully black brightness. The pill group is none under it, a swirl hides the pills at once, balloon taps are dropped, and the HUD view carries the flag so the pills and balloons are neither drawn nor touchable. Demo mode offers the Ghost pill only while `a_trick_is_playing_out` is set, which a trick sets and a talk or the return to normal play clears. `tests/unit/test_ui_off_hidden_picture.c`. `tests/unit/test_route_pad.c` opens the view in demo mode only with a trick playing out.

### 128. A ghost's core op moved whichever ghost was in play, and a script's own actor was looked up by the name "?"

Chapter 18's swaps name the new pair with ops 0xa4 and 0xa5, and ops 0x9c, 0xa4 and 0xa7 place Sissel, Missile and the boss (`func_020101c0` from `func_02023fdc`). The port ran all three as `script_soul_set_first_core`, so placing Missile while Sissel was in play pulled Sissel onto Missile's core. They now record that ghost's core, and move the soul only when that ghost is the one in play. The same scripts address their own actor as "?"; hold changes and the core enable looked for an object of that name and found none, so they now use the object running the script. The swap's `EXCHANGE` `ROOT` and `TARGET` also hard-hide both actors for the scripted exchange.

Guard: `tests/unit/test_soul_placed_on_core.c` places each ghost and checks that placing another ghost leaves the one in play where it is.

### 129. A Missile hop used another copy of the same shape as a stepping stone, and the hop plan ignored which core a push picks first

When the chain finds a core (`func_02061718`) with Missile active and that core has the same shape as the one the chain leaves, not 0, native links the two (`EXCHANGE` `CONNECTED`, root cause 124) instead of hopping. The route's hop plan (`first_hop_toward`) still counted such a core as a stepping stone, so a `move` could aim at it, link, and never arrive. It also judged a hop by distance alone, while the chain takes the first core its tip picks on the way out: in the Chapter 16 submarine room (st14_game020) the lantern `gim_lan` sits about 3 pixels off the line from the valve to the lever, so a push toward the lever picks the lantern.

A first fix made the tip pick only the core the route aimed at. That let auto-play skip a core a player's push cannot skip, and its aim started as object 0, so with no route (hand play on the Vita) the chain could lock onto nothing else; three unit suites failed from it.

Now the plan compares shapes from the core each hop starts on, and accepts a hop only when a push of the stick lands on that core first. The check runs a copy of the real chain (`soul_chain_aim`, `soul_chain_step`) under the pad's own stick (`stick_aim_axis_speed` at full tilt) and asks the live pick (`core_picked_by_a_tip`, which `core_under_the_tip` also uses) at each tick. It tries the core itself, then aim points 8 and 12 pixels around it, as a thumb can tilt the stick, and the pad pushes toward the aim point that worked. A core that every such push picks first becomes part of the path instead.

Guard: `tests/unit/test_route_pad.c`: a core on the line is the hop a push takes; a core a slanted push can miss is passed with an aim on the far side; a Missile hop never steps from a core to one of its shape, judged from the hop's own start. Route note: st14_game020 (steps 997 to 1012) is still a placeholder that does not follow the walkthrough (Missile swaps the baseball and the basketballs; Sissel crosses on the baseball to the valve and the phone), and the scene-skip replay fails there until Chapter 16 is routed.

### 130. Lessons from routing Chapter 9

Chapter 9 opens in `st03_game034_Expand`, the power room, and continues in `st03_game012_Expand`, the cell block. It then moves to `st03_game014_Expand`, the upper cells, `st03_game004_Expand`, the guard room, and `st03_game020_Expand`, the telephone outside (route steps 850 to 958).

- A scene can leave on arrival at a core, not on a trick. `stage_move`'s `SV_CHGCORE` handler in st03_game034 checks the core with op 0xa0 and jumps to `st03_root` when the soul reaches the spoon, so that block is a ghost-on and five moves.
- A cut scene can wait for the ghost view to close on one core. `stage_master`'s first act in st03_game012 handles `SV_END` only with the soul on the picture frame; the player presses Back there (footage 15045.5) and the hunters' cut scene plays. Without that `ghost_off` the trick that follows never becomes ready. The trick then waits on the act the cut scene leaves (`act:stage_master:st03_01`).
- Jowd's talk is a dead talk on `j_core`: its `DEADTALK` handler offers two topic panels until `select cancel`, so the block asks both topics (`choose m03_7560`, `choose m03_7570`) and then `choose cancel`, as the footage does.
- The sniper's loop is phased by the lower hatch. While it is open, `zen_kansyu3` falls through it, climbs back and crosses the ceiling about every 1240 ticks, and `stage_master`'s `st03_01` idle catches Jowd when the sniper drops with Jowd's x in [662,711] (op 0x65 checks, then `joedo_check` and `gameover_req`). Opening the hatch at any other time left a fall on Jowd's second call, so the ghost-on before it waits for Hunter #2's descent (`act:zen_kansyu2:kansyu2_03`), as the footage does, and the call fires at once as the sniper enters the ceiling (`began:zen_kansyu3:kansyu3_01`, pace instant).
- A guard's reaction can span two acts. The drum's `core action` sends `EVENT drum` to `zen_kansyu5`, who walks over in `kansyu5_01` (about 260 ticks) and then stands for 600 ticks in `kansyu5_07`; both acts handle `EVENT fall`. The drum's handler asks for the demo mode (op 0x68) and `kansyu5_01` ends by giving play back (op 0x67), so the ghost view opens only once he stands there, and the steps that drop him through the upper hatch wait on `act:zen_kansyu5:kansyu5_07`, not on the act the event starts.
- The ride on the sniper's vest is a wait on reach: with the lower hatch closed he walks the ceiling from x 686 to 402 and back, so the hop on waits for `reach:zen_arm3` from the upper hatch mechanism and the hop off for `reach:lan1`.
- An internal phone answers with a choice, not the phone book. `lan1`'s `core action` in st03_game014 opens panels with op 0x40 (`choose kansyu` is the guard room), and the guard room's `tel` in st03_game004 offers `naisen` and `gaisen`; only `gaisen` opens the book (op 0x77), so the ride out is `trick`, `choose gaisen`, `phone_call` with `place` 11. The place is the number `phone.xml` compares (op 0x65 on register 26). Read it from that script and check the scene it loads against the footage: the book's order says nothing, and places 16, 18, 15 and 1 each loaded a scene with no Jowd in it before 11 (`st03_game020_Expand`, the telephone where Cabanela finds him) did.
- The talk at that telephone is a dead talk with three topics (`choose m03_8070`, `m03_8080`, `m03_8090`); once all are heard the third panel becomes `talkdemo`, the flashback, and `choose cancel` after it ends the scene (`stage_master`'s `shime`, then st03_game021 and `chapter.xml`).

Guard: the whole-game replay without the stall breaker: 958 steps done, 0 failed, ending Chapter 9 and loading Chapter 10's st09_game001.

### 131. A route step could wait on an act only for its whole length, so it could not react to a guard turning

Chapter 9's escape (st03_game012) times each of Jowd's calls on a guard: the player presses the moment a hunter turns away. The hunters loop through acts (Hunter #1: `kansyu1_01` and `kansyu1_02`, 120 ticks each; Hunter #2 walks, climbs and comes back in five acts), and an `act:` wait holds from an act's start to its end. A call waiting on `act:zen_kansyu1:kansyu1_02` fired late in the act, the hunter turned back, and the scene ended in the game over (`kansyu1_03`). The pacing also could not help: a step's dwell began only once its wait held, and the wait had to hold through the whole dwell.

Guard: the wait word `began:<object>:<act>` holds while that act is current and began at most 30 ticks ago (`act_began_tick`, set where native starts an act, op 0x23 in `start_act`). A step whose wait has a `began:` word counts its dwell from the step's own start, so the press comes at the first turn after the footage's pause. `tests/unit/test_route_pad.c` (`test_a_began_wait_holds_only_just_after_the_act_starts`, with a dwell longer than the window).

### 132. A demo ghost view outlived its scene, so the next scene's ghost view could not aim

Op 0xe2 opens the red world for a demo (`func_02023fdc` case 0xe2 puts the game's state machine in `pGameMainSVforDemo` and pauses with `func_02010384`); op 0xe3 closes it. A script that changes scene while that view is open never runs 0xe3: the internal phone in st03_game014 opens it in its `core action`, offers its panels, and the panel's handler loads `st03_root` (op 0x24, `func_02010350`, which pauses the same way and hands the game to the load). The port kept `demo_soul_is_on` across the change, because the teardown cleared the player's view, the film frame and the swirl but not the demo view. Two scenes later, at the telephone of st03_game020, the player's ghost view opened but `a_scripted_event_holds_the_soul` still saw a demo view, so the chain never left the core and the hop to Jowd failed as out of reach at 38 pixels.

Guard: `tear_down_script` closes a demo view the script left open and tells the stage. `tests/unit/test_demo_soul_view_styles.c` (`test_a_script_change_closes_the_demo_view_the_earlier_script_opened`). The replay's end-of-run state now prints what holds the soul (`soul_held`), which named the cause.

### 133. Lessons from routing Chapter 10

Chapter 10 is the justice minister's office: `st09_game001_Expand` (the body and the rewind), `st09_game003_Expand` (the four minutes) and `st09_game004_Expand` (the wrap-up), route steps 959 to 1047.

- A trick can count only in one act of the scene's controller. `sys` handles `flag core_action` in `sys_act050`, which begins when the minister knocks the pitcher over (`mizusasi set`), and not in `sys_act040` before it, so the flap waits on `act:sys:sys_act050`.
- A controller of the normal pause class cannot see a hop until the view closes. `sys_act060` asks itself `check move_fan` (op 0xa0 against `fan`, op 0x1c to ask again) and that handler stands still while the ghost view pauses the world; the footage has the player press Back on the fan (16836.75), so the route has that `ghost_off`.
- A hop can close the view by itself. `paper_check`'s `has_soul tell` sends `CustomSoulVision` `DemoModeOn` and `ReturnDemo` when the soul lands on the flying paper, so no `ghost_off` follows that hop and the next `ghost_on` waits for `reach:kusuri`.
- A flying object is a moving core like a walking guard: the papers' cores follow the core words of their motion tables, so the hop from the fan waits on `reach:paper1` (pace instant), not on a time.
- Follow the footage where it and the walkthrough order differ: the player turns the hanging basket's base once, on the first visit (16928.75), and only rocks its top on the last.
- "Listen in" is a balloon, not a trick: op 0x46 puts a balloon over the minister that sends `sys balloon click`, so the step is `tap_balloon balloon:click`. After the first swing the soul sits on the sword Lynne carries (`lken`), not on the statue's.
- The ride out is `place` 16 (st07): the book entry is new, so the scene it loads was checked against the footage (the room with the candle and the star decorations), after place 14 loaded st05.

Not ported and hit here: ops 0x9f, 0xfd and 0xd3 (st09_game003), and 0x49 (st09_game004).

Guard: the whole-game replay without the stall breaker: 1122 steps done, 0 failed. It ends Chapter 10 with the ride to st07 (place 16) and plays the steps Chapter 11 already had in st07_game000 (1048 to 1122).

### 134. Lessons from routing Chapter 11

Chapter 11 is Kamila's old house, `st07_game000_Expand`: the contraption and the talk with Kamila, route steps 1048 to 1171.

- A step that follows a trick's cut scene waits on the act, not on `free` alone. The world is still playable for a tick after the press, and a demo that a trick plays out offers the ghost view (`script_soul_demo_mode_offers_the_ghost_view`), so a `ghost_on` gated on `free & no_talk` opened the view inside `toyAct030` and froze the toy at its op 0xa9 (wait for the motion table to end); the core stayed 92 pixels from the ball. The step waits on `act:toy:toyAct000`, the act the handler returns to (op 0x23).
- A `ghost_on` that waits on a swinging core is pace instant. The fan decoration (`movestarSV`) follows the fan's motion tables, which `fan`'s `sync` and `sync2` handlers change with flags 0x11 and 0x1a; at pace normal the dwell let the swing pass and the step became ready again and again without acting.
- The dead talk belongs to the object that holds the `SYSTEM DEADTALK` handler. Kamila's is `deadtalk`, a core-only object docked to her (op 0x85), not `book`: native `func_0203bd84` sends DEADTALK to the dead core the soul sits on (`func_02058b4c`) and nothing for a core that is neither dead nor a ghost's own. The hop waits on flag 0x8, which the `SV_START` demo sets when it ends.
- Its panels are read from the replay's `choice_opened` marks: `m07_0310`, then `m07_0330` in the same place, `m07_0350`, and `panel_2_2` once flag 0x5 is set; flags 0x4 and 0x6 together end the talk (`select_loop`), and `deadtalkAct030` sets flag 0xa, which `game`'s `check` turns into `gameAct050` and the chapter's end.

Guard: the whole-game replay without the stall breaker: 1220 steps done, 0 failed. It ends Chapter 11 through `st07_game001_Expand` and `chapter`, and plays the steps Chapter 12 already had (1172 to 1220).

### 135. A route step could not wait for a walker to pass under something

Chapter 13's chandelier catches the lady only when she stands under it: `madam_act04`'s `chandelier_down` handler measures the x distance between her and the chandelier (op 0x92, `func_02023fdc` case 0x92) and compares it (op 0x65) with 40 pixels to count as an attempt at all, then with a quarter pixel on the first attempt, 8 pixels once flag 0xd is set and 16 once flag 0xe is too. The footage drops it twice as she leaves the crank (two dodges, which set 0xd and 0xe) and a third time as she walks under it. The route had no word for that: three steps gated on `act:madam:madam_act04` all fired as the act began, she dodged each and went back to her desk (`madam_act08`), and the step that raises her never became ready.

Guard: the wait word `near:<object>:<other>:<pixels>` holds while both actors exist and their first points are at most that many pixels apart along x, the measure op 0x92 takes. `tests/unit/test_route_pad.c` holds a step at 17 pixels and lets it go at 16. A step that waits on it is pace instant, like one that waits on a moving core.

Lessons from routing Chapter 13 (`st05_game001_Expand` and `st09_game020_Expand`, route steps 1193 to 1251):

- The steps this scene had were a placeholder in the wrong order that happened to end the replay in a playable state; a block is not routed until each trick's act and flags are read against the footage.
- A trick that needs no demo leaves the world playable, so `free & no_talk` is no gate after it. The chandelier's fire lights the rat only while the world runs: the ghost view froze `e_fire_f` at its op 0xaa and `mouse_06` never got `fire`. The next `ghost_on` waits on `act:mouse:mouse_04`.
- Read the footage's bubbles for tricks a coarse sheet hides. The left latch (`tendoor3`) is turned on the way to the crank (19966), which sets flag 0x7 and sends `mouse_06` on its way; the walkthrough turns it later, the footage does not.
- The crank is `handle`: `handle_act02` drops the chandelier and `handle_act01` raises it, so the raise waits on `act:madam:madam_act05`, the act she is caught in.
- The talk with the minister starts from `deadtalk`, the core-only object docked to him, like Kamila's (root cause 134); its panels are `panel_1`, `panel_2`, `panel_3_1` and `panel_3_2`. The ride out is `place` 21 (st13), which loads `st13_game016`, `st13_game015`, `chapter` and `st13_game010`.

Not ported and hit here: ops 0xb5 and 0xb6 (st05_game001) and 0x9f (st09_game020).

The whole-game replay without the stall breaker: 1251 steps done, 0 failed, stopping at the park's `st13_game010_Expand`.

### 136. All:On dropped the Missile and Sissel switch buttons

Native `GameApp` `Button` `All:On` calls `func_02010f20(game, 5)`, which sets each button's bit (`func_0204fbfc` mode 5 ors it into board+0x38) but skips slot 5 (Jawed) and slots 6 and 7 (Missile, Cicel): it adds buttons and leaves the character ones as they were. `All:Off` (`func_02010f20(game, 2)`) clears every slot, those three included. The port's `All:On` assigned its own set, so it also cleared Jowd's, Missile's and Sissel's buttons. In the park (`st13_game013_Expand`) `GAME START` registers `Missile:On` and `Cicel:On` and the `timeup` handler then sends `All:On`, so the switch (`func_02035078`, which needs slots 6 and 7 through `func_02010f84`) was never offered and Missile could not be chosen.

Guard: `script_pills_register_all` ors the `All:On` set into the registered buttons. `tests/unit/test_character_buttons.c` keeps the switch across `All:On` and loses it across `All:Off`, `All:On`.

### 137. A button hidden while pressed kept every other button out

A pressed button holds the others back until its hold runs out, and the hold only counts down while the button is shown. `All:Off` hides every button at once (`script_pills_unregister_all`) but left the hold, so a button hidden during its press (the park's `tire_in` handler sends `All:Off` right after Missile's Swap) kept `some_pill_is_leaving` true for the rest of the scene and no button slid in again: the next Swap was linked but could never be pressed.

Guard: `script_pills_unregister_all` clears the hold with the state. `tests/unit/test_trick_pill_hold.c` presses a button, sends `All:Off` and `All:On`, and the button comes back.

Lessons from routing Chapter 14 (`st13_game010_Expand`, `st13_game012_Expand`, `st13_game013_Expand`, `st13_game014_Expand` and `st09_game021_Expand`, route steps 1252 to 1343):

- The switch pill is offered in the ghost view only (`func_02035078` runs in the view), so a `character` step follows a `ghost_on`.
- A swap is `swap <target core>` from the core Missile holds: the planner links same-shape cores and presses Swap. The first swap (baseball for `gj_basketball`) puts the basketball on the seesaw; the park protector's escape from the globe climber launches whatever lies there, so the next `ghost_on` waits on `near:basketball:kasa:12`, or the tire is launched instead.
- The world must run for the fate to arrive: after the tire swap the route waits out of the view until flag 0x17, when the scene opens Missile's view itself (`CustomSoulVision` `Start` in `move4` and `move5`), then hops `dust`, `rugbyball` and swaps with `mino`.
- Scattered leaflets (`biraA` to `biraD`) are cores only while they fly; a `ghost_on` gated on `reach:<final core>` at pace instant waits until the whole hop chain exists.
- The scene closes the view by itself after some hops (`chickenbox`); a `ghost_off` there fails and is left out.
- The talk with the park protector starts from `dtbira` (`panel_1`, `panel_2`, `panel_3`, which loads `st09_root`), and the minister's from `deadtalk` (`panel_1`, `panel_2`, `panel_3`, `panel_3_2`), which ends the chapter through `st09_game022` and `chapter.xml`. The placeholder blocks for `st09_game000` and `st13_demo000` named scripts the story never loads here and are gone.

Not ported and hit here: op 0x9f (st09_game021).

The whole-game replay without the stall breaker: 1343 steps done, 0 failed, stopping at Chapter 15's `st02_game030_Expand`.

### 138. The ghost's reach was a bare line

The port drew the reach (the chain `func_0205fa2c` moves, and the straight beam the route's hops use) as the tentacle strip alone, so it read as a thin line. The footage draws the strip on top of a cloud of the soul's own blobs, for Sissel and for Missile alike. Native's tentacle drawing is not read; the cloud is measured from the footage at 60 frames per second (23851 for Sissel, 24603 for Missile): blobs of 10 to 26 pixels radius at 720p cover the line from root to tip in a band about 30 pixels to each side, every blob is re-rolled every second frame, and a few dots of 3 to 9 pixels rise above the line. The cloud keeps the soul's blue in every ghost's view; only the strip is hue-turned.

Guard: `soul_chain_cloud` lays the blobs out from the chain's points and both beam paths in `script_view.c` draw them under the strip with the soul body's blob texture, so every ghost and every reach shares it. `tests/unit/test_soul_chain.c` checks that the blobs cover the line to its tip, vary in size, change with the step, send dots above the line, and that a collapsed chain has none. Not ported: the body flowing along the line during a jump, which the footage shows as one thick mass leaving droplets behind.

### 139. Jowd's, Missile's and Sissel's buttons outlived the scene that asked for them

The port kept every registered button across a script change, so `Jawed:On` from the prison escape (st03_game012, the only script that sends it) still showed Jowd's button in Chapter 10's office, and Missile's switch showed in scenes that never offer it. No script sends `Jawed:Off`, and every scene that uses the switch sends `Missile:On` and `Cicel:On` again at its own start (st02_game034 and 035, st13_game013 and 021, st14_game020, 022 and 033), so the three character slots (5, 6 and 7, the ones `func_02010f20` skips for All:On) cannot outlive a script. This lifetime is read from the scripts; the native teardown that clears the slots is not identified.

Guard: `tear_down_script` drops the three character buttons and keeps the rest. `tests/unit/test_demo_soul_view_styles.c` changes script with all buttons up and checks that only the character ones are gone.

### 140. A route could not scroll the camera to an object, and the follow pulled a scroll back

Chapter 15's junkyard (st02_game034) waits in `sys_missile`'s `check` `camera_pos` handler until the player scrolls the camera to the bottom right (op 0x65 on registers 0x14 and 0x15). The `camera` step only held Right for the camera tutorial. Scrolling by hand also failed: the normal-play follow recentred the camera on the soul's core each tick. Native follows only in the normal play state of `func_0203696c`, which a d-pad or touch scroll leaves through `func_020110b8`.

Guard: a `camera` step with an object holds the d-pad toward it until its actor is inside the view's margins or the camera is clamped (`script_soul_route_camera_push`, pressed by `script_route_pad`), and `follow_core_with_camera` stands still on a tick the player scrolled. `tests/unit/test_route_pad.c` scrolls to an object 600 pixels away and checks the step ends with it in view.

### 141. Op 0xa6 did not take the soul to a core

Native's case 0xa6 in `func_02023fdc` calls `func_020610ac`, which sends the soul along its chain to the named object's core; Missile's arrival in the junkyard uses it. The port skipped the op, so the scene waited on a soul that never came.

Guard: op 0xa6 calls `script_soul_travel_to_core`, the same drag a player's pick ends in. `tests/unit/test_soul_placed_on_core.c` runs the op in the ghost view and checks the soul's core.

### 142. A route step could not wait for two cores to come close

`near` measures actors' first points along x, as op 0x92 does. A rolling object keeps its actor point and moves only its core (the junkyard's trash can lid), so a link that depends on Missile's reach could not be timed.

Guard: the wait word `cores:<object>:<other>:<pixels>` holds while the two first cores are within that distance in both axes. `tests/unit/test_route_pad.c` covers it.

Lessons from routing Chapter 15's first three scenes (`st02_game030_Expand`, `st02_game032_Expand` and `st02_game034_Expand`, route steps 1344 to 1389):

- The death countdown ticks only in normal play outside the view (`func_02046ce8`), so every tick a route idles in the living world brings the death closer. The junkyard offers a balloon (`click` `cicel_talk`, op 0x4e) that starts Missile's demo at once; left alone, the demo starts 580 ticks later and the countdown ends before the last swap. The route taps it.
- A swap moves the two objects, not Missile: after the tire and lid swap Missile is still in `b1_03tire`, which now plays the lid's motions, and the door's swap target is `b1_floor_ex`. Read the `EXCHANGE` handlers for the object names.
- The lid falls flat inside `b1_03tire_act020` with no act change; the last ghost-on waits `ticks:200` after the ghost-off, the footage's four seconds. A wait word for an object's shape is still missing.

### 143. The default buttons did not come back at a script change

A scene that ends with `All:Off` left the next script with no Ghost or Trick button. 108 of the 117 game scripts never send `All:On`, and the scene after `st02_game035_Expand` (which ends with `All:Off`) sends none, so the wrap-up talk of Chapter 15 could not be entered. Chapter 1's scripts each send `All:Off` again when they want the buttons gone. The lifetime is read from the scripts; native's reset of the board at `game+0xfb8` on a script change is not identified.

Guard: `tear_down_script` registers the default buttons again, lets tricks be pressed again (`trick_is_on`, which `All:Off` clears and Chapter 16's first scene needs) and then drops the character ones (root cause 139). `tests/unit/test_demo_soul_view_styles.c` checks that Trick and Ghost return after an unregister and that the character buttons do not.

### 144. A scene that makes and deletes objects over and over ran out of object slots

`create_object` only appended, so the 128 slots (`SCRIPT_OBJECT_MAX`) filled with deleted objects. `st02_game036_Expand` creates its 17 flashback objects before each topic (`func_create_demo_obj`) and deletes them after (op 0x17 `@demo`); by the third flashback `demohum2`, `demohum3` and `demosys` were never created, op 0x83 fell back to the handler's owner and its op 0xa9 waited on a motion that never played. Native's object pool is not read; the port only needs deleted slots to be usable again.

Guard: at the cap `create_object` takes the first deleted slot that is not the running handler's owner, and notes `too_many_objects` only when there is none. `tests/unit/test_object_slots.c` fills the table, deletes and creates again.

### 145. A talk that starts again left the flashback's film frame on

A topic's flashback turns the film frame on with op 0x6f and several end handlers never run op 0x70 (`st04_demo_700` `end` in `st02_game036_Expand`): they send `GameApp DeadTalk|Restart`. Native's dead-talk start (`func_0203bd18`, which sets mode 0x15 and calls `func_0203b648`) closes the frame itself with `func_02010d18`. The port left it on, and the frame holds every button and the route pad, so after the talk's `cancel` the view could not be played.

Guard: `start_dead_talk` in `script_soul.c` closes the frame and reports it. `tests/unit/test_dead_talk.c` restarts a talk with the frame on.

Lessons from routing the rest of Chapter 15 (`st02_game035_Expand` and `st02_game036_Expand`, route steps 1390 to 1444):

- The `game_over` object sends `sys_sub` `start` `game_over` on every swivel of the desk lamp (`stand`) and every rock of the helmet (`f1_03met`). It is harmless or progress only inside `sys_sub_act020` with flags 0x12 and 0x13 set and 0x14 clear; anywhere else it jumps to `sys_sub_act100` and the scene is lost. Both tricks wait on `act:sys_sub:sys_sub_act020 & flag:0x13 & noflag:0x14`.
- The lamp must finish its swivel (flag 0x23) before the hop from its light core (slot 1) to the helmet, which is out of reach from the base.
- Trick Time begins at `sys_sub_act052`, not `act050`.
- The scene closes the view itself after the first swap, so no ghost-off follows it. The bullet's swap target is `f1_03book`, which carries the knit hat's look after the first swap.
- The footage changes character in the living world; the port offers the switch only in the view, so the route's `character` steps follow a ghost-on.
- The wrap-up talk: `move kaba_core` and `dead_talk kaba_core` from the telephone, topics `msg_000`, `msg_010`, `msg_020`, `msg_040`, then `cancel`; hops to `stand` and `kanri_core`, topics `msg_100`, `msg_110`, `msg_120`, then `cancel`, which ends the chapter through `func_check_select` and `sys_act020`.

### 146. The pause menu's tables never reached the Vita, because the upload kept its own copy of the converter's folder list

The converter writes into a fixed set of output folders and sweeps stale files from them. `vita_loop.py upload-assets` walked a second, hand-written copy of that list. When the pause menu added `pause/`, only the converter's copy changed. Its textures went up, but `pause/cat.txt`, `pause/cat_track.txt` and `pause/labels.txt` were never uploaded, so the device would have found no menu art and START would do nothing. The list now lives once, in `tools/assets/output_folders.py`, and the converter and the upload both read it.

Guard: `tests/tools/test_vita_loop.py` puts a file in every folder of `output_folders.NAMES` and checks that each is listed for upload.

### 147. People portraits were assigned by guessing from the image numbers, so twelve people showed someone else's face

The remaster's notebook images (`cmn_db_03_00` to `_35`) carry no table that ties them to the database names, and the DS notebook draws no faces (overlay 006 only draws text cards, `func_ov006_020d7108`), so there is no native order to follow. The first `portraits` map guessed from the image numbers and the story order. Each image was still used once, so nothing looked wrong in the data, but eight people were swapped among themselves and four took images that belonged to others. The fixes came from the user reading the screen. People whose image is not confirmed are now listed in `unknown_portraits` and show the unknown silhouette instead of a guess.

Guard: `tests/tools/test_pause_people.py` requires every listed person to be either in `portraits` or in `unknown_portraits`, never both, and every image to be used once.

### 148. The controls page wrote button names as words, and the longest action ran past its row

Each controls row drew a key name at a fixed x and its action at x 400, with no limit on the action's width, so the stick's action ran past the row's right edge. The key column now draws the Vita's button glyphs (`hud_art.glyph_cells`, the same art as the HUD prompts), and the action starts right after them. `pause_draw_text_within` shrinks any text wider than its room, so a longer translation cannot overflow either.

Guard: `tests/tools/test_pause_glyphs.py` checks that every glyph the pause screens name is baked, inside the sheet and on its own cell. `tests/unit/test_pause_widgets.c` checks glyph widths and that text wider than its room shrinks to fit.

### 149. Every fade to black was swallowed, and a fresh start drew the room before the opening's black

The opening flashed the junkyard for a few frames before its black narration screen. Two causes sat on the same output. First, root cause 116's white wipe changed `script_fades_update` to report the larger of the wipe level and the script's brightness. With no wipe running the wipe level is 0, so every negative brightness (a fade to black, op 0xc and its kin) was reported as 0, in every chapter, from that commit on. A whole-game replay reported no dark brightness at all; it now reports 79 in the first 20,000 ticks. The wipe now counts only while one is running. Second, the player started each script at full brightness and with no screen fade. On the DS a script is always entered from black (the title, a load or the previous script's fade-out hands over a black screen) and each script fades itself in, so `st01_game000` placing its actors at tick 10 and covering them at tick 13 shows nothing natively. A top-level start (`script_player_play`: the opening, Continue, a chapter replay, Title) now begins at brightness -16 with the screen faded (`script_machine_start_from_black`), and the replay starts the same way. Scripts chained inside a run keep the brightness they inherit. The honest whole-game replay is unchanged by both fixes (432 steps done, the same 4 failures).

Guard: `tests/unit/test_screen_brightness.c` checks that a fade to black reaches the brightness output when no white wipe runs, that a running wipe still wins, and that a fresh start reports black on its first tick.

### 150. Removing the controls page's key strings left the People tabs' L and R blank

Root cause 148 replaced the controls page's key names with button glyphs and deleted the `key_*` strings from `game.json`. The People page still drew `bar.key_l` and `bar.key_r` as words inside its shoulder pills, and a missing text key draws nothing, so the pills showed empty on the Vita. The tabs now draw the L and R button glyphs, like the controls page, and the unused shoulder pill is gone from the sheet. A string can be removed only when no pause code names it.

Guard: `tests/tools/test_pause_option_keys.py` checks that every literal `bar.`, `row.`, `header.` and `language.` key in `src/app/pause_*.c` has a string, that every Options row, value and bar text the menu builds from its row table has one, and that two-line bar texts stay two lines short enough for the bar.

### 151. Narration shown all at once took the dismissing press while its lines were still fading in

A narration page with the show-at-once option (DS timing, which the player uses) reports every line in one tick and goes straight to waiting for a press, while the view fades each line in over 62 ticks. So the first ✕ dismissed text that was still half drawn. The same gap left a group's fade running after a press that showed the whole page. The narration now counts down the fade from each line it reports: until it ends, a press finishes every fade at once (`SCRIPT_NARRATION_TEXT_FULL`, `scene_layers_finish_text_fade_ins`) instead of moving on, and the page arrow and auto-press wait for it. Dialogue already worked this way. The honest whole-game replay is unchanged (432 steps done, the same 4 failures).

Guard: `tests/unit/test_narration_skip.c` checks that a press during a shown-at-once page's fade finishes the lines without ending the page, that the second press ends it, and that the arrow waits for the fade.

### 152. A swap the scene itself closed the view on was never counted

`st14_game020_Expand` answers a swap and, in the same handler, leaves the ghost view (`torikae_demo`: `CustomSoulVision` `DemoModeOn`, `ReturnDemo`). `script_exchange_step` drops the link as soon as the view is gone, so the settle never finished, `swaps_done` stayed put and the route's `swap` step timed out as failed although the objects had changed places.

Guard: a link dropped while settling counts its swap. `tests/unit/test_exchange.c` closes the view mid-settle. Where Missile ends is unchanged: he stays in place, in the object the scripts name with op 0xa4 (the footage of Chapters 14 and 16 agrees; native's `func_0203447c` case 3 writes both ends, `func_020100f4` and `func_020266e8`, and which of them the cursor follows is not settled by reading).

Lessons from routing Chapter 16's first three scenes (`st14_game020_Expand`, `st14_game030_Expand` and `st14_game031_Expand`, route steps 1445 to 1485):

- Missile sits in the emergency light `gim_pato1`; the talk is a link from the telephone (`move gim_pato1`, `dead_talk gim_pato1`, which sends `CORETALK`).
- The footage links from the basketball back to the baseball, not the other way: Missile hops `gim_pato1` to `gim_tama1`, swaps with `gim_tama3` and is then in `gim_tama3` in the middle. For the second swap he steps aside to `gim_pato1` so Sissel can use the baseball, comes back and swaps `gim_tama3` with `gim_tama2`.
- A ghost cannot enter a core the other ghost holds, so the order is Missile off the baseball, Sissel across it to `gim_pato2`, Missile back on.
- Reaching the ringing phone (`gim_lan`, and `lan` in the control room) starts the call by itself (`SYSTEM` `SV_END` with the soul on that core); the route only answers the prompt of op 0x77 with `phone_call`.
- In the control room the first talk plays by itself; the wheel is `handle`, Lynne's core is `linne`, and the rewind is a `touch` on `timeleap_armed`.
- Four minutes before the death the soul starts on `linne_dead_core` and the gun comes into reach when Kamila levels it (`reach:gun`, pace instant); the path is gun, `handle`, `moni`, `lan` and the valve above the phone (`valve`), whose trick knocks the receiver off.
- Op 0xfd (the checkpoint at the fate change in `st14_game021_Expand`) is not ported.

### 153. A placing op run by an object with no unit moved the ghost into it

`st14_game040_Expand`'s `roll_demo` puts Sissel back after the torpedo turns: it selects `jairo_sv_left` or `jairo_sv_right` with op 0x83 and runs op 0x9c. Neither name is an object of that script, so the selection stays empty and the op runs on the handler's owner, `stage_master`, which has no unit. Native's cases 0x9c, 0xa4 and 0xa7 in `func_02023fdc` place a ghost only when the current object passes the class test `func_02008828(t, data_020cf898)`; the stage master does not, so Sissel stays in the right gyroscope, as the footage shows (he hops back from it over the detonator and the striker to the joint). The port placed him in `stage_master`, 357 pixels from any core, and every hop after the roll failed.

Guard: ops 0x9c, 0xa4 and 0xa7 act only on an object whose actor exists. `tests/unit/test_soul_placed_on_core.c` runs each on an object without one.

### 154. A route had no way to press on the frame a trick tests

The gyroscopes of `st14_game040_Expand` stop with the weight up only when their `core` `ready` handler finds frame register 2 in 10 to 12 of a 20-frame loop at 4 ticks a frame (frames 20, 1 and 2 stop it weight down, the rest do nothing). A route step could wait on acts, flags and distances but not on what an actor shows, so the press landed on whatever frame the loop was at.

Guard: the wait word `frame:<object>:<number>` holds while that actor shows that frame. The Trick pill takes about 24 ticks from the press to `ready`, so the route presses at frame 5. `tests/unit/test_route_pad.c` covers the word.

### 155. A monologue numbered its paragraphs across pages and never said a page had ended

Native's monologue update `func_02054f04` sends `*` `monologue` `paragraphN` when a paragraph has faded in, with N counted from the start of the page (state 3 sets the index back to 0 for each page) and only for the first four (`data_0209d70c`). When a page's last wait is over it sends `monologue` `pagefeed` if more pages follow and `monologue` `before end` on the last one, unless the page has option 2, which closes at once. The port numbered paragraphs across the whole message (eight names) and sent neither page event. `st15_demo000_Expand` builds its `paragraph1` handler on `pagefeed`, and `st01_game014` and `st02_game024` are the other scripts with `monologue` handlers.

Guard: `script_narration_report_to_outputs` numbers a paragraph within its page and sends the two page events. `tests/unit/test_monologue_events.c` checks the numbering on a second page, both page events and that a cut page sends none.

### 156. The 0x8000 flag bank was never cleared

Native keeps the flags from 0x8000 at game+0xd64 and `func_02011584` zeroes its 0x20 bytes from the scene start `func_02032018` in every branch except language select and the retry (`func_020322c4`, which restores the saved flags instead). The port cleared the bank only when the machine was made, so a flag one scene set was still set chapters later: `st14_game021_Expand` and `st02_game034_Expand` set 0x8001, and `st14_game061_Expand` then skipped the talk its first ghost view plays while 0x8001 is clear (footage 30400 to 30418). No script tests a flag of this bank that it does not set itself (nine flags, read from every script), so the bank belongs to one scene and its retries.

Guard: `begin_script` clears the bank unless the scene is being retried. `tests/unit/test_chapter_flags.c` checks both and that the 0x1000 bank is left alone. A retry that leaves through a root script (only where op 0xfd is missing) still clears it.

### 157. A core's trick info was never read, so every core took a trick

Native's op 0x9f (`func_02023fdc` case 0x9f) takes the current object (`func_02028148`), and when it is a `uGimmick` (`func_02008828(t, data_020cf87c)`) and the core index is below its core count (`func_020266f4`, at most 4) it writes the top half of that core's mFlags word (`func_020266dc`, `func_02026700`). The motion-table word 0xfff6 does the same through the gimmick's slot 20 (`func_02007cd4`, `func_020591cc`): core in bits 12 to 15, value in the low 12 bits. A new core starts at 0 (`func_02058910`). The value indexes the 8-byte rows of one table every gimmick points at (`func_02052f14` stores `0x0209c448` at gimmick+0xd4; `func_020591f0`, `func_02058ab8`): a sound or icon, the name text and the action text. The Trick press in normal play (`func_0203696c`) refuses with system sound 4 when the possessed core's row has no action (0 or 0x8000). The port skipped both the op and the word and fired every press, so a press the game refuses still sent `core ready`. The paper in `st03_game011_Expand` shows why both writers matter: its GAME START sets row 337 (no action) and the motion table its act plays later gives it a trick. The route's two late presses on `memo1` in `st03_game000_Expand` are refused in native too; a trick step now counts a refused press as taken, as the press happened. Which script classes are gimmicks is read from the class tree the `__sinit` functions build (`func_0201edc0`), with the class ids hashed from their names (`func_0201eee8`); the converter bakes which rows the scripts name that have no action into `soul.txt`. The row's name and action texts are not used: the port's core labels stay one per object.

Guard: `set_core_info` writes a gimmick's core info from op 0x9f and from the 0xfff6 word, and the Trick press refuses a core whose row has no action. `tests/unit/test_core_trick_info.c` checks both writers, that a human is left alone, the refusal and its sound, and a press that fires.

### 158. Ops 0x7f and 0x80 logged as not ported although native does nothing with them

Native's ops 0x7f and 0x80 (`func_02023fdc`) call `func_02015748` and `func_02015418` on the sound singleton `func_0201003c` builds (`func_02015864`, `func_020158c4`). Those reach slots 28 and 27 of its first member (`func_02016508`, vtable `0x020a3784`): `func_0201683c` and `func_02016828`, which call slots 30 and 29 of the player it holds at +0xc. `func_02016bec` makes that player one of two classes (`func_0201abcc`, `func_0201b170`), and both fill slots 29 and 30 with the empty `func_02016838` and `func_0201684c`. The game over master in `st14_game033_Expand` and the scripts of `st03_game012_Expand` and `st13_game000_Expand` send them around their sounds; in the retail game they change nothing. The port had no case and logged `op_not_ported`.

Guard: both ops are control ops of one word that step on. `tests/unit/test_sound_slot_ops.c` runs a script through them and checks nothing is logged as not ported.

### 159. The screen still never faded

Native's op 0xf6 (`func_02023fdc`) finds the first `uSpriteCaptureNormal` in the scene (`func_0202fb48` with `data_020cfd28`), the still that op 0xf3 makes, and writes the op's count to both halves of its fade (`func_02026838`, +0xca and +0xcc; the constructor `func_02063e90` sets both to -1). Its update (`func_02063ef8`) then counts the second down each frame, sets the still's alpha to left × 31 / total, and deletes the still when the count reaches 0. With no still the op does nothing. `st02_game036_Expand` fades its still over 30 frames before op 0xf5, and the last chapter's `st13_game021_Expand` over 180 frames before the demo it starts. The port had no case and logged `op_not_ported`, so the still stayed up at full strength until op 0xf5 or the scene ended.

Guard: `fade_the_screen_still` counts the still down from op 0xf6, sends its alpha in 31 steps and takes it down at 0. `tests/unit/test_screen_still_fade.c` checks the fade, that the alpha only falls, that the still goes away once, and that a fade with no still and a still with no fade change nothing.

### 160. Op 0xcd left the camera where it was

Native's op 0xcd (`func_02023fdc`) reads a speed word and calls `func_0200f958`, which takes the core Sissel is in (`func_02010194`) and pans to it with `func_0200f61c`, the pan op 0xce also ends in: the camera's left edge 128 pixels left of the core, and its top either from the camera band holding the core (`func_02012318`: the band's bottom less the 192-pixel view, not below 0) or, with no band, 96 pixels above the core, not below 0; the world offset at game+0xdd8 (`func_0200f9cc`) turns that into the port's world origin. Thirty scripts from `st01` to `st14` send it 66 times, 45 of them at 0x1000000, which is a cut, and the rest between 0x2000 and 0x100000. The port logged `op_not_ported`, so the camera stayed wherever a demo had left it. Three parts are not ported: when game+0xc bit 12 is set (cleared by `func_0200ef60`, tested by `func_0200f084` around the window at game+0xee0) native uses 96 pixels above the core even inside a band, and the port has no state for that window; the op also deletes every `uScrollBg` (`func_0202fad4` with `data_020cf93c`), which no script creates; and it clears game+8 bit 4 (`func_0200ee54`), whose reader is not identified.

Guard: `pan_to_the_soul_core` aims the pan from the soul's core. `tests/unit/test_pan_to_the_soul_core.c` checks the offsets, the band, the top of the room and that no soul core leaves the camera.

### 161. Every scene after the fifth ran with no read-ahead plan

The read-ahead plans are a port device with no native counterpart: the DS reads a scene's art from the cartridge in the loading mode `begin_script` starts, and the port instead reads textures in the background a few seconds before a paced replay first drew them. The player reads every script's `.ahead` file while it reads the chain, and `read_ahead_add_text` copied each line into one table of 1536 entries shared by the whole chain, stopping without a word when it was full. The game's chain holds 147 plans and 35,429 lines, so the table filled during the fifth plan read, `st01_game012_Expand`, and every scene from there to the last chapter read its art only when it was first drawn. In the Vita fast-forward run of build `30e5a251` that showed as the long stutter episodes from `st01_game012_Expand` on (`st06_game001_Expand`, `st01_game021_Expand`, `st02_demo003_Expand`, `st02_game004_Expand`, `st02_game010_Expand`): textures waited a mean 194 ms from request to upload, the player held the picture while each shown texture loaded, and the screen flipped as few as 1 to 15 times a second while the route went on.

Guard: the player keeps each plan's text, and `read_ahead_note_mark` reads only the beginning script's plan into the table at `script_begun`; the largest plan, `st03_game010_Expand`, has 1212 lines. `tests/unit/test_read_ahead_every_script.c` adds four plans that together overflow the table and checks that the last script still reads ahead, that a new script drops the earlier one's entries, and that a script with no plan reads nothing.

### 162. Read-ahead fetched a long animation's later pages too early to keep them

`read_ahead.py` scheduled every page of an actor's animation at the tick its motion table started, though a page is first drawn only after the frames before it have run. The runtime keeps a read-ahead texture wanted only until 30 ticks past its due tick, so the later pages of a long multi-page animation were read, fell to the cache's first eviction tier undrawn, and were taken by the next pages' reads before the animation reached them; they then loaded again as drawn textures while the picture held. In the Vita fast-forward run of build `30e5a251`, 696 textures loaded a second time within 2 seconds in the same script, among them each 9 MB page of `st01_killera_020` (pages 01 and 02 read at 143.1 s and again at 144.3 s).

Guard: the planner reads each frame's ticks from the `.anim` file and dates each page at the start tick plus the ticks of the frames before its first use. `tests/tools/test_read_ahead.py` (`test_each_page_of_a_long_animation_is_due_when_its_first_frame_is_shown`) checks a page after an empty frame, a tiled page, and a page used twice.

### 164. The ghost view pulled the camera to the soul's core when native leaves it alone

Native's ghost-view field update (`func_02032914`, the per-frame step of pGameMainSV, also run by pGameMainExchange through `func_0203447c`) centres the soul's core. It clamps the core to the room (`func_02033428`). While the core is more than 4 pixels from the view's centre (124 to 132 across, 92 to 100 down), it adds 4 pixels a tick to the camera (`func_020330c0`, `func_020330e0`). It does this only while the soul's core is not busy (`func_02058b64`) and the follow is not off (game+0xc bit 10, `CoreHoseiOff`). The tutorial view that `GameApp SoulVision On` opens (`func_0203de90`, pGameMainSVTutorial) never calls it, so the camera stays where the tutorial's script put it. The port ran the follow in every ghost view and on busy cores. In Chapter 1's tutorial the scene points the camera at the object it teaches, and the port then slid the camera back to Sissel's body. During a trick the port chased the moving object instead of holding still.

Guard: the soul marks a view opened by `SoulVision` as a tutorial view, and `follow_core_with_camera` follows nothing in it or while the core is busy. `tests/unit/test_ghost_view_camera_follow.c` checks four cases: a player's view follows the core; a tutorial view does not; a busy core does not; a player's view after a tutorial follows again.

### 165. The spirit's travel cloud churned twice as fast as the spirit

Root cause 138's cloud of blobs under the chain and under the travel beam re-rolled every second tick, as measured from the footage. The spirit's own body, wisps and lightning step every 4 ticks. On the Vita the user saw the cloud race ahead of the spirit it travels with. Native's tentacle drawing is still not read, so the spirit's body is the reference.

Guard: `soul_animation_step` in `soul_chain.c` is the one pace for every soul effect. The body, the wisps, the lightning and the chain and travel clouds all take their step from it, so no soul effect can run at its own rate. `tests/unit/test_soul_chain.c` (`test_the_cloud_keeps_pace_with_the_soul_body`) checks that the cloud holds for a whole step and changes on the next.

### 166. One drag could not carry the ghost through several cores

Root cause 69 read half of native's rule. `func_0205fa2c` does clear its drag flag (soul +0x7c bit 4) while the chain jumps (state 2). But when a pick connects, `func_0206001c` (state 1 with a target) moves the soul into the core and sets +0x7c bit 6. While bit 6 is set, `func_0205fa2c` skips the 16-pixel grab test and takes the stylus wherever it is. Only a release with no direction held clears the bit (`value == 0 && held == 0`); so does the soul's reset, `func_02060d5c`. So a held stylus carries on from each new core to the next one in reach, and only a fresh touch has to land within 16 pixels of the chain's end. The port dropped the drag at every arrival, so on the Vita a finger had to lift and find the tip again for each hop.

Guard: a chain jump sets `drag_carries_on`, which lets the held touch grab the tip at any distance. A release with no direction held, closing the view, and the swap link all clear it. Scripted holds (a tutorial's `MoveEnd`, demos, talks) still stop the chain, as before. `tests/unit/test_soul_touch_rules.c` checks two things: one long drag reaches every core in reach, and after a lift a touch away from the tip does not drag. The tutorial tests now check that the soul moved, not that it moved exactly one core.

### 167. Speech balloons popped up before their text

Native draws a balloon from graphics it already holds, so the bubble and its line appear on the ticks the balloon table gives: the bubble grows from tick 0 over 6 ticks, and the line (`bubble_m01_0050` and the rest, one texture per line) shows at tick 6. The port reads each line's texture only when it is first drawn. The art gate that holds the script for loading art (`shown_art_is_loading`) looked only at the room plates and the actors. So in Chapter 1 the bubble grew, sat empty while its line loaded, and the text came in late. The same gap let narration text lines and any shown overlay draw late.

Guard: a balloon's parts are not shown until every part of that balloon has loaded. `script_bridge_balloon_art_is_loading` asks for them from the moment the balloon opens and joins the art gate, so the game holds instead of showing an empty bubble. `scene_view_visible_art_is_loading` also counts shown overlays and text lines. Every balloon, "!" alert and narration line shares this path. `tests/unit/test_balloon_waits_for_its_text.c` checks that the bubble waits for its text, then shows on its own ticks, and that shown text and overlays hold the game while they load. The replay drives the machine without the bridge, so the replays are unchanged.

### 168. People-screen pictures left a hole until they loaded

The DS People screen (overlay 6; `func_ov006_020d6c20` draws it) shows the selected entry's portrait or place picture as soon as the cursor lands, because it reads its graphics in its own loading step. The port's People screen and its Illustrations tab read each large picture (portraits, place pictures, text cards, the illustrations themselves) only when first drawn, and drew nothing until it arrived. On the Vita the frame stood empty over the backdrop for a moment after every move, and longest on the Illustrations tab, whose pictures are the biggest.

Guard: while the screen is drawn, `pause_people_read_ahead` keeps the pictures and cards of the two entries on each side of the selection read ahead. Until the full picture is ready, the frame shows the entry's thumbnail from the resident thumbnail sheet, scaled up, so the frame never stands empty. `tests/unit/test_pause_people.c` (`test_pictures_around_the_selection_are_read_ahead`) checks that a neighbour's picture is ready without ever being drawn.

### 169. SELECT, the notebook pill, did nothing

Native opens its notebook (pGameSubDatabase, the People and Places database) from HUD button 3. `GameApp Button Database:On` registers it (`func_020135c8`), and pGameMainNormal's update (`func_0203696c`) pushes the database screen when the button is touched. The DS reads no hardware key for it. No script in the game sends `Database:On`, so the port's SELECT (mapped to that pill) only reached `pill_not_ported` and did nothing. The port's Record screen (`pause_people.c`, root cause 147) is the database's counterpart.

Guard: SELECT, or a press on the notebook pill when it shows, sets `record_is_wanted` whenever the player has the controls (any of the game's pills is on screen). The app then opens the pause menu straight on the Record screen, which fades in from black without the main menu, and CIRCLE there resumes the game. `tests/unit/test_select_wants_the_record.c` checks the request and its gate; `tests/unit/test_pause_menu.c` (`test_select_opens_straight_onto_the_record_and_circle_returns_to_the_game`) checks the direct opening and the way back.

### 170. The START button had no pause icon

The remaster's in-play pause button is one of the round HUD buttons on `cmn_button_02` (the sheet the port bakes as `hud_button_art`): the white pause bars on the navy disc at (0, 421), with its pressed look beside it at (80, 421). It sits in the same sheet as the hourglass, flame and notebook icons. The DS has no pause button, so the remaster is the reference. The port drew START as a plain pill holding only the START glyph, which did not read as the game's pause button.

Guard: the START pill is built like the hourglass pill, with the pause icon on the left and the START glyph on the right, from the same sheet cells. It keeps its place left of the hourglass. `tests/unit/test_script_ui_view.c` checks that the pause icon's cell is drawn at the top right while the pills show.

### 171. The pause menu did not say when or where the player is

Native has no clock on its pause screen. The story's times are shown only on the chapter cards: the remaster cards `cmn_ch_00_NN` read 7:02 PM for Chapter 1 through 5:10 AM for Chapter 17, and the final chapter's card has no time. The places are named only in the notebook (pGameSubDatabase): the `database.msg` keys `database_stageNN_YY_sub2_*` name a place for each stage and part, such as Junkyard for stage 1 and Pigeon Man's Office for stage 2. The port's pause menu showed neither, which left the player without the game's sense of the night passing.

Guard: `tools/assets/pause_clock.py` bakes the clock from `game.json` `pause.clock`. Each scene gets a time, moved on from its chapter's card time toward the next chapter's in story order. The scene lists come from the replay marks through `tools/clock_scenes.py`; the last chapter with a time runs on to `night_ends`. The script also bakes outlined glyphs for the digits, colon, AM and PM, and each place name, into the `pause_clock` sheet and `pause/clock.txt`. At run time `pause_clock.c` takes the time from each `script_begun` mark. It takes the place from the room's first plate when the menu opens (`stNN-YY` gives `stageNN_YY`, else `stageNN_00`, else the last place is kept). The menu draws both at the top left, laid out from the mockup the user supplied. A chapter with no times, the final chapter, shows only the place. `tests/unit/test_pause_clock.c` checks the twelve-hour text, the per-scene times and the place lookup. `tests/tools/test_pause_clock.py` checks the interpolation and the baked table. `tests/golden/pause_render.c` takes a `chapter:script:plate` argument so the clock can be rendered on the Mac.

### 172. Loading a chapter showed a black screen and the debug line

The DS loads a scene's files from the cartridge in a few frames behind a fade, so native has no loading screen. The port reads every table, script, animation and texture of a chain before it runs (`script_player`'s reading, animation and preloading phases), and on the Vita that takes seconds. During that time the screen was black apart from the debug HUD line ("loading N textures"). vitaGL also compiles each shader the first time a draw state is used, so the first sprite, beam, triangle batch and filled rect of a run each stalled a frame once the scene had started.

Guard: while the script player preloads, `loading_screen.c` draws the pause backdrop with the pause menu's black floor and red line, "Now Loading" in the menu's bar text, and a rounded track with a red fill. These are baked from the pause widget sheet (`loading_track` and `loading_fill`) in the game's own UI style. The fill follows `script_player_load_counts`: reading is the first fifth, animations the next three tenths, and textures the last half. The fill eases forward and never goes back. Each loading frame also draws every renderer primitive once, one pixel in size and fully clear, so the shaders are compiled behind the bar. The debug HUD still shows when debug controls are on. `tests/unit/test_loading_screen.c` checks the stage shares, that the fill never goes back, that the drawn fill grows, and that the warm-up draws all five primitives unseen.
Lessons from routing the rest of Chapter 16 (`st14_game021_Expand`, `st14_game040_Expand`, `st14_game032_Expand` and `st14_game033_Expand`, route steps 1486 to 1639), Chapter 17 and the last chapter (steps 1640 to 1718, written by workers on a frozen binary and spliced unchanged):

- In `st14_game021_Expand` the lever and the torpedo belong to the short window after Sith's demo: flag 0x6 is set just before its Trick Time, `t_set_up` slides the torpedo under the lift for 280 ticks, and `setup_end` then starts `st14_01`, which fires it. The lever's trick lowers the lift with the lever and the capsule on it, which brings the torpedo's tail core (slot 1) into reach. Waiting for `st14_02` is the failure branch, after the torpedo has gone.
- The conveyor capsule is `capcel`; the wastebasket's trick, the basketball and the emergency light `gim_pato2` are reach-gated (`reach:`), pace instant.
- In the torpedo the joint is `arm-button`, the striker `sinkan`, the detonator `red-button`; the second gyroscope's stop turns the torpedo by itself and the joint's trick ends the scene.
- A machine that moves cores while time runs is routed as cycles of `ghost_on` on `free & no_talk & reach:<next>`, `move`, `ghost_off`, all pace instant (the engine room's gears and pistons in `st14_game032_Expand` and `st14_game033_Expand`). The starter is `piston_l` and each trick runs one cycle.
- The steam valve that drops the glove is `valve_b` (it docks to `kata_p`), not `handle`; the fuse box is `haiden`, the switch `lver`.
- `st14_game033_Expand` is the same room turned on its side, so every core has new coordinates. Missile starts in the debris `dust3`. A touch between two fans or two pipes links them, so he must reach a fan or pipe from a core of another shape: `dust2`, `bonbe`, `gear1` to the middle fan `fan_r`, and from `pipe1` back over `fan_r` to `pipe2`. The swaps are `fan_r` with `fan_c`, then with `fan_l`, then (from `piston_c` to `fan_r` at the top) with `fan_mini`; `pipe4` with `pipe1` and `pipe2` with `pipe3`.
- The first pipe swap closes the view for a hint (`change_end`), so a `ghost_on` follows it; the fan swaps and the second pipe swap leave it open.
- Sissel reaches the debris only once Lynne has carried the flashlight near it (`ghost_off`, then `ghost_on` on `reach:dust3`). The hook `c_hook` is tricked twice, the second time on flag 0xf (Lynne is on it), and the door is `hatch`.
- Ops 0xfd (the checkpoints of `st14_game021_Expand`, `st14_game040_Expand` and `st13_game021_Expand`), 0x9f, 0xb6, 0xb7, 0xcd, 0xaf, 0x7f, 0x80 and 0xf6 log as not ported in these scenes. Without op 0xfd a game over in the torpedo restarts at `st14_game031_Expand`.

### 173. The stall breaker outlived the routes it carried

The stall breaker (stuck pushes, the scene timeout, the behind-the-story move and the game-over move-on) and the L+R and `skip` pushes were a stopgap while routes were incomplete. Native has none of them: a scene waits for its player as long as it takes. Once the honest route played the whole game (1718 steps, 0 failed, to GOTO TITLE), each piece only hid a missing step and kept code in the player's build that no player needs.

The breaker is gone from `script_machine.c`, `script_soul.c`, `script_player.c` and the replay, with `scenes.txt` and its scene order in `read_ahead.py`; the honest replay's marks and cues are byte for byte what they were before. The mode byte's values are named once in `script_machine.h` (`SCRIPT_MODE_LOADING`, `SCRIPT_MODE_DEMO` for `pGameMainDemoMode`, `SCRIPT_MODE_NORMAL` for `pGameMainNormalMode`, `SCRIPT_MODE_NARRATION`, `SCRIPT_MODE_STAFF_ROLL`), where the files had kept their own copies under different names (value 2 was "normal" in one file and "demo" in another). The player's build is the standard build and the device loop's is the debug build (`DEBUG=1`); the fps counter, the unlock switch, the debug controls and the commands file are debug only, with no flags of their own.

Re-baking the route also showed that the route pacing does not reproduce: `route_pacing.py` never shortens a hold it once lengthened, so a fresh run from an empty pacing settles elsewhere (1330 steps done, 5 failed). The settled pacing is now kept in `tools/assets/game_pacing.txt` and seeds the next run while the route is unchanged.

Guard: `make route-check` fails if route steps are done out of order, more steps are passed over than the story passes over, or the replay never reaches GOTO. `tests/unit/test_scene_retry.c` checks that auto-play never moves a waiting scene on. `tests/tools/test_route_pacing.py` (`KeptPacingTest`) checks the seeding and keeping. `make test` compiles the app without `DEBUG_BUILD` as well (`standard_build_compiles`), and `tests/tools/test_no_session_paths.py` fails a source that reads a machine or session folder. Root causes 77 and 80 describe the breaker and are superseded.

### 174. The port had no title screen

The DS reaches its title (pGameTitle, overlay 5) from boot and from the story's end. `func_02031e30`, which starts each scene, catches two scene names that are not files: `GOTO TITLE` pushes pLogo (the boot logos, which lead to the title) and `GOTO GAMETITLE` pushes pGameTitle. Only the last ending script (`st15_demo120_Expand`) changes to `GOTO TITLE`. The title's update `func_ov005_020d3b08` reads the save, puts the cursor on Continue when one exists, and fades in. `func_ov005_020d443c` builds five buttons through `func_ov005_020d41dc`, whose tables at `0x020d4738`-`0x020d4768` place them on the bottom screen. With a save they are Chapter Select, Continue, the trash can, Language, and a notebook button that shows only when the save's cleared byte is set. Without a save only a start button and Language show. `func_ov005_020d43c4` takes a press or a tap, and `func_ov005_020d40dc` maps the choice. 0 is pGameMain (a new game) or, with a save, pGameMainLoad (chapter select). 1 is pGameMainLoadContinue, 2 pGameMainFormat (erase), 3 the `LANGUAGE SELECT` scene and 4 pGameSubDatabaseAllOpen (the notebook with every entry). A pick that starts play fades to white (`uWhiteInMB`); erase and the notebook fade to black. The cleared byte is `game+0x103d` (`func_0203cbfc`, `func_0203cc08`). The save writer `func_0203c6a8` sets it in the ending's save (`func_0203c570`) and keeps it in every later save.

The port had none of this. It continued the newest save at boot. The pause menu's Title entry replayed the opening, and `GOTO TITLE` only ended the chain on the "End of the chapter" line. Three ways in, each doing something different, was the class: a screen the game reaches from several places needs one way in that all of them share.

Guard: `title_menu.c` is the title. `viewer_app.c` opens it through one `open_title` from boot, from the pause menu's Title entry and from `script_player_takes_title_request`. The script machine raises that request when a scene change names `GOTO TITLE` or `GOTO GAMETITLE`. The debug commands file's `play_script` closes it, so device runs still start the opening directly. The buttons follow the native set and places, scaled under the remaster's own title art: `load_back` (already the `pause_backdrop`) and the `title_up` logo, baked as `title_logo`. The DS's orange and blue chamfered buttons and its trash can are drawn in the pause widget sheet (`title_button`, `title_icon`, `trash`). The notebook button uses the remaster's notebook icon from `hud_button_art`. The port adds New Game, which asks first over a save, and Options. Chapter Select, Options, Language (the Options language list, which returns to the title when it closes) and the notebook (the People screen with every note) reuse the pause screens. Continue, New Game and a chapter fade to white as the DS does. Erase asks first and leaves only New Game, Options and Language. A save carries `game_cleared`. `game_save_mark_cleared` sets it in every slot when the story reaches `GOTO TITLE`, and `game_save_store_as_newest` keeps it, as `func_0203c6a8` does. The pause and title menus share `menu_input.c` (the stick press and the held-direction repeat) and `pause_draw_floor`, which the loading screen uses too. `tests/unit/test_title_menu.c` checks which buttons show with and without a save, where the cursor starts and moves, the asks before erase and New Game, Chapter Select, Language and Options, taps and the fades. `tests/unit/test_goto_title.c` checks both scene names and that a missing scene does not open the title. `tests/unit/test_game_save.c` checks the cleared mark. `tests/integration/test_viewer_app.c` checks the boot into the title, Continue over a save, the fallback without title art and `play_script`. `tools/pause_render.py --title none|save|cleared` draws it on the Mac.

### 175. Sissel's red view overlays drew in every ghost's view

The ghost view's entry spokes, its fine lines and the sweeping line were made for Sissel and hard-coded red in `soul_transition.c`, and the view drew them for every ghost. Missile's and the boss's views then added their own scanlines and sweep in `scene_view.c` on top (root cause 66), so Missile's green world carried two sweeping lines, one of them red. The footage of Missile's views (24585 to 24600 s) shows one sweep, green, moving down at the same pace as Sissel's, and the switch to Missile plays green spokes (24599 s) where Sissel's are red. Native picks each ghost's art by the active ghost (`func_0201132c` for Missile, `func_020328e4` for the boss, as in `func_02059cac` and `func_02052a7c`); the overlays' own drawing is not read.

Guard: one set of overlays, drawn by `soul_transition.c` for the ghost the view reports. Its colours go through `ghost_tint_turned` (`ghost_tint.c`), the same turn the soul-view object tints use: Sissel's red becomes green for Missile and blue for the boss. Each ghost's fine-line texture is built from Sissel's rows turned the same way. `scene_view.c` no longer draws its own scanlines or sweep. `tests/unit/test_soul_transition.c` checks that Missile's spokes, lines and sweep have no red in them and that each ghost gets its own line texture.

### 176. A stick resting off centre read as a push

The ghost view pans with the right stick (`pan_the_ghost_view`), and nothing else moves the camera there while the soul rests. The stick was read against a fixed centre of 128 with a 0.25 dead zone, so a stick that rests further off centre than that, or a pad read that failed and left the sample unset, pushed the camera to one side every tick. The user saw the camera pan left on its own in the ghost view. The DS has no stick, so there is no native reading to follow.

Guard: `stick_aim_read` learns each stick's rest. Once a stick has held still (within 3 raw units) for 60 frames anywhere inside 0.45 of the middle, that reading becomes its centre, and the dead zone is measured from there; a held push past 0.45 is never taken for the rest. `input.c` reads both sticks through it and treats a failed pad read as both sticks at rest. `tests/unit/test_stick_aim.c` checks that a stick resting at 80 stops pushing once it settles and still reads full pushes both ways, and that a held full push stays a push. The device reading behind the user's report was not taken, so this is not yet confirmed on the Vita.

### 177. The character switch slid the old flame across and made no sound

Native's ghost view switch (`func_02032914`, pGameMainSV's update) plays system sound 0x1f and sets the main state to pGameMainSV for Sissel or pGameMainExchange for Missile (`func_0201192c` with 4 or 0x16). That restarts the view: its start (`func_020326a4`) removes the soul object and makes a new one on the active ghost's core (`func_0205e8a8`), and plays the entry effect again. The footage of the junkyard (24597 to 24600 s) shows each switch replay the spokes in the new ghost's colour, with the flame appearing on the new core. The port only changed the active ghost, so the soul's flame eased across the screen from Sissel's core to Missile's, with no sound and no entry.

Guard: `switch_character` in `script_soul.c` plays sound 31, and in the ghost view rests the chain on the new core, drops any aim, and replays the entry (the view's entering effect and its 33-tick hold, after which the core mark shows as on any entry). `tests/unit/test_character_switch.c` checks the sound, the entry and that the flame is on Missile's core the tick after the switch. Not ported: the `SV_START` that `func_020326a4` sends again when the new state is not of the class at `data_020cdd3c`, and the switch native also offers in normal play (`func_02035078`, pGameMainNormalMode: sound 0x1f, a pan to the new core with `func_0200f9e8`, then a new player mark through `func_02011690`); the port shows the switch buttons only in the ghost view.

### 178. A sound the app plays itself was never baked

The converter bakes the sounds the scripts name plus a hand-kept list of the kinds the app plays on its own (`NATIVE_SOUND_KINDS`). Adding the switch's sound 31 to the app without adding it to that list would have played nothing: the sound table had no row for it. The list was a second copy of facts that live in the C code.

Guard: `tests/tools/test_native_sound_kinds.py` reads every `*SOUND*` number defined in `src/app/*.c` and fails when one is not in `NATIVE_SOUND_KINDS`, leaving out 0 and 0x21, which native's player (`func_0202a0c0`) plays as silence.

### 179. Missile's drag was purple

Root cause 67 marked the soul beam `ghost_variants`, so Missile's view drew `soul_beam-missile`, the beam turned 120 degrees in hue. The turn assumes soul red art, but the remaster's only tentacle art (`soultentacle_iml4`) is cyan, so the turn made it purple. The footage draws Missile's drag the same as Sissel's: a cyan and white strip over the blue cloud. The DS gives each ghost its own class (`func_02059cac`), but the remaster ships one texture for all three, and the port follows the remaster's art.

Guard: the soul beam is no longer marked, and every view draws `soul_beam`. The converter's hue turn (`apply_hue_turn`) refuses art that is not soul red (red at least twice green and blue, weighted by alpha), so a later mark on other art fails the bake. `tests/tools/test_texture_encoder.py` checks that cyan art is refused.

### 180. The title's picks were silent and left at once

Native pGameTitle (`func_ov005_020d3b08`) plays sound kind 1 on every cursor move and 0x1a on every pick. A pick that starts play flashes the screen white (`uWhiteInMB` over byte +0x45, 4 frames), holds for byte +0x44 ticks (120, about 2 s), then fades to black over 60 frames (`uFadeOutMB`) before `func_ov005_020d40dc` maps the pick. The constructor `func_02041990` sets both bytes. The port played no sound and faded to white over 16 ticks. The kind-to-sound table lives in the scripts' `audio_names.txt`, which only loads when a script plays, so no screen outside a script could play a kind at all.

Guard: `system_sounds.c` reads the sound rows of `audio_names.txt` at boot and plays a kind by name, for any screen outside a script. The title plays kind 1 on a direction press and 0x1a on every pick, and a pick that leaves holds in `TITLE_PHASE_HOLDING` (the 4-tick white flash, then the hold) before `TITLE_PHASE_HIDING` fades to black. Kinds 1 and 26 are in `NATIVE_SOUND_KINDS`, which `tests/tools/test_native_sound_kinds.py` checks against the app's defines. `tests/unit/test_title_menu.c` checks the sounds, the flash, the hold, the black fade and that the request comes only after it; `tests/unit/test_system_sounds.c` checks the lookup. Not ported: the label animation 0x1f and `func_0202fad4`, which are unread. The pressed button look is root cause 198.

### 181. A pan slid back to the core

The camera returned to the soul on its own after the player panned it, in both views. In the ghost view the right stick pans while the left stick aims, but the same tick's tip follow (`follow_point_in_ghost_view`, box 32 to 224) pulled the camera back toward the tip, so a pan while aiming crept back left. In the living world the follow (`follow_core_in_normal_play`) resumed the tick after a d-pad, stick or touch pan and brought the core back inside 16 to 240. It was one of two causes of the drift; the other was the stick itself (root cause 182). Native's normal mode (`func_02035078`) scrolls the camera from the d-pad and a touch fling (`func_020330e0`, `func_020330c0`); what pulls it back afterwards is not traced, so the port keeps its own rule.

Guard: a pan holds. In the ghost view the tip follow and the core follow both wait while `view_was_panned` is set, which only aiming the soul with the left stick or a touch clears. In the living world `free_camera_holds` keeps the camera still after a pan until the core it follows moves more than a pixel or the ghost view opens. The right stick's raw range and learned rest are logged once a second (`right_stick` in the run log) for root cause 176. The right stick also pans the living world: `move_free_camera` takes it when no direction is held, under the same rules as the d-pad. `tests/unit/test_script_input.c` checks that a living-world pan stays until the core moves, that the right stick pans the living world, and that a ghost-view pan while aiming is not pulled back.

### 182. A stick resting far off centre still pushed

Root cause 176 learned a stick's rest only within 0.45 of the middle. The `right_stick` log on the user's Vita read raw x 68 to 74 and y 186 to 200 with the stick untouched, drifting slowly: about 0.5 to 0.7 of full travel left and down. That is past 0.45, so the learner kept the 128 centre and the ghost view panned left and down on its own. The DS has no stick, so nothing native governs this.

Guard: `stick_aim_read` learns any rest that holds within 3 raw units for 60 frames and lies short of the rim (0.85 of full travel), so a held full push is still a push. Each side of a learned rest is scaled by the room left to the rim, so a far-off rest still reaches full strength both ways. `tests/unit/test_stick_aim.c` settles a stick at this Vita's reading and checks that it rests and still reaches full left, right and down.

### 183. Seventeen of the game's twenty cutscene movies froze the screen

In Chapter 18's st13_game020 the meteorite never struck Yomiel: the screen held still for 20 s, with the meteorite's sounds playing, until his body lay on the ground. The script stills the screen (op 0xf3), asks for a movie by class (op 0xec, class id 0x14f605fb), times its sounds, then lets the still go (op 0xf5). Native op 0xec (case in `func_02023fdc`) finds the class by the hash of its name in the class list (`func_0201ee6c` over `data_020bbbb4`) and pushes it on the main state machine (`func_02022f2c`); op 0xed waits while it runs. The hash is `func_0201eee8`, an xorshift (`func_01ff80b8`) reseeded from each pair of name characters. The port had a movie row for three class ids only, entered by hand with their pack keys, so the other seventeen ids that the scripts name (the meteorite three, the rollbacks, the track demo and the seven submarine scenes) logged `movie_of_class_not_baked` and showed whatever the still held.

Guard: `class_movies.py` ports the hash, and `game.json` names each movie row by its native class (`pGameMainMovieMeteor` and so on) and by its remaster file (`st13demo01_1440x1080`, read from `natives/android/streaming/gui/ui010500/tex/<file>.mov.1.android`). The converter fills the class id from the name, and the bake fails when any baked script asks for a class with no row (`check_every_class_has_a_movie`). The class names come from hashing every string in ARM9. The remaster files were matched by what they show; the seven submarine ones (st14demo01 to 06 and 05b) are matched by content alone, so check those against footage when it exists. Twelve of the remaster's movies are 4:3; the movie draw now keeps each frame's shape between black bars instead of stretching it. `tests/tools/test_class_movies.py` checks the hash on four known ids, the class id fill, the missing-class report and that no class or file is listed twice. `tests/unit/test_movie_waits_for_frames.c` checks a 4:3 movie's place on screen. Not read: whether native's movie states run the script on while they play; the port runs it alongside, as op 0xed implies, so the 30 s meteorite movie outlasts its 20 s still and ends over the next room.

### 184. The loading cat leapt while files loaded

The loading screen's walk advanced by elapsed time with no limit. While a chain loads, the app reads one script file per frame, and the Vita's log shows each read taking 40 to 90 ms, so the screen drew 12 to 15 frames a second and the cat skipped several drawings at once. The pause and title menus already cap their catch-up (`PAUSE_TICKS_PER_UPDATE_MOST`); the loading screen was the one clock-driven animation without a cap.

Guard: `walk_on` advances at most two walk ticks (one drawing) per frame and drops the time left over, so a long frame slows the cat instead of moving it on. The walk also runs at 40 ticks a second, two thirds of the game's pace, as the user asked. `tests/unit/test_loading_screen.c` checks the pace and that a half-second frame moves the cat by one drawing at most. The reads themselves moved off the frame in root cause 186.

### 185. Uploads from a clean copy of the repo sent the whole set again

`upload-assets` and `upload-eboot` run through the vitacompanion MCP seemed to hang. Builds run from `git archive HEAD` copies so that other sessions' uncommitted work stays out, but the upload record lived in `srcVita/build/uploaded_assets.json`, which such a copy lacks. With no record, the uploader treated all 30,796 files as new, each costing about a second, and a call gave no output until it ended hours later. `upload-eboot` run right after a quit failed with `550 File not found` while the app was still tearing down. Every file was also read back twice, and each new file also tried to delete two old-layout copies that cannot exist, each miss costing about 140 ms on a texture bucket.

Guard: the record now sits beside the asset set (its real path plus `.uploaded.json`), so every checkout that links `extracted` shares it, and the old build-folder record is read when it is missing. A refused write is tried up to 10 times, a second apart. One hash-checked read-back is followed by a size check after the rename. Old-layout copies are removed only for files uploaded before, and progress is printed every 100 files. `tests/tools/test_vita_loop.py` checks the shared and taken-over record, the retried write, the single read-back and the skipped deletes.

### 186. Loading a chain held each frame for one file

Native reads each scene's script from the ROM archive when the scene starts. The port reads the whole chain up front: the tables, then each script's `.cues`, `.lmg`, `.motions` and `.ahead`, 497 steps for the game's 163 scripts. `read_next_file` ran one step per frame on the main thread, so the loading screen drew only as fast as the files came: 8 to 20 fps for 30 s on the Vita, with a 1.65 s frame for `talk.txt` (0.43 s read, 1.2 s parse) and a 0.76 s frame for the six tables of step 6. Opening a file on the Vita's card takes about 50 ms whatever its size: the median chain read was 52 ms, even for files under 5 KB. About 140 steps also opened a file the bake never wrote (106 scripts have no `.cues`, 37 no `.motions`), at about 60 ms each. The ten seconds the preload waits for sounds and music counted from `script_player_play`, so a long read used them up. Chapter 18's load logged `audio_wait_gave_up` 19 ms into the preload and started its first scene with 128 sounds still loading.

Guard: the bake packs `scripts/` into `scripts/scripts.pack` (`tools/assets/script_pack.py`, run by every step that writes there), and the app reads the whole chain from that one file. `step_the_chain_read` runs the read on a background task (`background_task.h`, a thread on core 0 on the Vita, a holdable fake in tests). The frame only polls the task, and `begin_loading_animations` still runs on the main thread once the task ends. Stopping or replaying the player cancels the read and waits for it before freeing anything. Without a usable pack, the task lists the scripts folder once (`file_list_folder`) and never opens a name the listing lacks. The audio wait now counts from `begin_loading_animations`. On the Vita the game's chain now loads in 3.6 s (1.6 s for the 15 MB pack) at 55 to 60 fps, against 26 s for loose files off the frame. The pack stays in memory while the chain runs, so a scene's `.anims` no longer touches the card. `tests/integration/test_viewer_app.c` checks:
- no chain file is read on the frame while the task is held;
- a chain loads from its pack alone, without the folder listing;
- a broken pack falls back to the loose files;
- no missing file under `scripts/` is opened;
- loading still works with no listing;
- a stop during the read waits for it;
- a 20 s read still leaves the audio its full wait.

`tests/unit/test_script_pack.c` and `tests/tools/test_script_pack.py` check the pack format, what it leaves out, that an unchanged folder leaves it alone, and that the read-ahead step repacks.

### 187. Every scene began with a frame spent parsing its animations

At each `script_begun`, `load_script_animations` unpacked the scene's `.anims` bundle and parsed every animation in it on the main thread. In the fast-forward run that frame took 34 to 173 ms: 7 to 15 ms to unpack 260 to 527 KB of text, the parse, and for the largest bundles a run of evictions. It happened at the start of each of the game's 147 scenes with a bundle. Native has no such step: its animations are read from the ROM as an actor needs them.

Guard: the read-ahead step records which scene the paced replay played after each one, as a `next <script>` line in its `.ahead` (`next_scripts_from_cues` in `read_ahead.py`, `read_ahead_next_script` in the app). Once a scene's own bundle is in, the player starts the next scene's unpack and parse on the background task (`animation_bundle_prepare`, which only parses and never touches the library). When that scene begins, `animation_library_add_prepared` only inserts the parsed animations, freeing any already loaded. A scene that differs from the plan drops the prepared bundle and loads its own as before, and so does a scene the replay never reaches: the whole-game paced replay leaves a `next` in 104 of the 147 plans. A `next` skips scripts with no animations, such as `chapter` and `phone`: a first cut named them, so the 29 scenes after a chapter card or a telephone ride were never prepared. Stopping the player waits for the task and frees what it parsed. `tests/unit/test_animation_bundle.c` checks that a prepared bundle is parsed apart from the library and added later, that a duplicate is freed, and that a dropped bundle frees everything. `tests/integration/test_viewer_app.c` checks the hand-off, a scene the plan did not expect, and a stop mid-parse. `tests/unit/test_read_ahead.c` and `tests/tools/test_read_ahead.py` check the `next` line.

### 188. The debug build opened the commands file on the frame

Fast-forward runs on the debug build showed 50 to 320 ms frames whose update, draw and present together took a few milliseconds. `run_main_loop` looked for `commands.txt` every 6 frames with `file_read_whole` on the main thread. A failed open on the Vita's card took a median of 15 ms and up to 197 ms while textures streamed (1,021 opens over 10 ms in 300 s), and that time fell into the next frame. The player's build has no commands file, so the stutter was the harness's own, but it hid real stutters and cost the debug runs speed.

Guard: `command_reader.c` looks for the file on a thread of its own, ten times a second, and hands the text over under a mutex. The frame only takes what is waiting. `tests/tools/test_main_loop_io.py` fails if `run_main_loop`, `run_pending_commands`, `run_one_command` or `publish_frame_stats` calls a file function, and a host fake keeps the synchronous read for `tests/integration/test_main_loop.c`.

### 189. The loading screen's cat was lost against the backdrop

The DS has no loading screen (root cause 172), so this is the port's own screen. The cat that walks along its floor is drawn from the pause menu's frames, which are pure black (0, 0, 0). On the pause screen it sits in the database spotlight's circle: an inner disc of pure red at half opacity, a (99, 0, 0) ring and a (74, 0, 0) ring. On the loading screen it walked straight over the dark red backdrop. The background around its outline gave a median contrast of 1.06:1, so the black cat nearly vanished into the black and dark red behind it.

The class: anything drawn over the red and black backdrop (the pause, title and loading screens) gets its contrast from what is behind it, so it has to be measured, not judged by eye. WCAG's 3:1 for graphics can't be had with the circle's colours at low opacity. The two outer reds give a black shape at most 1.53:1 and 1.29:1 even fully opaque, and the inner red reaches 3:1 only near full opacity. The user chose a subtle glow anyway. The cat carries no information, which WCAG treats as decorative, and the "Now Loading" text and the bar say what is happening.

Guard: `loading_screen.c` draws a glow over the bottom 256 px above the floor, the cat's full height, in 4 px strips. It runs from the circle's inner red at the floor through its middle ring to its outer ring, at 35% opacity at the floor falling to none at the top. Measured from Mac renders across five points of the walk, the background 1 to 3 px from the cat's outline went from a median 1.06:1 to 1.22:1 (90th percentile 1.56:1 to 1.73:1). `tests/unit/test_loading_screen.c` checks that the strips cover the 256 px from the floor up, full width, red only, from the inner red down to the outer ring's red, never above 35% opacity, each fainter than the one below and drawn after the backdrop. `tools/pause_render.py --loading` draws the loading screen on the Mac, so its contrast can be measured from real pixels.

### 190. A new chapter's autosave stopped the frame

At each chapter start `take_checkpoint` stored the autosave and the chapter's slot on the main thread: `game_save_store_as_newest` read both save files to find the newest serial, then wrote the autosave, and `game_save_store_chapter` wrote `chapter_NN.txt`. With textures streaming, the card made those four opens cost 61 to 210 ms in one frame. Settings saves from Options did the same. Native backup writes go through the SDK's card requests (`func_0206a270` moves each page through its own buffer), off the game's frame.

The class: every save write. Guard: `game_save.c` and `game_settings.c` copy what they store and post it to `work_queue.c`, one thread that runs posted work in order; the newest-serial lookup runs there too. Every load, `game_save_erase_all` and `game_settings_load` wait for the queue first, and stopping the app waits for it, so a read never sees a half-written save. A full queue runs the write in place after waiting. `tests/unit/test_game_save.c` holds the host queue and checks that the stores leave the card untouched until a load waits for them, with the serial still counting up.

### 191. Large textures swizzled on the frame

In the fast-forward run of build `efba1a42-ff`, 187 texture uploads took over 16 ms of the main thread and the largest 42 ms. The time scaled with size, about 4.5 ms a MB. `glCompressedTexImage2D` has vitaGL swizzle the DXT blocks into the GPU's tiled order before it returns: a CPU swizzle for pages whose sides are not powers of two (a 2048 x 1772 page took 42 ms), and a copy into mapped memory and a GPU transfer for the rest (a 2048 x 2048 page, 22 ms). At normal speed each was one or two dropped frames as a scene's art streamed in. Native has no such step: the ROM holds DS art in the tile format the 2D engines draw from, so nothing is reordered at load.

The class: every compressed texture big enough to cost a frame. Guard: `texture_cache.c` sends a DXT texture of 512 KB or more (`TEXTURE_CACHE_FILL_OFF_FRAME_LEAST_BYTES`) through `renderer_texture_create_compressed_empty`, which only allocates it, and `renderer_texture_fill_compressed`, which calls vitaGL's exported `SwizzleTexData128Bpp` or `SwizzleTexData64Bpp` with the arguments vitaGL itself passes, on the work queue's thread. The texture stays loading, so it is neither drawn nor evicted, until the next frame finds it filled. Clearing the cache and loading a texture at once wait for the fills. `tests/unit/test_texture_cache.c` holds the queue and checks that a large page is created empty, kept loading and out of drawing until filled, that small textures still load at once, and that a clear waits; `tests/unit/test_vita_renderer.c` checks the swizzle arguments against vitaGL's for a page whose height is not a power of two and for DXT1.

### 192. The first scene of a chain parsed its animations on the frame

Root cause 187 prepared each scene's animations while the scene before it played, but nothing played before a chain's first scene. A chain starts at a root with no animations of its own (`route`, `st01-st01_root`, or the root a save resumes at), and the root starts the first scene on its first ticks, so that scene's bundle was unpacked and parsed on the first frame of play: 80 ms for `st01-st01_game000_Expand` in the fast-forward run of build `83f57590-ff`, right as the loading screen gave way.

Guard: `next_scripts_from_cues` in `read_ahead.py` also gives every script without animations the first scene with animations the paced replay played after it, and the step writes an `.ahead` holding only that `next` line for each of them (163 plans in the game set, up from 147, and no `.anims` for them). When the chain has loaded, `prepare_the_first_scene` in `script_player.c` starts the entry's own bundle if it has one, or else the one its `.ahead` names, on the background task while the loading screen finishes. One root serves several chapters, so one `next` per root guessed wrong on resume: Continue into a Chapter 13 save at `st05-st05_root` prepared `st05_game000`, the root started `st05_game001`, and the first frame of play took 206 ms (player build `3dc8ec01`). The paced replay's cues now carry a `chapter_mark N` line before each `script_begun`, `next_scripts_by_chapter_mark` gives each root a `next_for <mark> <scene>` line for every chapter mark it began at (`st05-st05_root` began at 3, 12 and 13), and the player picks the line for the chapter mark the save or chain starts with, falling back to `next`. A guess that still misses drops and loads as before. `tests/integration/test_viewer_app.c` enters a chain at a root and checks that the first scene's animations come in prepared, and `tests/tools/test_read_ahead.py` checks the roots' `next` and `next_for` lines; `tests/unit/test_read_ahead.c` checks that the chapter mark picks its line and that a `next_for` line never reads as a `next`.

### 193. Pressing Start froze the screen while shaders compiled

The loading screen's warm-up (root cause 172's guard) draws every renderer primitive once, clear, so vitaGL compiles each shader before play. Its first call came on the loading screen's first frame, right after the title closed, and that frame's draw took 139 ms (180 ms in all) in the run of build `83f57590-ff`: the screen stopped just as the player pressed Start. vitaGL compiles a shader the first time a draw state is used, so the compile costs a frame wherever it happens; it needs a frame nobody sees.

Guard: while the title screen's black cover is fully up (`title_menu_cover_alpha` at 1, as it is while the title waits for its art), `viewer_app_draw` runs the same warm-up, so the compiles happen behind black before the title shows. The loading screen keeps its call for runs that skip the title. `tests/integration/test_viewer_app.c` checks that the warm-up's five clear draws come while the title is still covered.

### 194. A 16 MB effect page could not be read once the heap broke up

In the run of build `ebb1558a-ff`, `fx_st03_mansui_00_00` (a 4096 x 4096 effect page that unpacks to 16 MB) loaded three times between 803 s and 908 s, then from 1250 s every read failed with `reason=heap`: 2,431 retries while the heap had 38 MB free, too broken up for 16 MB in one block. The texture stayed loading, so fast forward fell to one tick a frame for the rest of the chapter and, at normal speed, the effect would not have been drawn. The game set has 33 such pages (`fx_st13_kaze_00_*` and others), all 16 MB, and every other texture is 8 MB or less.

The class: any read that needs one block larger than a long-running heap can promise. Guard: `read_buffer.c` sets aside one 16 MB block when the reader starts. An unpack over 4 MB takes it if it is free and asks the heap only if it is not, and every holder of a read hands it back through `read_buffer_release` (the texture cache after its upload, the fill job after its swizzle). The log line `reader large_buffer` says whether the block was set aside. The first cut took the block out of the 112 MB heap, which left the rest 16 MB smaller: in the run of build `c55022d7-ff` the effect page loaded, but at 1236 s the 1.9 MB room picture `st03-08_0` could not be read with 21 MB nominally free. The heap now grows by the block (`MEMORY_HEAP_BYTES` is `MEMORY_GENERAL_HEAP_BYTES` plus 16 MB). vitaGL's pools did not shrink for it: the texture budget measured at start stayed 159 MB, so the 92 MB margin it keeps free is unchanged. `tests/unit/test_memory_plan.c` checks the general heap keeps its 112 MB. `tests/unit/test_read_buffer.c` caps the fake heap's largest block below 16 MB and checks that a large unpack still gets one, that a second waits for the heap while the first holds it, and that small reads never take it.

### 195. The Minutes Before Death label and hourglass stayed up over the red swirl

Reported from the device: when the game leaps back to four minutes before a death, the side label and the hourglass stayed drawn over the red swirling backdrop. Root cause 127 hid the pills and tap balloons under the swirl, but the side HUD was gated only on its flag (0x1020), a movie and the screen fade. Native keeps the swirl as bit 15 of `game+8` (`func_0200f084`, ops 0xdf and 0xe0), and every world draw asks `func_0203435c` before drawing; where native draws the fate hourglass is not traced, so the side HUD follows that world rule: it is part of the play screen the swirl replaces.

The class: anything that sets the swirl, the ops and the time leap alike, since all of them go through `script_machine_set_swirl`. The film strip and narration do not hide the side HUD; no native rule for them is traced, so they keep it as before.

Guard: the HUD view carries `swirl_is_on`, and `draw_side_hud` draws nothing while it is set. `tests/unit/test_script_ui_view.c` draws the side HUD with the swirl on and off; `tests/unit/test_ui_off_hidden_picture.c` checks the view reports the swirl and not the film strip.

### 196. Tap balloons answered SQUARE and TRIANGLE during scripted events

Reported: a tap balloon could be opened with SQUARE or TRIANGLE while a scripted event played in the living world, which broke into the event. The port took a tap whenever the world was not paused and the picture was not hidden, in any mode. Native has two rules. The tap balloon ops 0x45, 0x46 and 0x47 (`func_02010394`, `func_02010440`, `func_020104b4`, from `func_02023fdc`) close the balloon in the left slot and open nothing while the game byte at `game+0xa` (`func_02011320`) is 2, demo mode, the mode a scripted event or a trick plays out in. The right slot's balloon (class `data_020a86a4`, ops 0x4c, 0x4e and 0x4f) runs `func_02045cf8` each tick: it shows only once that byte is 3, normal play with the player in control, and the swirl bit (`func_0203435c`) and bit 15 of `game+0xc` (`func_02045ec4`) are clear, and it hides as soon as either changes.

The class: every tap balloon, both slots, and every way to tap one (SQUARE, TRIANGLE, the touch screen and the route pad). Not modelled: bit 15 of `game+0xc`, and ops 0x47 and 0x4f, which the port does not run.

Guard: `script_balloons_are_live` (normal play and the picture not hidden) gates the tap, the route pad's balloon lookup and, through the HUD view's `tap_balloons_are_live`, the drawing and the touch hit test. A balloon stays open while it is not live and comes back when the player has control again. The left slot's ops close the balloon there and open nothing in demo mode. `tests/unit/test_balloons_only_in_normal_play.c`.

### 197. The pause button and START stayed live during scripted events

Reported: the pause button showed, and START opened the pause menu, during scripted events and cutscenes (seen in Chapter 6). START opened the menu whenever a script was playing, button or not, and the button showed whenever any pill was on screen. The hourglass pill is wanted in every group but none (`soul->rollback_is_on`), and effects can put pills up, so a scripted event in demo mode kept the button up. The DS has no pause button (root cause 170), so native gives no rule for it. The port follows the HUD board's rule for the player's own buttons: the mode update asks `func_0204f850` for the normal group in normal play (`func_02036f5c`) and offers the Ghost button in demo mode only while a trick plays out (`func_02034260`), as root cause 127 ported.

The class: every way into the pause menu from play, START, a tap on the button and SELECT or the notebook pill onto the Record.

Guard: `script_pills_player_has_control` (root cause 201 renames it `script_pills_pause_is_offered` and takes the soul view out) is true in the normal group, the demo-mode play group and the player's own soul view (not a scripted view demo). The HUD view carries it as `player_has_control`; the pause button draws and answers a tap only with it, START opens the menu only while the button shows (`viewer_app.c`), and the Record request needs it too. `tests/unit/test_select_wants_the_record.c`, `tests/unit/test_script_ui_view.c` and `tests/integration/test_viewer_app.c` (`test_start_opens_the_pause_menu_only_while_its_button_shows`).

### 198. A picked title button never looked pressed

Native pGameTitle (`func_ov005_020d3b08`, state 8) answers a pick by calling `func_02067dd0` on the picked button, which switches it to the pressed animation `func_02067c88` stored at +0xe4, and `func_02067d80` (the plain animation at +0xdc) on every other button. Nothing changes the buttons again through the flash, the hold and the fade (states 9 to 11). The main buttons carry three looks (0x806b plain, 0x806d cursor, 0x806f pressed, tables `0x020d4780`, `0x020d4774`, `0x020d475c`); the trash can has 0x64, 0x65 and 0x66, and the notebook's pressed look is its cursor look (0x68 twice). The port drew only the plain and cursor looks, so a picked button looked the same as one the cursor rested on for the two seconds before the screen went black. The remaster's title button sheet `cmn_button_01` (06_id/10_select) holds the same three looks for the orange button: plain, a red ring, and blue in a red ring, the pressed one, at (234, 116).

The class: every title button, and every way of leaving it: a pick that starts play (hold and fade), a submenu pick (the pan), and a pick that asks first (erase, New Game over a save). The pause menu's main screen has no counterpart: its entries are labels that pan away, with no button and no fade.

Guard: `pause_art.py` bakes the remaster cell into the pause widget sheet as `title_button_pressed` and `title_icon_pressed`, sized like the other title slices. `title_menu.c` draws the picked button with it while the screen holds, fades or pans out, or while its ask is open, and the plain look on the others; the notebook keeps its cursor look as native does. A set baked before this falls back to the cursor look. `tests/unit/test_title_menu.c` (`test_a_picked_button_looks_pressed_through_the_hold_and_the_fade`, `test_the_trash_looks_pressed_while_its_ask_is_open_and_not_after_no`).

### 199. The cells closed over black

Reported: in the prison, the cell bars closing (`st03_ori_100`, objects `ori` and `ori2` in room 72, in `st03_game031` and again when the power returns in `st03_game012`) drew over black. The bars art is see-through between the bars; the cells behind them are room 72's picture `st03-08_0` (2520 x 1484, 1.9 MB unpacked). In the Vita run of build `c55022d7-ff` the cut to room 72 at 1236 s logged `texture retry name=st03-08_0 reason=heap` 40 times and more: the unpack buffer could not be placed in the broken-up heap, so the picture stayed loading and the frame held with the bars and the other objects over black. Root cause 194's 16 MB block sat free the whole time, because it only served unpacks over 4 MB, and the 1.9 MB picture asked the heap alone.

The class: any texture read whose single unpack block the heap cannot place, at any size. Root cause 194's heap growth (`MEMORY_HEAP_BYTES`) gives the heap room again, but a long session can break it up past any margin. Guard: `read_buffer_take` falls back to the reserved block for any unpack up to 16 MB that the heap refuses, while the block is free, and hands it back on release as before. `tests/unit/test_read_buffer.c` (`test_a_room_picture_the_broken_heap_refuses_gets_the_free_reserved_block`) caps the fake heap's largest block at 1 MB and checks that a 1.9 MB unpack gets the block, and that a second one waits while it is held.

### 200. The loading cat walked in spurts

Reported: on the loading screen, Sissel's cat inched forward in bursts. The DS has no loading screen (root cause 172), so there is no native walk to follow; the walk is the remaster's cycle `st01_neko_007`, baked unpinned so its own forward motion stays in the art (root cause 184 capped the catch-up). Measured from the baked frames, that motion is uneven: the silhouette's centre moves on by 0 to 23 px per drawing against a mean of 10 px, the paws with it, and the four drawings round the loop point (18, 19, 0 and 1) barely move at all, a fifth of a second standing in every second. `walk_feet_x` added nothing inside a cycle and a whole stride at the wrap, so the screen showed the art's spurts as they were. The catch-up cap, the 2-tick drawings and the whole-pixel snap were checked and are even.

The class: any animation the port carries along a path while its own drawings also move it. The art's motion per drawing has to be measured in the bake and taken out, or the two add up unevenly. Guard: `bake_cat` in `pause_art.py` measures each walk drawing's lead (`body_lead`, where its solid pixels balance, from the paw anchor) and writes it as a `lead` line in `pause/cat.txt`. `cat_track_text` turns the leads into a `pace` (the stride over the cycle's ticks) and a shift on every `walk` line (`walk_shifts`), so lead plus shift rises by one pace each tick and by one stride round the loop. `walk_feet_x` in `loading_screen.c` adds the tick's shift and the part of a tick that has passed, so the body glides at a constant speed between drawings and across the wrap. A track without shifts walks as before. `tests/unit/test_loading_screen.c` walks uneven drawings through irregular frame times and checks the drawn body moves by the pace for the time passed, within a pixel, over several crossings. `tests/tools/test_pause_art.py` checks the lead and that the baked shifts even out the body tick by tick and round the loop. The game set needs a rebake for the shifts to reach the Vita.

### 201. The pause button and START stayed live in Ghost view

Reported: the pause button showed, and START and SELECT opened the pause menu, in Ghost view, where the world is already stopped. Root cause 197 counted the player's own soul view as having the controls, so the pause rode along with the Ghost-view pills. The pause is the port's own (the DS has none, root cause 170), and Ghost view already holds the world, so it belongs to the living world only.

The class: every way into the pause menu, START, a tap on the button and SELECT or the notebook pill onto the Record, in every ghost view, Sissel's and Missile's.

Guard: `script_pills_pause_is_offered` (renamed from `script_pills_player_has_control`) is true only in the normal group and the demo-mode play group, never while the soul view is on. The HUD view's `pause_is_offered` gates the button, its tap and START, and the Record request asks it too. `tests/unit/test_select_wants_the_record.c` opens the soul view and checks that neither the pause nor the Record is offered.

### 202. The lavender body showed outside the puzzle's start

Reported: the lavender past-dead body (root cause 86) showed in scripted film events and did not always go away after the soul left. Native's body update (`func_02053dd4`, from `func_02053ea8` for `uHumPastDead` and `func_02053e90` for `uHumPastDeadMissile`) makes its `uPastDeadSprite` once, on the body's first visible frame (`func_02010b54`) with its frame read (`func_02059b88`). It parents the sprite to the body (`func_020153d8`) and ties the two by a pool handle (`func_02031b44`). The object list starts the sprite through slot 0x14 (`func_020294e4`), which sets it full with no program (slot 0xa8, `func_02029788`). Slot 0xac (`func_020297b0`) fades and removes it and marks the handle (`func_020297f0`), after which the body frees the handle and never makes another (`func_02031af4`, `func_02031b04`, body+0x10). Every script change kills every such sprite (`func_0200fe74` through `func_0202fad4`). Who calls slot 0xac is still not found. The port made the copy on the body's first visible frame wherever the soul was. After a failed rescue's retry (st02_game004), GAME START puts the soul on a checkpoint core, so the copy was made full and never faded, and it stayed through the film frame that followed.

The class: every past-dead body of either class in every chapter, at any entry to its scene: a first visit, a retry, a loaded save or a film frame.

Guard: `script_ghost_copy.c` makes a body's copy only while the soul sits on a start core (`script_class_is_soul_start`). A body that first shows with the soul on any other core, or while the film frame is on, is retired for good, as native's body+0x10 retires a body, and a film frame retires a copy already showing. `tests/unit/test_ghost_copy.c` checks the retry, the film frame, a body that waits for the soul to reach its start core, and Missile's classes. A whole-game replay shows each of the 14 bodies it reaches only from the tick the soul reaches its start core until it fades after the soul leaves, and none during a film frame.

### 203. Most dead talks played over the living world, not the red swirl

Reported: in Chapter 11 the talk with Kamila (`deadtalk` in st07_game000) stayed on the room instead of the red swirling backdrop. The port turned the swirl on only from script ops 0xdf and 0xe0 and the time leap, so a talk had it only when its own `DEADTALK` handler ran op 0xe0 (Kamila's does not, nor do Jowd's, Lynne's, Missile's CORETALK and most others). Native puts it up in the dead-talk state itself: entering `pGameMainDeadTalk` (`func_0203bd18`, mode 0x15, then `func_0203b648`) calls the swirl setter `func_0200f084(game, 1, ...)`, the state's update (`func_0203bd84`) waits for the swirl's opening effect (`data_020cfb44`) to end before `func_0203bfc8` sends `DEADTALK` or `CORETALK`, and leaving the state (`func_0203baac`) calls `func_0200f084(game, 0, ...)`. `DeadTalk|Finish` and `Abort` leave it (`func_0203c03c`) and `Restart` enters it again. `func_0200f084` does nothing when the bit already holds the asked value, so a script's own 0xe0 inside a talk changes nothing.

The class: every dead talk and core talk in every chapter: the 54 the whole-game replay starts, from Chapter 2's st06_game002 to st14_game061, whether the route, a dead core or `DeadTalk|Restart` starts it.

Guard: `start_dead_talk` turns the swirl on and `DeadTalk|Finish` and `Abort` turn it off, and `script_machine_set_swirl` ignores a call that changes nothing, as `func_0200f084` does. The opening effect's wait is not ported; the talk starts on the same tick. `tests/unit/test_dead_talk.c` checks a talk's swirl through Finish, Abort and Restart, and that a second Finish reports nothing. The whole-game replay plays the same steps with the swirl on for each talk.

### 204. A CROSS press during narration showed the whole page at once

Reported: rapid CROSS presses through narration skipped past lines, so the player could not read them one at a time. A press while a page was still revealing showed every line left on it (`show_the_whole_page`, from commit 789f966f). That was the port's own choice, made to match speech bubbles; native's handling of a press during a narration page's reveal is not traced, and the DS reveals the lines in order with its own waits (the timing in `load_page`). A press that landed while a page faded out was also kept and spent on the next page's first line.

The class: every narration page in every chapter, with both timings, and every way to press (CROSS, a held CIRCLE, which presses each tick, and the route pad).

Guard: a press finishes the line still fading in, or else shows the next line in full (`show_the_next_line`), and the page waits for a press only once its last line is shown. A press while the page fades out or the dim lifts is dropped. A page shown all at once (root cause 151) keeps its single finishing press. `tests/unit/test_narration_skip.c` presses through a three-line page one line at a time and presses every tick through two pages, checking that every line is reported in order.

### 205. TRICK did not press down on a core with no trick, and the pause button never pressed

Reported: on the Lady's Red Apartment chandelier, CROSS left the TRICK pill up. The port refused a press on a core whose trick info names no trick (root cause 157, op 0x9f) before it pressed the pill. Native's normal-play update (`func_0203696c`) presses the pill first, `func_02010ef4(game, 4, -1)`, then reads the core: no core, a held core (`func_02036f3c`, `func_020100e4`) or a trick info that names no trick (`func_02058ab8`, which plays sound 4 through `func_0202a0c0`) each return after the press. Every other pill in the same function also presses (`func_02010ef4` with ids 1, 3 and 5) before its own checks. The START pause button is the port's own (root cause 170) and drew only its raised look.

The class: every press of every pill in every chapter, whatever the core or the action does after it, and the pause button from START or a tap.

Guard: `take_press` presses the TRICK pill before it asks the trick info. The pause button keeps `start_press_ticks`, set by `script_ui_view_press_start_button` when START or a tap opens the menu and counted down by `script_ui_view_advance` over `SCRIPT_PILL_PRESS_TICKS`, the pills' own press length, so it shows pressed through the menu's darkening and rises when play resumes. `tests/unit/test_core_trick_info.c` (`test_a_refused_press_still_presses_the_trick_pill`) checks the refused press shows the pill pressed, and `tests/unit/test_script_ui_view.c` (`test_start_presses_the_start_button_down_and_it_rises_again`) checks the pause button's pressed ring, its rise and that a press while it is hidden does nothing.

### 206. Tricking a telephone off the route opened no phone book

Reported: tricking a telephone showed no place list unless that call was on the route. The port had no phone book at all. Op 0x77 only marked it open, and only the route's `phone_call` and `phone_stay` answered it, from inside the game (`route_no_player_input`), so a player's Dial left the game waiting with nothing on screen. The book also held nothing, so the route walked other steps while it was open and answered it much later.

Native: op 0x77 (case `0x020246fa`) stores the place with `func_02026608` and calls `func_020123a0`, which halts the world (`func_02010384`) and, for mode 0 (every op 0x77 in the scripts), switches to `pGameMainPhoneBook`. Its enter, `func_02037274`, holds the dial while script flag 0x1022 is set. Its list, `func_02038848`, is notebook list 0 (`func_0201ff10(game, 0)`) with place 1 and the op 0x77 place added, the cursor on that place. Register 0x1a (`func_0201fc30`) reads the picked place. Go There sends `phone movie` (`func_02033140`) and Stay Here `phone cancel` (`func_02037a20`).

The class: every telephone in every chapter, the player's pad and the route pad alike, and every route block that relied on walking past an open book.

Guard: `script_phone_book.c` builds the list as `func_02038848` does and takes the pad while the book is open, flag 0x1022 is clear and no talk is up: up and down (or the stick) move, CROSS goes, CIRCLE stays, and nothing else reaches the game. The HUD draws the list from the Record's place pictures, with the Go There and Stay Here words; native's dial wheel is not drawn. The route pad answers through the same pad and never picks a place the book lacks. Four route blocks walked past an open book, and the footage shows none of their Dials: st01_game020's junkyard Dial (4699 s) belongs before its ride, and the scene now routes its talks, the pistol and the return to the telephone through gameAct030 to gameAct090; st03_game000's and st05_game001's Dials are dropped; st09_game010's Dial moves to just before its ride. st03_game000's first ghost view now waits for the memo the guard hands over, and st13_game000's ride takes place 0, since its `phone movie` handler loads `st13_root.xml` itself. `tests/unit/test_phone_book.c` checks the player's picks, Stay Here, the 0x1022 hold, that L does nothing while the book is up, and that the route never picks a missing place. `route-check` stalls on any route that walks past a book. The new steps' `footage_s` are read from 3-second sheets, and the route pacing is not re-baked yet.

### 207. The chapter card stayed up over the opening narration

Reported: no narration text showed at the start of Chapter 13. The bridge (`script_bridge.c`) kept a chapter card (`uChapterStart`, root cause 35) up until the script's mask fade died, and once the card's own fade to black ended, at 273 ticks, it held the full black fade over everything. The fade and the card draw above the text lines (`scene_view_draw_covers`), so a narration that began while the mask was still alive drew under them. The device log showed the text lines shown and their textures loaded, with the card hidden only at tick 2367. In the whole-game replay the mask fade outlives the card in 13 of the 18 chapters (2, 3, 7 to 17); in Chapters 1, 4, 5, 6 and 18 it dies at the card's end, which is why it was missed. Native narrates these openings over the black mask, which draws below the text.

The class: every chapter card in every chapter, and anything drawn above the cover mask while a card was up.

Guard: the card hides when its own fade ends (`CARD_FADE_OUT_ENDS`), or sooner if the mask fade dies first, and leaves the mask fade to cover the world. `tests/unit/test_chapter_card_ends_on_its_own.c` holds the mask fade alive past the card and checks the card and the fade are gone at tick 273 and stay gone, and that a mask fade dying first still ends the card.

### 208. Effect movies kept whole 4096 px pages, and Chapter 13's wind froze the game for 8 s at a time

In the fast-forward run of build `69fad72b-ff`, from 2133 s in st13 every frame took 8.0 s (update 8,011,000 us), one every 8 s, until past 2484 s. Each such frame logged a `texture retry` of one of `fx_st13_kaze_00_04` to `_09` for `gpu_memory` or `heap`. The ten wind pages are 4096 x 4096 DXT5, 16 MB each, 160 MB in all, more than the 159 MB the texture cache may use. `keep_movie_pages_loaded` (`script_view.c`) asks for every page of a playing movie, so the cache evicted the rest of the scene and still could not fit them. When vitaGL cannot allocate a texture, `gpu_alloc_mapped_aligned_unsafe_for_cpu` and `_for_gpu` call `glFinish`, wake the garbage collector and sleep 1 s (`sceKernelDelayThread(1000000)`), up to four times, before they give up. The cache asked twice for each page, once for the empty texture the fill job needs and once for the plain upload, so each refused page held the main thread for 8 s. The pages are mostly clear: 0.5% of the wind pages' blocks have any alpha. Across the game the 139 movies took 726 MB of pages.

Native: the DS plays these movies (`func_020670bc`) from small cells in its own ROM art. The 4096 px sheets are the remaster's Android format, which native never loads, so how they are laid out on the Vita is the port's choice.

The class: every effect movie in every chapter. The 32 pages of 4096 px (st03 alert and flood, st11 timecode, st13 wind, meteor and finale) could not all stay loaded, and every other movie's page took far more memory than it draws.

Guard: `tools/assets/effect_pieces.py` bakes each movie after its pages are encoded. It keeps the 32 px squares of each frame's cell whose DXT5 alpha is above zero, joined into runs along each row, each copied with a 4 px border from the source page so bilinear filtering samples the same texels. It packs them onto pages of at most 2048 px, in the order the frames first use them. `effects.txt` now has `fxpiece` rows (page, texture rectangle, and the rectangle's share of the cell), and each `fxframe` names its run of pieces. `script_view.c` draws each piece at the cell's corners mixed by those shares. A movie with a quad that is not a parallelogram keeps each cell as one piece, so its two triangles map as before. The movies now take 131 MB, the largest 8 MB, and the wind 4.2 MB on two pages. The converter refuses a movie whose pages would take more than 16 MB (`MOVIE_MEMORY_MOST`). `tests/tools/test_effect_pieces.py` checks the squares, runs and borders, a cell that shows nothing, a bent quad, shared cells, that each piece shows the source page's blocks on a page of at most 2048 px, that a second bake reuses the stored layout, and the memory limit. `tests/unit/test_fx_playhead.c` checks that frames read their own movie's pieces and that a frame naming a piece past its movie's is refused. A one-off check over the wind, the second meteor movie, the street light and the first flood found every block that shows covered by a piece.

### 209. The ghost view's cores stayed drawn over the red swirl

Reported from the device: in Chapter 2's dead talk with Missile, the core markers sat on top of the red swirling backdrop. Root cause 203 put the swirl up for every dead talk, but the port keeps the ghost view on through the talk (so `DeadTalk|Finish` returns to it), and `draw_soul` drew the cores, the soul, its chain, sparks and flash after the swirl whenever the view was on. Native draws none of them there. Starting the talk switches the main state from pGameMainSV to pGameMainDeadTalk (`func_02032914` queues it with `func_02022f34(data_02099ed8)` and code 0x15), so pGameMainSV's exit `func_02033140` runs and releases its two handles with `func_0202f64c`: the uSoulBGEffect, whose children draw the cores (uSoulBGGimmickCore, `func_0205d0d4`), the silhouettes, the tube and the bar, and the uSoulTentacle. The bar's draw (`func_0205c8a4`) also asks the swirl bit (`func_0203435c`) itself. Leaving the talk back to the view (`func_0203baac`) restores the ghost palette with `func_0200ef08`.

The class: everything the ghost view draws over the world, in every dead talk and core talk of every chapter, and any other time the swirl is up while the view is on (a time leap from the view).

Guard: `draw_soul` (`script_view.c`) draws nothing while the swirl is on; the swirl itself covers the ghost-tinted world beneath. `tests/unit/test_swirl_hides_soul_view.c` draws a marked dead core, a live core and the soul with the swirl off, on and off again.

### 210. Packed textures over about 1.2 MB could not be read once the heap broke up, and fast-forward ran at normal speed

In the fast-forward run of build `0acccb48-ff`, from 2597 s in Chapter 14, four 2048 px textures (`fx_deadtricklight_00`, `st14_04stop_407_000_00_t00` and `_01_t00`, `st14_04-256wall_407_00_t00`) logged `texture retry ... reason=heap` over 1000 times each and never loaded until a scene change at 3152 s. At 3230 s `st14_linne_675_00_t00` did the same. While drawn art is loading, fast-forward runs one tick a frame (`slowed_for_art_frames=60`), so the game played at normal speed, with that art missing. The heap held 77 MB of its 112 MB, but no free block much over 1.2 MB: every packed file that loaded after 2597 s was 1.21 MB or smaller on the card, and every one refused was 1.30 MB or larger. The reader allocated the packed file from the heap before unpacking it, and only the unpack (root cause 194) could fall back on the reserved 16 MB block. So a large packed file failed with `FILE_ERR_MEMORY` before the block was ever tried.

Native: the DS streams art from the ROM into fixed VRAM banks and has no heap for it, so this is the port's own reader.

The class: every texture whose packed file is larger than the heap's largest free block, in every chapter, and any plain file the same.

Guard: the read and unpack moved from the Vita reader thread into `src/app/texture_read.c`, which the host fake reader runs too. `file_read_whole_into` (`file.h`) takes the buffer from the caller. When the heap refuses it, the file is read into the end of the reserved block and unpacked into its start, which do not overlap when both fit in 16 MB. A plain file stays where it was read, and `read_buffer_release` frees any pointer inside the block. While the block is held, the read reports `FILE_ERR_MEMORY` and is retried as before. `tests/unit/test_texture_read.c` checks a 1.6 MB packed 2048 px page with a 1 MB heap (read and unpacked in the block, its texels right, the block free after release), a read while the block is held, a plain 2 MB file, and that a packed file the heap can hold is freed once unpacked.

### 211. Auto-play searched the whole core graph twice every tick of a move

In the fast-forward run of build `0acccb48-ff`, 617 ticks spent over 15 ms in the route pad, and 1392 over 15 ms in the machine. How many of the machine's were this search is not yet measured; the phase split in `script tick_long` (runner, objects, motion, rest) names them on the next run. At 2074 s in st13_game012 a move toward the mino took 35 ms of machine and 33 ms of route each tick, 22 frames of 71 ms in a row. The Mac replay ran the same ticks in microseconds, so it ran the same work far faster: on the Vita, `first_hop_toward` (`script_soul.c`) is a breadth-first search over every pair of cores. Each link it tries runs `a_push_lands_on`, up to 17 simulated pushes of up to 90 ticks, and each simulated tick checks every core. `route_move_is_done` ran it inside the machine on every tick of a move step to name the next hop, and the route pad ran it again with one more `a_push_lands_on` (`script_soul_route_hop_aim`) to aim the stick. Nothing it reads changes while the cores stand still, so most ticks repeated the last answer.

Native: none. The route and its pad are the port's auto-play harness (`script_route.c`, `script_route_pad.c`); a player never runs them, so this slowed only fast-forward runs and their stutter scans.

The class: every move step in every chapter's route, in the machine and in the pad.

Guard: `script_soul.c` keeps the last eight answers, each keyed by the active core, the target and a hash of everything the search reads: the object count, the ghost, the reach, each object's shape, which slots can be cores and where each core is. A search runs again only when one of those changes, and the stick's aim is kept with its hop. The whole-game replay's marks are byte for byte the same with and without the memory. `tests/unit/test_route_pad.c` (`test_the_route_searches_again_only_when_a_core_moves`) asks 30 times with nothing moved and checks that no new search ran, then moves a core and checks that one did and the hop changed.

### 212. A held pose showed the object's previous animation, and Chapter 5's ending drew two Lynnes

Reported from the device: at the end of Chapter 5, as Lynne runs out of the tunnel, two Lynnes were drawn. The scene is `demo5` of st02_game023 (room 22). `demohum2` plays Lynne coming out of the tunnel (`st02_linne_331`) while `demohum1` is hidden (op 0x95). 43 ticks in, `demohum1` shows Lynne running on (`st02_linne_231`) and `demohum2` turns into the desk lamp (`st01_kuneri_125`). The lamp's page had been evicted, and the run log of the fast-forward run shows `pose held object=demohum2 frames=7 waited_for=st01_kuneri_125_00`. While a frame's texture loads, `draw_object` (`scene_view.c`) draws the object's last drawn picture, and that picture was the Lynne of `demohum2`'s previous animation, beside the real Lynne of `demohum1`.

Native: op 0x117 (`func_020280c8`, `func_0205991c`) queues the new table, and the actor's update (`func_0205964c`) starts it in the same tick, so the old motion's frame is never drawn again. Native art is resident, so the held pose is the port's own choice (root cause 5).

The class: every object whose motion changes to another animation while that animation's page is still loading, in every chapter. The whole-game replay changes animation 8,252 times on visible objects; 463 of those, in 188 objects over 12 stages, turn an object into another thing (demo actors, doors, props), where the held picture is a different character or object.

Guard: a held pose belongs to the animation that drew it. When the object shows another animation, the hold is forgotten and the object draws nothing until its new frame is ready, as the art gate holds the game meanwhile. A stall inside one animation still holds its last frame. Each such absence of 2 frames or more logs `pose absent object=… frames=… waited_for=…` beside `pose held`, so the stutter sweep can count both. `tests/unit/test_held_pose_keeps_its_animation.c` switches a running Lynne to a lamp whose page is loading and checks that Lynne is not drawn while it loads, that the lamp draws once it is ready and the absence is logged, that a stalled frame of the same animation still holds, and that a hold forgotten for another animation does not come back. The replay's watch lines now also print each watched actor's visibility and ghost copy level.

### 213. The credits' scenes filled the wide screen while the staff pages kept the 4:3 frame

The staff roll draws each page at the DS screen's size, 720 by 540 px in the middle of the Vita screen, but the scenes, movies and fades between and under the pages drew across all 960 px. So the frame jumped between 4:3 and the full width through the credits.

Native: the DS is 4:3 throughout, so the frame is the port's choice. Native marks the credits with its own state: op 0xf8 starts each page (`func_0203dbf0`), and the chain of st15 scripts that follow run their scenes between pages until `GOTO TITLE` (`func_02031e30` clears the ending mode, `game+0x103c`, on the way). The ending mode alone is not the credits: op 0x6a (`func_02011288`) also turns it on partway through the last scene of the story (st13_game022), before any page, and `func_0203dcc0` turns it off each time a page leaves.

The class: every frame of the credits, the pages and every scene, movie and fade between them, and nothing before the first page or after the title.

Guard: the machine keeps `credits_are_on` from the first staff page until the script chain asks for the title, and reports it through `credits_changed`. `script_view_draw_on_covers` draws black bars, last, either side of the DS frame, sized from the same width the staff pages use (`SCENE_VIEW_DS_SCREEN_WIDTH` times `VITA_PIXELS_PER_DS_PIXEL`). `tests/unit/test_credits_side_bars.c` plays a page, a scene after it and `GOTO TITLE`, and checks the credits stay on through the scene and end at the title, that the ending mode alone does not start them, and that the bars leave a 4:3 frame the width of a staff page and go away when the credits end or the view restarts.

### 214. Auto-play's push simulation tested every core in the scene on every simulated tick

Root cause 211 kept the core graph's answers while nothing moves, but in run 13 (build `71f107d5-ff`) cores were moving with their animations, so the search ran every tick anyway. In st06_game001 the `reach:santa` condition (`soul_can_reach_core`) took about 15 ms of the machine's runner phase each tick. In st03_game012 the route pad took about 40 ms a tick, which gave 20 frames of 51 to 57 ms. Each link the search tries runs `a_push_lands_on`, up to 17 simulated pushes of up to 90 ticks, and every simulated tick ran `core_picked_by_a_tip` over every core in the scene. A pushed tip never leaves its root's reach: `soul_chain_aim` clamps the head to the reach, and the tip only eases toward the head. A core is picked only within `SOUL_TIP_PICKUP_FAR` of the tip. So any core farther than the reach plus that distance from the root can never be picked by that push.

Native: none. The route, its pad and the push simulation are the port's auto-play harness; a player never runs them.

The class: every route search: move steps, the pad's aim and `reach:` waits, in every chapter.

Guard: `first_core_a_push_picks` (`script_soul.c`) gathers, once per push, the cores within the reach plus `SOUL_TIP_PICKUP_FAR` plus 1 px of its root, keeping the scene's order so ties still go to the same core. Each simulated tick tests only those. A push with none returns at once. The player's own pick, `core_picked_by_a_tip`, runs the same test over every core. The whole-game replay's marks are byte for byte the same as before. `tests/unit/test_route_pad.c` (`test_a_hop_search_tests_only_the_cores_a_tip_can_reach`) adds ten cores far out of reach and checks that one hop search makes under 400 tip tests; testing every core makes 1596.

### 215. The stick could pull the soul off a corpse while the connection played, so the talk started on the wrong core

Reported: after the ghost connected to a corpse, the player could still move the soul during the connection and hop off, and the talk then started on whatever core the soul had reached. The port held the tip only while the lightning ran (`drag_ticks_left`). Once the soul arrived, the time leap waits 102 ticks before its swirl closes the view (`LEAP_SWIRL_AFTER_TICKS`), and in that time the stick and the touch aimed the chain again. The dead talk targets the soul's current core (root cause 75), so it went to the wrong one.

Native: when the chain finds a dead core, `func_02061718` (through `func_020616dc` and `func_02058b4c`) sets the soul tentacle's state byte (+0x7e) to 4 with `func_02060d54`. The tentacle's input step `func_0205fa2c`, called each tick from its update `func_0205f7c0`, returns without moving the tip while the state is 3 to 8, so in state 4 nothing the player presses or touches moves it. The state stays 4 through the connection: `func_0206001c` only counts its timer and sets the connected bit (+0x7c bit 7) at tick 4, which the ghost-view update `func_02032914` reads (`func_02033408`) to start the talk. Entering the talk (state 0x15) also clears the tentacle's input bit (`func_02061138(actor, 0)`), and leaving the ghost view releases the tentacle, so a view opened later starts free.

The class: every connection to a dead body in the ghost view, in every chapter, by stick or by touch, and auto-play's pad, which goes through the same input.

Guard: `dead_core_holds_the_tip` (`script_soul.c`) is set when the soul arrives on a dead body with the view open and its time leap starts (`finish_drag_when_due`). While it holds, `move_soul_tip` treats input as it does during the lightning: the chain retracts and nothing is picked. It clears when the dead talk starts (`start_dead_talk`), when the view opens or closes (`set_soul_view_with_effect`), and when the soul is put on another core (`arrive_at_core`). `tests/unit/test_dead_core_holds_the_tip.c` pushes the stick toward a living core through the connection and checks the soul stays on the corpse, then checks the stick works again once the view reopens; before the fix the soul ended on the living core. Still open: native also holds the tip on a dead core whose talk waits for the yes/no prompt (`func_02032914`, mode 1 and 2); the prompt is not ported (root cause 92), so the port leaves the tip free there, or the player could never leave.

### 216. Ray told the player to press START for the Record, which SELECT opens

Reported: in Chapter 1's tutorial Ray says to press START to view the Record. On the Vita, SELECT opens it (`send_buttons` in `script_input.c` sends the notebook pill), and START opens the pause menu. The remaster's page (`m01_1200`) has no button word in it. It says `<ICON SAVE>`, and the talk baker drew that icon as a START button. The baker kept its own map from icon to glyph, separate from the input code, so nothing checked one against the other.

Native: none. The DS and the remaster show touch pills here; which Vita button stands for each pill is the port's choice (`script_input.c`).

The class: every `<ICON>` tag in every talk page, in every chapter and every language, since the tags are the same in each language's message files and the glyph is drawn by the baker, not taken from the text.

Guard: `BUTTON_OF_ICON` (`talk_assets.py`) names the Vita button each icon stands for, and the glyphs come from it. `<ICON SAVE>` is now the SELECT button. `tests/tools/test_talk_icon_buttons.py` reads `send_buttons` and checks that every icon names the button wired to that icon's action (press, ghost, hourglass, notebook), and that no icon is missing from the map.

### 217. Visual Filter's DS setting still drew the remaster's sprites, text and sounds, and the DS look needed the APK

Reported: in DS mode the characters, objects and effects were the remaster's. The DS setting swapped only the room plates. The texture cache read a name from `textures_ds/` when that folder listed it, and fell back to the remaster's `textures/` for every other name, so anything without a DS copy quietly showed remaster art. The DS look also could not exist without the APK, because the one set held both kinds of art, and its tables (text placement, balloons, rooms, records per DS frame) were sized for the remaster's.

Native: a motion table's `begin` names a cell resource; `func_0200b598` resolves it through the ARM9 tables into a palette (block 0) and a cell file and graphics (block 3), and `func_0200bf54` draws frame word N as cell N's OAM records (`func_0200c2c0`), reading streamed tiles per frame where the bank says so (`func_0200857c`). Text is the script's language file in the DS font (`func_0201c7c8`), and sounds are `sound_data.sdat`'s sequence archives (`func_02007250`, `func_02004a84`).

The class: every texture, animation, text line and sound the DS look shows, in every chapter, and every future file a DS build might take from the APK.

Guard: the converter builds two self-contained sets (`--art remastered` from the APK, `--art ds` from the cartridge alone), and the app runs one of them (`asset_sets.c`), relaunching at the title when the filter changes. No per-name fallback remains. During a DS build an audit hook (`refuse_apk_reads_from` in `convert_assets.py`) refuses every open or listing under the APK folder, so any DS code that reaches for the remaster's files fails the build; `tests/tools/test_ds_set_guard.py` checks the hook. `tests/integration/test_asset_sets.c` plays a scene with both sets installed and checks that the running set reads no file of the other. A DS talk page ends the way its control codes say (0x21 pause, 0x23 close, 0x24 fade in, 0x15 page turn; table `data_020848ac`, `func_02015d48`), so the tutorial's `pause in` reaches the script. The cartridge's own talks can run longer than the remaster's, so auto-play no longer counts the ticks an open talk holds the player against a route step's limit (`run_route`, `tests/unit/test_route_step_talk_time.c`). `make route-check PLAN_SET=game_ds` replays the whole route on the DS set. Still open: the cartridge's pause menu, talk window, balloons, HUD, effects and movies are not decoded, so the DS set draws none of them.

### 218. Choice panels were never drawn, so a choice over the red swirl looked like a softlock

Reported: in st02_game000 (installed build 3d516164, Remastered set, panels `panel1_1`, `panel2_1` and `m02_0310`) nothing showed over the red swirl, so the game looked stuck. The choice itself was open and took the d-pad and CROSS (root causes 46 and 69), but no build had ever drawn its panels, its marker or its captions, and the captions' text was never baked.

Native: the select control (`func_0205a9e8`, built by `func_0205ad48`) makes one slot per panel (`func_0205b420`). Each slot draws a bar (anim 0xc0, `func_0205b59c`) at x 0 and y `(1 - row) * 42` plus 32 when four or more panels show (`func_0205b838`), an icon at x -92 (`func_0205b850`) that is a check (0xc4) when the panel's flag is set and an empty circle (0xc5) otherwise (`func_0205bb40`), and the caption: the panel's label looked up in the message data and drawn as a text label (`func_02015bf0`). The marker (0xc1) sits on the cursor's row (`func_0205b164`); a pick turns it to 0xc2. The remaster draws the same parts from `06_id/10_select/cmn_button_01`: black buttons with a grey edge, the same with the red ring for the marker, the check and the white circle.

The class: every panel of ops 0x3c, 0x3d, 0x40 and 0x41 in every scene (71 caption labels across 41 scripts), in both the remastered and the DS set, since both bake their talk tables from the same panel label list.

Guard: `Script.talk_labels_of_choice_panels` now lists each panel's caption (word 0 of 0x3c and 0x40, word 1 of 0x3d and 0x41, the words `script_choice.c` reads) as well as its talk, so every caption is baked as a talk message in `talk.txt` for either set. `script_choice_fill_view` puts the open choice on the HUD view (shown panels, captions from the talk book, chosen flags, cursor, cancel), and `script_ui_view.c` draws each panel as a stretched `ui_choice_buttons` cell (the sheet's top 236 rows) with its icon and its caption in the talk font, centred, over the swirl and the hidden picture, with a CIRCLE Back hint when cancel is offered; when the button art is missing it still draws each panel as a plain box with its caption. `tests/tools/test_choice_talk_labels.py` checks the caption words of all four ops, `tests/unit/test_choice_panels.c` (`test_an_open_choice_reports_every_panel_with_its_caption`) the HUD view, and `tests/unit/test_script_ui_view.c` that every open panel draws its button, icon and caption, and still draws without the art. Port's choice: the panel size (600 by 88) and the caption's slight shift right of the icon; the DS bar's own size is not read. Still open: native shows the slots one by one 12 ticks apart and the marker after them, and plays the pressed look for 16 ticks before acting; the port shows all at once and acts on the press. A tap on a panel does not pick it yet.

### 219. The ghost view showed through the red swirl when the swirl's art was not in memory

Reported from the device: in Chapter 4's talk with Lynne before her flashback (st02_game023, the `......` page before the fade to white that `DeadTalk|Abort` opens the flashback with), the red swirl vanished and showed the ghost-tinted basement, Lynne in the ghost view's purple and pieces of the room missing. The machine still had the swirl on, which is why no cores were drawn (root cause 209). Native draws nothing of the room under the swirl: `func_0200f084` opening it calls `func_0200a200` on the room (`game+0xee0`), which stops the room's scrolling and streaming (`func_02009c7c` returns while the byte `func_0200a248` sets is on) and clears entry 0x10 of the table at `+0x4004`, and closing it reloads the whole room (`func_0200a260`). The port drew the room and every person under the swirl each frame, so the texture cache had to hold them all with the swirl's own art. When it ran short, the cache evicted whichever texture was drawn longest ago, the swirl among them, and `draw_swirl` drew nothing while it read back.

The class: every swirl, in every dead talk and core talk, every op 0xe0 and every time leap, in every chapter. A replay from st01_game021 to the end opens the swirl 89 times in 35 scripts, from st01_game021 to st13_game022.

Guard: the bridge's swirl output also sets `world_is_covered` on the stage, and the stage clear resets it. While it is set, `scene_view_draw_world` fills black and draws no plates or objects, and `scene_view_visible_art_is_loading` neither waits for nor reads their art. The swirl's art is opaque (DXT1) and covers the screen at any angle, so nothing visible changes while it is loaded, and a black screen replaces the ghost view while it is not. `tests/unit/test_swirl_hides_the_world.c` drives the swirl through the bridge's outputs and checks that the room and a person are not drawn, a new object's art is not read, and a stage clear uncovers the world. The Mac replay now writes a `swirl` cue.

### 220. The High Contrast preview cut its room plate in remastered pixels, so the DS plate drew as streaks

Reported: with the Visual Filter on DS, the Off/On picture beside the High Contrast row showed a squeezed strip of the room along the top, streaks to its right and thin vertical lines over black below. The preview names a room plate (`st01-00-02`) and the texture cache resolves that name to the art set in use: 1440 by 540 remastered, 512 by 192 from the cartridge. `pause/widgets.txt` gave the part to show as pixels of the remastered plate (60, 70, 832 by 468), and `pause_draw_from` divided them by the size of whichever texture loaded. On the DS plate the part ran from 0.12 to 1.74 across and 0.36 to 2.8 down, and everything past the edge repeated the plate's last column and row.

Native: none. The options screen and its preview are the port's own; the DS game has no such screen.

The class: any picture cut in pixels from a texture whose name the visual filter can resolve to art of another size. Every `st*` name has both sizes (334 names in `textures_ds`). The preview was the only pause picture that cut one in pixels: the Record's pictures and the loading backdrop draw the whole texture, and the widget, label, clock, cat and thumbnail sheets are baked once and have no DS twin. Scenes place plates by the manifest's world size, not by texture pixels.

Guard: the preview's part is fractions of its plate, in `game.json` (`pause.widgets.preview.part`) and in the `preview` line of `pause/widgets.txt`; `pause_draw_preview` multiplies them by the loaded texture's size. `pause_art.preview_lines` refuses a part that is not inside 0 to 1, and `pause_widgets_read_text` drops a `preview` line whose part is outside it and logs `pause preview part_outside`, so a table baked before this draws the card without a picture. `pause_draw_from` draws nothing when the part it is asked for leaves the texture, so a pixel cut that meets art of another size shows nothing in place of streaks. `tests/unit/test_pause_widgets.c` draws the preview from a 1440 by 540 and a 512 by 192 plate and checks both show the same part, and that a part outside the texture draws nothing; `tests/tools/test_pause_art.py` checks the baked line and that `game.json`'s part passes. Cut a picture that has two art sizes by fractions, never by pixels.

### 221. Hints had to follow a player through a route written for auto-play

The route moves on by counting: a trick step is done when any trick fires or is refused, a tap step when any tap is taken. That is safe while auto-play makes exactly the route's presses, but a player presses anything. The hint system (`script_hints.c`) needs to know which route step the player is on, so it follows the player's own actions: each trick, balloon tap, swap, character button, talk with the dead and phone answer is matched against the route's next step that acts on an object, and only that step.

Running that follower beside auto-play over the whole game showed three ways the route's counting hides what happened:

- A CROSS on a core with no trick is refused (root cause 205), and the route still counts it. st06_game004, st01_game020 and st03_game000 have such steps, so the follower counts a refused press as its trick too.
- A talk with the dead starts again after each choice in it. Matching by action alone let the first talk's restarts finish the next talk's step in st02_game036, so a talk is matched by its object.
- A retry after a death does not rewind the route: auto-play keeps its step across the reload (`route_step_after_retry`). The follower keeps its place too and goes to a step only when that step waits on `retried`.

Native: none. The game has no hints. The balloon is drawn and tapped through the tap balloons' own path (`script_balloons_are_live`, root cause 196), but it is kept out of the eight native balloon slots, because scripts test and close those slots (ops 0x4b, 0x50, 0x4d).

The class: every routed scene, and anything else that reads the route to learn where a player is.

Guard: at every finished route step the replay asks whether the follower is behind or ahead of auto-play and marks `hint_behind_route` or `hint_ahead_of_route`, and `make route-check` fails on either. The whole-game replay has none. It marks `hint_has_no_target` for a step a hint could not point at when it begins: 25 of the object steps (18 balloons, 5 character buttons and 2 phone answers), and no trick, swap or talk with the dead. `tests/unit/test_hints.c` covers the clock, the tap button choice, the one hint per scene, the camera pan, the retry and the steps with no object. `tests/tools/test_hud_art.py` checks that the "?" cell the converter draws on `hud_sheet` is the cell the app reads.

### 222. The title's Options button played the confirm sound, as if the game were starting

Reported: on the title menu, picking Options, Language, the trash or the Record played the same pick sound (system sound 26) as Continue and New Game, so a player who only opened a menu heard the game start. `pick` in `title_menu.c` played sound 26 on every branch.

Native: none. The title menu and its buttons are the remaster's; the DS game has no such screen.

The class: every button on the title menu, and any button added to it later.

Guard: `title_menu_item_enters_game` reads one table of which buttons lead into the game (Continue, New Game, Chapter Select). Those play the pick sound; every other button plays the cursor sound (system sound 1). Leaving for the game still plays the pick sound in `leave_with`. `tests/unit/test_title_menu.c` (`test_only_buttons_that_enter_the_game_play_the_pick_sound`) picks every shown button with and without a save, checks the sound each one plays against the table, and checks that no button the table calls menu-only leaves the title.

### 223. The hourglass pill and R did nothing

Reported: R and the hourglass pill did nothing. `script_soul_note_pill` pressed the pill and logged `pill_not_ported hourglass`.

Native: the hourglass is HUD pill 1. Op 0xfe and op 0xff (`func_02011cdc`) set and clear `game+8` bit 8 and put pill 1 in mode 5 or 0. Normal play's update (`func_0203696c`) takes the press when no trick talk is up, `func_02010b38` is clear, no fade runs (`func_0201572c`), `game+8` bits 13 and 8 are set (`func_020322d4`, `func_02013a14`) and nothing pauses the world (`func_0200f330`). It presses pill 1, sets mode 0x13 and the pause bit, and starts `pGameMainRBMforNormal`. The RBM scenes for normal play, demo mode, the ghost view, the exchange and the game over share one update (`func_0203b72c`). Their menu shows the `rollback_00` text. Choice 5 cancels back to the game. Choice 1 fades out (`uFadeOutMB`) and goes to state 0x84, the same retry as after a game over: the hourglass goes off, and the scene gets `*` `SYSTEM|RESTART` if a script handles it or a reload through `pGameRestartShort` if not. Choices 2 to 4 tear the stage down and play the rollback movie.

The class: every press of the hourglass, by R or by touch, in every chapter. Before this fix the pill was also wanted whenever `rollback_is_on` was set, outside the four minutes too.

Guard: a press sets `rollback_is_pending`. The soul's step takes it only while the pill is up, the world is not paused, and no talk, choice or game over is waiting. It then runs the game-over retry (`retry_the_fate_attempt`, marked `rollback_retry restart` or `reload`), which also turns the hourglass off as state 0x84 does and keeps the soul's core (root cause 94). The pill is wanted only while flag 0x1020 is set (`script_soul_fate_section_is_on`, the Minutes Before Death side label). The user asked for this rule. Native checks bit 13 instead, and its setter is not read. `tests/unit/test_hourglass_rollback.c` checks that the pill and a press do nothing without flag 0x1020, that a press with it reloads the attempt, and that a press during a talk is dropped.

Changed on 2026-11-23, Liam's call: the hourglass asks before it restarts. Native's RBM menu (`func_0203b72c`) waits in state 1 for a choice: choice 5 cancels (`v11`, then `func_02022f6c` pops the scene and play goes on) and choice 1 fades out and retries through state 0x84. The port draws no RBM menu; it asks one question instead, "Start over from the beginning?" (`row.rollback_ask`, in every language), with the pause menu's dialog box and No marked. A press that `take_rollback` accepts presses the pill and opens the question (`script_soul_open_question` with `SCRIPT_QUESTION_ROLLBACK`, marked `question_opened rollback`). While it is open `script_machine_tick` stops after `script_soul_step_question`, as it does for an eye-catch: no script, object, talk or timer runs and the tick count holds, the way native's pause bit holds the world under the RBM menu. The pills leave (`current_group` gives `GROUP_NONE`), the side hourglass stops (`world_is_running` is clear), and `script_input_frame` sends only the question's input: CROSS takes the marked answer, LEFT and RIGHT move the mark, CIRCLE or L is No, a touch on a button answers. Yes (`question_answered yes`) runs the same `retry_the_fate_attempt(machine, "rollback_retry")` as before; No closes it and play goes on. A game over still retries without asking. The question carries its kind (`question_kind` on the soul and the HUD view): `question_rules` in `script_soul.c` gives each kind its marked answer and `act_on_the_answer` what Yes does, and `question_lines` in `script_ui_view.c` its line, so another question adds a kind, a rule and a line. No route step presses the hourglass, but a player can press R while auto-play drives; the route pad then reads the question for `FRAMES_TO_READ_A_QUESTION` frames, counted by the pad because the machine's tick holds, and answers Yes. `tests/unit/test_hourglass_rollback.c` checks that a press opens the question and restarts nothing while the tick holds and the pills leave, that Yes restarts the attempt as before, that the marked No, CIRCLE, L and a touch on No close it with nothing restarted and the pill back, that a touch on Yes restarts, and that the route pad answers Yes after its read time.

### 224. A copied set had every texture in the wrong folder

Found while installing by hand over USB: the Vita build reads every texture from `textures/<crc32(name) % 64>/<name>.gtex` (`join_texture_path` in `src/app/texture_cache.c`, `TEXTURE_FOLDER_BUCKETS` 64 in the Makefile), but the converter wrote `textures/<name>.gtex`. Only the device uploader, `vita_loop.py`, moved each texture into its bucket on the way up, so every install through Vita Companion worked and every copy the README described would have shown magenta for all 24,000 textures.

The class: any file whose place on the Vita differs from its place in the converted set. Two tools held the bucket rule apart from the converter, so the set was never the install.

Guard: the converter writes the Vita layout. `output_folders.disk_path_of` maps a texture's name to its bucket and is the one rule: `BakeRecord` stores and reads through it, the sweep keeps a texture only in its own bucket, `vita_loop.py` lists bucketed files by name and uploads them to the same place, and `pause_render.py` reads them there. A set baked flat is moved into buckets by rename the first time a newer converter opens it, so nothing is baked again. `tests/tools/test_texture_layout.py` checks the buckets against the C test's examples (`a` in `03`, `movie_time_jump_012` in `30`) and the Makefile's count, the store, the reuse after the move, the sweep and the uploader's listing.

### 225. A fresh setup's pacing stopped the whole-game replay in Chapter 13

Found when `route-check` failed on vita-app with 1341 of 1730 steps and no `GOTO`, while the same code passed with the older pacing. That older file named an earlier route, so the app ignored it (`script route_pacing_stale`) and every step kept its own pace. The setup's "Plan the loading" stage then baked pacing for the current route, and it stretched step 1310, a `ghost_on seesaw` the route paces `instant` in st13_game013, to 171 ticks. By the time the soul was up, the seesaw had thrown the baseball out of the tip's reach, the next moves failed and the swap to the mino never came. Pacing is a test-harness choice that only holds auto-play back to the footage's gaps; native has no route.

The class: a pacing stretch on a step timed against something moving in the room. `route_pacing.py` dropped a stretch only when its own step never began, so a stretch that made a later step fail was kept and the bake settled on a broken replay. 12 `instant` steps were stretched.

Guard: `paced_dwells` never stretches a step whose pace is `instant`, and each pass drops the last stretch before any `route_step_failed` in the same scene, so the bake cannot settle on it; a settled bake prints the steps that still failed. `tests/tools/test_route_pacing.py` checks both, and that no `instant` step in `game.json`'s route can be stretched. The kept `game_pacing.txt` is baked again for the current route, so a fresh setup starts from pacing that passes.

### 226. Headers declared functions that only their own file called

Found in the 1.0 audit: 15 functions were declared in `include/` and called only inside the file that defines them (`script_machine_end_block`, `game_save_store`, `pause_menu_step`, `title_menu_step`, the two audio thread steps and nine more), and `work_queue_is_busy` had no caller at all. Three constants and the machine's `noted_effects` bytes were written once and never read. None of it changed behaviour, but each one made a header promise something no caller used, and a later change had to keep it working.

The class: any function a header exposes for a caller that has gone, in every module.

Guard: `tests/tools/test_headers_expose_what_is_used.py` fails when a function declared in `include/` has no user outside its own file among the sources, the other headers and the tests. Make such a function `static`, or delete it.

### 227. The player's build carried the dev tools and hashed the screen on every frame

Found in the 1.0 audit: the Makefile compiled every source into both builds, so the player's build linked the command reader, the commands parser, the BMP writer, the screen capture, the burst recorder and the frame rate overlay, none of which it can reach. It also ran one of them: `screen_watch_note_frame` asked the system for the frame buffer and hashed a 16 px grid of it (2,040 reads of display memory) after every frame, to log `screen flips` once a second. That line is a development measurement, and the player's build paid for it on every frame. It also made an empty `captures` folder on the memory card.

The class: any code only the debug build needs, wherever it is called from, and any game function that nothing in the player's build reaches.

Guard: the sources named in the Makefile's `DEBUG_ONLY_NAMES` are left out of the player's build, and `main.c` includes and calls them only under `DEBUG_BUILD`. `make test` links the player's build on the Mac with dead stripping (`tests/tools/check_player_link.py`, parser tests in `tests/tools/test_player_link.py`) and fails when a game function is unreached and `tests/player_link/unreached.txt` does not name it, or when the list names one that is reached or gone. The list holds only what the tests and the debug build call. The link uses the test fakes for `src/vita`, so a name a device source uses counts as reached.

Not measured: the saving per frame, because no device run was allowed. The Vita link dropped 5.3 KB of code.

### 228. The link line named libraries nothing needed

Found in the 1.0 audit: the link line named `SceAppUtil_stub`, which neither the game nor vitaGL imports, and `SceLibKernel_stub`, `SceSysmodule_stub`, `SceIofilemgr_stub`, `SceProcessmgr_stub` and `c`, which the toolchain links on its own. The rest are needed: vitaGL's fixed pipeline builds its shaders on the device, so it needs vitaShaRK, `SceShaccCgExt`, `SceShaccCg_stub` and `taihen_stub`, and it uses libmathneon, `SceCommonDialog_stub`, `SceKernelDmacMgr_stub` and the C++ runtime.

The class: any library left on the link line after its last caller went.

Guard: `make check-libs` relinks without each library in turn and fails if a link still succeeds. The imports of the linked app are the same as before; only the order of the stub sections changed.

### 229. The player's build wrote up to 256 MB of log to the memory card

Found when checking what the released build leaves on a player's card: `run_log.c` and `src/vita/log_sink.c` were in both builds, so every player's Vita wrote `ux0:data/ghosttrick/run.log` at about 2 MB an hour on the title screen and more in play, kept up to 128 MB in it and 128 MB more in `run.log.earlier`, and formatted each line on the game's own thread. Native writes no log; the run log is the port's own development tool.

The class: a development aid that is on in the player's build because nothing separates it from the game.

Guard: in the player's build `run_log_write` and `run_log_flush` do nothing, `run_log_open` deletes both log files, and `log_sink` is in `DEBUG_ONLY_NAMES`, so the writer thread and its 256 KB of buffers are not linked. `check_player_link.py --never-linked` fails `make test` if the player's link reaches `log_sink_start`, `log_sink_write` or `log_sink_flush` (`tests/tools/test_player_link.py`). Bug reports use the debug package, which the release workflow publishes as `ghosttrick-<tag>-debug.vpk`.

### 230. Every language but English showed English text

The Options and title Language lists offered nine languages, but picking any but English logged `not_ported` and kept English. The converter read only the English slot of the remaster's message files (`ENGLISH_SLOT`), drew every line in the English fonts, took the `.en` copy of every picture with words in it, and wrote one set of text files and textures. Yet every message file holds all nine languages: Japanese in slot 0, English 1, French 2, Italian 3, German 4, Spanish 5, Korean 11, Traditional Chinese 12 and Simplified Chinese 13. The APK has each language's fonts and its own copy of the title logo and the 18 chapter cards.

Native: the USA DS cartridge holds English, French, German, Italian and Spanish (`.en`, `.fr`, `.gr`, `.it`, `.sp` language files), and the DS title's Language entry switches between them. The remaster adds the other four. The words the port draws itself (Options rows, help lines, title buttons, HUD and pause labels, the Record's phone word) have no game text, so the port carries its own translations in `tools/assets/languages.json`. The DS text decoder knows only a minimal alphabet and drops accented letters, so the system line ("New info has been added.") comes from that file for every language but English.

The class: every output that holds text or depends on it: narration lines, bubble text, the talk book and its glyph atlas, the HUD and pause sheets, the pause tables, `text_lines.txt`, `balloons.txt`, the preload list, narration cues, and art with words in it. Every chapter, and any new kind of text output.

Guard: the set description names its language file (`"languages": "languages.json"`). After the English pass, `convert_language` (`tools/assets/convert_assets.py`) runs every text stage again on `languages.localized_set`: fonts swapped by the language's font map, `.en` art suffixes swapped, the port's words merged in, the DS cue language switched, and the message slot carried by the text key (`message_file.LanguageKey`). Each stage writes through `languages.LanguageRecord`, so an output with the same bake key as English is reused from the base set, and one that differs lands in `lang/<code>/` and is reused on the next run. `lang/<code>/names.txt` lists the language's files. The app reads every overridden file through `asset_language` (`src/app/asset_language.c`): the texture cache, the script player's tables and the pause tables. The language's talk atlas has its own name (`talk_font_<code>`), so a talk already open keeps the old book until it closes. The font map follows the remaster's slot files (`gui/font/font_*.fslt.4`): New Cinema, Helvetica (bubble text) and Chiaro sit in slots 0 to 2, and each language puts its own font in the same slot (Helvetica becomes Rodin in Japanese, Seolsans Black in Korean, MYingHei W8 in both Chinese sets). The Language list and the button keys are left out of the swap (`languages.KEEPS_ITS_FONTS`): each language name is drawn in a font that has its letters, so "Français" stays in Chiaro when the menus are in Chinese, and L, R and START match the Vita's buttons in every language. The line box rose from 1.0 to 1.125 em above the baseline (`LINE_BOX_ASCENT_EM`), because accented capitals in New Cinema reach 1.06 em and failed the ink check in French, German and Italian; every line's place comes from the same box, so the baselines on screen do not move. Text the chosen font cannot draw now fails the bake (`TextFonts.require`, reading the font's `cmap`), and a talk line wider than its box is named with `failed:`. `tests/tools/test_languages.py` checks that the app, `game.json` and `languages.json` list the same languages in the same order, that every language translates every English port string, and the overlay record, font check, CJK wrapping and atlas sizing; `tests/unit/test_asset_language.c` checks the lookup, the system language map and the texture cache's language folder. An overlay texture sits in the same bucket folder as the base set's, `lang/<code>/textures/<bucket>/<name>.gtex` (`output_folders.disk_path_of`, `join_language_texture_path` in `texture_cache.c`), so a copied set is the install in every language; `tests/unit/test_asset_language.c` and `tests/tools/test_texture_layout.py` check both sides.


### 231. Picking the DS filter on the title relaunched into the remastered set

Reported on the device: Options on the title screen switched Visual Filter to DS, the app relaunched, and it came back in the remastered set with the filter on Remastered. The options screen saves the settings when it closes, but `viewer_app_wants_relaunch` breaks the main loop on the same frame the filter changes, so `sceAppMgrLoadExec` ran before anything was written, and `asset_sets_choose` read the old filter at the next boot. The log shows `pause option visual_filter=1`, then `relaunch for the other art set` with no `settings saved` between them.

The class: any setting whose effect needs a relaunch is lost if the relaunch comes before the save.

Guard: `viewer_app_keep_settings_for_relaunch` saves the settings and waits for the write, and `main` calls it before it leaves the loop to relaunch. `tests/integration/test_asset_sets.c` picks DS, relaunches through it, loads the settings again and checks that the DS set is chosen.

### 232. The Credits row in Options could not play the credits

Options listed Credits greyed out, on the title and in the pause menu, because the staff roll lived only inside the script machine: the ending's scripts start each page (op 0xf8), fade it in (0xb), hold the script while it runs (0xf9), sometimes wait (0x2f), fade it out (0xc) and close it (0xfa), with a demo scene between pages. Nothing outside a running script knew a page's fades or the music under the roll, so the title, where no script is loaded, had nothing to play.

Native (root cause 125) sets the page timing in `func_0203dcc0`, `func_02064750` and `func_02064b88`, and the closing in `func_0203ddc4`; the fades are the ending script's. Page 0 in `st15_demo000` fades in over 15 ticks, waits 120 after the page and fades out over 45. Pages 1 to 10 reach a shared `0x2d` call that fades in over 25 and out over 45. Music 35 starts (op 0x106) before page 0.

The class: anything the script gives a sequence, a later screen that replays it needs baked, not copied by hand.

Guard: the converter reads each 0xf8 site, follows a `0x2d` call, and bakes `staffpage <page> <screens> <fade in> <hold> <fade out>` plus `staffmusic <name>` from the music started before the first page (`staff_roll_rows` in `chapter_assets.py`). It stops the bake if a page is not followed by a fade in, the page's wait and a fade out, or is started twice with different fades. `script_staff_page_row_read` reads the row for both the script tables and `staff_credits.c`. The credits player runs every baked page in page order on the native `script_staff_roll` machine and the fade arithmetic of `script_fades.c` (`script_fade_start`, `script_fade_step`), and draws pages through the same `script_view_draw_staff_page` the ending uses. It skips the scenes, waits in black until a page's screens are loaded, and plays the baked music. It hands back whatever music the menu had (`music_remember`, `music_restore`) when the roll ends, on CIRCLE or on a tap of the back pill. The Options row is greyed only when the set bakes no staff pages, as the DS set does. Tests: `tests/tools/test_staff_roll_rows.py`, `tests/unit/test_staff_credits.c` (page order, fades from and to black, waiting for art, the hold, music and its hand-back) and `tests/unit/test_pause_options.c` (the row is selectable, CROSS plays, CIRCLE and the last page return to Options).

### 233. The Vita build stopped linking when the system language arrived beside the audit's shorter link line

Found when the setup script built the package: `system_state_language` calls `sceAppUtilInit` and `sceAppUtilSystemParamGetInt`, and the audit (root cause 228) had dropped `-lSceAppUtil_stub` because nothing on its branch called them yet. The two changes merged cleanly and every Mac test passed, since the host build links fakes, but `arm-vita-eabi-gcc` failed with undefined references.

The class: a Vita library the link line lacks for a header the device code includes. Only a VitaSDK link shows it, and the Mac suites never run one.

Guard: `-lSceAppUtil_stub` is back in `LIBS`. `tests/tools/test_link_libraries.py` maps each `psp2/` header the sources include to its stub library and fails when the link line lacks it, or when a new header is not in the map; headers whose functions the default link spec supplies (`psp2/io/`, `psp2/kernel/`) are listed apart.

### 234. A conversion swept away every language's text art

Found after converting with the languages on vita-app: each `lang/<code>/` overlay held 9 text files and none of its 1,400 or so textures. Resolving the rebase onto the bucket layout kept vita-app's `wanted_paths` line in `write_outputs` (`tools/assets/convert_assets.py`), which lacked `| set(language_paths)`, so `languages.remove_overlay_files_not_listed` deleted every overlay texture the language passes had just written and `record.forget_all_except` dropped their bake entries.

The class: an output the converter writes but does not name as wanted, which the sweep then deletes.

Guard: `wanted_paths` includes the language paths again. `tests/tools/test_texture_layout.py` writes a base texture and a French overlay texture through `write_outputs`, then checks that both are still on disk in their buckets and that the overlay is still reused from the record; with the language paths left out the test fails.

### 235. A remastered build could not be made without the DS cartridge

The remastered set read four things from the cartridge: the scene scripts, the ARM9 tables (rooms, sounds, core infos, faces, effect kinds), the DS language files for faces and narration cues, and the system line's text. A player with only the Android release could not build it. The remaster holds the same scripts as RSZ record lists, keeps the tables in `libruntime.so`'s static constructors, and keeps faces and cue timing in its message files, so none of this needs the cartridge.

Native: the script VM step `func_02023fdc` defines what a compiled 1LMG script must hold, and the motion frame reader `func_0200853c` defines how a motion table packs its frames (a value is written only when it changes). The remaster's tables differ from the cartridge's in places: rooms 153 to 188 are shifted by three, the cartridge's face reader ran on past the table's 271 rows, and 12 effects are the remaster's replacements. The compiled scripts match 99.6% of paired DS ops (root cause 238); the rest are the remaster's edits, and they also reach six scripts and two rooms the DS chain never enters.

The class: every read of the cartridge in the remastered path, including ones a later change adds.

Guard: `convert_assets.py --apk-only` sets an audit hook that refuses opening the ROM path or any `.nds` file, so a new cartridge read stops the build plainly. Every ARM9 table read goes through one provider (`Arm9Tables` or `RemasterTables` in `chapter_assets.py`), and scripts come through one source (`RomScripts` or `RemasterScripts`) in `discover_chain`, so ds_set.py and the combined build keep the cartridge path unchanged. `tests/tools/test_apk_only_build.py` covers the tripwire, both providers, faces (a cut-in with `CutInFade` is the pose code), cues, system lines, message-file discovery and the script bake. `tests/tools/check_remaster_scripts.py` and `check_remaster_tables.py` compare the package against the cartridge on real data. Mutations for each guard are in `tools/mutation_check.py`.

### 236. The remaster's placeholder character stopped the conversion

Found by the first Android-only conversion: the remaster's chara lists name `dummy` for ids it has no art for, and its timeline has no records and a frame count of -1, so `timeline_file.decode_timeline` refused it and the whole set failed. The DS names never matched it, so the combined build had not met it.

The class: a chara row that points at a timeline with no frames, which is the remaster's way of saying "no art", like resource id 0.

Guard: `resolve_tables` drops every animation whose timeline has no frames (`timeline_file.has_frames`) and lists its tables with the other tables with no art, so `dummy` and `st04_nabe_000` draw nothing, as native does for a missing table. `test_apk_only_build.py` checks `has_frames`.

### 237. Two remaster pages had tags the message reader refused

Found by the Android-only cue bake: `<TRCK mono_pos_set posY ="5" />` and `<TRCK mes_bgm_stop time= "120" />` put spaces around `=`, and `message_file.TRCK_TAG_PATTERN` and `ATTRIBUTE_PATTERN` allowed none, so the reader called them unknown tags.

The class: TRCK attributes written with spaces around the equals sign, in any page of any language.

Guard: both patterns allow spaces around `=`. `test_apk_only_build.py` reads a page whose tags have them.

### 238. Compiled scripts lost every motion table past their first 64 KB

Found by the whole-game replay on the Android-only scripts: Chapter 3 stopped at route step 563 (`w_bell1` in st03_game010), where every route step passes on the cartridge's scripts. In that 120 KB script, 163 of 454 motion ops pointed into the middle of a table or at an op. The class table gave `SC_MOTIONTBL`'s second word as `hi0`, the top half of the record index (always 0), where native op 0x117 reads the top half of the table's byte offset. `SC_SCRIPT`'s block end had the same fault (`c0`). An offset under 64 KB still came out right, so small scripts and the unit tests passed, and the real-data check absorbed the misses in its 1.5% margin.

The class: an offset word in the class table whose high half is not the same offset's.

Guard: both classes now use `O`, the high half of the offset fixup. `tests/tools/test_remaster_scripts.py` fails when any `o<n>` word is not followed by `O<n>`, and compiles a script whose table and block end lie past 64 KB. `check_remaster_scripts.py` now needs 99.5% of paired ops the same (the fix raised it from 99.40% to 99.63%). The whole-game replay on Android-only scripts is the test that found it.

### 239. Compiled scripts wrote learned constants where the record had a value

Found by the whole-game replay on the Android-only scripts after root cause 238: Chapter 17 looped from route step 1645. The c_hook trick in st14_game033 never started Lynne's act, because the pipe puzzle never sent `pipe_clr`. Op 0x92 (`SC_GET_DIST_X`) compared `pipe_ofs2` with `pipe4` at offset 0 where the cartridge's script uses offset 2. The class table gave that word as `c0`: every paired sample the rules were learned from had 0 there, so a constant fit as well as the record's own field. A sweep found 31 classes the same way: high halves of 32-bit fields written as 0, string fields written as one DS string offset (`SC_EFFECT_PAUSE` `c432`), and single-sample classes whose every word was a constant (`SC_CICELTALK_FLAG_ON`).

The class: a word rule that is a constant while the record holds a field that fits every sample.

Guard: those words now read the field (`hi<n>` after `lo<n>`, the unread field in order, `s<n>` for names). A field no rule reads must be listed in its row's `unread`, and `tests/tools/test_remaster_scripts.py` fails otherwise. It also checks that the distance op keeps both offsets. `SC_CODE32`, `SC_SETPAL` and `SC_MOVIE_MOTION_SUSPEND` keep their constants, because no field fits their samples.

### 240. The DS set's manifest listed more animations than the app reads

Found on the Vita: with Visual Filter on DS the app found the DS set but logged `manifest invalid code=-2` and `viewer ready has_assets=0`, and drew nothing. The DS converter listed all 5048 of its animations in `manifest.txt`, while `asset_manifest_parse` holds at most 512 and refuses the whole file past that. The remastered converter lists none, so only the DS set failed. The manifest is the port's own browse list for the asset viewer; no native function reads it, and the game finds each animation by its file path.

The class: a converter output longer than a fixed table in the app.

Guard: `asset_writers.manifest_text` refuses more scene, background or animation names than the app holds, so the bake stops instead of writing a set the app rejects. `tests/tools/test_asset_writers.py` reads the limits from `include/asset_manifest.h` and fails when the two disagree. The DS set no longer lists its animations, the same as the remastered set.

### 241. A manifest list past the app's table stopped the whole set

The app side of root cause 240. `asset_manifest_parse` returned an error at the first name past `ASSET_MANIFEST_SCENE_MAX`, `ASSET_MANIFEST_BACKGROUND_MAX` or `ASSET_MANIFEST_ANIMATION_MAX`, and `viewer_app` then ran with no assets at all, though the game finds every file by its path and only the debug browser reads those lists. Root cause 240 stops the converter writing such a set; a set already on a card still had it. No native function reads the manifest, which is the port's own.

The class: one optional list too long, and the reader throws away everything.

Guard: the parser keeps the names that fit, checks the rest for safe characters and counts them in `names_left_out`, which the `manifest loaded` line logs. `viewer_app_show_animation` only refuses a name the manifest lacks when no name was left out. `tests/unit/test_asset_manifest.c` parses a list one past the table and expects the set to load. The smoke test's `boot_title` flow fails when the manifest cannot be read (`make smoke`, and `make smoke-mutations` breaks the path to prove it).

### 242. Visual Filter on the title quit the app instead of switching sets

Found by the smoke test's `visual_filter_relaunch` flow, the first run of the whole app on the Mac with both sets: after Visual Filter changed on the title screen, the settings were saved and the app exited without starting again, so the player was dropped to the LiveArea. `main` asked `viewer_app_wants_relaunch()` after `viewer_app_stop()`, and stopping closes the title menu, whose request the answer is read from. The relaunch through `sceAppMgrLoadExec` is the port's own; native has one set and no such switch.

The class: asking a module a question after it was stopped.

Guard: `main` reads the answer before the stop and logs it as `exit clean relaunch=`. The flow runs the change on an install with both sets and requires the launch to end in a relaunch into the other set; `make smoke-mutations` puts the two lines back in the old order and requires the flow to fail.

### 243. A softlock looked like a slow test

Found while proving the smoke test: with CROSS broken on purpose, each whole-chapter flow ran for a quarter of an hour before its `until` line gave up, because a line that waits for the chapter's end must allow a whole chapter's frames. A test that takes that long to say a softlock happened will not be run.

The class: one time limit for a long wait, sized for the good case.

Guard: the flow runner's `stall <frames> <text>` line fails a flow once that many frames pass with no log line holding the text; the fast-forward flows watch `route_step_done` with 20000 frames, fourteen times the longest gap in a good run, and a broken story now fails in under a minute. `tests/unit/test_smoke_run.c` covers the line.

### 244. Three looping sounds loop past their own end and play once

Found by the smoke test's `set remastered loop points` check, and by `sound bad_loop_file` in the whole-story flows of Chapters 4, 7, 8 and 11: `sounds/gim_189.loop` held `2 41438` for a sound of 41438 frames, `gim_232` `2 92927` of 92927 and `gim_279` `0 61393` of 61393. The app takes the second number as the last frame of the loop and refuses a loop whose last frame is not below the frame count, so the three played once. The loop points are the remaster's: the bake copies the srcd wrapper's two words at 53 and 57. Of the APK's 79 looping streams, 67 at 44100 Hz and the 2 at 48000 Hz end the loop on their last frame, frame count minus one; 7 end it earlier; only these three give the frame count itself. So the word at 57 is the last frame, as `audio_file.py` reads it, and the three are the remaster's own off-by-one, which its player clamps where ours refused. No native function is involved: the DS plays these sounds from its sequence archive, and the converter's DS paths already end on `loop_end - 1`.

The class: a loop end copied from a source without checking it against the sound actually written beside it.

Guard: `audio_file.loop_text` takes the baked Ogg and clamps the loop's last frame to the frame count its last page gives, minus one, and refuses a loop that starts past the end. Every `.loop` the converter writes passes through it (remaster music and sounds, DS music and DS sounds). The app's two readers share `loop_file_read` in `src/app/loop_file.c`, so music and sounds apply one rule. `tests/tools/test_audio_file.py` and `tests/tools/test_end_to_end.py` bake a loop ending at and past the frame count and require it to end on the last frame; `tests/unit/test_loop_file.c` refuses a last frame at or past the frame count; the smoke test's loop point check fails a set holding one; `tools/mutation_check.py` moves the clamp and the bound by one and requires the tests to fail.

### 245. Modes inside ported ops logged a note and did nothing

Found by reading the whole-game replay's marks for `_not_ported`: an op that is ported can still hold a mode that is not, and the note names only the first object that reaches it. Four were reached.

- `GameApp` `EventFrame` (3 sends). Native's command (`func_02013518`) clears bit 2 of `game+0xc` for `On` and otherwise sets it and closes the film frame (`func_02010d18`), as op 0x70 does. Only `func_02010ca4` and `func_02010cdc` read that bit, and ops 0x6f and 0x71 clear it before they call them, so `On` changes nothing to see.
- Beam ramp mode 2 (op 0xbc, st14_game020's `spot`). Native's `func_0205a458` ramps the beam's own position for mode 2, its width for 1, its far end for 4 and its alpha for 8, and ignores any other mode. The port kept the position fixed.
- Light circle modes 8 and 16 (op 0xbf, st03_game012's `demo_light`). Native's `func_02059f3c` writes `mbFixPos` for 8, and for 16 the node `mCenterPosId` together with the object that ran the op. `func_0206241c` then moves the circle to that object's node every frame, and `func_02062468` draws it at screen coordinates when `mbFixPos` is 1, at the soul's core when it is 2 (`func_02010194`) and in the world otherwise. The create op 0xbe carries the same two fields in its tenth and eleventh words, which the port never read: 14 of the game's 37 creates pin the circle to the screen or to the soul's core, and 8 follow a node.
- Framing with a negative speed (op 0xca, st03_game012). Native's `func_0200f634` then also moves the camera's height to the middle of the core and the followed actor, 20 rows above centre (`data_020a2f28`), at the speed's size. The port kept the height.

The class: a switch inside a ported op whose default case logs. The note is keyed by the object's name, so one note can hide several modes.

Guard: `tests/tools/check_script_modes.py` reads the engine's own tables and scans every script of both sets for a `GameApp` verb, an `EyeCatch` kind, a beam or light mode, or a motion-table word the engine has no case for; it finds none, so no script in the game reaches `eyecatch_kind_not_ported` or `motion_word_not_ported`. `tests/tools/test_script_modes.py` checks the scan on made-up scripts. `tests/unit/test_event_frame_command.c`, `test_light_ops.c`, `test_pinned_light_draw.c` and `test_framing_both_ways.c` run each mode.

### 246. Visual findings could only be checked on the device

The Chapter 1 list in the README dated from 2026-11-07 and nothing on the Mac could show whether an item still held, so fixed items stayed listed. `tests/golden/scene_render.c` now plays the real app code on the Mac with auto-play: `tests/host/host_platform.c` reads the baked set from disk and `tests/host/host_renderer.c` draws the renderer's calls in software (the Vita's blend, tint and flat modes, DXT1 and DXT5). It reaches a tick, route step, script or act with fast-forward and saves frames at normal speed.

Checked with it against `chapter1_720p60.mp4` (the footage's 4:3 picture is 720 px wide at the port's own 2.8125 px per DS pixel, so the two overlay directly):

- The stairs walk is not slow. From the room appearing to "It's done." takes 12.3 s in both, and the camera's drop starts 3.7 s in, in both.
- The stairs room's camera is not off. The port's picture sits exactly 120 px left of the footage's, which is `(960 - 720) / 2`: the camera is at the room's left edge, so the wide view slides to stay inside the art, as `docs/asset_format.md` says. Where the room is wide enough (the corpse's corner, 12 s later) the shift is 0.

Guard: none in `make test`; the tool needs the baked set. Run it before listing or closing a visual finding.

### 247. The split-screen's black left panel was measured on a picture the device never draws

The README said the phone call's split screen kept its left panel black for about 1.4 s. Drawn every tick from tick 99700 (the device draws every frame), the port's left panel darkens over 6 ticks as the right panel slides in (99796 to 99801), is fully black for 9 ticks (99802 to 99810) and opens from the middle over 10 (99811 to 99820). The footage at 60 fps from 1907.55 s darkens over 7 frames, is fully black for 10 and opens over 7. The doors are `phone_split.c`'s 16 steps of 16.875 px, so the opening's length in a fixed window depends on the window's height; both open within the same third of a second. Nothing to fix in the port.

What did differ was the renderer. A strip started at tick 99780, 10 ticks before the split takes its snapshot (`PHONE_SPLIT_CAPTURE_AGE`, age 13), showed the whole picture black for 20 ticks. `scene_render`'s fast-forward only updates and never draws, so no art is requested on the way; the first ten frames drawn could not load the room before the snapshot was taken, and the snapshot kept the empty screen. A strip started at 99700 shows the room. The class: any snapshot the app takes on a set tick (the split's capture, the ghost view's entry at tick 11, a film still) taken within the first frames after the fast-forward. Drawing through the fast-forward would fix it, but the software renderer takes about 50 ms a frame, which makes a whole-chapter strip take hours.

Guard: none in code. The tool's README row says to start a strip at least a second before the moment under test.

### 248. Props in the phone view were tinted, not silhouetted, so see-through art stayed see-through

In the ghost talk after the first phone ride (st01_game014, footage 1949 s, port tick 102214), the footage draws Lynne's umbrella (`lkasa`) as one solid red shape, about (200, 30, 4) all over. The port drew it dark in the middle, (57, 15, 7), and redder at the stripes, with the scaffolding showing through. In the phone view every object that is not human takes the phone-view prop style, and that style was the row of class 0x789c5740 (`PHONE_VIEW_PROP_CLASS_ID`), which is not flat: its red multiplied the remaster art's own colours and alpha. The umbrella art is green and white and partly clear, so it came out dark and see-through. Native draws ghost-view silhouettes as one colour wherever the sprite has a pixel (the DS sprite's palette is swapped, `func_02052a7c` picks the class by the ghost); the port's flat paint is how it draws the human and core silhouettes already.

The class: any object in any phone view whose remaster art has colour or partial alpha, in every chapter with a phone ride.

Guard: the phone view's style is its own named row, `object phone_view` in `soul.txt` (`game.json` `soul_view.objects`), flat and turning with the ghost, found by `script_tables_phone_view_style`; the bridge no longer borrows a gimmick class's row. `soul_text` gives object rows the same red-turns flag as class rows. `tests/unit/test_soul_ghost_turn.c` reads the row; `tests/tools/test_ghost_plates.py` fails when any named silhouette row of the game is not flat or its turn flag does not follow its colour. Rendered again, the umbrella is (184 to 223, 30, 19), solid.

### 249. The core ring was a guessed soft glow

Effect 4 (kind 0x23, `cicelcore` and `shadankicore` in st01_game000, `mark` and `mark1`/`mark2` in st03, `core_a` in st13) drew `soul_glow`, a soft disc that grew from 75% to full size every 60 ticks. No remaster effect list has a row for kind 0x23. Native gives ids 4, 7, 8 and 9 the same key (slot33 0x4a0, `func_02066b80`) and loops id 4 (slot88). The footage at 124 to 129 s, measured per frame with a radial profile around the core, shows a thin white ring that restarts every 70 frames: radius 40 footage px on its first frame, 57 by its third at full strength, then about 0.6 px a frame out to 82 while it fades almost linearly to a tenth, and gone after frame 44. The remaster's art for it is the ring cell of `ig_010207_00` (the sheet the player mark already uses, `ui_mark`): its line sits at radius 85.5 of 182 px.

The class: effect rows drawn from an invented pulse rather than the remaster's art and a measured timing. The other `ring` rows (7, 11, 12 and 13, the tutorial's touch markers, kinds 0x100 to 0x104 in the remaster) are the same guess and are not measured yet.

Guard: `effects.txt` takes `pulse` lines (period, then tick, size and alpha keys, run in a straight line between keys and hidden after the last), and a ring row whose texture names a pulse draws by it. Effect 4 draws `core_ring`, the cropped ring cell, with the keys 0 (64 px, 0.65), 3 (91 px, 1.0) and 44 (131 px, 0.105) over 70 ticks. `tests/unit/test_effect_pulse.c` reads and runs a pulse; `tests/tools/test_table_effects.py` fails when a `ring` row draws neither a pulse nor a flipbook and is not in `RING_ROWS_STILL_ON_THE_GUESSED_GLOW`. Rendered every tick from 6600, the port's ring restarts at 6679 and 6749, is 43 Vita px (57 footage px) at its third tick and 61 (81) at its forty-third, and is gone after its forty-fourth.

### 250. The remastered set keyed effect art by a shifted DS kind, so stage 6 drew one effect's art for another

Item 10 of the known issues asked for a fresh look at effects 8 and 9. The remaster's own id-to-kind table (`remaster_tables.json` `effect_kinds`, read from `libruntime.so`) differs from the cartridge's (`data_02086624`) in 85 ids. A remastered set built with the ROM read the DS kind and turned it into a remaster list row with one rule, "one later from 0x86" (`effect_list_row_of_kind`). The real shift starts at 0x84, 0xd8 does not move, and the kinds from 0xf6 up are renumbered. So id 209 (DS 0x84, the remaster's 0x85) was looked up at 0x84 and found nothing, and id 61 (DS 0x85, the remaster's 0x86) was looked up at 0x85 and took `st06_ef_doughnat`, the art of 209 in st06_game001's list. Effect rows are kept per id, so every scene that spawns 61 (`ST06_EXCLAMATION`, also from st03_demo010's tables) drew the doughnut, and 209 drew nothing. `check_remaster_tables.py` already listed 61 and 209 as remaster changes; only the converter's lookup ignored them. A build with `--apk-only` keyed by the remaster's table and was right.

Fresh look, with the remaster's own kinds over all 144 effect lists: kind 0xd8 (effect 8, `efc_tsunagu`) and 0xcf (effect 9, `efc_chain`) have no row in any list, nor do 0x23 (4, drawn since root cause 244), 0x24 (5), 0x03 (6), 0x21 (42), 0x22 (43), 0x86 (61), 0x8d (62) or 0x83 (207). These effects have no art in the remaster. 32 more spawned ids are class effects with no port (50, 54, 56, 57, 59, 64, 65, 99 to 102, 118, 119, 146 to 151, 165 to 168, 191 to 193, 227 to 229, 242, 243 and 270).

The class: any effect id whose remaster kind differs from the DS kind in a set built with the ROM, and any spawned id the README does not name when it draws nothing.

Guard: the remastered set always takes list rows from the remaster's table (`convert_assets.remaster_effect_rows`, read only when an effect needs a row, and an error naming `remaster_tables.py` when it is missing), and `set_up_game.py` reads the tables whenever it builds that set. `Arm9Tables` has no list-row lookup any more. `tests/tools/test_table_effects.py` keys 61 and 209 by a remaster table and fails if the DS lookup comes back. `tests/tools/check_undrawn_effects.py`, run by `make route-check` for the remastered set, fails when a spawned id has no row and is not in its list (the README's), or when a listed id gains one; `tests/tools/test_undrawn_effects.py` checks it. The DS set draws only decoded cartridge art and logs the rest, so it is not checked.

### 251. "Soul view class gates" were drawn long ago

The README listed "Soul view class gates. They are not drawn." The words come from the 2026-11-06 README's "Object tints and class gates are not drawn": which classes draw in which view. Root cause 44 moved that rule into `script_classes.c` (`script_class_draws_in_living_view`, `script_class_draws_in_soul_view`), and the bridge applies it to every actor (`scene_stage_set_object_views`); the tints are root causes 243 and earlier. `tests/unit/test_ghost_copy.c` checks the gates. Nothing is left to draw, so the line is gone.

### 252. "Film-still bubbles 40% too wide" compared a 4:3 frame with the wide screen

The README said film-still bubbles were about 40% too wide and their close squashed. In the film still after the rescue (footage 1013 s, port tick 51553), the talk box's height, its text's size and the text's place from the box's left edge match within a few pixels once the footage's 4:3 picture is scaled to 720 by 540. The box spans the picture in both: 716 of the footage's 720 px with rounded corners just inside its edges, and all 960 px of the port, whose scene fills the wide screen (the README's display rule). 960 over 716 is the 1.34 the line measured. The opening's "So long, sister." balloon is about 330 Vita px wide in both.

The close, at 60 fps around "(Unconscious, eh?)" (footage 791.0 s, port tick 40163): in both the box slides down off the bottom while the portrait slides out to the left; the footage's box is gone after 6 frames and its portrait after 6 or 7, the port's after 7 and 8. Neither squashes.

Guard: none needed; the README states the wide-screen rule once.

### 253. Opening narration pacing was measured with the talk pages included

The README said the opening's narration ran about 9.6 s longer in total than the footage, its first page alone about 4 s, and talk pages cycled every 2.8 s against 2.5 s. Measured from the first narration page to the swirl, the port takes 74 s and the footage 75 s; the first page takes 7 s and 8 s. Talk pages wait for the player's press in both games, so their length is how fast the player presses: the footage's player held two pages for 3.5 and 4.0 s, and the port's auto-play holds them for 3.0 and 2.8 s by its own pacing. Neither is game behaviour.

Guard: none needed; auto-play's pacing is a test harness choice.

### 254. "A second speech bubble is missing" did not hold up shot by shot

The README said a second speech bubble went missing late in Chapter 1, in the footage's 1200 to 1250 s and 1320 to 1380 s. Shot by shot, every balloon there is drawn in the port (ticks 62000 to 72000): "So long, sister.", the top box "Just a few more steps!", Lynne's "!", "Oof!", both "Hold it!", "Who's there?!", the second "!", the "×××" balloon together with "That's because I put it up.", "You hear something?", "........" and "Oh!". Two balloons share the screen in both wherever the footage shows two. What does differ is how a thought balloon opens: the footage's rises from the speaker as a dot and shows "···" while it grows, the port's wobbles blank. That is listed in the README on its own.

Guard: none; the README line now names the opening.

### 255. "Short black or white holds" were one movie end, not a pattern

The README said the port had short black or white holds the footage does not have. The whole chapter was rendered every 20 ticks (`scene_render game tick:30 5440 20`, 5440 frames to tick 107386) and each frame's luma kept; a hold is a run of frames all at or under 56 (black) or all at or over 225 (white), the same test as the footage's 60 fps scan. The port has 23 black and 9 white holds. Placed on the footage's time through five anchors (ticks 6679, 40163, 51553, 64460 and 99796 at 124.5, 791.0, 1013.0, 1246.0 and 1907.6 s), each lies within a few seconds of a footage hold of the same kind: the opening's cards, the swirl cuts into and out of the ghost view, the film stills' white flashes, the chapter card, the rewind clock and the closing scenes. The footage has holds the port does not: the player opening the People screen at 1679 and 1697 s, which the route does not do.

One pairing differs inside the hold. At the forward time jump (port tick 73049, footage 1409.5 s) the footage holds white about 0.75 s and fades to black, while the port goes from the movie's last white frame to 16 black ticks during the scene load and only then fades from white. That is the README's one remaining line about holds.

Guard: none in code; the README names the movie end.

### 256. The DS set had no faces and no interface art, because one of the cartridge's two packs was never read

Found on the Vita: with Visual Filter on DS, every talk drew its text with no box and no face, and the log listed `talk_box_top`, `talk_box_bottom`, `ui_choice_buttons`, `ui_tap_balloon`, the rewind rocks and more as missing. The DS converter read only `cpac_2d.bin`. The DS draws its faces and nearly all of its interface on the 3D layer, as textured quads whose textures sit in `cpac_3d.bin` block 0 (`.tds` files, 1,136 of them, loaded by the same entry loader `func_0201e664`). The face table at `data_02085724`, which the remastered set already reads for its own faces, names a face by its entry in that block, so face 3's first field, 216, is texture 216. Entries from 322 on come in groups of five, one per cartridge language.

The class: art the DS draws from a pack the converter does not open.

Guard: `tools/assets/ds_textures.py` decodes the five texture formats the cartridge uses (4, 16 and 256 colours, A3I5 and A5I3) and refuses anything else. `ds_set_text.py` reads each page's face, pose and head codes, as the remastered set does, and bakes `ds_face_<entry>` at DS size for every face a page names; the head icons come from the cartridge's own head table (root cause 261). `tools/assets/ds_ui.py` builds each interface texture the app names from the parts `ds_textures` gives it in the set description: a crop of a cartridge texture, scaled to the nearest pixel into the place the app's layout reads, with nine-slice parts for panels. A texture whose source is missing or does not decode is left out and named on one line, so the app still logs it and draws nothing. `tests/tools/test_ds_textures.py`, `test_ds_ui.py` and the face tests in `test_ds_set_text.py` cover the decoder, the composer, the page fields and the description.

What the DS really shows is read from the cartridge running in melonDS: `tools/ds_screen_dump.py` presses buttons on a schedule, saves the video memory and captures the main screen's finished picture, 3D layer included, through the DS display capture unit; `tools/ds_screen_render.py` draws both screens from a dump. The top talk box was matched against such a capture (a 208 by 48 box at x 24, built from texture 23). The bottom box uses the same texture and has not been seen in a capture, because the captures stop at the first touch tutorial: the tools press buttons but cannot touch.

### 257. The DS set's HUD buttons could not be built from the remaster's pill pieces

The app builds each HUD pill from pieces of two remaster textures: a ring in three slices, a fill it draws itself, an icon, a word and a Vita button. The DS has each button as one finished texture, resting and touched (`cpac_3d.bin` block 0: Ghost 542 and 547, Back 552 and 557, Trick 582 and 587, Swap 562 and 567, Cancel 572 and 577, the notebook 91 and 92, the hourglass 93 and 94, Jowd 95 and 96, the switch to Missile 97 and 98 and to Sissel 99 and 100), drawn by the HUD board `func_0204f850`. No crop of those fits the ring, word and icon cells, so the DS set had no pills at all and the player could not see which buttons were up.

The class: interface art the DS keeps whole where the remaster keeps parts.

Guard: a sharp set (`texture_cache_sharp_art_is_on`) draws each pill as one sprite from `hud_pills`, a sheet with one row per `script_pill_id` and a resting and a pressed column, each cell 88 by 32 DS pixels. The converter trims each cartridge texture to its ink and sets it against the side of the cell that faces the screen edge (`cell` and `align` parts in `ds_ui.py`), and the app places the cell the same way: left pills from their left edge, right pills from their right edge, the rest centred, 4 pixels from the top or bottom. With no sheet the pills draw nothing, and the remastered set is untouched. `tests/unit/test_script_ui_view.c` checks every pill draws once from its own cell and stays on screen; `tests/tools/test_ds_ui.py` checks the sheet has two cells for every pill in `script_outputs.h`. Not done: the pills carry no Vita button, the green pills of Missile's ghost world (592 to 607) are not used, and the placing has not been seen on the device.

### 258. DS sprite pages taller than the app's texture limit drew as magenta blocks

The DS converter packed every frame of a resource onto one page, 1024 wide and as tall as the frames needed. The cartridge holds each frame as its own cell and draws them one at a time (`func_0200b598`, `func_0200bf54`), so nothing on the DS bounds a resource's total size. Four junkyard resources (3773, 4039, 4319, 4320) came out taller than `TEXTURE_FILE_MAX_DIMENSION` (4096), the app refused them (`texture missing name=ds_sprite_3773 reason=header code=-4`) and the crane and platform drew as magenta placeholders on the device.

The class: converter output larger than a limit the app enforces, the same as root cause 240's manifest list.

Guard: `place_frames` in `ds_set.py` starts a new page when a frame would pass 2048 rows, pages after the first are named `ds_sprite_<resource>_<page>`, and each animation frame names its own page. `tests/tools/test_ds_set_guard.py` reads the limit from `include/texture_file.h` and checks no page of a tall resource passes it, that every frame lands whole on one page, and that a frame names the page it was placed on.

### 259. DS mode had no title screen art and its pause menu never opened

The title, pause menu and loading screen are the port's own, and the converter drew their sheets from the Android package: a font file for every word, shapes it draws itself, the cat from remastered pages, a backdrop and a logo. The DS set reads no package, so it baked none of them. The title showed a red line on black, and START did nothing, because `pause_menu_is_ready` waits for the cat's frames, the labels and the widget table.

The class: art the port builds for itself from one set's sources, which the other set then lacks whole.

Guard: `ds_pause.py` builds the same sheets and tables, cell for cell, from the cartridge. Words are written in the cartridge's main font (`fonts2bpp_en.imb`, the one `ds_text.py` reads for talk) at two screen pixels a DS pixel, labels at three with a one-pixel rim. The title buttons are the cartridge's orange buttons (`cpac_3d.bin` block 0, textures 192 resting, 193 selected, 194 pressed, loaded by `func_0201e664`), the meter flame is texture 31, and the port's own pills, arrows and bin are the remastered shapes hardened to whole DS pixels. The cat is the DS set's own `st01_neko` animations: `pause_art.cat_frames` now takes its pages from whoever holds them, and `ds_set.SpritePages` hands it the sprite pages the cartridge frames were packed on (`func_0200b598`), stored pixel for pixel. The logo is cut out of the cartridge's title picture (texture 1042) by a `cut_out` part in `ds_ui.py`, which keeps what is not the red backdrop. The backdrop is a flat red `fill`, the user's choice on 2026-11-15, because every cartridge picture with that backdrop has a logo or chapter text over it. `tests/tools/test_ds_pause.py` checks the DS sheet has every cell the remastered sheet has, that every menu string can be written in the DS font (the four languages the cartridge never had are named and left out), that the sheet fits the app's texture limit and that a label fits its cell.

Not done in DS mode: the clock (`pause/clock.txt`; the Record came with root cause 303), the spotlight under the cat, the option preview plate, and the Vita button glyphs, which sit on `hud_sheet`.

### 260. DS talk text sat far from the left edge of its box

The DS set places talk text where the DS does: the window's own left edge and margin (`data_02084a8c`), mapped into the middle 720 pixels of the screen. The app draws its boxes where the remaster has them: the bottom box across all 960 pixels, the top box 584 wide at 188, or at 255.5 beside a head icon. So text began 120 pixels further in than the bottom box and 67 further in than a top box with no head, and looked indented.

The class: DS content placed by the DS screen inside a frame the app places by the remaster's layout.

Guard: `talk_message_rows` in `ds_set_text.py` moves each page's lines left by the distance between the DS window's mapped edge and the edge of the box the app draws for that page (`APP_BOX_LEFT`), so the text keeps the DS margin inside the box. Centred system lines are not moved. `tests/tools/test_ds_set_text.py` checks the margin for a bottom page and a top page with no head, and reads the three box edges out of `script_view.c` so the two cannot drift apart.

### 261. Ray's speech bubble showed Missile's head

Root cause 256 gave the DS set a hand-written list of head icons copied from the remastered set's four entries, and it sent head values 4 and 5 to Missile's picture. Native reads the icon from a table: `func_0201d95c` indexes `data_02099cb0` (6-byte rows) with the head value less one, and the row's second word is the `cpac_3d.bin` texture, 0xffff for no icon. Value 4, Ray, is the desk lamp (314); 2 is Missile (315); 5, 6 and 14 have no icon, which is how the game keeps the first voice in Chapter 1 unseen.

The class: a table the cartridge holds, retyped by hand from the other set.

Guard: `read_head_textures` in `ds_set_text.py` reads all 14 rows from the cartridge and the set description lists none. `tests/tools/test_ds_set_text.py` checks a row with no icon gives a top box with no head. Not checked: the remastered set's `head_faces` still lists only values 1, 3, 4 and 5, with 4 and 5 on one file.

### 262. Switching the Visual Filter restarted the game without warning

The Visual Filter row flipped on one press, and the app then restarts to load the other art set, so a player lost the progress since the last save with no notice.

Guard: the row opens the options' yes or no dialog first (`row.filter_ask`, `bar.filter_warning`, in every language) and flips only on yes; a pinned filter still does nothing. `tests/unit/test_pause_options.c` checks a press opens the dialog, No leaves the filter alone and Yes switches it.

### 263. Talk text sat further from the box's left edge than from its top and bottom

In both sets the first glyph of a talk page began further in from the box's left edge than the text block sat from the box's top and bottom: the remastered set placed it 40 pixels in from the bottom box (15.5 above and below a full page) and 24 in from the top box (21.5), and the DS set kept the DS window's own margin, which root cause 260 measured against a different frame. The user asked for the left margin to equal the top and bottom margin, the same way in both sets.

The class: a text margin set by hand per set and per box, so the two sets and the three boxes drifted apart.

Guard: both talk bakers pass their finished rows through `talk_assets.even_left_margins`, which measures, for each box kind, the median top-and-bottom ink margin of the fullest pages that fit inside the box, and moves every page so its ink starts that far from the box's left edge (`BOX_LEFT`). Centred system lines are left alone. It replaces root cause 260's `APP_BOX_LEFT` shift. The remastered set lands at 15.5 (bottom) and 21.5 (top), the DS set at 10 and 16, since its font and line pitch are the cartridge's. `tests/tools/test_talk_margins.py` checks the margin on synthetic pages, that a fixed talk stays, that a page spilling out of its box does not set the margin, and that the box edges match `script_view.c`.

### 264. The DS pause and loading cat was drawn with hard pixel steps

Root cause 259 baked the DS cat at its DS pixels, enlarged with no filtering. The user asked for a slightly smoothed cat at the same size and from the same sprites.

Guard: `pause_art.SOFT_FILTER` keeps the nearest-neighbour enlargement and then runs a 1-2-1 blur on premultiplied colour (`softened`), so each DS pixel's edge blends over one screen pixel without darkening the outline; the page stays uncompressed RGBA. `ds_pause.bake_cat` uses it. `tests/tools/test_ds_pause.py` checks the size and colour are kept, the edge is part-covered and the page stays RGBA8888.

### 265. The soul and its line to a core did not draw in DS mode

The app draws the soul in the ghost world from four textures the remastered set cuts from Android art: flat blobs for the body and puddle (`soul_blob`), an orb at its base (`soul_orb`), the glow on each core (`soul_glow`) and the band to the next core (`soul_beam`). The DS set had none of them, so only the possessed object's lavender tint showed. A melonDS capture of the Chapter 1 tutorial shows the DS draws the soul the same way: a flat cyan body, a lavender-white orb at its base, a blue glow on the cores and a pale band, from `cpac_3d.bin` block 0 textures 6 (a disc), 15 (a small glow), 14 (a soft glow) and 28 (a band), with the body in texture 26's cyan.

The class: an app texture slot with no `ds_textures` entry, found only when someone sees it missing on the device.

Guard: `ds_textures` fills the four slots, and a part can take the main colour of another cartridge texture (`flat_color_of`), so the blob is the cartridge's own cyan. The blob keeps the 59% disc share `script_view.c` draws it by. `tests/tools/test_ds_texture_slots.py` collects every texture name the app's source asks for, and the banner names, and fails unless each has a `ds_textures` entry or is in its list of known cartridge gaps, which names why. `tests/tools/test_ds_ui.py` checks the colour rule and the blob's disc share. `soul_ring` was dropped from both sets: nothing drew it, the app only preloaded it.

Not done: the flower of lines around the soul (`soul_dim`, a flipbook in the remastered set) and Ray's light, which is effect movie `ef01_light_000`/`001` (effect ids 84 and 91) in the remastered set; the DS set bakes no effects yet.

### 266. The DS pause menu showed a red block in a corner and a cat in pieces

Seen on the Vita: in DS mode the pause menu drew a red rectangle in the top-left corner over a dark screen, and the cat showed only its eye, an ear and its red scarf. `pause_menu.c` drew the backdrop at its own pixel size, which filled the screen only because the remastered backdrop is 960 by 544. Root cause 259 gave the DS set a flat red backdrop 240 by 136, so it covered a quarter of the width, and the black cat stood on the dark clear colour where only its light parts showed. The cat pages were whole. The loading screen already stretched the same texture to the screen.

The class: a screen-filling texture drawn at its texel size, which holds only while every set bakes it at screen size.

Guard: the pause menu stretches the backdrop across the screen (`draw_across_screen`), as the loading screen does. `tests/unit/test_pause_menu.c` checks an 8 by 8 backdrop is drawn over the whole screen.

### 267. The TRICK, TRACE and FATE banners had no words in DS mode

The centre labels between the red lines drew nothing, and the log listed `banner_trick`, `banner_trace`, `banner_averted`, `banner_changed` and `banner_star` missing. A search of `cpac_3d.bin` block 0, where the interface textures are, found no such lettering, because the DS keeps it in effect files. The eyecatch command (`func_02013a88`) sets game modes 9 to 0xd, `func_02032488` starts `pGameEyeCatch`, whose init (`func_020318c4`, `func_02031b88`) spawns a generic effect with kind 326, 331, 241 or 211 from `data_02099d08`, and `func_0204235c` loads that kind from `cpac_3d.bin` block 3 (`func_0201e888(reader, 3, kind)`). An effect file is a count, offsets, a `bws.` script and `sdt.` textures behind an 8-byte header whose bit 0 marks LZ10. In each eyecatch effect, item 2 is the fluted band, item 3 the lettering ("Trick Time!", "Trace Complete!", "Fate Averted!", "Fate Changed!", one per language) and item 4 the star.

The class: DS art drawn by an effect, not by an interface texture or a sprite resource.

Guard: `ds_effects.py` reads block 3 effects, and a `ds_textures` part can name `{"effect": kind, "item": n}`; DS direct-colour textures (format 7) decode too. The five banners crop the lettering to the shape `ui_clips.py` draws them at. `tests/tools/test_ds_effects.py` checks the container, that every effect a set names decodes in all five languages, that every texture item of every block 3 effect decodes, and the banners' shape. Before calling a DS slot a cartridge gap, follow the native class that draws it to the block it loads from.

Not done: the DS fluted band (item 2) has no slot and the app draws its own red band; timing follows the remaster's keyframes, since the `bws.` script is not decoded; the banners draw at about 2.5 Vita pixels a DS pixel against 2.83 for the whole screen.

### 268. DS talk text left a gap where the DS draws a button picture

The DS message text carries icon codes 275 to 286. Native looks each up in `data_02084ae8` (8-byte rows: code, `cpac_3d.bin` texture, width, height; `func_0201bddc`) and places a sprite from `func_02018fa8`, one letter gap past the pen and centred 17 rows below the row top (the sprite's default place, `setPos(-(0x80 - w / 2), -0x40)`, plus the row's y + 1, less 0x90 in the top window), then moves the pen on by the width and two gaps. The DS talk baker kept the advance and drew nothing, so Ray's tutorial read "try the ___ button". The talk tint could only multiply a glyph by a text colour, so a coloured picture had no way to show either.

The class: every icon on every talk page in every chapter and language.

Guard: `ds_talk_icons.py` reads the table and each icon's art per cartridge language, and `TalkGlyphs` bakes each icon as its own colour cell in the talk atlas. A glyph code's tint field is now three bits at bit 13 (`script_talk.h`, `talk_assets.COLOR_SHIFT`), and tint 4 draws a cell as it is (`script_view.c`'s `text_colors`). `tests/tools/test_ds_talk_icons.py` checks the table, the placement, that an icon with no art keeps its gap, and that the baker and the app agree on the tint field. Every set's `talk.txt` must be rebaked with this change, which the converter's fingerprint does on its own.

The port's choice: DS mode shows the cartridge's own pills, d-pad and buttons, not Vita buttons as the remastered set does, because DS mode's HUD draws the same cartridge pills and they work by touch; the cartridge has no CROSS, L or R picture. Not checked: the vertical place against a DS capture (melonDS cannot touch through to the tutorial), and whether icon 281 (texture 982) is the hourglass.

### 269. The DS rewind button had no words

Seen on the Vita: in DS mode the rewind screen showed its red rocks and the orange button, but not "Rewind Time" on the button. The app draws the button as the plain orange shape (`ui_select_button`) and lays the words over it from a cell of the remastered `hud_sheet`, which the DS set does not have. The cartridge keeps each of these buttons as one texture with its words in it: "Rewind Time" is `cpac_3d.bin` texture 672, with 677 and 682 its highlighted and pressed states, one per cartridge language (beside "From Beginning", 642, and "After Fate-Change", 657).

The class: a DS button whose words the cartridge draws into the button, which the app builds from a shape and a label from another set's sheet. Root cause 257 found the same for the pills.

Guard: with sharp art on, the app draws the rewind button whole from `rewind_button` (resting and pressed cells, from 672 and 682) at the pills' 45/16 screen pixels a DS pixel, centred where the remastered button sits. `tests/unit/test_script_ui_view.c` checks a sharp set draws it as one sprite from the resting cell, centred; `tests/tools/test_ds_texture_slots.py` makes the DS set carry it.

### 270. Thought and shout balloons did not draw in DS mode

The remastered set builds its balloon rows in `chapter_assets.expand` from Android bubble art and a text texture per label; `ds_set.expand` never built them, so the DS set wrote an empty `balloons.txt` and every op 0x51 balloon logged `no_balloon_row`. Native draws a worded balloon (`uBalloonPicture`, `func_02044b48`) as one texture that text parser type 1 composes: ovals (`func_0201a254`, `func_0201a680`) from pieces 110 to 113 for one line or 116 to 119 for two or three on canvas 109 or 115, the middle a run of 6-pixel segments as wide as the widest line; thorns (`func_0201a3ac`) from 114, 108 or 107 by width; the palette from `cpac_2d.bin` block 0 entry 7 (`func_0201a97c`); text in the balloon font placed by `func_0201aa88`, control code 3 centring a line (`func_0201a1b4`). The shape grows white over 8 ticks, then the real palette shows the text (`func_02044800`, `func_02044d7c`).

The class: a table the app reads that one set builds and the other never does. And a second: a balloon row found by its label alone, though six labels open with two different kinds.

Guard: `ds_balloons.py` reads the native tables from the arm9 and composes every label and kind op 0x51 opens, shapes shared by geometry (52) plus one text each (309); a part may carry `texture_scale` so DS-size art draws at DS size. `script_tables_find_balloon` matches a row's kind when the row names one (remastered rows keep -1, any kind). `tests/tools/test_ds_balloons.py` fails unless every op 0x51 label and kind in the game chain has a row with baked textures, and checks the oval and thorn rules; `tests/unit/test_balloon_row_kinds.c` covers the kind match. The bake matches the melonDS capture of "So long, sister." pixel for pixel.

Not done: op 0x52's "!" and other bursts (`uSwfPlayForBalloon`, effect art), op 0x45's tap balloons, the close animation, and two- or three-line and thorn layouts against a capture.

### 271. DS mode drew no effect movies: not the soul's flower of lines, not Ray's light

The DS set listed no effect movies, so DS mode left out every effect a script spawns as a movie, among them the flower of lines around the soul (effect 31) and Ray's light (84 and 91). Native builds every effect whose kind (`data_02086624`) is below 0x8000 as a `uScriptGameEffect`, a `uSwfPlayGame` (`func_02052ad8`, `func_02066af8`); its `func_02066074` loads the movie through `func_0204235c(kind)`, which reads `cpac_3d.bin` block 3 entry `kind` (`func_0201e888`). That entry holds a `bws.` movie (shapes, sprites, frames with matrices, 5-bit colour and percent alpha, and ActionScript-style frame scripts) and its own LZ textures. The flower is entry 29: ten two-colour quarter textures, each drawn four times turned, at 15 fps, looping.

The class: effect art the cartridge keeps as SWF-like movies, which the converter did not read; the remaster bakes the same movies into frame pages.

Guard: `ds_effect_movies.py` reads the `bws.` movie, walks its timeline the way the remastered movies play (`effect_file.playback_of`, now shared by both sets), renders each frame from the cartridge textures and writes the same movie rows and pages for every spawned effect id of a sprite kind that has no row of its own; `only_in_soul_view` and sounds come from `game.json` by id. Movies over 16 MB of pages, or drawing nothing at bake time, are left out and named. `tests/tools/test_ds_effect_movies.py` checks matrix order, colour, `setVertex` corners and nested playback on synthetic movies, and on the cartridge that every block 3 movie reads, that every spawned sprite-kind id gets a row and that the flower is four turned copies of one quarter.

Not done: ids 42, 240, 248, 249 and 255 (full-screen, over the page limit) and 123, 141, 170 and 216 (driven by status labels the game sets). Branching frame scripts play as plain timelines. The built-in call ids are named from the remaster's records, not from the native interpreter. Not checked on the device; the pale band to a core in the capture may be `soul_beam` (root cause 265) rather than effect 84 or 91.

### 272. DS mode listed languages it had no text or font for

The DS set baked English only, but the app's language list was its fixed nine, so DS mode showed blank rows for the languages the cartridge font cannot write (Japanese, both Chinese, Korean) and offered French, Italian, German and Spanish with English text. The user decided on 2026-11-15 that DS mode shows only the five languages the USA cartridge carries, each baked from the cartridge's own message files.

The class: the app assumed every set carries every language; which languages a set has was never recorded by the bake that makes them.

Guard: the converter writes `lang/languages.txt` from what it baked: the remastered set its nine, the DS set English and each language whose DS code differs from English's and whose script message files the cartridge holds (`ds_languages.py`; fr, gr, it and sp, read from `<script>.<code>.xml.lz` and `system_0000.<code>.msg.xml.lz`). The DS overlays run the shared text, interface, balloon and menu bake per language, with the `cpac_3d.bin` texture group for that language. The app lists, uses and maps the system language only to listed languages and falls back to English without rewriting the saved setting (`asset_language_carries`, `asset_language_carried_or_english`); a set with no list shows all nine, as before. `ds_languages.check_listed_languages` refuses a listed language with no overlay, a list unlike the bake, or a language with no texture group, which replaces a silent fall back to English textures. `tests/tools/test_ds_languages.py`, `tests/unit/test_asset_language.c` and `tests/unit/test_pause_options.c` cover it.

### 273. A Phone book ride after a debug Chapter select went back to the place's first chapter

Reported on the Vita (debug build, Unlock Everything on, a chapter with no save of the player's own): picking a place in the Phone book took Sissel back to the chapter that place first appeared in, rewinding the story. The VM did what native does. Go There sends `phone movie`, the telephone's handler loads `phone.xml`, and `phone.xml` compares register 0x1a (the picked place, `func_0201fc30`) and loads that place's root, for example `st01/st01_root.xml` for place 1. The root picks its scene from story flags 0x1000 to 0x1fff alone (op 0x32 and 0x33 or 0x34, `func_02023fdc`), and op 0x74 (`func_020125d4`) never lowers the chapter mark. But Chapter select without a chapter save (`game_save_for_chapter_select`) entered `chapter.xml` with every flag clear. So every root took its first branch: place 1 opened `st01_game000`, and the chapter start itself opened the wrong scene too (Chapter 9 opened Chapter 6's `st03_game000`, Chapter 13 opened Chapter 4's `st05_game000`). From a played-through state at Chapter 12 the same rides kept the chapter mark and opened each place's scene for that point in the story.

The class: any script that picks its scene from story flags (every stage root, `chapter.xml`, `phone.xml`) after a start that did not carry the story's flags.

Guard: `make read-ahead` now has the whole-game replay (`script_replay --chapter-starts`) write the state it had each time it entered `chapter.xml` (`game_save_capture`, as the app's chapter autosave does) into `scripts/chapter_starts.txt`, one section per chapter from 2 to 18. Debug Chapter select marks its bare save `is_chapter_select`, and `script_player.c` replaces it with that chapter's baked start from the script pack before the chain loads. `tests/unit/test_chapter_starts.c` reads each chapter's section, opens a chapter from its baked start in the scene its story flag picks, checks that a phone ride from there lands in the same scene with the chapter mark, game flags and notes unchanged, and checks that a start without the flags rides back to the first scene. `make route-check` runs `tools/chapter_starts_check.py`: it fails when the set's `chapter_starts.txt` differs from the fresh replay's, when a chapter has no start, when a start opens a scene other than the one the story opens, or when a ride to any place in that start's Phone book opens no scene or lowers the chapter mark. All 17 starts in both sets open the story's scene.

### 274. DS mode's pause, loading and title screens stood on a flat red fill

Seen on the Vita: the chapter cards show the red gear backdrop, and the pause menu, the loading screen and the title stood on a plain red fill. The user asked on 2026-11-15 that they share that backdrop. The cartridge keeps no copy of it without words: the title screen (`cpac_3d.bin` texture 1042, one per language) carries the logo and copyright, and each chapter card (1047 on, drawn whole by the eyecatch, `func_02013a88`) carries its chapter and time. Every copy has the same backdrop under different words, and natively none is drawn without them.

The class: a picture the DS only ever draws with words baked in, which a screen the port adds needs bare.

Guard: a `ds_textures` part may name a range of textures in `text_free_of` with a `text_rule`. `ds_ui.text_free` masks each texture's lettering (green above the limit, grown by `grow` pixels, then anything far from the running median) and takes each pixel's median from the textures whose words miss it. A pixel every texture covers is filled from its mirror across the centre, then from its neighbours. `pause_backdrop` is 1042 to 1135, cropped to 16:9. The user chose this over the remaster's art on 2026-11-15, knowing the time line covers every card's centre, so a band there stays smudged. The cartridge-only LiveArea draws the logo over the same backdrop. `tests/tools/test_ds_ui.py` checks the median on synthetic cards and that the DS backdrop has no fill.

### 275. The Minutes Before Death card drew its bands but no words in DS mode

Seen on the Vita: the card before a death showed its bands without "4 Minutes Before Death", its clock or its hands. Native `uBeforeDeathEffect` (`func_02048100`, `func_02048188`) makes a watch, `uSwfPlayWatch` (`func_02047720`, `func_0204776c`), which plays `cpac_3d.bin` block 3 effect 202 (item 1 the red clock face, item 2 the red ring); its draw (`func_0204791c`) gives the two hands block 0 textures 190 and 191. After 24 ticks `func_02048224` makes the telop (`func_02047bc8`), effect 281, whose item 2 holds the localized lettering. That is the eyecatch banners' route (root cause 267), but `test_ds_texture_slots.py` read only `banner_*` names from `ui_clips.py`, so the card's slots, like every other texture a clip names, had neither a DS entry nor a listed gap and the guard still passed.

The class: a texture slot named only in the clip tables, which the slot guard never read.

Guard: `card_clock`, `card_wave`, `card_hand_hour`, `card_hand_minute`, `card_top_text` and `card_bottom_text` map onto those effect items and textures, each at the shape the clip draws it, the text in every cartridge language. `tests/tools/test_ds_texture_slots.py` now collects every texture `ui_clips.all_clips()` draws and lists the rest as known gaps; `tests/tools/test_ds_effects.py` checks that every clip sprite with a DS entry keeps the clip's shape.

Not done: the middle line (time from textures 177 to 189, place from room labels 687 on, chosen by `func_020265fc` through `data_0209b7b4`) has no app slot; the DS has no seconds hand; `hud_disc`, the death count and `wipe_time_passing` are not traced. Which hand is hour and which minute is read from their length. Not checked on the device or in melonDS.

### 276. The "!" and "?" balloons did not draw in DS mode; only their sound played

Native `func_02044b48` (`uBalloonPicture` start), given no text, makes a `uSwfPlayForBalloon` and plays `cpac_3d.bin` block 3 effect 0x8c (the "?" cloud) for kind 4 and 0x85 (the "!" burst) for every other kind, through `func_02066074`, `func_0204235c` and `func_0201e888(reader, 3, kind)`. Each is a 40-frame movie: the burst (item 2) shows at frame 10, the mark (item 1) at frame 14, both centred on the balloon, and the movie removes itself at frame 39. `ds_set` took its balloon table only from the worded op 0x51 bake (root cause 270), so op 0x52 kinds 2, 4 and 5 had no row and logged `no_balloon_row`.

The class: balloon rows with no text, which native draws with an effect movie rather than a composed texture, and which the DS set never built. The remastered set has a row for kind 5 only.

Guard: `ds_alert_balloons.py` builds one row for each op 0x52 kind in the game chain from the cartridge movie (delay, part order, settled position, grow) and bakes its textures at DS size; `ds_balloons.bake` adds them. It names any kind it cannot place (mirrored kinds 3 and 6, unused by the chain). `tests/tools/test_ds_alert_balloons.py` fails unless every op 0x52 kind has a row with decoded art in every language, and unless `ds_balloons.bake` returns those rows. Checked against a melonDS capture of the opening "!" over Lynne.

Not done: the movie's own end at frame 39 (the app keeps the parts until the balloon's timer ends), the 0.8 to 1.2 to 1.0 overshoot, the burst's 0.9 alpha and the mark's wobble; the row format has no field for them. Not checked on the device.

### 277. DS mode's tutorial talk sent the player to a top screen the Vita does not have

The user asked on 2026-11-15 that DS mode stop naming the top screen. Native prints each message as the cartridge's 1LMG file spells it, through the talk window table (`data_02084a8c`), and the DS shows the trick name, the countdown and the swap shape on its second screen. Four talk messages in every cartridge language name that screen: Ray's tour of the trick name (st01_game001 `m01_0460`), the hourglass (st01_game011 `m01_0660`) and the bullet's shape (st02_game035 `m02_3857` and `m02_3930`). The remaster rewords the first two for one screen ("the middle of the screen", "the right side of the screen") and cuts the bullet's top screen page, because one screen shows no shape. The DS set baked the cartridge's wording.

The class: cartridge text that assumes the DS's two screens, in any language and any message the shared DS text bake reads.

Guard: `ds_one_screen.py` runs over every message `ds_set_text.MessageBook` and `read_system_messages` read, so talk, narration, balloons and system lines all pass through it. `ds_one_screen.json` holds, per cartridge language code, phrase rewrites in the remaster's wording, the pages to cut, and the refused phrases. A rewrite matches the letters and line breaks of a page and keeps every control code inside the phrase in its order, placed where the rewrite's `^` marks say; a cut page hands its controls (speaker, sound, colour) to the next page. A message that still holds a refused phrase stops the bake. `tests/tools/test_ds_one_screen.py` bakes every script and system message of every cartridge language through `MessageBook`, fails on "top screen", "upper screen", "écran supérieur", "oberen Bildschirm", "schermo superiore" or "pantalla superior", fails on any table entry the cartridge no longer uses, and fails on a rewritten line wider than the DS talk window. `ds_text.CODE_OF_CHARACTER` now holds the cartridge's accented letters for every user (the pause menu took its copy from there).

Left as the cartridge has them: "slide the screen" and "Touch the screen" (the Vita has a touch screen), "the button displayed on-screen", the German "Die Sanduhr da oben" (the remaster keeps it), and the system lines that ask to reinsert the DS Card, which DS mode never shows. The cut leaves the bullet talk's next page, "Now I know what it's shaped like", as the cartridge has it. Not checked on the device.

### 278. The dead talk's swirl, its waves and its opening drew nothing in DS mode

Seen on the Vita: in DS mode a dead talk showed a black screen with no red swirl, no moving lines and no opening, while its sounds played. The swirl setter `func_0200f084` with its effect flag makes three things: the opening `uTimeMoveEffect` (`func_0205c044`; `func_0205e4f4`, `func_0205e5fc`, `func_0205e6d0`), the backdrop through `func_0200a200`, and `uDeadKaiwaParticle` (`func_0204e2b4`, drawn by `func_0204e6ec`). The glow and particles are `cpac_3d.bin` textures 176 and 175, the originals of the remaster's art, but `swirl_back` and `swirl_particle` had no `ds_textures` entry. The waves are a 2D affine BG, `cBgCtrl::uBgDeadKaiwa` (`func_0200aaac`, `func_0200ab84`), read from `cpac_2d.bin` block 4 entry 1, which takes the next of its 16 palettes every 16 frames and turns 32/65536 of a turn a frame; no set ported it. The opening (spokes, spun capture and a red ring from frame 3 to 13) was never ported (root cause 203), in either set. Its flag is the dead-talk state's +8: 0 on a normal start, so the opening plays, and 3 for `DeadTalk|Restart` (`func_0203c03c`), so it does not.

The class: art a native BG layer or a code-driven effect draws, rather than an effect file or a texture slot, which a slot audit cannot see.

Guard: `ds_backdrops.py` bakes any palette-cycled cartridge BG named in `ds_backdrops` as a flipbook, one frame a palette, and `swirl_waves.c` draws it wrapped and turning at the native rate under the glow, which then shows at 20/31. The swirl setter carries the native effect flag (`script_machine_set_swirl_with_effect`; on for a normal dead talk and a flagged op 0xe0, off for a restart), and `soul_transition` plays the opening, `TRANSITION_TIME_MOVE`, in both sets. `tests/tools/test_ds_texture_slots.py` requires every flipbook the app names to have a DS source; `test_ds_backdrops.py`, `test_swirl_waves.c`, `test_soul_transition.c` and `test_route_pad.c` cover the bake, the drawing, the ring and when the opening fires.

Not done: the ring's screen size is read from native units (4096 to 96 DS pixels) and may be too large if native projects in perspective; the glow's size and turn and the particles keep the remaster's values; system sounds 14 and 15 and the closing `uSoulCloseEffect` are not added. Not checked on the device or in melonDS.

### 279. The junkyard crane drew as a stretched, blocky jib in DS mode

Seen on the Vita: the crane over the junkyard showed a huge beige jib end and a band of tall lattice under the wrecking ball. The app reads a script's animations from its packed bundle (`scripts/<script>.anims`, inside `scripts.pack`), which only the read-ahead step wrote; the converter kept bundles as they were. Root cause 258 split tall DS sprite pages and changed the texture coordinates in `animations/*.anim`, but the bundles kept those of the old 994x6056 page, so on the new 833x1927 page the crane (`t_topa_wait`, resource 3773) sampled a 349x76 corner and stretched it to its 416x240 frame. 41 of the DS set's 126 bundles were stale (47 animations: the crane, its ropes, `dcraneb`, `jodo`, `bigniku`). Native draws each frame from its own cells (`func_0200b598`, `func_0200bf54`), so the port's page coordinates must match the pages it bakes.

The class: an output built from other outputs and rebuilt only by a separate step, so it goes stale when they change.

Guard: `write_outputs` calls `read_ahead.write_every_bundle` after writing the animations and motions and before packing the scripts, so every conversion rebuilds every bundle from what it just wrote, in both sets. `tests/tools/test_anim_bundles.py` checks that a stale bundle is rewritten, that every bundle equals `bundle_text` of its motions and animations, and that the pack carries it.

Not done: the `.ahead` lists still come only from `make read-ahead`, and name only a split resource's first page; rerun it for the DS set. Not checked on the device.

### 280. The DS phone book listed no contacts

Seen on the Vita: in DS mode the phone book showed its title and choices over empty slots. The app drew each contact as a remastered place photo from `pause/people.txt` (`pause_people_draw_place`), a table only the remastered set bakes. Natively the book (`pGameSubPhoneBook`; list `func_02038848`, items `func_02038e04`) shows a cartridge name plate per place: `func_0203971c` picks the localized `cpac_2d.bin` sprite resource from `data_0209a7cc` (24 places by 3 levels; +1 selected, +2 touched), `func_02038e04` raises the level once the place's list-2 note level reaches the steps in `data_0209a76c`, so a place reads as its number, such as (SYS)-1729, before its name, and `func_0203936c` lays the items on a 7-slot arc.

The class: a DS-mode screen whose content comes only from a table the remastered set alone bakes.

Guard: `ds_phone_book.py` reads both native tables from the arm9 and bakes every place's resting and selected plates, per cartridge language, into `ds_phone_book` and `pause/phone_book.txt`; the app draws them on the native arc at twice DS size through `phone_book_plates` when that table is loaded, the remastered path unchanged. To fit five slots, the title moves up to y 72 and the hints down to y 472 when the plates draw. `tests/tools/test_ds_phone_book.py` checks that every language's text-and-menu pass bakes the book and that every place, level and state has a plate in all five languages; `tests/unit/test_phone_book_plates.c` checks the reveal levels, the plate choice and the view's note levels.

Not done: five slots shown, not native's seven; the native prompt plates and Go There / Stay Here plates are decoded but not wired; the highlight and reveal animations (`func_020396ac`, `func_02039500`) are not ported. The DS set still bakes no `people.txt`, so the pause menu's People tab is empty in DS mode. Not checked on the device.

### 281. DS cutscene movies skipped

Seen on the Vita: in DS mode every full-screen movie skipped. The DS set baked no movie frames and wrote its movie rows without class ids, so op 0xec's `movie_of_class_started` found no movie and went on. The cartridge holds the movies as Mobiclip `.mods` files, played by `cMobiMovieImpl` (`func_ov004_020d7050`) from each movie class's start method (vtable slot 11, such as `func_ov004_020d7028`); the classes are registered by name in `__sinit_0207ef1c`. The rewind starts `pGameMainMovieRollback` (`func_0203e97c`), which on the DS plays `timejump.mods`, and `pGameMainMovieRollbackFinal` plays `longjump.mods`, both unlike the remaster's files. The files carry no audio; native plays the sound on its own.

The class: every full-screen movie the DS scripts start, which the DS set never baked.

Guard: `ds_movies.py` follows that chain in the cartridge code for every movie row at bake time, decodes with ffmpeg's mobiclip decoder, converts the full-range YCgCo planes itself, and bakes DXT1 frames at native size and 15 fps through the bake record. It prints any row it cannot bake and any class the scripts start that has no frames. `tests/tools/test_ds_movies.py` checks that every class id the DS scripts start has a cartridge movie, pins the native mapping and checks that the files carry no audio.

Not done: Final's own sound (0x1eb with `func_0202a080`) is not played; `time_jump` and `long_jump` bake the same file twice. Not checked on the device.

### 282. DS chapter starts queued every face in the game ahead of the first room

Found on the Vita in a DS autoplay run: at the opening, 128 textures waited over 2 seconds, the worst a room plate (`ds_plate_9`) at 16.8 seconds. The DS set's `chain.preload` listed all 109 DS face textures, because the DS text bake added every face it used to the talk textures the chapter start preloads. The remastered set lists only its talk font and windows there and loads a face when a talk page draws it (`draw_face` in `script_view.c`). The chapter preload is the port's own; native loads a face when its talk shows it.

The class: art the whole game shares put on a chapter start's preload list, where it queues in front of the first room.

Guard: the DS text bake hands only the talk font to the preload, as the remastered bake does, and `script_assets.chapter_preload_text` refuses more than 64 textures. `tests/tools/test_script_assets.py` checks the limit.

### 283. Fast-forward still read every page for two seconds

Reported by the user watching a DS autoplay at `ff 4`: dialogue crawled, each page holding at its end. The route pad (`read_the_page` in `script_route_pad.c`) waited `TICKS_TO_READ_A_PAGE` (120 ticks) after a page finished revealing before it pressed CROSS, and never pressed while text was still revealing, whatever the fast-forward rate. Native has no reading pause of its own; a press finishes a revealing page and the next moves on (`script_talk_note_press`).

The class: a pause meant for a person watching at normal speed kept in the fast-forward path.

Guard: while fast-forward runs more than one tick a frame, the pad presses CROSS on every tick a page or narration holds the player, so dialogue moves as fast as the game takes presses. `tests/unit/test_route_pad.c` checks that a waiting page gets its press at once under fast-forward and not at normal speed.

### 284. The DS pause and loading cat had finer pixels than the backdrop behind it

Asked by the user on the Vita: the DS cat should keep its size but show pixels as large as the backdrop's. The cat was baked at 5.625 screen pixels per cartridge pixel and softened (root cause 264). The DS backdrop is a 256-pixel-wide cartridge picture stretched across 960 screen pixels, 3.75 to a pixel. The pause menu is the port's own, so no native function draws either.

The class: menu art from the cartridge drawn on a pixel grid other than the screen it sits on.

Guard: `pause_art.GRID_FILTER` keeps the cat's baked size and then averages it into blocks of `DS_BACKDROP_SCREEN_PIXELS_PER_PIXEL` (960/256) with hard edges (`on_backdrop_grid`); `ds_pause.bake_cat` uses it in place of the soft filter. `tests/tools/test_ds_pause.py` checks the size is kept, alpha is only fully on or off, and colour changes fall on the backdrop's grid.

### 285. The DS soul was drawn with an invented flame

Reported by the user on the Vita: the soul in DS mode still moved like the remastered build. `soul_body_blobs` in `soul_chain.c` draws a made-up flame of ten rising wisps and a base blob, with no native source. Native draws the soul as a particle cloud (`uSoulTentacle`, emitter `func_0205e930` set up by `func_0205f59c`, stepped every tick by `func_0205eb78`, drawn by `func_0205f080`). It has 64 hard discs, cartridge 3D texture 24 tinted `0x76c0`. Each spawns within 2 pixels of the soul with a random size of up to 7.94 times a 2.4-pixel quad, rises up to 2 pixels a tick, sways on a spring toward its spawn x (`vx += (tx - x) >> 1`) and shrinks by 0x59a a tick. Turning the soul on starts a burst: for 36 ticks the draws come from the top half of each range, and each respawn holds the rest for `level` ticks. The lavender orb belongs to every core (`uSoulBGGimmickCore`, `func_0205d0d4`), not to the soul.

The class: an effect drawn from a guess where native code exists.

Guard: `soul_cloud.c` ports the emitter for a soul at rest. `script_view.c` steps it every tick and draws it in place of the blob flame and soul orb whenever the DS art runs. `tests/unit/test_soul_cloud.c` checks an off cloud is empty, a lit cloud rises from the soul within native reach and size, and turning it off lets it die out. Not ported: the spawn along a reaching chain's segments, and the boss's dark-blue colour (`0x3500`).

### 286. Fast-forward pressed through pages still writing and lost Chapter 2's first fate

Seen on the Vita at `ff 8` (DS set): after the caster trick in st02_game004 the route waited for `sys_act080`, Lynne died, and the game-over retry looped 34 times. Root cause 283's fast path pressed CROSS on every tick a talk was open, including while a page was opening or writing out. Each press there cut a page short (`script_talk_note_press`), so talks ended in a few ticks. Actors that run beside a talk then had far less time than at normal speed, and the fate came out differently. The normal-speed replay never ran this path, so `make route-check` passed.

The class: a route-pad mode that the device plays but that the route check never replays.

Guard: the fast path presses only once a page waits for a press (`script_soul_page_wants_a_press`), as normal speed does, and only drops the 120-tick reading pause. `script_replay --reads-at-once` replays with the fast path, and `make route-check` now replays the whole route both ways through `route_check.py`. `tests/unit/test_route_pad.c` checks a page still opening or writing gets no press under fast-forward.

### 287. DS choice captions sat below their panels, and the marked panel was not orange

Reported by the user on the Vita: in DS mode the text of a dialogue choice sat below its box, though it was centred across it, and the panels should be orange like the port's other buttons. `draw_caption` in `script_ui_view.c` centred a caption with a fixed ink middle of 18 pixels, measured on the remastered talk font (glyph tops near 10, ink middle 23). The DS talk font is baked from the cartridge font (`ds_text.py`) with its ink lower in the line (tops near 34, ink middle 49.5), so every DS caption drew about 26 pixels low. Both of the DS set's choice cells were cut from cartridge texture 88, a black box with a red rim, so the marked panel only grew. The panels are the port's own layout, so no native function places them.

The class: a text position tuned to one font's metrics, used with every font.

Guard: `script_talk_book_parse` measures the book's ink middle as the median glyph middle (`ink_middle`), and captions centre on it with the panel art's 5-pixel lift. The remastered set lands where it did. The DS set bakes the marked cell from texture 193, the orange button the rewind select uses. `tests/unit/test_script_ui_view.c` checks a caption lands at the same height in its panel for a font with high ink and one with low ink, and the smoke flow `choice_panels` captures an open choice in both sets.

### 288. The DS soul lost the lighter circle at its centre

Reported by the user on the Vita after root cause 285: the DS soul had no light circle in its middle. The port leaves the soul's own core out of the core marks (`script_soul.c`), because the remastered soul draws its own orb, and root cause 285 dropped that orb for the DS cloud. Native draws every core's orb, the soul's current core (`+0x68`) included (`func_0205d0d4`, glow `0x7e60` and orb `0x76d6`), as translucent polygons. The DS draws translucent polygons after opaque ones, so the orb shows over the soul's opaque particle cloud.

The class: a part of the soul that the remastered drawing gave the soul, while native gives it to the core.

Guard: `draw_soul` in `script_view.c` draws the core orb (`draw_core_orb`, the same glow and orb as every other core) at the soul over the DS cloud. `tests/unit/test_swirl_hides_soul_view.c` checks the orb draws after the cloud's last particle. Not ported: native fades the current core's orb in over its first 60 ticks (`+0x58`), and gives special cores other colours (`func_02058b4c`, `func_02058b64`).

### 289. The smoke test failed 63 checks after the branches merged

Seen running `make smoke` on both sets before a device run. Three causes came in with branches merged after the smoke test was written. `language carried` (`asset_language.c`) and `phone_book plates` (`pause_menu.c`) are new log kinds that `tests/smoke/log_kinds.txt` did not list, which failed 60 flows. The DS set has no `hud_button_art`, because its pills are whole sprites in `hud_pills`, but `keep_the_interface_art_resident` in `script_ui_view.c` asked for it every tick, and the title (`title_menu.c`) for the Record button's notebook, so both logged it missing. The DS set lacks 32 textures in all, each one a slot the cartridge has no art for, and every miss failed the flows. Visual Filter now asks before it restarts the game (root cause 244), and `visual_filter_relaunch` never answered the prompt. Interface art is the port's own, so no native function is involved.

The class: a merge that changes what the app logs, loads or asks, without a smoke run.

Guard: both log kinds are listed as `ok`. The resident list asks for `hud_pills` in place of `hud_button_art` while the DS art runs, the start pill, which has no DS art, does not ask for it, and the DS title leaves the notebook out, as it drew nothing there already. The gap list moved from `test_ds_texture_slots.py` into `game.json` (`ds_absent_textures`, a reason for each name, `*` for a prefix). The converter writes it into the DS set as `absent_textures.txt`, and the texture cache logs a declared name as `texture absent`, a known kind, so any other miss still fails. The flow answers Yes before each relaunch. `tests/unit/test_script_ui_view.c` and `tests/unit/test_title_menu.c` check the DS set never asks for the remastered pill art, `tests/unit/test_texture_cache.c` checks a declared name logs as absent and any other as missing, and `make smoke` on both sets runs before each device run.

### 290. DS choice panels were black until marked

Reported by the user on the Vita after root cause 287: the DS dialogue choices showed orange only on the marked panel, while the title's buttons are orange at rest and gain a red ring when selected. The plain choice cell was still cut from cartridge texture 88, a black box with a red rim. The DS title bakes its buttons from cartridge textures 192 (resting, orange with a light bevel) and 193 (selected, the same with a red ring), set in `game.json` under `pause.ds_art.title_buttons`. The panels are the port's own layout, so no native function places them.

The class: one kind of button drawn from different cartridge art in different menus.

Guard: the plain DS choice cell is baked from texture 192 (ink box 14, 8, 228 by 32), the marked one from 193 as before. `tests/tools/test_ds_button_and_box_art.py` checks the DS choice cells use exactly the title's resting and selected textures.

### 291. Talk text sat close to the left edge of the rounder DS box

Reported by the user on the Vita: wrapped talk lines came close to the box edges, and the DS box, with rounder corners than the remastered one, squeezed its text against the left side. `even_left_margins` in `talk_assets.py` (both sets pass through it) set each box's left margin to the median top and bottom gap of its full pages, measured from the box's straight edge. The DS box art (cartridge texture 23) rounds 7 DS pixels in from its corner, about 20 Vita pixels at the box's scale, and the remastered art about 10, so the DS text began 10 pixels from the box edge, inside the curve. The boxes are the port's layout, so no native function places them.

The class: a margin measured from a box's straight edge, ignoring how far its art rounds in.

Guard: `even_left_margins` adds the box art's corner rounding to the left margin, from `talk.box_corner` (10) and `talk.ds_box_corner` (20) in `game.json`. `tests/tools/test_talk_margins.py` checks the corner moves the text by exactly that much, `tests/tools/test_ds_button_and_box_art.py` measures the rounding of both DS talk boxes from the cartridge art and checks it equals `ds_box_corner`, and the smoke flow `talk_boxes` captures a bottom and a top box in both sets.

### 292. Dead-talk corpses after Chapter 1 showed normal cores

Reported by the user on the Vita: corpses after Chapter 1 showed blue cores with no squiggly outline. Native colours a core warm in two cases (`func_0205d0d4`): the object is a `uGmkDeadTalk` (`func_02058b4c`, type 0x020cf27c, glow 0x19f), or op 0xe1 set bit 2 of its byte 0xde (`func_02058b64`, `func_020267f4`, glow 0x17bf). The port's core marks and object styles (`report_soul_view` in `script_soul.c`) passed only the op 0xe1 flag, though the soul's own rules (`core_is_for_the_dead`) already counted the dead-talk class. Chapter 1's corpse is marked by op 0xe1; later dead-talk corpses are not.

The class: two answers to "is this a corpse" in one module.

Guard: the core marks and object styles ask `core_is_for_the_dead`, as the soul does. `tests/unit/test_dead_talk_core_looks_dead.c` checks a dead-talk body and an op 0xe1 body both mark and style as dead while a lamp does not. Not ported: the two native warm colours differ (orange for dead talk, yellow for op 0xe1); the port draws one.

### 293. Every language but English was greyed in the language list

Reported by the user on the Vita: the language list drew every language but the first at the greyed alpha, though all nine work (root cause 230). `draw_dropdown` in `pause_options.c` still dimmed every row after the first, a leftover from when English was the only language the set carried. The list is the port's own menu, so no native function draws it.

The class: a placeholder state left in a list after the feature behind it landed.

Guard: every row of the list draws at full white, and the language list already shows only the languages the set carries (`asset_language_carried_count`). `tests/unit/test_pause_options.c` opens the list and checks every language row draws at full alpha.

### 294. The DS menu cat looked blurry on the backdrop's grid

Reported by the user on the Vita after root cause 284: the pause and loading cat still did not match the backdrop's pixels and looked softly rasterized. `on_backdrop_grid` in `pause_art.py` averaged each 3.75-pixel block of the cat, which the bake had scaled to 5.625 screen pixels per cartridge pixel. A block covers 1.5 cartridge pixels, so most blocks mixed two colours into a shade the cartridge art does not have, and outlines and the scarf showed stepped gradients. The menu is the port's own, so no native function draws it.

The class: resampling pixel art onto a grid by averaging, which invents colours wherever the grids do not line up.

Guard: each block takes the one cartridge colour under its centre from the point-scaled art, with alpha fully on or off, so the cat keeps its size and the backdrop's 3.75-pixel grid and shows only its own colours. `tests/tools/test_ds_pause.py` checks a striped picture whose stripes do not fit the grid comes out in its own colours only.

### 295. Switching the Visual Filter froze the Vita on a black screen

Reported by the user on the Vita: picking Remastered under Visual Filter froze the console on a black screen until a hard reboot, and the other way froze it until HOME closed the app. The log shows both switches reach `exit clean relaunch=1`, then the new process logs its first lines and stops inside `renderer_init` (`vglInitExtended`). The app handed itself to `sceAppMgrLoadExec` with vitaGL still running and every worker thread alive. This SDK's vitaGL has no `vglEnd`, so the GPU cannot be ended first, and the next process's GPU start races the old one's teardown. The relaunch is the port's own, so no native function is involved.

The class: handing the process to the system while it still holds the GPU.

Guard: the app no longer relaunches. When the filter changes on the title, `main` saves the settings, stops the viewer and starts it again on the other set's folder in the same process (`switch_to_the_other_set`), with `asset_sets_choose` and the memory budget run again; `sceAppMgrLoadExec` is gone. `tests/integration/test_asset_sets.c` starts the remastered set, switches in place and checks the DS set runs with sharp art and reads nothing from the remastered folder, and the smoke flow `visual_filter_switch` switches to DS and back in one run on both sets.

### 296. The DS pause menu asked for the remastered spotlight

Found by the smoke test once `pause_and_resume` ran on the DS set (root cause 294): `pause_menu.c` asked for `pause_spotlight` every frame, which only the remastered set bakes (from the APK), so the DS log filled with `texture missing`. The pause menu is the port's own, so no native function draws it.

The class: menu art from one set asked for by the other, as in root cause 289.

Guard: with sharp art on, the menu neither waits for, keeps nor draws the spotlight. `tests/unit/test_pause_menu.c` checks the remastered menu asks for it and the DS menu never does, and `pause_and_resume` runs on both sets.

### 297. The ending credits drew black bars over the wide screen

Reported by the user on the Vita: the ending's credit scenes showed black bars at both sides. Root cause 213 drew them from the first staff page to the title so the scenes kept the 4:3 frame of the pages. The user wants the ending to fill the screen; only the credits under Options keep their own frame. Native is 4:3 throughout, so this is the port's choice.

The class: a frame choice for one credits screen applied to the other.

Guard: the machine no longer tracks the credits for the view, and `script_view.c` draws no side bars; the staff pages still draw at the DS frame's size in the middle. `tests/unit/test_ending_credits_have_no_side_bars.c` plays a page, a scene after it and `GOTO TITLE` and checks no full-height black bar is drawn on any tick.

### 298. The DS High Contrast preview was an empty black panel

Reported by the user on the Vita: the preview under High Contrast showed only its black box in the DS set. The remastered widget table ends with a `preview` line naming the plate to show; the DS widget bake (`ds_pause.py`) never wrote one, so `pause_draw_preview` had no plate. The menu is the port's own, so no native function draws it.

The class: a widget table line one set's bake writes and the other's leaves out.

Guard: `game.json` names the DS plate for the same room (`ds_plate`, the cartridge's plate for room `st01-00-02`, the same 8:3 framing), and the DS bake appends `pause_art.preview_lines(widgets, "ds_plate")`. `tests/tools/test_pause_art.py` checks both lines share the part, the DS bake writes the line, and both plates sit on the same room row in the baked `rooms.txt`.

### 299. A view test that skipped the texture cache's start hung the whole test run

Found while running the tests for root causes 290 to 298: `make test-parallel` sat on one core for over 15 minutes with no output. `test_ending_credits_have_no_side_bars` drew a staff page without `texture_cache_init`, and `find_entry_by_name` in `texture_cache.c` never returned. The name index marked an empty link with -1, so before the init its zeroed memory read as entry 0 linked to itself. No `make` step limited how long a test could run, so the hang stalled the run silently. The cache is the port's own, so no native function is involved.

The class: an index whose empty value is not zero, so memory that was never started looks like a loop, and a test run that waits on a hung test forever.

Guard: the bucket links hold an entry's index plus one, so zeroed memory is an empty index, and the credits test starts the cache as the other view tests do. `make test` and `make test-parallel` stop any test binary still running after `TEST_SECONDS_MOST` (300) seconds and fail it by name. `tests/unit/test_texture_cache_before_init.c` looks names up before the init and checks each call returns. The same run found the in-process switch test of root cause 295 watching for reads before `asset_sets_choose`, which opens each set's manifest to see which sets exist; it now chooses first, as the other DS-set test does. The smoke flows `choice_panels` and `talk_boxes` gained the mutations `test_smoke_mutations.py` asks every flow to have.

### 300. The DS loading screen's cat came out in pieces because its set was half uploaded

Seen on the Vita: the cat walking across the DS loading screen was cut into fragments. The converter was not at fault. The cartridge's ten walk frames (`st01_neko_007`, resource 574) decode whole, and a fresh bake of the cat gives the same `pause/cat.txt` and `pause/cat_track.txt` as the Mac's `game_ds` set. On the Vita, `assets_ds` held that set's five cat pages but an earlier bake's two tables. The earlier bake put all ten walk frames on page 4; the current one puts three on page 3 and packs page 4 differently, so each frame cut a rectangle across two or three cats. `upload-assets` sends `textures` first and `pause` last, the last upload stopped in between, and the upload record on the Mac had no `pause/` entry at all. Nothing on the device or in the tools said the set was mixed. The loading screen is the port's own, so no native function is involved.

The class: a set on the device made of two bakes, which is any table that names page coordinates (the cat, the widgets, every `.anim` and script bundle) beside pages from another bake, after any upload that stops partway.

Guard: `upload_assets` in `vita_loop.py` writes `upload_unfinished.txt` into the set's folder on the device before it changes the first file and removes it only when the whole set is up. `launch` refuses to start the app while either set carries the mark and names the set to upload again, so a smoke run or a screenshot never tests a mixed set. `tests/tools/test_vita_loop.py` checks that a failed upload leaves the mark, that the mark is up before the first file, that a finished upload clears it, that a DS set is marked in its own folder and that a marked set is not launched. Files copied to the card by hand are not covered: use `upload-assets --check-device` after any of those.

### 301. A player could not start a talk with the dead

Root causes 43 and 92 left it open: outside the tutorial kind, a dead talk started only from the route's `dead_talk` step, which acted inside the game. Auto-play, the smoke test and the whole-game replay all finished, so nothing showed that a person playing by hand had no way in from Chapter 3 on. Native's ghost-view update (`func_02032914`) asks: once the chain settles on a core that talks (`func_02033408` reads bit 7 of the chain actor's byte at +0x7c), it hides the pills, opens a yes/no prompt (pills 10 and 11, trigger bits 0x1000 and 0x2000), and on yes pushes `pGameMainDeadTalk` (state 0x15). The chain connects that way to a dead core, and with two ghosts to the object the other ghost holds (`func_02061718`: `func_02027684` and `func_0202768c`, or both ghosts in one object), which `func_0203bfc8` turns into `CORETALK`.

The class: an action only auto-play could do, hidden because every check that plays the game uses auto-play.

Guard: arriving in the ghost view on a core for the dead (a dead object or a dead-talk gimmick), or on the object the other ghost holds while the scene handles `SYSTEM|CORETALK`, opens a Yes/No prompt once no talk, choice or narration is up. The pills leave, the soul holds still, CROSS takes the marked answer, LEFT and RIGHT move the mark, CIRCLE or L is No, and a touch on either button answers. Yes calls `start_dead_talk` on the soul's core; No closes the prompt and CROSS on the same core asks again. The prompt draws with the pause menu's dialog box and its `row.yes` and `row.no` words, so it needs no new art and no rebake; it shows no question line and none of native's pill art. The route's `dead_talk` step now waits for the prompt and presses CROSS through `script_route_pad`; the pad answers CIRCLE to a prompt its step does not ask for. In the whole-game replay 17 of the 21 dead talks start through the prompt and `make route-check` passes. `tests/unit/test_route_pad.c` covers yes, no, the mark on No, asking again, the held soul, the other ghost's core, a living core no ghost holds, and the pad's yes and its decline.

Corrected on 2026-11-23: the prompt belongs to fewer cores than this entry first gave it. Native's chain (`func_02061718`) connects to a core whose owner is a `uGmkDeadTalk` (`func_020616dc`, `func_02058b4c`), to a core whose owner op 0xe1 marked dead (`func_020616f4`, `func_02058b78`), or to the other ghost's core. Only the first and last set the bit `func_02033408` reads (`func_0206001c`, four ticks into state 4, and only while the chain is not held). A marked corpse holds the chain for 0x46 ticks, the soul then lands on it (`func_020100f4`), and `func_020333e4` sends the ghost view to state 0x14, the time leap (`pGameMainTimeleap`, `timeleap start`), which the port already runs. So a corpse never asks, and its scene's `timeleap start` handler plays its talk. The port asked on corpses too, and 15 scenes have a marked corpse with no `SYSTEM|DEADTALK` handler, where a Yes would have left the swirl up with nothing to end it.

Guard: `core_asks_before_its_talk` is true for the dead-talk gimmick class only, and `a_dead_talk_prompt_may_open` needs it. A `dead_talk` step naming a corpse the soul is on passes without starting anything, since the time leap has the talk. A `dead_talk` step whose core the soul is not on is walked by the pad: it opens the ghost view if it has to, steers to the core `script_soul_route_dead_talk_core` gives (the named gimmick core, or the object the other ghost holds), answers Yes, and afterwards walks back to the core it left and closes the view it opened, so the steps that follow find the soul where the footage had it. In the whole-game replay of both sets no step logs `route_no_player_input` now. `tests/unit/test_route_pad.c` checks that a corpse raises no prompt and that its step starts no talk.

Changed on 2026-11-23, Liam's call: there is no Yes/No, and the soul stays on its own core. Re-reading native: the tip's pick (`func_0205fa2c`) hands the core it found to `func_02061718`, which connects (state 4 through `func_02060d54`) instead of jumping when the owner is a `uGmkDeadTalk` (`func_020616dc`), a corpse op 0xe1 marked (`func_020616f4`, which also sets the hold bit), or the other ghost's core (`game+0xe40` through `func_02027684` while Missile is active, `game+0xe44` through `func_0202768c` while Sissel is), and records it at `game+0xef8` (`func_020266e8`). For a talk core nothing moves the soul: `func_0206001c` only sets the connected bit at tick 4, and `func_02032914` either pushes `pGameMainDeadTalk` or, as decompiled, asks (modes 1 and 2; No calls `func_02061230`, which lets the chain go). For a corpse native does move the soul: at tick 0x46 `func_0206001c` clears the hold and calls `func_020100f4`, which writes the active ghost's core (`game+0xe40`, `+0xe44` or `+0xe48`), and `func_020333e4` sees that change and sends the view to state 0x14, the time leap. The port follows Liam on both: the question is gone, and the soul stays on its core for a corpse too, which differs from native.

The port now treats a chain that reaches a talk core (the dead-talk gimmick class, or the object the other ghost holds while the scene handles `SYSTEM|CORETALK`) or a corpse (marked dead and not that class) as a reach (`reach_for_core`): the lightning plays for `SOUL_LIGHTNING_TICKS`, then `finish_the_reach` marks `core_reached`, sends `SV_DEADCORE` for a dead core, and starts the talk (`start_dead_talk`) or the corpse's sparks, dead-trick light and time leap (and the talk at once in the tutorial kind, root cause 92). `active_object`, the chain's root, the soul's mark and the camera's core stay on the soul's own core; the camera frames both cores while a reach holds. The reached core is kept as `reached_object` (native's `game+0xef8`), which `start_dead_talk` targets when none is named. For a corpse, what scripts read as the soul's core follows native: `corpse_object` and `active_core_name` name the corpse, and op 0xa0, the 0xa1/0xa2 compare for the active ghost, the iris on the soul core, the pans to the soul core and the ghost-position register read it through `script_soul_core_the_scripts_see`. The tip stays held (root cause 215's `dead_core_holds_the_tip`) and the Trick pill stays shut until the view closes. The time leap's swirl now gives back the pause it took when it sends `timeleap start` (`leap_holds_the_world`): before, a player who waited on a corpse without pressing Back hung in the leap's `timeleap start` handler, which waits for the pause to lift; the replay never showed it because its `ghost_off` steps always left first.

The Yes/No itself is kept as a neutral question, which root cause 223's hourglass now opens: `script_soul_open_question`, `question_is_open`, `question_cursor`, `question_answer` (CROSS takes the marked answer, LEFT and RIGHT move the mark, CIRCLE or L is No, a touch on a button answers, the pills leave and the soul holds still), drawn by `draw_the_question` with the pause menu's dialog box.

The class: every connection the chain makes without jumping, in every chapter, and everything that read the soul's core as the place the talk or the leap belonged to. Guard: `tests/unit/test_route_pad.c` reaches a dead-talk core, the other ghost's core and a corpse by hand and checks the lightning, the talk or the time leap, the soul left on its own core, no question, the scripts' core on the corpse and the pause lifted after the leap; it checks that the pad plays a `dead_talk` step by reaching, that a `move` onto a talk core reaches it and passes the `dead_talk` step after it, that the pad opens the view for a step and closes it afterwards,. The question's answers are checked in `tests/unit/test_hourglass_rollback.c` (root cause 223). `tests/unit/test_dead_core_holds_the_tip.c` holds the chain through a corpse's connection with the soul on its own core. The pad's route search no longer hops through a talk core or a corpse on the way to another core, because reaching one does not move the soul.

Still open: route step 1637 (st14_game033, `move pipe3` with Sissel, then 1638 `move lamp2` waiting on `core:pipe3`) asks the soul to move onto the object Missile holds after the swap at step 1632, in a scene whose `core_talk` object handles `SYSTEM|CORETALK` (a hint talk gated on flags 0x11fa, 0x11fb and 0x10). Native `func_02061718` connects there too, so this reach starts Missile's hint talk and the soul stays on valve2, and the route's next wait never holds. With that one object left out by hand, as an experiment only, both sets replay the whole game with `route check passed 0 problems` twice, `chapter starts check passed`, 31 reaches and no `route_no_player_input` or `op_not_ported`; as committed, `make route-check` stops at step 1638. The route data was not changed.

### 302. Twenty script ops the whole game reaches did nothing

The whole-game run on the Vita logged `op_not_ported` for 20 ops. Each had a length in `script_opcodes.c` and no family, so `run_op` stepped over it. All are cases of native `func_02023fdc`. What each does, and what the port now does:

- **0xd1, 0xd2** save and restore the camera. Native writes the camera (`func_0200f178`, `func_0200f190`) into the scene object's `mBgPosX` and `mBgPosY` properties, and 0xd2 reads them back, adds the world origin (`func_0201230c`) and cuts there (`func_02026784`, `func_0200f918`). The port keeps the pair on the machine, clears it when a script begins and cuts back as op 0xc8 does. Used around the car movie and the recollection of st04_game001 and in st11.
- **0xd3** sets a camera home (`func_0200fa64`: a point at `game+0xdac` and bit 26 of `game+0x10`; 0xffff in either word clears it). `func_0200f9e8` reads it: when the ghost view closes (`func_02033140`) the camera pans to the home at 0x10000 and sends `* camera custom cancel`, instead of going back to where it was. The port does that in `set_soul_view_with_effect`. 52 sites, in the prison, st09 and st13. Native also uses the home on the character switch and in `func_02035078`; those are not ported.
- **0xc2, 0xc3** wait for the room picture and load the rest of it now (`func_0200eec4`, `func_0200eee0` on the tile loader at `game+0xee0`). The port has the whole room as soon as op 0xc1 returns, so both pass, as op 0xc4 already did.
- **0x08, 0x09** swap which DS screen shows the main picture (`func_02026534`, `func_02026544`, read by `func_0203164c`, which writes POWCNT1 bit 15). **0xc5, 0xc6, 0xd4** show, place and slide the ending's picture on the other screen (`pGameSubEnding`, `func_02026770`, `func_0203f1b8`, `func_0203f134`, stepped by `func_0203f024`). The port keeps all of it as state (`main_picture_is_on_the_top_screen`, `ending_picture`), steps the slide, and yields after 0xc5 as native does. Nothing is drawn: the port has one screen, and how st15_demo110's two-screen pan should look on it is undecided.
- **0x48** sets how near the soul's core must be to a tap balloon (`func_020106ec`, balloon `+0x124`, read by `func_020451ec`: a far balloon is dimmed and its tap sends `* talk out of range`). The port keeps `reach_px` and does not enforce it. Measured from the balloon's anchor, the technician's balloon in st01_game020 is 161 pixels from the umbrella's core against a reach of 144, yet the footage route taps it from there and the scene goes on, so the port's point is not native's. Enforcing it stopped the route at step 251.
- **0x49** keeps the camera where it is on a tap (`func_0201070c`, balloon `+0x11c`). Without it native's tap (`func_020453fc`) pans to the balloon's owner (`func_0200f718`, 0x10000) and delivers when the pan ends (`func_020450ec`). The port delivered at once and never panned, as if every balloon had 0x49. `script_balloons_tap` now pans first for a left-group balloon with an owner, and `script_balloons_count_down` delivers when the pan is over.
- **0x14** sets an actor colour filter (`func_0205953c`, actor `+0xc3`): mode 9 is the red alarm filter `data_020cf828` (`func_0200d2c8`: saturation times 3/4, red times 5/4, green and blue times 3/4), any other mode none. The port keeps `color_filter` on the actor and multiplies green and blue by 0.6 outside the ghost view, which is close and not exact. st14, beside the `red_efec` effect.
- **0x9a, 0xaf** set and lift a palette override on an actor (`func_0205902c`, actor `+0xc0`, applied by `func_0205964c` through `func_0200bb8c`; `func_02059020` and slot 15 restore it). The port keeps `palette_override` and draws nothing from it: the second palette is not baked. One guard in st03_game003 and four sites in st14_game050.
- **0x119** makes an actor share another's tile memory (`func_020598a4`, read by the tile streamer `func_0200857c`). It is VRAM saving for the prison's repeated props and changes nothing a player sees, so it is filed as stores-only. The earlier note that it links motion controllers was wrong.
- **0xb4, 0xb5, 0xb6, 0xb7** move an effect (`func_020271c8`), seek its movie to a tick (`func_0202722c`, `func_0202d8f8`), set its scale (`func_02027274`) and ramp its scale over ticks (`func_020272d0`, stepped by `func_02066928`). `script_view` now keeps a scale and a ramp per effect and applies them to movie frames and flipbooks. Op 0xb0's own scale words were dropped too, so `pipe_efc2` (0.5) and the st14 steam drew at full size; `spawn_effect` now passes them. The effect visitor's `@part` name form (`func_02026d34`) is matched as well.
- **0xeb** clears the trick hold on every core of the named actors (`func_02026ff0`), the bit ops 0xe9 and 0xea write.
- **0xfd** keeps a checkpoint. Native `func_02011b04` serialises the whole running scene (flags, live objects, status words) to `game+0x1098`, and the rollback menu's default entry resumes from it (`func_02011bfc`, then `* SYSTEM ROLLBACK START`). The port keeps the scene name, the 0x1000 flag bank and the fate level. A retry goes back to the checkpoint's scene only when the checkpoint is from another scene, which needs flag 0x100a to have kept it across the change; flag 0x1009 hides it, as `func_02011c80` reads. In the same scene the port still restarts or reloads as before.

The class: an op in the length table with no family logs once and is skipped, and the smoke test filed every `*_not_ported` line as known, so a newly reached op could not fail a run.

Guard: `tests/smoke/log_kinds.txt` now fails a run on `op_not_ported` and keeps the other three kinds as known by name. `tests/unit/test_ops_ported_for_the_beta.c` runs each of the 20 ops in a scene and checks none logs it, and checks the camera save, the home pan and its message, the ending picture's slide, the effect ops' values, the alarm filter, the trick release, the balloon pan and the checkpoint's keeping and dropping.

Still open: the ending's second picture, the palette override's art, the balloon reach, a resume from the checkpoint mid-scene with the rollback menu, and the camera home on the character switch.

### 303. Four pieces of DS art the DS set left empty

The whole-game run on the Vita in DS mode logged `texture absent` for `core_ring`, `phone_cap`, `phone_strip` and `staff_00_01` to `staff_03_01`, and the DS title offered no Record. `game.json` `ds_absent_textures` listed the first three groups as not found or not decoded, and the DS set baked no `pause/people.txt`. Each was in the cartridge:

- **Staff roll.** Op 0xf8 pushes `pGameMainStaffRoll`; `func_0203dbf0` makes a paged `uStaffRoll` (`func_02064464`) or, for page 9, the long roll (`func_02064478`). The pages are pictures: `data_0209e010` gives each page its first row and screen count, `data_0209e048` the `cpac_2d.bin` block 2 entries of each screen (0x6000 bytes of 4bpp tiles, a 32x24 map), `data_0209e028` the long roll's three banks, and block 0 entry 0x82 the palette (`func_020648b4`, `func_02065178`, `func_02064a60`). `ds_staff_roll.py` reads those tables and bakes `staff_PP_SS` at 256x192, and `ds_set.expand` now writes the `staffpage` rows the DS set never had, which is why the device asked only for screen 1 of each page and Options greyed out Credits. Page 9 is cut into eight screens and paced like the other pages; native scrolls it at half a pixel a tick with no rests.
- **Phone split.** Effects 48 and 49 are `uTelephoneChangeEffect` (`func_02049538`), whose border is a `uPhoneBorderEffect` (`func_02049134`, drawn by `func_020491c0`) from `cpac_3d.bin` block 0 textures 0x13 (the cap, 32x32) and 0x14 (the strip, 16x16). Both are plain `ds_textures` entries now.
- **Core ring.** Effect 4 is movie kind 0x23 (`data_02086624[4]`, `func_02052ad8`): a white ring that grows from 14 to 48 pixels over 70 frames above a blue glow. The set keeps the port's ring row and pulse and takes the ring bitmap (item 1 of effect 0x23) as `core_ring`. The glow is not drawn.
- **Record.** Overlay 6 (`func_ov006_020d42d8`, `func_ov006_020d6c20`) draws a 128x192 portrait texture per person (`data_ov006_020dacdc`), a 256x168 picture per place (`data_ov006_020d88fc`), list plates (`data_020d8a48`) and text from `system/database_0000_Expand.<language>.xml.lz`, by the level lists at `data_ov006_020d8a50`. Root cause 147's note that the DS notebook draws no faces was wrong. `ds_people.py` reads the overlay's tables by address and bakes the same `pause/people.txt` the remastered set has: a face per person (the portrait centred on native's striped ground, texture 0x51), a picture per place, a note card per native level drawn with the DS font and the name bands (textures 0x4e and 0x4f), and a thumbnail sheet. A person's thumbnail is a cutout of the portrait around the face, from a face centre per person in `game.json` `pause.people.ds_face_centers`; a place's is its native plate. Sissel's face turns to the soul's at level 150, as `func_ov006_020d41c8` does. The DS title shows the first person's thumbnail on its Record button.

Two more gaps showed once the Record was on screen. Every Back button in DS mode drew without its circle, and the Record's tabs without L and R, because the button glyphs sit on `hud_sheet`, which only the remastered set bakes; `ds_pause.py` now puts the four face-button glyphs and L and R on the DS widget sheet as `wglyph` lines, and `pause_widgets.c` draws glyphs from that sheet when a table has them. The DS title's Record button shows the cartridge's notebook button (`cpac_3d.bin` block 0 texture 91, the HUD's Database button) as the `book` cell. `upload-assets` also never sent a set's `absent_textures.txt`, so a set whose list shrank kept the old one on the Vita; it is listed with the manifest now.

Not done: native's left half of the People top screen (a spotlight arc, texture 0x50, behind live sprite actors), the other four picture swaps of `func_ov006_020d41c8` (Jowd, the chef and two places), the long roll's own pace, and the effect 4 glow.

The class: a slot marked absent with "not found" stayed that way because nothing listed what the device actually asked for in DS mode.

Guard: `tests/tools/test_ds_staff_roll.py` and `tests/tools/test_ds_people.py` check the table reads, the tile decode with both flips, the refusals, the staff rows and the face cutout; `test_ds_texture_slots.py` already forces `ds_textures` and the absent list to agree. `tests/golden/pause_render.c` finds textures in their buckets again (it had looked in the flat folder since the buckets came in, so it drew neither set) and draws a DS set when `PAUSE_RENDER_DS_SET` is set.

## Still open

- The title screen lacks the DS bottom screen's red wireframe junkyard and Sissel's glowing pose, which are DS art in `cpac_2d.bin` that no set bakes yet; the remastered set uses the remaster's title art, and the DS set draws none. The boot logos (pLogo) that `GOTO TITLE` passes through are not shown, and the title plays no music.
- Talks and balloons the remaster's text lacks (49 labels, root cause 60): bake them from the DS ROM's language file for the script.

- Which verb an object's action line shows. `st01_id_st01a_acn_*` holds state variants and native selection is not traced, so objects with an empty first variant show no action.
- Telephone, Lynne and the dead core have no name entry in the tables read so far.
- Action selection for player mark layouts: 14 shapes exist and the port uses 4 by a position rule.
- Effect classes with no port yet, named from the class registry in the RAM dump: `uTimeOverChainEffect` (id 50, the time-over sequence; constructor `func_020473ec`, 0xd8 bytes), `uPhoneBorderEffect` (id 59), `uBlackDarkness` (ids 151, 228, 229), `uLanternLightBlackSt02_01` (id 119, 57 sites in stage 2), `uNightVisionGoggle` (57), `uElectricTorch` (56). Class ids group by factory in `data_020ac194`; key any port by factory, as the chapter cards are.
- Plain effect kinds with no row in any remaster effect list (root cause 250 lists them all): kind 3 (id 6, the ripple, 50 sites in nine stages), 0x83 (id 207, the dog's bark) and 0x86 (id 61, `ST06_EXCLAMATION`). Where the remaster draws them from is not found.
- Telephone lines and neon (root cause 78): free units are tracked but not drawn. The line art comes from `func_0205693c` on the sprite set at `data_020cf724`, with `uTelephoneCore`'s icon at `data_020cfe2c`; neither set is mapped to a remaster texture yet.
- Op 0x9f (case at `0x020253bc`): on a `uGimmick`, writes `words[2]` into the top half of core `words[1]`'s `+0x14` word (`func_02026700`); the enable bit 0 of that word is ops 0x9d and 0x9e. What reads the top half is not found, so it is not ported (state nothing reads).
- Op 0x5b (14 scripts, always after op 0x4d) is native `func_02010874`: it clears the eight balloon slot words at `game+0xe1c` and closes the balloons with the force bit set (`func_020109c8`); the port closes every balloon, as op 0x57 does. Whether a closing animation plays is not traced.
- The port draws one light circle at a time, the last one created. Native keeps up to 16 and shrinks a circle that sits inside a larger one (`func_02062510`); st03_game012 creates three names (`demo_light`, `core_light` and `joedo_light`). Root cause 240 places the drawn one only.
- `no_motion_row` for `sofa`, `table`, `ftendoor`, `souldead` and `carcass` is expected: their tables (`st06_sofa`, `table_wait`, `st05_tendoor_wait`, `onlycore_000`) belong to objects that are only a core or whose art is in the room plate, so nothing is drawn natively either.
- Soul reach (root cause 68): the st05 hop from the dictionary to the lamp (route step 218) is 182 units in the port but one hop in the footage, far past Sissel's native 48, so one of those cores sits in the wrong place or a core between them is missing. The route still places the soul there.
- When op 0x77's phone book appears. The port opens it as the op runs; native op 0x77 (case `0x020246fa`) calls `func_020123a0`, which only records a pending state on the sequencer (`func_02022f34` writes `+0x28` and `+0x2c`), and `func_02023094` applies it only while the current state's byte `+0xc` is set. In st02_game004, telephone_004_act030 runs `op_077 0 0` and then a wait and a talk (ops 0x2f, 0x37, 0x39) before it jumps to act060, and the footage shows that talk before the book. The route waits for act060 before Go There; what sets the state's `+0xc` byte is not traced.
- The rollback menu after a failed rescue or a hourglass press (root causes 51 and 223 port the retry) is not drawn. Native's rollback scene (`func_0203b72c`, started from `func_02015428`) offers choices; on retry it sends `*` `SYSTEM|RESTART` when any script handles it (13 scripts do, from st01_game021 on) and otherwise reloads the scene, and the other choices tear down (`func_0200fe74`) and leave. The port draws no menu: a hourglass press asks one Yes/No question (root cause 223), whose Yes is the native default retry and whose No is the menu's cancel, and a game over retries without asking. The menu's default entry, a resume from the op 0xfd checkpoint (`func_02011c80` reads it at `game+0x1098`; root cause 302 keeps the checkpoint and goes back to its scene only from another scene), and the `SYSTEM|RESTART ROLLBACK` path (state 0x85, `pGameMainMovieRollbackShort` then `pGameMainRollback`) are not ported. `SYSTEM|LOAD START` (pushed in st01_game022) belongs with it: the event a scene gets when it is entered from a save.
- The stage 6 dog in room 65 (st06 game001, around route steps 144 to 160): run 19 drew its bark lines with no dog, while the footage shows the dog by the TV stand inside the same lines. Its two heights (y 264 and y 296) are the same ground point (200) under two table origins (point 0 at y -64 or -96), so it is not drawn low. More likely it is drawn behind the TV stand while the bark lines, an effect, draw on top: native draws each sprite on list `level + signed byte at actor+0x4f` (`func_0200bd94`), the port ignores that byte, and nothing found so far writes it with a plain `strb` (a wider store or computed address may). Present before the counted-loop port.
- The Minutes Before Death card's middle line (the time and place on the black band) is not drawn. Traced so far: the card is class `uBeforeDeathEffect` (effect ids 51 and 52, constructor `func_02048100`); its init `func_02048188` reads a four-digit time string from `game+0xd84` (`func_0203cbe4`) and passes it to a child `uBeforeDeathEffect::uSwfPlayWatch` (`func_0204821c`). Script op 0x7c writes that string (case at `0x0202475c`, `func_0202663c`, only when it is four characters); the port files 0x7c as stores-only. Where the place name comes from is not traced. Porting needs the time store, the place source and the drawn line together.
- How long the world stays held after a ghost view closes. The port holds it for the 13 ticks of the black spokes (`SOUL_VIEW_LEAVE_TICKS`), sends `SYSTEM|SV_END` and shows the player mark 5 ticks in. In the footage of Chapter 5's party room (st02_game022) the world stays still much longer. At 20 fps after the view closes at 8148.45, the ball hangs mid-bounce from 8148.70 to 8149.20, and the player mark and the countdown banner come back at about 8149.2, some 45 ticks after the press. After step 363's close at about 8132.15, demo100's camera (its `SV_END` handler, 0x42b6) moves at about 8133.15, some 60 ticks after. The ball's positions in the three views fit the port's physics only when each close holds the world about 45 ticks: view 1 opens at the top of the first bounce (180,111) and view 2 as it lands (176,159), while view 3 finds the ball 61 from the Toy Cake. Native sends `SV_END` from pGameMain's start (`func_02032328`) when the mode byte is 4, and pGameMainReturnNormal (`func_0203267c`) waits on `func_02010b38` (game+8 bits 9 and 10, game+0x1a); the frame count of the close itself is not read. The Chapter 5 block opens view 1 at once (step 405, pace instant) to absorb the difference; it plays to demo070 under the bake. If the hold is ported, every route's living time between views shrinks and the pacing must be rebaked.

- The film frame's slide. Ops 0x71 and 0x72 (root cause 117) animate the frame in and out natively (states 2 and 3 of the panel at `game+0xfb4`); the port switches it at once, as it does for 0x6f and 0x70.
- The route driver starts a dead talk from any distance. In st04_game010 a `dead_talk` step on `linne_core` ran with the soul on the telephone, six hops away, and skipped the bell, the pot ride and Rindge's talk. The route now walks there, but whether the pad path on the Vita enforces reach for a dead talk is not checked.
- Missile's exchange (root cause 124), what is not ported or not checked: the swap effect (`func_0205e294`, `func_0205e2a4`, waited on by `func_0205e2b4`; the port waits the 0x19 chain ticks only), the outline drawn on the two cores, the 0x30-tick fade of the chain after Unlock (soul state 6), the button icons (the two pills are baked words, `Swap` and `Unlock`, with no icon, and their look is not compared to footage), and op 0xa5 outside a link, where native moves the chain root (`func_0206115c`). No routed scene uses a swap yet, so the pad path is covered by unit tests only.
- Effects (root cause 125), what is not ported or not checked: `st03_onnpu_00` picks one of eight labels from a roll and a second variable the game sets, which the converter cannot describe, so it still plays as one plain loop (it is the one name in `STILL_PLAYED_AS_A_PLAIN_LOOP` in `check_effect_scripts.py`). Ops 0xb3 and 0xb9 on class effects with no port (lamps, goggles, `ef_black`) do nothing. `coreactionexchangecore` stops at frame 30 until native sends it to its label `finish`; the sender is not found and no set bakes it yet. The staff roll and the fades were not compared to footage or seen on the device, and the ending's sub screen (`pGameSubEnding`) is not drawn.
- Chapter 18 (st13_game020 to 022, then st15) can be entered in `script_replay` with `--start chapter --chapter-mark 17 --set-flag 1232 --set-flag 11c0 --set-flag 1145 --set-flag 11c3 --set-flag 11ea`. That is a diagnostic entry, not a route.
