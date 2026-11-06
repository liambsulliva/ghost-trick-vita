# srcVita

The PS Vita app for the Ghost Trick port, built on vitaGL. It installs on a real Vita and plays the opening of Chapter 1 at 60 fps, using art converted from the remaster.

The opening is a hand-written scene for now, not the game running. It has the opening's music, sounds, narration, balloons and spotlights. The DS game logic, talk text boxes and player input are not connected yet. See [What is missing](#what-is-missing).

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

A good first read is `src/app/viewer_app.c`, then `scene_player.c` and `scene_view.c`.

The scene fills the Vita's 960 by 544 screen at 2.8125 screen pixels per DS pixel, so wide rooms show about 85 more DS pixels across than the DS did. Art that stops short of the screen, such as room 11's close-up and the chapter card, is centred with black around it and never stretched. `docs/asset_format.md` has the rule.

## Build and test

You need VitaSDK with vitaGL, vitaShaRK, SceShaccCgExt, libmathneon and taihen installed.

```bash
export VITASDK=/usr/local/vitasdk PATH="/usr/local/vitasdk/bin:$PATH"
```

```bash
make -C srcVita -j8
```

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

## Make the assets

[The top-level README](../README.md#run-it-on-a-vita) explains how to copy the game files out of your Android package, convert them and put them on the Vita. The app loads them from `ux0:data/ghosttrick/assets/`.

`tools/assets/chapter1_opening.json` lists what goes into the set and scripts the scene.

Converting needs the text key, which step 2 of the top-level README finds in your own copy of the package.

Narration and bubble text are rendered by the converter, not the app. `convert_assets.py` reads the English text from the remaster's `.msg` files, decodes its `.oft` fonts into its temporary work folder, and draws each line with `magick` into its own DXT5 texture. The app draws these like any other texture, so it has no fonts. `magick` needs FreeType support, which `magick -list format | grep OTF` shows. Decoded fonts are deleted when the converter finishes, fails or is stopped with SIGINT, SIGTERM or SIGHUP, even twice. A SIGKILL can leave them in the system's temporary folder, and the next conversion removes any such folder older than an hour. The output folder holds each line only as a picture, and no story text or font reaches the output folder or git. The file formats and scene steps are in `docs/asset_format.md`.

Music and sounds are the remaster's own Ogg Vorbis files, unchanged apart from losing their wrappers.

The DS scene scripts are baked from your own USA DS ROM, if you have one, into `scripts/`, with the tables the app needs to run them. If `scripts/chain.txt` is there, the app plays the opening from those scripts. Otherwise it plays the hand-written scene. [The top-level README](../README.md#3-convert-the-art-sound-and-text) has the optional step, and `docs/asset_format.md` the format.

The converter bakes only what changed. `.bake_record.json` in the output folder holds hashes for each texture and script, so one is baked again only when its source bytes, its settings or the code that bakes it changes, or when its output file is missing or edited. Music, sounds, animations, the scene and the manifest are rewritten only when their bytes differ. On this Mac a clean conversion took about 4 seconds and a repeat with nothing changed about 0.2 seconds, and a repeat and a clean run gave the same files byte for byte. If a texture fails, it is named on stderr with `failed:`, the rest still bake, and the last good scene and manifest stay.

An animation can have up to 1024 frames, and the app allocates each animation's frame list to the size its own file needs.

## Run on the Vita

This loop is for working on the app. It needs Vita Companion on the Vita, at the address in `VITA_HOST` (10.0.0.151 if unset). It uses FTP on port 1337 and the command port 1338, which stay up while the app runs, so nobody has to touch the device.

```bash
python3 -I srcVita/tools/vita_loop.py upload-assets extracted/vita_assets/chapter1_opening
```

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

## Debug controls

SELECT turns the debug controls on and shows the HUD. While they are on, LEFT, RIGHT, UP and DOWN browse backgrounds and animations, CROSS pauses, TRIANGLE toggles the HUD, R opens the animation list and L restarts the scene. SELECT turns them off again, which resumes a paused scene and restarts one you browsed away from.

They stay off during normal play, so that no button can stall the scene. The log records every button press, pause and skipped hold, as `input pressed buttons=...`, `scene paused` and `scene hold_skipped`.

## The script VM

`script_machine.c` and the `script_*` files beside it run the DS's own script bytes, one tick per 16,666 microseconds. The core has no SDK calls and no clock, and everything it tells the rest of the app goes through the `void` functions in `script_outputs.h`. On the Vita those are `script_bridge.c`, which drives the same stage, effects, text, overlays and audio the hand-written scene uses. In `tests/golden/script_replay.c` they print. `script_player.c` reads the files before tick zero, one per frame, and owns the clock.

An op, table word, effect id or GameApp pair the VM does not know halts the whole machine by name. The last picture stays, a line at the bottom of the screen names the reason, the op, its offset, the tick, the file and the object, and the last 64 fetches go to the log. The loop keeps turning and the buttons still work.

From the Vita's commands file, `play_script route` plays the VM and `play_scene opening` plays the hand-written scene. X replays whichever one just finished, and it has no skip in script mode.

`make -C srcVita golden` compares the VM's fetches with the saved DS trace, on a machine that has one in `extracted/trace/vm_golden/`. It exits 2 with a message otherwise.

## Measured on the Vita

| What | Result |
|---|---|
| Frame rate through the full scene | 59.9 fps, about 16.68 ms a frame, no slow frames outside captures |
| Draw time | about 0.4 ms a frame |
| Late or skipped animation frames | none (14 of 14 and 38 of 38 frames shown) |
| Scene preload | about 2.4 s for 39 textures, none missing |
| Asset size | 8.6 MB of files, about 20 MB of texture memory |
| Free memory during the scene | about 66 MB video, 147 MB main |
| Sprite placement against the video | 0 offset on all 28 object and motion measurements |

vitaGL rounds compressed textures up to power-of-two sizes, so a texture can take about twice its file size in memory. The cache counts the rounded size, and the converter crops each atlas page to the cells in use.

## What is missing

- **Game logic past the opening.** The VM runs the opening's 61 distinct opcodes and stops at the act after it. The saved trace holds 63 in all, and Chapter 1's scripts hold 152, of which 89 no trace has run. The first op it would halt on is `0x37`, the first dialogue.
- **Talk text boxes, most effects and the HUD.** None are drawn yet. The opening's narration and its balloons are.
- **Player input.** There is nothing to play yet. CROSS skips to the next step of the script, or replays the scene once it has ended. SQUARE saves a capture and START quits.

The hand-written scene's timing is hand-tuned against the reference video rather than taken from the game. The VM's comes from the script, except narration holds, which follow `chain.txt`'s timing profile, and the card's ramp and the close-up's dissolve, which are named constants in `script_bridge.c`.

The converter was built and checked with the Android files only. The Steam copy has not been compared.
