import os
import shutil
import struct
import tempfile
import unittest

import asset_writers
import bake_record
import ds_set_text
import ds_text
import gtex_pack
import narration
import talk_assets
import test_ds_text

BAR = test_ds_text.BAR
WHITE = (255, 255, 255, 255)
GREY = (128, 128, 128, 255)
BLACK = (0, 0, 0, 255)


def palette():
    colors = [(0, 0, 0, 0)] + [GREY, GREY, WHITE] + [GREY] * 9 + [WHITE, GREY, BLACK]
    return [bytes(color) for color in colors]


def windows():
    return {
        ds_set_text.BOTTOM_ROW: {"x": 0, "y": 144, "width": 256, "height": 48, "lift": -1, "left": 6, "tops": [8, -1, -6]},
        ds_set_text.NARRATION_ROW: {"x": 0, "y": 0, "width": 256, "height": 192, "lift": -1, "left": 0, "tops": [15, 0, 0]},
        ds_set_text.TOP_ROW: {"x": 48, "y": 0, "width": 256, "height": 48, "lift": -1, "left": 8, "tops": [8, -2, -6]},
    }


def font():
    made = test_ds_text.make_font({1: BAR, 2: BAR}, {1: (0, 3), 2: (0, 5)})
    made.shape["line_height"] = 21
    return made


def message(*lines_of_items):
    items = []
    for index, line in enumerate(lines_of_items):
        if index:
            items.append(("control", ds_text.NEW_LINE, ()))
        items.extend(line)
    return {"items": items, "pages": ds_text.pages_of_items(items)}


def chars(*codes):
    return [("char", code) for code in codes]


class FakeScript:
    def __init__(self, narration_labels, talk_labels):
        self.narration_labels = narration_labels
        self.talk_labels = talk_labels

    def strings_of_ops(self, ops):
        if ops == (ds_set_text.chapter_assets.NARRATION_OP,):
            return list(self.narration_labels)
        if tuple(ops) == tuple(ds_set_text.chapter_assets.TALK_OPS):
            return list(self.talk_labels)
        return []

    def talk_labels_of_choice_panels(self):
        return []


class FakeBook:
    def __init__(self, messages):
        self.messages = messages

    def find(self, label, _path):
        return self.messages.get(label)


class ScreenGridTests(unittest.TestCase):
    def test_sixteen_ds_pixels_cover_forty_five_screen_pixels(self):
        self.assertEqual(ds_set_text.screen_of(16), 45)
        self.assertEqual(ds_set_text.screen_x_of(256), 840)
        self.assertEqual(ds_set_text.screen_y_of(192), 542)

    def test_every_screen_pixel_samples_the_ds_pixel_that_covers_it(self):
        for ds_pixel in range(64):
            for screen_pixel in range(ds_set_text.screen_of(ds_pixel), ds_set_text.screen_of(ds_pixel + 1)):
                self.assertEqual(ds_set_text.ds_of(screen_pixel), ds_pixel)


class TableTests(unittest.TestCase):
    def test_window_rows_come_from_the_arm9_table(self):
        arm9 = bytearray(0x90000)
        base = ds_set_text.WINDOW_TABLE_ADDRESS - ds_text.ARM9_LOAD_ADDRESS
        struct.pack_into("<9h", arm9, base, 0, 144, 256, 48, -1, 6, 8, -1, -6)
        struct.pack_into("<9h", arm9, base + 3 * ds_set_text.WINDOW_ROW_SIZE, 0, 0, 256, 192, -1, 0, 15, 0, 0)
        rows = ds_set_text.read_window_rows(arm9)
        self.assertEqual(rows[ds_set_text.BOTTOM_ROW], windows()[ds_set_text.BOTTOM_ROW])
        self.assertEqual(rows[ds_set_text.NARRATION_ROW]["tops"], [15, 0, 0])

    def test_the_palette_reads_bgr555(self):
        colors = ds_set_text.text_palette(struct.pack("<16H", *([0, 0x7fff, 0x001f] + [0] * 13)))
        self.assertEqual(colors[1], bytes(WHITE))
        self.assertEqual(colors[2], bytes((255, 0, 0, 255)))

    def test_ink_alpha_is_the_darkness_of_the_colour_group(self):
        self.assertEqual(ds_set_text.ink_alphas(palette(), ds_set_text.TALK_COLOR), {1: 0, 2: 127, 3: 255})

    def test_the_window_top_and_line_lift_follow_the_page_s_line_count(self):
        window = windows()[ds_set_text.BOTTOM_ROW]
        self.assertEqual([ds_set_text.top_of_kind(window, kind) for kind in (1, 2, 3)], [8, -1, -6])
        self.assertEqual([ds_set_text.lift_of_kind(kind) for kind in (1, 2, 3)], [-1, -1, -7])
        self.assertEqual(ds_set_text.page_line_count(chars(1) + [("control", ds_text.NEW_LINE, ())] * 2), 3)


class NarrationTests(unittest.TestCase):
    def test_a_line_is_centred_on_the_screen_and_scaled_on_the_screen_grid(self):
        items = chars(1, 1)
        image, color = ds_set_text.narration_line_image(font(), palette(), items, 3, windows()[ds_set_text.NARRATION_ROW], 20)
        self.assertEqual(color, 3)
        self.assertEqual(image["width"] % 2, 0)
        self.assertEqual(image["top"], ds_set_text.screen_y_of(20))
        self.assertEqual(image["height"], ds_set_text.screen_y_of(24) - ds_set_text.screen_y_of(20))
        pixels, _, _, _ = ds_text.draw_items(font(), items, 3)
        ds_left = (256 - ds_text.measured_width(font(), items)) // 2
        inked = sorted(set(ds_left + x for x, _, _ in pixels))
        texture_left = ds_set_text.SCREEN_CENTER_X - image["width"] // 2
        alphas = [image["rgba"][column * 4 + 3] for column in range(image["width"])]
        expected = [255 if ds_set_text.ds_of(texture_left + column - ds_set_text.CAMERA_LEFT_ON_SCREEN) in inked else 0 for column in range(image["width"])]
        self.assertEqual(alphas, expected)
        self.assertEqual(image["rgba"][alphas.index(255) * 4:alphas.index(255) * 4 + 4], bytes(WHITE))

    def test_a_line_of_spaces_bakes_nothing(self):
        image, _ = ds_set_text.narration_line_image(font(), palette(), chars(0xff), 3, windows()[ds_set_text.NARRATION_ROW], 20)
        self.assertIsNone(image)

    def test_groups_restart_lower_and_lines_step_by_the_line_pitch(self):
        page = [[{}, {}], [{}]]
        pitch = 21 - 2
        self.assertEqual(ds_set_text.narration_tops(font(), page, 25, windows()[ds_set_text.NARRATION_ROW]), [[25, 25 + pitch], [25 + pitch + 2 * pitch]])


class TalkTests(unittest.TestCase):
    def test_advances_add_up_to_the_screen_position_of_every_glyph(self):
        placed, _ = ds_set_text.placed_glyphs(font(), chars(1, 2, 1, 0xff, 2), 15)
        glyphs = ds_set_text.TalkGlyphs({1: 0, 2: 127, 3: 255})
        start = 7
        ids = [glyphs.id_of(glyph, start + glyph["x"], 150) for glyph in placed]
        cells = glyphs.cells()
        pen = ds_set_text.screen_x_of(start + placed[0]["x"])
        for glyph, glyph_id in zip(placed, ids):
            self.assertEqual(pen, ds_set_text.screen_x_of(start + glyph["x"]))
            pen = pen + cells[glyph_id]["advance"]

    def test_a_colour_code_picks_the_port_s_tint(self):
        line = {"items": chars(1) + [("control", ds_text.COLOR, (9,))] + chars(1), "codes": [], "text": ""}
        glyphs = ds_set_text.TalkGlyphs({1: 0, 2: 127, 3: 255})
        rows, color = ds_set_text.talk_line_rows(font(), glyphs, [line], windows()[ds_set_text.BOTTOM_ROW], 1, 15)
        codes = [int(code) for code in rows[0].split()[4:]]
        self.assertEqual([code >> talk_assets.COLOR_SHIFT for code in codes], [0, 2])
        self.assertEqual(color, 9)
        self.assertEqual(rows[0].split()[2], str(ds_set_text.screen_y_of(144 + 8)))

    def test_a_head_page_moves_to_the_top_window_and_an_auto_end_closes(self):
        items = [("control", talk_assets.HEAD_CODE, (6,))] + chars(1) + [("control", ds_set_text.PAUSE_CODE, ()), ("control", ds_set_text.AUTO_END_CODE, ())]
        rows = ds_set_text.talk_message_rows(font(), ds_set_text.TalkGlyphs({1: 0, 2: 127, 3: 255}), windows(), "m01_0099", {"items": items, "pages": ds_text.pages_of_items(items)})
        self.assertEqual(rows[0], "talk m01_0099")
        page = rows[1].split()
        self.assertEqual(int(page[3]), talk_assets.BOX_TOP_WITH_HEAD)
        self.assertEqual(int(page[5]), talk_assets.ENDING_PAUSE_THEN_CLOSE)
        self.assertEqual(rows[2].split()[1], str(ds_set_text.screen_x_of(48 + 8)))


def control(code, *parameters):
    return ("control", code, parameters)


def page_fields(items):
    rows = ds_set_text.talk_message_rows(font(), ds_set_text.TalkGlyphs({1: 0, 2: 127, 3: 255}), windows(), "a", {"items": items, "pages": ds_text.pages_of_items(items)})
    fields = []
    for row in rows:
        parts = row.split()
        if parts[0] == "page":
            fields.append({"reveal_is_fade": int(parts[4]), "ending": int(parts[5]), "flashes": int(parts[8]), "quake_level": int(parts[9]), "quake_frames": int(parts[10]), "pauses_before": int(parts[11])})
    return fields


class PageControlTests(unittest.TestCase):
    def test_white_in_fades_the_page_in(self):
        fields = page_fields([control(ds_set_text.WHITE_IN_CODE, 60)] + chars(1) + [control(ds_text.NEW_PAGE)] + chars(2))
        self.assertEqual([page["reveal_is_fade"] for page in fields], [1, 0])

    def test_a_pause_then_an_auto_page_waits_then_turns(self):
        items = chars(1) + [control(ds_set_text.PAUSE_CODE), control(ds_text.AUTO_PAGE, 0)] + chars(2) + [control(ds_set_text.PAUSE_CODE), control(ds_set_text.AUTO_END_CODE)]
        self.assertEqual([page["ending"] for page in page_fields(items)], [talk_assets.ENDING_PAUSE_THEN_NEXT, talk_assets.ENDING_PAUSE_THEN_CLOSE])

    def test_an_auto_page_alone_does_not_wait_on_the_script(self):
        items = chars(1) + [control(ds_text.AUTO_PAGE, 30)] + chars(2)
        self.assertEqual([page["ending"] for page in page_fields(items)], [talk_assets.ENDING_PRESS, talk_assets.ENDING_PRESS])

    def test_pauses_before_the_text_are_counted_and_the_page_ends_on_a_press(self):
        items = chars(1) + [control(ds_text.NEW_PAGE), control(ds_set_text.PAUSE_CODE), control(ds_set_text.PAUSE_CODE)] + chars(ds_text.SPACE_CODE, 2)
        fields = page_fields(items)
        self.assertEqual([page["pauses_before"] for page in fields], [0, 2])
        self.assertEqual(fields[1]["ending"], talk_assets.ENDING_PRESS)

    def test_a_pause_after_text_keeps_the_ending(self):
        items = chars(1) + [control(ds_set_text.PAUSE_CODE)] + chars(2) + [control(ds_text.NEW_PAGE)] + chars(1)
        fields = page_fields(items)
        self.assertEqual(fields[0]["ending"], talk_assets.ENDING_PAUSE_THEN_NEXT)
        self.assertEqual(fields[0]["pauses_before"], 0)

    def test_flash_and_shake_carry_their_parameters(self):
        fields = page_fields([control(ds_set_text.FLASH_CODE), control(ds_set_text.SHAKE_CODE, 1, 40)] + chars(1))
        self.assertEqual((fields[0]["flashes"], fields[0]["quake_level"], fields[0]["quake_frames"]), (1, 1, 40))

    def test_every_native_pause_makes_its_page_send_pause_in(self):
        codes = [ds_set_text.PAUSE_CODE, ds_set_text.AUTO_END_CODE, ds_set_text.WHITE_IN_CODE, ds_text.NEW_LINE]
        for first in codes + ["char"]:
            for second in codes + ["char"]:
                for third in codes + ["char"]:
                    items = []
                    for code in (first, second, third):
                        if code == "char":
                            items.extend(chars(1))
                        else:
                            items.append(control(code, *((60,) if code == ds_set_text.WHITE_IN_CODE else ())))
                    items.append(control(ds_text.AUTO_PAGE, 0))
                    items.extend(chars(2))
                    page = page_fields(items)[0]
                    if ds_set_text.PAUSE_CODE in (first, second, third) or ds_set_text.AUTO_END_CODE in (first, second, third):
                        self.assertTrue(page["ending"] != talk_assets.ENDING_PRESS or page["pauses_before"] > 0, (first, second, third))
                    else:
                        self.assertEqual((page["ending"], page["pauses_before"]), (talk_assets.ENDING_PRESS, 0), (first, second, third))


class GlyphLimitTests(unittest.TestCase):
    def test_too_many_glyph_cells_drop_the_row_phase(self):
        limit = ds_set_text.GLYPH_LIMIT
        ds_set_text.GLYPH_LIMIT = 4
        try:
            talk_messages = [("a", message(chars(1, 2), chars(1, 2), chars(1, 2)))]
            glyphs, _ = ds_set_text.talk_rows(font(), palette(), windows(), talk_messages, {}, {})
        finally:
            ds_set_text.GLYPH_LIMIT = limit
        self.assertFalse(glyphs.keeps_row_phase)


class BakeTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.folder)

    def bake(self):
        record = bake_record.BakeRecord(self.folder)
        scripts = {"st01/st01_game000.xml": FakeScript(["m01_0000"], ["m01_0090", "m01_9999"])}
        book = FakeBook({"m01_0000": message(chars(1, 2), chars(2)), "m01_0090": message(chars(1), chars(2))})
        baked = ds_set_text.bake_from_messages(record, font(), palette(), windows(), book, scripts)
        record.save()
        return baked, record

    def test_narration_talk_and_textures_come_back_in_the_shapes_the_tables_read(self):
        baked, _ = self.bake()
        self.assertEqual(list(baked["narration_lines"]), ["m01_0000"])
        lines = baked["narration_lines"]["m01_0000"]["paragraphs"][0]
        self.assertEqual([line["texture"] for line in lines], [narration.line_texture_name("m01_0000", 0), narration.line_texture_name("m01_0000", 1)])
        self.assertEqual([line["id"] for line in lines], [1, 2])
        self.assertEqual(baked["narration_lines"]["m01_0000"]["pos_y"], narration.DEFAULT_POS_Y)
        talk = baked["talk_files"][talk_assets.TALK_FILE].decode("ascii").split("\n")
        self.assertEqual(talk[0], "font " + talk_assets.FONT_TEXTURE)
        self.assertIn("talk m01_0090", talk)
        self.assertNotIn("talk m01_9999", talk)
        self.assertEqual(baked["talk_textures"], [talk_assets.FONT_TEXTURE])
        self.assertEqual(baked["bubble_rows"], [])
        self.assertEqual(sorted(baked["texture_sizes"]), sorted([line["texture"] for line in lines] + [talk_assets.FONT_TEXTURE]))
        for relative_path in baked["texture_paths"]:
            with open(bake_record.disk_path(self.folder, relative_path), "rb") as source:
                data = gtex_pack.unpack_gtex(source.read())
            self.assertEqual(asset_writers.read_gtex_header(data)[2], asset_writers.FORMAT_RGBA8888)

    def test_a_second_bake_reuses_every_texture(self):
        first, _ = self.bake()
        second, record = self.bake()
        self.assertEqual(second, first)
        self.assertEqual(record.baked_in("textures"), 0)
        self.assertEqual(record.reused_in("textures"), len(first["texture_paths"]))


if __name__ == "__main__":
    unittest.main()
