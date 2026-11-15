import os
import tempfile
import unittest
from unittest import mock

import smoke_mutations
import smoke_test


class EntryTests(unittest.TestCase):
    def test_every_source_mutation_finds_its_text_once(self):
        for breaks, relative_path, old, new, caught_by in smoke_mutations.SOURCE_MUTATIONS + smoke_mutations.FULL_SOURCE_MUTATIONS:
            with open(os.path.join(smoke_mutations.SRC_VITA_FOLDER, relative_path), encoding="utf-8") as source:
                self.assertEqual(source.read().count(old), 1, breaks)
            self.assertNotEqual(old, new, breaks)
            self.assertTrue(caught_by, breaks)

    def test_every_flow_template_has_a_mutation_it_must_catch(self):
        with open(smoke_test.FLOWS_PATH, encoding="utf-8") as source:
            templates = smoke_test.parse_flows(source.read())
        named = " ".join(name for mutation in smoke_mutations.SOURCE_MUTATIONS + smoke_mutations.FULL_SOURCE_MUTATIONS + smoke_mutations.SET_FLOW_MUTATIONS for name in mutation[-1])
        not_covered = [template.name for template in templates if template.name.split("{")[0] not in named]
        self.assertEqual(not_covered, smoke_mutations.FLOWS_BROKEN_BY_NO_MUTATION)


class NameTests(unittest.TestCase):
    def test_flow_names_get_their_install(self):
        self.assertEqual(smoke_mutations.names_caught_by(["boot_title", "relaunch@both", "pictures a@remastered differ from b"]), ["flow boot_title@remastered", "flow relaunch@both", "pictures a@remastered differ from b"])

    def test_a_picture_check_runs_both_flows(self):
        self.assertEqual(smoke_mutations.flows_to_run(["pictures language_ja@remastered differ from language_en"]), ["language_ja", "language_en"])

    def test_only_a_failed_line_for_the_named_check_counts(self):
        lines = ["ok    flow a@remastered", "FAIL  flow b@remastered: broke [root causes 1]", "      flow a@remastered"]
        self.assertTrue(smoke_mutations.is_caught(lines, ["flow b@remastered"]))
        self.assertFalse(smoke_mutations.is_caught(lines, ["flow a@remastered"]))
        self.assertTrue(smoke_mutations.is_caught(lines, ["flow b*"]))


class SetCopyTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.real = os.path.join(self.folder.name, "real", "game")
        os.makedirs(os.path.join(self.real, "textures", "0a"))
        for path, text in (("manifest.txt", "art remastered\n"), ("textures/0a/x.gtex", "pixels")):
            with open(os.path.join(self.real, path), "w") as output:
                output.write(text)
        patcher = mock.patch.object(smoke_mutations, "WORK_FOLDER", os.path.join(self.folder.name, "work"))
        patcher.start()
        self.addCleanup(patcher.stop)
        self.copy = smoke_mutations.SetCopy(self.real, "one")

    def real_text(self, path):
        with open(os.path.join(self.real, path)) as source:
            return source.read()

    def test_the_copy_reads_as_the_set(self):
        with open(os.path.join(self.copy.folder, "textures", "0a", "x.gtex")) as source:
            self.assertEqual(source.read(), "pixels")

    def test_a_rewrite_leaves_the_real_set_alone(self):
        self.copy.rewrite("manifest.txt", lambda text: text + "more\n")
        with open(os.path.join(self.copy.folder, "manifest.txt")) as source:
            self.assertEqual(source.read(), "art remastered\nmore\n")
        self.assertEqual(self.real_text("manifest.txt"), "art remastered\n")

    def test_a_removal_leaves_the_real_set_alone(self):
        self.copy.remove("textures/0a/x.gtex")
        self.assertFalse(os.path.exists(os.path.join(self.copy.folder, "textures", "0a", "x.gtex")))
        self.assertEqual(self.real_text("textures/0a/x.gtex"), "pixels")

    def test_a_rewrite_that_changes_nothing_is_refused(self):
        with self.assertRaises(ValueError):
            self.copy.rewrite("manifest.txt", lambda text: text)


if __name__ == "__main__":
    unittest.main()
