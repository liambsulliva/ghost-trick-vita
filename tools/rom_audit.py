import sys
import os
import re
import subprocess
import hashlib
import tempfile
import base64

TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_DIR = os.path.dirname(TOOLS_DIR)
RULES_PATH = os.path.join(TOOLS_DIR, "audit_rules.txt")
MANIFEST_PATH = os.path.join(REPO_DIR, "extracted", "nds", "manifest.txt")
LOCAL_KEY_PATH = os.path.join(REPO_DIR, "extracted", "remaster_apk", "text_key.bin")
LOCAL_KEY_NAME = "extracted/remaster_apk/text_key.bin"
RULES_FILE_NAME = "tools/audit_rules.txt"
GITLINK_MODE = b"160000"
TEXT_KEY_SIZE = 16
SELF_TEST_KEY = bytes(range(0xA0, 0xB0))
SELF_TEST_NEAR_KEY = bytes(range(0xA0, 0xA7)) + bytes([0xFF]) + bytes(range(0xA8, 0xAF)) + bytes([0xFF])
KEY_WINDOW_SIZE = 8
KEY_WORD_SIZES = [2, 4]
KEY_DIGEST_NAMES = ["md5", "sha1", "sha256", "sha512"]
HEX_WORD_PATTERN = re.compile(rb"(?<![0-9a-z])(0x|x|u00)?([0-9a-f]+)h?(?![0-9a-z])")
NUMBER_PATTERN = re.compile(rb"[0-9]+")


def load_rules():
    extensions = []
    magics = []
    names = []
    f = open(RULES_PATH, "r")
    for line in f:
        parts = line.strip().split(" ")
        if len(parts) < 2:
            continue
        if parts[0] == "ext":
            extensions.append(parts[1].lower())
        if parts[0] == "name":
            names.append(parts[1].lower())
        if parts[0] == "magic":
            offset = int(parts[1])
            pattern = bytes.fromhex(parts[2])
            label = " ".join(parts[3:])
            magics.append((offset, pattern, label))
    f.close()
    return extensions, magics, names


def load_manifest_hashes():
    hashes = set()
    if not os.path.exists(MANIFEST_PATH):
        return hashes
    f = open(MANIFEST_PATH, "r")
    for line in f:
        parts = line.split()
        if len(parts) > 0:
            hashes.add(parts[0])
    f.close()
    return hashes


def check_file(path, extensions, magics, names, manifest_hashes):
    problems = []
    lower = path.lower()
    for ext in extensions:
        if lower.endswith(ext):
            problems.append("forbidden extension " + ext)
    if os.path.basename(lower) in names:
        problems.append("forbidden file name")
    if not os.path.isfile(path):
        return problems
    f = open(path, "rb")
    data = f.read()
    f.close()
    return problems + data_problems(data, magics, manifest_hashes)


def data_problems(data, magics, manifest_hashes):
    problems = []
    for magic in magics:
        offset = magic[0]
        pattern = magic[1]
        if data[offset:offset + len(pattern)] == pattern:
            problems.append("looks like " + magic[2])
    if len(manifest_hashes) > 0:
        digest = hashlib.sha1(data).hexdigest()
        if digest in manifest_hashes:
            problems.append("same bytes as a file extracted from the ROM")
    return problems


def swapped_words(key, word_size):
    swapped = b""
    for start in range(0, len(key), word_size):
        swapped = swapped + key[start:start + word_size][::-1]
    return swapped


def key_byte_orders(key):
    orders = [key, key[::-1]]
    for word_size in KEY_WORD_SIZES:
        orders.append(swapped_words(key, word_size))
    return orders


def key_windows(key):
    windows = []
    for start in range(len(key) - KEY_WINDOW_SIZE + 1):
        windows.append(key[start:start + KEY_WINDOW_SIZE])
    return windows


def decimal_text(values):
    numbers = []
    for value in values:
        numbers.append(b"%d" % value)
    return b"," + b",".join(numbers) + b","


def octal_text(values):
    parts = []
    for value in values:
        parts.append(b"\\%03o" % value)
    return b"".join(parts)


def encoded_forms(key):
    forms = [
        base64.b64encode(key).rstrip(b"="),
        base64.urlsafe_b64encode(key).rstrip(b"="),
        base64.b32encode(key).rstrip(b"="),
        base64.b32encode(key).rstrip(b"=").lower(),
        base64.b85encode(key),
        base64.a85encode(key),
        b"%d" % int.from_bytes(key, "big"),
    ]
    return forms


def key_search_texts(key):
    search = {"plain": set(), "hex": set(), "decimal": set()}
    for order in key_byte_orders(key):
        for window in key_windows(order):
            search["plain"].add(window)
            search["plain"].add(repr(window)[2:-1].encode("ascii"))
            search["plain"].add(octal_text(window))
            search["hex"].add(window.hex().encode("ascii"))
            search["decimal"].add(decimal_text(window))
        search["plain"].update(encoded_forms(order))
        for name in KEY_DIGEST_NAMES:
            digest = hashlib.new(name, order)
            search["plain"].add(digest.digest())
            search["hex"].add(digest.hexdigest().encode("ascii"))
    return search


def hex_digits_in(lowered):
    digits = []
    for prefix, word in HEX_WORD_PATTERN.findall(lowered):
        if len(word) % 2 == 0:
            digits.append(word)
        elif prefix != b"":
            digits.append(b"0" + word)
    return b"".join(digits)


def numbers_in(data):
    numbers = []
    for number in NUMBER_PATTERN.findall(data):
        numbers.append(number.lstrip(b"0") or b"0")
    return b"," + b",".join(numbers) + b","


def text_holds_key(data, search):
    for text in search["plain"]:
        if text in data:
            return True
    lowered = data.lower()
    digits = hex_digits_in(lowered)
    for text in search["hex"]:
        if text in lowered or text in digits:
            return True
    numbers = numbers_in(data)
    for text in search["decimal"]:
        if text in numbers:
            return True
    return False


def holds_key(path, search):
    if search is None or not os.path.isfile(path):
        return False
    f = open(path, "rb")
    data = f.read()
    f.close()
    return data_holds_key(data, search)


def data_holds_key(data, search):
    if text_holds_key(data, search):
        return True
    if b"\0" in data:
        return text_holds_key(data.replace(b"\0", b""), search)
    return False


def read_local_key():
    if not os.path.isfile(LOCAL_KEY_PATH):
        return None
    f = open(LOCAL_KEY_PATH, "rb")
    key = f.read()
    f.close()
    return key


def repo_files():
    try:
        result = subprocess.run(["git", "ls-files", "-z", "--cached", "--others", "--exclude-standard"], cwd=REPO_DIR, stdout=subprocess.PIPE)
    except OSError:
        return None
    if result.returncode != 0:
        return None
    files = []
    for name in result.stdout.split(b"\0"):
        if name != b"":
            files.append(os.fsdecode(name))
    return files


def staged_entries():
    result = subprocess.run(["git", "ls-files", "-z", "--stage"], cwd=REPO_DIR, stdout=subprocess.PIPE)
    if result.returncode != 0:
        return None
    entries = []
    for line in result.stdout.split(b"\0"):
        if line != b"":
            details, name = line.split(b"\t", 1)
            mode, object_id, _ = details.split(b" ")
            if mode != GITLINK_MODE:
                entries.append((os.fsdecode(name), object_id))
    return entries


def read_staged_blob(reader, object_id):
    reader.stdin.write(object_id + b"\n")
    reader.stdin.flush()
    header = reader.stdout.readline().split()
    if len(header) != 3:
        return None
    data = reader.stdout.read(int(header[2]))
    reader.stdout.read(1)
    return data


def working_copy_matches(path, data):
    if not os.path.isfile(path):
        return False
    f = open(path, "rb")
    working_data = f.read()
    f.close()
    return working_data == data


def staged_copy_problems(name, data, search, magics, manifest_hashes):
    problems = []
    if search is not None and data_holds_key(data, search):
        problems.append("the staged copy holds the text key")
    if name != RULES_FILE_NAME:
        for problem in data_problems(data, magics, manifest_hashes):
            problems.append("the staged copy " + problem)
    return problems


def audit_staged_copies(search, magics, manifest_hashes):
    entries = staged_entries()
    if entries is None:
        return ["FAIL could not list the staged files with git, so they were not checked"]
    failures = []
    reader = subprocess.Popen(["git", "cat-file", "--batch"], cwd=REPO_DIR, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    for name, object_id in entries:
        data = read_staged_blob(reader, object_id)
        if data is None:
            failures.append("FAIL " + printable_name(name) + ": could not read the staged copy")
        elif not working_copy_matches(os.path.join(REPO_DIR, name), data):
            for problem in staged_copy_problems(name, data, search, magics, manifest_hashes):
                failures.append("FAIL " + printable_name(name) + ": " + problem)
    reader.stdin.close()
    reader.stdout.close()
    reader.wait()
    return failures


def printable_name(name):
    return name.encode("ascii", "backslashreplace").decode("ascii")


def audit_tree():
    extensions, magics, names = load_rules()
    manifest_hashes = load_manifest_hashes()
    failed = 0
    search = None
    key = read_local_key()
    if key is None:
        print("key search skipped: no " + LOCAL_KEY_NAME)
    elif len(key) == TEXT_KEY_SIZE:
        search = key_search_texts(key)
    else:
        print("FAIL " + LOCAL_KEY_NAME + ": wrong size, so the audit cannot look for the key")
        failed = failed + 1
    files = repo_files()
    if files is None:
        print("FAIL could not list the files with git, so nothing was checked")
        return 1
    for name in files:
        path = os.path.join(REPO_DIR, name)
        problems = []
        if holds_key(path, search):
            problems.append("holds the text key")
        if name != RULES_FILE_NAME:
            problems = problems + check_file(path, extensions, magics, names, manifest_hashes)
        for problem in problems:
            print("FAIL " + printable_name(name) + ": " + problem)
            failed = failed + 1
    for failure in audit_staged_copies(search, magics, manifest_hashes):
        print(failure)
        failed = failed + 1
    if failed > 0:
        print(str(failed) + " problem(s) found")
        return 1
    print("audit passed")
    return 0


def wrapped(pieces, before, after, between):
    parts = []
    for piece in pieces:
        parts.append(before + piece + after)
    return between.join(parts)


def little_endian_words(key, word_size):
    words = []
    for start in range(0, len(key), word_size):
        value = int.from_bytes(key[start:start + word_size], "little")
        words.append(b"0x%0*x" % (word_size * 2, value))
    return b", ".join(words)


def key_text_forms(key):
    pairs = []
    decimals = []
    padded = []
    short_hex = []
    for value in key:
        pairs.append(b"%02x" % value)
        decimals.append(b"%d" % value)
        padded.append(b"%03d" % value)
        short_hex.append(b"%x" % value)
    back_pairs = []
    back_decimals = []
    for value in key[::-1]:
        back_pairs.append(b"%02x" % value)
        back_decimals.append(b"%d" % value)
    forms = [
        key,
        repr(key).encode("ascii"),
        b"".join(pairs),
        b"".join(pairs).upper(),
        b" ".join(pairs),
        b":".join(pairs),
        b"-".join(pairs).upper(),
        b'"' + b'" "'.join(pairs) + b'"',
        b", ".join(decimals),
        b",".join(decimals),
        b" ".join(decimals),
        b",\n    ".join(decimals),
        wrapped(pairs, b"0x", b"", b", "),
        wrapped(pairs, b"0x", b"", b",").upper(),
        wrapped(pairs, b"\\x", b"", b""),
        key[::-1],
        b"".join(back_pairs),
        wrapped(back_pairs, b"0x", b"", b", "),
        b", ".join(back_decimals),
        base64.b64encode(key),
        base64.b64encode(key).rstrip(b"="),
        base64.urlsafe_b64encode(key),
        base64.b32encode(key),
        base64.b32encode(key).lower(),
        base64.b85encode(key),
        base64.a85encode(key),
        hashlib.sha256(key).hexdigest().encode("ascii"),
        hashlib.sha256(key).hexdigest().upper().encode("ascii"),
        hashlib.sha256(key).digest(),
        hashlib.md5(key).hexdigest().encode("ascii"),
        hashlib.md5(key).digest(),
        key[:KEY_WINDOW_SIZE],
        key[len(key) - KEY_WINDOW_SIZE:],
        b"".join(pairs[:KEY_WINDOW_SIZE]),
        wrapped(pairs[:KEY_WINDOW_SIZE], b"0x", b"", b", "),
        key[:len(key) - 1],
        little_endian_words(key, 4),
        little_endian_words(key, 8),
        b"%d" % int.from_bytes(key, "big"),
        wrapped(pairs, b"\\u00", b"", b""),
        octal_text(key),
        b"\\\n".join(pairs),
        b"/".join(pairs),
        b"|".join(pairs),
        wrapped(pairs, b"0x", b"", b", /* one byte */ "),
        wrapped(pairs, b"(byte)0x", b"", b", "),
        wrapped(pairs, b"(unsigned char)0x", b"", b", "),
        wrapped(pairs, b"{0x", b"}", b","),
        wrapped(pairs, b"<", b">", b""),
        wrapped(pairs, b"", b"h", b", "),
        wrapped(pairs, b"$", b"", b" "),
        b", ".join(padded),
        wrapped(decimals, b"[", b"]", b""),
        b"".join(pairs).decode("ascii").encode("utf-16-le"),
        wrapped(short_hex, b"0x", b"", b", "),
    ]
    return forms


def self_test():
    extensions, magics, names = load_rules()
    folder = tempfile.mkdtemp()
    planted = []
    count = 0
    for ext in extensions:
        path = os.path.join(folder, "planted_" + str(count) + ext)
        f = open(path, "wb")
        f.write(b"x")
        f.close()
        planted.append(path)
        count = count + 1
    for magic in magics:
        path = os.path.join(folder, "planted_" + str(count) + ".txt")
        f = open(path, "wb")
        f.write(bytes(magic[0]) + magic[1] + bytes(16))
        f.close()
        planted.append(path)
        count = count + 1
    for name in names:
        path = os.path.join(folder, name)
        f = open(path, "wb")
        f.write(b"x")
        f.close()
        planted.append(path)
    clean_path = os.path.join(folder, "clean.txt")
    f = open(clean_path, "wb")
    f.write(b"hello\n")
    f.close()

    failed = 0
    for path in planted:
        if len(check_file(path, extensions, magics, names, set())) == 0:
            print("SELF-TEST FAIL: planted file was not caught: " + os.path.basename(path))
            failed = failed + 1
    if len(check_file(clean_path, extensions, magics, names, set())) != 0:
        print("SELF-TEST FAIL: clean file was flagged")
        failed = failed + 1

    search = key_search_texts(SELF_TEST_KEY)
    key_path = os.path.join(folder, "planted_key.txt")
    key_form_count = 0
    for form in key_text_forms(SELF_TEST_KEY):
        f = open(key_path, "wb")
        f.write(b"text before " + form + b" text after\n")
        f.close()
        key_form_count = key_form_count + 1
        if not holds_key(key_path, search):
            print("SELF-TEST FAIL: planted key form number " + str(key_form_count) + " was not caught")
            failed = failed + 1
    for form in key_text_forms(SELF_TEST_NEAR_KEY):
        f = open(key_path, "wb")
        f.write(b"text before " + form + b" text after\n")
        f.close()
        if holds_key(key_path, search):
            print("SELF-TEST FAIL: a different key was flagged")
            failed = failed + 1
    os.remove(key_path)

    for path in planted:
        os.remove(path)
    os.remove(clean_path)
    os.rmdir(folder)
    if failed > 0:
        return 1
    print("self-test passed: " + str(len(planted)) + " planted files and " + str(key_form_count) + " planted key forms caught")
    return 0


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--self-test":
        sys.exit(self_test())
    sys.exit(audit_tree())


main()
