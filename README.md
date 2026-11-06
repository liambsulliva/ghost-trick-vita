# Ghost Trick Vita

A PS Vita port using the original DS game logic with the 2023 remaster's art and a single-screen interface.

The first target is **a playable Chapter 1 build for on-device feedback**. The remaining game is a stretch goal after that test. All prepared assets, logic and research are retained for later use. See the [Chapter 1 plan](docs/chapter1.md).

The current work is an autonomous function-matching campaign across the full prepared code inventory, with local checkpoint commits. Chapter 1 remains the first playable release target.

## Legal disclaimer

This is an unofficial fan project, not affiliated with or endorsed by Capcom, Nintendo or Sony. *Ghost Trick* belongs to Capcom, and all trademarks belong to their owners.

This repository contains only original work. It contains **no ROM files and no extracted Nintendo or Capcom assets**: no game art, music, sound, text, fonts or script data. Two things in it are worth knowing about:

- `src/` holds C/C++ reconstructed from a study of the DS game's compiled code, along with the symbol and layout data in `config/`. It is not the game's original source code.
- `srcVita/src/third_party/stb_vorbis.c` is an open-source library by its own author, under its own license.

**You must source these files yourself**, from your own legally obtained copy of the game. This project does not provide them or say where to get them. Please don't post game files, or links to them, in this repository.

## Decompilation progress

[![Ghost Trick function matching progress](docs/progress-treemap.svg)](https://liambsulliva.github.io/ghost-trick-vita/)

Each rectangle is one function, sized by its byte span and grouped by module. Green means reconstructed C/C++ passes the compiler and linked-function byte check; gray means unmatched. Full-game C link integration and Vita execution are separate milestones.

Click the preview to open the [interactive treemap](https://liambsulliva.github.io/ghost-trick-vita/), then click a green rectangle to open its verified C/C++ source on GitHub. Gray rectangles have no verified source yet. The README image itself is static; the viewer provides the function links. See [the viewer setup](docs/progress-viewer.md).

This shows the **full prepared code inventory**, not the number of functions required for Chapter 1. The latter still needs runtime dependency tracing. The denominator comes from ds-decomp's current symbol configuration; the older Ghidra export has a different function census. Shared code ranges count once in byte totals. DS Protect is excluded from the Vita target.

All ten DS binary modules already rebuild byte-identically from assembly. That baseline, Ghidra drafts, compiling candidates and asset preparation do not count as C/C++ matches.

Regenerate the chart whenever symbols or verified progress change:

```
python3 -I tools/progress_treemap.py
python3 -I tools/progress_treemap.py --check
```

Verified functions are recorded in [config/decomp_progress.json](config/decomp_progress.json), with module, address, size, source path, source SHA-256 and verification-record SHA-256. Add a function only after checking its address, size, instruction mode and every linked body byte against the reference. A function match alone does not establish recovery of its external data, use by the full-game build, or correct operation on Vita. The generator checks source identity and inventory consistency; it does not rerun the private ROM/compiler verification. Keep generated binaries and detailed ROM-derived evidence under ignored `extracted/`.

## Run it on a Vita

The VPK holds only the app. It has no game data in it, so it also needs art, sound and text converted from your own copy of the remaster's Android release. The app plays Chapter 1 from the DS game's own scripts, with its music, sounds, narration and dialogue. [The app's README](srcVita/README.md#controls) lists the buttons.

You need:

- Python 3, `make`, `clang`, `unzip` and ImageMagick 7 (the `magick` command). The converter draws the game's text with `magick`, which needs FreeType support: `magick -list format | grep OTF` should print a line.
- About 3.1 GB of free disk space for the game files.
- VitaSDK, to build the VPK. [The app's README](srcVita/README.md#build-and-test) lists its libraries.
- A Vita that can install homebrew with VitaShell.
- The Vita's shader compiler at `ur0:data/libshacccg.suprx`. It does not come with the VPK. Extract it once with ShaRKF00D, if another app has not already made you do this.

The converter steps have been run on macOS only. This project's own testing installs with Vita Companion, so the VitaShell install and FTP copy below are the usual routes, but they have not been tried with this build.

Run every command below from the top of this repository.

### 1. Copy the game files out of the Android package

The converter reads three files from the package's `assets/` folder. Put them in `extracted/remaster_apk/`, and keep these exact names:

| File | Size |
|---|---|
| `re_chunk_000.pak` | about 1.0 GB |
| `data01` | about 1.3 GB |
| `data02` | about 0.8 GB |

An APK is a zip file, so `unzip` can copy them out. Replace the path with your own copy:

```bash
mkdir -p extracted/remaster_apk
```

```bash
unzip -j "path/to/your-copy.apk" assets/re_chunk_000.pak assets/data01 assets/data02 -d extracted/remaster_apk
```

Leave `data01` and `data02` as they are. They are zip files too, and the converter reads the packs inside them.

These steps need one APK with all three files in its `assets/` folder. They were only tested with Android v1.0.0 packaged that way. A copy installed on a phone may keep its packs separately, and the converter cannot read that layout yet, so those copies will not work. The Steam copy has not been tried.

### 2. Find the text key

The narration and speech bubbles are stored encrypted in the packs. Their 16-byte key is not in this repository, so you get it from your own copy of the package. The converter needs this key and stops without it, so do this step before converting. Replace the path with your own copy:

```bash
python3 -I srcVita/tools/assets/extract_text_key.py "path/to/your-copy.apk"
```

This takes a few seconds, saves the key as `extracted/remaster_apk/text_key.bin` and never prints it.

Everything under `extracted/` is ignored by git. `tools/rom_audit.py` refuses a file named `text_key.bin` and, when your key file is in place, any file git would track that holds the key as raw bytes, hex or decimal text. It cannot spot every encoding or a piece shorter than 8 bytes, and it reads working and staged copies but not commit history, so never paste the key or part of it anywhere.

If the tool says the package has no library, found no key, or found several, your copy may be a different build of the game.

### 3. Convert the art, sound and text

```bash
make -C srcVita/tools/assets
```

```bash
python3 -I srcVita/tools/assets/convert_assets.py --set chapter1_opening --output extracted/vita_assets/chapter1_opening
```

The first run takes a few seconds and writes about 17.5 MB to `extracted/vita_assets/chapter1_opening/`. A later run bakes only what changed, so it takes under a second if nothing did. Everything under `extracted/` is ignored by git. Keep it there, because it is game data and this repository is public.

**Optional: scripts from your own DS cartridge.** With these, the app plays the opening from the DS game's own scripts instead of the hand-written scene. If you have a dump of your own USA cartridge of the DS game (game code `BGTE`), the converter also bakes its scene scripts into `scripts/`. Put the file at `extracted/ds_rom/game.nds`, or name it with `--ds-rom`. Without one, the converter prints one warning and everything else still converts. A cartridge from another region is refused, because no other region's scripts have been checked yet. The converter then stops without converting anything, art included, so remove that file or name another one with `--ds-rom`.

If the converter stops with `FileNotFoundError`, `BadZipFile` or `KeyError`, one of the three files is missing or came from a different build of the game. Go back to step 1 and check the names and sizes. If it stops with `no text key`, or says the saved text key did not decode a file, go back to step 2. If it prints `failed:` lines, each names a file it could not make. The rest still bake, and the last good scene and manifest stay.

### 4. Put everything on the Vita

Build the VPK with [the commands in the app's README](srcVita/README.md#build-and-test). This writes `srcVita/build/ghosttrick.vpk`.

Then copy these across:

| On this computer | On the Vita |
|---|---|
| `srcVita/build/ghosttrick.vpk` | Anywhere under `ux0:`, then install it with VitaShell |
| `extracted/vita_assets/chapter1_opening/manifest.txt` | `ux0:data/ghosttrick/assets/manifest.txt` |
| `extracted/vita_assets/chapter1_opening/textures/` | `ux0:data/ghosttrick/assets/textures/` |
| `extracted/vita_assets/chapter1_opening/animations/` | `ux0:data/ghosttrick/assets/animations/` |
| `extracted/vita_assets/chapter1_opening/scenes/` | `ux0:data/ghosttrick/assets/scenes/` |
| `extracted/vita_assets/chapter1_opening/music/` | `ux0:data/ghosttrick/assets/music/` |
| `extracted/vita_assets/chapter1_opening/sounds/` | `ux0:data/ghosttrick/assets/sounds/` |

The art goes in the Vita's data folder, not inside the VPK or the app's folder. Copy what is inside `chapter1_opening`, not the folder itself. A path like `ux0:data/ghosttrick/assets/chapter1_opening/manifest.txt` will not be found. Leave `.bake_record.json` behind. Only the converter uses it.

You can copy by hand or with a script:

- **By hand:** put the Vita and this computer on the same Wi-Fi network. In VitaShell, press SELECT to start its FTP server. If SELECT starts USB instead, press START and set **SELECT button** to **FTP**. Connect an FTP client to the address and port the Vita shows, and keep VitaShell open until the copy finishes. In the client, open `ux0:` and then `data`, create a folder `ghosttrick`, and inside it a folder `assets`. Copy the six items into `assets`, and the VPK into `ux0:`. Then select the VPK in VitaShell, press X and confirm. A **Ghost Trick Vita** bubble appears on the home screen.
- **With a script:** with Vita Companion running on the Vita, this uploads the converted files and checks each uploaded file by hash. Replace the address with your Vita's. It does not install the VPK.

```bash
VITA_HOST=192.168.1.50 python3 -I srcVita/tools/vita_loop.py upload-assets extracted/vita_assets/chapter1_opening
```

### Check that it worked

Launch the app. If it was already open, quit it with START first, because it reads the files only when it starts. The screen stays black for about 5 seconds, a `loading` bar shows at the top, and the chapter card appears about 8 seconds after launch.

If you see something else:

| What you see | What is wrong | Look in the log for |
|---|---|---|
| Black or a crash before anything else appears | `ur0:data/libshacccg.suprx` is probably missing | |
| A checker pattern and `no assets in ux0:data/ghosttrick/assets` | `manifest.txt` is missing, misplaced or out of date | `manifest missing`, `manifest invalid` |
| A black screen that never changes | `scenes/` is missing or out of date | `scene missing`, `scene invalid` |
| Magenta blocks | `textures/` is missing or out of date | `texture missing` |
| Rooms with nothing in them | `animations/` is missing or out of date | `animation missing`, `animation invalid` |

The log is `ux0:data/ghosttrick/run.log`. It keeps earlier launches, so read below the last `--- launch` line.

After updating this repository, repeat steps 3 and 4. The file formats can change, and older files do not always produce an error: some just play wrongly. Step 2 does not need repeating unless your copy of the game changes.

## Working on the port

- [Handoff and local setup](HANDOFF.md)
- [The Vita app: build, test, run on the device](srcVita/README.md)
- [Asset inventory and build baseline](docs/inventory.md)
- [DS-to-remaster animation mapping](docs/frame_mapping.md)
- [Decompilation workflow references](docs/decomp_workflow.md)

The decompilation workflow draws on [ds-decomp](https://github.com/AetiasHax/ds-decomp) and the compiler-driven, AI-assisted process in [sm64ds-decomp](https://github.com/tangosdev/sm64ds-decomp). Their game-specific compiler pins and ARM-only matching assumptions are not applied blindly to Ghost Trick's mostly Thumb code.

Build inputs come from the user's own copies. ROMs, extracted assets, proprietary tools and reference footage stay local and gitignored. The current remaster format research has not yet been checked against a Steam copy. The Vita app in [srcVita](srcVita/README.md) installs and runs on real hardware, where it plays the opening of Chapter 1 at 60 fps from locally converted remaster art. The DS scripts now run the whole chapter, with player input connected. Most effects are not drawn yet.
