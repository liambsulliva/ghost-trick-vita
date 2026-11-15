import json
import os
import unittest

import ds_alert_balloons
import ds_effect_movies
import script_assets

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
BURST = 4
MARK = 2


def item(slot, character, depth, scale, x, y, alpha=1.0):
    return {"slot": slot, "character": character, "depth": depth, "first": 1, "matrix": (scale, scale, 0.0, 0.0, x, y), "rgba": (1.0, 1.0, 1.0, alpha)}


def fake_movie(frames):
    shapes = {BURST: {"rect": (0, 32, 0, 32), "fills": [{"bitmap": 3, "matrix": (1.0, 1.0, 0.0, 0.0, 0.0, 0.0)}]}, MARK: {"rect": (0, 32, 0, 32), "fills": [{"bitmap": 1, "matrix": (1.0, 1.0, 0.0, 0.0, 0.0, 0.0)}]}}
    return {"fps": 60, "names": [], "bitmap_ids": [1, 3], "shapes": shapes, "sprites": [{"sprite_id": ds_effect_movies.ROOT_SPRITE, "fps": 60, "frames": [{"label": "", "items": items} for items in frames], "scripts": []}]}


def alert_frames():
    frames = [[] for _ in range(10)]
    frames.append([item(0, BURST, 2, 0.8, 115, 83)])
    frames.append([item(0, BURST, 2, 1.0, 112, 80)])
    frames.append([item(0, BURST, 2, 1.0, 112, 80), item(1, MARK, 3, 1.0, 111, 80)])
    return frames


class UnkeptRecord:
    def key_for(self, *parts):
        return "key"

    def read_if_current(self, relative_path, key):
        return None

    def store(self, *arguments):
        pass


class MovieTests(unittest.TestCase):
    def test_the_row_waits_for_the_first_picture_and_places_each_where_it_settles_from_the_stage_centre(self):
        row = ds_alert_balloons.row_of_kind(5, ds_alert_balloons.ALERT_EFFECT, fake_movie(alert_frames()))
        burst, mark = row["parts"]
        self.assertEqual((row["kind"], row["delay"]), (5, 10))
        self.assertEqual((burst["texture"], burst["dx"], burst["dy"], burst["after"], burst["grow"]), (ds_alert_balloons.TEXTURE_NAME % (0x85, 2), 0.0, 0.0, 0, 1))
        self.assertEqual((mark["texture"], mark["dx"], mark["dy"], mark["after"], mark["grow"]), (ds_alert_balloons.TEXTURE_NAME % (0x85, 1), -1.0, 0.0, 2, 0))
        self.assertNotIn("label", row)

    def test_parts_are_drawn_in_their_movie_depth_order(self):
        frames = alert_frames()
        frames[-1] = [item(1, MARK, 1, 1.0, 111, 80), item(0, BURST, 2, 1.0, 112, 80)]
        row = ds_alert_balloons.row_of_kind(5, ds_alert_balloons.ALERT_EFFECT, fake_movie(frames))
        self.assertEqual([part["texture"][-1] for part in row["parts"]], ["1", "2"])

    def test_a_slot_that_settles_turned_is_refused(self):
        frames = alert_frames()
        turned = item(0, BURST, 2, 1.0, 112, 80)
        turned["matrix"] = (1.0, 1.0, 0.5, 0.0, 112, 80)
        frames.append([turned])
        with self.assertRaises(ds_alert_balloons.AlertRefused):
            ds_alert_balloons.row_of_kind(5, ds_alert_balloons.ALERT_EFFECT, fake_movie(frames))

    def test_the_question_kind_plays_its_own_effect_and_mirrored_kinds_are_named(self):
        self.assertEqual(ds_alert_balloons.effect_of_kind(4), 0x8c)
        self.assertEqual(ds_alert_balloons.effect_of_kind(2), 0x85)
        self.assertEqual(ds_alert_balloons.effect_of_kind(5), 0x85)
        rows, textures, refused = ds_alert_balloons.compose({3: "script"}, lambda effect: b"")
        self.assertEqual((rows, textures), ([], {}))
        self.assertIn("mirrored", refused[0])

    def test_the_row_is_written_with_no_label_at_ds_size(self):
        row = ds_alert_balloons.row_of_kind(5, ds_alert_balloons.ALERT_EFFECT, fake_movie(alert_frames()))
        lines = script_assets.balloons_text([row], {part["texture"]: (32, 32) for part in row["parts"]}).splitlines()
        self.assertEqual(lines[0], "balloon 5 - 10")
        self.assertEqual(lines[1].split()[4:6], ["32", "32"])


@unittest.skipUnless(os.path.isfile(ROM_PATH), "needs the cartridge")
class CartridgeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import ds_set
        with open(GAME_DESCRIPTION, encoding="utf-8") as source:
            cls.description = json.load(source)
        cls.cartridge = ds_set.Cartridge(ROM_PATH)
        _, cls.scripts, cls.arm9, *_ = ds_set.expand(cls.description, ROM_PATH, cls.cartridge)

    def test_every_balloon_kind_the_game_opens_with_no_text_gets_a_row_with_cartridge_art(self):
        import ds_effects
        pack = ds_effects.EffectPack(self.cartridge.textures.pack)
        wanted = ds_alert_balloons.balloon_kinds(self.scripts)
        self.assertIn(5, wanted)
        for language in range(1, 6):
            rows, textures, refused = ds_alert_balloons.compose(wanted, lambda effect: pack.effect_bytes(effect, language))
            self.assertEqual(refused, [])
            self.assertEqual({row["kind"] for row in rows}, set(wanted))
            for row in rows:
                self.assertTrue(row["parts"])
                for part in row["parts"]:
                    width, height, rgba = textures[part["texture"]]
                    self.assertEqual(len(rgba), width * height * 4)
                    self.assertTrue(any(rgba[3::4]))

    def test_the_ds_balloon_table_carries_the_rows_with_no_text(self):
        import ds_balloons
        rows, paths, sizes = ds_balloons.bake(self.description, self.scripts, self.arm9, self.cartridge, ROM_PATH, UnkeptRecord())
        kinds = {row["kind"] for row in rows if "label" not in row}
        self.assertEqual(kinds, set(ds_alert_balloons.balloon_kinds(self.scripts)))
        for row in rows:
            for part in row["parts"]:
                self.assertIn(part["texture"], sizes)
                self.assertIn("textures/%s.gtex" % part["texture"], paths)


if __name__ == "__main__":
    unittest.main()
