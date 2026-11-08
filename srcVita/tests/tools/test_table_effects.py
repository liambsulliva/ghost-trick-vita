import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))
import chapter_assets


class FakeScript:
    def __init__(self, operations, tables):
        self.operations = operations
        self.tables = tables


class SpawnedEffectIdsTest(unittest.TestCase):
    def test_ids_come_from_ops_and_table_commands(self):
        script = FakeScript([(0, "op", chapter_assets.EFFECT_SPAWN_OP, [12, 0]), (4, "op", 0x10, [7])],
                            [(8, "motion", [(8, "frame", [1, 2]), (10, chapter_assets.TAG_EFFECT, [207, 0, 0, 0, 0, 3]), (22, chapter_assets.TAG_SOUND, [5, 0])])])
        self.assertEqual(list(chapter_assets.spawned_effect_ids(script)), [12, 207])


if __name__ == "__main__":
    unittest.main()
