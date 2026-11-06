# srcVita

The PS Vita app for the Ghost Trick port, built on vitaGL. It installs on a real Vita and plays the opening of Chapter 1 at 60 fps, using art converted from the remaster.

The opening is a script for now, not the game running. The DS game logic, text boxes, audio and player input are not connected yet. See [What is missing](#what-is-missing).

No game data lives in this folder. Converted assets, captures and reference video stay in the ignored `extracted/` and `build/` folders.

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
| `tools/vita_loop.py` | Installs, launches and reads back from the Vita |
| `tools/longplay_check.py` | Compares the Vita's screen against the reference video |
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

Every mutation should be caught. A survivor means the tests have a gap.

## Make the assets

The converter reads your own copy of the remaster's Android files from `extracted/remaster_apk/`. It also needs ImageMagick's `magick`.

```bash
make -C srcVita/tools/assets
```

```bash
python3 -I srcVita/tools/assets/convert_assets.py --set chapter1_opening --output extracted/vita_assets/chapter1_opening
```

`tools/assets/chapter1_opening.json` lists what goes into the set and scripts the scene.

## Run on the Vita

The loop needs Vita Companion on the Vita, at 10.0.0.151. It uses FTP on port 1337 and the command port 1338, which stay up while the app runs, so nobody has to touch the device.

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

The log is `ux0:data/ghosttrick/run.log`. Each second it records frame timing, free memory and texture totals, and each animation reports which frames it showed and how many arrived late. Run `vita_loop.py` with `--help` for the other commands, including `capture <name>`, which saves the screen and pulls it back as a BMP.

## Check against the reference video

This needs the reference video at `extracted/reference/chapter1_720p60.mp4`, plus `ffmpeg`, `magick` and Python's `numpy`.

```bash
python3 -I srcVita/tools/longplay_check.py
```

It first checks that the Vita has the same `eboot.bin` and manifest as this machine. Then it uploads temporary scenes that hold one exact pose, captures each on the Vita, deletes them, and compares each capture with the matching video frame. Each measured sprite is also captured alone, so only its own pixels are scored. Only the DS camera's 720 by 540 window is compared, because the video shows no more than that. The check fails if a sprite sits more than 0.4 DS pixels from the video, if a sprite or the picture differs too much, if motion frames match the video out of order, or if the screen's left and right edges are wrong: wide rooms must have art there, and the close-up and chapter card must have black. Side-by-side images land in `build/longplay_check/`. Use `--score-only` to re-score saved captures without the device.

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
- **Input.** Buttons only drive the debug viewer: LEFT, RIGHT, UP and DOWN browse, CROSS pauses, TRIANGLE toggles the HUD, R opens the list, L restarts the scene, SQUARE saves a capture and START quits.

Some scene timing is hand-tuned rather than taken from the game: message holds use a fixed length, card fades were borrowed from Chapter 2, and the scene ends with a pan across the room so every object gets seen.

The converter was built and checked with the Android files only. The Steam copy has not been compared.
