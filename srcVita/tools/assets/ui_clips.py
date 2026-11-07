import math

VITA_CENTER_X = 480.0
VITA_CENTER_Y = 272.0
VITA_WIDTH = 960.0
VITA_PIXELS_PER_GUI_UNIT = 0.5
BANNER_RED = (1.0, 0.0, 0.0)
WHITE = (1.0, 1.0, 1.0)
BANNER_KINDS = {
    "TRICK": {"texture": "banner_trick", "size": (692.0, 152.0), "y": 0.0, "delay": 0, "frames": 114},
    "BANGO": {"texture": "banner_trace", "size": (694.0, 158.0), "y": 0.0, "delay": 0, "frames": 114},
    "KANRYO": {"texture": "banner_averted", "size": (714.0, 154.0), "y": 10.0, "delay": 0, "frames": 114},
    "KOSHIN": {"texture": "banner_changed", "size": (708.0, 154.0), "y": 7.5, "delay": 10, "frames": 124},
}
HOURGLASS_LARGE_SCALE = 0.25


def constant(value):
    return [(0.0, value)]


def shifted(keys, delay):
    return [(frame + delay, value) for frame, value in keys]


def scaled(keys, factor):
    return [(frame, value * factor) for frame, value in keys]


def element(kind, texture, color, **tracks):
    return {"kind": kind, "texture": texture, "color": color, "tracks": tracks}


def banner_clip(name, kind):
    delay = kind["delay"]
    width, height = kind["size"]
    text_y = VITA_CENTER_Y + kind["y"]
    text_scale_x = [(0, 1.0), (24, 2.3), (35, 1.0)]
    text_scale_y = [(0, 1.0), (24, 2.3), (35, 1.0), (90, 1.0), (95, 2.5), (100, 0.1)]
    echo_scale = [(0, 1.0), (35, 1.0), (45, 2.3)]
    elements = []
    if name == "KOSHIN":
        elements.append(element("hourglass", "ui_hourglass", WHITE, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(HOURGLASS_LARGE_SCALE), alpha=[(0, 0.0), (20, 1.0), (110, 1.0), (124, 0.0)], window=[(40, 0.0), (90, 1.0)]))
    elements.append(element("sprite", kind["texture"], WHITE, x=constant(VITA_CENTER_X), y=constant(text_y), w=shifted(scaled(echo_scale, width), delay), h=shifted(scaled(echo_scale, height), delay), alpha=shifted([(0, 0.0), (34, 0.0), (35, 1.0), (45, 0.0)], delay)))
    elements.append(element("sprite", "banner_star", WHITE, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=shifted([(0, 174.0), (100, 174.0), (114, 348.0)], delay), h=shifted([(0, 174.0), (100, 174.0), (114, 348.0)], delay), rot=shifted([(0, 0.0), (100, 0.0), (114, 90.0)], delay), alpha=shifted([(0, 0.0), (99, 0.0), (100, 1.0), (114, 0.0)], delay)))
    elements.append(element("rect", "-", BANNER_RED, x=shifted([(0, 0.0), (10, VITA_CENTER_X)], delay), y=constant(VITA_CENTER_Y), w=shifted([(0, 0.0), (10, VITA_WIDTH)], delay), h=shifted([(0, 25.0), (100, 25.0), (114, 5.0)], delay), alpha=shifted([(0, 1.0), (9, 1.0), (12, 0.0), (99, 0.0), (100, 1.0), (114, 0.0)], delay)))
    elements.append(element("band", "-", BANNER_RED, h=shifted([(0, 0.1), (9, 0.1), (25, 1.0), (90, 1.0), (95, 2.5), (100, 0.1)], delay), alpha=shifted([(0, 0.0), (9, 0.0), (10, 1.0), (95, 1.0), (100, 0.502), (114, 0.0)], delay)))
    elements.append(element("sprite", kind["texture"], WHITE, x=constant(VITA_CENTER_X), y=constant(text_y), w=shifted(scaled(text_scale_x, width), delay), h=shifted(scaled(text_scale_y, height), delay), alpha=shifted([(0, 0.0), (24, 0.0), (35, 1.0), (90, 1.0), (100, 0.0)], delay)))
    return {"name": name, "frames": kind["frames"], "elements": elements}


def wipe_clip(name, stays_faint_longer):
    hourglass_alpha = [(0, 0.0), (10, 0.0), (40, 1.0), (102, 1.0), (133, 0.0), (143, 0.0)] if stays_faint_longer else [(0, 0.0), (11, 1.0), (133, 1.0), (143, 0.0)]
    return {"name": name, "frames": 143, "elements": [
        element("hourglass", "ui_hourglass", WHITE, x=[(0, 777.5), (40, 480.0), (102, 480.0), (143, 777.5)], y=[(0, -268.0), (40, 272.0), (102, 272.0), (143, -268.0)], w=constant(HOURGLASS_LARGE_SCALE), alpha=hourglass_alpha),
        element("reveal", "wipe_time_passing", WHITE, x=constant(480.0), y=constant(285.0), w=constant(692.0), h=constant(156.0), alpha=constant(1.0), window=[(0, -380.0), (35, -380.0), (118, 1090.0)]),
    ]}


def digit_clip(name, digit_texture, widths, heights):
    frames = [0, 16, 30, 32, 34]
    ring_alpha = [(0, 0.0), (7, 0.0), (17, 1.0), (22, 1.0), (25, 0.0)]
    ring_scale = [(0, 0.0), (7, 0.0), (25, 3.0)]
    ring_elements = []
    ring_half = 230.0 * VITA_PIXELS_PER_GUI_UNIT
    for sign_x, sign_y in ((-1.0, -1.0), (1.0, -1.0), (-1.0, 1.0), (1.0, 1.0)):
        ring_elements.append(element("sprite", "ui_wave_quadrant", WHITE,
            x=[(frame, VITA_CENTER_X + sign_x * ring_half * value / 2.0) for frame, value in ring_scale],
            y=[(frame, VITA_CENTER_Y + sign_y * ring_half * value / 2.0) for frame, value in ring_scale],
            w=[(frame, sign_x * ring_half * value) for frame, value in ring_scale],
            h=[(frame, sign_y * ring_half * value) for frame, value in ring_scale],
            alpha=ring_alpha))
    label_width = 268.75
    label_right = [(0, 0.0), (9, 0.0), (19, 456.0), (20, 463.5), (21, 435.0), (23, 456.0)]
    return {"name": name, "frames": 80, "elements": [
        element("rect", "-", WHITE, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(VITA_WIDTH), h=constant(544.0), alpha=[(12, 0.0), (13, 1.0), (14, 0.0)]),
        element("hourglass", "ui_hourglass", WHITE, x=constant(491.0), y=constant(VITA_CENTER_Y), w=[(0, 0.125), (10, 0.25)], alpha=[(0, 1.0), (26, 1.0), (58, 0.0)]),
    ] + ring_elements + [
        element("sprite", "digit_label", WHITE, x=[(frame, right - label_width / 2.0) for frame, right in label_right], y=constant(275.5), w=constant(label_width), h=constant(134.375), alpha=[(0, 1.0), (58, 1.0), (80, 0.0)]),
        element("sprite", digit_texture, WHITE, x=constant(590.0), y=constant(252.5), w=[(frame, value * VITA_PIXELS_PER_GUI_UNIT) for frame, value in zip(frames, widths)], h=[(frame, value * VITA_PIXELS_PER_GUI_UNIT) for frame, value in zip(frames, heights)], alpha=[(0, 0.0), (16, 0.0), (30, 1.0), (58, 1.0), (80, 0.0)]),
    ]}


CARD_RED = (0.72, 0.01, 0.02)
CARD_OUTER_RED = (0.62, 0.0, 0.01)
CARD_BLACK = (0.0, 0.0, 0.0)
CARD_CLOCK_SIZE = 500.0
CARD_HAND_LAST_TURN_FRAME = 50
CARD_HANDS = (
    ("card_hand_hour", 74.0, 203.0, 84.0, -180.0, -478.0),
    ("card_hand_minute", 53.0, 252.0, 108.0, 0.0, -1080.0),
    ("card_hand_seconds", 52.0, 211.0, 80.0, -90.0, -2610.0),
)


def value_at(keys, frame):
    for (first_frame, first_value), (last_frame, last_value) in zip(keys, keys[1:]):
        if frame <= last_frame:
            return first_value + (last_value - first_value) * (frame - first_frame) / (last_frame - first_frame)
    return keys[-1][1]


def card_hand(texture, width, height, reach, first_turn, last_turn, clock_scale, clock_alpha):
    frames = list(range(0, CARD_HAND_LAST_TURN_FRAME + 1))
    turns = [first_turn + (last_turn - first_turn) * frame / CARD_HAND_LAST_TURN_FRAME for frame in frames]
    scales = [value_at(clock_scale, frame) for frame in frames]
    return element(
        "sprite", texture, WHITE,
        x=[(frame, VITA_CENTER_X + reach * scale * math.sin(math.radians(turn))) for frame, scale, turn in zip(frames, scales, turns)],
        y=[(frame, VITA_CENTER_Y - reach * scale * math.cos(math.radians(turn))) for frame, scale, turn in zip(frames, scales, turns)],
        w=[(frame, width * scale) for frame, scale in zip(frames, scales)],
        h=[(frame, height * scale) for frame, scale in zip(frames, scales)],
        rot=list(zip(frames, turns)),
        alpha=clock_alpha,
    )


def card_wave(first_turn, last_turn):
    return element(
        "sprite", "card_wave", WHITE, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y),
        w=[(0, 432.0), (30, 518.0), (45, 864.0)], h=[(0, 384.0), (30, 460.8), (45, 768.0)],
        rot=[(0, first_turn), (30, first_turn), (45, last_turn)], alpha=[(0, 0.0), (29, 0.0), (30, 1.0), (45, 0.0)],
    )


def card_clip():
    text_scale = [(0, 3.0), (32, 3.0), (60, 1.0)]
    text_alpha = [(0, 0.0), (31, 0.0), (60, 1.0), (183, 1.0), (193, 0.0)]
    clock_alpha = [(0, 0.0), (30, 1.0), (183, 1.0), (193, 0.0)]
    clock_scale = [(0, 3.5), (31, 0.6), (32, 0.72), (33, 0.615), (68, 0.615)]
    return {"name": "CARD_BEFORE_DEATH", "frames": 193, "elements": [
        element("sprite", "hud_disc", CARD_OUTER_RED, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(329.0), h=constant(329.0), alpha=text_alpha),
        element("sprite", "hud_disc", CARD_BLACK, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(313.0), h=constant(313.0), alpha=text_alpha),
        element("sprite", "card_clock", WHITE, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=scaled(clock_scale, CARD_CLOCK_SIZE), h=scaled(clock_scale, CARD_CLOCK_SIZE), alpha=clock_alpha),
        card_wave(0.0, 10.0),
        card_wave(180.0, 170.0),
    ] + [card_hand(texture, width, height, reach, first_turn, last_turn, clock_scale, clock_alpha) for texture, width, height, reach, first_turn, last_turn in CARD_HANDS] + [
        element("rect", "-", CARD_OUTER_RED, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(VITA_WIDTH), h=scaled(text_scale, 147.0), alpha=text_alpha),
        element("rect", "-", CARD_BLACK, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(VITA_WIDTH), h=scaled(text_scale, 132.0), alpha=text_alpha),
        element("rect", "-", CARD_RED, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(VITA_WIDTH), h=scaled(text_scale, 87.0), alpha=text_alpha),
        element("sprite", "card_top_text", WHITE, x=constant(VITA_CENTER_X + 1.0), y=[(frame, VITA_CENTER_Y - 86.0 * value) for frame, value in text_scale], w=scaled(text_scale, 273.0), h=scaled(text_scale, 81.0), alpha=text_alpha),
        element("sprite", "card_bottom_text", WHITE, x=constant(VITA_CENTER_X + 0.5), y=[(frame, VITA_CENTER_Y + 92.5 * value) for frame, value in text_scale], w=scaled(text_scale, 356.0), h=scaled(text_scale, 79.0), alpha=text_alpha),
        element("rect", "-", WHITE, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(VITA_WIDTH), h=constant(544.0), alpha=[(60, 0.0), (62, 1.0), (68, 0.0)]),
    ]}


def all_clips():
    clips = [banner_clip(name, kind) for name, kind in BANNER_KINDS.items()]
    clips.append(wipe_clip("TIME_3", True))
    clips.append(wipe_clip("TIME_2", False))
    clips.append(wipe_clip("TIME_1", False))
    clips.append(digit_clip("DEATH_COUNT_3", "digit_3", [287, 861, 287, 344.25, 287], [627, 1881, 627, 752.4, 627]))
    clips.append(digit_clip("DEATH_COUNT_2", "digit_2", [287, 1149, 383, 459.5, 383], [627, 2112, 704, 844.8, 704]))
    clips.append(digit_clip("DEATH_COUNT_1", "digit_1", [149.125, 447.25, 149.125, 178.875, 149.125], [578, 1734, 578.1, 693.6, 578]))
    clips.append(digit_clip("DEATH_COUNT_0", "digit_0", [277, 692.5, 277, 332.25, 277], [590.4, 1475, 590, 708, 590]))
    clips.append(card_clip())
    clips.append({"name": "WHITE_FILTER", "frames": 177, "elements": [
        element("rect", "-", WHITE, x=constant(VITA_CENTER_X), y=constant(VITA_CENTER_Y), w=constant(VITA_WIDTH), h=constant(544.0), alpha=[(0, 0.0), (12, 1.0), (84, 1.0), (177, 0.0)]),
    ]})
    return clips


def number_text(value):
    return ("%.3f" % value).rstrip("0").rstrip(".")


def clips_text(clips=None):
    lines = []
    for clip in all_clips() if clips is None else clips:
        lines.append("clip %s %d" % (clip["name"], clip["frames"]))
        for item in clip["elements"]:
            lines.append("element %s %s %s %s %s" % (item["kind"], item["texture"], number_text(item["color"][0]), number_text(item["color"][1]), number_text(item["color"][2])))
            for prop, keys in item["tracks"].items():
                lines.append("track %s %s" % (prop, " ".join("%s %s" % (number_text(frame), number_text(value)) for frame, value in keys)))
    return "\n".join(lines) + "\n"
