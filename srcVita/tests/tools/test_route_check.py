import os
import shutil
import tempfile
import unittest

import route_check


class TracedCallTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.route_path = os.path.join(self.folder, "route.txt")
        with open(self.route_path, "w", encoding="ascii") as route:
            route.write("step a phone_call tel 4 0 -\n")

    def test_a_place_asked_of_a_traced_call_is_a_problem(self):
        marks = [("route_step_done", "0:phone_call:tel:4"), ("route_place_in_traced_call", "0:phone_call:tel:4")]
        self.assertEqual(route_check.route_problems(marks, self.route_path, 0), ["route_place_in_traced_call 0:phone_call:tel:4"])

    def test_a_place_picked_from_the_list_is_no_problem(self):
        marks = [("route_step_done", "0:phone_call:tel:4")]
        self.assertEqual(route_check.route_problems(marks, self.route_path, 0), [])


if __name__ == "__main__":
    unittest.main()
