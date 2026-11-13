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

Guard: `script_choice.c` holds the lists and the pick with the native rules, the machine clears them at every scene change, and play is held while a choice is open. A `choose` route step (wait `choice`) picks a panel by its event, talk label or caption label, or `cancel`; with no such step, auto-play picks the first panel whose flag is not set after a second and logs `choice_picked_without_route`, and a press does the same in manual play. Still open: the panels are not drawn, and the turned panel's flip (`game+0x120` of the control) is read as the turned panel showing at its index.

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

Guard: after an 0xdb talk closes, auto-play retries after a second and a press retries in manual play (`game_over_retry restart` or `reload`). A reload starts the scene again and the route resumes at the step it was on, so the second attempt plays the steps after the failure instead of the whole block again. The resume step and the route's furthest acted step live on the machine, not the soul: every script change resets the soul (`tear_down_script` calls `script_soul_reset`), which at first wiped both, so neither the resume nor root cause 47's block rule ever acted until audit 30 caught it. Still open: the rollback menu itself (its choices and art), so the retry is always the native default.

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

Guard: the converter bakes a hue-turned copy of every screen texture marked `ghost_variants` (`soul_beam-missile` turned 120 degrees, `soul_beam-boss` 240) and flags every soul-view class whose tint is soul red (red at least twice green and blue); the view picks the beam by the active ghost and turns flagged tints to green for Missile and blue for the boss. `tests/tools/test_ghost_plates.py` checks the flag rule over every class in game.json and the per-ghost jobs, `tests/tools/test_texture_encoder.py` the hue turn, and `tests/unit/test_soul_ghost_turn.c` the table flag.

### 68. Every ghost reached as far as Missile

Native's reach is `func_020615e8`: Missile 112 DS pixels, the boss 48, and Sissel the soul's own value at +0xa4, which only the soul's constructor `func_0205f18c` writes (0x30000, 48 pixels); each becomes 168 while its `MissileHandLong`, `BossHandLong` or `CicelHandLong` exception is on (`func_02013b18` sets game+0xc bit 30 and game+0x10 bits 19 and 17). `func_0206167c` holds the tip within that reach of the core, and `func_0205fa2c` picks a core near the tip. The port held every ghost's tip at 112, so Sissel's drags reached more than twice as far as the game allows and picked cores the scene never offers.

Guard: `script_soul_reach` follows `func_020615e8` and the exceptions set each ghost's long hand; the chain and the route's hop planner use it. `tests/unit/test_soul_touch_rules.c` checks each ghost's reach and the long hand. The replay with the breaker plays 397 steps (440 with the old reach); the 31 hops beyond reach are the next routing work: a player reaches those cores through others, and the route will plan them once it plays by simulated touch.

### 69. One drag hopped twice and the tutorial never heard the touch

Native drops the drag once a pick connects (`func_0205fa2c` clears its drag flag when the chain settles) and picks the drag up again only while the stylus is within 16 pixels of the chain's end. The port kept dragging from wherever the stylus was, so one long drag picked a core and then the next one past it. The touch tutorial (`func_0203de90`, pGameMainSVTutorial) sends `onClick` on the first tick the chain is out or the d-pad is held, `onTouch` on every tick after that, and `onRelease` when it lets go; MoveStart sends `move` on every held tick, and the script's handler checks each time whether the soul now sits in the core it asked for (op 0xa0, `func_02027630`). The port sent `onClick` only for a press, ignored a touch while waiting for it, and sent `onTouch` and `move` once on arrival, so a player's drag through Chapter 1's first tutorial core went on to another and the tutorial never let go of the ghost view.

Guard: a held touch grabs the tip only within 16 pixels of it and an arrival ends the drag; the tutorial events follow `func_0203de90` every tick. The route waits on TouchCheck and drags, as a player does, in both touch tutorials (Chapter 1's st01_game000 and Chapter 6's st06_game002). `tests/unit/test_soul_touch_rules.c` drags past every core, drags on after a new touch, touches away from the tip, and checks each tutorial event. Without the end-of-drag rule the long drag reaches the third core.

### 70. A player could not choose a panel

Native's choice control (`func_0205a9e8`, state 9) keeps a marker on one panel: it starts on the first panel (`func_0205ad48`, unless a newly added panel is flagged), d-pad down moves it to the next and wraps to the first (`func_0205b0b8`), up moves it back and wraps to the last (`func_0205b110`), A confirms the marked panel, a tap on a panel picks it, and B cancels when the choice offers it, sending `select cancel` (state 12). The port had no marker: CROSS always took the first panel not yet chosen and the d-pad and CIRCLE did nothing, so a player could never pick a later panel first, and dead talks whose answer is not the first unseen panel could not be finished by hand. Only the route's `choose` step, which picks by name, got through.

Guard: the choice keeps a cursor that starts on the first panel; d-pad up and down move it with native's wrap, CROSS confirms it, and CIRCLE cancels when offered. `tests/unit/test_choice_input.c` picks the first panel, moves down past the last, moves up from the first, and cancels with and without the offer. Still open: the panels and the marker are not drawn, and a tap on a panel is not tested, because the port has no choice view yet.

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

The breaker is gone from `script_machine.c`, `script_soul.c`, `script_player.c` and the replay, with `scenes.txt` and its scene order in `read_ahead.py`; the honest replay's marks and cues are byte for byte what they were before. The mode byte's values are named once in `script_machine.h` (`SCRIPT_MODE_LOADING`, `SCRIPT_MODE_DEMO` for `pGameMainDemoMode`, `SCRIPT_MODE_NORMAL` for `pGameMainNormalMode`, `SCRIPT_MODE_NARRATION`, `SCRIPT_MODE_STAFF_ROLL`), where the files had kept their own copies under different names (value 2 was "normal" in one file and "demo" in another). The player's build is the standard build and the device loop's is the fast forward build; the fps counter, the unlock switch, the debug controls and the commands file are fast forward only, with no flags of their own.

Re-baking the route also showed that the route pacing does not reproduce: `route_pacing.py` never shortens a hold it once lengthened, so a fresh run from an empty pacing settles elsewhere (1330 steps done, 5 failed). The settled pacing is now kept in `tools/assets/game_pacing.txt` and seeds the next run while the route is unchanged.

Guard: `make route-check` fails if route steps are done out of order, more steps are passed over than the story passes over, or the replay never reaches GOTO. `tests/unit/test_scene_retry.c` checks that auto-play never moves a waiting scene on. `tests/tools/test_route_pacing.py` (`KeptPacingTest`) checks the seeding and keeping. `make test` compiles the app without `FAST_FORWARD_BUILD` as well (`standard_build_compiles`), and `tests/tools/test_no_session_paths.py` fails a source that reads a machine or session folder. Root causes 77 and 80 describe the breaker and are superseded.

## Still open
- Talks and balloons the remaster's text lacks (49 labels, root cause 60): bake them from the DS ROM's language file for the script.

- Which verb an object's action line shows. `st01_id_st01a_acn_*` holds state variants and native selection is not traced, so objects with an empty first variant show no action.
- Telephone, Lynne and the dead core have no name entry in the tables read so far.
- Action selection for player mark layouts: 14 shapes exist and the port uses 4 by a position rule.
- Effect classes with no port yet, named from the class registry in the RAM dump: `uTimeOverChainEffect` (id 50, the time-over sequence; constructor `func_020473ec`, 0xd8 bytes), `uPhoneBorderEffect` (id 59), `uBlackDarkness` (ids 151, 228, 229), `uLanternLightBlackSt02_01` (id 119, 57 sites in stage 2), `uNightVisionGoggle` (57), `uElectricTorch` (56). Class ids group by factory in `data_020ac194`; key any port by factory, as the chapter cards are.
- Plain effect kinds with no row in any remaster effect list: kind 3 (id 6, the ripple, 50 sites in nine stages), 0x83 (id 207, the dog's bark) and 0x84 (id 209). Where the remaster draws them from is not found.
- Telephone lines and neon (root cause 78): free units are tracked but not drawn. The line art comes from `func_0205693c` on the sprite set at `data_020cf724`, with `uTelephoneCore`'s icon at `data_020cfe2c`; neither set is mapped to a remaster texture yet.
- Op 0x9f (case at `0x020253bc`): on a `uGimmick`, writes `words[2]` into the top half of core `words[1]`'s `+0x14` word (`func_02026700`); the enable bit 0 of that word is ops 0x9d and 0x9e. What reads the top half is not found, so it is not ported (state nothing reads).
- Op 0x48 (case at `0x02024ae2`): writes `words[1] << 12` to `+0x124` of the balloon in slot 0 (`game+0xdfc`, `func_020106ec`). Its meaning is not traced; it is only seen after the routed chapters (st01_game020).
- Op 0x5b (14 scripts, always after op 0x4d) is native `func_02010874`: it clears the eight balloon slot words at `game+0xe1c` and closes the balloons with the force bit set (`func_020109c8`); the port closes every balloon, as op 0x57 does. Whether a closing animation plays is not traced.
- Ops 0x49 (st02_game000, object kaba) and 0xfd (st01_game012, st01_game021, st02_game022 and 11 more) are hit by the whole-game replay and not ported; 0x49 sets byte +0x11c of the slot-0 balloon (`func_0201070c`). 0xfd is the rollback checkpoint, not save data: `func_02011b04` copies the scene name (`game+0x1c`) to `game+0x1058` and builds a checkpoint object at `game+0x1098`, the one the rollback menu's second entry needs; the port's retry always reloads the scene.
- Op 0x119 (case at `0x02025540`) links the current actor's motion controller (+0x114) to a named actor's (`func_02059894`, `func_020598a4`). It is not ported and logs `op_not_ported`.
- `no_motion_row` for `sofa`, `table`, `ftendoor`, `souldead` and `carcass` is expected: their tables (`st06_sofa`, `table_wait`, `st05_tendoor_wait`, `onlycore_000`) belong to objects that are only a core or whose art is in the room plate, so nothing is drawn natively either.
- Soul reach (root cause 68): the st05 hop from the dictionary to the lamp (route step 218) is 182 units in the port but one hop in the footage, far past Sissel's native 48, so one of those cores sits in the wrong place or a core between them is missing. The route still places the soul there.
- When op 0x77's phone book appears. The port opens it as the op runs; native op 0x77 (case `0x020246fa`) calls `func_020123a0`, which only records a pending state on the sequencer (`func_02022f34` writes `+0x28` and `+0x2c`), and `func_02023094` applies it only while the current state's byte `+0xc` is set. In st02_game004, telephone_004_act030 runs `op_077 0 0` and then a wait and a talk (ops 0x2f, 0x37, 0x39) before it jumps to act060, and the footage shows that talk before the book. The route waits for act060 before Go There; what sets the state's `+0xc` byte is not traced.
- The rollback menu after a failed rescue (root cause 51 ports the retry) is not drawn. Native's rollback scene (`func_0203b72c`, started from `func_02015428`) offers choices; on retry it sends `*` `SYSTEM|RESTART` when any script handles it (13 scripts do, from st01_game021 on) and otherwise reloads the scene, and the other choices tear down (`func_0200fe74`) and leave. The port retries with the native default and draws no menu. `SYSTEM|LOAD START` (pushed in st01_game022) belongs with it: the event a scene gets when it is entered from a save.
- The stage 6 dog in room 65 (st06 game001, around route steps 144 to 160): run 19 drew its bark lines with no dog, while the footage shows the dog by the TV stand inside the same lines. Its two heights (y 264 and y 296) are the same ground point (200) under two table origins (point 0 at y -64 or -96), so it is not drawn low. More likely it is drawn behind the TV stand while the bark lines, an effect, draw on top: native draws each sprite on list `level + signed byte at actor+0x4f` (`func_0200bd94`), the port ignores that byte, and nothing found so far writes it with a plain `strb` (a wider store or computed address may). Present before the counted-loop port.
- The Minutes Before Death card's middle line (the time and place on the black band) is not drawn. Traced so far: the card is class `uBeforeDeathEffect` (effect ids 51 and 52, constructor `func_02048100`); its init `func_02048188` reads a four-digit time string from `game+0xd84` (`func_0203cbe4`) and passes it to a child `uBeforeDeathEffect::uSwfPlayWatch` (`func_0204821c`). Script op 0x7c writes that string (case at `0x0202475c`, `func_0202663c`, only when it is four characters); the port files 0x7c as stores-only. Where the place name comes from is not traced. Porting needs the time store, the place source and the drawn line together.
- How long the world stays held after a ghost view closes. The port holds it for the 13 ticks of the black spokes (`SOUL_VIEW_LEAVE_TICKS`), sends `SYSTEM|SV_END` and shows the player mark 5 ticks in. In the footage of Chapter 5's party room (st02_game022) the world stays still much longer. At 20 fps after the view closes at 8148.45, the ball hangs mid-bounce from 8148.70 to 8149.20, and the player mark and the countdown banner come back at about 8149.2, some 45 ticks after the press. After step 363's close at about 8132.15, demo100's camera (its `SV_END` handler, 0x42b6) moves at about 8133.15, some 60 ticks after. The ball's positions in the three views fit the port's physics only when each close holds the world about 45 ticks: view 1 opens at the top of the first bounce (180,111) and view 2 as it lands (176,159), while view 3 finds the ball 61 from the Toy Cake. Native sends `SV_END` from pGameMain's start (`func_02032328`) when the mode byte is 4, and pGameMainReturnNormal (`func_0203267c`) waits on `func_02010b38` (game+8 bits 9 and 10, game+0x1a); the frame count of the close itself is not read. The Chapter 5 block opens view 1 at once (step 405, pace instant) to absorb the difference; it plays to demo070 under the bake. If the hold is ported, every route's living time between views shrinks and the pacing must be rebaked.

- Ops 0xd1 and 0xd2 (9 and 8 sites, first met in st04_game001) save and restore the camera. Native 0xd1 writes the camera position (`func_0200f178`, `func_0200f190`) into two properties of the object found by `func_0202fb48(…, data_020cedc4, -1)`; 0xd2 reads them back, adds `func_0201230c`'s point and moves the camera there (`func_02026784`, then `func_0200f918(…, 1)`). Not ported; both log `op_not_ported`.
- Op 0xd3 (52 sites in 14 scripts, first met in st13_game000) is native `func_0200fa64`: it stores a point at `game+0xdac` and sets bit 26 of `game+0x10`, or clears the bit when either word is 0xffff. What reads the point is not traced. Not ported.
- The film frame's slide. Ops 0x71 and 0x72 (root cause 117) animate the frame in and out natively (states 2 and 3 of the panel at `game+0xfb4`); the port switches it at once, as it does for 0x6f and 0x70.
- The route driver starts a dead talk from any distance. In st04_game010 a `dead_talk` step on `linne_core` ran with the soul on the telephone, six hops away, and skipped the bell, the pot ride and Rindge's talk. The route now walks there, but whether the pad path on the Vita enforces reach for a dead talk is not checked.
- Missile's exchange (root cause 124), what is not ported or not checked: the swap effect (`func_0205e294`, `func_0205e2a4`, waited on by `func_0205e2b4`; the port waits the 0x19 chain ticks only), the outline drawn on the two cores, the 0x30-tick fade of the chain after Unlock (soul state 6), the button icons (the two pills are baked words, `Swap` and `Unlock`, with no icon, and their look is not compared to footage), and op 0xa5 outside a link, where native moves the chain root (`func_0206115c`). No routed scene uses a swap yet, so the pad path is covered by unit tests only.
- Effects (root cause 125), what is not ported or not checked: `st03_onnpu_00` picks one of eight labels from a roll and a second variable the game sets, which the converter cannot describe, so it still plays as one plain loop (it is the one name in `STILL_PLAYED_AS_A_PLAIN_LOOP` in `check_effect_scripts.py`). Ops 0xb3 and 0xb9 on class effects with no port (lamps, goggles, `ef_black`) do nothing. `coreactionexchangecore` stops at frame 30 until native sends it to its label `finish`; the sender is not found and no set bakes it yet. The staff roll and the fades were not compared to footage or seen on the device, and the ending's sub screen (`pGameSubEnding`) is not drawn.
- Chapter 18 (st13_game020 to 022, then st15) can be entered in `script_replay` with `--start chapter --chapter-mark 17 --set-flag 1232 --set-flag 11c0 --set-flag 1145 --set-flag 11c3 --set-flag 11ea`. That is a diagnostic entry, not a route. Its scripts still use these unported ops: 0xf6, 0xfd (st13), 0xc5, 0xd4, 0x08, 0x09 (st15_demo110).
