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

## READMEs
- The root `README.md` holds the pertinent, high-level details and links to the other two for the rest.
- `srcVita/README.md` holds granular detail about the port only.
- `src/README.md` holds granular detail about the decompilation effort.
- State each fact in one of the three, never twice.
