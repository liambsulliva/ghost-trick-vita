import json
import os
import unittest
from unittest import mock

import numpy as np

import ds_pause
import ds_text
import pause_art
from test_ds_text import BAR, make_font

GAME_DESCRIPTION = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "tools", "assets", "game.json")
LANGUAGES_THE_DS_FONT_CANNOT_WRITE = ["chinese_simplified", "chinese_traditional", "japanese", "korean"]
CODE_OF_A = 10
RED = (255, 0, 0, 255)
CLEAR = (0, 0, 0, 0)


def game_pause():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return json.load(source)["pause"]


def font_with_a_bar_for_every_letter():
    return make_font({code: BAR for code in range(0x120)}, {code: (0, 4) for code in range(0x120)})


def solid(width, height, color=RED):
    pixels = np.zeros((height, width, 4), dtype=np.uint8)
    pixels[...] = color
    return pixels


class FakePack:
    def __init__(self, pixels):
        self.pixels = pixels

    def texture(self, index, language=1, first_color_is_clear=True):
        return self.pixels.shape[1], self.pixels.shape[0], self.pixels.tobytes()

    def texture_bytes(self, index, language=1):
        return b"texture"


class WritingTests(unittest.TestCase):
    def test_a_character_the_font_lacks_is_refused_by_name(self):
        with self.assertRaises(ds_pause.DsPauseError) as refusal:
            ds_pause.items_of("A日")
        self.assertIn("日", str(refusal.exception))

    def test_accented_letters_agree_with_the_codes_the_message_reader_knows(self):
        for code, character in ds_text.CHARACTER_OF_CODE.items():
            self.assertEqual(ds_pause.CODE_OF_CHARACTER[character], code)
        codes = list(ds_pause.CODE_OF_CHARACTER.values())
        self.assertEqual(len(codes), len(set(codes)))
        self.assertEqual(ds_pause.CODE_OF_CHARACTER["ñ"], 0x67)

    def test_every_menu_string_can_be_written_except_the_languages_the_cartridge_never_had(self):
        pause = game_pause()
        widgets = pause["widgets"]
        texts = [text for style in ("row", "header", "bar", "title") for text in widgets[style].values()]
        texts += [widgets["chapter_format"] % number for number in range(1, widgets["chapter_count"] + 1)] + pause["labels"]
        for text in texts:
            for line in text.split("\n"):
                ds_pause.items_of(line)
        unwritten = []
        for language in widgets["languages"]:
            try:
                ds_pause.items_of(language["text"])
            except ds_pause.DsPauseError:
                unwritten.append(language["key"])
        self.assertEqual(sorted(unwritten), LANGUAGES_THE_DS_FONT_CANNOT_WRITE)


class TextTests(unittest.TestCase):
    def setUp(self):
        self.box = mock.patch.multiple(ds_pause, LINE_TOP=0, LINE_ROWS=4)
        self.box.start()
        self.addCleanup(self.box.stop)
        self.font = font_with_a_bar_for_every_letter()

    def test_text_is_the_font_s_ink_in_the_asked_color_at_whole_ds_pixels(self):
        pixels = ds_pause.text_pixels(self.font, "A", "#ff8000", 2)
        self.assertEqual(pixels.shape[0], 8)
        inked = pixels[pixels[..., 3] > 0]
        self.assertGreater(len(inked), 0)
        self.assertTrue((inked[:, :3] == (255, 128, 0)).all())
        self.assertTrue((pixels[0::2, 0::2] == pixels[1::2, 1::2]).all())

    def test_two_lines_stack_and_drop_their_empty_rows(self):
        one = ds_pause.ink_of_text(self.font, "A")
        two = ds_pause.ink_of_text(self.font, "A\nAA")
        self.assertGreater(two.shape[0], one.shape[0])
        self.assertTrue(two[0].any())
        self.assertTrue(two[-1].any())

    def test_an_outlined_word_has_a_solid_rim_one_ds_pixel_wide(self):
        plain = ds_pause.text_pixels(self.font, "A", "#ffffff", 1)
        outlined = ds_pause.outlined_text_pixels(self.font, "A", "#ffffff", "#000080", 1)
        self.assertEqual(outlined.shape[0], plain.shape[0] + 2)
        self.assertEqual(outlined.shape[1], plain.shape[1] + 2)
        rim = outlined[1:-1, 1:-1][plain[..., 3] == 0]
        self.assertTrue(any(tuple(pixel) == (0, 0, 128, 255) for pixel in outlined.reshape(-1, 4)))
        self.assertTrue(all(tuple(pixel) in ((0, 0, 128, 255), CLEAR) for pixel in rim))


class ShapeTests(unittest.TestCase):
    def test_a_soft_edge_becomes_whole_ds_pixels_that_are_solid_or_clear(self):
        soft = solid(8, 8)
        soft[:, :2, 3] = 60
        soft[:, 2:4, 3] = 200
        hard = ds_pause.pixelated(soft)
        self.assertEqual(hard.shape, soft.shape)
        self.assertEqual(sorted(set(hard[..., 3].reshape(-1).tolist())), [0, 255])
        self.assertTrue((hard[0::2, 0::2] == hard[1::2, 1::2]).all())
        self.assertEqual(hard[0, 0, 3], 0)
        self.assertEqual(hard[0, 2, 3], 255)

    def test_an_odd_sized_shape_keeps_its_size(self):
        self.assertEqual(ds_pause.pixelated(solid(11, 7)).shape, (7, 11, 4))

    def test_a_button_keeps_its_ends_and_a_piece_of_its_middle(self):
        texture = np.zeros((40, 100, 4), dtype=np.uint8)
        texture[4:36, 10:90] = RED
        texture[4:36, 10:12] = (0, 0, 255, 255)
        button = ds_pause.button_pixels(texture)
        self.assertEqual(button.shape[:2], (32 * ds_pause.BUTTON_SCALE, (2 * ds_pause.BUTTON_CORNER + ds_pause.BUTTON_MIDDLE) * ds_pause.BUTTON_SCALE))
        self.assertEqual(tuple(button[0, 0]), (0, 0, 255, 255))
        self.assertEqual(tuple(button[0, -1]), RED)

    def test_a_button_smaller_than_its_corners_is_refused(self):
        with self.assertRaises(ds_pause.DsPauseError):
            ds_pause.button_pixels(solid(8, 8))

    def test_a_picture_is_fitted_inside_both_sides_of_its_room(self):
        self.assertEqual(ds_pause.fitted(solid(76, 40), 40, 48).shape[:2], (21, 39))
        self.assertEqual(ds_pause.fitted(solid(20, 96), 40, 48).shape[:2], (48, 10))

    def test_an_unlit_flame_is_the_lit_one_in_grey(self):
        dark = ds_pause.darkened(solid(2, 2, (0, 0, 255, 255)))
        self.assertEqual(dark[0, 0, 0], dark[0, 0, 2])
        self.assertEqual(dark[0, 0, 3], 255)


class SheetTests(unittest.TestCase):
    def cells(self):
        pause = game_pause()
        font = font_with_a_bar_for_every_letter()
        pack = FakePack(solid(64, 40))
        with mock.patch.multiple(ds_pause, LINE_TOP=0, LINE_ROWS=4), mock.patch.object(pause_art, "slice_pixels", lambda folder, width, height, *rest: solid(width, height)), mock.patch.object(pause_art, "arrow_down_pixels", lambda folder: solid(*pause_art.ARROW_DOWN_CELL)), mock.patch.object(pause_art, "trash_pixels", lambda folder: solid(*pause_art.TRASH_CELL)), mock.patch.object(ds_pause, "glyph_pixels", lambda folder, arguments: solid(ds_pause.hud_art.GLYPH_CELL, ds_pause.hud_art.GLYPH_CELL)):
            return pause, ds_pause.widget_cells(font, pack, 1, None, pause["widgets"], pause["ds_art"])

    def test_the_ds_sheet_has_every_cell_the_remastered_sheet_has(self):
        pause, (cells, refused) = self.cells()
        keys = set(kind + " " + name.split(" ")[0] if kind == "slice" else kind + " " + name for kind, name, _ in cells)
        for name, *_ in pause_art.SLICES + pause_art.TITLE_SLICES:
            self.assertIn("slice " + name, keys)
        for name, *_ in pause_art.TITLE_PRESSED_SLICES:
            self.assertIn("slice " + name, keys)
        for number in range(1, pause_art.METER_STEPS + 1):
            for lit in (0, 1):
                self.assertIn("flame %d %d" % (number, lit), keys)
        self.assertIn("cell arrow_down", keys)
        self.assertIn("cell trash", keys)
        self.assertIn("cell book", keys)
        for kind in ("cross", "circle", "square", "triangle", "l", "r", "start"):
            self.assertIn("wglyph " + kind, keys)
        widgets = pause["widgets"]
        for style in ("row", "header", "bar"):
            for key in widgets[style]:
                self.assertIn("text %s.%s" % (style, key), keys)
        for key in widgets["title"]:
            self.assertIn("text title." + key, keys)
        self.assertIn("text row.chapter_%d" % widgets["chapter_count"], keys)
        self.assertEqual(sorted(text.split(" ")[0] for text in refused), ["language." + key for key in LANGUAGES_THE_DS_FONT_CANNOT_WRITE])

    def test_the_sheet_fits_the_app_s_texture_limit(self):
        _, (cells, _) = self.cells()
        sheet, lines = pause_art.pack_widget_cells(cells)
        self.assertLessEqual(sheet.shape[0], 4096)
        self.assertEqual(len(lines), len(cells))

    def test_the_label_sheet_names_a_plain_and_a_selected_word_for_every_label(self):
        font = font_with_a_bar_for_every_letter()
        words = game_pause()["labels"]
        with mock.patch.multiple(ds_pause, LINE_TOP=0, LINE_ROWS=4), mock.patch.object(pause_art, "magick_rgba", lambda folder, width, height, arguments: solid(width, height)):
            sheet, lines = ds_pause.label_sheet(font, None, words)
        self.assertEqual([line.split(" ")[0] for line in lines], ["label"] * len(words) + ["arrows", "selected_column"])
        cell_width, cell_height = pause_art.LABEL_CELL
        for row in range(len(words)):
            self.assertTrue(sheet[row * cell_height:(row + 1) * cell_height, :cell_width, 3].any())
            self.assertTrue(sheet[row * cell_height:(row + 1) * cell_height, cell_width:2 * cell_width, 3].any())

    def test_a_label_at_the_real_font_s_size_fits_its_cell(self):
        self.assertLessEqual((ds_pause.LINE_ROWS + 2) * ds_pause.LABEL_SCALE, pause_art.LABEL_CELL[1])


class FakePages:
    def __init__(self):
        self.asked = []

    def pixels(self, name):
        self.asked.append(name)
        return 2, 1, bytes((255, 0, 0, 255)) + bytes(4)

    def source(self, name):
        return name.encode("ascii")


class CatTests(unittest.TestCase):
    def test_the_cat_s_animations_are_the_ds_set_s_own_by_the_remastered_names(self):
        wanted = game_pause()["cat_animations"]
        texts = {pause_art.TABLE_PREFIX + entry["name"]: "frame ds_sprite_1 4 0 0 2 1 0 0 1 1\n" for entry in wanted}
        animations = ds_pause.cat_animations(wanted, texts)
        self.assertEqual([animation["name"] for animation in animations], [pause_art.TABLE_PREFIX + entry["name"] for entry in wanted])
        self.assertEqual([animation["mirror"] for animation in animations], [entry["mirror"] for entry in wanted])

    def test_a_cat_animation_the_ds_set_lacks_is_refused_by_name(self):
        with self.assertRaises(ds_pause.DsPauseError) as refusal:
            ds_pause.cat_animations([{"name": "st01_neko_001", "mirror": True}], {})
        self.assertIn("st01_neko_001", str(refusal.exception))

    def test_a_ds_page_reaches_the_cat_baker_at_the_remastered_art_s_size(self):
        pages = FakePages()
        page = ds_pause.art_sized_page(pages, "ds_sprite_568")
        self.assertEqual(pages.asked, ["ds_sprite_568"])
        scale = ds_pause.frame_layout.ART_PER_DS_PIXEL
        self.assertEqual(page.shape, (scale, 2 * scale, 4))
        self.assertTrue((page[:, :scale] == (255, 0, 0, 255)).all())
        self.assertTrue((page[:, scale:] == 0).all())

    def test_a_doubled_page_keeps_flat_areas_and_rounds_a_stair_step(self):
        ink = (10, 20, 30, 255)
        page = np.zeros((2, 2, 4), dtype=np.uint8)
        page[0, 1] = ink
        page[1, 0] = ink
        doubled = ds_pause.doubled_with_smooth_edges(page)
        self.assertEqual(doubled.shape, (4, 4, 4))
        self.assertEqual(tuple(doubled[0, 3]), ink)
        self.assertEqual(tuple(doubled[1, 1]), ink)
        self.assertEqual(tuple(doubled[0, 0]), (0, 0, 0, 0))
        self.assertEqual(tuple(doubled[2, 2]), ink)
        self.assertEqual(tuple(doubled[3, 3]), (0, 0, 0, 0))
        flat = np.full((3, 3, 4), 7, dtype=np.uint8)
        self.assertTrue((ds_pause.doubled_with_smooth_edges(flat) == 7).all())

    def test_the_cat_baker_takes_its_pages_from_whoever_holds_them(self):
        page = solid(8, 4)
        asked = []

        def page_of(animation, texture):
            asked.append((animation["name"], texture))
            return page

        animations = [{"name": "t_cat", "mirror": False, "walk": True, "text": "frame sheet 4 0 0 2 1 0 0 0.5 1\nframe sheet 4 1 0 2 1 0.5 0 0.5 1\n"}]
        baked = pause_art.cat_frames(page_of, animations)
        self.assertEqual(asked, [("t_cat", "sheet")])
        self.assertEqual(len(baked[0][1]), 2)
        self.assertEqual(baked[0][1][0]["pixels"].shape, (4, 4, 4))

    def test_a_sharp_cat_page_is_stored_pixel_for_pixel(self):
        page = np.zeros((pause_art.PAGE_SIZE, pause_art.PAGE_SIZE, 4), dtype=np.uint8)
        page[0, 0] = RED
        gtex = pause_art.cat_page_gtex(None, "pause_cat_00", page, pause_art.SHARP_FILTER)
        width, height, texture_format = ds_pause.asset_writers.read_gtex_header(gtex)
        self.assertEqual((width, height, texture_format), (pause_art.PAGE_SIZE, pause_art.PAGE_SIZE, ds_pause.asset_writers.FORMAT_RGBA8888))

    def test_a_soft_cat_keeps_its_size_and_colour_and_softens_its_edge(self):
        image = np.zeros((8, 8, 4), dtype=np.uint8)
        image[2:6, 2:6] = RED
        soft = pause_art.softened(image)
        self.assertEqual(soft.shape, image.shape)
        self.assertEqual(tuple(soft[3, 3]), tuple(RED))
        self.assertTrue(0 < soft[1, 3, 3] < 255)
        self.assertEqual(tuple(soft[1, 3, :3]), tuple(RED[:3]))
        self.assertEqual(soft[0, 0, 3], 0)
        gtex = pause_art.cat_page_gtex(None, "pause_cat_00", np.zeros((pause_art.PAGE_SIZE, pause_art.PAGE_SIZE, 4), dtype=np.uint8), pause_art.SOFT_FILTER)
        self.assertEqual(ds_pause.asset_writers.read_gtex_header(gtex)[2], ds_pause.asset_writers.FORMAT_RGBA8888)


    def test_a_cat_on_the_backdrop_grid_keeps_its_size_with_pixels_the_backdrop_size(self):
        image = np.zeros((30, 30, 4), dtype=np.uint8)
        image[0:15, 0:15] = RED
        grid = pause_art.on_backdrop_grid(image)
        self.assertEqual(grid.shape, image.shape)
        self.assertEqual(set(np.unique(grid[:, :, 3])), {0, 255})
        columns = [len(set(map(tuple, grid[row, :, :].tolist()))) for row in range(30)]
        self.assertTrue(max(columns) <= 2)
        edges = [x for x in range(1, 30) if tuple(grid[0, x]) != tuple(grid[0, x - 1])]
        self.assertTrue(all(abs(x / pause_art.DS_BACKDROP_SCREEN_PIXELS_PER_PIXEL - round(x / pause_art.DS_BACKDROP_SCREEN_PIXELS_PER_PIXEL)) * pause_art.DS_BACKDROP_SCREEN_PIXELS_PER_PIXEL < 1 for x in edges))
        with open(ds_pause.__file__, encoding="utf-8") as source:
            self.assertIn("pause_art.GRID_FILTER", source.read())

    def test_a_cat_on_the_backdrop_grid_shows_only_colours_the_art_has(self):
        image = np.zeros((45, 45, 4), dtype=np.uint8)
        for column in range(45):
            image[:, column] = RED if (column * 2 // 11) % 2 == 0 else (10, 200, 30, 255)
        grid = pause_art.on_backdrop_grid(image)
        source_colours = set(map(tuple, image.reshape(-1, 4).tolist()))
        self.assertTrue(set(map(tuple, grid.reshape(-1, 4).tolist())) <= source_colours)


if __name__ == "__main__":
    unittest.main()
