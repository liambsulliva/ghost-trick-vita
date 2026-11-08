# Writing an auto-play route

Chapter 1's route (steps 0 to 89 of `tools/assets/game.json`) is the model: it plays back almost exactly like the reference footage. Write every later chapter the way Chapter 1 is written. Step numbers below are Chapter 1 examples.

## What a step is

`{script, action, object, pace, footage_s, wait}`, plus `slot` (an object's second core is slot 1) or `place` (a phone ride's destination). The player runs a script's steps from that script's first step, in order, and stops at the first step that belongs to another script. Each script's steps are therefore one block, in footage order, and the blocks follow the order the scripts are visited.

## The rules Chapter 1 follows

1. **Gate each beat on the story, not on idleness.** The first step of every puzzle beat waits on the act that opens that beat: the game controller's act (`act:game:gameAct030 & flag:3 & playable`, step 2) or the act an object enters after the previous trick (`act:tire:tireAct010 & usable:tire & free`, step 28; `act:bed:bedAct000`, step 15). `free & no_talk` alone holds in short gaps before a cut scene starts and fires the step too early.
2. **Follow the footage's soul path core by core.** List every core the player passed through, not only the target: cardoor, hyousiki, then bed (steps 10 to 12); mannequin, then tire (24, 25). Each hop is a `move` waiting on `view & usable:<object>`.
3. **Use the tight waits for leaving ghost view and tricking.** `ghost_off` waits on `view & core:<object>` (26), `trick` on `core:<object> & playable` (27). A second core carries its slot in both the step and the wait: `move bicycle` with `"slot": 1` waits on `view & usable:bicycle:1` (54), then `core:bicycle:1` (55, 56).
4. **Re-enter ghost view on the act the trick leaves behind.** After a trick, the next `ghost_on` waits on the act the tricked object moves to, or on the flag its trick sets (`flag:8 & free`, step 39), so the soul waits for the trick's animation as the player did.
5. **Read `footage_s` from the video, to half a second.** It is the moment the player pressed, read from frames. The pacing bake (`route_pacing.py`) stretches each step's dwell so the port never acts sooner after the previous step than the footage did; a guessed time makes the port wait too long or not at all.
6. **Use `quick` for fast runs of tricks** (steps 43 to 71), `normal` elsewhere.
7. **Gate exits on the story too.** A phone ride waits on the controller's act that offers it (`act:game:gameAct060`, step 89), never on an object act that already holds when the scene opens. Balloons are tapped when they are up (`act:game:gameAct020 & balloon:cicel_talk`, step 8; `tap_balloon` names the event the balloon sends). Where several balloons send the same event under different kinds (Chapter 4's office: `sys` `cicel` `click` and `sys` `cicel_talk` `click`), name both as `kind:event`: `tap_balloon` object `cicel:click`, wait `balloon:cicel:click`.
8. **Tutorials, the camera and rewinds are steps too**: `touch` on `tutorial:touch`, `camera` on `camera_tutorial` (7), `touch` on `timeleap_armed` for the rewind (20), `dead_talk` (object `-`) for a talk with the dead, and `choose` on `choice` for a choice panel (object: the event the panel sends, its talk label or its caption label, or `cancel`). A choice with no `choose` step picks the first panel not yet chosen and logs `choice_picked_without_route`.

## Finding the acts and flags

Read the decoded script (`../extracted/lmg/<stage>/<script>_Expand.xml.lz.txt`): find the `op_024` that leaves the scene, the handler and act it sits in, and the flag or event that handler waits for; then who sets or sends it (another object's act, a balloon, `SV_CHGCORE` or `SV_END` on a core). That chain gives the beats and their gating acts. The replay's `act_begun` marks show when each act starts, and its `stuck_pushed_handler` lines list what a scene was still waiting for.

## Checking a chapter

Replay the whole game from Chapter 1 with the scene timeout off (`scene_most_ticks` removed from the route copy) and `--marks`. A chapter is done when every step is done with no `route_step_failed` or `route_hop_beyond_reach`, no `stuck_pushed_handler` appears between its steps, and each script hands over to the next as the footage shows. Then run it on the Vita and compare bursts with the footage shot by shot.

## Routing chapters side by side

Chapters are routed three at a time and linked afterwards. Each worker starts its replays at its own chapter instead of from Chapter 1: `script_replay --save-state-at <script> <file>` saves the chapter mark, phone place and flag banks when a whole-game replay first reaches the chapter's first script, and `--load-state <file> --start <script>` starts there. Where the saved flags differ from the story (earlier chapters not yet routed), the worker corrects them with `--set-flag` and `--clear-flag` and records the entry it settled on as one `entry:` line. Linking appends each chapter's blocks in story order and replays the whole game from Chapter 1: a chapter links when the flags the earlier chapters leave match its `entry:` line, and a difference names the step that should have set or cleared the flag.
