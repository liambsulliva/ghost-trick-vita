# srcVita

The PS Vita app for the Ghost Trick port, built on vitaGL. It installs on a real Vita and plays the opening of Chapter 1 at 60 fps, using art converted from the remaster.

The opening runs from the DS's own script bytes on the script VM (see [The script VM](#the-script-vm)). Only Chapter 1 is baked, and parts of the game are still missing. See [What is missing](#what-is-missing).

No game data lives in this folder. Converted assets, captures, reference video and the text key stay in the ignored `extracted/` and `build/` folders.

## Where things are

| Path | What it holds |
|---|---|
| `src/main.c` | The main loop: read input, update, draw, log |
| `src/vita/` | Everything that calls the Vita SDK or vitaGL |
| `src/app/` | Everything else. It has no Vita calls, so the tests can run it on a Mac |
| `include/` | Headers for both |
| `tests/unit`, `tests/integration` | C tests, from one function up to the whole app |
| `tests/fakes/` | Stand-ins for `src/vita` that the C tests link against |
| `tests/tools/` | Tests for the asset converter |
| `tools/assets/` | The asset converter |
| `tools/assets/extract_text_key.py` | Finds the text key in your own copy of the Android package |
| `tools/vita_loop.py` | Installs, launches and reads back from the Vita |
| `tools/longplay_check.py` | Compares the Vita's screen against the reference video |
| `tools/audio_check.py` | Judges a run log's audio timing and checksums, with no listener |
| `tools/mutation_check.py` | Breaks the code on purpose to prove the tests notice |
| `docs/asset_format.md` | The file formats shared by the converter and the app |

A good first read is `src/app/viewer_app.c`, then `script_player.c`, `script_machine.c` and `scene_view.c`.

The scene fills the Vita's 960 by 544 screen at 2.8125 screen pixels per DS pixel, so wide rooms show about 85 more DS pixels across than the DS did. Art that stops short of the screen, such as room 11's close-up and the chapter card, is centred with black around it and never stretched. `docs/asset_format.md` has the rule.

## Build and test

You need VitaSDK with vitaGL, vitaShaRK, SceShaccCgExt, libmathneon and taihen installed.

```bash
export VITASDK=/usr/local/vitasdk PATH="/usr/local/vitasdk/bin:$PATH"
```

```bash
make -C srcVita -j8
```

The build plays the DS soundtrack by default, rendered from your own ROM during the bake. `make -C srcVita -j8 DS_MUSIC=0` plays the remaster's arranged soundtrack instead. The bake always writes both, so switching only needs a rebuild and a reinstall, and the build relinks every time so a changed flag always reaches the package.

This writes the installable package to `srcVita/build/ghosttrick.vpk`. It holds only the app. The art goes on the Vita separately, as [the top-level README](../README.md#run-it-on-a-vita) explains, and the Vita also needs its shader compiler at `ur0:data/libshacccg.suprx`.

The tests run on the Mac and need no SDK. The first command runs the C tests, and the second also runs the converter and tool tests, which need Python's `numpy` and ImageMagick's `magick`.

```bash
make -C srcVita test
```

```bash
make -C srcVita test-all
```

```bash
python3 -I srcVita/tools/mutation_check.py
```

Every mutation should be caught. A survivor means the tests have a gap. The run works on a copy of this folder plus `tools/lz.py` and `tools/lmg_decode.py`, so it leaves your files alone. It breaks one thing at a time, in the C code and the Python tools, including the text key tool. A C mutation rebuilds only the file it changed. A Python mutation runs the Python tests with `--failfast`, starting with the test files most likely to catch that tool and going on to all the others, so it stops at the first failure and is only called a survivor once every test has passed. `audio_check.py` and `audio_file.py` run only the audio tests, and `longplay_check.py` runs only its own. Before the Python mutations, the whole suite also runs once with its test files in reverse order, so a test that passes only in the usual order stops the run instead of making mutations look caught. The last full run had 778 mutations and took about 36 minutes on a busy Mac, about 11 of them in the Python ones. It does not cover `tools/rom_audit.py`, because it does not copy that file.

Before pushing, run the audit from the top of the repository with your key file in place. If it prints `key search skipped`, the key was not searched for.

```bash
python3 -I tools/rom_audit.py
```

### LiveArea art

The package's LiveArea icon, background and startup image are built while the package is built. With your extracted game files in place they come from the game's title logo; without them they are a plain monogram. To bundle the logo into your own package:

1. Run the setup script in [the top-level README](../README.md#set-everything-up-with-one-command), which puts the remaster's files in `extracted/remaster_apk`.
2. Build the converter's texture tool, which the logo needs to decode the game's texture:

   ```bash
   make -C srcVita/tools/assets
   ```

3. Build the package. It picks up the logo by itself:

   ```bash
   make -C srcVita -j8
   ```

4. Install `srcVita/build/ghosttrick.vpk` on the Vita with VitaShell. The Vita reads LiveArea art only when a package is installed, so replacing `eboot.bin` alone does not change it.

A package with the logo contains game art, so keep it for your own Vita. The package published on GitHub is built with `make -C srcVita release`, which always uses the monogram, as if no game files were present.

## Make the assets

[The top-level README](../README.md#set-everything-up-with-one-command) has one script, `tools/set_up_game.py`, that copies the game files out of your Android package, finds the text key, converts everything, plans the loading and builds the package. The app loads them from `ux0:data/ghosttrick/assets/`.

`tools/assets/chapter1.json` lists what goes into the chapter's set.

Converting needs the text key, which the setup script finds in your own copy of the package.

Narration and bubble text are rendered by the converter, not the app. `convert_assets.py` reads the English text from the remaster's `.msg` files, decodes its `.oft` fonts into its temporary work folder, and draws each line with `magick` into its own DXT5 texture. The app draws these like any other texture, so it has no fonts. `magick` needs FreeType support, which `magick -list format | grep OTF` shows. Decoded fonts are deleted when the converter finishes, fails or is stopped with SIGINT, SIGTERM or SIGHUP, even twice. A SIGKILL can leave them in the system's temporary folder, and the next conversion removes any such folder older than an hour. The output folder holds each line only as a picture, and no story text or font reaches the output folder or git. The file formats and scene steps are in `docs/asset_format.md`.

After every bake, run the loading step:

```bash
make -C srcVita read-ahead
```

It replays the chapter's scripts on the Mac at auto-play pace and writes two files beside each script in `scripts/`:

- `<script>.ahead` lists every texture the scene's actors and effects use, keyed to the scene's last `game` act and the ticks since it. The app reads each texture a few seconds before it is due, so a room change or a new animation finds its art already loaded.
- `<script>.anims` holds all of the scene's animations in one file. The app reads it once while the scene loads, so starting a new animation never waits on the memory card.

Upload the assets after this step, so the Vita gets both files.

Music and sounds are the remaster's own Ogg Vorbis files, unchanged apart from losing their wrappers.

The DS scene scripts are baked from your own USA DS ROM into `scripts/`, with the tables the app needs to run them. The app always plays the opening from those scripts, so the ROM is required. Without `scripts/chain.txt` it logs `opening needs_scripts` and opens the asset browser instead. [The top-level README](../README.md#set-everything-up-with-one-command) passes the ROM to the converter, and `docs/asset_format.md` has the format.

The converter bakes only what changed. `.bake_record.json` in the output folder holds hashes for each texture and script, so one is baked again only when its source bytes, its settings or the code that bakes it changes, or when its output file is missing or edited. Music, sounds, animations, the scene and the manifest are rewritten only when their bytes differ. On this Mac a clean conversion took about 4 seconds and a repeat with nothing changed about 0.2 seconds, and a repeat and a clean run gave the same files byte for byte. If a texture fails, it is named on stderr with `failed:`, the rest still bake, and the last good scene and manifest stay.

An animation can have up to 1024 frames, and the app allocates each animation's frame list to the size its own file needs.

vitaGL rounds compressed textures up to power-of-two sizes, so a texture can take about twice its file size in memory. The cache counts the rounded size, and the converter crops each atlas page to the cells in use.

## Run on the Vita

This loop is for working on the app. It needs Vita Companion on the Vita, at the address in `VITA_HOST` (10.0.0.151 if unset). It uses FTP on port 1337 and the command port 1338, which stay up while the app runs, so nobody has to touch the device.

```bash
python3 -I srcVita/tools/vita_loop.py upload-assets extracted/vita_assets/chapter1
```

Under `ux0:data/ghosttrick/assets/`, textures live in 64 folders: `textures/<two hex digits>/<name>.gtex`, where the digits are `crc32(name) % 64` in hex. The uploader moves an older flat copy into its folder by rename when the size matches, and uploads the file if the Vita refuses the rename. Upload while the game is running, because the app keeps the Vita awake only while it runs.

```bash
python3 -I srcVita/tools/vita_loop.py first-install
```

After the first install, `upload-eboot` quits the app and swaps in a new `eboot.bin`, which is much faster. Quit before launching, because launching a running app does not restart it.

```bash
python3 -I srcVita/tools/vita_loop.py upload-eboot
```

```bash
python3 -I srcVita/tools/vita_loop.py quit
```

```bash
python3 -I srcVita/tools/vita_loop.py launch
```

```bash
python3 -I srcVita/tools/vita_loop.py log
```

The log is `ux0:data/ghosttrick/run.log`. Each second it records frame timing, free memory, texture totals and how often the displayed picture really changed (`screen flips=... changed=... longest_still=...`). Moving objects log each frame change with the frames passed over, and each animation reports which frames it showed and how many arrived late. `system state overlay=... bgm_adopt=...` is logged at startup and whenever either value changes, because the only freeze seen so far came with the system UI overlaid and the BGM port not adopted. The SDK header says that adopt value means the port is in use, and no log has it from a healthy device before a music port opens, so 0 at startup means nothing yet; `music port ... adopt=0` is the line to look for. That freeze stopped the whole process with no crash until the app called `sceKernelPowerTick`, so it now does at startup and every 30 frames. It uses `SCE_KERNEL_POWER_TICK_DEFAULT`, which the SDK header describes as cancelling every timer, so that the screen does not dim or turn off during long stretches with no input, including on the end prompt and in browse view. **This tick kind is untested:** only `SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND` has been run on hardware, and neither the freeze nor the dimming has been checked with this one. Run `vita_loop.py` with `--help` for the other commands, including `capture <name>`, which saves the screen and pulls it back as a BMP.

`burst` pulls half-size pictures of the screen from the Vita into `--output`. Use a scratch folder: it fills with up to 1000 pictures of each kind. `--ms-between-pictures` sets the gap in milliseconds. It defaults to 200, five a second, and the app raises anything under 33 to 33.

```bash
python3 -I srcVita/tools/vita_loop.py burst --seconds 20 --ms-between-pictures 33 --output build/bursts
```

## Check against the reference video

This needs the reference video at `extracted/reference/chapter1_720p60.mp4`, plus `ffmpeg`, `magick` and Python's `numpy`.

```bash
python3 -I srcVita/tools/longplay_check.py
```

It first checks that the Vita has the same `eboot.bin` and manifest as this machine. Then it uploads temporary scenes that hold one exact pose, captures each on the Vita, deletes them, and compares each capture with the matching video frame. Each measured sprite is also captured alone, so only its own pixels are scored. Only the DS camera's 720 by 540 window is compared, because the video shows no more than that. The check fails if a sprite sits more than 0.4 DS pixels from the video, if a sprite or the picture differs too much, if motion frames match the video out of order, or if the screen's left and right edges are wrong: wide rooms must have art there, and the close-up and chapter card must have black. Side-by-side images land in `build/longplay_check/`.

Use `--score-only` to re-score saved captures without the device; a capture that is not there fails as not captured. Each device run deletes the old captures and pair images first, so what is saved always comes from one run. If a capture is missed, for example because the app froze, the check says whether the log is still growing and whether the app read its commands file, names any load failures, then scores the captures that run took and exits non-zero. If the device stops answering, it says so and does the same.

## Audio timing

`sceAudioOutGetRestSample` always returns 0 on this hardware, so the only sign of a gap is timing. The port runs at a fixed rate, so the time from one submit return to the next (the period) stays at one grain unless the port ran dry. These limits were measured on one device at 333 MHz:

| Port | Grain | Latest next submit after a return | Shortest block | Slack |
|---|---|---|---|---|
| Music, BGM | 92.88 ms | about 22 ms | 70.8 ms | 1 ms |
| Sound, voice | 11.61 ms | about 11.5 ms | 0.1 ms | 2 ms |

A submit later than that deadline starts its grain late: the period stretches, the next one stays at one grain, and the schedule stays shifted. That this means silence is inferred from the fixed rate; nothing has been checked by ear. A return that is itself late, with the next submit on time, stretches one period and shortens the next by the same amount, so nothing was lost.

So `late` counts a stretch over the slack only if the next period does not pay it back to within the slack. The count lands one grain after the stretch, which is the next window when the stretch was a window's last grain. A late return that uses up the deadline leaves a real gap, and the two periods add up to it. Two late returns in a row, both over the slack, count as late even with no gap.

Each window line logs `period_us_max`, the largest period, stretched or not, and `late`, the running count. `gap_us_max` is return to the start of the next submit. A music grain ramps in again when its submit will land past the deadline plus the slack, counted from when the last return should have come, using the previous grain's time from decode end to submit. It has to be decided ahead, because the grain after the silence is already in the port when the period is known. If the real deadline differs from the measured one, a gap is still counted but its grain gets no ramp, and a submit past the assumed deadline can ramp with no gap.

Most sound port opens stretch one early period by about 7.3 ms and the next period pays it back. One open in the saved logs instead slipped about 21 ms and never paid it back, in a window with no voice. Neither has a known cause. Later periods in the saved logs stay within about 1 ms of a grain. `audio_check.py` also fails any sound submit over 11 ms after a return, and any fitted rate over 0.5% off. It fails a late sound grain when a voice played in that window or the one before, because the count lands a grain after the gap and a gap in silence cannot be heard. So a slip like that one in the first window of a port fails if a voice starts in the same window, even after the slip.

**These numbers are provisional.** The music deadline was only measured with BGM adopt at 0, a state where the port ran at 44101.8 frames per second rather than 44100, and period jitter has never been measured per grain with adopt at 1. The sound slack rests on one per-grain probe and on window lines, which cannot show a single period. Until that sweep is repeated, `audio_check.py` holds `decode_us_max` under 18 ms and the decode share under 20%. `audio_check.py` prints the fitted rate and largest period of each port, so a port on the wrong clock or a slack that is too tight shows up in any run.

## How the music thread takes requests

The main thread never touches the decoder. It posts a request to the mailbox and the music worker acts on it.

- Each request replaces the last. post_wish overwrites the one struct music_wish in the mailbox and adds 1 to mailbox.generation, so the worker only ever sees the newest request.
- The worker keeps two counters. handled_generation is the last request it acted on while idle. fade_generation is the last request it has reacted to while playing. start_playing sets fade_generation = handled_generation, so the request that started the music does not fade it straight back out.
- While playing, a generation different from fade_generation starts a fade from the current gain, over that request's fade_ticks. A newer request during a fade replaces it, again from the current gain.
- While idle, a generation different from handled_generation is acted on: load the track if it changed, publish prepared_name, and start it if asked to play.
- A stop goes in this order: the fade reaches zero, one zero grain is sent, the last window is logged, the port drains and closes, and music stopped is logged. Only then does the worker look at the newest request.
- The worker owns the decoder, the track memory, the grain buffer and the port. The main thread touches only the mailbox and prepared_name, and only while holding audio_mailbox_lock.
- Nothing logs while holding the lock, so a slow log write cannot hold up the other threads.
- music_starve is the one exception: it does not change the generation, and the worker clears it when it reads it.

## Fast forward build

The build can play through the game by itself, at normal speed or far faster than a person:

```bash
make -C srcVita FAST_FORWARD=1
```

It adds `-ff` to the build id and keeps its objects in `build/obj_fast_forward`, so it never mixes with the normal build. It reads these extra lines from the commands file, which `vita_loop.py app-command` writes:

| Line | What it does |
|---|---|
| `auto_play`, `auto_play off` | Play by itself at normal speed, following the baked route in `route_steps.txt` with the same inputs a player has, and pausing long enough to read each page |
| `ff <n>` | Run `n` script ticks per frame, pressing buttons by itself |
| `ff_until script <name>`, `ff_until act <name>`, `ff_until tick <n>` | Fast forward until that point, then pause |
| `resume` | Carry on from a pause |
| `skip` | Same as L and R together: push on to the next act or script |
| `press <hex>` | Press a button mask for one frame |
| `drag x0 y0 x1 y1 frames delay` | Move an injected finger in a straight line from `x0,y0` to `x1,y1` over `frames` frames, after waiting `delay` frames |
| `drag_soul dx dy frames delay` | Start the finger on the soul's screen position and move it by `dx,dy` over `frames` frames, after `delay` frames. Does nothing if the soul is not on screen |
| `burst <seconds> [ms]`, `burst 0` | Record a half-size picture of the screen for that long, `ms` apart (default 200; anything under 33 is raised to 33), into one file on the Vita. It then pauses the script until `resume`. `vita_loop.py burst` drives this, so nothing talks to the Vita while it records |

Automatic play closes dialogue and follows the route in `tools/assets/chapter1.json`, baked to `route_steps.txt`, through the same inputs a player uses. Each step waits for a named game state. A step it can't finish logs `route_step_failed` and play carries on, and `make route-check` fails if a replay stops matching the route. Sounds stay quiet at speed. Each picture copy costs the game about 59 ms, so the game runs at about 50 fps during a burst and 59.9 fps outside one.

The Mac replay does the same with no device:

```bash
make -C srcVita build/tests/golden/script_replay
```

```bash
srcVita/build/tests/golden/script_replay --scripts extracted/vita_assets/chapter1/scripts --timing remaster --auto-press --marks --stop-tick 500000
```

Its `mark` lines say which scripts and acts began, which tricks fired and when the stuck check had to push.

## Controls

Touch is the main way to play, as the in-game tutorials teach. Every touch action also has a button.

| Input | What it does in a chapter |
|---|---|
| Touch | Drag the soul from its flame to aim it at a core, and tap pills and balloons |
| Left stick | Aim the soul in the soul view, more slowly with a smaller push. Pushing it in the living world opens the soul view |
| D-pad | The same as the left stick, at full speed |
| CROSS | Advance dialogue, or do the trick of the core you are in |
| L | Open the soul view, or go back from one you opened |
| CIRCLE | Go back from the soul view |
| TRIANGLE or SQUARE | Tap the left or right balloon, when a script opens one that can be tapped |
| R | The hourglass pill |
| START | The notebook pill |
| L and R together | Skip ahead |

## Debug controls

SELECT turns the debug controls on and shows the HUD. While they are on, LEFT, RIGHT, UP and DOWN browse backgrounds and animations, CROSS pauses, TRIANGLE toggles the HUD, R opens the animation list and L restarts the opening. SELECT turns them off again, which resumes a paused scene and restarts the opening if you browsed away from it.

They stay off during normal play, so that no button can stall the scene. The log records every button press, pause and skipped hold, as `input pressed buttons=...`, `scene paused` and `scene hold_skipped`.

## The script VM

`script_machine.c` and the `script_*` files beside it run the DS's own script bytes, one tick per 16,666 microseconds. The core has no SDK calls and no clock, and everything it tells the rest of the app goes through the `void` functions in `script_outputs.h`. On the Vita those are `script_bridge.c`, which drives the stage, effects, text, overlays and audio. In `tests/golden/script_replay.c` they print. `script_player.c` reads the files before tick zero, one per frame, and owns the clock.

Nothing halts the game. An op, table word, effect id or GameApp pair the VM does not know writes one note to the log and the script carries on. If the script stops making progress, a stuck check closes the talk box, ends the narration, fires a waiting handler, moves on to the next act and at last the next script, so the chapter can always be finished. L and R together does the same on demand.

`script_soul.c` is the player's side: soul view, core selection, tricks and the game events that only native code sends. Pressing the trick button sends `core|ready` at once and `core|action` 8 ticks later, as the DS does. A dead person's core sends `timeleap|start` instead.

From the Vita's commands file, `play_script route` restarts the opening. X replays it when it finishes. `play_scene <name>` plays a single baked scene, which the reference check uses to hold exact poses.

`make -C srcVita golden` compares the VM's fetches with the saved DS trace, on a machine that has one in `extracted/trace/vm_golden/`. It exits 2 with a message otherwise.

## What is missing

As of 2026-11-07. Items under "Built, not proven" are written but not checked on the device.

**Seen working on the device**

- Textures in their folders, with none missing.
- The core-to-core snap sound (sound 16), checked by ear.
- The soul's eight-point chain reaches about 48 DS pixels and shrinks back on release.
- No placeholder colours anywhere.
- The soul's ring and label clear when talk opens.
- DS music, touch, rain, split-screen rooms and the Chapter 2 card.

**Built, not proven**

- The chain's bend and trail. This needs a faster burst.
- The camera panning with the tip while it is held.
- Tap balloons advancing only while play runs.
- The Back pill, shown only in a ghost view the player opened.

**Open Chapter 1 findings**, checked against the reference footage

- The umbrella is dim and see-through in the phone view.
- A second speech bubble is missing late in the chapter.
- Some core labels are on the wrong side, or on one line instead of two.
- The thin pill words.
- The soul's fly-in from the trick pill.
- Film-still bubbles are about 40% too wide, and the bubble close is squashed.
- The camera is about 110 px off in the stairs room.
- The split-screen's left panel is black for about 1.4 s at its start.
- Short black or white holds that the footage does not have.
- The stairs walk is slow.
- Pacing: narration pages in the opening run about 9.6 s longer in total than the footage (the first page alone about 4 s). Talk pages cycle about every 2.8 s, against 2.5 s.

**Effects**

- Effect 31, the loops on the corpse in ghost view, is the remaster movie `cmn_shig_01` (native kind 0x1d, from the st01_game000 effect list). It is now drawn as that movie, shown only in ghost view; not yet checked on the device.
- Effect 4, the core ring, is drawn as a soft glow. The footage shows a thin white ring that pulses every 70 frames.
- Effects 8 (`efc_tsunagu`, kind 0xd8) and 9 (`efc_chain`, kind 0xce) have no art in any remaster effect list. `coreactionexchangecore` is an unconfirmed candidate for 8. Notes are in `extracted/audit_notes_2026-11-06/effects_8_9.md`.
- Effects without art are noted in the log and not drawn.

**Not in the port yet**

- **Soul view art past the rings.** Object tints and class gates are not drawn, and every core can be reached from every other, with no range limit.
- **Some HUD pieces.** The countdown runs and sends its events but draws nothing, and the same goes for the `EyeCatch` banners.
- **Touch.** The back touch pad is not read. The front screen works alongside the buttons: tap pills, balloons, Rewind Time and talk, drag the soul in Ghost view, and drag the camera where the game allows it.
- **A second chapter.** `chapter.xml` is not baked, so Chapter 1's end goes to an end screen.

The VM's timing comes from the script, except narration holds, which follow `chain.txt`'s timing profile, and the card's ramp and the close-up's dissolve, which are named constants in `script_bridge.c`.

The converter was built and checked with the Android files only. The Steam copy has not been compared.

## Roadmap

1. Finish Chapter 1 to zero findings against the footage. Each pass uses full-chapter burst captures and reviews.
2. Chapter 2, played by auto-play. The route has 93 steps and is listed in `extracted/audit_notes_2026-11-06/chapter2_route.md`. It starts at st01_game015, goes through `chapter.xml` to st14 (the parlor), then `phone.xml` with place 15, then st06 game000, demo000, game001, game002, game003 and game006, and back to `chapter.xml` for Chapter 3. The blockers are:
   - Phone-book place selection (register 0x1a).
   - The converter bakes one stage at a time, so `phone.xml` and st06 are dropped, and st06 text is baked per script.
   - Op 0x69.
   - Class-1 scripts are frozen in ghost view.
   - Effect 48 events.
   - SV_DEADCORE with DemoModeOn/Off, ReturnGame, OpenSubScreen and DeadTalk.
   - Ops 0x127, 0x128 and 0x12d.
   - About 20 missing effect rows, 67 and 52 among them.
   - Core positions.
   - DS music bgm 8, 21 and 43.
3. Later chapters, on the same data-driven code.
4. Make the APK completely optional. A future goal, not current work: a player who can supply only the DS cartridge gets the whole port from the DS game's own assets, and the remaster's files become an optional upgrade. Each part below serves that goal:
   - Art: the DS sprites, rooms and effects, converted from the cartridge's own graphics and drawn at the DS's resolution, scaled up to the Vita's screen.
   - Text: the DS text and font in place of the remaster's encrypted text, so no text key is needed.
   - Sound: the DS soundtrack is already rendered from the cartridge; DS sound effects would replace the remaster's.
   - The setup script would choose this path by itself when no APK is given, and the LiveArea would use the monogram.
