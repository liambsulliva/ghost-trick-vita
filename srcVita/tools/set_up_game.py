import argparse
import os
import shutil
import subprocess
import sys
import threading
import time
import zipfile

TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(TOOLS_FOLDER)
REPOSITORY_FOLDER = os.path.dirname(SRC_VITA_FOLDER)
APK_FOLDER = os.path.join(REPOSITORY_FOLDER, "extracted", "remaster_apk")
DEFAULT_DS_ROM = os.path.join(REPOSITORY_FOLDER, "extracted", "ds_rom", "game.nds")
SET_NAME = "game"
OUTPUT_FOLDER = os.path.join(REPOSITORY_FOLDER, "extracted", "vita_assets", SET_NAME)
TEXT_KEY_PATH = os.path.join(APK_FOLDER, "text_key.bin")
DEFAULT_VITASDK = "/usr/local/vitasdk"
PACK_NAMES = ("re_chunk_000.pak", "data01", "data02")
APK_PACK_FOLDER = "assets/"
COPY_CHUNK_BYTES = 4 * 1024 * 1024
BAR_WIDTH = 30
REDRAW_SECONDS = 0.2
TAIL_LINES_ON_FAILURE = 20
STATUS_WIDTH = 100
STAGES = (
    "Check the tools",
    "Copy the game files out of the Android package",
    "Find the text key",
    "Build the converter",
    "Convert the art, sound, text and DS scripts",
    "Plan the loading",
    "Build the package",
    "Copy everything to the Vita",
)


class SetupFailed(Exception):
    pass


def is_terminal():
    return sys.stdout.isatty()


def show(text, end="\n"):
    sys.stdout.write(text + end)
    sys.stdout.flush()


def show_status(text):
    if is_terminal():
        show("\r\033[K" + text[:STATUS_WIDTH], end="")


def finish_status(text):
    if is_terminal():
        show("\r\033[K" + text)
    else:
        show(text)


def stage_header(number, detail=""):
    show("")
    show("[%d/%d] %s%s" % (number, len(STAGES), STAGES[number - 1], (" - " + detail) if detail else ""))


def size_text(byte_count):
    for unit, size in (("GB", 1 << 30), ("MB", 1 << 20), ("KB", 1 << 10)):
        if byte_count >= size:
            return "%.1f %s" % (byte_count / size, unit)
    return "%d B" % byte_count


def bar_text(done, total, started):
    share = done / total if total else 1.0
    filled = int(BAR_WIDTH * share)
    elapsed = time.time() - started
    rate = done / elapsed if elapsed > 0 else 0
    return "  [%s%s] %3d%%  %s of %s  %s/s" % ("#" * filled, "." * (BAR_WIDTH - filled), int(share * 100), size_text(done), size_text(total), size_text(int(rate)))


def check_tools():
    missing = [name for name in ("make", "clang", "magick") if shutil.which(name) is None]
    if missing:
        raise SetupFailed("install these first: " + ", ".join(missing))
    formats = subprocess.run(["magick", "-list", "format"], capture_output=True, text=True).stdout
    if "OTF" not in formats:
        raise SetupFailed("ImageMagick has no FreeType support: magick -list format shows no OTF line")
    show("  make, clang and magick with FreeType are installed")


def pack_members(apk_path):
    with zipfile.ZipFile(apk_path) as package:
        found = {os.path.basename(info.filename): info for info in package.infolist() if info.filename.startswith(APK_PACK_FOLDER) and os.path.basename(info.filename) in PACK_NAMES}
    missing = [name for name in PACK_NAMES if name not in found]
    if missing:
        raise SetupFailed("the package has no " + ", ".join(APK_PACK_FOLDER + name for name in missing) + "; these steps need one APK with all three")
    return found


def packs_already_copied():
    return all(os.path.exists(os.path.join(APK_FOLDER, name)) for name in PACK_NAMES)


def copy_packs(apk_path):
    members = pack_members(apk_path)
    os.makedirs(APK_FOLDER, exist_ok=True)
    total = sum(members[name].file_size for name in PACK_NAMES)
    done = 0
    started = time.time()
    last_redraw = 0.0
    with zipfile.ZipFile(apk_path) as package:
        for name in PACK_NAMES:
            info = members[name]
            target = os.path.join(APK_FOLDER, name)
            if os.path.exists(target) and os.path.getsize(target) == info.file_size:
                done = done + info.file_size
                continue
            partial = target + ".part"
            with package.open(info) as source, open(partial, "wb") as output:
                while True:
                    chunk = source.read(COPY_CHUNK_BYTES)
                    if not chunk:
                        break
                    output.write(chunk)
                    done = done + len(chunk)
                    if time.time() - last_redraw >= REDRAW_SECONDS:
                        show_status(bar_text(done, total, started) + "  " + name)
                        last_redraw = time.time()
            os.replace(partial, target)
    finish_status(bar_text(total, total, started))


def run_with_progress(command, label, environment=None):
    lines = []
    process = subprocess.Popen(command, cwd=REPOSITORY_FOLDER, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace", env=environment)

    def read_output():
        for line in process.stdout:
            lines.append(line.rstrip("\n"))

    reader = threading.Thread(target=read_output, daemon=True)
    reader.start()
    started = time.time()
    spinner = "|/-\\"
    turn = 0
    while process.poll() is None:
        latest = lines[-1].strip() if lines else ""
        show_status("  %s %s  %ds  %s" % (spinner[turn % len(spinner)], label, int(time.time() - started), latest))
        turn = turn + 1
        time.sleep(REDRAW_SECONDS)
    reader.join()
    seconds = time.time() - started
    if process.returncode != 0:
        finish_status("  %s failed after %ds" % (label, int(seconds)))
        for line in lines[-TAIL_LINES_ON_FAILURE:]:
            show("    " + line)
        raise SetupFailed(label + " failed")
    finish_status("  %s done in %ds" % (label, int(seconds)))
    return lines


def find_text_key(apk_path):
    run_with_progress([sys.executable, "-I", os.path.join(TOOLS_FOLDER, "assets", "extract_text_key.py"), apk_path], "searching the package")


def convert(ds_rom):
    lines = run_with_progress([sys.executable, "-I", os.path.join(TOOLS_FOLDER, "assets", "convert_assets.py"), "--set", SET_NAME, "--output", OUTPUT_FOLDER, "--ds-rom", ds_rom], "converting")
    for line in lines:
        if line.startswith(("textures:", "music:", "sounds:", "scripts:", "talk:")):
            show("    " + line)


def vitasdk_folder():
    folder = os.environ.get("VITASDK", DEFAULT_VITASDK)
    return folder if os.path.isdir(os.path.join(folder, "bin")) else None


def build_package(folder):
    environment = dict(os.environ)
    environment["VITASDK"] = folder
    environment["PATH"] = os.path.join(folder, "bin") + os.pathsep + environment.get("PATH", "")
    run_with_progress(["make", "-C", "srcVita", "-j8"], "building", environment)
    show("  package: srcVita/build/ghosttrick.vpk")


def run_stages(arguments):
    stage_header(1)
    check_tools()

    if packs_already_copied():
        stage_header(2, "already in extracted/remaster_apk, skipped")
    else:
        if arguments.apk is None:
            raise SetupFailed("name your copy of the Android package with --apk")
        stage_header(2)
        copy_packs(arguments.apk)

    if os.path.exists(TEXT_KEY_PATH):
        stage_header(3, "already found, skipped")
    else:
        if arguments.apk is None:
            raise SetupFailed("name your copy of the Android package with --apk, so the text key can be found")
        stage_header(3)
        find_text_key(arguments.apk)

    stage_header(4)
    run_with_progress(["make", "-C", "srcVita/tools/assets"], "building the converter")

    if not os.path.exists(arguments.ds_rom):
        raise SetupFailed("no DS ROM at %s; the app plays the opening from the DS game's own scripts, so name your own USA cartridge dump with --ds-rom" % arguments.ds_rom)
    stage_header(5)
    convert(arguments.ds_rom)

    stage_header(6)
    run_with_progress(["make", "-C", "srcVita", "read-ahead", "PLAN_SET=" + SET_NAME], "replaying the game")

    folder = vitasdk_folder()
    if folder is None:
        stage_header(7, "VitaSDK not found, skipped")
    else:
        stage_header(7)
        build_package(folder)

    stage_header(8)
    show("  install srcVita/build/ghosttrick.vpk with VitaShell")
    show("  copy what is inside extracted/vita_assets/%s to ux0:data/ghosttrick/assets/" % SET_NAME)


def main():
    parser = argparse.ArgumentParser(description="Prepares everything the Vita app needs from your own copies of the game, in one go.")
    parser.add_argument("--apk", help="your copy of the remaster's Android package; not needed once its files are copied and the key is found")
    parser.add_argument("--ds-rom", default=DEFAULT_DS_ROM, help="your own dump of the USA DS cartridge (game code BGTE)")
    arguments = parser.parse_args()
    started = time.time()
    try:
        run_stages(arguments)
    except SetupFailed as failure:
        show("")
        show("Stopped: " + str(failure))
        return 1
    except KeyboardInterrupt:
        show("")
        show("Stopped by you. Running it again carries on from where it stopped.")
        return 130
    show("")
    show("Finished in %ds." % int(time.time() - started))
    return 0


if __name__ == "__main__":
    sys.exit(main())
