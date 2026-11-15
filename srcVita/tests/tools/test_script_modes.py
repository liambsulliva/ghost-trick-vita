import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import check_script_modes

STRINGS = {0: "GameApp", 8: "EventFrame", 19: "Off", 23: "EyeCatch", 32: "Trick", 38: "Confetti", 47: "Karaoke", 55: "*"}


def send(target, verb, arg):
    return (0, "op", check_script_modes.SEND_OP, [target, verb, arg])


def ramp(opcode, mode):
    return (0, "op", opcode, [0, 0, 0, 0, 0, 30, 0, mode, 0])


class ScriptModes(unittest.TestCase):
    def setUp(self):
        self.engine = check_script_modes.engine_modes()

    def lacking(self, items):
        return check_script_modes.modes_the_engine_lacks("yard", STRINGS, items, self.engine)

    def test_the_engine_knows_the_modes_the_game_uses(self):
        self.assertTrue({"EventFrame", "EyeCatch", "CustomSoulVision"} <= self.engine["gameapp"])
        self.assertTrue({"Trick", "DestinyUpdate", "DestinyLock", "NumberGet", "NumberGetForce"} <= self.engine["eyecatch"])
        self.assertEqual(self.engine["beam"], {1, 2, 4, 8})
        self.assertEqual(self.engine["light"], {1, 2, 4, 8, 16})
        self.assertIn(0xFFF8, self.engine["motion"])

    def test_handled_commands_and_modes_pass(self):
        items = [send(0, 8, 19), send(0, 23, 32), ramp(check_script_modes.BEAM_RAMP_OP, 2), ramp(check_script_modes.LIGHT_RAMP_OP, 16), (0, "motion", [(0, -8, []), (0, "frame", [1, 2])])]
        self.assertEqual(self.lacking(items), [])

    def test_an_unknown_gameapp_verb_is_found(self):
        self.assertEqual(self.lacking([send(0, 47, 19)]), [("yard", "GameApp", "Karaoke")])

    def test_an_unknown_eyecatch_kind_is_found(self):
        self.assertEqual(self.lacking([send(0, 23, 38)]), [("yard", "EyeCatch", "Confetti")])

    def test_a_message_to_an_object_is_not_a_gameapp_command(self):
        self.assertEqual(self.lacking([send(55, 47, 19)]), [])

    def test_an_unknown_beam_or_light_mode_is_found(self):
        self.assertEqual(self.lacking([ramp(check_script_modes.BEAM_RAMP_OP, 16), ramp(check_script_modes.LIGHT_RAMP_OP, 32)]), [("yard", "beam mode", "16"), ("yard", "light mode", "32")])

    def test_an_unknown_motion_word_is_found(self):
        self.assertEqual(self.lacking([(0, "motion", [(0, -15, [])])]), [("yard", "motion word", "0xfff1")])


if __name__ == "__main__":
    unittest.main()
