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

Guard: `script_classes.c` holds every class rule in one place, keyed by the class ids read from the native class registry (id at +0x18 of each class info): which classes are human, which views each draws in, the soul style a class takes, and its default level (humans 6, dead gimmicks 4 from `func_02053010`, other gimmicks 9). The bridge and the motion code ask it instead of comparing ids. A new class goes in that file, never in a caller. Still open: the `uPastDeadSprite` itself is not ported. Native (class info `0x020ca128`, methods at `0x020a4e24`, update `func_020294f0`, draw `func_020570cc`) watches its source body object: while the source is hidden its level is 0; when the source becomes visible it fades in from 0 to 31 over 60 ticks, then can hold, fall and rise on timers, and it draws at that level. So native shows a body as a fading copy only while the body object is visible. The doubles were most likely body objects visible when native keeps them hidden; the hold-rule divergences in `object_visibility.md` (ops 0x96 to 0x99 showing targets native leaves hidden) are the first suspect. Porting both would bring back the bodies native does show, such as `caron_dead`, which is visible for long stretches in the late chapters.

## Still open

- Which verb an object's action line shows. `st01_id_st01a_acn_*` holds state variants and native selection is not traced, so objects with an empty first variant show no action.
- Telephone, Lynne and the dead core have no name entry in the tables read so far.
- Action selection for player mark layouts: 14 shapes exist and the port uses 4 by a position rule.
- Effect classes with no port yet, named from the class registry in the RAM dump: `uTimeOverChainEffect` (id 50, the time-over sequence; constructor `func_020473ec`, 0xd8 bytes), `uPhoneBorderEffect` (id 59), `uBlackDarkness` (ids 151, 228, 229), `uLanternLightBlackSt02_01` (id 119, 57 sites in stage 2), `uNightVisionGoggle` (57), `uElectricTorch` (56). Class ids group by factory in `data_020ac194`; key any port by factory, as the chapter cards are.
- Plain effect kinds with no row in any remaster effect list: kind 3 (id 6, the ripple, 50 sites in nine stages), 0x83 (id 207, the dog's bark) and 0x84 (id 209). Where the remaster draws them from is not found.
- Telephone lines: op 0x82 makes a `uTelephoneLine` free unit and op 0x84 kills every task of a class (always a telephone line class). Neither is ported, so no line is drawn and 0x84 has nothing to remove.
- Op 0x9f (case at `0x020253bc`): on a `uGimmick`, writes `words[2]` into the top half of core `words[1]`'s `+0x14` word (`func_02026700`); the enable bit 0 of that word is ops 0x9d and 0x9e. What reads the top half is not found, so it is not ported (state nothing reads).
- Op 0x48 (case at `0x02024ae2`): writes `words[1] << 12` to `+0x124` of the balloon in slot 0 (`game+0xdfc`, `func_020106ec`). Its meaning is not traced; it is only seen after the routed chapters (st01_game020).
- Ops 0x49 (st02_game000, object kaba) and 0xfd (st01_game012) are hit by the whole-game replay and not ported; 0x49 sets byte +0x11c of the slot-0 balloon (`func_0201070c`), 0xfd calls `func_02011b04`, which looks like save data.
- Op 0xf7 (case at `0x02026176`, 41 sites from Chapter 3 on) takes a name and a value. For `BG` it stores `value << 4` at `game+0xdd4` and hands it to the background object (`func_0200f1a8`, `func_0200a2f8`); for `*`, `?` or an object name it builds two visitors (`func_02026b18`, `func_02026b2c`) that apply the value to every matching object. Values are 140 to 400 out of 256. What the value sets (a scale, a level or a colour) is not read yet. Op 0x119 (case at `0x02025540`) links the current actor's motion controller (+0x114) to a named actor's (`func_02059894`, `func_020598a4`). Neither is ported; both log `op_not_ported`.
- `no_motion_row` for `sofa`, `table`, `ftendoor`, `souldead` and `carcass` is expected: their tables (`st06_sofa`, `table_wait`, `st05_tendoor_wait`, `onlycore_000`) belong to objects that are only a core or whose art is in the room plate, so nothing is drawn natively either.
- Soul reach: the port lets the soul pick cores within 112 + 16 px. Native's radius is `func_020615e8`: 168 px (`0xa8000`) in some modes (game+0x10 bit 17, or a dead-core mode with bit 19), 112 or 48 px in others, and otherwise a per-soul value at +0xa4 whose initial value is not found. The st05 hop from the dictionary to the lamp (route step 218) is 182 units in the port but one hop in the footage, so either Sissel's normal reach is larger than 112 or one of those cores sits in the wrong place. The route recovers by placing the soul there.
- The retry after a failed rescue is not ported. Native's rollback scene (`func_0203b72c`, started from `func_02015428`) offers choices; on retry it sends `*` `SYSTEM|RESTART` when any script handles it (13 scripts do, from st01_game021 on) and otherwise reloads the scene, and the other choices tear down (`func_0200fe74`) and leave. The port only keeps `rollback_is_on`, so a failed rescue sits until the stuck detector pushes `SYSTEM|RESTART` or the scene timeout moves on. Routed play never fails, so this matters for players and for unrouted chapters. `SYSTEM|LOAD START` (pushed in st01_game022) belongs with it: the event a scene gets when it is entered from a save.
- The stage 6 dog in room 65 (st06 game001, around route steps 144 to 160): run 19 drew its bark lines with no dog, while the footage shows the dog by the TV stand inside the same lines. Its two heights (y 264 and y 296) are the same ground point (200) under two table origins (point 0 at y -64 or -96), so it is not drawn low. More likely it is drawn behind the TV stand while the bark lines, an effect, draw on top: native draws each sprite on list `level + signed byte at actor+0x4f` (`func_0200bd94`), the port ignores that byte, and nothing found so far writes it with a plain `strb` (a wider store or computed address may). Present before the counted-loop port.
- The Minutes Before Death card's middle line (the time and place on the black band) is not drawn. Traced so far: the card is class `uBeforeDeathEffect` (effect ids 51 and 52, constructor `func_02048100`); its init `func_02048188` reads a four-digit time string from `game+0xd84` (`func_0203cbe4`) and passes it to a child `uBeforeDeathEffect::uSwfPlayWatch` (`func_0204821c`). Script op 0x7c writes that string (case at `0x0202475c`, `func_0202663c`, only when it is four characters); the port files 0x7c as stores-only. Where the place name comes from is not traced. Porting needs the time store, the place source and the drawn line together.
