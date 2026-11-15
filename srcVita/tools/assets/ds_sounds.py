import struct
import subprocess

import audio_file
import bake_record
import chapter_assets
import ds_music
import ds_rom
import sseq_render

ARCHIVE_PATH = "sound_data.sdat"
SOUNDS_FOLDER = "sounds"
CODE_FILES = ["audio_file.py", "ds_music.py", "ds_sounds.py", "sseq_render.py"]
SEQUENCE_ARCHIVE_TABLE = 1
SEQUENCE_ARCHIVE_MAGIC = b"SSAR"
SEQUENCE_ARCHIVE_DATA_OFFSET_FIELD = 0x18
SEQUENCE_ARCHIVE_COUNT_FIELD = 0x1C
SEQUENCE_ARCHIVE_ENTRY_LIST = 0x20
SEQUENCE_ARCHIVE_ENTRY_SIZE = 12
EMPTY_ENTRY_OFFSET = 0xFFFFFFFF


def sound_path(name, suffix):
    return SOUNDS_FOLDER + "/" + name + suffix


def sequence_archive_file(archive, group):
    offset = sseq_render.info_entry_offset(archive, SEQUENCE_ARCHIVE_TABLE, group)
    if offset is None:
        return None
    data = sseq_render.read_archive_file(archive, struct.unpack_from("<I", archive["data"], offset)[0])
    if data[:len(SEQUENCE_ARCHIVE_MAGIC)] != SEQUENCE_ARCHIVE_MAGIC:
        return None
    return data


def sequence_archive_entry(archive, group, index):
    data = sequence_archive_file(archive, group)
    if data is None or index < 0 or index >= struct.unpack_from("<I", data, SEQUENCE_ARCHIVE_COUNT_FIELD)[0]:
        return None
    offset, bank, volume, channel_priority, player_priority, player = struct.unpack_from("<IHBBBB", data, SEQUENCE_ARCHIVE_ENTRY_LIST + SEQUENCE_ARCHIVE_ENTRY_SIZE * index)
    if offset == EMPTY_ENTRY_OFFSET:
        return None
    if sseq_render.info_entry_offset(archive, sseq_render.BANK_TABLE, bank) is None:
        return None
    return {"offset": offset, "bank": bank, "volume": volume, "channel_priority": channel_priority, "player_priority": player_priority, "player": player, "data": data[struct.unpack_from("<I", data, SEQUENCE_ARCHIVE_DATA_OFFSET_FIELD)[0]:]}


class SequenceArchiveSequencer(sseq_render.Sequencer):
    def __init__(self, archive, entry):
        self.archive = archive
        self.data = entry["data"]
        self.bank = sseq_render.read_bank(archive, entry["bank"])
        self.instruments = {}
        self.waves = {}
        self.wave_archive_files = {}
        self.channel_mask = sseq_render.player_channel_mask(archive, entry["player"]) or 0xFFFF
        self.channels = [sseq_render.Channel(index) for index in range(sseq_render.CHANNEL_COUNT)]
        self.tracks = [None] * sseq_render.TRACK_COUNT
        self.tempo = 120
        self.tempo_counter = sseq_render.TICK_COST
        self.ticks_left = 0
        self.player_volume = 127
        self.external_fader = sseq_render.DECIBEL_TABLE[min(entry["volume"], 127)]
        self.local_variables = [-1] * sseq_render.VARIABLE_COUNT
        self.global_variables = [-1] * sseq_render.VARIABLE_COUNT
        self.random_state = sseq_render.RANDOM_START
        self.playing = True
        self.update_index = 0
        self.cycles_done = 0
        self.samples_done = 0
        self.voices = []
        self.one_shot_voices = []
        self.seen_states = {}
        self.loop_start_update = None
        self.loop_end_update = None
        self.loop_start_sample = None
        self.loop_end_sample = None
        self.cycles_past_sample_at_loop_start = 0
        self.start_offset = entry["offset"]
        self.prepare_tracks()

    def prepare_tracks(self):
        first = sseq_render.Track(self.channel_mask)
        first.position = self.start_offset
        self.tracks[0] = first
        if self.read_byte(first) != sseq_render.COMMAND_ALLOCATE_TRACKS:
            first.position = self.start_offset
            return
        mask = self.read_short(first) >> 1
        for number in range(1, sseq_render.TRACK_COUNT):
            if mask & 1:
                self.tracks[number] = sseq_render.Track(self.channel_mask)
            mask >>= 1

    def run(self):
        updates_after_end = 0
        while self.update_index < sseq_render.LONGEST_RENDER_UPDATES:
            if not self.playing:
                if updates_after_end >= sseq_render.LONGEST_RELEASE_UPDATES or not any(channel.active or channel.start_requested or channel.stop_requested for channel in self.channels):
                    break
                updates_after_end += 1
            self.run_update()
            if self.update_index == self.loop_end_update:
                break
            if self.loop_start_update is None and self.playing and self.hold_loop():
                break
        for voice in self.voices:
            voice.stop(self.samples_done)

    def hold_loop(self):
        held = held_voices(self)
        if not held:
            return False
        starts = []
        periods = []
        for voice, first_step, period_steps in held:
            starts.append(first_periodic_sample(voice, first_step))
            periods.append(period_steps * voice.timer / sseq_render.TIMER_STEPS_PER_SAMPLE)
        period = max(1, int(round(max(periods))))
        loop_start = max(max(starts), self.samples_done - period)
        self.loop_start_sample = loop_start
        self.loop_end_sample = loop_start + period
        self.samples_done = self.loop_end_sample
        return True


def channel_is_held(channel):
    voice = channel.voice
    if not channel.active or channel.start_pending or channel.start_requested or channel.stop_requested:
        return False
    if voice is None or not voice.running or channel.length != -1 or channel.envelope_stage != sseq_render.ENVELOPE_SUSTAIN:
        return False
    if channel.lfo_depth != 0 or (channel.sweep_pitch != 0 and channel.sweep_counter < channel.sweep_length):
        return False
    if voice.registers != (channel.timer_register, channel.volume_register, channel.pan_register):
        return False
    return periodic_steps(voice) is not None


def periodic_steps(voice):
    if voice.kind == sseq_render.KIND_PCM:
        if not voice.wave["looped"] or voice.wave["loop_length"] <= 0:
            return None
        return voice.wave["loop_start"] + voice.wave["delay"], voice.wave["loop_length"]
    if voice.kind == sseq_render.KIND_PSG:
        return 1, 8
    return 1, sseq_render.NOISE_PERIOD


def held_voices(sequencer):
    live_tracks = [track for track in sequencer.tracks if track is not None and track.position is not None]
    if not live_tracks:
        return []
    for track in live_tracks:
        if not track.note_finish_wait or not track.channels:
            return []
    held = []
    for channel in sequencer.channels:
        if channel.active:
            if not channel_is_held(channel):
                return []
            first_step, period_steps = periodic_steps(channel.voice)
            held.append((channel.voice, first_step, period_steps))
        elif channel.voice is not None and channel.voice.running:
            return []
    for voice in sequencer.one_shot_voices:
        if voice.running:
            return []
    return held


def steps_played_by(voice, sample):
    steps = 0.0
    changes = voice.changes
    for number, change in enumerate(changes):
        segment_end = changes[number + 1][0] if number + 1 < len(changes) else sample
        segment_end = min(segment_end, sample)
        if segment_end > change[0]:
            steps += (segment_end - change[0]) * sseq_render.TIMER_STEPS_PER_SAMPLE / change[1]
    return steps


def first_periodic_sample(voice, first_step):
    steady_from = voice.changes[-1][0]
    missing = first_step - steps_played_by(voice, steady_from)
    if missing <= 0:
        return steady_from
    return steady_from + int(-(-missing * voice.timer // sseq_render.TIMER_STEPS_PER_SAMPLE)) + 1


def simulate_sound(sdat_bytes, group, index):
    archive = sseq_render.read_archive(sdat_bytes)
    entry = sequence_archive_entry(archive, group, index)
    if entry is None:
        raise ValueError("the sound archive has no sequence-archive entry %d in group %d" % (index, group))
    sequencer = SequenceArchiveSequencer(archive, entry)
    sequencer.run()
    return sequencer


def render_sound(sdat_bytes, group, index):
    sequencer = simulate_sound(sdat_bytes, group, index)
    has_loop = sequencer.loop_end_sample is not None
    return {
        "samples": sseq_render.mix_voices(sequencer.voices, sequencer.samples_done),
        "sample_rate": sseq_render.SAMPLE_RATE,
        "loop_start": sequencer.loop_start_sample if has_loop else None,
        "loop_end": sequencer.loop_end_sample if has_loop else None,
    }


def sound_row_of_kind(arm9, kind):
    if kind < 0 or kind >= chapter_assets.SOUND_ROW_COUNT:
        return None
    index, group, _, _ = struct.unpack_from("<hhHH", arm9, chapter_assets.SOUND_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.SOUND_ROW_SIZE * kind)
    if group not in chapter_assets.SOUND_GROUP_NAMES:
        return None
    return {"name": "%s_%03d" % (chapter_assets.SOUND_GROUP_NAMES[group], index), "group": group, "index": index}


def sound_rows(sdat_bytes, kinds, arm9):
    archive = sseq_render.read_archive(sdat_bytes)
    sounds = {}
    sound_kinds = {}
    for kind in kinds:
        row = sound_row_of_kind(arm9, kind)
        if row is None or sequence_archive_entry(archive, row["group"], row["index"]) is None:
            continue
        sounds[row["name"]] = row
        sound_kinds[str(kind)] = row["name"]
    return list(sounds.values()), sound_kinds


def sound_rows_of_scripts(sdat_bytes, scripts, arm9):
    return sound_rows(sdat_bytes, chapter_assets.sound_kinds_used(scripts), arm9)


def bytes_a_sound_reads(archive, entry):
    bank = sseq_render.read_bank(archive, entry["bank"])
    parts = [entry["data"], struct.pack("<IBB", entry["offset"], entry["volume"], entry["player"]), bank["data"]]
    for wave_archive in bank["wave_archives"]:
        offset = None if wave_archive == sseq_render.NO_WAVE_ARCHIVE else sseq_render.info_entry_offset(archive, sseq_render.WAVE_ARCHIVE_TABLE, wave_archive)
        parts.append(b"" if offset is None else sseq_render.read_archive_file(archive, struct.unpack_from("<I", archive["data"], offset)[0]))
    parts.append(struct.pack("<H", sseq_render.player_channel_mask(archive, entry["player"])))
    return b"".join(struct.pack("<I", len(part)) + part for part in parts)


def loop_points_of(rendered):
    if rendered["loop_start"] is None:
        return None
    return [ds_music.frames_at_playback_rate(rendered["loop_start"], rendered["sample_rate"]), ds_music.frames_at_playback_rate(rendered["loop_end"], rendered["sample_rate"]) - 1]


def bake_sound(sdat_bytes, archive, row, record, code):
    relative_path = sound_path(row["name"], ".ogg")
    entry = sequence_archive_entry(archive, row["group"], row["index"])
    key = record.key_for(code, {"group": row["group"], "index": row["index"]}, bytes_a_sound_reads(archive, entry))
    ogg = record.read_if_current(relative_path, key)
    loop_points = record.details_of(relative_path).get("loop_points")
    if ogg is None:
        rendered = render_sound(sdat_bytes, row["group"], row["index"])
        ogg = ds_music.encode_ogg(rendered["samples"], rendered["sample_rate"])
        loop_points = loop_points_of(rendered)
        record.store(relative_path, key, ogg, {"loop_points": loop_points})
    return ogg, loop_points


def code_fingerprint():
    return bake_record.code_fingerprint(CODE_FILES, subprocess.run(["ffmpeg", "-version"], capture_output=True).stdout.decode("utf-8", "replace"))


def sound_files(sdat_bytes, sounds, record, code=None):
    archive = sseq_render.read_archive(sdat_bytes)
    code = code_fingerprint() if code is None else code
    files = {}
    for row in sounds:
        ogg, loop_points = bake_sound(sdat_bytes, archive, row, record, code)
        files[sound_path(row["name"], ".ogg")] = ogg
        if loop_points is not None:
            files[sound_path(row["name"], ".loop")] = audio_file.loop_text(tuple(loop_points), ogg).encode("ascii")
    return files


def read_sound_archive(rom_path, archive_path=ARCHIVE_PATH):
    with ds_rom.DsRom(rom_path) as rom:
        return rom.read_file(archive_path)


def sound_output_files(rom_path, scripts, arm9, record, archive_path=ARCHIVE_PATH):
    sdat_bytes = read_sound_archive(rom_path, archive_path)
    sounds, sound_kinds = sound_rows_of_scripts(sdat_bytes, scripts, arm9)
    return sound_files(sdat_bytes, sounds, record), sounds, sound_kinds
