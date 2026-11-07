import math
import struct

import numpy

SAMPLE_RATE = 32728
CYCLES_PER_UPDATE = 64 * 2728
CYCLES_PER_SAMPLE = 1024
TIMER_STEPS_PER_SAMPLE = 512
TICK_COST = 240
LONGEST_RENDER_UPDATES = 20 * 60 * 192
LONGEST_RELEASE_UPDATES = 60 * 192
MOST_COMMANDS_IN_A_TICK = 65536

SDAT_MAGIC = b"SDAT"
SECTION_TABLE_OFFSET = 16
SEQUENCE_TABLE = 0
BANK_TABLE = 2
WAVE_ARCHIVE_TABLE = 3
PLAYER_TABLE = 4
SEQUENCE_DATA_OFFSET_FIELD = 0x18
ENTRY_COUNT_OFFSET = 0x38
ENTRY_LIST_OFFSET = 0x3C
WAVE_HEADER_SIZE = 12
INSTRUMENT_ENTRY_SIZE = 12
FILE_ID_MASK = 0xFFFFFF
NO_WAVE_ARCHIVE = 0xFFFF

FORMAT_PCM8 = 0
FORMAT_PCM16 = 1
FORMAT_ADPCM = 2
PCM_START_DELAY = 3
ADPCM_START_DELAY = 11
ADPCM_HEADER_NIBBLES = 8
ADPCM_LIMIT = 0x7FFF
ADPCM_BLOCK_NIBBLES = 4096

KIND_PCM = 1
KIND_PSG = 2
KIND_NOISE = 3
KIND_DRUM_SET = 16
KIND_KEY_SPLIT = 17
CHANNEL_MASK_OF_KIND = {KIND_PCM: 0xFFFF, KIND_PSG: 0x3F00, KIND_NOISE: 0xC000}
BASE_TIMER_OF_TONE_GENERATOR = 8006
PSG_TIMER_MASK = 0xFFFC
CHANNEL_ALLOCATION_ORDER = (4, 5, 6, 7, 2, 0, 3, 1, 8, 9, 10, 11, 14, 12, 15, 13)
CHANNEL_COUNT = 16
TRACK_COUNT = 16
VARIABLE_COUNT = 16

SILENT_DECIBELS = -32768
QUIETEST_SQUARE_DECIBELS = -722
QUIETEST_CHANNEL_DECIBELS = -723
ENVELOPE_START = -92544
ENVELOPE_FRACTION_BITS = 7
ENVELOPE_ATTACK = 0
ENVELOPE_DECAY = 1
ENVELOPE_SUSTAIN = 2
ENVELOPE_RELEASE = 3
ATTACK_RATES_ABOVE_108 = (0, 1, 5, 14, 26, 38, 51, 63, 73, 84, 92, 100, 109, 116, 123, 127, 132, 137, 143)
INSTANT_ATTACK_RATE = 0
FASTEST_DECAY = 0xFFFF

LFO_PITCH = 0
LFO_VOLUME = 1
LFO_PAN = 2
PITCH_STEPS_PER_SEMITONE = 64
PITCH_STEPS_PER_OCTAVE = 768
SLOWEST_TIMER = 0xFFFF
FASTEST_TIMER = 0x10
VOLUME_DIVIDERS = (1, 2, 4, 16)
VOLUME_LOUDNESS_SHIFTS = (0, 1, 2, 4)
VOLUME_MIX_SHIFTS = (4, 3, 2, 0)
TOP_OF_VOLUME_RANGES_ROUNDED_DOWN = (-121, -61, -1)
CENTER_PAN = 64
HARDWARE_FULL_SCALE = 128

ARGUMENT_BYTE = 0
ARGUMENT_SHORT = 1
ARGUMENT_VARIABLE_LENGTH = 2
ARGUMENT_RANDOM = 3
ARGUMENT_FROM_VARIABLE = 4
PREFIX_RANDOM = 0xA0
PREFIX_FROM_VARIABLE = 0xA1
PREFIX_IF = 0xA2
COMMAND_ALLOCATE_TRACKS = 0xFE
END_OF_DATA = 0xFF
CALL_STACK_SIZE = 3
RANDOM_MULTIPLIER = 0x19660D
RANDOM_INCREMENT = 0x3C6EF35F
RANDOM_START = 0x12345678

IMA_STEP_SIZES = (
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166,
    1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845,
    8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767,
)
IMA_INDEX_CHANGES = (-1, -1, -1, -1, 2, 4, 6, 8)
NOISE_PERIOD = 32767


def decibel_table():
    return [SILENT_DECIBELS] + [round(200 * math.log10(step / 127)) for step in range(1, 128)]


def decibel_square_table():
    return [SILENT_DECIBELS] + [max(QUIETEST_SQUARE_DECIBELS, round(400 * math.log10(step / 127))) for step in range(1, 128)]


def volume_divider_code(decibels):
    if decibels < -240:
        return 3
    if decibels < -120:
        return 2
    if decibels < -60:
        return 1
    return 0


def channel_volume_table():
    table = []
    for decibels in range(QUIETEST_CHANNEL_DECIBELS, 1):
        level = round(HARDWARE_FULL_SCALE * VOLUME_DIVIDERS[volume_divider_code(decibels)] * 10 ** (decibels / 200))
        table.append(min(level, 126 if decibels in TOP_OF_VOLUME_RANGES_ROUNDED_DOWN else 127))
    return table


def pitch_table():
    return [round(65536 * (2 ** (step / PITCH_STEPS_PER_OCTAVE) - 1)) for step in range(PITCH_STEPS_PER_OCTAVE)]


def lfo_sine_table():
    quarter = [round(127 * math.sin(step * math.pi / 64)) for step in range(33)]
    table = []
    for step in range(128):
        if step < 32:
            table.append(quarter[step])
        elif step < 64:
            table.append(quarter[64 - step])
        elif step < 96:
            table.append(-quarter[step - 64])
        else:
            table.append(-quarter[32 - (step - 96)])
    return table


def duty_cycle_table():
    rows = [[-ADPCM_LIMIT] * (7 - duty) + [ADPCM_LIMIT] * (duty + 1) for duty in range(7)]
    rows.append([-ADPCM_LIMIT] * 8)
    return numpy.array(rows, dtype=numpy.int64)


def noise_table():
    state = 0x7FFF
    levels = []
    for _ in range(NOISE_PERIOD):
        if state & 1:
            state = (state >> 1) ^ 0x6000
            levels.append(-ADPCM_LIMIT)
        else:
            state >>= 1
            levels.append(ADPCM_LIMIT)
    return numpy.array(levels, dtype=numpy.int64)


def ima_next_index_table():
    table = []
    for index in range(len(IMA_STEP_SIZES)):
        for change in IMA_INDEX_CHANGES + IMA_INDEX_CHANGES:
            table.append(min(max(index + change, 0), len(IMA_STEP_SIZES) - 1))
    return table


DECIBEL_TABLE = decibel_table()
DECIBEL_SQUARE_TABLE = decibel_square_table()
CHANNEL_VOLUME_TABLE = channel_volume_table()
PITCH_TABLE = pitch_table()
LFO_SINE_TABLE = lfo_sine_table()
DUTY_CYCLE_TABLE = duty_cycle_table()
NOISE_TABLE = noise_table()
IMA_NEXT_INDEX_TABLE = ima_next_index_table()
IMA_STEP_SIZE_ARRAY = numpy.array(IMA_STEP_SIZES, dtype=numpy.int64)


def wrap_signed_byte(value):
    return ((value + 128) & 0xFF) - 128


def wrap_signed_short(value):
    return ((value + 32768) & 0xFFFF) - 32768


def attack_rate(attack):
    if attack < 109:
        return 255 - attack
    return ATTACK_RATES_ABOVE_108[127 - attack]


def decay_rate(decay):
    if decay == 127:
        return FASTEST_DECAY
    if decay == 126:
        return 0x3C00
    if decay < 50:
        return decay * 2 + 1
    return 0x1E00 // (126 - decay)


def channel_volume_register(decibels):
    decibels = min(max(decibels, QUIETEST_CHANNEL_DECIBELS), 0)
    return CHANNEL_VOLUME_TABLE[decibels - QUIETEST_CHANNEL_DECIBELS] | (volume_divider_code(decibels) << 8)


def channel_timer(base_timer, pitch):
    octave, step = divmod(-pitch, PITCH_STEPS_PER_OCTAVE)
    timer = base_timer * (PITCH_TABLE[step] + 0x10000)
    shift = octave - 16
    if shift <= 0:
        timer >>= -shift
    elif shift >= 32:
        return SLOWEST_TIMER
    else:
        timer <<= shift
    return min(max(timer, FASTEST_TIMER), SLOWEST_TIMER)


def voice_gains(volume_register, pan_register):
    level = volume_register & 0xFF
    if level == 127:
        level = HARDWARE_FULL_SCALE
    shift = VOLUME_MIX_SHIFTS[volume_register >> 8]
    pan = HARDWARE_FULL_SCALE if pan_register == 127 else pan_register
    return ((HARDWARE_FULL_SCALE - pan) * level) << shift, (pan * level) << shift


def channel_loudness(volume_register):
    return ((volume_register & 0xFF) << 4) >> VOLUME_LOUDNESS_SHIFTS[volume_register >> 8]


def read_archive(sdat_bytes):
    data = bytes(sdat_bytes)
    if data[:len(SDAT_MAGIC)] != SDAT_MAGIC:
        raise ValueError("the sound archive does not start with SDAT")
    sections = struct.unpack_from("<8I", data, SECTION_TABLE_OFFSET)
    archive = {"data": data, "symbol_offset": sections[0], "info_offset": sections[2], "file_table_offset": sections[4]}
    archive["info_tables"] = struct.unpack_from("<8I", data, archive["info_offset"] + 8)
    return archive


def info_entry_offset(archive, table, index):
    data = archive["data"]
    table_offset = archive["info_offset"] + archive["info_tables"][table]
    if index >= struct.unpack_from("<I", data, table_offset)[0]:
        return None
    entry = struct.unpack_from("<I", data, table_offset + 4 + 4 * index)[0]
    if entry == 0:
        return None
    return archive["info_offset"] + entry


def info_entry_count(archive, table):
    return struct.unpack_from("<I", archive["data"], archive["info_offset"] + archive["info_tables"][table])[0]


def read_archive_file(archive, file_id):
    offset, size = struct.unpack_from("<II", archive["data"], archive["file_table_offset"] + 12 + 16 * (file_id & FILE_ID_MASK))
    return archive["data"][offset:offset + size]


def sequence_symbol(archive, index):
    if archive["symbol_offset"] == 0:
        return None
    data = archive["data"]
    table_offset = archive["symbol_offset"] + struct.unpack_from("<I", data, archive["symbol_offset"] + 8)[0]
    if index >= struct.unpack_from("<I", data, table_offset)[0]:
        return None
    name_offset = struct.unpack_from("<I", data, table_offset + 4 + 4 * index)[0]
    if name_offset == 0:
        return None
    start = archive["symbol_offset"] + name_offset
    return data[start:data.index(b"\x00", start)].decode("ascii", "replace")


def named_sequences(archive):
    named = {}
    for index in range(info_entry_count(archive, SEQUENCE_TABLE)):
        if info_entry_offset(archive, SEQUENCE_TABLE, index) is not None:
            named[sequence_symbol(archive, index) or "SEQ_%d" % index] = index
    return named


def read_sequence_info(archive, index):
    file_id, bank, volume, channel_priority, player_priority, player = struct.unpack_from("<IHBBBB", archive["data"], info_entry_offset(archive, SEQUENCE_TABLE, index))
    return {"file_id": file_id, "bank": bank, "volume": volume, "channel_priority": channel_priority, "player_priority": player_priority, "player": player}


def read_bank(archive, bank_index):
    offset = info_entry_offset(archive, BANK_TABLE, bank_index)
    if offset is None:
        raise ValueError("the sound archive has no bank %d" % bank_index)
    file_id, wave_archives = struct.unpack_from("<I", archive["data"], offset)[0], struct.unpack_from("<4H", archive["data"], offset + 4)
    return {"data": read_archive_file(archive, file_id), "wave_archives": wave_archives}


def player_channel_mask(archive, player):
    offset = info_entry_offset(archive, PLAYER_TABLE, player)
    if offset is None:
        return 0
    return struct.unpack_from("<H", archive["data"], offset + 2)[0]


def sequence_payload(sequence_file):
    return sequence_file[struct.unpack_from("<I", sequence_file, SEQUENCE_DATA_OFFSET_FIELD)[0]:]


def instrument_at(bank_data, kind, offset):
    wave, wave_archive, root_key, attack, decay, sustain, release, pan = struct.unpack_from("<HHBBBBBB", bank_data, offset)
    return {"kind": kind, "wave": wave, "wave_archive": wave_archive, "root_key": root_key, "attack": attack, "decay": decay, "sustain": sustain, "release": release, "pan": pan}


def read_instrument(bank_data, program, key):
    if program >= struct.unpack_from("<I", bank_data, ENTRY_COUNT_OFFSET)[0]:
        return None
    record = struct.unpack_from("<I", bank_data, ENTRY_LIST_OFFSET + 4 * program)[0]
    kind = record & 0xFF
    offset = record >> 8
    if kind in CHANNEL_MASK_OF_KIND:
        return instrument_at(bank_data, kind, offset)
    if kind == KIND_DRUM_SET:
        lowest, highest = bank_data[offset], bank_data[offset + 1]
        if key < lowest or key > highest:
            return None
        entry = offset + 2 + (key - lowest) * INSTRUMENT_ENTRY_SIZE
    elif kind == KIND_KEY_SPLIT:
        for region in range(8):
            if key <= bank_data[offset + region]:
                break
        else:
            return None
        entry = offset + 8 + region * INSTRUMENT_ENTRY_SIZE
    else:
        return None
    if bank_data[entry] not in CHANNEL_MASK_OF_KIND:
        return None
    return instrument_at(bank_data, bank_data[entry], entry + 2)


def ima_step_sizes(nibbles, step_index):
    next_index = IMA_NEXT_INDEX_TABLE
    indices = []
    append = indices.append
    for nibble in nibbles.tolist():
        append(step_index)
        step_index = next_index[step_index * 16 + nibble]
    return IMA_STEP_SIZE_ARRAY[numpy.array(indices, dtype=numpy.int64)]


def limited_running_sum(start, changes):
    levels = numpy.empty(len(changes), dtype=numpy.int64)
    level = start
    position = 0
    while position < len(changes):
        running = level + numpy.cumsum(changes[position:position + ADPCM_BLOCK_NIBBLES])
        beyond = numpy.flatnonzero(numpy.abs(running) > ADPCM_LIMIT)
        stop = int(beyond[0]) if beyond.size else len(running) - 1
        level = min(max(int(running[stop]), -ADPCM_LIMIT), ADPCM_LIMIT)
        levels[position:position + stop] = running[:stop]
        levels[position + stop] = level
        position += stop + 1
    return levels


def decode_ima_adpcm(body):
    if len(body) < 4:
        return numpy.zeros(0, dtype=numpy.int64)
    start_level, header_index = struct.unpack_from("<hH", body, 0)
    packed = numpy.frombuffer(body, dtype=numpy.uint8, offset=4)
    nibbles = numpy.empty(len(packed) * 2, dtype=numpy.int64)
    nibbles[0::2] = packed & 15
    nibbles[1::2] = packed >> 4
    steps = ima_step_sizes(nibbles, min(header_index & 0x7F, len(IMA_STEP_SIZES) - 1))
    sizes = (steps >> 3) + (steps >> 2) * (nibbles & 1) + (steps >> 1) * ((nibbles >> 1) & 1) + steps * ((nibbles >> 2) & 1)
    return limited_running_sum(start_level, numpy.where(nibbles & 8, -sizes, sizes))


def decode_wave(wave_archive_data, index):
    if index >= struct.unpack_from("<I", wave_archive_data, ENTRY_COUNT_OFFSET)[0]:
        return None
    offset = struct.unpack_from("<I", wave_archive_data, ENTRY_LIST_OFFSET + 4 * index)[0]
    if offset == 0:
        return None
    wave_format, looped, _, timer, loop_words, length_words = struct.unpack_from("<BBHHHI", wave_archive_data, offset)
    body = wave_archive_data[offset + WAVE_HEADER_SIZE:offset + WAVE_HEADER_SIZE + (loop_words + length_words) * 4]
    if wave_format == FORMAT_PCM8:
        levels = numpy.frombuffer(body, dtype=numpy.int8).astype(numpy.int64) << 8
        delay, loop_start, loop_length = PCM_START_DELAY, loop_words * 4, length_words * 4
    elif wave_format == FORMAT_PCM16:
        levels = numpy.frombuffer(body[:len(body) & ~1], dtype="<i2").astype(numpy.int64)
        delay, loop_start, loop_length = PCM_START_DELAY, loop_words * 2, length_words * 2
    elif wave_format == FORMAT_ADPCM:
        levels = decode_ima_adpcm(body)
        delay, loop_start, loop_length = ADPCM_START_DELAY, max(loop_words * 8 - ADPCM_HEADER_NIBBLES, 0), length_words * 8
    else:
        return None
    loop_length = min(loop_length, len(levels) - loop_start)
    return {"levels": levels, "timer": timer, "delay": delay, "looped": bool(looped) and loop_length > 0, "loop_start": loop_start, "loop_length": loop_length}


class Track:
    __slots__ = (
        "position", "wait", "note_wait", "muted", "tie", "note_finish_wait", "portamento", "compare_flag", "stack_depth",
        "return_positions", "loop_counts", "program", "priority", "volume", "expression", "pan", "pitch_bend", "bend_range",
        "transpose", "portamento_key", "portamento_time", "sweep_pitch", "attack", "decay", "sustain", "release", "lfo_target",
        "lfo_speed", "lfo_depth", "lfo_range", "lfo_delay", "channel_mask", "channels",
    )

    def __init__(self, channel_mask):
        self.position = None
        self.wait = 0
        self.note_wait = True
        self.muted = False
        self.tie = False
        self.note_finish_wait = False
        self.portamento = False
        self.compare_flag = True
        self.stack_depth = 0
        self.return_positions = [0] * CALL_STACK_SIZE
        self.loop_counts = [0] * CALL_STACK_SIZE
        self.program = 0
        self.priority = 64
        self.volume = 127
        self.expression = 127
        self.pan = 0
        self.pitch_bend = 0
        self.bend_range = 2
        self.transpose = 0
        self.portamento_key = 60
        self.portamento_time = 0
        self.sweep_pitch = 0
        self.attack = 0xFF
        self.decay = 0xFF
        self.sustain = 0xFF
        self.release = 0xFF
        self.lfo_target = LFO_PITCH
        self.lfo_speed = 16
        self.lfo_depth = 0
        self.lfo_range = 1
        self.lfo_delay = 0
        self.channel_mask = channel_mask
        self.channels = []

    def state(self):
        depth = self.stack_depth
        return (
            self.position, self.wait, self.note_wait, self.muted, self.tie, self.note_finish_wait, self.portamento, self.compare_flag,
            depth, tuple(self.return_positions[:depth]), tuple(self.loop_counts[:depth]), self.program, self.priority, self.volume,
            self.expression, self.pan, self.pitch_bend, self.bend_range, self.transpose, self.portamento_key if self.portamento else None, self.portamento_time,
            self.sweep_pitch, self.attack, self.decay, self.sustain, self.release, self.lfo_target, self.lfo_speed, self.lfo_depth,
            self.lfo_range, self.lfo_delay,
        )


class Channel:
    __slots__ = (
        "index", "kind", "wave", "duty", "base_timer", "owner", "active", "start_pending", "stop_requested", "start_requested",
        "auto_sweep", "key", "root_key", "velocity", "initial_pan", "fader", "track_volume", "track_pitch", "track_pan", "attenuation",
        "envelope_stage", "attack", "decay", "sustain", "release", "priority", "volume_register", "timer_register", "pan_register",
        "length", "sweep_pitch", "sweep_length", "sweep_counter", "lfo_target", "lfo_speed", "lfo_depth", "lfo_range", "lfo_delay",
        "lfo_delay_counter", "lfo_counter", "voice",
    )

    def __init__(self, index):
        self.index = index
        self.kind = KIND_PCM
        self.wave = None
        self.duty = 0
        self.base_timer = 0
        self.owner = None
        self.active = False
        self.start_pending = False
        self.stop_requested = False
        self.start_requested = False
        self.auto_sweep = True
        self.key = 60
        self.root_key = 60
        self.velocity = 127
        self.initial_pan = 0
        self.fader = 0
        self.track_volume = 0
        self.track_pitch = 0
        self.track_pan = 0
        self.attenuation = ENVELOPE_START
        self.envelope_stage = ENVELOPE_ATTACK
        self.attack = INSTANT_ATTACK_RATE
        self.decay = FASTEST_DECAY
        self.sustain = 127
        self.release = FASTEST_DECAY
        self.priority = 0
        self.volume_register = 0
        self.timer_register = 0
        self.pan_register = 0
        self.length = 0
        self.sweep_pitch = 0
        self.sweep_length = 0
        self.sweep_counter = 0
        self.lfo_target = LFO_PITCH
        self.lfo_speed = 16
        self.lfo_depth = 0
        self.lfo_range = 1
        self.lfo_delay = 0
        self.lfo_delay_counter = 0
        self.lfo_counter = 0
        self.voice = None

    def prepare_for_owner(self, owner, priority):
        self.owner = owner
        self.stop_requested = True
        self.start_requested = False
        self.active = False
        self.start_pending = False
        self.length = 0
        self.priority = priority
        self.volume_register = 127
        self.auto_sweep = True
        self.key = 60
        self.root_key = 60
        self.velocity = 127
        self.initial_pan = 0
        self.fader = 0
        self.track_volume = 0
        self.track_pitch = 0
        self.track_pan = 0
        self.sweep_pitch = 0
        self.sweep_length = 0
        self.sweep_counter = 0
        self.attack = INSTANT_ATTACK_RATE
        self.decay = FASTEST_DECAY
        self.sustain = 127
        self.release = FASTEST_DECAY
        self.lfo_target = LFO_PITCH
        self.lfo_speed = 16
        self.lfo_depth = 0
        self.lfo_range = 1
        self.lfo_delay = 0

    def start(self, kind, wave, duty, base_timer, length):
        self.kind = kind
        self.wave = wave
        self.duty = duty
        self.base_timer = base_timer
        self.attenuation = ENVELOPE_START
        self.envelope_stage = ENVELOPE_ATTACK
        self.length = length
        self.lfo_delay_counter = 0
        self.lfo_counter = 0
        self.start_pending = True
        self.active = True

    def step_envelope(self):
        stage = self.envelope_stage
        if stage == ENVELOPE_ATTACK:
            self.attenuation = -((-self.attenuation * self.attack) >> 8)
            if self.attenuation == 0:
                self.envelope_stage = ENVELOPE_DECAY
        elif stage == ENVELOPE_DECAY:
            sustain_level = DECIBEL_SQUARE_TABLE[self.sustain] << ENVELOPE_FRACTION_BITS
            self.attenuation -= self.decay
            if self.attenuation <= sustain_level:
                self.attenuation = sustain_level
                self.envelope_stage = ENVELOPE_SUSTAIN
        elif stage == ENVELOPE_RELEASE:
            self.attenuation -= self.release
        return self.attenuation >> ENVELOPE_FRACTION_BITS

    def step_sweep(self):
        if self.sweep_pitch == 0 or self.sweep_counter >= self.sweep_length:
            return 0
        scaled = self.sweep_pitch * (self.sweep_length - self.sweep_counter)
        result = abs(scaled) // self.sweep_length
        if self.auto_sweep:
            self.sweep_counter += 1
        return -result if scaled < 0 else result

    def lfo_value(self):
        if self.lfo_depth == 0 or self.lfo_delay_counter < self.lfo_delay:
            return 0
        value = LFO_SINE_TABLE[self.lfo_counter >> 8] * self.lfo_depth * self.lfo_range
        if self.lfo_target == LFO_VOLUME:
            return (value * 60) >> 14
        return (value << 6) >> 14

    def step_lfo(self):
        if self.lfo_delay_counter < self.lfo_delay:
            self.lfo_delay_counter += 1
            return
        advance = self.lfo_speed << 6
        whole = (self.lfo_counter + advance) >> 8
        while whole >= 128:
            whole -= 128
        self.lfo_counter = ((self.lfo_counter + advance) & 0xFF) | (whole << 8)


class Voice:
    __slots__ = ("kind", "wave", "duty", "start_sample", "end_sample", "running", "timer", "registers", "changes", "elapsed_steps")

    def __init__(self, channel, start_sample):
        self.kind = channel.kind
        self.wave = channel.wave
        self.duty = channel.duty
        self.start_sample = start_sample
        self.end_sample = None
        self.running = True
        self.timer = channel.timer_register
        self.registers = (channel.timer_register, channel.volume_register, channel.pan_register)
        self.changes = [(start_sample, channel.timer_register) + voice_gains(channel.volume_register, channel.pan_register)]
        self.elapsed_steps = 0.0

    def stop(self, sample):
        if self.running:
            self.running = False
            self.end_sample = sample

    def set_registers(self, channel, sample):
        registers = (channel.timer_register, channel.volume_register, channel.pan_register)
        if registers != self.registers:
            self.registers = registers
            self.timer = channel.timer_register
            self.changes.append((sample, channel.timer_register) + voice_gains(channel.volume_register, channel.pan_register))

    def is_one_shot(self):
        return self.kind == KIND_PCM and not self.wave["looped"]

    def advance_one_shot(self, samples, sample_after):
        self.elapsed_steps += samples * TIMER_STEPS_PER_SAMPLE / self.timer
        if int(self.elapsed_steps) - self.wave["delay"] >= len(self.wave["levels"]):
            self.running = False
            self.end_sample = sample_after


class Sequencer:
    def __init__(self, archive, sequence_index):
        info = read_sequence_info(archive, sequence_index)
        self.archive = archive
        self.data = sequence_payload(read_archive_file(archive, info["file_id"]))
        self.bank = read_bank(archive, info["bank"])
        self.instruments = {}
        self.waves = {}
        self.wave_archive_files = {}
        self.channel_mask = player_channel_mask(archive, info["player"]) or 0xFFFF
        self.channels = [Channel(index) for index in range(CHANNEL_COUNT)]
        self.tracks = [None] * TRACK_COUNT
        self.tempo = 120
        self.tempo_counter = TICK_COST
        self.ticks_left = 0
        self.player_volume = 127
        self.external_fader = DECIBEL_TABLE[min(info["volume"], 127)]
        self.local_variables = [-1] * VARIABLE_COUNT
        self.global_variables = [-1] * VARIABLE_COUNT
        self.random_state = RANDOM_START
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
        self.prepare_tracks()

    def prepare_tracks(self):
        first = Track(self.channel_mask)
        first.position = 0
        self.tracks[0] = first
        if self.read_byte(first) != COMMAND_ALLOCATE_TRACKS:
            first.position = 0
            return
        mask = self.read_short(first) >> 1
        for number in range(1, TRACK_COUNT):
            if mask & 1:
                self.tracks[number] = Track(self.channel_mask)
            mask >>= 1

    def read_byte(self, track):
        position = track.position
        track.position = position + 1
        if position >= len(self.data):
            return END_OF_DATA
        return self.data[position]

    def read_short(self, track):
        low = self.read_byte(track)
        return low | (self.read_byte(track) << 8)

    def read_three_bytes(self, track):
        low = self.read_short(track)
        return low | (self.read_byte(track) << 16)

    def next_random(self):
        self.random_state = (self.random_state * RANDOM_MULTIPLIER + RANDOM_INCREMENT) & 0xFFFFFFFF
        return self.random_state >> 16

    def read_variable(self, number):
        if number < VARIABLE_COUNT:
            return self.local_variables[number]
        if number < VARIABLE_COUNT * 2:
            return self.global_variables[number - VARIABLE_COUNT]
        return None

    def write_variable(self, number, value):
        if number < VARIABLE_COUNT:
            self.local_variables[number] = wrap_signed_short(value)
        elif number < VARIABLE_COUNT * 2:
            self.global_variables[number - VARIABLE_COUNT] = wrap_signed_short(value)

    def read_argument(self, track, kind):
        if kind == ARGUMENT_BYTE:
            return self.read_byte(track)
        if kind == ARGUMENT_SHORT:
            return self.read_short(track)
        if kind == ARGUMENT_VARIABLE_LENGTH:
            value = 0
            while True:
                byte = self.read_byte(track)
                value = (value << 7) | (byte & 0x7F)
                if not byte & 0x80:
                    return value
        if kind == ARGUMENT_FROM_VARIABLE:
            value = self.read_variable(self.read_byte(track))
            return 0 if value is None else value
        lowest = wrap_signed_short(self.read_short(track))
        highest = wrap_signed_short(self.read_short(track))
        return lowest + ((self.next_random() * (highest - lowest + 1)) >> 16)

    def instrument(self, program, key):
        lookup = (program, key)
        if lookup not in self.instruments:
            self.instruments[lookup] = read_instrument(self.bank["data"], program, key)
        return self.instruments[lookup]

    def wave(self, wave_archive_slot, index):
        lookup = (wave_archive_slot, index)
        if lookup not in self.waves:
            self.waves[lookup] = self.load_wave(wave_archive_slot, index)
        return self.waves[lookup]

    def load_wave(self, wave_archive_slot, index):
        if wave_archive_slot >= len(self.bank["wave_archives"]):
            return None
        wave_archive = self.bank["wave_archives"][wave_archive_slot]
        if wave_archive == NO_WAVE_ARCHIVE:
            return None
        if wave_archive not in self.wave_archive_files:
            offset = info_entry_offset(self.archive, WAVE_ARCHIVE_TABLE, wave_archive)
            if offset is None:
                return None
            self.wave_archive_files[wave_archive] = read_archive_file(self.archive, struct.unpack_from("<I", self.archive["data"], offset)[0])
        return decode_wave(self.wave_archive_files[wave_archive], index)

    def allocate_channel(self, mask, priority, owner):
        chosen = None
        for index in CHANNEL_ALLOCATION_ORDER:
            if not mask & (1 << index):
                continue
            channel = self.channels[index]
            if chosen is None:
                chosen = channel
            elif channel.priority < chosen.priority:
                chosen = channel
            elif channel.priority == chosen.priority and channel_loudness(chosen.volume_register) > channel_loudness(channel.volume_register):
                chosen = channel
        if chosen is None or priority < chosen.priority:
            return None
        if chosen.owner is not None:
            chosen.owner.channels.remove(chosen)
        chosen.prepare_for_owner(owner, priority)
        return chosen

    def start_channel_note(self, channel, instrument, key, velocity, length):
        release = instrument["release"]
        if release == 0xFF:
            length = -1
            release = 0
        kind = instrument["kind"]
        if kind == KIND_PCM:
            wave = self.wave(instrument["wave_archive"], instrument["wave"])
            if wave is None:
                return False
            channel.start(kind, wave, 0, wave["timer"], length)
        elif kind == KIND_PSG:
            channel.start(kind, None, instrument["wave"] & 7, BASE_TIMER_OF_TONE_GENERATOR, length)
        else:
            channel.start(kind, None, 0, BASE_TIMER_OF_TONE_GENERATOR, length)
        channel.key = key
        channel.root_key = instrument["root_key"]
        channel.velocity = velocity
        channel.attack = attack_rate(instrument["attack"])
        channel.decay = decay_rate(instrument["decay"])
        channel.sustain = instrument["sustain"]
        channel.release = decay_rate(release)
        channel.initial_pan = wrap_signed_byte(instrument["pan"] - CENTER_PAN)
        return True

    def play_note(self, track, key, velocity, length):
        channel = None
        if track.tie and track.channels:
            channel = track.channels[0]
            channel.key = key
            channel.velocity = velocity
        if channel is None:
            instrument = self.instrument(track.program, key)
            if instrument is None:
                return
            channel = self.allocate_channel(CHANNEL_MASK_OF_KIND[instrument["kind"]] & track.channel_mask, 64 + track.priority, track)
            if channel is None:
                return
            note_length = -1 if track.tie or length <= 0 else length
            if not self.start_channel_note(channel, instrument, key, velocity, note_length):
                channel.priority = 0
                channel.owner = None
                return
            track.channels.insert(0, channel)
        if track.attack != 0xFF:
            channel.attack = attack_rate(track.attack)
        if track.decay != 0xFF:
            channel.decay = decay_rate(track.decay)
        if track.sustain != 0xFF:
            channel.sustain = track.sustain
        if track.release != 0xFF:
            channel.release = decay_rate(track.release)
        channel.sweep_pitch = track.sweep_pitch
        if track.portamento:
            channel.sweep_pitch = wrap_signed_short(channel.sweep_pitch + (track.portamento_key - key) * PITCH_STEPS_PER_SEMITONE)
        if track.portamento_time == 0:
            channel.sweep_length = length if length > 0 else -1
            channel.auto_sweep = False
        else:
            channel.sweep_length = (track.portamento_time * track.portamento_time * abs(channel.sweep_pitch)) >> 11
        channel.sweep_counter = 0

    def update_track_channels(self, track, release_finished_notes):
        volume = DECIBEL_SQUARE_TABLE[min(track.volume, 127)] + DECIBEL_SQUARE_TABLE[min(track.expression, 127)] + DECIBEL_SQUARE_TABLE[min(self.player_volume, 127)]
        volume = max(volume, SILENT_DECIBELS)
        fader = max(self.external_fader, SILENT_DECIBELS)
        pitch = (track.pitch_bend * (track.bend_range << 6)) >> 7
        for channel in track.channels:
            channel.fader = fader
            if channel.envelope_stage == ENVELOPE_RELEASE:
                continue
            channel.track_volume = volume
            channel.track_pitch = pitch
            channel.track_pan = track.pan
            channel.lfo_target = track.lfo_target
            channel.lfo_speed = track.lfo_speed
            channel.lfo_depth = track.lfo_depth
            channel.lfo_range = track.lfo_range
            channel.lfo_delay = track.lfo_delay
            if channel.length == 0 and release_finished_notes:
                channel.priority = 1
                channel.envelope_stage = ENVELOPE_RELEASE

    def release_track_channels(self, track, release):
        self.update_track_channels(track, False)
        for channel in track.channels:
            if channel.active:
                if release >= 0:
                    channel.release = decay_rate(release)
                channel.priority = 1
                channel.envelope_stage = ENVELOPE_RELEASE

    def free_track_channels(self, track):
        for channel in track.channels:
            channel.owner = None
        track.channels = []

    def close_track(self, number):
        track = self.tracks[number]
        if track is not None:
            self.release_track_channels(track, -1)
            self.free_track_channels(track)
            self.tracks[number] = None

    def run_note(self, track, command, argument_kind, runs):
        velocity = self.read_byte(track)
        length = self.read_argument(track, ARGUMENT_VARIABLE_LENGTH if argument_kind is None else argument_kind)
        if not runs:
            return
        key = min(max(command + track.transpose, 0), 127)
        if not track.muted:
            self.play_note(track, key, min(velocity, 127), length)
        track.portamento_key = key
        if track.note_wait:
            track.wait = length
            if length == 0:
                track.note_finish_wait = True

    def run_jump_command(self, track, command, runs):
        if command == 0x93:
            number = self.read_byte(track)
            offset = self.read_three_bytes(track)
            if runs and number < TRACK_COUNT:
                opened = self.tracks[number]
                if opened is not None and opened is not track:
                    self.release_track_channels(opened, -1)
                    self.free_track_channels(opened)
                    opened.position = offset
        elif command == 0x94:
            offset = self.read_three_bytes(track)
            if runs:
                track.position = offset
        elif command == 0x95:
            offset = self.read_three_bytes(track)
            if runs and track.stack_depth < CALL_STACK_SIZE:
                track.return_positions[track.stack_depth] = track.position
                track.stack_depth += 1
                track.position = offset

    def run_mute(self, track, mode):
        if mode == 0:
            track.muted = False
        elif mode == 1:
            track.muted = True
        elif mode == 2:
            track.muted = True
            self.release_track_channels(track, -1)
        elif mode == 3:
            track.muted = True
            self.release_track_channels(track, 127)
            self.free_track_channels(track)

    def run_byte_command(self, track, command, value):
        if command == 0xC0:
            track.pan = wrap_signed_byte(value - CENTER_PAN)
        elif command == 0xC1:
            track.volume = value
        elif command == 0xC2:
            self.player_volume = value
        elif command == 0xC3:
            track.transpose = wrap_signed_byte(value)
        elif command == 0xC4:
            track.pitch_bend = wrap_signed_byte(value)
        elif command == 0xC5:
            track.bend_range = value
        elif command == 0xC6:
            track.priority = value
        elif command == 0xC7:
            track.note_wait = bool(value & 1)
        elif command == 0xC8:
            track.tie = bool(value & 1)
            self.release_track_channels(track, -1)
            self.free_track_channels(track)
        elif command == 0xC9:
            track.portamento_key = (value + track.transpose) & 0xFF
            track.portamento = True
        elif command == 0xCA:
            track.lfo_depth = value
        elif command == 0xCB:
            track.lfo_speed = value
        elif command == 0xCC:
            track.lfo_target = value
        elif command == 0xCD:
            track.lfo_range = value
        elif command == 0xCE:
            track.portamento = bool(value & 1)
        elif command == 0xCF:
            track.portamento_time = value
        elif command == 0xD0:
            track.attack = value
        elif command == 0xD1:
            track.decay = value
        elif command == 0xD2:
            track.sustain = value
        elif command == 0xD3:
            track.release = value
        elif command == 0xD4:
            if track.stack_depth < CALL_STACK_SIZE:
                track.return_positions[track.stack_depth] = track.position
                track.loop_counts[track.stack_depth] = value
                track.stack_depth += 1
        elif command == 0xD5:
            track.expression = value
        elif command == 0xD7:
            self.run_mute(track, value)

    def run_short_command(self, track, command, value):
        if command == 0xE0:
            track.lfo_delay = value & 0xFFFF
        elif command == 0xE1:
            self.tempo = value & 0xFFFF
        elif command == 0xE3:
            track.sweep_pitch = wrap_signed_short(value)

    def run_variable_command(self, track, command, number, value):
        current = self.read_variable(number)
        if current is None:
            return
        if command == 0xB0:
            self.write_variable(number, value)
        elif command == 0xB1:
            self.write_variable(number, current + value)
        elif command == 0xB2:
            self.write_variable(number, current - value)
        elif command == 0xB3:
            self.write_variable(number, current * value)
        elif command == 0xB4:
            if value != 0:
                quotient = abs(current) // abs(value)
                self.write_variable(number, quotient if (current < 0) == (value < 0) else -quotient)
        elif command == 0xB5:
            self.write_variable(number, current << value if value >= 0 else current >> -value)
        elif command == 0xB6:
            result = (self.next_random() * (abs(value) + 1)) >> 16
            self.write_variable(number, -result if value < 0 else result)
        elif command == 0xB8:
            track.compare_flag = current == value
        elif command == 0xB9:
            track.compare_flag = current >= value
        elif command == 0xBA:
            track.compare_flag = current > value
        elif command == 0xBB:
            track.compare_flag = current <= value
        elif command == 0xBC:
            track.compare_flag = current < value
        elif command == 0xBD:
            track.compare_flag = current != value

    def run_control_command(self, track, command):
        if command == 0xFC:
            depth = track.stack_depth
            if depth == 0:
                return False
            count = track.loop_counts[depth - 1]
            if count != 0:
                count -= 1
                if count == 0:
                    track.stack_depth = depth - 1
                    return False
            track.loop_counts[depth - 1] = count
            track.position = track.return_positions[depth - 1]
        elif command == 0xFD:
            if track.stack_depth != 0:
                track.stack_depth -= 1
                track.position = track.return_positions[track.stack_depth]
        elif command == END_OF_DATA:
            return True
        return False

    def run_command(self, track):
        command = self.read_byte(track)
        runs = True
        argument_kind = None
        if command == PREFIX_IF:
            command = self.read_byte(track)
            runs = track.compare_flag
        if command == PREFIX_RANDOM:
            command = self.read_byte(track)
            argument_kind = ARGUMENT_RANDOM
        if command == PREFIX_FROM_VARIABLE:
            command = self.read_byte(track)
            argument_kind = ARGUMENT_FROM_VARIABLE
        if command < 0x80:
            self.run_note(track, command, argument_kind, runs)
            return False
        group = command & 0xF0
        if group == 0x80:
            value = self.read_argument(track, ARGUMENT_VARIABLE_LENGTH if argument_kind is None else argument_kind)
            if runs:
                if command == 0x80:
                    track.wait = value
                elif command == 0x81 and value < 0x10000:
                    track.program = value
        elif group == 0x90:
            self.run_jump_command(track, command, runs)
        elif group == 0xC0 or group == 0xD0:
            value = self.read_argument(track, ARGUMENT_BYTE if argument_kind is None else argument_kind)
            if runs:
                self.run_byte_command(track, command, value & 0xFF)
        elif group == 0xE0:
            value = self.read_argument(track, ARGUMENT_SHORT if argument_kind is None else argument_kind)
            if runs:
                self.run_short_command(track, command, value)
        elif group == 0xB0:
            number = self.read_byte(track)
            value = wrap_signed_short(self.read_argument(track, ARGUMENT_SHORT if argument_kind is None else argument_kind))
            if runs:
                self.run_variable_command(track, command, number, value)
        elif group == 0xF0 and runs:
            return self.run_control_command(track, command)
        return False

    def step_track(self, track):
        for channel in track.channels:
            if channel.length > 0:
                channel.length -= 1
            if not channel.auto_sweep and channel.sweep_counter < channel.sweep_length:
                channel.sweep_counter += 1
        if track.note_finish_wait:
            if track.channels:
                return False
            track.note_finish_wait = False
        if track.wait > 0:
            track.wait -= 1
            if track.wait > 0:
                return False
        commands_run = 0
        while track.wait == 0 and not track.note_finish_wait:
            commands_run += 1
            if commands_run > MOST_COMMANDS_IN_A_TICK or self.run_command(track):
                return True
        return False

    def step_tick(self):
        still_playing = False
        for number in range(TRACK_COUNT):
            track = self.tracks[number]
            if track is None or track.position is None:
                continue
            if self.step_track(track):
                self.close_track(number)
            else:
                still_playing = True
        return still_playing

    def sequencer_state(self):
        track_states = []
        for track in self.tracks:
            if track is None or track.position is None:
                track_states.append(None)
            elif track.note_finish_wait:
                return None
            else:
                track_states.append(track.state())
        return (self.tempo, self.player_volume, tuple(self.local_variables), tuple(self.global_variables), tuple(track_states))

    def remember_tick_state(self):
        state = self.sequencer_state()
        if state is None:
            return
        fingerprint = hash(state)
        earlier = self.seen_states.get(fingerprint)
        if earlier is None:
            self.seen_states[fingerprint] = (self.update_index, self.tempo_counter, self.ticks_left, (self.cycles_done + CYCLES_PER_UPDATE) % CYCLES_PER_SAMPLE)
            return
        earlier_update, earlier_counter, earlier_ticks_left, earlier_cycles_past_sample = earlier
        if earlier_update == self.update_index:
            return
        self.tempo_counter = earlier_counter
        self.ticks_left = earlier_ticks_left
        self.cycles_past_sample_at_loop_start = earlier_cycles_past_sample
        self.loop_start_update = self.update_index + 1
        self.loop_end_update = self.loop_start_update + self.update_index - earlier_update

    def step_sequencer(self):
        while self.tempo_counter >= TICK_COST:
            self.tempo_counter -= TICK_COST
            self.ticks_left += 1
        while self.ticks_left > 0:
            self.ticks_left -= 1
            if not self.step_tick():
                for number in range(TRACK_COUNT):
                    self.close_track(number)
                self.playing = False
                self.ticks_left = 0
                break
            if self.loop_end_update is None:
                self.remember_tick_state()
        self.tempo_counter = (self.tempo_counter + self.tempo) & 0xFFFF
        for track in self.tracks:
            if track is not None:
                self.update_track_channels(track, True)

    def finish_channel(self, channel):
        if channel.owner is not None:
            channel.owner.channels.remove(channel)
            channel.owner = None
        channel.priority = 0
        channel.volume_register = 0
        channel.active = False

    def step_channel(self, channel):
        if channel.start_pending:
            channel.start_requested = True
            channel.start_pending = False
        elif channel.voice is None or not channel.voice.running:
            self.finish_channel(channel)
            return
        decibels = DECIBEL_SQUARE_TABLE[channel.velocity] + channel.step_envelope() + channel.fader + channel.track_volume
        pitch = (channel.key - channel.root_key) * PITCH_STEPS_PER_SEMITONE + channel.step_sweep() + channel.track_pitch
        pan = channel.initial_pan + channel.track_pan
        lfo = channel.lfo_value()
        channel.step_lfo()
        if lfo != 0:
            if channel.lfo_target == LFO_PITCH:
                pitch += lfo
            elif channel.lfo_target == LFO_VOLUME:
                if decibels > SILENT_DECIBELS:
                    decibels += lfo
            elif channel.lfo_target == LFO_PAN:
                pan += lfo
        if channel.envelope_stage == ENVELOPE_RELEASE and decibels <= QUIETEST_CHANNEL_DECIBELS:
            channel.stop_requested = True
            channel.start_requested = False
            self.finish_channel(channel)
            return
        timer = channel_timer(channel.base_timer, pitch)
        if channel.kind == KIND_PSG:
            timer &= PSG_TIMER_MASK
        channel.volume_register = channel_volume_register(decibels)
        channel.timer_register = timer
        channel.pan_register = min(max(pan + CENTER_PAN, 0), 127)

    def step_channels(self):
        for channel in self.channels:
            if channel.active:
                self.step_channel(channel)

    def apply_channel_registers(self):
        for channel in self.channels:
            if channel.stop_requested:
                channel.stop_requested = False
                if channel.voice is not None:
                    channel.voice.stop(self.samples_done)
                    channel.voice = None
            if channel.start_requested:
                channel.start_requested = False
                channel.voice = Voice(channel, self.samples_done)
                self.voices.append(channel.voice)
                if channel.voice.is_one_shot():
                    self.one_shot_voices.append(channel.voice)
            elif channel.voice is not None and channel.voice.running:
                channel.voice.set_registers(channel, self.samples_done)

    def advance_one_shot_voices(self, samples):
        sample_after = self.samples_done + samples
        still_running = []
        for voice in self.one_shot_voices:
            if voice.running:
                voice.advance_one_shot(samples, sample_after)
                if voice.running:
                    still_running.append(voice)
        self.one_shot_voices = still_running

    def run_update(self):
        self.apply_channel_registers()
        if self.playing:
            self.step_sequencer()
        self.step_channels()
        self.cycles_done += CYCLES_PER_UPDATE
        sample_after = self.cycles_done // CYCLES_PER_SAMPLE
        self.advance_one_shot_voices(sample_after - self.samples_done)
        self.samples_done = sample_after
        self.update_index += 1
        if self.update_index == self.loop_start_update:
            self.loop_start_sample = self.samples_done
            self.cycles_done = self.samples_done * CYCLES_PER_SAMPLE + self.cycles_past_sample_at_loop_start
        if self.update_index == self.loop_end_update:
            self.loop_end_sample = self.samples_done

    def run(self):
        updates_after_end = 0
        while self.update_index != self.loop_end_update and self.update_index < LONGEST_RENDER_UPDATES:
            if not self.playing:
                if updates_after_end >= LONGEST_RELEASE_UPDATES or not any(channel.active or channel.start_requested or channel.stop_requested for channel in self.channels):
                    break
                updates_after_end += 1
            self.run_update()
        for voice in self.voices:
            voice.stop(self.samples_done)


def pcm_levels(wave, positions):
    levels = numpy.zeros(len(positions), dtype=numpy.int64)
    indices = positions - wave["delay"]
    first = int(numpy.searchsorted(indices, 0))
    if wave["looped"]:
        audible = indices[first:]
        end = wave["loop_start"] + wave["loop_length"]
        wrapped = numpy.where(audible >= end, wave["loop_start"] + (audible - wave["loop_start"]) % wave["loop_length"], audible)
        levels[first:] = wave["levels"][wrapped]
    else:
        last = int(numpy.searchsorted(indices, len(wave["levels"])))
        levels[first:last] = wave["levels"][indices[first:last]]
    return levels


def tone_levels(table, positions, modulus):
    levels = numpy.zeros(len(positions), dtype=numpy.int64)
    first = int(numpy.searchsorted(positions, 1))
    levels[first:] = table[(positions[first:] - 1) % modulus]
    return levels


def voice_levels(voice, positions):
    if voice.kind == KIND_PCM:
        return pcm_levels(voice.wave, positions)
    if voice.kind == KIND_PSG:
        return tone_levels(DUTY_CYCLE_TABLE[voice.duty], positions, 8)
    return tone_levels(NOISE_TABLE, positions, NOISE_PERIOD)


def mix_voice(voice, left_mix, right_mix, total_samples):
    stop = min(voice.end_sample if voice.end_sample is not None else total_samples, total_samples)
    if stop <= voice.start_sample:
        return
    changes = numpy.array(voice.changes, dtype=numpy.int64)
    lengths = numpy.diff(numpy.minimum(numpy.append(changes[:, 0], stop), stop))
    positions = numpy.floor(numpy.cumsum(numpy.repeat(TIMER_STEPS_PER_SAMPLE / changes[:, 1], lengths))).astype(numpy.int64)
    levels = voice_levels(voice, positions)
    left_mix[voice.start_sample:stop] += (levels * numpy.repeat(changes[:, 2], lengths)) >> 10
    right_mix[voice.start_sample:stop] += (levels * numpy.repeat(changes[:, 3], lengths)) >> 10


def mix_voices(voices, total_samples):
    left_mix = numpy.zeros(total_samples, dtype=numpy.int64)
    right_mix = numpy.zeros(total_samples, dtype=numpy.int64)
    for voice in voices:
        mix_voice(voice, left_mix, right_mix, total_samples)
    stereo = numpy.empty(total_samples * 2, dtype="<i2")
    stereo[0::2] = numpy.clip(left_mix >> 8, -32768, 32767)
    stereo[1::2] = numpy.clip(right_mix >> 8, -32768, 32767)
    return stereo.tobytes()


def simulate_sequence(sdat_bytes, name):
    archive = read_archive(sdat_bytes)
    named = named_sequences(archive)
    if name not in named:
        raise ValueError("the sound archive has no sequence named " + name)
    sequencer = Sequencer(archive, named[name])
    sequencer.run()
    return sequencer


def sequence_names(sdat_bytes):
    return list(named_sequences(read_archive(sdat_bytes)))


def render_sequence(sdat_bytes, name):
    sequencer = simulate_sequence(sdat_bytes, name)
    has_loop = sequencer.loop_end_sample is not None
    return {
        "samples": mix_voices(sequencer.voices, sequencer.samples_done),
        "sample_rate": SAMPLE_RATE,
        "loop_start": sequencer.loop_start_sample if has_loop else None,
        "loop_end": sequencer.loop_end_sample if has_loop else None,
    }
