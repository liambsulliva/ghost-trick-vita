import io
import os
import tempfile
import unittest
import zipfile

import smoke_test

FLOW_TEXT = """
block enter
press cross
wait 2

block enter_twice
use enter
use enter

flow first
about Reach the title
guards 12 34
sets remastered both
start st01_010
shows shot
settings language=2 music=0
frames 900
log_has title open
log_lacks crash
known texture missing* | not baked yet
route_steps {steps}
launch
use enter_twice
both: press left
ds: press right
capture shot
launch relaunches
expect title

flow each_{language}
for language in languages
needs full
launch
press down
"""


def bmp_bytes(width, height, color):
    row = bytes(color) * width + b"\0" * ((4 - width * 3 % 4) % 4)
    header = b"BM" + (54 + len(row) * height).to_bytes(4, "little") + bytes(4) + (54).to_bytes(4, "little") + (40).to_bytes(4, "little") + width.to_bytes(4, "little") + height.to_bytes(4, "little") + (1).to_bytes(2, "little") + (24).to_bytes(2, "little") + bytes(24)
    return header + row * height


def story_of(marks, steps):
    story = smoke_test.Story(marks, steps)
    return story


class FlowFileTests(unittest.TestCase):
    def setUp(self):
        self.flows = smoke_test.parse_flows(FLOW_TEXT)

    def test_headers_are_read(self):
        flow = self.flows[0]
        self.assertEqual((flow.name, flow.about, flow.guards, flow.installs, flow.start_state, flow.frames_most), ("first", "Reach the title", "12 34", ["remastered", "both"], "st01_010", 900))
        self.assertEqual((flow.shows, flow.settings, flow.log_has, flow.log_lacks), (["shot"], {"language": "2", "music": "0"}, ["title open"], ["crash"]))
        self.assertEqual(flow.known_lines, [("texture missing*", "not baked yet")])

    def test_blocks_expand_through_other_blocks(self):
        self.assertEqual(self.flows[0].launches[0]["lines"][:4], ["press cross", "wait 2", "press cross", "wait 2"])

    def test_second_launch_expects_a_relaunch(self):
        self.assertEqual([launch["relaunches"] for launch in self.flows[0].launches], [False, True])

    def test_install_lines_reach_only_their_install(self):
        both = smoke_test.instance_of(self.flows[0], "both", {"steps": "1,2"})
        alone = smoke_test.instance_of(self.flows[0], "remastered", {"steps": "1,2"})
        self.assertIn("press 4", both.launches[0]["lines"])
        self.assertNotIn("press 4", alone.launches[0]["lines"])
        self.assertNotIn("press 8", both.launches[0]["lines"])

    def test_buttons_become_the_mask_the_app_reads(self):
        self.assertEqual(smoke_test.lines_for_install(["press cross", "press l r", "press start"], "remastered", {}), ["press 10", "press 300", "press 400"])

    def test_a_name_the_set_lacks_stops_the_flow(self):
        with self.assertRaises(KeyError):
            smoke_test.instance_of(self.flows[0], "remastered", {})

    def test_an_unknown_line_is_refused(self):
        with self.assertRaises(ValueError):
            smoke_test.parse_flows("flow a\nwibble 3\n")

    def test_a_block_written_later_is_refused(self):
        with self.assertRaises(ValueError):
            smoke_test.parse_flows("flow a\nlaunch\nuse later\n")

    def test_quick_leaves_out_full_flows_and_loops_expand(self):
        story = story_of([("script_begun", "s1")], [])
        quick = smoke_test.expand_flows(self.flows[1:], {"remastered": story}, True, [])
        full = smoke_test.expand_flows(self.flows[1:], {"remastered": story}, False, [])
        self.assertEqual(quick, [])
        self.assertEqual([flow.name for flow in full], ["each_" + code for code in smoke_test.language_codes()])

    def test_only_keeps_matching_names(self):
        story = story_of([("script_begun", "s1")], [])
        flows = smoke_test.expand_flows(self.flows[1:], {"remastered": story}, False, ["each_j*"])
        self.assertEqual([flow.name for flow in flows], ["each_ja"])

    def test_the_repository_flow_file_parses_and_names_no_flow_twice(self):
        with open(smoke_test.FLOWS_PATH, encoding="utf-8") as source:
            flows = smoke_test.parse_flows(source.read())
        names = [flow.name for flow in flows]
        self.assertEqual(len(names), len(set(names)))
        self.assertTrue(all(flow.launches and flow.about for flow in flows))
        self.assertTrue(all(install in smoke_test.INSTALLS for flow in flows for install in flow.installs))


class StoryTests(unittest.TestCase):
    def setUp(self):
        marks = [("script_begun", "a"), ("route_step_done", "0:move:x"), ("script_begun", "b"), ("route_step_done", "1:trick:x"), ("script_begun", "a"), ("route_step_done", "2:move:x"), ("script_begun", "c")]
        self.story = story_of(marks, [["step", "x", "move", "-"], ["step", "x", "trick", "-"], ["step", "x", "move", "-"]])
        self.story.use_states([("a", 1, 10), ("b", 1, 20), ("c", 2, 30)])

    def test_steps_between_scenes_stop_at_the_first_visit(self):
        self.assertEqual(self.story.steps_between("a", last_scene="b"), [0])
        self.assertEqual(self.story.steps_between("b", last_scene="c"), [1, 2])

    def test_first_step_of_each_action(self):
        self.assertEqual(self.story.first_step_of_each_action(), {"move": 0, "trick": 1})

    def test_parts_begin_at_the_first_scene_of_each_chapter(self):
        self.assertEqual(self.story.parts, [(1, "a"), (2, "c")])

    def test_story_parts_end_at_the_next_part(self):
        rows = smoke_test.loop_values("part in story_parts", self.story)
        self.assertEqual([(row["part"], row["scene"], row["stop_line"], row["steps"]) for row in rows], [("01", "a", "ff_until script c", "0,1,2"), ("02", "c", "", "")])

    def test_route_action_rows_start_at_the_scene_before_the_step(self):
        rows = smoke_test.loop_values("action in route_actions", self.story)
        self.assertEqual([(row["action"], row["scene"], row["steps"]) for row in rows], [("move", "a", "0"), ("trick", "b", "1")])


class LogTests(unittest.TestCase):
    RULES = [("fail", "texture missing", ""), ("known", "sound bad_file", "*name=old_*"), ("fail", "sound bad_file", ""), ("ok", "frame *", "")]

    def test_only_timed_lines_are_messages(self):
        self.assertEqual(smoke_test.log_messages(["[      1.250] frame ready", "--- launch", "[x] no"]), ["frame ready"])

    def test_kinds(self):
        self.assertEqual(smoke_test.kind_of("texture missing name=a"), "texture missing")
        self.assertEqual(smoke_test.kind_of("manifest=3 loaded"), "manifest loaded")

    def test_a_failing_kind_is_a_problem(self):
        problems = smoke_test.log_kind_problems(["texture missing name=a", "texture missing name=b", "frame ready"], self.RULES)
        self.assertEqual(len(problems), 1)
        self.assertIn("2 of 'texture missing'", problems[0])

    def test_an_unlisted_kind_is_a_problem(self):
        problems = smoke_test.log_kind_problems(["wobble begun count=3"], self.RULES)
        self.assertIn("does not list", problems[0])

    def test_a_known_line_passes_and_its_kind_still_fails_otherwise(self):
        self.assertEqual(smoke_test.log_kind_problems(["sound bad_file name=old_1"], self.RULES), [])
        self.assertEqual(len(smoke_test.log_kind_problems(["sound bad_file name=new_1"], self.RULES)), 1)

    def test_flow_known_lines_are_taken_out_and_counted(self):
        kept, found = smoke_test.take_known_lines(["a b", "texture missing name=q"], [("texture missing*", "why"), ("never*", "gone")])
        self.assertEqual(kept, ["a b"])
        self.assertEqual(found, [("texture missing*", "why", 1), ("never*", "gone", 0)])

    def test_every_rule_in_the_repository_has_a_known_class(self):
        rules = smoke_test.read_log_kinds()
        self.assertTrue(rules)
        self.assertEqual({rule[0] for rule in rules} - {"ok", "fail", "known"}, set())

    def test_smoke_failures_never_pass_as_log_kinds(self):
        self.assertTrue(smoke_test.log_kind_problems(["smoke fail name=a line=1 reason=x frames=1 text=y"], smoke_test.read_log_kinds()))


class RouteTests(unittest.TestCase):
    def messages(self, steps):
        return ["route_step_done name=%d:move:x" % step for step in steps]

    def test_the_replays_steps_pass(self):
        self.assertEqual(smoke_test.route_problems(self.messages([3, 4, 5]), "3,4,5"), [])

    def test_a_missing_step_is_named(self):
        self.assertIn("not finished: 5", smoke_test.route_problems(self.messages([3, 4]), "3,4,5")[0])

    def test_an_extra_step_is_named(self):
        self.assertIn("does not finish here: 6", smoke_test.route_problems(self.messages([3, 4, 5, 6]), "3,4,5")[0])

    def test_another_order_fails(self):
        self.assertIn("another order", smoke_test.route_problems(self.messages([4, 3]), "3,4")[0])


class PictureTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)

    def picture(self, name, width, height, color):
        path = os.path.join(self.folder.name, name)
        with open(path, "wb") as output:
            output.write(bmp_bytes(width, height, color))
        return path

    def test_a_missing_picture_fails(self):
        self.assertIn("not written", smoke_test.picture_problems(os.path.join(self.folder.name, "none.bmp"), False)[0])

    def test_another_size_fails(self):
        self.assertIn("not the screen's size", smoke_test.picture_problems(self.picture("a.bmp", 16, 16, (9, 9, 9)), False)[0])

    def test_a_black_picture_fails_only_where_something_must_show(self):
        path = self.picture("b.bmp", smoke_test.SCREEN_WIDTH, smoke_test.SCREEN_HEIGHT, (0, 0, 0))
        self.assertEqual(smoke_test.picture_problems(path, False), [])
        self.assertIn("almost all black", smoke_test.picture_problems(path, True)[0])

    def test_a_lit_picture_passes(self):
        path = self.picture("c.bmp", smoke_test.SCREEN_WIDTH, smoke_test.SCREEN_HEIGHT, (90, 40, 40))
        self.assertEqual(smoke_test.picture_problems(path, True), [])

    def test_pictures_differ_only_past_a_few_pixels(self):
        first = self.picture("d.bmp", 64, 64, (1, 2, 3))
        same = self.picture("e.bmp", 64, 64, (1, 2, 3))
        other = self.picture("f.bmp", 64, 64, (200, 2, 3))
        self.assertFalse(smoke_test.pictures_differ(first, same))
        self.assertTrue(smoke_test.pictures_differ(first, other))


class PackageTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.package = os.path.join(self.folder.name, "ghosttrick-debug.vpk")
        self.elf = os.path.join(self.folder.name, "ghosttrick-debug.elf")

    def write(self, names, elf_text=b"..smoke begin name=%s..", sfo=b"TITLE GHTR00001"):
        with zipfile.ZipFile(self.package, "w") as package:
            for name in names:
                package.writestr(name, sfo if name.endswith("param.sfo") else b"data")
        with open(self.elf, "wb") as output:
            output.write(elf_text)

    def test_a_whole_package_passes(self):
        self.write(smoke_test.PACKAGE_FILES)
        self.assertEqual(smoke_test.package_problems(self.package), [])

    def test_a_missing_file_is_named(self):
        self.write([name for name in smoke_test.PACKAGE_FILES if name != "sce_sys/icon0.png"])
        self.assertEqual(smoke_test.package_problems(self.package), ["the package lacks sce_sys/icon0.png"])

    def test_a_player_build_is_refused(self):
        self.write(smoke_test.PACKAGE_FILES, elf_text=b"no runner here")
        self.assertEqual(smoke_test.package_problems(self.package), ["the debug app does not hold the smoke runner"])

    def test_another_title_is_refused(self):
        self.write(smoke_test.PACKAGE_FILES, sfo=b"TITLE OTHER0000")
        self.assertEqual(smoke_test.package_problems(self.package), ["param.sfo lacks the title id"])

    def test_no_package_is_a_problem(self):
        self.assertIn("no package", smoke_test.package_problems(self.package)[0])


class ReportTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)

    def report(self, results, gaps):
        lines = []
        failed = smoke_test.report(results, gaps, self.folder.name, lines.append)
        return failed, lines

    def result(self, name, problem=None):
        result = smoke_test.Result(name, "77")
        if problem:
            result.fail(problem)
        return result

    def test_every_check_gets_a_line_and_failures_count(self):
        failed, lines = self.report([self.result("a"), self.result("b", "broke"), self.result("c")], {})
        self.assertEqual(failed, 1)
        self.assertEqual([line[:5].strip() for line in lines[:2]], ["ok", "FAIL"])
        self.assertIn("[root causes 77]", lines[1])
        self.assertTrue(any(line.startswith("ok    c") for line in lines))
        self.assertTrue(os.path.isfile(os.path.join(self.folder.name, "failures", "b.txt")))
        self.assertTrue(os.path.isfile(os.path.join(self.folder.name, "report.txt")))

    def test_a_known_gap_does_not_fail(self):
        failed, lines = self.report([self.result("flow x@ds", "no title")], {"flow x@*": "DS has no title art"})
        self.assertEqual(failed, 0)
        self.assertTrue(lines[0].startswith("known flow x@ds: DS has no title art"))

    def test_a_known_gap_that_passes_fails_so_the_list_cannot_rot(self):
        failed, lines = self.report([self.result("flow x@ds")], {"flow x@*": "DS has no title art"})
        self.assertEqual(failed, 1)
        self.assertIn("passes now", lines[0])


class SettingsTests(unittest.TestCase):
    def test_settings_keep_defaults_for_the_rest(self):
        text = smoke_test.settings_text({"language": "3"})
        self.assertIn("language 3\n", text)
        self.assertIn("master 10\n", text)

    def test_an_unknown_setting_is_refused(self):
        with self.assertRaises(ValueError):
            smoke_test.settings_text({"wobble": "1"})


if __name__ == "__main__":
    unittest.main()
