# Working on this repository

## Native source first
- `src/` is the DS decompilation (mostly `src/thumb/`; names are addresses) and the ground truth.
- Before changing port behaviour, find the matching native code and follow it. Footage measures only what native code leaves open.
- Name the native function in the change's notes.

## Vita port: fix the class, not the instance
- Ask which other animations, effects, sounds or text share the cause, in this chapter and later ones.
- Fix it where all of them pass through: converter, baked tables, loading step or shared engine code. Never special-case one asset by name.
- Sweep the whole chapter for other instances, then leave a check that catches the class, and commit that test with the fix.
- Record the lesson and its guard in `srcVita/docs/root_causes.md`, so later chapters inherit it.

## Running on the Mac
- Game data, the baked sets, VitaSDK, the DS toolchain and the Vita itself are on Liam's Mac only. Run these with `~/.dotagents/bin/mac-run` from this checkout, never on the NAS: asset bakes (`convert_assets.py`, `set_up_game.py`), `make -C srcVita` builds and tests, `tools/smoke_test.py`, the mutation run, `pause_render.py` and `vita_loop.py`.
- Example: `~/.dotagents/bin/mac-run make -C srcVita test`. A bake writes into the Mac's real `extracted/`, shared with every other run.
- The run folder has no `.git`. For a package that will be installed, pass `BUILD_ID="$MAC_RUN_REV-$(date +%Y%m%d%H%M%S)"` to make.
- The Vita MCP tools run on the Mac. `vitacompanion`'s build uses the mac-run folder, so run `~/.dotagents/bin/mac-run true` before `vita_build` to send the current files.
- Bring results back with `--get`, for example `--get srcVita/build/<file>` (paths are relative to the repo root). Leave baked assets and packages on the Mac.

## READMEs
- The root `README.md` holds the pertinent, high-level details and links to the other two for the rest.
- `srcVita/README.md` holds granular detail about the port only.
- `src/README.md` holds granular detail about the decompilation effort.
- State each fact in one of the three, never twice.

## Commits
- Commit messages carry no co-author trailers and no tool footers; the shared `commit-msg` hook strips them.
