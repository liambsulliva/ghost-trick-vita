import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import check_undrawn_effects


class UndrawnEffectsTest(unittest.TestCase):
    def test_a_spawned_effect_with_no_row_and_not_named_fails(self):
        found = check_undrawn_effects.problems({4: "a.lmg", 6: "b.lmg", 300: "c.lmg"}, {4}, {6})
        self.assertEqual(len(found), 1)
        self.assertIn("effect 300", found[0])

    def test_a_named_effect_that_gained_a_row_fails_so_the_list_stays_true(self):
        found = check_undrawn_effects.problems({4: "a.lmg", 6: "b.lmg"}, {4, 6}, {6})
        self.assertEqual(len(found), 1)
        self.assertIn("has a row now", found[0])

    def test_every_spawned_effect_drawn_or_named_passes(self):
        self.assertEqual(check_undrawn_effects.problems({4: "a.lmg", 6: "b.lmg"}, {4}, {6}), [])

    def test_the_two_lists_do_not_overlap(self):
        self.assertFalse(check_undrawn_effects.PLAIN_KINDS_WITH_NO_REMASTER_ART & check_undrawn_effects.CLASS_EFFECTS_NOT_PORTED)


if __name__ == "__main__":
    unittest.main()
