import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import numpy

import asset_writers
import effect_pieces
import gtex_pack

SHOWN_BLOCK = bytes([255, 255]) + bytes(6) + bytes([7, 7, 7, 7, 0, 0, 0, 0])


def page_with_shown_blocks(width, height, shown):
    rows = height // 4
    columns = width // 4
    blocks = numpy.zeros((rows, columns, 16), dtype=numpy.uint8)
    for column, row in shown:
        blocks[row, column] = numpy.frombuffer(SHOWN_BLOCK, dtype=numpy.uint8)
    blocks[:, :, 8] = (numpy.arange(columns) % 256)[numpy.newaxis, :]
    blocks[:, :, 9] = (numpy.arange(rows) % 256)[:, numpy.newaxis]
    return asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_DXT5, blocks.tobytes())


def frame_of(page, source, corners=None):
    x, y, width, height = source
    corners = corners or ((0.0, 0.0), (width / 2.0, 0.0), (width / 2.0, height / 2.0), (0.0, height / 2.0))
    return {"quads": [{"page": page, "source": source, "corners": corners}]}


def block_grid(page):
    page = gtex_pack.unpack_gtex(page)
    width, height, _ = asset_writers.read_gtex_header(page)
    return numpy.frombuffer(page[20:], dtype=numpy.uint8).reshape(height // 4, width // 4, 16)


def texel_block(blocks, x, y):
    return bytes(blocks[y // 4, x // 4])


class FakeRecord:
    def __init__(self):
        self.files = {}
        self.details = {}

    def key_for(self, code, settings, source_bytes):
        return code + str(len(source_bytes))

    def details_of(self, relative_path):
        return self.details.get(relative_path, {})

    def read_if_current(self, relative_path, key):
        entry = self.files.get(relative_path)
        return entry[1] if entry and entry[0] == key else None

    def store(self, relative_path, key, data, details=None):
        self.files[relative_path] = (key, gtex_pack.pack_gtex(data))
        if details is not None:
            self.details[relative_path] = details


class FakePacks:
    def __init__(self, pages):
        self.pages = pages

    def read(self, path):
        return self.pages[path]


class EffectPiecesTest(unittest.TestCase):
    def test_only_squares_that_show_become_pieces_with_a_block_of_gutter(self):
        page = effect_pieces.blocks_of(page_with_shown_blocks(256, 256, {(2, 2), (40, 30)}))
        pieces = effect_pieces.pieces_of_cell(page, (0, 0, 200, 160), False)
        self.assertEqual([piece["inner"] for piece in pieces], [(0, 0, 32, 32), (160, 96, 192, 128)])
        self.assertEqual(pieces[1]["stored"], (39, 23, 49, 33))
        self.assertEqual(pieces[1]["fractions"], (0.8, 0.6, 0.96, 0.8))

    def test_neighbouring_squares_join_into_one_run(self):
        page = effect_pieces.blocks_of(page_with_shown_blocks(256, 256, {(1, 1), (9, 1), (17, 1)}))
        pieces = effect_pieces.pieces_of_cell(page, (0, 0, 256, 32), False)
        self.assertEqual([piece["inner"] for piece in pieces], [(0, 0, 96, 32)])

    def test_a_cell_that_shows_nothing_has_no_pieces(self):
        page = effect_pieces.blocks_of(page_with_shown_blocks(64, 64, set()))
        self.assertEqual(effect_pieces.pieces_of_cell(page, (0, 0, 64, 64), False), [])

    def test_a_movie_with_a_bent_quad_keeps_each_cell_whole(self):
        page = effect_pieces.blocks_of(page_with_shown_blocks(64, 64, {(1, 1)}))
        bent = frame_of(0, (0, 0, 64, 64), ((0.0, 0.0), (32.0, 4.0), (30.0, 30.0), (0.0, 32.0)))
        pieces, spans, pages = effect_pieces.layout_of({"frames": [bent]}, {0: page})
        self.assertEqual([piece["inner"] for piece in pieces], [(0, 0, 64, 64)])
        self.assertEqual(pieces[0]["fractions"], (0.0, 0.0, 1.0, 1.0))

    def test_frames_that_show_the_same_cell_share_its_pieces(self):
        page = effect_pieces.blocks_of(page_with_shown_blocks(128, 128, {(1, 1), (20, 1)}))
        frames = [frame_of(0, (0, 0, 64, 64)), frame_of(0, (64, 0, 64, 64)), frame_of(0, (0, 0, 64, 64))]
        pieces, spans, pages = effect_pieces.layout_of({"frames": frames}, {0: page})
        rows = [effect_pieces.frame_row(frame, spans) for frame in frames]
        self.assertTrue(rows[0].startswith("fxframe 0 1 "))
        self.assertTrue(rows[1].startswith("fxframe 1 1 "))
        self.assertEqual(rows[0], rows[2])

    def test_each_piece_shows_the_same_texels_on_its_new_page(self):
        source = page_with_shown_blocks(4096, 4096, {(10, 10), (500, 300), (1000, 1000)})
        effect = {"name": "wind", "page_paths": ["wind0"], "page_size": (4096, 4096), "frames": [frame_of(0, (0, 0, 2048, 2048)), frame_of(0, (2048, 2048, 2048, 2048))]}
        record = FakeRecord()
        lines, stored = effect_pieces.bake_movie(FakePacks({"wind0": source}), "wind", effect, "", record, "code", lambda folder, name, data, width, height: data)
        self.assertEqual(len(stored), 1)
        new_page = gtex_pack.unpack_gtex(stored[0])
        width, height, _ = asset_writers.read_gtex_header(new_page)
        self.assertLessEqual(max(width, height), effect_pieces.PAGE_SIDE_MOST)
        self.assertLess(width * height, 4096 * 4096 // 64)
        new_blocks = block_grid(new_page)
        source_blocks = block_grid(source)
        pieces = [line.split() for line in lines if line.startswith("fxpiece")]
        frames = [line.split() for line in lines if line.startswith("fxframe")]
        self.assertEqual(len(pieces), 3)
        for frame, cell in zip(frames, [(0, 0, 2048, 2048), (2048, 2048, 2048, 2048)]):
            for piece in pieces[int(frame[1]):int(frame[1]) + int(frame[2])]:
                u0, v0, u1, v1, s0, t0, s1, t1 = [float(value) for value in piece[2:]]
                source_x = int(round(cell[0] + s0 * cell[2]))
                source_y = int(round(cell[1] + t0 * cell[3]))
                for step in range(0, 32, 4):
                    self.assertEqual(texel_block(new_blocks, int(round(u0 * width)) + step, int(round(v0 * height)) + step), texel_block(source_blocks, source_x + step, source_y + step))
                self.assertAlmostEqual((u1 - u0) * width, (s1 - s0) * cell[2], places=2)
                self.assertAlmostEqual((v1 - v0) * height, (t1 - t0) * cell[3], places=2)

    def test_a_second_bake_reuses_the_stored_pages_and_layout(self):
        source = page_with_shown_blocks(256, 256, {(3, 3)})
        effect = {"name": "puff", "page_paths": ["puff0"], "page_size": (256, 256), "frames": [frame_of(0, (0, 0, 128, 128))]}
        record = FakeRecord()
        first = effect_pieces.bake_movie(FakePacks({"puff0": source}), "puff", effect, "", record, "code", lambda folder, name, data, width, height: data)

        def refuses(folder, name, data, width, height):
            raise AssertionError("the page was encoded again")

        self.assertEqual(effect_pieces.bake_movie(FakePacks({"puff0": source}), "puff", effect, "", record, "code", refuses), first)

    def test_a_movie_whose_pages_outgrow_its_share_of_the_cache_is_refused(self):
        page = gtex_pack.pack_gtex(asset_writers.gtex_bytes(2048, 2048, asset_writers.FORMAT_DXT5, bytes(2048 * 2048)))
        effect_pieces.check_movie_memory("fits", [page] * 4)
        with self.assertRaises(ValueError):
            effect_pieces.check_movie_memory("wind", [page] * 5)

    def test_the_header_names_the_baked_page_count(self):
        self.assertEqual(effect_pieces.movie_lines(["fxmovie wind 2 1 0 -1 10", "fxjump 1 0 0 0"], 2, ["fxpiece 0 0 0 1 1 0 0 1 1"]), ["fxmovie wind 2 1 0 -1 2", "fxjump 1 0 0 0", "fxpiece 0 0 0 1 1 0 0 1 1"])


if __name__ == "__main__":
    unittest.main()
