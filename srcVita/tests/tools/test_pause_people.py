import json
import os
import unittest

import pause_people

GAME_JSON = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets", "game.json")


def small_people():
    return {
        "names": ["linne", "linne_g", "cicel"],
        "places": ["stage01_00", "stage02_00"],
        "order": [3, 1],
        "skip_people": [2],
        "skip_places": [],
        "first_people": [3],
        "first_places": [1],
        "portraits": {"cicel": 0, "linne": 1},
        "place_pictures": {"stage01_00": 0},
    }


class NoteJobsTest(unittest.TestCase):
    def test_each_baked_level_becomes_one_card_with_the_native_label_formats(self):
        entries = ["database_linne_sub_010", "database_linne_010", "database_linne_sub_020", "database_linne_020", "database_cicel_sub_010", "database_stage01_00_sub_010", "database_stage01_00_sub2_010", "database_stage01_00_010", "other_linne_sub_030"]
        jobs = pause_people.note_jobs(small_people(), entries, "database_")
        cards = [(job["list"], job["id"], job["level"], job["texture"]) for job in jobs]
        self.assertEqual(cards, [(1, 1, 10, "pause_note_1_01_010"), (1, 1, 20, "pause_note_1_01_020"), (1, 3, 10, "pause_note_1_03_010"), (2, 1, 10, "pause_note_2_01_010")])
        self.assertEqual(jobs[0]["keys"], ["database_linne_sub_010", "database_linne_010"])
        self.assertEqual(jobs[3]["keys"], ["database_stage01_00_sub_010", "database_stage01_00_sub2_010", "database_stage01_00_010"])


class TableLinesTest(unittest.TestCase):
    def test_art_lines_use_one_based_ids_like_the_script_notebook(self):
        lines = pause_people.table_lines(small_people())
        self.assertEqual(lines[0], "order 3 1")
        self.assertIn("face 1 pause_face_01", lines)
        self.assertIn("face 3 pause_face_00", lines)
        self.assertIn("place 1 pause_place_00", lines)
        self.assertIn("place_count 2", lines)


class ThumbPlacesTest(unittest.TestCase):
    def test_cells_sit_on_a_pitch_that_keeps_the_sheet_block_aligned(self):
        places = pause_people.thumb_places([("a", ""), ("unknown", ""), ("frame", "")])
        self.assertEqual(places[2], ("frame", 2 * pause_people.CELL_PITCH[0], 0, 116, 90))
        self.assertEqual(pause_people.CELL_PITCH[0] % 4, 0)
        self.assertEqual(pause_people.CELL_PITCH[1] % 4, 0)
        self.assertGreaterEqual(pause_people.CELL_PITCH[1], pause_people.FRAME_SIZE[1])


class GameTableTest(unittest.TestCase):
    def test_every_listed_person_has_a_portrait_and_every_shown_place_a_picture(self):
        with open(GAME_JSON, encoding="ascii") as source:
            people = json.load(source)["pause"]["people"]
        names = people["names"]
        listed = [names[index - 1] for index in people["order"] if index not in people["skip_people"]]
        waiting = people["unknown_portraits"]
        self.assertEqual(sorted(listed), sorted(list(people["portraits"]) + waiting))
        self.assertEqual(set(waiting) & set(people["portraits"]), set())
        shown = [name for index, name in enumerate(people["places"]) if index + 1 not in people["skip_places"]]
        self.assertEqual(sorted(shown), sorted(people["place_pictures"]))
        self.assertEqual(len(set(people["portraits"].values())), len(people["portraits"]))


if __name__ == "__main__":
    unittest.main()
