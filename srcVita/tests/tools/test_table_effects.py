import json
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))
import chapter_assets
import script_assets

GAME_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets", "game.json")
RING_ROWS_STILL_ON_THE_GUESSED_GLOW = {7, 11, 12, 13}


class FakeScript:
    def __init__(self, operations, tables):
        self.operations = operations
        self.tables = tables


class SpawnedEffectIdsTest(unittest.TestCase):
    def test_ids_come_from_ops_and_table_commands(self):
        script = FakeScript([(0, "op", chapter_assets.EFFECT_SPAWN_OP, [12, 0]), (4, "op", 0x10, [7])],
                            [(8, "motion", [(8, "frame", [1, 2]), (10, chapter_assets.TAG_EFFECT, [207, 0, 0, 0, 0, 3]), (22, chapter_assets.TAG_SOUND, [5, 0])])])
        self.assertEqual(list(chapter_assets.spawned_effect_ids(script)), [12, 207])


class RingArtTest(unittest.TestCase):
    def test_a_pulse_bakes_its_period_and_keys_in_order(self):
        text = script_assets.pulses_text([{"name": "core_ring", "period_ticks": 70, "keys": [[0, 64, 0.65], [3, 91, 1], [44, 131, 0.105]]}])
        self.assertEqual(text, "pulse core_ring 70 0 64 0.65 3 91 1 44 131 0.105\n")

    def test_every_ring_row_draws_measured_art_or_is_named_as_a_guess(self):
        with open(GAME_PATH) as source:
            game = json.load(source)
        pulses = {pulse["name"]: pulse for pulse in game.get("pulses", [])}
        flipbooks = {book["name"] for book in game.get("flipbooks", [])}
        textures = {texture["name"] for texture in game["screen_textures"]}
        guessed = set()
        for row in game["effects"]:
            if row["kind"] != "ring":
                continue
            if row["texture"] in pulses:
                self.assertIn(row["texture"], textures, row["id"])
            elif row["texture"] not in flipbooks:
                guessed.add(row["id"])
        self.assertEqual(guessed, RING_ROWS_STILL_ON_THE_GUESSED_GLOW)

    def test_every_pulse_has_rising_keys_inside_its_period(self):
        with open(GAME_PATH) as source:
            game = json.load(source)
        for pulse in game.get("pulses", []):
            ticks = [key[0] for key in pulse["keys"]]
            self.assertEqual(ticks, sorted(set(ticks)), pulse["name"])
            self.assertLess(ticks[-1], pulse["period_ticks"], pulse["name"])


if __name__ == "__main__":
    unittest.main()
