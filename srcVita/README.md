# srcVita

The PS Vita app for the Ghost Trick port, built on vitaGL. It installs on a real Vita and plays the game at 60 fps with the remastered art, or a fixed 30 fps with the DS art, whose sprites change at most 15 times a second; the Options Framerate row is then held at 30.

The game runs from the DS's own script bytes on the script VM (see [The script VM](#the-script-vm)). The whole story plays from start to finish. [Known issues](#known-issues) lists what still differs from the original.

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
| `tools/pause_render.py` | Draws the pause menu, or with `--title` the title screen or `--loading` the loading screen, on the Mac from the baked set, after a list of button presses, through `tests/golden/pause_render.c` |
| `tests/golden/scene_render.c` | Plays the game on the Mac from a baked set with auto-play and saves the screen as PPM pictures at a tick, route step, script or act: `make scene-render`, then run `build/host/scene_render <set folder> <output folder> tick:N [frames] [frames between]` from `extracted/vita_assets`. Its fast-forward draws nothing, so the room's art loads only once saving starts: start a strip at least a second before the moment under test, or a snapshot (the split screen, the ghost view's entry, a film still) can catch an empty screen. `tests/host/` holds its disk files and software renderer |
| `tools/audio_check.py` | Judges a run log's audio timing and checksums, with no listener |
| `tools/mutation_check.py` | Breaks the code on purpose to prove the tests notice |
| `tools/smoke_test.py`, `tests/smoke/` | The [smoke test](#smoke-test): its driver, its flows and the Mac build of the whole app it runs them in |
| `docs/asset_format.md` | The file formats shared by the converter and the app |

A good first read is `src/app/viewer_app.c`, then `script_player.c`, `script_machine.c` and `scene_view.c`.

The scene fills the Vita's 960 by 544 screen at 2.8125 screen pixels per DS pixel, so wide rooms show about 85 more DS pixels across than the DS did, and talk boxes span the whole screen as they span the DS picture. Art that stops short of the screen, such as room 11's close-up and the chapter card, is centred with black around it and never stretched. `docs/asset_format.md` has the rule.

## Build and test

You need VitaSDK with vitaGL, vitaShaRK, SceShaccCgExt, libmathneon and taihen installed.

```bash
export VITASDK=/usr/local/vitasdk PATH="/usr/local/vitasdk/bin:$PATH"
```

```bash
make -C srcVita -j8
```

This is the player's build, at normal speed with none of the development tools. It plays the DS soundtrack, rendered from your own ROM during the bake, until Options switches to the remaster's arranged one; the bake always writes both. Work on the device uses the [debug build](#debug-build) instead. The two share every fix: they differ only in the dev tools the debug build adds, and `make test` compiles the app both ways. It also links the player's build on the Mac and lists the game functions nothing in that build reaches. A new one fails the test until it is deleted or named in `tests/player_link/unreached.txt`, the list of functions kept for the tests and the debug build. The source files in the Makefile's `DEBUG_ONLY_NAMES` are compiled into the debug build only. With the SDK installed, `make -C srcVita check-libs` relinks the app without each library in turn and fails if one is not needed. The build relinks every time, so a changed build always reaches the package.

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

Every mutation should be caught. A survivor means the tests have a gap. The run works on a copy of this folder plus `tools/lz.py` and `tools/lmg_decode.py`, so it leaves your files alone. It breaks one thing at a time, in the C code and the Python tools, including the text key tool. A C mutation rebuilds only the file it changed. A Python mutation runs the Python tests with `--failfast`, starting with the test files most likely to catch that tool and going on to all the others, so it stops at the first failure and is only called a survivor once every test has passed. `audio_check.py` and `audio_file.py` run only the audio tests. Before the Python mutations, the whole suite also runs once with its test files in reverse order, so a test that passes only in the usual order stops the run instead of making mutations look caught. The last full run had 778 mutations and took about 36 minutes on a busy Mac, about 11 of them in the Python ones. It does not cover `tools/rom_audit.py`, because it does not copy that file.

### Smoke test

The smoke test is the engine light for a release: it plays the things a player does most, through the whole app, and says which one broke. It needs both converted sets in `extracted/vita_assets` and reads them as they lie.

```bash
make -C srcVita smoke
```

```bash
make -C srcVita smoke SMOKE_QUICK=1
```

The first takes about three minutes; the quick one leaves out the flows that play every chapter and takes about one. `SMOKE_ARGUMENTS` passes the driver's own options through, such as `--only 'pause*'` for some flows or `--set remastered` for one set; `python3 -I srcVita/tools/smoke_test.py --help` lists them.

Every check runs whatever the others did, and prints one line: `ok`, `FAIL` with the first reason and the root causes it guards, `known` for a gap listed in `tests/smoke/known_gaps.txt`, or `skip`. The run ends non-zero if any line is `FAIL`. `build/smoke/report.txt` holds the same lines, `build/smoke/failures/` one file per failed check with its reasons, the end of its log and the folder the app ran in, and `build/smoke/pictures/` one contact sheet per flow for a look by eye; no check compares a picture with a kept one.

What it checks, in order:

- Each set: the manifest names its art and every background has a texture, every texture sits in its bucket folder, each language lists files that exist, every `.loop` file points inside its sound, and the baked route is the one in `tools/assets/game.json`.
- The whole-game replay of each set finishes every route step. It also writes each scene's start state into `extracted/smoke/states/`, which the flows start from.
- The debug build links and packs with VitaSDK (found through `VITASDK` or in `/usr/local/vitasdk`), and the package holds its files and the flow runner. `SMOKE_NO_SDK=1` skips this on a Mac with no SDK.
- The flows in `tests/smoke/flows.txt`. Each runs in `build/smoke_host/smoke_host`, which is `src/main.c` and all of `src/app` linked against a disk, a clock that steps one frame per input poll, a software renderer that draws every triangle the app sends and audio ports that pull from the real music and sound threads. A flow passes when the app finishes its lines, its log holds no line of a failing or unlisted kind, the route steps it finished are the ones the replay finished over the same stretch, and the pictures it names show something.

The flows cover booting to the title, the opening at a player's speed, each route action through the pad in the scene that first uses it, the retry after a failed rescue, every chapter and the whole story, the pause menu (resume, Save and Continue, Title, Chapters), the Record, the title's Chapter Select, New Game over a save, erasing saves, the language list, every Options row, a setting kept across launches, the credits, the Visual Filter relaunch, the hint, the touch controls, and the title, narration, talk, pause menu and Options in each of the nine languages, whose pictures must differ from the English ones.

A flow is a block of lines:

| Line | What it means |
|---|---|
| `flow <name>`, `about <text>`, `guards <root causes>` | Start a flow and say what it is for |
| `sets <installs>` | Which installs it runs on: `remastered`, `ds`, `both`. One run per install |
| `needs full` | Left out by `SMOKE_QUICK=1` |
| `for <name> in route_actions`, `story_parts`, `languages` | One flow per row, with `{name}` and the row's other values filled in |
| `start <scene>`, `chapters <n>`, `settings <name>=<value> ...` | Files the app finds at launch: a save at that scene, chapter saves up to `n`, a settings file |
| `frames <n>` | The most frames a launch may take |
| `shows <pictures>` | Captures that must not be black |
| `log_has <text>`, `log_lacks <text>`, `known <pattern> \| <reason>` | Log lines that must, must not, or may appear; a `known` line that stops appearing fails |
| `route_steps <list>` | The route steps the app must finish, in order |
| `differs_from <flow>` | Its pictures must differ from that flow's |
| `launch`, `launch relaunches` | Start the app; the second kind must end by relaunching itself |
| `block <name>`, `use <name>` | Name some lines and use them in a launch |
| `remastered: <line>`, `ds: <line>`, `both: <line>` | A line for one install only |

Under a `launch`, a line is a commands-file line of the [debug build](#debug-build), with buttons by name in `press`, or one of the flow runner's own:

| Line | What it does |
|---|---|
| `wait <frames>` | Let that many frames pass |
| `until <frames> [not] <condition>` | Wait for the condition, and fail the flow after that many frames without it |
| `expect [not] <condition>` | Fail the flow unless the condition holds now |
| `stall <frames> <text>`, `stall 0` | From here on, fail the flow when that many frames pass with no log line holding the text, so a softlock ends the flow in seconds; `stall 0` stops watching |

A condition is `log <text>` for a log line written since the flow began, each line counted once, or a word `smoke_condition_holds` in `src/main.c` answers from the app's state: `title`, `title_submenu`, `title_cursor <n>`, `title_dialog`, `pause`, `pause_submenu`, `pause_cursor <n>`, `pause_open`, `game`, `loading`, `paused`, `script <name>`, `tick <n>`, `shows talk`, `shows narration`, `shows choice`, `shows ghost_view`, `options_row <n>`, `record_tab <n>`, `route <wait>` with a route step's wait words, `set <kind>`, `language <code>`, `setting <name>=<value>`, `save auto`, `save manual`, `assets` and `textures_missing_most <n>`.

`tests/smoke/log_kinds.txt` sorts log lines by their first two words into `ok`, `fail` and `known`; a kind it does not list fails the flow, so a new warning gets a decision. `tests/smoke/known_gaps.txt` lists checks that fail for a stated reason; a listed check that passes fails the run, so the list can only shrink.

The flow runner is part of the debug build (`src/app/smoke_run.c`), so the same flows run on the Vita, where the app logs `smoke ok` or `smoke fail` with the line and the reason:

```bash
python3 -I srcVita/tools/vita_loop.py smoke
```

It runs the quick flows for the install the Vita holds (`--full` for all, `--only` for some), one launch at a time, and puts the player's saves and settings back when it ends; a copy of them stays in `build/smoke_device/device_backup`. Its report and pictures go to `build/smoke_device`.

```bash
make -C srcVita smoke-mutations
```

This proves the checks: it breaks the app in a copy of this folder and the set in a folder of links, one thing at a time, and requires the check for that thing to fail. It needs an earlier `make smoke` for the story, the start states and the package. `SMOKE_ARGUMENTS=--full` adds the breaks only the whole-story flows catch. It takes about two minutes, or five with `--full`.

Before pushing, run the audit from the top of the repository with your key file in place. If it prints `key search skipped`, the key was not searched for.

```bash
python3 -I tools/rom_audit.py
```

### LiveArea art

The package's LiveArea icon, background and startup image are built while the package is built. With the remaster's extracted files in place they come from the remaster's title logo; with only your DS cartridge dump at `extracted/ds_rom/game.nds` they come from the cartridge's own title logo over DS mode's red; with neither, the icon and startup image are the release art in `tools/livearea/release/`, a blue fire emoji from Twemoji on a near-black backdrop with the title in the banner (its `NOTICE` has the credit), and the background is a plain monogram. To bundle the remaster's logo into your own package:

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

A package with the logo contains game art, so keep it for your own Vita. The package published on GitHub is built with `make -C srcVita release`, which always uses that release art, as if no game files were present.

### Publish a release

Releases are built and published by `.github/workflows/release.yml`. To cut one:

1. Set `APP_VER` in `Makefile` to the new version as `MM.mm`, so `01.01` for v1.1.0. A pre-release such as v1.0.0-beta.1 shares its `APP_VER` with v1.0.0.
2. Commit, then tag and push the tag:

   ```bash
   git tag v1.1.0
   ```

   ```bash
   git push origin v1.1.0
   ```

The workflow builds in the `vitasdk/vitasdk` container. It refuses a tag whose major and minor numbers do not match `APP_VER`, runs `tools/rom_audit.py`, installs vitaGL, vitaShaRK, SceShaccCgExt, libmathneon and taihen with `vdpm`, runs `make -C srcVita release`, and attaches `ghosttrick-<tag>.vpk` and its SHA-256 to a GitHub release of that tag, with notes listing the changes since the last one. A tag with a hyphen in it, such as `v1.0.0-beta.1` or `v1.0.0-rc.1`, is published as a pre-release, so the Releases page does not show it as the latest version. Running the workflow by hand with an existing tag rebuilds and republishes that tag.

## Make the assets

[The top-level README](../README.md#set-everything-up-with-one-command) has one script, `tools/set_up_game.py`, that copies the game files out of your Android package, finds the text key, converts everything, plans the loading and builds the package. The app loads them from `ux0:data/ghosttrick/assets/`.

`tools/assets/game.json` lists what goes into the set.

Converting needs the text key, which the setup script finds in your own copy of the package.

Narration and bubble text are rendered by the converter, not the app. `convert_assets.py` reads the text from the remaster's `.msg` files, decodes its `.oft` fonts into its temporary work folder, and draws each line with `magick` into its own DXT5 texture. The app draws these like any other texture, so it has no fonts. `magick` needs FreeType support, which `magick -list format | grep OTF` shows. Decoded fonts are deleted when the converter finishes, fails or is stopped with SIGINT, SIGTERM or SIGHUP, even twice. A SIGKILL can leave them in the system's temporary folder, and the next conversion removes any such folder older than an hour. The output folder holds each line only as a picture, and no story text or font reaches the output folder or git. The file formats and scene steps are in `docs/asset_format.md`.

`game.json`'s `talk.pages_left_out` names talk pages the port does not bake, by `.msg` entry name. Every language is a slot of the same entry, so a page left out is gone in all of them, and the pages after it keep their own DS face and box, which are read by the page's place in the original message. A name that matches no talk page stops the bake. It holds pages 05 to 07 of `m01_0460`, the cot tutorial's "you first have to return to the land of the living" before a trick and Sissel's reply, which the ghost-view trick (see Controls) makes untrue; the talk goes from the cot's trick straight to Back being the way out. `tests/tools/test_talk_pages_left_out.py` covers it.

After every bake, run the loading step:

```bash
make -C srcVita read-ahead
```

It replays the game's scripts on the Mac at auto-play pace and writes two files beside each script in `scripts/`:

- `<script>.ahead` lists every texture the scene's actors and effects use, keyed to the scene's last `game` act and the ticks since it. The app reads each texture a few seconds before it is due, so a room change or a new animation finds its art already loaded. Its first line, `next <script>`, names the scene the paced replay played next. A script with no animations of its own, such as a chain's root, gets an `.ahead` holding only that line plus a `next_for <chapter mark> <scene>` line for each chapter mark it began at, and the app starts parsing the first scene for the save's chapter mark while the chain's loading screen is up.
- `<script>.anims` holds all of the scene's animations in one file, so starting a new animation never waits on the memory card. While a scene plays, the app unpacks and parses the next scene's bundle on a background task and adds it when that scene begins. A scene the plan did not name is loaded as before, on its first frame.

It also paces the route into `scripts/route_pacing.txt`. Pacing depends on the passes before it, so a fresh run does not reproduce a settled one: each settled pacing is kept in `tools/assets/game_pacing.txt`, and the next run starts from it while the route is unchanged.

It also writes `scripts/chapter_starts.txt`, the state (flags, notes, chapter mark) the settled replay had each time it entered `chapter.xml`, one `chapter_start <chapter>` section per chapter from 2 to 18. `make route-check` fails when that file differs from a fresh replay's.

Upload the assets after this step, so the Vita gets every file.

Music and sounds are the remaster's own Ogg Vorbis files, unchanged apart from losing their wrappers.

The DS scene scripts are baked into `scripts/`, with the tables the app needs to run them: from your own USA DS ROM when the converter is given one, and otherwise compiled from the Android package (see below). The app always plays the opening from those scripts. Without `scripts/chain.txt` it logs `opening needs_scripts` and opens the asset browser instead. [The top-level README](../README.md#set-everything-up-with-one-command) passes the ROM to the converter, and `docs/asset_format.md` has the format.

The converter bakes only what changed. `.bake_record.json` in the output folder holds hashes for each texture and script, so one is baked again only when its source bytes, its settings or the code that bakes it changes, or when its output file is missing or edited. Music, sounds, animations and the manifest are rewritten only when their bytes differ. On this Mac a clean conversion took about 4 seconds and a repeat with nothing changed about 0.2 seconds, and a repeat and a clean run gave the same files byte for byte. If a texture fails, it is named on stderr with `failed:`, the rest still bake, and the last good manifest stays.

### Without the DS cartridge

`convert_assets.py --apk-only` builds the remastered set from the Android package alone, and `set_up_game.py` passes it when no ROM is given. The converter then never opens the ROM: an audit hook refuses any open of the ROM path or of any `.nds` file, and the build stops with `must read nothing from the DS cartridge`.

The two things the cartridge gave come from the package instead:

- **Tables.** `tools/assets/remaster_tables.py --package <apk>` runs the static constructors of the package's `libruntime.so` in a small AArch64 runner (`tools/assets/arm64_runner.py`) and saves rooms, sounds, core infos, faces, effect kinds and the script constants into `extracted/remaster_apk/remaster_tables.json`. `chapter_assets.RemasterTables` serves them where `Arm9Tables` reads the ARM9 binary. `tests/tools/check_remaster_tables.py` compares them with the cartridge's. The remaster's tables differ in places: rooms 153 to 188 are shifted by three, the face table has 271 rows where the cartridge's reader ran on past its end, and 12 effects are the remaster's replacements.
- **Scripts.** The package holds each scene as an RSZ record list (`natives/android/scriptcode/data/script/**.user.2`). `tools/assets/remaster_scripts.py` compiles each list back into a DS 1LMG script that the script VM runs as native `func_02023fdc` does. `tools/assets/remaster_script_classes.json` gives each record class its fields and how its words map onto the DS op. The rules are: jumps name records, a block op takes the jump after it, a message names the label of the entry its GUID points to, motion tables are re-packed per frame and drop repeated values as `func_0200853c` reads them, and table names come from the scene's chara resource list. Message files and chara lists are found from the stages and scenes of the chain itself. `tests/tools/check_remaster_scripts.py` compiles all 232 files and pairs each with its DS script: 99.6% of the paired ops are the same, and the rest are the remaster's own edits. Content no DS op can say is left out and counted on the `left out of the Android scripts` line. The whole-game replay (`make read-ahead`) passes all 1729 route steps on these scripts, as it does on the cartridge's.

The text the cartridge gave is read from the package too. Faces and poses come from each message entry's `CutInOrFace` and `CutInFade` attributes (`talk_assets.read_remaster_faces`). Narration cues come from the page's TRCK tags (`message_cues.remaster_page`: groups at `paragraph_end`, the last `mes_wait_set` of each, `auto_pf` and `mono_type_set`). System lines come from `languages.json`. DS music (`ds_music` in the set) is left out with a line saying so. `tests/tools/test_apk_only_build.py` covers the tripwire, the table provider, faces, cues, system lines and the script bake.

### Languages

The set holds all nine of the remaster's languages: English, Japanese, French, Italian, German, Spanish, Traditional Chinese, Simplified Chinese and Korean. Every story line, name and note is the game's own text, read from that language's slot of the remaster's message files. `game.json` names `tools/assets/languages.json`, which gives each language its message slot, the DS language file whose cues it uses (English's for the four languages the USA cartridge lacks), the suffix of its art with words in it (the title logo and the chapter cards), the remaster fonts it swaps in, and the port's own words in that language: menus, help lines, HUD and pause labels, and the "New info" line. Those words are the port's translations, not game text.

The converter bakes English first, then runs every text stage again for each other language. An output whose bake key matches English's is not written twice. One that differs goes into `lang/<code>/`, and `lang/<code>/names.txt` lists them. The converter fails a bake when a line uses a character its font cannot draw, and names any talk line wider than its box.

Display Language in Options, and Language on the title screen, switch the language at once. Menus redraw in it, and the next talk page, narration line, bubble and card come in it; a talk already open finishes in the old language. On the first start, with no settings file, the app picks the Vita's system language when it is one of the nine, and English otherwise. The choice is saved in `settings.txt` as `language`, from 0 (English) to 8 (Korean) in the order the list shows; saves hold no text, so a save works in any language.

An animation can have up to 1024 frames, and the app allocates each animation's frame list to the size its own file needs.

vitaGL rounds compressed textures up to power-of-two sizes, so a texture can take about twice its file size in memory. The cache counts the rounded size, and the converter crops each atlas page to the cells in use. A compressed texture of 512 KB or more is only allocated on the frame; vitaGL's own swizzle fills it on the work queue's thread, and the texture counts as loading until it is filled. The reader keeps one 16 MB block for unpacking textures over 4 MB (`src/app/read_buffer.c`), so a large texture never waits on a heap too broken up to hold one; a smaller unpack the heap refuses takes the block too while it is free. When the heap cannot hold a packed file itself, `src/app/texture_read.c` reads it into the block's end and unpacks it into the block's start. The remaster's effect movies come on pages up to 4096 px that are mostly clear. The converter (`tools/assets/effect_pieces.py`) keeps only the 32 px squares of each frame that show, each with a 4 px border copied from its source page, and packs them onto pages of at most 2048 px. A movie keeps all its pages loaded while it plays, so the converter refuses one whose pages would take more than 16 MB. Movies with a bent quad keep each frame whole.

## Run on the Vita

This loop is for working on the app. It needs Vita Companion on the Vita, at the address in `VITA_HOST` (10.0.0.151 if unset). It uses FTP on port 1337 and the command port 1338, which stay up while the app runs, so nobody has to touch the device.

```bash
python3 -I srcVita/tools/vita_loop.py upload-assets extracted/vita_assets/game
```

The uploader copies the set's own layout, texture buckets included ([the asset format](docs/asset_format.md)). It moves an older flat copy on the Vita into its folder by rename when the size matches, and uploads the file if the Vita refuses the rename. What the Vita already holds is recorded beside the set, in `extracted/vita_assets/game.uploaded.json` (the set's real path plus `.uploaded.json`), so a clean copy of the repo that links `extracted` skips the same files; an older `srcVita/build/uploaded_assets.json` is read when that file is missing. Each file is read back once and checked by hash before it replaces the old one. A new file costs about a second, mostly in the Vita's card writes, so a set with a few thousand new movie frames takes the better part of an hour; the uploader prints a line every 100 files. Upload while the game is running, because the app keeps the Vita awake only while it runs.

```bash
python3 -I srcVita/tools/vita_loop.py first-install
```

After the first install, `upload-eboot` quits the app and swaps in a new `eboot.bin`, which is much faster. While the app tears down, the Vita refuses writes into its folder with `550`, so each write is tried up to 10 times a second apart. Quit before launching, because launching a running app does not restart it.

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

Only the debug build writes a log, `ux0:data/ghosttrick/run.log`, with up to 128 MB in it and 128 MB more in `run.log.earlier`. The player's build formats nothing, has no log writer linked in, and deletes both files when it starts, so a player who once ran a debug build gets the space back. `make test` fails if the player's build reaches `log_sink_start`, `log_sink_write` or `log_sink_flush`. `make -C srcVita release-debug` builds the debug package the Releases page publishes beside the player's. Each second the log records frame timing, free memory and texture totals; the debug build also records how often the displayed picture really changed (`screen flips=... changed=... longest_still=...`), which the player's build leaves out because it reads the frame buffer on every frame. Moving objects log each frame change with the frames passed over, and each animation reports which frames it showed and how many arrived late. `system state overlay=... bgm_adopt=...` is logged at startup and whenever either value changes, because the only freeze seen so far came with the system UI overlaid and the BGM port not adopted. The SDK header says that adopt value means the port is in use, and no log has it from a healthy device before a music port opens, so 0 at startup means nothing yet; `music port ... adopt=0` is the line to look for. That freeze stopped the whole process with no crash until the app called `sceKernelPowerTick`, so it now does at startup and every 30 frames. It uses `SCE_KERNEL_POWER_TICK_DEFAULT`, which the SDK header describes as cancelling every timer, so that the screen does not dim or turn off during long stretches with no input, including on the end prompt and in browse view. **This tick kind is untested:** only `SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND` has been run on hardware, and neither the freeze nor the dimming has been checked with this one. Run `vita_loop.py` with `--help` for the other commands, including `capture <name>`, which saves the screen and pulls it back as a BMP.

`burst` pulls half-size pictures of the screen from the Vita into `--output`. Use a scratch folder: it fills with up to 1000 pictures of each kind. `--ms-between-pictures` sets the gap in milliseconds. It defaults to 200, five a second, and the app raises anything under 33 to 33.

```bash
python3 -I srcVita/tools/vita_loop.py burst --seconds 20 --ms-between-pictures 33 --output build/bursts
```

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

## Debug build

The device loop installs this build. It can play through the game by itself, at normal speed or far faster than a person, and holds every development tool:

```bash
make -C srcVita DEBUG=1
```

It adds `-debug` to the build id and keeps its objects in `build/obj_debug`, so it never mixes with the player's build. It adds an Unlock Everything switch at the end of Options: while it is on, every chapter in the Chapters grid opens and every People and Places note shows at its highest level. A chapter with no save starts from its entry in the set's `scripts/chapter_starts.txt`, entering `chapter.xml` with the flags and notes the whole-game replay had there, so it opens in its own scene and a Phone book ride stays at that point in the story. Chapter 1, and any chapter the set has no entry for, start through `chapter.xml` with empty flags and the chapter mark one below the chapter; the log says which with `save chapter_select chapter=<n> baked_start=<0|1>`. The player's build ignores the saved switch. SQUARE saves a screen capture, and the [debug controls](#debug-controls) are on. It reads these lines from the commands file, which `vita_loop.py app-command` writes. A thread of its own looks for the file ten times a second (`src/vita/command_reader.c`), so a debug run's frames carry no card opens the player's build would not:

| Line | What it does |
|---|---|
| `auto_play`, `auto_play off` | Play by itself at normal speed, following the baked route in `route_steps.txt` with the same inputs a player has, and pausing long enough to read each page |
| `ff <n>` | Run `n` script ticks per frame, pressing buttons by itself |
| `ff_until script <name>`, `ff_until act <name>`, `ff_until tick <n>` | Fast forward until that point, then pause |
| `ff_until ready <step>` | Fast forward until that route step is ready for its input, then pause |
| `resume` | Carry on from a pause |
| `smoke <name>` | Hand the rest of the commands file to the flow runner as the flow `name`; see the [smoke test](#smoke-test) |
| `press <hex>` | Press a button mask for one frame |
| `drag x0 y0 x1 y1 frames delay` | Move an injected finger in a straight line from `x0,y0` to `x1,y1` over `frames` frames, after waiting `delay` frames |
| `drag_soul dx dy frames delay` | Start the finger on the soul's screen position and move it by `dx,dy` over `frames` frames, after `delay` frames. Does nothing if the soul is not on screen |
| `burst <seconds> [ms]`, `burst 0` | Record a half-size picture of the screen for that long, `ms` apart (default 200; anything under 33 is raised to 33), into one file on the Vita. It then pauses the script until `resume`. `vita_loop.py burst` drives this, so nothing talks to the Vita while it records |
| `fps on`, `fps off` | Draw the frame rate in the top-left corner: the average over the last 60 frames and the slowest of them, so a dip shows in screenshots and burst captures |

Automatic play closes dialogue and follows the route in `tools/assets/game.json`, baked to `route_steps.txt`, through the same inputs a player uses. Each step waits for a named game state. A step it can't finish logs `route_step_failed` and play carries on; nothing pushes a stuck scene on, so the route alone has to reach the end. `make route-check` fails if a replay does a step out of order, passes over more steps than the story does, or never reaches the game's end. Sounds stay quiet at speed. Each picture copy costs the game about 59 ms, so the game runs at about 50 fps during a burst and 59.9 fps outside one.

The Mac replay does the same with no device:

```bash
make -C srcVita build/tests/golden/script_replay
```

```bash
srcVita/build/tests/golden/script_replay --scripts extracted/vita_assets/game/scripts --timing remaster --auto-paced --marks --stop-tick 4200000
```

Its `mark` lines say which scripts and acts began, which tricks fired and which route steps were done or failed.

## Controls

Touch is the main way to play, as the in-game tutorials teach. Every touch action also has a button.

| Input | What it does in a chapter |
|---|---|
| Touch | Drag the soul from its flame to aim it at a core, and tap pills and balloons |
| Left stick | Aim the soul in the soul view, more slowly with a smaller push. In the living world it pans the camera |
| Right stick | Pan the camera, in the living world and the soul view |
| D-pad | The same as the left stick, at full speed. Neither moves anything while a cutscene, a trick or a talk has the controls |
| CROSS | Finish or advance dialogue and narration (in dialogue the first press finishes text still revealing or fading in and the next moves on; in narration each press shows only the next line in full, so rapid presses still step through every line in order, and the press after the last line moves on), do the trick of the core you are in (in the soul view too, which it leaves first), or swap the two objects Missile has linked |
| L | Open the soul view, or go back from one you opened |
| CIRCLE | Go back from the soul view, or unlock Missile's link. Held while dialogue or narration shows, it finishes and moves on through the text as fast as the game allows (a press each tick); it never picks a choice |
| TRIANGLE or SQUARE | Tap the left or right balloon, when a script opens one that can be tapped; balloons show and answer only while the player has control, not during a scripted event |
| R | The hourglass pill: asks "Start over from the beginning?" (No marked) while the world holds still; Yes restarts the fate attempt from the start of the four minutes, as a retry after a death does, and No goes back to play. It shows and answers only during the four minutes before a death |
| SELECT | Opens the pause menu straight on the Record (People) screen while the pause is offered (see START), as the notebook pill does; CIRCLE there goes back to the game |
| START | Open the pause menu, as does a tap on the pause button at the top right: Resume, Save, Record, Title, Chapters and Options. The button shows, and START and SELECT answer, only in the living world while the player has the controls and a pill shows: never in Ghost view, where the world is already stopped, nor during a scripted event or a cutscene |

### Hints

The original has no hints. The port offers one from the auto-play route (`src/app/script_hints.c`), so every routed scene is covered with no code or text of its own.

- **The clock.** It counts only ticks where the player has the controls, in the living world or the ghost view. Talk, narration, choices, the phone book, scripted events and the pause menu do not count. It starts again when the player does the route's next object step or a new scene loads, and it carries on through a retry after a death. Options has the wait: Off, 2, 5 (the default) or 10 minutes.
- **The balloon.** Once the wait is up, a "?" tap balloon opens over the core Sissel is in. It follows the tap balloons' rules: living world only, and only while the player has control. It takes SQUARE if no balloon has it, otherwise TRIANGLE if no balloon or character pill has it, and it waits while both are taken. A tap on it works too.
- **The hint.** It points at the object of the route's next step that acts on something: a trick, a balloon, a swap, a character button, a talk with the dead or a phone answer. A step with no object on the stage points at the core the soul rides just before it, and a step with neither is passed over for the next one. If the object is off screen the camera pans to it, holds for a second and pans back, with the world paused and the player mark shown on return. The object flashes white three times and then once every five seconds. In the ghost view its core pulses instead. Both stop when that step is done.
- **One per scene.** After a hint is used, no other balloon opens until the game loads another scene.
- **Where it cannot help.** The route knows one solution. A player who solves a beat another way is followed only when an object reaches an act a later step waits on. After a death the route cannot tell how far back the scene went, so the hint restarts only at a step the route marks `retried`. A target in another room gives no balloon until the camera can reach it.

Each pill and the pause button drops into its yellow pressed look whenever its button or a tap takes it, CROSS on a core with no trick included, where the refusal sound plays and nothing happens.

In a soul view the player opened, the Trick pill shows beside Back whenever the soul's core has a trick ready: the Trick button is registered, the core is not busy, its core info names an action, and no talk, tutorial, exchange or other scripted event holds the soul. CROSS or a tap on it plays the Back press first (normal play, the leave effect, `SV_FINAL`, the player mark and `SV_END`), then on the tick the leave ends fires the trick through the same `fire_trick` as a living-world press: `core ready`, then `core action` 8 ticks later. So whatever the trick starts sees the same state the DS's two presses leave (Back from pGameMainSV through pGameMainReturnNormal, then the Trick press `func_0203696c`). If the trick is no longer playable when the leave ends, it is dropped and logged as `trick_from_ghost_view_dropped`. Talk, narration, choices and Missile's swap still take CROSS first. Auto-play is unchanged: its `trick` step still presses CROSS once the Trick pill is up, and the routes keep their `ghost_off` steps. `tests/unit/test_trick_from_ghost_view.c` covers it.

The app opens on the title screen (`src/app/title_menu.c`), as does the pause menu's Title entry and the end of the story. With a save it shows Chapter Select, Continue (picked first), New Game, Options, Language and the trash can, which erases every save after asking. Without one it shows New Game, Options and Language. Once the story has been finished, a notebook button at the top left opens the People screen with every note. The d-pad, left stick or a tap moves between them, CROSS picks and CIRCLE backs out of a screen. A picked button turns blue in a red ring, the remaster's pressed look, and keeps it while the screen flashes, holds and fades out or pans to the chosen screen.

The pause menu keeps settings in `ux0:data/ghosttrick/settings.txt`, the Manual Save and Autosave slots in `save_manual.txt` and `save_auto.txt`, each chapter's start in `chapter_NN.txt`, and how much of Sissel the Record reveals in `record_reveal.txt` (`src/app/record_reveal.c`; the triggers and faces are the `reveal` lines the converter writes into `pause/people.txt` from `game.json`'s `reveals`, and erasing the saves deletes it); the saves use `src/app/game_save.c`'s text format. A save holds the state at the start of the current script. Saves and settings are written by one thread that runs writes in order (`src/vita/work_queue.c`), and every load waits for it.

### Save icon

Every save `src/app/game_save.c` queues (the Autosave slot, the Manual slot and each chapter's start) shows a soul in the bottom-right corner (`src/app/save_icon.c`). Settings and Record writes do not. The DS code has no indicator to follow: its save write is `func_020090a4` (`src/base/pick_result_a4.c`), and no caller that draws while it runs has been found.

- The flame is the control soul's, built by the same `soul_body_blobs` in `src/app/soul_chain.c`, at one third of its size and 24 px from the right and bottom edges.
- It is redrawn 8 times a second on real time, with each disc nudged up to 0.6 px per drawing and a flat core in place of the glowing orb, so it reads as drawn by hand next to the control soul's smooth motion.
- It fades in over 0.25 s, holds for 1.5 s or until the write thread is idle, whichever is longer, and fades out over 0.5 s. A save during that time restarts the hold.
- It draws over the game, dialogue, the loading screen and the pause menu, and not on the title screen. There is no Options switch.

The pause menu's Chapters grid fills the chapter being played in dark red, in a grey ring or, under the cursor, a red one, and opens with the cursor on it. Picking it asks "Start from the beginning?"; picking any other reached chapter asks to play that chapter from its start. From the title screen no chapter is being played, so none is marked.

## Debug controls

While a script chain loads, a loading screen shows the pause backdrop with "Now Loading" and a red bar in the pause menu's floor (`src/app/loading_screen.c`): reading the tables and scripts fills the first fifth, animations the next three tenths and textures the last half. The tables and scripts are read from one file, `scripts/scripts.pack` (`docs/asset_format.md`), and parsed on a background thread on core 0 (`src/vita/background_task.c`) while the frame keeps drawing. On the Vita the whole game's chain loads in about 3.5 s. Sissel's cat walks along the floor's top edge from off the left side to off the right side and starts again on the left until the chain has loaded. It uses the remaster's walk cycle (`st01_neko_007`), baked with the pause cat's frames but left unpinned (`walk` in game.json's `cat_animations`), so each step's forward motion stays in the art. The converter writes the cycle's stride into `pause/cat_track.txt`, and the cat moves on by one stride per cycle at 40 ticks a second, two thirds of the game's pace. The drawings carry the cat forward unevenly, so the converter also writes a shift for each tick that evens them out, and the cat's body glides at a constant speed (root cause 200). A frame that runs long while a file loads moves the cat by one drawing at most, so it slows down rather than leaping. Behind the cat, a faint glow in the pause circle's three reds rises 256 px from the floor, at 35% opacity at its foot, to lift the black cat off the dark backdrop. Behind it, every renderer primitive is drawn once, clear, so vitaGL compiles its shaders before play; the title screen does the same while its black cover is still fully up, so the compiles land on frames nobody sees. With the debug controls on, the debug HUD shows instead.

In the debug build, SELECT outside a chapter turns the debug controls on and shows the HUD. While they are on, LEFT, RIGHT, UP and DOWN browse backgrounds and animations, CROSS pauses, TRIANGLE toggles the HUD, R opens the animation list and L restarts the opening. SELECT turns them off again, which resumes a paused scene and restarts the opening if you browsed away from it.

They stay off during normal play, so that no button can stall the scene. The log records every button press, pause and skipped hold, as `input pressed buttons=...`, `scene paused` and `scene hold_skipped`.

## The script VM

`script_machine.c` and the `script_*` files beside it run the DS's own script bytes, one tick per 16,666 microseconds. The core has no SDK calls and no clock, and everything it tells the rest of the app goes through the `void` functions in `script_outputs.h`. On the Vita those are `script_bridge.c`, which drives the stage, effects, text, overlays and audio. In `tests/golden/script_replay.c` they print. `script_player.c` reads the files before tick zero, one per frame, and owns the clock.

Nothing halts the game. An op, table word, effect id or GameApp pair the VM does not know writes one note to the log and the script carries on.

`script_soul.c` is the player's side: soul view, core selection, tricks and the game events that only native code sends. Pressing the trick button sends `core|ready` at once and `core|action` 8 ticks later, as the DS does. A dead person's core sends `timeleap|start` instead.

In the debug build's commands file, `play_script route` starts the opening, from the title screen too. X replays it when it finishes.

`make -C srcVita golden` compares the VM's fetches with the saved DS trace, on a machine that has one in `extracted/trace/vm_golden/`. It exits 2 with a message otherwise.

## Known issues

These are the known differences from the original. Bug-fix releases work through them.

**Chapter 1 differences**, found against the reference footage

- Thought balloons open differently. In the footage (1309.6 s, the "×××" balloon) a small dot rises from the speaker and grows into the balloon with "···" in it for about a fifth of a second before the words show, and a dot stays by its tail; the port wobbles a blank ellipse for about 12 ticks, then shows the words.
- Chapter 1's cores draw no label. The port keeps one label per object, listed by hand in `game.json` for stages 6 and 14 only. Native takes each core's name and action from its info row (`data_0209c448`, the remaster's `iCoreInfo` rows), which the port already tracks per core for the Trick press. In the footage the label rises from the core and runs along a line, name above and action below, to the left for a core right of centre ("Soul", 1087 s) and to the right otherwise. While the soul stays in a core the footage shows the ring and label for 120 frames in every 260 (621 to 641 s), also during talks; the port shows them once for 165 ticks and drops them while a talk is up.
- The pill words ("Ghost", "Trick", "Back") are drawn in the talk font with a hairline outline. The footage's are a bold serif face with a dark outline, at the same size; which remaster font or texture draws them is not found.
- The film still after "(Unconscious, eh?)" (footage 795 s, port tick 40220) turns from grey to full colour after about 30 ticks; the footage keeps it grey, with only Lynne in colour.
- The soul's fly-in from the trick pill.
- When the forward time-jump movie (`time_jump_b`, `pGameMainMovieForward`) ends on white, the port draws 16 black ticks (73050 to 73065, the HUD over black, then plain black) while the next scene loads, then fades that scene in from white. The footage (1409.5 s) stays white across the load. `draw_movie` drops a finished movie and nothing holds its last frame; what native shows between the movie's end and the next fade is not traced.

**Effects**

- Effects 8 (`efc_tsunagu`) and 9 (`efc_chain`) have no art in the remaster: none of its 144 effect lists has a row for their kinds (0xd8 and 0xcf), so they are not drawn.
- Effects 5, 6 (the ripple), 42, 43, 61, 62 and 207 (the dog's bark) have no art in the remaster either and are not drawn.
- These class effects have no port and are not drawn: 50, 54, 56, 57, 59, 64, 65, 99 to 102, 118, 119, 146 to 151, 165 to 168, 191 to 193, 227 to 229, 242, 243 and 270.

**Not in the port**

- **Touch.** The back touch pad is not read. The front screen works alongside the buttons: tap pills, balloons, Rewind Time and talk, drag the soul in Ghost view, and drag the camera where the game allows it.
The VM's timing comes from the script, except narration holds, which follow `chain.txt`'s timing profile, and the card's ramp and the close-up's dissolve, which are named constants in `script_bridge.c`.

The converter was built and checked with the Android files only. The Steam copy has not been compared.
