import argparse
import os
import shutil
import subprocess
import sys
import zipfile
from functools import partial

TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(TOOLS_FOLDER)
sys.path.insert(0, os.path.join(TOOLS_FOLDER, "assets"))
sys.path.insert(0, TOOLS_FOLDER)

import output_folders
import smoke_test

WORK_FOLDER = os.path.join(SRC_VITA_FOLDER, "build", "smoke_mutations")
TREE_FOLDER = os.path.join(WORK_FOLDER, "tree")
TREE_PARTS = ("Makefile", "src", "include", "tests")
HOST_IN_TREE = os.path.join("build", "smoke_host", "smoke_host")
FLOW = "flow %s@remastered"
STORY_PARTS = "story_part_*"
FLOWS_BROKEN_BY_NO_MUTATION = []

SOURCE_MUTATIONS = [
    ("the texture folders use another bucket rule than the converter's", "src/app/texture_cache.c", "(unsigned int)strlen(file_name)) % bucket_count;", "(unsigned int)strlen(file_name)) % (bucket_count + 1u);", ["boot_title"]),
    ("the manifest is never found", "src/app/viewer_app.c", 'file_path_join(path, assets_folder, "manifest", ".txt")', 'file_path_join(path, assets_folder, "manifes", ".txt")', ["boot_title"]),
    ("CROSS does nothing in the game", "src/app/script_input.c", "    if (pressed & INPUT_CROSS) {\n        script_machine_note_press(machine);\n    }", "", ["opening_normal_speed", "action_trick", "action_touch", "action_swap"]),
    ("the stick does not move the soul", "src/app/script_input.c", "    script_soul_set_held(machine, stick_aim_axis_speed(frame->stick_x), stick_aim_axis_speed(frame->stick_y));", "    script_soul_set_held(machine, 0.0f, 0.0f);", ["action_move"]),
    ("a held direction does not pan the camera", "src/app/script_input.c", "        script_soul_set_held(machine, (float)held_x, (float)held_y);", "        script_soul_set_held(machine, 0.0f, 0.0f);", ["action_camera"]),
    ("CROSS does not pick a choice", "src/app/script_soul.c", "        script_choice_pick(machine, machine->choice.cursor);", "        (void)machine;", ["action_choose"]),
    ("L does not open the ghost view", "src/app/script_input.c", "    if (pressed & INPUT_L) {\n        script_soul_note_ghost(machine);\n    }", "", ["action_ghost_on"]),
    ("CIRCLE does not close the ghost view", "src/app/script_input.c", "        } else if (machine->soul.is_on) {\n            script_soul_note_ghost(machine);\n        }", "        }", ["action_ghost_off"]),
    ("SQUARE does not open a balloon", "src/app/script_input.c", "        script_soul_note_tap(machine, SCRIPT_TAP_GROUP_RIGHT);", "", ["action_tap_balloon"]),
    ("TRIANGLE does not switch the character", "src/app/script_input.c", "        script_soul_note_tap(machine, SCRIPT_TAP_GROUP_LEFT);\n        script_soul_note_character_button(machine);", "        script_soul_note_tap(machine, SCRIPT_TAP_GROUP_LEFT);", ["action_character"]),
    ("CROSS does not take a phone line", "src/app/script_input.c", "        script_phone_book_go_there(machine);", "        (void)machine;", ["action_phone_call"]),
    ("CIRCLE does not leave the phone book", "src/app/script_input.c", "        script_phone_book_stay_here(machine);", "        (void)machine;", ["action_phone_stay"]),
    ("the game-over retry does nothing", "src/app/script_soul.c", '    retry_the_fate_attempt(machine, "game_over_retry");', "    (void)machine;", ["retry_after_failure"]),
    ("START does not open the pause menu", "src/app/viewer_app.c", "((input->pressed & INPUT_START) || start_is_tapped)", "(start_is_tapped)", ["pause_and_resume"]),
    ("a tap on the pause button does nothing", "src/app/viewer_app.c", "return lands && script_player_is_active() && script_ui_view_start_button_is_at(input->touch_x, input->touch_y);", "return lands && !script_player_is_active() && script_ui_view_start_button_is_at(input->touch_x, input->touch_y);", ["touch_pause_button"]),
    ("Save writes the wrong slot", "src/app/game_save.c", "file_write_whole(game_save_path(slot), (const unsigned char *)text, size)", "file_write_whole(game_save_path(GAME_SAVE_AUTO), (const unsigned char *)text, size)", ["pause_save_then_continue"]),
    ("the pause menu's Title never leaves the game", "src/app/pause_menu.c", "        title_is_wanted = 1;", "        title_is_wanted = 0;", ["pause_to_title"]),
    ("a chapter picked in the pause menu does not play", "src/app/pause_chapters.c", "            return PAUSE_CHAPTERS_PLAYS;", "            return PAUSE_CHAPTERS_STAYS;", ["pause_chapter_replay", "title_chapter_select"]),
    ("L and R do not turn the Record's tabs", "src/app/pause_people.c", "    if (pressed & INPUT_L) {", "    if (pressed & 0) {", ["record_people"]),
    ("New Game starts over a save without asking", "src/app/title_menu.c", '            ask(JOB_NEW_GAME, "row.new_game_ask", NULL);', "            leave_with(TITLE_REQUEST_NEW_GAME, 0);", ["title_new_game_over_save"]),
    ("LEFT and RIGHT do not change a level in Options", "src/app/pause_options.c", "set_level(rows[selected].job, *level + ((pressed & INPUT_RIGHT) ? 1 : -1));", "set_level(rows[selected].job, *level);", ["options_every_row"]),
    ("the credits cannot be left", "src/app/staff_credits.c", '        finish(credits, "left");', "        (void)credits;", ["options_credits"]),
    ("erasing keeps the automatic save", "src/app/game_save.c", "    erased = file_delete(GAME_SAVE_AUTO_PATH) == FILE_OK;", "    erased = 1;", ["title_erase_saves"]),
    ("settings are never written", "src/app/game_settings.c", "result = size > 0 && file_write_whole(settings_path_to_write, (const unsigned char *)text, size) == FILE_OK;", "result = size > 0;", ["options_setting_persists", "title_language"]),
    ("the Visual Filter change never starts the other set", "src/main.c", "    viewer_app_start(asset_sets_choose(ASSET_SETS_REMASTERED_FOLDER, ASSET_SETS_DS_FOLDER));\n    use_the_memory_this_device_has();\n    log_memory_and_textures();\n    run_log_write(\"art set switch done", "    viewer_app_start(ASSET_SETS_REMASTERED_FOLDER);\n    use_the_memory_this_device_has();\n    log_memory_and_textures();\n    run_log_write(\"art set switch done", ["visual_filter_switch@both"]),
    ("the hint is never offered", "src/app/script_hints.c", "hints->stuck_ticks >= hints->delay_ticks && the_bubble_can_show(machine)", "hints->stuck_ticks > hints->delay_ticks * 100 && the_bubble_can_show(machine)", ["hint_after_wait"]),
    ("a finger does not drag the soul", "src/app/script_input.c", "        script_soul_set_touch(machine, 1, world_x, world_y);", "        script_soul_set_touch(machine, 0, world_x, world_y);", ["touch_drag_soul"]),
    ("a dialogue choice never reports that it opened", "src/app/script_choice.c", '    mark(machine, "choice_opened", names);', "    (void)names;", ["choice_panels@both"]),
    ("a talk page never counts as open", "src/app/script_talk.c", "    machine->pause_bit = 1;\n    machine->talk_is_open = 1;", "    machine->pause_bit = 1;", ["talk_boxes@both"]),
    ("a language's own files are never used", "src/app/asset_language.c", "    if (language == ASSET_LANGUAGE_ENGLISH || !language_folder(", "    if (language >= ASSET_LANGUAGE_ENGLISH || !language_folder(", ["pictures language_ja@remastered differ from language_en", "pictures language_fr@remastered differ from language_en"]),
]

FULL_SOURCE_MUTATIONS = [
    ("CROSS does not take a phone line, which the story first needs past its opening", "src/app/script_input.c", "        script_phone_book_go_there(machine);", "        (void)machine;", [STORY_PARTS, "whole_game@ds"]),
    ("TRIANGLE does not open a balloon", "src/app/script_input.c", "        script_soul_note_tap(machine, SCRIPT_TAP_GROUP_LEFT);\n", "", [STORY_PARTS]),
    ("a talk with the dead never reaches its script", "src/app/script_soul.c", '"SYSTEM", "DEADTALK");', '"SYSTEM", "DEADTALC");', [STORY_PARTS]),
]


def names_caught_by(caught_by):
    return [name if " " in name else ("flow " + name if "@" in name else FLOW % name) for name in caught_by]


def flows_to_run(caught_by):
    flows = []
    for name in caught_by:
        if name.startswith("pictures "):
            flows += [name.split()[1].split("@")[0], name.split()[-1]]
        else:
            flows.append(name.split("@")[0])
    return flows


def copy_tree():
    shutil.rmtree(TREE_FOLDER, ignore_errors=True)
    os.makedirs(TREE_FOLDER)
    for part in TREE_PARTS:
        source = os.path.join(SRC_VITA_FOLDER, part)
        if os.path.isdir(source):
            shutil.copytree(source, os.path.join(TREE_FOLDER, part))
        else:
            shutil.copyfile(source, os.path.join(TREE_FOLDER, part))


def build_host():
    built = subprocess.run(["make", "-C", TREE_FOLDER, "-j8", HOST_IN_TREE], capture_output=True, text=True, errors="replace")
    return built.returncode == 0, (built.stdout + built.stderr).splitlines()[-12:]


def smoke_lines(extra_arguments, flows, sets, is_full):
    arguments = smoke_test.build_parser().parse_args(["--output", os.path.join(WORK_FOLDER, "out"), "--no-package", "--reuse-replay"] + ([] if is_full else ["--quick"]) + extra_arguments + [word for flow in flows for word in ("--only", flow)])
    arguments.set = sets
    lines = []
    smoke_test.run(arguments, lines.append)
    return lines


def failed_names(lines):
    return [line[len("FAIL  "):].split(": ")[0] for line in lines if line.startswith("FAIL  ")]


def is_caught(lines, wanted_names):
    import fnmatch
    failed = failed_names(lines)
    return any(fnmatch.fnmatchcase(name, wanted) for name in failed for wanted in wanted_names)


def run_source_mutation(mutation, is_full, write):
    breaks, relative_path, old, new, caught_by = mutation
    path = os.path.join(TREE_FOLDER, relative_path)
    with open(path, encoding="utf-8") as source:
        original = source.read()
    if original.count(old) != 1:
        write("INVALID  %s: %s holds its text %d times" % (breaks, relative_path, original.count(old)))
        return False
    try:
        with open(path, "w", encoding="utf-8") as output:
            output.write(original.replace(old, new))
        built, build_lines = build_host()
        if not built:
            write("INVALID  %s: the broken app does not build" % breaks)
            for line in build_lines:
                write("         " + line)
            return False
        sets = ["remastered", "ds"] if any("@both" in name or "@ds" in name for name in caught_by) else ["remastered"]
        lines = smoke_lines(["--host", os.path.join(TREE_FOLDER, HOST_IN_TREE)], flows_to_run(caught_by), sets, is_full)
    finally:
        with open(path, "w", encoding="utf-8") as output:
            output.write(original)
    wanted = names_caught_by(caught_by)
    caught = [name for name in wanted if is_caught(lines, [name])]
    missed = [name for name in wanted if name not in caught]
    if missed:
        write("SURVIVED %s: not caught by %s" % (breaks, ", ".join(missed)))
        return False
    write("caught   %s: %s" % (breaks, ", ".join(caught)))
    return True


class SetCopy:
    def __init__(self, real_folder, name):
        self.real = os.path.abspath(real_folder)
        self.root = os.path.join(WORK_FOLDER, "sets", name)
        shutil.rmtree(self.root, ignore_errors=True)
        self.folder = os.path.join(self.root, os.path.basename(self.real))
        os.makedirs(self.folder)
        for entry in os.listdir(self.real):
            os.symlink(os.path.join(self.real, entry), os.path.join(self.folder, entry))

    def own_folder(self, relative_path):
        path = self.folder
        real = self.real
        for part in relative_path.split("/"):
            path = os.path.join(path, part)
            real = os.path.join(real, part)
            if os.path.islink(path):
                os.unlink(path)
                os.makedirs(path)
                for entry in os.listdir(real):
                    os.symlink(os.path.join(real, entry), os.path.join(path, entry))
        return path

    def own_file(self, relative_path):
        folder = self.own_folder(os.path.dirname(relative_path)) if os.path.dirname(relative_path) else self.folder
        path = os.path.join(folder, os.path.basename(relative_path))
        real = os.path.realpath(path)
        os.unlink(path)
        shutil.copyfile(real, path)
        return path

    def remove(self, relative_path):
        folder = self.own_folder(os.path.dirname(relative_path)) if os.path.dirname(relative_path) else self.folder
        os.unlink(os.path.join(folder, os.path.basename(relative_path)))

    def rewrite(self, relative_path, change):
        path = self.own_file(relative_path)
        with open(path, encoding="utf-8") as source:
            text = source.read()
        changed = change(text)
        if changed == text:
            raise ValueError("%s was not changed" % relative_path)
        with open(path, "w", encoding="utf-8") as output:
            output.write(changed)


def first_background(copy):
    lines = smoke_test.read_lines(os.path.join(copy.folder, "manifest.txt"))
    return next(line.split(" ", 1)[1] for line in lines if line.startswith("background "))


def a_texture(copy):
    textures = os.path.join(copy.real, output_folders.TEXTURE_FOLDER)
    bucket = sorted(entry for entry in os.listdir(textures) if os.path.isdir(os.path.join(textures, entry)))[0]
    name = sorted(entry for entry in os.listdir(os.path.join(textures, bucket)) if output_folders.is_texture_file_name(entry))[0]
    return bucket, name


def a_loop_file(copy):
    for folder in ("music", "sounds"):
        names = sorted(name for name in os.listdir(os.path.join(copy.real, folder)) if name.endswith(".loop"))
        good = [name for name in names if smoke_test.loop_point_problem(os.path.join(copy.real, folder, name)) is None]
        if good:
            return folder + "/" + good[0]
    raise ValueError("the set has no loop file")


def break_art_line(copy):
    copy.rewrite("manifest.txt", lambda text: text.replace(smoke_test.SET_ART_LINES["remastered"] + "\n", "", 1))


def break_background(copy):
    copy.remove(output_folders.disk_path_of("textures/%s.gtex" % first_background(copy)))


def break_bucket_by_loose_file(copy):
    bucket, name = a_texture(copy)
    folder = copy.own_folder(output_folders.TEXTURE_FOLDER)
    shutil.copyfile(os.path.join(copy.real, output_folders.TEXTURE_FOLDER, bucket, name), os.path.join(folder, name))


def break_bucket_by_wrong_folder(copy):
    bucket, name = a_texture(copy)
    textures = os.path.join(copy.real, output_folders.TEXTURE_FOLDER)
    other = sorted(entry for entry in os.listdir(textures) if os.path.isdir(os.path.join(textures, entry)))[1]
    folder = copy.own_folder(output_folders.TEXTURE_FOLDER + "/" + other)
    shutil.copyfile(os.path.join(textures, bucket, name), os.path.join(folder, name))


def break_language_list(copy):
    copy.remove(output_folders.LANGUAGE_FOLDER + "/fr/names.txt")


def break_language_file(copy):
    names = smoke_test.read_lines(os.path.join(copy.real, output_folders.LANGUAGE_FOLDER, "ja", "names.txt"))
    copy.remove(output_folders.LANGUAGE_FOLDER + "/ja/" + output_folders.disk_path_of(names[0].strip()))


def break_loop_points(copy):
    copy.rewrite(a_loop_file(copy), lambda text: "%s 999999999\n" % text.split()[0])


def break_route_text(copy):
    copy.rewrite("scripts/route_steps.txt", lambda text: text.replace(" move ", " trick ", 1))


def break_route_for_replay(copy):
    copy.rewrite("scripts/route_steps.txt", lambda text: text.replace(next(line for line in text.splitlines() if " trick " in line) + "\n", "", 1))


def break_title_art(copy):
    copy.remove(output_folders.disk_path_of("textures/title_logo.gtex"))


SET_MUTATIONS = [
    ("the manifest does not name its art", break_art_line, lambda copy: smoke_test.check_manifest("remastered", copy.folder)),
    ("a background has no texture", break_background, lambda copy: smoke_test.check_manifest("remastered", copy.folder)),
    ("a texture sits outside the bucket folders", break_bucket_by_loose_file, lambda copy: smoke_test.check_texture_buckets("remastered", copy.folder)),
    ("a texture sits in another texture's bucket", break_bucket_by_wrong_folder, lambda copy: smoke_test.check_texture_buckets("remastered", copy.folder)),
    ("a language has no file list", break_language_list, lambda copy: smoke_test.check_languages("remastered", copy.folder)),
    ("a language lists a file it does not have", break_language_file, lambda copy: smoke_test.check_languages("remastered", copy.folder)),
    ("a sound loops past its end", break_loop_points, lambda copy: smoke_test.check_loop_points("remastered", copy.folder)),
    ("the set's route is not the repository's", break_route_text, lambda copy: smoke_test.check_route_is_current("remastered", copy.folder, os.path.join(TOOLS_FOLDER, "assets", smoke_test.SET_DESCRIPTIONS["remastered"]))),
    ("the route lacks a step the story needs", break_route_for_replay, lambda copy: smoke_test.run_replay("remastered", copy.folder, os.path.join(WORK_FOLDER, "states"), os.path.join(SRC_VITA_FOLDER, "build", "tests", "golden", "script_replay"))[0]),
]

SET_FLOW_MUTATIONS = [
    ("the title's logo is not in the set", break_title_art, ["boot_title"]),
]


def run_set_mutations(sets_folder, write):
    survived = 0
    real = os.path.join(sets_folder, smoke_test.SET_FOLDERS["remastered"])
    for number, (breaks, change, check) in enumerate(SET_MUTATIONS):
        whole = check(SetCopy(real, "whole"))
        copy = SetCopy(real, "set_%02d" % number)
        change(copy)
        result = check(copy)
        known = bool(smoke_test.gap_of(whole.name, smoke_test.read_known_gaps())[0])
        if result.passed or (not whole.passed and not known) or len(result.problems) <= len(whole.problems) and not whole.passed:
            survived += 1
            write("SURVIVED %s: %s still passes" % (breaks, result.name) if result.passed else "SURVIVED %s: %s fails the same way on the set as it lies" % (breaks, result.name))
        else:
            write("caught   %s: %s: %s" % (breaks, result.name, result.problems[-1]))
    for number, (breaks, change, caught_by) in enumerate(SET_FLOW_MUTATIONS):
        copy = SetCopy(real, "flow_%02d" % number)
        change(copy)
        lines = smoke_lines(["--sets", copy.root], flows_to_run(caught_by), ["remastered"], False)
        if is_caught(lines, names_caught_by(caught_by)):
            write("caught   %s: %s" % (breaks, ", ".join(names_caught_by(caught_by))))
        else:
            survived += 1
            write("SURVIVED %s: not caught by %s" % (breaks, ", ".join(names_caught_by(caught_by))))
    return survived


def run_package_mutations(write):
    package = os.path.join(SRC_VITA_FOLDER, "build", "smoke", "package", "ghosttrick-debug.vpk")
    if smoke_test.package_problems(package):
        write("skip     the package checks: no good debug package from an earlier smoke run")
        return 0
    survived = 0
    folder = os.path.join(WORK_FOLDER, "package")
    shutil.rmtree(folder, ignore_errors=True)
    os.makedirs(folder)
    copy = os.path.join(folder, os.path.basename(package))
    for breaks, left_out, elf_text in (("the package lacks its app", "eboot.bin", None), ("the package lacks its picture", "sce_sys/icon0.png", None), ("the app in the package is the player's build", None, b"player")):
        with zipfile.ZipFile(package) as source, zipfile.ZipFile(copy, "w") as output:
            for name in source.namelist():
                if name != left_out:
                    output.writestr(name, source.read(name))
        elf = copy[:-len(".vpk")] + ".elf"
        if elf_text is None:
            shutil.copyfile(package[:-len(".vpk")] + ".elf", elf)
        else:
            with open(elf, "wb") as output:
                output.write(elf_text)
        problems = smoke_test.package_problems(copy)
        if problems:
            write("caught   %s: vita debug package: %s" % (breaks, problems[0]))
        else:
            survived += 1
            write("SURVIVED %s: vita debug package still passes" % breaks)
    return survived


def main():
    parser = argparse.ArgumentParser(description="Breaks the app and the converted set one thing at a time and checks that the smoke test's check for that thing fails. Run make smoke first: this takes its story, start states and package from that run, and works on copies.")
    parser.add_argument("--sets", default=os.path.join(SRC_VITA_FOLDER, "..", "extracted", "vita_assets"), help="folder holding the converted sets")
    parser.add_argument("--only", action="append", default=[], help="run only the source mutations whose description holds this text; may repeat")
    parser.add_argument("--full", action="store_true", help="also break what only the whole-story flows catch")
    parser.add_argument("--no-sets", action="store_true", help="leave out the set and package mutations")
    arguments = parser.parse_args()
    say = partial(print, flush=True)
    survived = 0
    if not arguments.no_sets and not arguments.only:
        survived += run_set_mutations(os.path.abspath(arguments.sets), say)
        survived += run_package_mutations(say)
    copy_tree()
    built, lines = build_host()
    if not built:
        say("the copy of the app does not build:\n" + "\n".join(lines))
        return 2
    mutations = [(mutation, False) for mutation in SOURCE_MUTATIONS] + ([(mutation, True) for mutation in FULL_SOURCE_MUTATIONS] if arguments.full else [])
    for mutation, is_full in mutations:
        if arguments.only and not any(text in mutation[0] for text in arguments.only):
            continue
        if not run_source_mutation(mutation, is_full, say):
            survived += 1
    say("smoke mutations: %d not caught" % survived)
    return 1 if survived else 0


if __name__ == "__main__":
    sys.exit(main())
