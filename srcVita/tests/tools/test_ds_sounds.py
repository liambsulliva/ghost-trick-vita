import struct
import unittest
from unittest import mock

import numpy

import chapter_assets
import ds_sounds
import sseq_render
from test_sseq_render import (
    NOTE_C4, allocate_tracks, basic_bank, info_section, note, open_track, pcm16_wave, rest, sequence_file, sound_archive,
    wave_archive_file, wave_entry,
)

SEQUENCE_NAME = "SEQ_TEST"
EMPTY = None
RAMP_LENGTH = 64


def sequence_archive_file(entries, blob):
    data_offset = ds_sounds.SEQUENCE_ARCHIVE_ENTRY_LIST + ds_sounds.SEQUENCE_ARCHIVE_ENTRY_SIZE * len(entries)
    rows = bytearray()
    for entry in entries:
        if entry is EMPTY:
            rows += struct.pack("<IHBBBBH", ds_sounds.EMPTY_ENTRY_OFFSET, 0, 0, 0, 0, 0, 0)
        else:
            offset, volume = entry
            rows += struct.pack("<IHBBBBH", offset, 0, volume, 64, 64, 0, 0)
    body = struct.pack("<II", data_offset, len(entries)) + bytes(rows) + blob
    return b"SSAR" + struct.pack("<HHIHH", 0xFEFF, 0x0100, 24 + len(body), 16, 1) + b"DATA" + struct.pack("<I", 8 + len(body)) + body


def archive_with_groups(groups, bank=None, waves=None):
    files = [sequence_archive_file(entries, blob) for entries, blob in groups] + [bank or basic_bank(), wave_archive_file(waves or [pcm16_wave()])]
    bank_file_id = len(groups)
    group_rows = [struct.pack("<I", index) for index in range(len(groups))]
    bank_row = struct.pack("<I4H", bank_file_id, 0, 0xFFFF, 0xFFFF, 0xFFFF)
    wave_archive_row = struct.pack("<I", bank_file_id + 1)
    player_row = struct.pack("<HHI", 1, 0, 0)
    info = info_section([[], group_rows, [bank_row], [wave_archive_row], [player_row], [], [], []])
    info_offset = 64
    file_table_offset = info_offset + len(info)
    file_table_size = 12 + 16 * len(files)
    file_section_offset = file_table_offset + file_table_size
    rows = bytearray()
    position = file_section_offset + 16
    for file_bytes in files:
        rows += struct.pack("<IIII", position, len(file_bytes), 0, 0)
        position += len(file_bytes)
    file_table = b"FAT " + struct.pack("<II", file_table_size, len(files)) + bytes(rows)
    file_section = b"FILE" + struct.pack("<II", position - file_section_offset, len(files)) + bytes(4) + b"".join(files)
    header = b"SDAT" + struct.pack("<HHIHH", 0xFEFF, 0x0100, position, 64, 4)
    header += struct.pack("<8I", 0, 0, info_offset, len(info), file_table_offset, file_table_size, file_section_offset, len(file_section))
    return header + bytes(64 - len(header)) + info + file_table + file_section


def frames_of(result):
    return numpy.frombuffer(result["samples"], dtype="<i2").reshape(-1, 2).astype(numpy.int64)


def ramp_wave(timer):
    levels = b"".join(struct.pack("<h", 400 * step - 12000) for step in range(RAMP_LENGTH))
    return wave_entry(sseq_render.FORMAT_PCM16, levels, looped=True, timer=timer)


def arm9_with_rows(rows):
    arm9 = bytearray(chapter_assets.SOUND_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.SOUND_ROW_SIZE * chapter_assets.SOUND_ROW_COUNT)
    for kind in range(chapter_assets.SOUND_ROW_COUNT):
        struct.pack_into("<hhHH", arm9, chapter_assets.SOUND_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.SOUND_ROW_SIZE * kind, 0, -1, 0, 0)
    for kind, (index, group) in rows.items():
        struct.pack_into("<hhHH", arm9, chapter_assets.SOUND_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.SOUND_ROW_SIZE * kind, index, group, 0, 0)
    return bytes(arm9)


class FakeRecord:
    def __init__(self):
        self.stored = {}
        self.details = {}

    def key_for(self, code, settings, data):
        return (code, tuple(sorted(settings.items())), data)

    def read_if_current(self, path, key):
        stored = self.stored.get(path)
        return stored[1] if stored is not None and stored[0] == key else None

    def details_of(self, path):
        return self.details.get(path, {})

    def store(self, path, key, data, details=None):
        self.stored[path] = (key, data)
        self.details[path] = details or {}


class EntryTests(unittest.TestCase):
    def setUp(self):
        blob = note(NOTE_C4, 12) + bytes([0xFF])
        self.archive = sseq_render.read_archive(archive_with_groups([([(0, 100), EMPTY], blob)]))

    def test_an_entry_gives_its_offset_volume_and_the_data_after_the_entry_list(self):
        entry = ds_sounds.sequence_archive_entry(self.archive, 0, 0)
        self.assertEqual((entry["offset"], entry["volume"], entry["bank"], entry["player"]), (0, 100, 0, 0))
        self.assertEqual(entry["data"][:len(note(NOTE_C4, 12))], note(NOTE_C4, 12))

    def test_an_empty_entry_like_native_offset_minus_one_is_absent(self):
        self.assertIsNone(ds_sounds.sequence_archive_entry(self.archive, 0, 1))

    def test_an_index_past_the_count_or_a_missing_group_is_absent(self):
        self.assertIsNone(ds_sounds.sequence_archive_entry(self.archive, 0, 2))
        self.assertIsNone(ds_sounds.sequence_archive_entry(self.archive, 1, 0))
        with self.assertRaises(ValueError):
            ds_sounds.render_sound(archive_with_groups([([(0, 100), EMPTY], bytes([0xFF]))]), 0, 1)


class RenderTests(unittest.TestCase):
    def assert_same_as_sequence(self, sequence_payload, blob, offset, volume=127):
        sequence_result = sseq_render.render_sequence(sound_archive([(SEQUENCE_NAME, sequence_payload, volume)], basic_bank(), [pcm16_wave()]), SEQUENCE_NAME)
        sound_result = ds_sounds.render_sound(archive_with_groups([([(0, 127), (offset, volume)], blob)]), 0, 1)
        self.assertEqual(sound_result["samples"], sequence_result["samples"])
        self.assertGreater(numpy.abs(frames_of(sound_result)).max(), 0)
        return sound_result

    def test_an_entry_at_an_offset_plays_like_the_same_bytes_as_a_sequence(self):
        payload = note(NOTE_C4, 24) + rest(6) + note(NOTE_C4 + 5, 12) + bytes([0xFF])
        padding = note(NOTE_C4 + 2, 3) + bytes([0xFF])
        result = self.assert_same_as_sequence(payload, padding + payload, len(padding), volume=90)
        self.assertIsNone(result["loop_start"])

    def test_track_offsets_inside_an_entry_count_from_the_start_of_the_shared_data(self):
        padding = note(NOTE_C4 + 2, 3) + bytes([0xFF])
        header_size = len(allocate_tracks(0b11)) + len(open_track(1, 0))
        first = note(NOTE_C4, 24) + bytes([0xFF])
        second = note(NOTE_C4 + 7, 12) + bytes([0xFF])
        sequence_payload = allocate_tracks(0b11) + open_track(1, header_size + len(first)) + first + second
        blob = padding + allocate_tracks(0b11) + open_track(1, len(padding) + header_size + len(first)) + first + second
        self.assert_same_as_sequence(sequence_payload, blob, len(padding))


class HeldLoopTests(unittest.TestCase):
    def render_held(self, timer, wave=None):
        blob = note(NOTE_C4, 0) + bytes([0xFF])
        return ds_sounds.render_sound(archive_with_groups([([(0, 127)], blob)], waves=[wave or ramp_wave(timer)]), 0, 0)

    def test_a_note_held_on_a_looped_wave_ends_in_one_wave_loop(self):
        timer = 1024
        result = self.render_held(timer)
        self.assertEqual(result["loop_end"] - result["loop_start"], RAMP_LENGTH * timer // sseq_render.TIMER_STEPS_PER_SAMPLE)
        frames = frames_of(result)
        self.assertEqual(len(frames), result["loop_end"])
        self.assertTrue(numpy.array_equal(frames[result["loop_end"] - 32:result["loop_end"]], frames[result["loop_start"] - 32:result["loop_start"]]))
        self.assertGreater(numpy.abs(frames).max(), 0)

    def test_the_loop_starts_only_once_the_wave_is_inside_its_loop(self):
        intro_and_loop = b"".join(struct.pack("<h", 250 * step - 16000) for step in range(RAMP_LENGTH * 2))
        wave = wave_entry(sseq_render.FORMAT_PCM16, intro_and_loop, looped=True, timer=512, loop_words=RAMP_LENGTH // 2)
        result = self.render_held(512, wave)
        self.assertEqual(result["loop_end"] - result["loop_start"], RAMP_LENGTH)
        self.assertGreaterEqual(result["loop_start"], RAMP_LENGTH + sseq_render.PCM_START_DELAY)
        frames = frames_of(result)
        self.assertTrue(numpy.array_equal(frames[result["loop_end"] - 1], frames[result["loop_start"] - 1]))

    def test_a_held_render_stops_at_the_loop_instead_of_the_longest_render(self):
        result = self.render_held(512)
        self.assertLess(result["loop_end"], sseq_render.SAMPLE_RATE)

    def test_a_note_on_a_wave_that_does_not_loop_does_not_loop(self):
        wave = pcm16_wave(looped=False)
        blob = note(NOTE_C4, 0) + bytes([0xFF])
        result = ds_sounds.render_sound(archive_with_groups([([(0, 127)], blob)], waves=[wave]), 0, 0)
        self.assertIsNone(result["loop_start"])


class SequencerShapeTests(unittest.TestCase):
    def test_the_archive_sequencer_sets_everything_the_sequence_sequencer_sets(self):
        sequence_archive = sseq_render.read_archive(sound_archive([(SEQUENCE_NAME, bytes([0xFF]), 127)], basic_bank(), [pcm16_wave()]))
        base = sseq_render.Sequencer(sequence_archive, 0)
        group_archive = sseq_render.read_archive(archive_with_groups([([(0, 127)], bytes([0xFF]))]))
        sound = ds_sounds.SequenceArchiveSequencer(group_archive, ds_sounds.sequence_archive_entry(group_archive, 0, 0))
        self.assertEqual(set(vars(base)), set(vars(sound)) - {"start_offset"})


class RowTests(unittest.TestCase):
    def test_kinds_name_their_sound_like_the_remaster_rows_and_skip_missing_entries(self):
        blob = note(NOTE_C4, 6) + bytes([0xFF])
        sdat = archive_with_groups([([(0, 127)] * 6, blob), ([(0, 127)] * 3 + [EMPTY], blob)])
        arm9 = arm9_with_rows({1: (5, 0), 4: (2, 1), 7: (3, 1), 9: (8, 0), 12: (2, 1)})
        sounds, kinds = ds_sounds.sound_rows(sdat, [1, 4, 7, 9, 12, 600], arm9)
        self.assertEqual(kinds, {"1": "sys_005", "4": "gim_002", "12": "gim_002"})
        self.assertEqual(sounds, [{"name": "sys_005", "group": 0, "index": 5}, {"name": "gim_002", "group": 1, "index": 2}])


class FileTests(unittest.TestCase):
    def test_sounds_write_ogg_and_a_loop_file_only_where_they_hold(self):
        held = note(NOTE_C4, 0) + bytes([0xFF])
        short = note(NOTE_C4, 6) + bytes([0xFF])
        sdat = archive_with_groups([([(0, 127), (len(held), 127)], held + short)], waves=[ramp_wave(1024)])
        record = FakeRecord()
        rows = [{"name": "sys_000", "group": 0, "index": 0}, {"name": "sys_001", "group": 0, "index": 1}]
        with mock.patch.object(ds_sounds.ds_music, "encode_ogg", return_value=b"ogg") as encode:
            files = ds_sounds.sound_files(sdat, rows, record, code="code")
            again = ds_sounds.sound_files(sdat, rows, record, code="code")
        self.assertEqual(encode.call_count, 2)
        self.assertEqual(files, again)
        self.assertEqual(set(files), {"sounds/sys_000.ogg", "sounds/sys_000.loop", "sounds/sys_001.ogg"})
        start, last = (int(value) for value in files["sounds/sys_000.loop"].split())
        self.assertGreater(last, start)


if __name__ == "__main__":
    unittest.main()
