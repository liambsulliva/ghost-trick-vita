import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import route_pacing


def route_with(footage_times, pace="normal"):
    return {
        "dwell_ticks": {"normal": 90, "quick": 45, "instant": 0},
        "steps": [{"action": "move", "object": "x%d" % index, "pace": pace, "footage_s": seconds} for index, seconds in enumerate(footage_times)],
    }


class StepTimesTest(unittest.TestCase):
    def test_uses_the_last_ready_before_the_action_began(self):
        lines = [
            "100\tmark route_step_ready 3:move:a:0\n",
            "120\tmark route_step_ready 3:move:a:0\n",
            "210\tmark route_step_began 3:move:a:0\n",
            "211\tmark route_step_done 3:move:a:0\n",
            "250\tmark route_step_ready 4:trick:a:0\n",
        ]
        ready, began = route_pacing.step_times(lines)
        self.assertEqual(ready, {3: 120})
        self.assertEqual(began, {3: 210})

    def test_ignores_other_cues(self):
        ready, began = route_pacing.step_times(["5\tballoon_open slot=0\n", "6\tmark core memo\n", "\n"])
        self.assertEqual((ready, began), ({}, {}))


class PacedDwellsTest(unittest.TestCase):
    def test_stretches_a_step_that_acts_sooner_than_the_footage(self):
        route = route_with([10.0, 15.0])
        dwells = route_pacing.paced_dwells(route, {1: 1100}, {0: 1000}, set(), {})
        self.assertEqual(dwells, {1: 200})

    def test_keeps_the_base_dwell_when_the_port_is_already_slower(self):
        route = route_with([10.0, 11.0])
        self.assertEqual(route_pacing.paced_dwells(route, {1: 1100}, {0: 1000}, set(), {}), {})

    def test_caps_the_stretch_and_reports_the_drift(self):
        route = route_with([10.0, 30.0])
        drift = {}
        dwells = route_pacing.paced_dwells(route, {1: 1100}, {0: 1000}, set(), drift)
        self.assertEqual(dwells, {1: 90 + route_pacing.MOST_EXTRA_TICKS})
        self.assertEqual(drift, {1: 1200 - 100 - 90 - route_pacing.MOST_EXTRA_TICKS})

    def test_skips_steps_whose_window_was_too_short(self):
        route = route_with([10.0, 15.0])
        self.assertEqual(route_pacing.paced_dwells(route, {1: 1100}, {0: 1000}, {1}, {}), {})

    def test_skips_steps_without_footage_times(self):
        route = route_with([10.0, 15.0])
        del route["steps"][0]["footage_s"]
        self.assertEqual(route_pacing.paced_dwells(route, {1: 1100}, {0: 1000}, set(), {}), {})

    def test_never_stretches_a_step_that_waits_for_a_moving_core(self):
        route = route_with([10.0, 15.0])
        route["steps"][1]["wait"] = "reach:tel & free"
        self.assertEqual(route_pacing.paced_dwells(route, {1: 1100}, {0: 1000}, set(), {}), {})

    def test_never_stretches_a_step_that_waits_on_an_act(self):
        route = route_with([10.0, 15.0])
        route["steps"][1]["wait"] = "act:madam:madam_act01 & free"
        self.assertEqual(route_pacing.paced_dwells(route, {1: 1100}, {0: 1000}, set(), {}), {})

    def test_uses_the_pace_of_the_step_as_the_floor(self):
        route = route_with([10.0, 12.0], pace="quick")
        self.assertEqual(route_pacing.paced_dwells(route, {1: 1050}, {0: 1000}, set(), {}), {1: 70})


class PacingFileTest(unittest.TestCase):
    def test_names_the_route_it_was_baked_for(self):
        text = route_pacing.pacing_file_text(b"step a ghost_on b 0 90 free\n", {3: 120})
        self.assertEqual(text.splitlines()[0], "route 201922e6")
        self.assertEqual(text.splitlines()[1], "dwell 3 120")

    def test_round_trips_the_stretched_steps(self):
        with tempfile.TemporaryDirectory() as folder:
            path = os.path.join(folder, route_pacing.PACING_FILE_NAME)
            with open(path, "w") as output:
                output.write(route_pacing.pacing_text({7: 300, 2: 120}))
            with open(path) as source:
                self.assertEqual(source.read(), "dwell 2 120\ndwell 7 300\n")
            self.assertEqual(route_pacing.read_pacing(path), {2, 7})

    def test_reads_a_missing_file_as_nothing_stretched(self):
        self.assertEqual(route_pacing.read_pacing("/nonexistent/route_pacing.txt"), set())


if __name__ == "__main__":
    unittest.main()
