import unittest

import check_player_link

MAP_TEXT = """# Path: build/player_objects/player
# Object files:
[  0] linker synthesized
[  1] build/player_objects/src/app/toast.o
[  2] build/player_objects/tests/fakes/fake_renderer.o
# Sections:
# Address	Size    	Segment	Section
# Symbols:
# Address	Size    	File  Name
0x100000000	0x00000010	[  1] _toast_show
# Dead Stripped Symbols:
#        	Size    	File  Name
<<dead>> 	0x00000010	[  1] _toast_is_showing
<<dead>> 	0x00000010	[  1] _fade_share
<<dead>> 	0x00000010	[  1] _toast_seen_by_the_device
<<dead>> 	0x00000010	[  2] _fake_renderer_reset
"""


def exports(object_path):
    return {"_toast_show", "_toast_is_showing", "_toast_seen_by_the_device", "_fake_renderer_reset"} if object_path else set()


class PlayerLinkTest(unittest.TestCase):
    def found(self):
        return check_player_link.unreached_names(MAP_TEXT, "/src/", {"toast_seen_by_the_device"}, exports)

    def test_only_exported_game_functions_the_device_layer_does_not_call_count(self):
        self.assertEqual(self.found(), {"toast_is_showing"})

    def test_a_new_unreached_function_is_a_problem(self):
        self.assertEqual(check_player_link.problems(self.found(), set()), ["unreached in the player build and not listed: toast_is_showing"])

    def test_a_listed_function_passes(self):
        self.assertEqual(check_player_link.problems(self.found(), {"toast_is_showing"}), [])

    def test_a_stale_listing_is_a_problem(self):
        self.assertEqual(len(check_player_link.problems(self.found(), {"toast_is_showing", "toast_gone"})), 1)


if __name__ == "__main__":
    unittest.main()
