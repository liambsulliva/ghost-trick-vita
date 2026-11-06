# srcVita

The PS Vita app for the Ghost Trick port, built on vitaGL. It installs on a real Vita and plays the opening of Chapter 1 at 60 fps, using art converted from the remaster.

The opening is a script for now, not the game running. The DS game logic, text boxes, audio and player input are not connected yet. See [What is missing](#what-is-missing).

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
| `tools/audio_check.py` | Judges a run log's audio with no listener |
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

Every mutation should be caught. A survivor means the tests have a gap. The run works on a copy of this folder, so it leaves your files alone. It breaks one thing at a time, in the C code and the Python tools, including the text key tool. A C mutation rebuilds only the file it changed. A Python mutation reruns all the Python tests, except that `audio_check.py` and `audio_file.py` run only the audio tests. The last full run had 558 mutations and took about 75 minutes on a busy Mac, about an hour of it in the Python ones. It does not cover `tools/rom_audit.py`, because it copies only this folder.

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

The converter bakes only what changed. `.bake_record.json` in the output folder holds hashes for each texture, so one is baked again only when its source bytes, its settings or the code that bakes it changes, or when its output file is missing or edited. Music, sounds, animations, the scene and the manifest are rewritten only when their bytes differ. On this Mac a clean conversion took about 4 seconds and a repeat with nothing changed about 0.2 seconds, and a repeat and a clean run gave the same files byte for byte. If a texture fails, it is named on stderr with `failed:`, the rest still bake, and the last good scene and manifest stay.

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

The log is `ux0:data/ghosttrick/run.log`. Each second it records frame timing, free memory, texture totals and how often the displayed picture really changed (`screen flips=... changed=... longest_still=...`). Moving objects log each frame change with the frames passed over, and each animation reports which frames it showed and how many arrived late. Run `vita_loop.py` with `--help` for the other commands, including `capture <name>`, which saves the screen and pulls it back as a BMP.

## Check against the reference video

This needs the reference video at `extracted/reference/chapter1_720p60.mp4`, plus `ffmpeg`, `magick` and Python's `numpy`.

```bash
python3 -I srcVita/tools/longplay_check.py
```

It first checks that the Vita has the same `eboot.bin` and manifest as this machine. Then it uploads temporary scenes that hold one exact pose, captures each on the Vita, deletes them, and compares each capture with the matching video frame. Each measured sprite is also captured alone, so only its own pixels are scored. Only the DS camera's 720 by 540 window is compared, because the video shows no more than that. The check fails if a sprite sits more than 0.4 DS pixels from the video, if a sprite or the picture differs too much, if motion frames match the video out of order, or if the screen's left and right edges are wrong: wide rooms must have art there, and the close-up and chapter card must have black. Side-by-side images land in `build/longplay_check/`. Use `--score-only` to re-score saved captures without the device.

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

- **Game logic.** The scene is a fixed script. Running the DS scene scripts needs a VM for 57 opcodes, and the DS logic still has to be brought up behind these Vita layers.
- **The opening's spotlights and narration.** The real opening hides most of the room behind black with spotlight shapes, and shows narration over it. Here the room is fully lit, and the art for the spotlight shapes has not been identified.
- **Text boxes, speech bubbles, effects and the HUD.** None are drawn yet.
- **Audio.** Nothing plays yet.
- **Player input.** There is nothing to play yet. CROSS skips to the next step of the script, or replays the scene once it has ended. SQUARE saves a capture and START quits.

Some scene timing is hand-tuned rather than taken from the game: message holds use a fixed length, card fades were borrowed from Chapter 2, and the scene ends with a pan across the room so every object gets seen.

The converter was built and checked with the Android files only. The Steam copy has not been compared.
