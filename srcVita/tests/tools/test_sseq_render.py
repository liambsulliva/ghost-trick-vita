import math
import struct
import unittest

import numpy

import sseq_render

SEQUENCE_NAME = "SEQ_TEST"
ONE_WAVE_STEP_PER_SAMPLE = 512
CONSTANT_LEVEL = 16000
RECORD_LIST_OFFSET = 0x3C
NOTE_C4 = 60
END_TRACK = bytes([0xFF])
FAST_RELEASE_SAMPLES = 1000
LOW_NIBBLES_THEN_HIGH = bytes([0x31, 0x75, 0x98, 0xCA, 0x0F, 0xFF, 0xFF, 0x77, 0x77, 0x07, 0x88, 0xF8, 0x24, 0x6B] * 40)


def nitro_file(magic, body):
    return magic + struct.pack("<HHIHH", 0xFEFF, 0x0100, 24 + len(body), 16, 1) + b"DATA" + struct.pack("<I", 8 + len(body)) + body


def sequence_file(payload):
    return nitro_file(b"SSEQ", struct.pack("<I", 0x1C) + payload)


def instrument_parameters(wave=0, wave_archive=0, root_key=NOTE_C4, attack=127, decay=127, sustain=127, release=127, pan=64):
    return struct.pack("<HHBBBBBB", wave, wave_archive, root_key, attack, decay, sustain, release, pan)


def bank_file(instruments):
    data_offset = RECORD_LIST_OFFSET + 4 * len(instruments)
    records = bytearray()
    blob = bytearray()
    for kind, data in instruments:
        records += struct.pack("<I", kind | ((data_offset + len(blob)) << 8))
        blob += data
    return nitro_file(b"SBNK", bytes(32) + struct.pack("<I", len(instruments)) + bytes(records) + bytes(blob))


def drum_set(lowest, entries):
    return bytes([lowest, lowest + len(entries) - 1]) + b"".join(bytes([kind, 0]) + parameters for kind, parameters in entries)


def key_split(top_keys, entries):
    return bytes(top_keys) + bytes(8 - len(top_keys)) + b"".join(bytes([kind, 0]) + parameters for kind, parameters in entries)


def wave_entry(wave_format, data, looped=False, timer=ONE_WAVE_STEP_PER_SAMPLE, loop_words=0):
    length_words = len(data) // 4 - loop_words
    return struct.pack("<BBHHHI", wave_format, 1 if looped else 0, 32728, timer, loop_words, length_words) + data


def pcm16_wave(level=CONSTANT_LEVEL, count=64, looped=True, timer=ONE_WAVE_STEP_PER_SAMPLE):
    return wave_entry(sseq_render.FORMAT_PCM16, struct.pack("<h", level) * count, looped=looped, timer=timer)


def wave_archive_file(waves):
    data_offset = RECORD_LIST_OFFSET + 4 * len(waves)
    offsets = bytearray()
    blob = bytearray()
    for wave in waves:
        offsets += struct.pack("<I", data_offset + len(blob))
        blob += wave
    return nitro_file(b"SWAR", bytes(32) + struct.pack("<I", len(waves)) + bytes(offsets) + bytes(blob))


def info_section(tables):
    header_size = 64
    table_bytes = bytearray()
    entry_bytes = bytearray()
    entry_base = header_size + sum(4 + 4 * len(table) for table in tables)
    table_offsets = []
    for table in tables:
        table_offsets.append(header_size + len(table_bytes))
        table_bytes += struct.pack("<I", len(table))
        for entry in table:
            if entry is None:
                table_bytes += struct.pack("<I", 0)
            else:
                table_bytes += struct.pack("<I", entry_base + len(entry_bytes))
                entry_bytes += entry
    body = struct.pack("<8I", *table_offsets) + bytes(24) + bytes(table_bytes) + bytes(entry_bytes)
    return b"INFO" + struct.pack("<I", 8 + len(body)) + body


def symbol_section(names):
    names_offset = 64 + 4 + 4 * len(names)
    offsets = bytearray()
    blob = bytearray()
    for name in names:
        offsets += struct.pack("<I", names_offset + len(blob))
        blob += name.encode("ascii") + b"\x00"
    body = struct.pack("<8I", 64, 0, 0, 0, 0, 0, 0, 0) + bytes(24) + struct.pack("<I", len(names)) + bytes(offsets) + bytes(blob)
    return b"SYMB" + struct.pack("<I", 8 + len(body)) + body


def sound_archive(sequences, bank, waves, with_symbols=True, channel_mask=0):
    files = [sequence_file(payload) for _, payload, _ in sequences] + [bank, wave_archive_file(waves)]
    bank_file_id = len(sequences)
    sequence_rows = [struct.pack("<IHBBBB", index, 0, volume, 64, 64, 0) for index, (_, _, volume) in enumerate(sequences)]
    bank_row = struct.pack("<I4H", bank_file_id, 0, 0xFFFF, 0xFFFF, 0xFFFF)
    wave_archive_row = struct.pack("<I", (bank_file_id + 1) | 0x01000000)
    player_row = struct.pack("<HHI", 1, channel_mask, 0)
    info = info_section([sequence_rows, [], [bank_row], [wave_archive_row], [player_row], [], [], []])
    symbols = symbol_section([name for name, _, _ in sequences]) if with_symbols else b""
    file_table_size = 12 + 16 * len(files)
    symbols_offset = 64
    info_offset = symbols_offset + len(symbols)
    file_table_offset = info_offset + len(info)
    file_section_offset = file_table_offset + file_table_size
    file_rows = bytearray()
    position = file_section_offset + 16
    for file_bytes in files:
        file_rows += struct.pack("<IIII", position, len(file_bytes), 0, 0)
        position += len(file_bytes)
    file_table = b"FAT " + struct.pack("<II", file_table_size, len(files)) + bytes(file_rows)
    file_section = b"FILE" + struct.pack("<II", position - file_section_offset, len(files)) + bytes(4) + b"".join(files)
    header = b"SDAT" + struct.pack("<HHIHH", 0xFEFF, 0x0100, position, 64, 4)
    header += struct.pack("<8I", symbols_offset if with_symbols else 0, len(symbols), info_offset, len(info), file_table_offset, file_table_size, file_section_offset, len(file_section))
    return header + bytes(64 - len(header)) + symbols + info + file_table + file_section


def variable_length(value):
    encoded = [value & 0x7F]
    value >>= 7
    while value:
        encoded.insert(0, (value & 0x7F) | 0x80)
        value >>= 7
    return bytes(encoded)


def note(key, length, velocity=127):
    return bytes([key, velocity]) + variable_length(length)


def rest(ticks):
    return bytes([0x80]) + variable_length(ticks)


def byte_command(command, value):
    return bytes([command, value & 0xFF])


def short_command(command, value):
    return bytes([command]) + struct.pack("<h", value)


def offset_command(command, offset):
    return bytes([command]) + struct.pack("<I", offset)[:3]


def open_track(number, offset):
    return bytes([0x93, number]) + struct.pack("<I", offset)[:3]


def allocate_tracks(mask):
    return bytes([0xFE]) + struct.pack("<H", mask)


def basic_bank():
    return bank_file([(sseq_render.KIND_PCM, instrument_parameters())])


def simulate(payload, bank=None, waves=None, volume=127, channel_mask=0):
    data = sound_archive([(SEQUENCE_NAME, payload, volume)], bank or basic_bank(), waves or [pcm16_wave()], channel_mask=channel_mask)
    return sseq_render.simulate_sequence(data, SEQUENCE_NAME)


def render(payload, bank=None, waves=None, volume=127):
    data = sound_archive([(SEQUENCE_NAME, payload, volume)], bank or basic_bank(), waves or [pcm16_wave()])
    result = sseq_render.render_sequence(data, SEQUENCE_NAME)
    frames = numpy.frombuffer(result["samples"], dtype="<i2").reshape(-1, 2).astype(numpy.int64)
    return result, frames


def voice_start_of_tick_update(update_index):
    return (update_index + 1) * sseq_render.CYCLES_PER_UPDATE // sseq_render.CYCLES_PER_SAMPLE


def voice_timers(voice):
    return [change[1] for change in voice.changes]


class ArchiveTests(unittest.TestCase):
    def test_names_come_from_the_symbols_in_table_order(self):
        data = sound_archive([("SEQ_B", END_TRACK, 127), ("SEQ_A", END_TRACK, 127)], basic_bank(), [pcm16_wave()])
        self.assertEqual(sseq_render.sequence_names(data), ["SEQ_B", "SEQ_A"])

    def test_an_archive_without_symbols_gets_numbered_names(self):
        data = sound_archive([("SEQ_B", END_TRACK, 127), ("SEQ_A", END_TRACK, 127)], basic_bank(), [pcm16_wave()], with_symbols=False)
        self.assertEqual(sseq_render.sequence_names(data), ["SEQ_0", "SEQ_1"])

    def test_a_file_that_is_not_an_archive_is_refused(self):
        with self.assertRaisesRegex(ValueError, "SDAT"):
            sseq_render.sequence_names(b"RIFF" + bytes(64))

    def test_an_unknown_sequence_name_is_refused_by_name(self):
        data = sound_archive([(SEQUENCE_NAME, END_TRACK, 127)], basic_bank(), [pcm16_wave()])
        with self.assertRaisesRegex(ValueError, "SEQ_OTHER"):
            sseq_render.render_sequence(data, "SEQ_OTHER")

    def test_the_result_is_16_bit_stereo_at_the_mixer_rate(self):
        result, frames = render(note(NOTE_C4, 4) + END_TRACK)
        self.assertEqual(result["sample_rate"], 32728)
        self.assertEqual(len(result["samples"]), len(frames) * 4)


class TableTests(unittest.TestCase):
    def test_the_decibel_tables_hold_the_driver_values(self):
        self.assertEqual(sseq_render.DECIBEL_TABLE[:4], [-32768, -421, -361, -325])
        self.assertEqual(sseq_render.DECIBEL_TABLE[64], -60)
        self.assertEqual(sseq_render.DECIBEL_TABLE[127], 0)
        self.assertEqual(sseq_render.DECIBEL_SQUARE_TABLE[:4], [-32768, -722, -721, -651])
        self.assertEqual(sseq_render.DECIBEL_SQUARE_TABLE[64], -119)
        self.assertEqual(sseq_render.DECIBEL_SQUARE_TABLE[120:], [-10, -8, -7, -6, -4, -3, -1, 0])

    def test_the_channel_volume_table_has_723_steps_and_its_divider_ranges(self):
        self.assertEqual(len(sseq_render.CHANNEL_VOLUME_TABLE), 724)
        self.assertEqual(sseq_render.channel_volume_register(0), 127)
        self.assertEqual(sseq_render.channel_volume_register(-1), 126)
        self.assertEqual(sseq_render.channel_volume_register(-60), 64)
        self.assertEqual(sseq_render.channel_volume_register(-61), 126 | (1 << 8))
        self.assertEqual(sseq_render.channel_volume_register(-121), 126 | (2 << 8))
        self.assertEqual(sseq_render.channel_volume_register(-241), 127 | (3 << 8))
        self.assertEqual(sseq_render.channel_volume_register(-723), 3 << 8)
        self.assertEqual(sseq_render.channel_volume_register(-30000), 3 << 8)
        self.assertEqual(sseq_render.channel_volume_register(5), 127)

    def test_the_pitch_table_and_timer_use_1_over_64_of_a_semitone(self):
        self.assertEqual(sseq_render.PITCH_TABLE[:4], [0, 0x3B, 0x76, 0xB2])
        self.assertEqual(sseq_render.PITCH_TABLE[-2:], [0xFF14, 0xFF8A])
        self.assertEqual(sseq_render.channel_timer(8006, 0), 8006)
        self.assertEqual(sseq_render.channel_timer(8006, 768), 4003)
        self.assertEqual(sseq_render.channel_timer(8006, -768), 16012)
        self.assertEqual(sseq_render.channel_timer(8006, 64), 8006 * (sseq_render.PITCH_TABLE[704] + 0x10000) >> 17)

    def test_the_timer_stays_inside_the_hardware_range(self):
        self.assertEqual(sseq_render.channel_timer(16, 768), 16)
        self.assertEqual(sseq_render.channel_timer(0xF000, -768), 0xFFFF)

    def test_rate_conversions_follow_the_driver(self):
        self.assertEqual(sseq_render.attack_rate(0), 255)
        self.assertEqual(sseq_render.attack_rate(108), 147)
        self.assertEqual(sseq_render.attack_rate(109), 143)
        self.assertEqual(sseq_render.attack_rate(127), 0)
        self.assertEqual(sseq_render.decay_rate(127), 0xFFFF)
        self.assertEqual(sseq_render.decay_rate(126), 0x3C00)
        self.assertEqual(sseq_render.decay_rate(49), 99)
        self.assertEqual(sseq_render.decay_rate(50), 0x1E00 // 76)

    def test_the_lfo_sine_covers_a_whole_cycle(self):
        self.assertEqual(sseq_render.LFO_SINE_TABLE[0], 0)
        self.assertEqual(sseq_render.LFO_SINE_TABLE[32], 127)
        self.assertEqual(sseq_render.LFO_SINE_TABLE[64], 0)
        self.assertEqual(sseq_render.LFO_SINE_TABLE[96], -127)


class TimingTests(unittest.TestCase):
    def test_a_rest_delays_the_next_note_at_the_default_tempo(self):
        sequencer = simulate(note(NOTE_C4, 1) + rest(47) + note(NOTE_C4, 1) + END_TRACK)
        self.assertEqual(sequencer.voices[0].start_sample, voice_start_of_tick_update(0))
        self.assertEqual(sequencer.voices[1].start_sample, voice_start_of_tick_update(96))

    def test_a_note_length_in_two_bytes_is_read_as_variable_length(self):
        sequencer = simulate(note(NOTE_C4, 200) + note(NOTE_C4, 1) + END_TRACK)
        self.assertEqual(sequencer.voices[1].start_sample, voice_start_of_tick_update(400))

    def test_a_tempo_command_changes_ticks_per_update(self):
        doubled = simulate(short_command(0xE1, 240) + rest(48) + note(NOTE_C4, 1) + END_TRACK)
        self.assertEqual(doubled.voices[0].start_sample, voice_start_of_tick_update(48))
        halved = simulate(short_command(0xE1, 60) + rest(48) + note(NOTE_C4, 1) + END_TRACK)
        self.assertEqual(halved.voices[0].start_sample, voice_start_of_tick_update(192))

    def test_a_tempo_above_240_runs_more_than_one_tick_in_an_update(self):
        sequencer = simulate(short_command(0xE1, 480) + rest(48) + note(NOTE_C4, 1) + END_TRACK)
        self.assertEqual(sequencer.voices[0].start_sample, voice_start_of_tick_update(24))

    def test_updates_alternate_170_and_171_samples(self):
        sequencer = simulate(note(NOTE_C4, 2) + END_TRACK)
        self.assertEqual(voice_start_of_tick_update(0), 170)
        self.assertEqual(voice_start_of_tick_update(1), 341)
        self.assertEqual(voice_start_of_tick_update(2), 511)
        self.assertEqual(sequencer.voices[0].start_sample, 170)


class LoopTests(unittest.TestCase):
    def assert_passes_match(self, result, frames, samples_for_tails_to_end=FAST_RELEASE_SAMPLES):
        loop_start = result["loop_start"]
        loop_end = result["loop_end"]
        self.assertEqual(len(frames), loop_end)
        length = loop_end - loop_start
        self.assertGreaterEqual(loop_start, length)
        settled = samples_for_tails_to_end
        self.assertTrue(numpy.array_equal(frames[loop_start + settled:loop_end], frames[loop_start - length + settled:loop_start]))
        self.assertTrue(numpy.array_equal(frames[loop_end - 1], frames[loop_start - 1]))

    def test_a_jump_back_gives_the_intro_and_two_passes(self):
        intro = note(NOTE_C4 + 12, 24)
        body = note(NOTE_C4, 12) + rest(12) + note(NOTE_C4 + 7, 24)
        result, frames = render(intro + body + offset_command(0x94, len(intro)))
        self.assertEqual(result["loop_end"] - result["loop_start"], 48 * 2 * 341 // 2)
        self.assert_passes_match(result, frames)
        self.assertGreater(numpy.abs(frames).max(), 0)

    def test_the_loop_is_sample_exact_with_a_tempo_that_is_not_a_whole_number_of_updates(self):
        body = short_command(0xE1, 140) + note(NOTE_C4, 13) + rest(12) + note(NOTE_C4 + 7, 23)
        result, frames = render(body + offset_command(0x94, 3))
        self.assertNotEqual(48 * sseq_render.TICK_COST % 140, 0)
        self.assert_passes_match(result, frames)

    def test_the_loop_is_sample_exact_with_an_odd_number_of_updates(self):
        body = short_command(0xE1, 240) + note(NOTE_C4, 13) + rest(12) + note(NOTE_C4 + 7, 24)
        result, frames = render(body + offset_command(0x94, 3))
        self.assertEqual((result["loop_end"] - result["loop_start"]) % 2, 1)
        self.assert_passes_match(result, frames)

    def test_a_tail_that_crosses_the_loop_point_is_at_the_loop_start_and_the_seam_is_clean(self):
        slow_release = bank_file([(sseq_render.KIND_PCM, instrument_parameters(release=120))])
        body = note(NOTE_C4, 24) + rest(24)
        result, frames = render(body + offset_command(0x94, 0), bank=slow_release)
        tail_updates = 92544 // sseq_render.decay_rate(120) + 1
        self.assertGreater(tail_updates, 48)
        self.assert_passes_match(result, frames, samples_for_tails_to_end=tail_updates * 171)
        self.assertGreater(abs(frames[result["loop_start"]][0]), 0)

    def test_tracks_of_different_lengths_repeat_only_at_their_common_multiple(self):
        first_body = note(NOTE_C4, 16)
        second_body = note(NOTE_C4 + 7, 24)
        header = allocate_tracks(0b11)
        opening = open_track(1, 0)
        first_start = len(header) + len(opening)
        second_start = first_start + len(first_body) + 4
        payload = header + open_track(1, second_start) + first_body + offset_command(0x94, first_start) + second_body + offset_command(0x94, second_start)
        result, frames = render(payload)
        self.assertEqual(result["loop_end"] - result["loop_start"], 48 * 2 * 341 // 2)
        self.assert_passes_match(result, frames)

    def test_a_sequence_that_ends_has_no_loop_and_renders_through_its_release(self):
        slow_release = bank_file([(sseq_render.KIND_PCM, instrument_parameters(release=100))])
        result, frames = render(note(NOTE_C4, 8) + END_TRACK, bank=slow_release)
        self.assertIsNone(result["loop_start"])
        self.assertIsNone(result["loop_end"])
        released_at = voice_start_of_tick_update(16)
        self.assertGreater(len(frames), released_at + 500)
        self.assertGreater(numpy.abs(frames[released_at + 100]).max(), 0)
        self.assertLess(numpy.abs(frames[-1]).max(), numpy.abs(frames[released_at]).max() // 100)

    def test_a_track_that_jumps_to_itself_without_waiting_is_ended(self):
        result, _ = render(offset_command(0x94, 0))
        self.assertIsNone(result["loop_end"])


class WaveTests(unittest.TestCase):
    def test_pcm8_is_shifted_to_16_bits_and_keeps_its_loop_points(self):
        data = struct.pack("<8b", 1, -1, 2, -2, 3, -3, 4, -4)
        wave = sseq_render.decode_wave(wave_archive_file([wave_entry(sseq_render.FORMAT_PCM8, data, looped=True, loop_words=1)]), 0)
        self.assertEqual(wave["levels"].tolist(), [256, -256, 512, -512, 768, -768, 1024, -1024])
        self.assertTrue(wave["looped"])
        self.assertEqual((wave["loop_start"], wave["loop_length"], wave["delay"]), (4, 4, 3))

    def test_pcm16_keeps_its_loop_points(self):
        data = struct.pack("<4h", 100, -100, 200, -200)
        wave = sseq_render.decode_wave(wave_archive_file([wave_entry(sseq_render.FORMAT_PCM16, data, looped=True, loop_words=1)]), 0)
        self.assertEqual(wave["levels"].tolist(), [100, -100, 200, -200])
        self.assertEqual((wave["loop_start"], wave["loop_length"], wave["delay"]), (2, 2, 3))

    def test_ima_adpcm_decodes_like_the_hardware_sample_by_sample(self):
        for start_level, start_index in ((0, 0), (-1000, 30), (32000, 88), (-32000, 88)):
            with self.subTest(start_level=start_level, start_index=start_index):
                body = struct.pack("<hH", start_level, start_index) + LOW_NIBBLES_THEN_HIGH
                self.assertTrue(numpy.array_equal(sseq_render.decode_ima_adpcm(body), reference_ima_decode(start_level, start_index, LOW_NIBBLES_THEN_HIGH)))

    def test_ima_adpcm_loop_points_leave_out_the_header(self):
        body = struct.pack("<hH", 0, 0) + bytes(12)
        wave = sseq_render.decode_wave(wave_archive_file([wave_entry(sseq_render.FORMAT_ADPCM, body, looped=True, loop_words=2)]), 0)
        self.assertEqual(len(wave["levels"]), 24)
        self.assertEqual((wave["loop_start"], wave["loop_length"], wave["delay"]), (8, 16, 11))

    def test_each_format_renders_at_half_level_through_a_center_panned_channel(self):
        full = struct.pack("<h", 0x4000) * 256
        low_bytes = bytes([0x40]) * 256
        ima = struct.pack("<hH", 0, 40) + LOW_NIBBLES_THEN_HIGH
        waves = {
            "pcm8": (wave_entry(sseq_render.FORMAT_PCM8, low_bytes), numpy.full(256, 0x4000), 3),
            "pcm16": (wave_entry(sseq_render.FORMAT_PCM16, full), numpy.full(256, 0x4000), 3),
            "adpcm": (wave_entry(sseq_render.FORMAT_ADPCM, ima[:len(ima) & ~3]), None, 11),
        }
        for name, (wave, expected, delay) in waves.items():
            with self.subTest(name):
                _, frames = render(note(NOTE_C4, 1000) + END_TRACK, waves=[wave])
                start = voice_start_of_tick_update(0)
                if expected is None:
                    expected = sseq_render.decode_wave(wave_archive_file([wave]), 0)["levels"]
                count = min(len(expected), 400)
                rendered = frames[start:start + delay - 1 + count, 0]
                self.assertTrue(numpy.all(rendered[:delay - 1] == 0))
                wanted = expected[:count]
                self.assertLessEqual(numpy.abs(rendered[delay - 1:] - (wanted >> 1)).max(), 1)

    def test_a_looped_wave_keeps_playing_and_a_one_shot_wave_stops(self):
        ramp = struct.pack("<8h", *[1000 * step for step in range(1, 9)])
        _, looped = render(note(NOTE_C4, 30) + END_TRACK, waves=[wave_entry(sseq_render.FORMAT_PCM16, ramp, looped=True, loop_words=1)])
        start = voice_start_of_tick_update(0)
        self.assertEqual(looped[start + 2 + 14, 0], looped[start + 2 + 8, 0])
        self.assertEqual(looped[start + 2 + 8, 0], 3000 >> 1)
        sequencer = simulate(note(NOTE_C4, 30) + END_TRACK, waves=[wave_entry(sseq_render.FORMAT_PCM16, ramp)])
        self.assertLess(sequencer.voices[0].end_sample, voice_start_of_tick_update(4))


def reference_ima_decode(level, index, packed):
    decoded = []
    for byte in packed:
        for nibble in (byte & 15, byte >> 4):
            step = sseq_render.IMA_STEP_SIZES[index]
            difference = step >> 3
            if nibble & 1:
                difference += step >> 2
            if nibble & 2:
                difference += step >> 1
            if nibble & 4:
                difference += step
            if nibble & 8:
                level = max(level - difference, -0x7FFF)
            else:
                level = min(level + difference, 0x7FFF)
            index = min(max(index + (-1, -1, -1, -1, 2, 4, 6, 8)[nibble & 7], 0), 88)
            decoded.append(level)
    return decoded


class InstrumentTests(unittest.TestCase):
    def test_a_program_outside_the_bank_plays_nothing(self):
        self.assertEqual(simulate(byte_command(0x81, 5) + note(NOTE_C4, 1) + END_TRACK).voices, [])

    def test_a_psg_record_sounds_a_square_with_its_duty_at_the_root_key(self):
        bank = bank_file([(sseq_render.KIND_PSG, instrument_parameters(wave=3))])
        _, frames = render(note(NOTE_C4, 1000) + END_TRACK, bank=bank)
        start = voice_start_of_tick_update(0)
        part = frames[start + 1000:start + 1000 + 32728 // 4, 0]
        crossings = numpy.count_nonzero(numpy.diff(numpy.sign(part)) != 0)
        self.assertAlmostEqual(crossings, 261.6 * 2 / 4, delta=2)
        self.assertEqual(part.max(), 0x7FFF >> 1)
        self.assertEqual(part.min(), -0x7FFF >> 1)
        self.assertAlmostEqual(float(part.mean()), 0.0, delta=300)

    def test_psg_duty_7_is_silent_low_and_narrow_duty_is_lopsided(self):
        narrow = sseq_render.DUTY_CYCLE_TABLE[0]
        self.assertEqual(numpy.count_nonzero(narrow > 0), 1)
        self.assertEqual(numpy.count_nonzero(sseq_render.DUTY_CYCLE_TABLE[6] > 0), 7)
        self.assertEqual(numpy.count_nonzero(sseq_render.DUTY_CYCLE_TABLE[7] > 0), 0)

    def test_psg_uses_only_channels_8_to_13(self):
        bank = bank_file([(sseq_render.KIND_PSG, instrument_parameters(wave=3))])
        notes = b"".join(note(NOTE_C4, 40) for _ in range(8))
        sequencer = simulate(byte_command(0xC7, 0) + notes + rest(100) + END_TRACK, bank=bank)
        started_together = [voice for voice in sequencer.voices if voice.end_sample > voice_start_of_tick_update(10)]
        self.assertEqual(len(started_together), 6)

    def test_a_noise_record_sounds_both_extremes_and_uses_only_two_channels(self):
        bank = bank_file([(sseq_render.KIND_NOISE, instrument_parameters())])
        _, frames = render(note(NOTE_C4, 1000) + END_TRACK, bank=bank)
        part = frames[2000:6000, 0]
        self.assertEqual(part.max(), 0x7FFF >> 1)
        self.assertEqual(part.min(), -0x7FFF >> 1)
        notes = b"".join(note(NOTE_C4, 40) for _ in range(4))
        sequencer = simulate(byte_command(0xC7, 0) + notes + rest(100) + END_TRACK, bank=bank)
        self.assertEqual(len([voice for voice in sequencer.voices if voice.end_sample > voice_start_of_tick_update(10)]), 2)

    def test_the_noise_generator_repeats_after_32767_steps(self):
        self.assertEqual(len(sseq_render.NOISE_TABLE), 32767)
        self.assertEqual(sseq_render.NOISE_TABLE[0], -0x7FFF)
        self.assertLess(abs(int(sseq_render.NOISE_TABLE.sum())), 0x7FFF * 2)

    def test_a_drum_set_chooses_by_key_and_ignores_keys_outside_it(self):
        entries = [(sseq_render.KIND_PCM, instrument_parameters(wave=0, root_key=60)), (sseq_render.KIND_PCM, instrument_parameters(wave=1, root_key=61))]
        bank = bank_file([(sseq_render.KIND_DRUM_SET, drum_set(60, entries))])
        waves = [pcm16_wave(timer=1000), pcm16_wave(timer=2000)]
        sequencer = simulate(note(60, 1) + note(61, 1) + note(62, 1) + note(59, 1) + END_TRACK, bank=bank, waves=waves)
        self.assertEqual([voice_timers(voice)[0] for voice in sequencer.voices], [1000, 2000])

    def test_a_key_split_chooses_the_first_region_at_or_above_the_key(self):
        entries = [(sseq_render.KIND_PCM, instrument_parameters(wave=0, root_key=59)), (sseq_render.KIND_PCM, instrument_parameters(wave=1, root_key=72))]
        bank = bank_file([(sseq_render.KIND_KEY_SPLIT, key_split([59, 72], entries))])
        waves = [pcm16_wave(timer=1000), pcm16_wave(timer=2000)]
        sequencer = simulate(note(59, 1) + note(72, 1) + note(73, 1) + END_TRACK, bank=bank, waves=waves)
        self.assertEqual([voice_timers(voice)[0] for voice in sequencer.voices], [1000, 2000])

    def test_a_key_above_the_root_shortens_the_timer_by_a_semitone(self):
        sequencer = simulate(note(NOTE_C4 + 12, 1) + note(NOTE_C4 + 1, 1) + END_TRACK, waves=[pcm16_wave(timer=4096)])
        self.assertEqual(voice_timers(sequencer.voices[0])[0], 2048)
        self.assertEqual(voice_timers(sequencer.voices[1])[0], sseq_render.channel_timer(4096, 64))


class EnvelopeTests(unittest.TestCase):
    def channel(self, attack=127, decay=127, sustain=127, release=127):
        channel = sseq_render.Channel(0)
        channel.attack = sseq_render.attack_rate(attack)
        channel.decay = sseq_render.decay_rate(decay)
        channel.sustain = sustain
        channel.release = sseq_render.decay_rate(release)
        return channel

    def test_attack_multiplies_by_rate_over_256_from_the_bottom(self):
        channel = self.channel(attack=0)
        self.assertEqual(channel.attenuation, -92544)
        channel.step_envelope()
        self.assertEqual(channel.attenuation, -((92544 * 255) >> 8))

    def test_an_instant_attack_goes_to_decay_on_the_first_update(self):
        channel = self.channel(attack=127)
        self.assertEqual(channel.step_envelope(), 0)
        self.assertEqual(channel.envelope_stage, sseq_render.ENVELOPE_DECAY)

    def test_decay_subtracts_until_the_sustain_level_from_the_square_table(self):
        channel = self.channel(attack=127, decay=100, sustain=64)
        channel.step_envelope()
        channel.step_envelope()
        self.assertEqual(channel.attenuation, -sseq_render.decay_rate(100))
        for _ in range(2000):
            channel.step_envelope()
        self.assertEqual(channel.envelope_stage, sseq_render.ENVELOPE_SUSTAIN)
        self.assertEqual(channel.attenuation, -119 << 7)
        self.assertEqual(channel.step_envelope(), -119)

    def test_release_subtracts_its_rate(self):
        channel = self.channel(release=100)
        channel.attenuation = -1000
        channel.envelope_stage = sseq_render.ENVELOPE_RELEASE
        channel.step_envelope()
        self.assertEqual(channel.attenuation, -1000 - sseq_render.decay_rate(100))

    def test_track_commands_override_the_instrument_envelope(self):
        payload = byte_command(0xD0, 0) + byte_command(0xD1, 1) + byte_command(0xD2, 2) + byte_command(0xD3, 3) + note(NOTE_C4, 1) + rest(20) + END_TRACK
        archive = sseq_render.read_archive(sound_archive([(SEQUENCE_NAME, payload, 127)], basic_bank(), [pcm16_wave()]))
        sequencer = sseq_render.Sequencer(archive, 0)
        sequencer.run_update()
        channel = next(channel for channel in sequencer.channels if channel.active)
        self.assertEqual((channel.attack, channel.decay, channel.sustain, channel.release), (255, 3, 2, 7))

    def test_a_track_volume_change_does_not_reach_a_note_already_in_release(self):
        bank = bank_file([(sseq_render.KIND_PCM, instrument_parameters(release=100))])
        changed_at = 20
        sequencer = simulate(note(NOTE_C4, 4) + rest(changed_at - 4) + byte_command(0xC1, 0) + rest(800) + END_TRACK, bank=bank)
        self.assertGreater(sequencer.voices[0].end_sample, voice_start_of_tick_update(changed_at * 2) + 2000)

    def test_a_note_ends_when_release_falls_below_the_quietest_volume(self):
        bank = bank_file([(sseq_render.KIND_PCM, instrument_parameters(release=80))])
        sequencer = simulate(note(NOTE_C4, 4) + rest(800) + END_TRACK, bank=bank)
        expected_updates = math.ceil(723 * 128 / sseq_render.decay_rate(80))
        ended_after = sequencer.voices[0].end_sample - voice_start_of_tick_update(8)
        self.assertAlmostEqual(ended_after, expected_updates * 170.5, delta=3 * 171)


class CommandTests(unittest.TestCase):
    def test_a_call_runs_the_routine_and_returns(self):
        intro = offset_command(0x95, 9) + note(NOTE_C4 + 2, 1) + END_TRACK
        self.assertEqual(len(intro), 8)
        payload = intro + bytes([0xFF]) + note(NOTE_C4 + 1, 1) + bytes([0xFD])
        sequencer = simulate(payload, waves=[pcm16_wave(timer=4096)])
        self.assertEqual([voice_timers(voice)[0] for voice in sequencer.voices], [sseq_render.channel_timer(4096, 64), sseq_render.channel_timer(4096, 128)])

    def test_a_loop_command_repeats_its_count(self):
        sequencer = simulate(byte_command(0xD4, 3) + note(NOTE_C4, 1) + bytes([0xFC]) + note(NOTE_C4 + 12, 1) + END_TRACK)
        self.assertEqual(len(sequencer.voices), 4)

    def test_a_loop_end_without_a_loop_start_does_nothing(self):
        sequencer = simulate(bytes([0xFC]) + note(NOTE_C4, 1) + END_TRACK)
        self.assertEqual(len(sequencer.voices), 1)

    def test_transpose_moves_the_key(self):
        sequencer = simulate(byte_command(0xC3, 12) + note(NOTE_C4, 1) + byte_command(0xC3, -12) + note(NOTE_C4, 1) + END_TRACK, waves=[pcm16_wave(timer=4096)])
        self.assertEqual([voice_timers(voice)[0] for voice in sequencer.voices], [2048, 8192])

    def test_pitch_bend_scales_by_the_bend_range(self):
        sequencer = simulate(byte_command(0xC5, 2) + byte_command(0xC4, 64) + note(NOTE_C4, 1) + byte_command(0xC5, 12) + byte_command(0xC4, -128) + note(NOTE_C4, 1) + END_TRACK, waves=[pcm16_wave(timer=4096)])
        self.assertEqual(voice_timers(sequencer.voices[0])[0], sseq_render.channel_timer(4096, 64))
        self.assertEqual(voice_timers(sequencer.voices[1])[0], sseq_render.channel_timer(4096, -768))

    def test_poly_mode_starts_notes_together_and_mono_mode_waits(self):
        poly = simulate(byte_command(0xC7, 0) + note(NOTE_C4, 8) + note(NOTE_C4 + 2, 8) + rest(8) + END_TRACK)
        self.assertEqual(poly.voices[0].start_sample, poly.voices[1].start_sample)
        mono = simulate(note(NOTE_C4, 8) + note(NOTE_C4 + 2, 8) + END_TRACK)
        self.assertEqual(mono.voices[1].start_sample, voice_start_of_tick_update(16))

    def test_a_note_of_length_zero_waits_for_its_channel_to_finish(self):
        short_wave = pcm16_wave(count=64, looped=False, timer=ONE_WAVE_STEP_PER_SAMPLE * 16)
        sequencer = simulate(note(NOTE_C4, 0) + note(NOTE_C4, 1) + END_TRACK, waves=[short_wave])
        self.assertEqual(len(sequencer.voices), 2)
        self.assertGreaterEqual(sequencer.voices[1].start_sample, sequencer.voices[0].end_sample)

    def test_tie_changes_the_sounding_voice_instead_of_starting_one(self):
        sequencer = simulate(byte_command(0xC8, 1) + note(NOTE_C4, 4) + note(NOTE_C4 + 12, 4) + byte_command(0xC8, 0) + rest(4) + END_TRACK, waves=[pcm16_wave(timer=4096)])
        self.assertEqual(len(sequencer.voices), 1)
        self.assertEqual(voice_timers(sequencer.voices[0])[0], 4096)
        self.assertIn(2048, voice_timers(sequencer.voices[0]))

    def test_portamento_glides_from_the_given_key_over_the_note(self):
        payload = byte_command(0xC9, NOTE_C4 + 12) + note(NOTE_C4, 8) + rest(8) + END_TRACK
        timers = voice_timers(simulate(payload, waves=[pcm16_wave(timer=4096)]).voices[0])
        self.assertEqual(timers[0], 2048)
        self.assertGreater(len(set(timers)), 4)
        self.assertEqual(timers, sorted(timers))
        self.assertEqual(timers[-1], 4096)

    def test_portamento_time_sets_the_glide_length(self):
        payload = byte_command(0xC9, NOTE_C4 + 1) + byte_command(0xCF, 8) + note(NOTE_C4, 100) + END_TRACK
        voice = simulate(payload, waves=[pcm16_wave(timer=8192)]).voices[0]
        glide_updates = (8 * 8 * 64) >> 11
        self.assertEqual(glide_updates, 2)
        self.assertEqual(voice_timers(voice)[:3], [sseq_render.channel_timer(8192, 64), sseq_render.channel_timer(8192, 32), 8192])

    def test_portamento_can_be_switched_off(self):
        payload = byte_command(0xC9, NOTE_C4 + 12) + byte_command(0xCE, 0) + note(NOTE_C4, 8) + END_TRACK
        self.assertEqual(voice_timers(simulate(payload, waves=[pcm16_wave(timer=4096)]).voices[0])[0], 4096)

    def test_sweep_starts_away_from_the_pitch_and_settles_on_it(self):
        payload = short_command(0xE3, -768) + note(NOTE_C4, 16) + rest(4) + END_TRACK
        timers = voice_timers(simulate(payload, waves=[pcm16_wave(timer=4096)]).voices[0])
        self.assertGreater(timers[0], 4096)
        self.assertLessEqual(timers[0], 8192)
        self.assertEqual(timers[-1], 4096)

    def test_pitch_modulation_waits_for_its_delay_then_moves_the_timer(self):
        modulation = byte_command(0xCA, 127) + byte_command(0xCB, 64) + byte_command(0xCC, 0) + byte_command(0xCD, 4) + short_command(0xE0, 50)
        voice = simulate(modulation + note(NOTE_C4, 200) + END_TRACK, waves=[pcm16_wave(timer=4096)]).voices[0]
        self.assertEqual(voice_timers(voice)[0], 4096)
        self.assertGreater(voice.changes[1][0], voice.start_sample + 45 * 170)
        self.assertGreater(max(voice_timers(voice)), 4096)
        self.assertLess(min(voice_timers(voice)), 4096)

    def test_volume_modulation_scales_by_60_and_pan_modulation_moves_the_pan(self):
        channel = sseq_render.Channel(0)
        channel.lfo_depth = 127
        channel.lfo_range = 1
        channel.lfo_counter = 32 << 8
        channel.lfo_target = sseq_render.LFO_VOLUME
        self.assertEqual(channel.lfo_value(), (127 * 127 * 60) >> 14)
        channel.lfo_target = sseq_render.LFO_PAN
        self.assertEqual(channel.lfo_value(), (127 * 127 << 6) >> 14)

    def test_pan_commands_reach_the_hardware_gains(self):
        _, left = render(byte_command(0xC0, 0) + note(NOTE_C4, 100) + END_TRACK)
        self.assertEqual(numpy.abs(left[:, 1]).max(), 0)
        self.assertEqual(numpy.abs(left[:, 0]).max(), CONSTANT_LEVEL)
        _, right = render(byte_command(0xC0, 127) + note(NOTE_C4, 100) + END_TRACK)
        self.assertEqual(numpy.abs(right[:, 0]).max(), 0)
        self.assertEqual(numpy.abs(right[:, 1]).max(), CONSTANT_LEVEL)

    def test_instrument_pan_adds_to_track_pan(self):
        bank = bank_file([(sseq_render.KIND_PCM, instrument_parameters(pan=0))])
        _, frames = render(byte_command(0xC0, 32) + note(NOTE_C4, 100) + END_TRACK, bank=bank)
        self.assertEqual(numpy.abs(frames[:, 1]).max(), 0)

    def test_velocity_volume_expression_and_master_each_add_their_square_decibels(self):
        for command in (None, 0xC1, 0xD5, 0xC2):
            with self.subTest(command=command):
                if command is None:
                    payload = note(NOTE_C4, 8, velocity=64) + END_TRACK
                else:
                    payload = byte_command(command, 64) + note(NOTE_C4, 8) + END_TRACK
                sequencer = simulate(payload)
                self.assertEqual(sequencer.voices[0].changes[0][2:], sseq_render.voice_gains(sseq_render.channel_volume_register(-119), 64))

    def test_the_sequence_volume_goes_through_the_plain_decibel_table(self):
        sequencer = simulate(note(NOTE_C4, 8) + END_TRACK, volume=64)
        self.assertEqual(sequencer.external_fader, -60)
        self.assertEqual(sequencer.voices[0].changes[0][2:], sseq_render.voice_gains(sseq_render.channel_volume_register(-60), 64))

    def test_a_sequence_volume_of_zero_is_silent(self):
        _, frames = render(note(NOTE_C4, 8) + END_TRACK, volume=0)
        self.assertEqual(numpy.abs(frames).max(), 0)

    def test_open_track_needs_its_bit_in_the_allocation_mask(self):
        second = note(NOTE_C4 + 7, 1) + END_TRACK
        allowed_prefix = allocate_tracks(0b11) + open_track(1, 12)
        allowed = allowed_prefix + note(NOTE_C4, 1) + END_TRACK
        self.assertEqual(len(allowed), 12)
        self.assertEqual(len(simulate(allowed + second).voices), 2)
        refused = allocate_tracks(0b01) + open_track(1, 12) + note(NOTE_C4, 1) + END_TRACK + second
        self.assertEqual(len(simulate(refused).voices), 1)

    def test_if_prefix_skips_when_the_compare_flag_is_clear(self):
        skipped = short_variable(0xB0, 0, 5) + short_variable(0xB8, 0, 6) + bytes([0xA2]) + note(NOTE_C4, 1) + note(NOTE_C4 + 12, 1) + END_TRACK
        sequencer = simulate(skipped, waves=[pcm16_wave(timer=4096)])
        self.assertEqual([voice_timers(voice)[0] for voice in sequencer.voices], [2048])
        taken = short_variable(0xB0, 0, 5) + short_variable(0xB8, 0, 5) + bytes([0xA2]) + note(NOTE_C4, 1) + END_TRACK
        self.assertEqual(len(simulate(taken).voices), 1)

    def test_variable_prefix_reads_its_argument_from_a_variable(self):
        payload = short_variable(0xB0, 3, 48) + bytes([0xA1, 0x80, 3]) + note(NOTE_C4, 1) + END_TRACK
        self.assertEqual(simulate(payload).voices[0].start_sample, voice_start_of_tick_update(96))

    def test_variable_operations_change_the_variable(self):
        operations = short_variable(0xB0, 2, 6) + short_variable(0xB1, 2, 4) + short_variable(0xB2, 2, 1) + short_variable(0xB3, 2, 4) + short_variable(0xB4, 2, 3) + short_variable(0xB5, 2, 2) + short_variable(0xB5, 2, -1)
        sequencer = simulate(operations + END_TRACK)
        self.assertEqual(sequencer.local_variables[2], 24)

    def test_random_prefix_stays_inside_its_range(self):
        payload = bytes([0xA0, 0x80]) + struct.pack("<hh", 10, 12) + note(NOTE_C4, 1) + END_TRACK
        start = simulate(payload).voices[0].start_sample
        self.assertIn(start, [voice_start_of_tick_update(ticks * 2) for ticks in (10, 11, 12)])

    def test_mute_stops_new_notes_only_until_it_is_lifted(self):
        payload = byte_command(0xD7, 1) + note(NOTE_C4, 1) + byte_command(0xD7, 0) + note(NOTE_C4, 1) + END_TRACK
        self.assertEqual(len(simulate(payload).voices), 1)


def short_variable(command, number, value):
    return bytes([command, number]) + struct.pack("<h", value)


class VoiceLimitTests(unittest.TestCase):
    def voices_at(self, sequencer, sample):
        return [voice for voice in sequencer.voices if voice.start_sample <= sample < voice.end_sample]

    def test_never_more_than_16_voices_sound_together(self):
        notes = b"".join(note(NOTE_C4, 100) + rest(1) for _ in range(20))
        sequencer = simulate(byte_command(0xC7, 0) + notes + rest(100) + END_TRACK)
        self.assertEqual(len(sequencer.voices), 20)
        self.assertEqual(len(self.voices_at(sequencer, voice_start_of_tick_update(48))), 16)

    def test_a_lower_priority_note_cannot_take_a_channel_from_higher_ones(self):
        loud = byte_command(0xC6, 100) + b"".join(note(NOTE_C4, 100) + rest(1) for _ in range(16))
        quiet = byte_command(0xC6, 10) + note(NOTE_C4 + 12, 40)
        sequencer = simulate(byte_command(0xC7, 0) + loud + quiet + rest(100) + END_TRACK)
        self.assertEqual(len(sequencer.voices), 16)

    def test_a_higher_priority_note_takes_a_channel(self):
        quiet = byte_command(0xC6, 10) + b"".join(note(NOTE_C4, 100) + rest(1) for _ in range(16))
        loud = byte_command(0xC6, 100) + note(NOTE_C4 + 12, 40)
        sequencer = simulate(byte_command(0xC7, 0) + quiet + loud + rest(100) + END_TRACK)
        self.assertEqual(len(sequencer.voices), 17)
        self.assertEqual(len(self.voices_at(sequencer, voice_start_of_tick_update(38))), 16)

    def test_the_first_channels_taken_follow_the_driver_order(self):
        archive = sseq_render.read_archive(sound_archive([(SEQUENCE_NAME, byte_command(0xC7, 0) + note(NOTE_C4, 8) + note(NOTE_C4, 8) + note(NOTE_C4, 8) + rest(8) + END_TRACK, 127)], basic_bank(), [pcm16_wave()]))
        sequencer = sseq_render.Sequencer(archive, 0)
        sequencer.run_update()
        self.assertEqual([channel.index for channel in sequencer.channels if channel.active], [4, 5, 6])

    def test_a_player_channel_mask_limits_the_channels(self):
        notes = b"".join(note(NOTE_C4, 40) for _ in range(4))
        sequencer = simulate(byte_command(0xC7, 0) + notes + rest(100) + END_TRACK, channel_mask=0b11)
        self.assertEqual(len(self.voices_at(sequencer, voice_start_of_tick_update(10))), 2)


if __name__ == "__main__":
    unittest.main()
