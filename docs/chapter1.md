# Chapter 1 Vita target

The immediate goal is a playable first chapter on the user's Vita, followed by their feedback on controls, readability, animation and performance. Porting the remaining chapters is a stretch goal, conditional on that result.

This narrows delivery and new work. Keep all prepared assets, logic, extracted data, tools and research for later reuse. Shared engine work remains useful to the full game. Do not delete later content, undo existing preparation, or spend time trimming working tools just to enforce the smaller scope.

## What the first build includes

- A small launch flow that starts Chapter 1 from the beginning, with the required opening sequence.
- Original DS scene logic for that chapter, including dialogue progression, possession, tricks, timing, failure and retry behavior.
- Remaster art and a single-screen interface adapted for Vita controls. Only the UI and effects encountered during Chapter 1 need implementation.
- The chapter's required backgrounds, characters, default poses, animations, sound and video, loaded from the user's locally converted data.
- A clear stop at Chapter 1 completion, with restart or return to the test menu. The test build must not silently continue into an unsupported chapter.
- Basic pause, error logging and the chapter's required checkpoint/retry behavior. Preserve the DS save path where used; a full chapter-select or cross-chapter save interface can wait.

Use English for the first test build unless the user requests another language. Keep the other language data available. Extra menus, galleries, later chapters, complete localization and full-game animation coverage are deferred. General fixes already completed remain in place.

## Completion and feedback

The chapter is ready for feedback when the user can install the build, start it without developer tools and play from its opening to the chapter boundary using Vita controls. Required text must be readable, puzzle interactions must work, failure must lead to a working retry, and transitions must not crash or lose required state.

Report observed frame time, free memory, input problems and visible timing or placement issues on the device. Target smooth 60 Hz presentation; measure the practical asset and memory budgets with the first viewer. A frame viewer is an intermediate check, not completion of the playable-chapter goal. Broader port work starts only after the user has tested this build and wants to proceed.

## Boundary and prepared data

The first saved runtime trace enters `st01/st01_game000_Expand.xml.lz` through `st01_root`. Do not equate the whole `st01` folder with Chapter 1. It also has later scenes, including `game020`, `game021` and `game022`, and a transition to `st02_root`. Their presence in an inventory does not make them first-chapter requirements. Confirm the actual completion transition with the DS oracle and the opening chapter of the reference video before fixing the build's end gate.

The wider stage inventory is already available:

```
python3 -I tools/map_motions.py --stage st01
```

It writes to `extracted/motion_map/st01/`, preserving the full-game reports. The initial result is 82 decoded files and 1,454 motion tables. All 2,298 motion calls resolve, with no zero targets or zero jumps. There are 366 stage-folder remaster animation stems: 352 naming matches and 14 gaps. Six gaps have stricter default-pose candidates, one more has a broader candidate, and seven have no stage-local candidate. These are inventory counts; trace actual use before treating a missing name as a Chapter 1 blocker.

The corrected CPAC extraction covers all 247 loads in the saved startup/stage 1 trace. The resource lookup reproduces the bicycle set and resolves every nonzero component reference from the decoded motion tables. Existing decoded data and wider mapping results remain available for future chapters.

The reference video is downloaded and verified as AV1, 1280×720, 60 fps, 33,598.083333 seconds. FFmpeg decoded the opening 180 seconds without errors. It stays under ignored `extracted/reference/` and must not be reproduced in outputs.

## Work order

1. Establish the exact Chapter 1 entry, completion transition and required scene/resource set using the existing DS oracle. Record input and state transitions without copying dialogue. Prioritize the characters, backgrounds, poses and animations actually reached in that path.
2. Build a small Vita application and asset viewer. Locate/configure VitaSDK for this repository, choose the smallest useful converted asset set, and measure loading, rendering and memory on the device. The Vita is currently off; ask the user to turn it on when an installable build is ready for testing.
3. Bring up the DS logic behind Vita replacements for files, memory, timing, input, drawing and audio. The byte-identical DS build is the reference; its ARM instructions alone do not make DS hardware calls work on Vita. Preserve the existing baseline and omit DS Protect. Matching C for the whole game is not a prerequisite for this chapter test.
4. Connect Chapter 1 motion/resource hooks to remaster playback, implement its required UI/input and effects, and calibrate timing and placement against the reference. Recheck the needed remaster assets against the user's Steam copy before using them as the converter's source of truth.
5. Package the chapter build, verify the full path and retry/checkpoint behavior, then give the user installation steps and a short feedback checklist. Fix the issues they identify before expanding scope.

There is no Vita application or device-tested renderer in this repository yet. The Steam copy is not on this Mac, and VitaSDK has not been configured for this project. Existing asset research uses the Android repack only for format inspection; retain that work but do not treat it as authoritative Steam validation. These are concrete next dependencies, not reasons to resume full-game research.

The prior BFM phase milestones remain recorded in `docs/inventory.md`. The user has now prioritized this chapter test, so a full matching-decomp campaign and later-stage mapping investigations are deferred. Downloading proprietary Metrowerks tools still requires their explicit approval.
