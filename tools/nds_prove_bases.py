import sys
import os
import struct

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from lz import read_u32


def is_text_byte(b):
    return b >= 0x20 and b < 0x7F


def string_starts(data):
    starts = set()
    i = 0
    while i < len(data):
        if is_text_byte(data[i]) and (i == 0 or data[i - 1] == 0):
            j = i
            while j < len(data) and is_text_byte(data[j]):
                j = j + 1
            if j - i >= 4 and j < len(data) and data[j] == 0:
                starts.add(i)
            i = j
        i = i + 1
    return starts


def is_prologue(data, offset, thumb):
    if thumb:
        if offset < 0 or offset + 2 > len(data) or offset % 2 != 0:
            return False
        half = struct.unpack_from("<H", data, offset)[0]
        return (half & 0xFF00) == 0xB500
    if offset < 0 or offset + 4 > len(data) or offset % 4 != 0:
        return False
    word = read_u32(data, offset)
    return (word & 0xFFFF4000) == 0xE92D4000


def score(data, words, starts, base):
    string_hits = 0
    function_hits = 0
    for word in words:
        offset = word - base
        if offset < 0 or offset >= len(data):
            continue
        if offset in starts:
            string_hits = string_hits + 1
        elif word % 2 == 1 and is_prologue(data, offset - 1, True):
            function_hits = function_hits + 1
        elif word % 4 == 0 and is_prologue(data, offset, False):
            function_hits = function_hits + 1
    return string_hits, function_hits


def prove(name, data, base, words):
    starts = string_starts(data)
    true_strings, true_functions = score(data, words, starts, base)
    true_total = true_strings + true_functions
    best_other = 0
    best_base = base
    for shift in [-0x10000, -0x1000, -0x100, -0x40, -0x20, -0x10, -0x8, -0x4, -0x2, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x100, 0x1000, 0x10000]:
        s, fn = score(data, words, starts, base + shift)
        if s + fn > best_other:
            best_other = s + fn
            best_base = base + shift
    if true_total < 8:
        verdict = "TOO SMALL TO PROVE"
    elif true_total >= best_other * 2:
        verdict = "PROVEN"
    else:
        verdict = "NOT PROVEN"
    print(name.ljust(5) + " base " + hex(base) + ": " + str(true_strings) + " string + " + str(true_functions) + " function pointers = " + str(true_total) + "; best shifted base " + hex(best_base) + ": " + str(best_other) + " -> " + verdict)
    return verdict


def words_of(data):
    words = []
    i = 0
    while i + 4 <= len(data):
        words.append(read_u32(data, i))
        i = i + 4
    return words


def main():
    nds_dir = sys.argv[1]
    segments = []
    f = open(os.path.join(nds_dir, "loadmap.txt"), "r")
    for line in f:
        parts = line.split()
        if parts[0] == "segment" or parts[0] == "overlay":
            seg_file = open(os.path.join(nds_dir, parts[4]), "rb")
            data = seg_file.read()
            seg_file.close()
            segments.append((parts[1], int(parts[2], 16), data))
    f.close()

    main_words = []
    for segment in segments:
        if segment[0] == "main":
            main_words = words_of(segment[2])

    failed = 0
    small = 0
    for segment in segments:
        words = words_of(segment[2])
        if segment[0] != "main":
            words = words + main_words
        verdict = prove(segment[0], segment[2], segment[1], words)
        if verdict == "NOT PROVEN":
            failed = failed + 1
        if verdict == "TOO SMALL TO PROVE":
            small = small + 1
    print(str(len(segments) - failed - small) + " proven, " + str(small) + " too small, " + str(failed) + " not proven")
    if failed > 0:
        sys.exit(1)


main()
