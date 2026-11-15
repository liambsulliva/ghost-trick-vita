import os
import re
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "tools", "assets"))

import talk_assets

SCRIPT_INPUT_PATH = os.path.join(HERE, "..", "..", "src", "app", "script_input.c")
ACTION_OF_ICON = {
    "TRICK": "script_machine_note_press(machine)",
    "GHOST": "script_soul_note_ghost(machine)",
    "BACK": "script_soul_note_ghost(machine)",
    "RESTART": "SCRIPT_PILL_HOURGLASS",
    "SAVE": "SCRIPT_PILL_NOTEBOOK",
}
BUTTON_BLOCK_PATTERN = re.compile(r"if \(pressed & INPUT_(\w+)\) \{(.*?)\n    \}", re.S)


def buttons_by_action():
    with open(SCRIPT_INPUT_PATH, encoding="utf-8") as source:
        text = source.read()
    body = text[text.index("static void send_buttons("):]
    body = body[:body.index("\n}\n")]
    found = {}
    for button, block in BUTTON_BLOCK_PATTERN.findall(body):
        for action in ACTION_OF_ICON.values():
            if action in block.split("} else", 1)[0]:
                found.setdefault(action, button)
    return found


class TalkIconButtonTests(unittest.TestCase):
    def test_every_talk_icon_shows_the_button_the_port_wires_to_its_action(self):
        wired = buttons_by_action()
        for icon, action in ACTION_OF_ICON.items():
            with self.subTest(icon=icon):
                self.assertEqual(talk_assets.BUTTON_OF_ICON[icon], wired[action])

    def test_every_icon_the_baker_knows_has_an_action(self):
        self.assertEqual(set(talk_assets.BUTTON_OF_ICON), set(ACTION_OF_ICON))

    def test_the_record_icon_reads_select(self):
        section = talk_assets.parse_page("Tap <ICON SAVE> to view a record.")[0]
        characters = [character for line in section["lines"] for character, _ in line]
        self.assertIn(talk_assets.ICON_SELECT, characters)
        self.assertIn("SELECT", talk_assets.icon_shape_arguments(talk_assets.ICON_SELECT, "font.ttf", talk_assets.ICON_SIZE))


if __name__ == "__main__":
    unittest.main()
