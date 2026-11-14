import json
import os
import sys
import unittest

ASSET_TOOLS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets")
sys.path.insert(0, ASSET_TOOLS)

import class_movies


class ClassMovieTest(unittest.TestCase):
    def test_the_native_hash_names_the_classes_the_scripts_ask_for(self):
        self.assertEqual(class_movies.native_class_hash("pGameMainMovieTelephone"), 0x41ee4e1e)
        self.assertEqual(class_movies.native_class_hash("pGameMainMovieRollbackShort"), 0x034157e8)
        self.assertEqual(class_movies.native_class_hash("pGameMainMovieForward"), 0x7b0ddf0f)
        self.assertEqual(class_movies.native_class_hash("pGameMainMovieMeteor"), 0x14f605fb)

    def test_a_movie_row_named_by_class_gets_that_class_id(self):
        expanded = class_movies.add_to_set({"movies": [{"name": "meteor", "class": "pGameMainMovieMeteor"}, {"name": "time_jump"}]})
        self.assertEqual(expanded["movies"][0]["class_id"], "14f605fb")
        self.assertNotIn("class_id", expanded["movies"][1])

    def test_a_class_with_no_movie_row_is_named(self):
        movies = class_movies.add_to_set({"movies": [{"name": "meteor", "class": "pGameMainMovieMeteor"}]})["movies"]
        missing = class_movies.classes_with_no_movie(movies, [("st13_game020", [0x14f605fb]), ("st13_game021", [0x03084241])])
        self.assertEqual(missing, {0x03084241: "st13_game021"})

    def test_every_class_movie_in_the_game_set_names_its_own_movie_file(self):
        with open(os.path.join(ASSET_TOOLS, "game.json")) as source:
            movies = class_movies.add_to_set(json.load(source))["movies"]
        class_ids = [movie["class_id"] for movie in movies if "class_id" in movie]
        files = [movie["file"] for movie in movies]
        self.assertEqual(len(class_ids), len(set(class_ids)))
        self.assertEqual(len(files), len(set(files)))


if __name__ == "__main__":
    unittest.main()
