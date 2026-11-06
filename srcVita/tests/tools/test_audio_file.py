import struct
import unittest

import audio_file

OGG_BYTES = b"OggS" + b"\x00" * 60


def wrapped(ogg_bytes=OGG_BYTES, rate=44100, channels=2, loop_flag=0, loop_start=0, loop_last=0):
    header = bytearray(audio_file.WRAPPER_SIZE)
    header[0:4] = b"srcd"
    struct.pack_into("<I", header, 8, len(ogg_bytes))
    header[12:16] = b"ogg "
    struct.pack_into("<I", header, 28, channels)
    struct.pack_into("<I", header, 36, rate)
    struct.pack_into("<I", header, 40, rate)
    header[52] = loop_flag
    struct.pack_into("<I", header, 53, loop_start)
    struct.pack_into("<I", header, 57, loop_last)
    struct.pack_into("<I", header, 78, audio_file.WRAPPER_SIZE)
    return bytes(header) + ogg_bytes


class ReadStreamTests(unittest.TestCase):
    def test_the_wrapper_is_stripped_and_its_facts_are_read(self):
        stream = audio_file.read_stream(wrapped(channels=1, rate=44100), "a")
        self.assertEqual(stream["ogg"], OGG_BYTES)
        self.assertEqual(stream["channels"], 1)
        self.assertEqual(stream["rate"], 44100)
        self.assertIsNone(stream["loop_points"])

    def test_loop_points_come_from_the_wrapper_when_its_flag_is_set(self):
        stream = audio_file.read_stream(wrapped(loop_flag=1, loop_start=1000, loop_last=2000), "a")
        self.assertEqual(stream["loop_points"], (1000, 2000))
        self.assertEqual(audio_file.loop_text(stream["loop_points"]), "1000 2000\n")

    def test_loop_fields_without_the_flag_are_ignored(self):
        stream = audio_file.read_stream(wrapped(loop_flag=0, loop_start=5, loop_last=9), "a")
        self.assertIsNone(stream["loop_points"])

    def test_bad_wrappers_are_refused_and_name_the_source(self):
        good = wrapped()
        bad_files = {
            "too short": good[:40],
            "the stub": b"srch" + good[4:],
            "wrong tag": good[:12] + b"wav " + good[16:],
            "wrong size": good[:8] + struct.pack("<I", 1) + good[12:],
            "wrong offset": good[:78] + struct.pack("<I", 90) + good[82:],
            "no ogg": good[:audio_file.WRAPPER_SIZE] + b"RIFF" + good[audio_file.WRAPPER_SIZE + 4:],
        }
        for reason, data in bad_files.items():
            with self.subTest(reason):
                with self.assertRaisesRegex(ValueError, "some/source"):
                    audio_file.read_stream(data, "some/source")


class RateTests(unittest.TestCase):
    def test_44100_is_accepted(self):
        audio_file.check_playback_rate(audio_file.read_stream(wrapped(rate=44100), "a"), "a")

    def test_another_rate_is_refused_with_a_clear_message(self):
        stream = audio_file.read_stream(wrapped(rate=18000), "se/odd.asrc.31")
        with self.assertRaisesRegex(ValueError, "se/odd.asrc.31 is 18000 Hz"):
            audio_file.check_playback_rate(stream, "se/odd.asrc.31")


if __name__ == "__main__":
    unittest.main()
