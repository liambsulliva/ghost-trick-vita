import unittest

import synthetic_files
import timeline_file


class TimelineFileTests(unittest.TestCase):
    def test_decode_reads_records_pages_and_blanks(self):
        records = [
            (synthetic_files.record_shorts(10, (-4, -6), (-2, 8)), (40.0, 80.0), 0),
            (synthetic_files.record_shorts(10, (-4, -6), (-2, 8)), (40.0, 80.0), 3),
            (synthetic_files.record_shorts(0, (0, 0), (0, 0)), (0.0, 0.0), 3),
            (synthetic_files.record_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 4),
        ]
        data = synthetic_files.timeline_file(["charfiles/2D/a_00_IML4.tex", "charfiles/2D/a_01_IML4.tex"], (64, 32), (16, 16), [8, 2], records, 2)
        timeline = timeline_file.decode_timeline(data)
        self.assertEqual(timeline["atlas_size"], (64, 32))
        self.assertEqual(timeline["cell_size"], (16, 16))
        self.assertEqual(timeline["records_per_ds_frame"], 2)
        self.assertEqual(len(timeline["records"]), 4)
        self.assertEqual(timeline["records"][0]["page"], 0)
        self.assertEqual(timeline["records"][0]["anchor"], (-4, -6))
        self.assertEqual(timeline["records"][0]["point"], (-2, 8))
        self.assertEqual(timeline["records"][1]["cell_in_page"], 3)
        self.assertTrue(timeline["records"][2]["is_blank"])
        self.assertIsNone(timeline["records"][2]["page"])
        self.assertEqual(timeline["records"][3]["position"], (1.0, 2.0))

    def test_decode_reads_the_placement_shift_and_ignores_record_adjust(self):
        records = [(synthetic_files.record_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 0, (12, -2))]
        data = synthetic_files.timeline_file(["a"], (64, 32), (16, 16), [8], records, 1, (24, -4))
        timeline = timeline_file.decode_timeline(data)
        self.assertEqual(timeline["placement_shift"], (24, -4))
        self.assertNotIn("adjust", timeline["records"][0])

    def test_records_per_ds_frame_comes_from_the_file(self):
        record = (synthetic_files.record_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 0)
        data = synthetic_files.timeline_file(["a"], (64, 32), (16, 16), [8], [record] * 8, 2)
        self.assertEqual(timeline_file.decode_timeline(data)["records_per_ds_frame"], 4)

    def test_decode_finds_the_second_page(self):
        records = [(synthetic_files.record_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 9)]
        data = synthetic_files.timeline_file(["a", "b"], (64, 32), (16, 16), [8, 2], records, 1)
        record = timeline_file.decode_timeline(data)["records"][0]
        self.assertEqual(record["page"], 1)
        self.assertEqual(record["cell_in_page"], 1)

    def test_records_on_pages_without_art_are_blank_and_counted(self):
        records = [(synthetic_files.record_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 0), (synthetic_files.record_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 9)]
        empty_page = timeline_file.decode_timeline(synthetic_files.timeline_file(["a"], (64, 32), (16, 16), [0], records[:1], 1))
        self.assertTrue(empty_page["records"][0]["is_blank"])
        self.assertEqual(empty_page["records_without_art"], 1)
        unlisted_pages = timeline_file.decode_timeline(synthetic_files.timeline_file([], (64, 32), (16, 16), [8, 2], records, 1))
        self.assertTrue(all(record["is_blank"] for record in unlisted_pages["records"]))
        self.assertEqual(unlisted_pages["records_without_art"], 2)

    def test_not_a_user_file_is_refused(self):
        with self.assertRaises(timeline_file.TimelineError):
            timeline_file.decode_timeline(b"NOPE" + bytes(64))


if __name__ == "__main__":
    unittest.main()
