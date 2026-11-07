import unittest

import ds_music


class FakeRecord:
    def __init__(self):
        self.stored = {}

    def key_for(self, code, settings, data):
        return "key"

    def read_if_current(self, path, key):
        return self.stored.get(path)

    def details_of(self, path):
        return {}

    def store(self, path, key, data, details=None):
        self.stored[path] = data


class DsMusicTests(unittest.TestCase):
    def test_a_track_with_no_sequence_keeps_the_remaster_audio(self):
        files = ds_music.music_files({"sequences": {}}, b"", [{"name": "song", "ogg": b"ogg", "loop_points": (5, 9)}], FakeRecord())
        self.assertEqual(files["music_ds/song.ogg"], b"ogg")
        self.assertEqual(files["music_ds/song.loop"], b"5 9\n")

    def test_a_track_with_no_loop_writes_no_loop_file(self):
        files = ds_music.music_files({"sequences": {}}, b"", [{"name": "song", "ogg": b"ogg", "loop_points": None}], FakeRecord())
        self.assertNotIn("music_ds/song.loop", files)

    def test_frames_scale_to_the_playback_rate(self):
        self.assertEqual(ds_music.frames_at_playback_rate(32728, 32728), 44100)


if __name__ == "__main__":
    unittest.main()
