import os
import re
import struct
import sys
import unittest

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(TESTS_FOLDER, "..", "..", "..", "tools"))

import lmg_decode
import pack_files
import remaster_scripts

CLASSES, BY_TYPE = remaster_scripts.load_classes()
TYPE_OF = {name: type_hash for type_hash, name in BY_TYPE.items()}
TAIL_TYPE = 0xcc361682
RSZ_AT = 0x40
GUID = "11" * 16
LOST_GUID = "22" * 16


def pad_to(data, alignment):
    data += b"\0" * (-len(data) % alignment)


def record_bytes(data, name, values):
    pad_to(data, 2)
    data += struct.pack("<H", CLASSES[name]["code"])
    for kind, value in zip(CLASSES[name]["fields"], values):
        if kind == "b":
            data += struct.pack("<B", value)
        elif kind == "h":
            pad_to(data, 2)
            data += struct.pack("<H", value & 0xffff)
        elif kind == "w":
            pad_to(data, 4)
            data += struct.pack("<I", value & 0xffffffff)
        elif kind == "x":
            pad_to(data, 4)
            text = (value + "\0").encode("utf-16-le")
            data += struct.pack("<I", len(text) // 2) + text
        elif kind == "g":
            pad_to(data, 8)
            data += bytes.fromhex(value)


def user_file(records, tail=True):
    values = bytearray()
    for name, fields in records:
        record_bytes(values, name, fields)
    types = [0] + [TYPE_OF[name] for name, _ in records] + ([TAIL_TYPE] if tail else [])
    table_at = 0x30
    values_at = table_at + 8 * len(types)
    rsz = bytearray(values_at)
    struct.pack_into("<I", rsz, 12, len(types))
    struct.pack_into("<QQ", rsz, 24, table_at, values_at)
    for index, type_hash in enumerate(types):
        struct.pack_into("<II", rsz, table_at + 8 * index, type_hash, 0)
    head = bytearray(RSZ_AT)
    struct.pack_into("<Q", head, 16, RSZ_AT)
    return bytes(head + rsz + values)


def compiled(records, tables=None, messages=None):
    logged = []
    data = remaster_scripts.compile_script("st01/st01_test_Expand.xml", user_file(records), CLASSES, BY_TYPE, {"iEffect::LAMP": 7}, messages or {GUID: "m01_0010"}, tables or {}, lambda kind, detail: logged.append(kind))
    return lmg_decode.decode(data), logged


def operations(decoded):
    return [item for item in decoded[3] if item[1] == "op"]


def text_at(decoded, word):
    return decoded[1].get(word)


class CompileTests(unittest.TestCase):
    def test_a_block_takes_its_jump_and_jumps_land_on_the_records_they_name(self):
        decoded, logged = compiled([
            ("SC_GHOSTML", []),
            ("SC_SCRIPT", [0]),
            ("SC_JUMP", [6]),
            ("SC_OBJECT", ["lamp"]),
            ("SC_JUMP", [6]),
            ("SC_WAIT", [30]),
            ("SC_SENDMSG", ["EVENT", "light_on", "lamp"]),
            ("SC_JUMP", [5]),
        ])
        ops = operations(decoded)
        self.assertEqual(decoded[4], [])
        self.assertEqual([op[2] for op in ops], [0x15, 0x16, 0x2f, 0x1d, 0x22])
        send = ops[3]
        self.assertEqual([text_at(decoded, word) for word in send[3]], ["lamp", "EVENT", "light_on"])
        start_of = {index: op[0] for index, op in zip((1, 3, 5, 6, 7), ops)}
        self.assertEqual(ops[0][3][1] | ops[0][3][2] << 16, start_of[6])
        self.assertEqual(ops[4][3][0] | ops[4][3][1] << 16, start_of[5])
        self.assertEqual(decoded[1][0], "*")
        self.assertIn(["st01_test"], decoded[2].values())
        self.assertIn(["lamp"], decoded[2].values())
        self.assertEqual(logged, [])

    def test_an_empty_string_becomes_the_placeholder_and_an_empty_effect_name_becomes_the_symbol(self):
        decoded, _ = compiled([
            ("SC_EVENT", ["SYSTEM", ""]),
            ("SC_JUMP", [2]),
            ("SC_EFFECT", ["iEffect::LAMP", 7, 0, 0, 4096, 4096, "", 65535, 65535, 0, 0, 65535, 0, 65535]),
        ])
        event, effect = operations(decoded)
        self.assertEqual(text_at(decoded, event[3][4]), "*")
        self.assertEqual(effect[3][0], 7)
        self.assertEqual(text_at(decoded, effect[3][9]), "iEffect::LAMP")

    def test_message_references_become_the_label_of_the_message_entry(self):
        decoded, logged = compiled([
            ("SC_MESCALL", [2]),
            ("SC_MESCALL", [3]),
            ("SC_MESSAGE", [0, 1, GUID]),
            ("SC_MESSAGE", [0, 1, LOST_GUID]),
        ])
        first, second = operations(decoded)
        self.assertEqual(text_at(decoded, first[3][0]), "m01_0010")
        self.assertEqual(text_at(decoded, second[3][0]), "*")
        self.assertEqual(logged, ["message_guid_has_no_entry"])

    def test_motion_tables_keep_only_changes_and_are_named_from_the_chara_list(self):
        table = [
            ("SC_MTBL", [0xfffd, 1, 30]),
            ("SC_MTBL_CENTER", [0xfff4, -10, 4]),
            ("SC_MTBL_POINT", [0xfff5, 1, 5, 6]),
            ("SC_MTBL_ANIM", [0 | 4 << 10, 4, 0 | 4 << 10]),
            ("SC_MTBL_CENTER", [0xfff4, -10, 4]),
            ("SC_MTBL_POINT", [0xfff5, 1, 5, 6]),
            ("SC_MTBL_ANIM", [1 | 4 << 10, 4, 1 | 4 << 10]),
            ("SC_MTBL_ANIM", [0xfffb, 100, 64]),
            ("SC_MTBL_GMKINFO", [1, 487]),
            ("SC_MTBL_LOOP", [0xfffe, 0]),
        ]
        decoded, _ = compiled([("SC_MOTIONTBL", [2, 0, 1, 0, 0, 0]), ("SC_NULL", [])] + table + [table[0]] + table[-1:], tables={30: "st01_cicel_003"})
        self.assertEqual(decoded[4], [])
        motions = [item for item in decoded[3] if item[1] == "motion"]
        self.assertEqual([record[1] for record in motions[0][2]], [-3, -12, -11, "frame", "frame", -5, -10, -2])
        self.assertEqual(motions[0][2][5][2], [64, 100])
        self.assertEqual(motions[0][2][6][2][0] & 0xffff, 1 << 12 | 487)
        names = {item[0]: decoded[2][item[0]] for item in motions}
        self.assertEqual(names[motions[0][0]], ["st01_cicel_003.mtbl.xml"])
        self.assertEqual(names[motions[1][0]], ["st01_cicel_003_01.mtbl.xml"])
        table_op = operations(decoded)[0]
        self.assertEqual(table_op[3][0] | table_op[3][1] << 16, motions[0][0])

    def test_every_offset_word_is_followed_by_the_high_half_of_the_same_offset(self):
        for name, row in CLASSES.items():
            words = row.get("words", [])
            for index, rule in enumerate(words):
                if rule.startswith("o"):
                    self.assertEqual(words[index + 1:index + 2], ["O" + rule[1:]], name)

    def test_every_field_is_read_by_a_word_rule_or_named_as_unread(self):
        for name, row in CLASSES.items():
            rules = row.get("words", row.get("motion"))
            if rules is None:
                continue
            read = {int(number) for rule in rules if not rule.startswith("c") for number in re.findall(r"\d+", rule)}
            self.assertEqual(sorted(set(range(len(row["fields"]))) - read), sorted(row.get("unread", [])), name)

    def test_a_distance_op_keeps_both_of_its_record_s_offsets(self):
        decoded, _ = compiled([("SC_GET_DIST_X", ["pipe_ofs2", 2, "pipe4", 1])])
        self.assertEqual(operations(decoded)[0][3][1::2], [2, 1])

    def test_a_table_and_a_block_end_past_64_kb_are_reached_whole(self):
        filler = [("SC_WAIT", [1])] * 17000
        records = [("SC_SCRIPT", [0]), ("SC_JUMP", [4 + len(filler) + 2]), ("SC_MOTIONTBL", [4 + len(filler), 0, 1, 0, 0, 0]), ("SC_NULL", [])] + filler + [("SC_MTBL", [0xfffd, 1, 30]), ("SC_MTBL_LOOP", [0xfffe, 0])]
        decoded, _ = compiled(records, tables={30: "st01_cicel_003"})
        ops = operations(decoded)
        table = [item for item in decoded[3] if item[1] == "motion"][0]
        self.assertGreater(table[0], 0xffff)
        self.assertEqual(ops[1][3][0] | ops[1][3][1] << 16, table[0])
        self.assertEqual(ops[0][3][1] | ops[0][3][2] << 16, lmg_decode.CODE_START + decoded[0]["code_size"])

    def test_a_record_class_the_table_does_not_know_is_refused(self):
        data = bytearray(user_file([("SC_WAIT", [1]), ("SC_WAIT", [2])]))
        rsz = struct.unpack_from("<Q", data, 16)[0]
        table_at = struct.unpack_from("<Q", data, rsz + 24)[0]
        struct.pack_into("<I", data, rsz + table_at + 8, pack_files.murmur3_hash(b"app.assets.Script.Classes.GhostSrcEx.SC_NOT_KNOWN"))
        with self.assertRaises(remaster_scripts.RemasterScriptError):
            remaster_scripts.read_records(bytes(data), CLASSES, BY_TYPE)


if __name__ == "__main__":
    unittest.main()
