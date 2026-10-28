import sys
import os
import subprocess
import hashlib
import tempfile

TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_DIR = os.path.dirname(TOOLS_DIR)
RULES_PATH = os.path.join(TOOLS_DIR, "audit_rules.txt")
MANIFEST_PATH = os.path.join(REPO_DIR, "extracted", "nds", "manifest.txt")


def load_rules():
    extensions = []
    magics = []
    f = open(RULES_PATH, "r")
    for line in f:
        parts = line.strip().split(" ")
        if len(parts) < 2:
            continue
        if parts[0] == "ext":
            extensions.append(parts[1].lower())
        if parts[0] == "magic":
            offset = int(parts[1])
            pattern = bytes.fromhex(parts[2])
            label = " ".join(parts[3:])
            magics.append((offset, pattern, label))
    f.close()
    return extensions, magics


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


def check_file(path, extensions, magics, manifest_hashes):
    problems = []
    lower = path.lower()
    for ext in extensions:
        if lower.endswith(ext):
            problems.append("forbidden extension " + ext)
    if not os.path.isfile(path):
        return problems
    f = open(path, "rb")
    data = f.read()
    f.close()
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


def repo_files():
    result = subprocess.run(["git", "ls-files", "--cached", "--others", "--exclude-standard"], cwd=REPO_DIR, stdout=subprocess.PIPE, text=True)
    files = []
    for line in result.stdout.splitlines():
        if line.strip() != "":
            files.append(line.strip())
    return files


def audit_tree():
    extensions, magics = load_rules()
    manifest_hashes = load_manifest_hashes()
    failed = 0
    for name in repo_files():
        if name == "tools/audit_rules.txt":
            continue
        problems = check_file(os.path.join(REPO_DIR, name), extensions, magics, manifest_hashes)
        for problem in problems:
            print("FAIL " + name + ": " + problem)
            failed = failed + 1
    if failed > 0:
        print(str(failed) + " problem(s) found")
        return 1
    print("audit passed")
    return 0


def self_test():
    extensions, magics = load_rules()
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
    clean_path = os.path.join(folder, "clean.txt")
    f = open(clean_path, "wb")
    f.write(b"hello\n")
    f.close()

    failed = 0
    for path in planted:
        if len(check_file(path, extensions, magics, set())) == 0:
            print("SELF-TEST FAIL: planted file was not caught: " + os.path.basename(path))
            failed = failed + 1
    if len(check_file(clean_path, extensions, magics, set())) != 0:
        print("SELF-TEST FAIL: clean file was flagged")
        failed = failed + 1
    for path in planted:
        os.remove(path)
    os.remove(clean_path)
    os.rmdir(folder)
    if failed > 0:
        return 1
    print("self-test passed: " + str(len(planted)) + " planted files caught")
    return 0


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--self-test":
        sys.exit(self_test())
    sys.exit(audit_tree())


main()
