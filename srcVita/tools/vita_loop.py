import argparse
import array
import ftplib
import hashlib
import io
import json
import os
import socket
import struct
import subprocess
import sys
import time

VITA_HOST = os.environ.get("VITA_HOST", "10.0.0.151")
FTP_PORT = 1337
COMMAND_PORT = 1338
TITLE_ID = "GHTR00001"
APP_FOLDER = "/ux0:/app/" + TITLE_ID
DATA_FOLDER = "/ux0:/data/ghosttrick"
INSTALL_FOLDER = DATA_FOLDER + "/install"
LOG_PATH = DATA_FOLDER + "/run.log"
PREVIOUS_LOG_PATH = DATA_FOLDER + "/run_previous.log"
COMMANDS_PATH = DATA_FOLDER + "/commands.txt"
CAPTURE_FOLDER = DATA_FOLDER + "/captures"
BURST_WINDOW_PATH = CAPTURE_FOLDER + "/burst/window.bin"
BURST_SLOTS = 1000
BURST_MAGIC = b"GTB1"
BURST_HEADER_SIZE = 20
BURST_BYTES_PER_PIXEL = 2
SECONDS_FOR_BURST_TO_FINISH_WRITING = 3
JPEG_QUALITY = "70"
BURST_PICTURE_NAME = "f%03d"
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD_FOLDER = os.path.join(SRC_VITA_FOLDER, "build")
ASSETS_FOLDER = DATA_FOLDER + "/assets"
UPLOAD_RECORD_PATH = os.path.join(BUILD_FOLDER, "uploaded_assets.json")
PARTIAL_FILE_SUFFIX = ".new"
SECONDS_FOR_APP_TEARDOWN = 3
SECONDS_FOR_DEVICE_REPLY = 3
SECONDS_FOR_PROMOTE_REPLY = 60
SECONDS_FOR_APP_TO_READ_COMMANDS = 5
SECONDS_TO_WATCH_THE_LOG = 3
COMMAND_FILE_POLL_SECONDS = 0.2
LAUNCH_MARKER = "--- launch"
SCENE_READY_TEXT = "scene ready"
PACKAGE_FILES = [
    ("eboot.bin", "eboot.bin"),
    ("param.sfo", "sce_sys/param.sfo"),
    ("sce_sys/icon0.png", "sce_sys/icon0.png"),
    ("sce_sys/livearea/contents/bg.png", "sce_sys/livearea/contents/bg.png"),
    ("sce_sys/livearea/contents/startup.png", "sce_sys/livearea/contents/startup.png"),
    ("../sce_sys/livearea/contents/template.xml", "sce_sys/livearea/contents/template.xml"),
]


def scene_ready_text(name):
    return SCENE_READY_TEXT + " name=" + name + " "


def capture_done_text(name):
    return "capture name=" + name + " result=0 "


def send_device_command(text, reply_seconds=SECONDS_FOR_DEVICE_REPLY):
    with socket.create_connection((VITA_HOST, COMMAND_PORT), timeout=10) as connection:
        connection.sendall(text.encode("ascii") + b"\n")
        connection.settimeout(reply_seconds)
        reply = b""
        try:
            while True:
                chunk = connection.recv(4096)
                if not chunk:
                    break
                reply += chunk
        except socket.timeout:
            pass
    return reply.decode("ascii", "replace").strip()


def open_ftp():
    connection = ftplib.FTP()
    connection.connect(VITA_HOST, FTP_PORT, timeout=60)
    connection.login("anonymous", "anonymous")
    connection.voidcmd("TYPE I")
    return connection


def make_folders(connection, path):
    built = ""
    for part in path.strip("/").split("/"):
        built = built + "/" + part
        if part.endswith(":"):
            continue
        try:
            connection.mkd(built)
        except ftplib.error_perm:
            pass


def sha256_of_bytes(data):
    return hashlib.sha256(data).hexdigest()


def download_bytes(connection, path):
    output = io.BytesIO()
    connection.retrbinary("RETR " + path, output.write)
    return output.getvalue()


def download_or_none(path):
    connection = open_ftp()
    try:
        return download_bytes(connection, path)
    except ftplib.error_perm:
        return None
    finally:
        connection.close()


def upload_verified(connection, local_path, remote_path):
    with open(local_path, "rb") as source:
        data = source.read()
    temporary_path = remote_path + PARTIAL_FILE_SUFFIX
    connection.storbinary("STOR " + temporary_path, io.BytesIO(data))
    read_back = download_bytes(connection, temporary_path)
    if sha256_of_bytes(read_back) != sha256_of_bytes(data):
        raise RuntimeError("read-back hash differs for " + temporary_path + "; not retrying")
    delete_if_present(connection, remote_path)
    connection.rename(temporary_path, remote_path)
    read_back = download_bytes(connection, remote_path)
    if sha256_of_bytes(read_back) != sha256_of_bytes(data):
        raise RuntimeError("final hash differs for " + remote_path + "; not retrying")
    return sha256_of_bytes(data)


def remote_size_or_none(connection, path):
    try:
        return connection.size(path)
    except ftplib.error_perm:
        return None


def app_is_installed(connection):
    try:
        connection.size(APP_FOLDER + "/eboot.bin")
        return True
    except ftplib.error_perm:
        return False


def stage_package(connection):
    for local_name, remote_name in PACKAGE_FILES:
        remote_path = INSTALL_FOLDER + "/" + remote_name
        make_folders(connection, os.path.dirname(remote_path))
        local_path = os.path.normpath(os.path.join(BUILD_FOLDER, local_name))
        digest = upload_verified(connection, local_path, remote_path)
        print("staged", remote_name, digest[:16])


def first_install():
    connection = open_ftp()
    try:
        if app_is_installed(connection):
            print("already installed")
            return 0
        make_folders(connection, DATA_FOLDER)
        stage_package(connection)
    finally:
        connection.close()
    reply = send_device_command("promote " + INSTALL_FOLDER.replace("/ux0:/", "ux0:", 1), SECONDS_FOR_PROMOTE_REPLY)
    print("promote:", reply)
    return 0 if "Promoted" in reply else 1


def quit_and_wait():
    print("quit:", send_device_command("quit " + TITLE_ID))
    time.sleep(SECONDS_FOR_APP_TEARDOWN)


def upload_eboot():
    quit_and_wait()
    connection = open_ftp()
    try:
        upload_verified(connection, os.path.join(BUILD_FOLDER, "param.sfo"), APP_FOLDER + "/sce_sys/param.sfo")
        digest = upload_verified(connection, os.path.join(BUILD_FOLDER, "eboot.bin"), APP_FOLDER + "/eboot.bin")
    finally:
        connection.close()
    print("deployed eboot", digest[:16])
    return 0


def sha256_of_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as source:
        while True:
            chunk = source.read(1024 * 1024)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest()


def load_upload_record():
    if not os.path.exists(UPLOAD_RECORD_PATH):
        return {}
    with open(UPLOAD_RECORD_PATH, encoding="utf-8") as source:
        return json.load(source)


def save_upload_record(record):
    os.makedirs(os.path.dirname(UPLOAD_RECORD_PATH), exist_ok=True)
    temporary_path = UPLOAD_RECORD_PATH + ".new"
    with open(temporary_path, "w", encoding="utf-8") as output:
        json.dump(record, output, indent=0, sort_keys=True)
    os.replace(temporary_path, UPLOAD_RECORD_PATH)


def list_local_files(folder):
    files = [(os.path.join(folder, "manifest.txt"), "manifest.txt")]
    for folder_name in ("textures", "animations", "scenes", "music", "sounds", "scripts"):
        if not os.path.isdir(os.path.join(folder, folder_name)):
            continue
        for name in sorted(os.listdir(os.path.join(folder, folder_name))):
            if not name.startswith(".") and not name.endswith(PARTIAL_FILE_SUFFIX):
                files.append((os.path.join(folder, folder_name, name), folder_name + "/" + name))
    return files


def loop_files_the_set_no_longer_has(relative_paths):
    stale = []
    for relative_path in relative_paths:
        if relative_path.startswith("music/") and relative_path.endswith(".ogg"):
            loop_path = relative_path[:-len(".ogg")] + ".loop"
            if loop_path not in relative_paths:
                stale.append(loop_path)
    return stale


def upload_assets(folder):
    if not os.path.isfile(os.path.join(folder, "manifest.txt")):
        print("no manifest.txt in: " + folder)
        return 1
    record = load_upload_record()
    uploaded = 0
    skipped = 0
    uploaded_bytes = 0
    started = time.time()
    connection = open_ftp()
    try:
        made_folders = set()
        local_files = list_local_files(folder)
        for local_path, relative_path in local_files:
            digest = sha256_of_file(local_path)
            remote_path = ASSETS_FOLDER + "/" + relative_path
            if record.get(relative_path) == digest and remote_size_or_none(connection, remote_path) == os.path.getsize(local_path):
                skipped = skipped + 1
                continue
            remote_folder = os.path.dirname(remote_path)
            if remote_folder not in made_folders:
                make_folders(connection, remote_folder)
                made_folders.add(remote_folder)
            record.pop(relative_path, None)
            save_upload_record(record)
            upload_verified(connection, local_path, remote_path)
            record[relative_path] = digest
            save_upload_record(record)
            uploaded = uploaded + 1
            uploaded_bytes = uploaded_bytes + os.path.getsize(local_path)
        for loop_path in loop_files_the_set_no_longer_has([relative_path for _, relative_path in local_files]):
            delete_if_present(connection, ASSETS_FOLDER + "/" + loop_path)
            if record.pop(loop_path, None) is not None:
                save_upload_record(record)
    finally:
        connection.close()
    print("assets uploaded", uploaded, "skipped", skipped, "bytes", uploaded_bytes, "seconds", int(time.time() - started))
    return 0


def log_lines():
    data = download_or_none(LOG_PATH)
    if data is None:
        return []
    return data.decode("utf-8", "replace").splitlines()


def latest_run_lines():
    lines = log_lines()
    start = 0
    for index, line in enumerate(lines):
        if LAUNCH_MARKER in line:
            start = index
    return lines[start:]


def count_launch_markers():
    count = 0
    for line in log_lines():
        if LAUNCH_MARKER in line:
            count = count + 1
    return count


def delete_if_present(connection, path):
    try:
        connection.delete(path)
    except ftplib.error_perm:
        pass


def clear_pending_commands():
    connection = open_ftp()
    try:
        delete_if_present(connection, COMMANDS_PATH)
        delete_if_present(connection, COMMANDS_PATH + ".new")
    finally:
        connection.close()


def set_aside_old_log():
    connection = open_ftp()
    try:
        delete_if_present(connection, PREVIOUS_LOG_PATH)
        if remote_size_or_none(connection, LOG_PATH) is not None:
            connection.rename(LOG_PATH, PREVIOUS_LOG_PATH)
    finally:
        connection.close()


def launch(seconds=30):
    clear_pending_commands()
    set_aside_old_log()
    markers_before = count_launch_markers()
    print("launch:", send_device_command("launch " + TITLE_ID))
    deadline = time.time() + seconds
    while time.time() < deadline:
        if count_launch_markers() > markers_before:
            print("new run started")
            return 0
        time.sleep(1)
    print("no new run appeared within", seconds, "seconds")
    return 1


def count_lines_containing(wanted):
    return len([line for line in latest_run_lines() if wanted in line])


def wait_for_text(wanted, seconds, lines_to_skip=0):
    deadline = time.time() + seconds
    while time.time() < deadline:
        matching_lines = [line for line in latest_run_lines() if wanted in line]
        if len(matching_lines) > lines_to_skip:
            return matching_lines[lines_to_skip]
        time.sleep(1)
    return None


class CommandNotRead(RuntimeError):
    pass


def wait_for_app_to_read_commands(connection):
    deadline = time.time() + SECONDS_FOR_APP_TO_READ_COMMANDS
    while time.time() < deadline:
        if remote_size_or_none(connection, COMMANDS_PATH) is None:
            return
        time.sleep(COMMAND_FILE_POLL_SECONDS)
    raise CommandNotRead("the app has not read the previous command in " + COMMANDS_PATH)


def write_app_commands(text):
    connection = open_ftp()
    try:
        wait_for_app_to_read_commands(connection)
        temporary_path = COMMANDS_PATH + ".new"
        connection.storbinary("STOR " + temporary_path, io.BytesIO(text.encode("ascii")))
        delete_if_present(connection, COMMANDS_PATH)
        connection.rename(temporary_path, COMMANDS_PATH)
    finally:
        connection.close()


def log_growth_over(seconds):
    connection = open_ftp()
    try:
        size_before = remote_size_or_none(connection, LOG_PATH)
        time.sleep(seconds)
        return size_before, remote_size_or_none(connection, LOG_PATH)
    finally:
        connection.close()


def commands_file_is_waiting():
    connection = open_ftp()
    try:
        return remote_size_or_none(connection, COMMANDS_PATH) is not None
    finally:
        connection.close()


def request_capture(name, output_folder, seconds=15):
    done_text = capture_done_text(name)
    lines_before = count_lines_containing(done_text)
    write_app_commands("capture " + name + "\n")
    line = wait_for_text(done_text, seconds, lines_before)
    if line is None:
        print("capture not acknowledged:", name)
        return 1
    print(line)
    return pull_capture(name, output_folder)


def pull_capture(name, output_folder):
    data = download_or_none(CAPTURE_FOLDER + "/" + name + ".bmp")
    if data is None:
        print("no capture file:", name)
        return 1
    os.makedirs(output_folder, exist_ok=True)
    path = os.path.join(output_folder, name + ".bmp")
    with open(path, "wb") as output:
        output.write(data)
    print("saved", path, len(data))
    return 0


def every_565_color_as_rgb():
    table = []
    for color in range(65536):
        red = color >> 11
        green = (color >> 5) & 0x3f
        blue = color & 0x1f
        table.append(bytes(((red << 3) | (red >> 2), (green << 2) | (green >> 4), (blue << 3) | (blue >> 2))))
    return table


def split_burst_window(data):
    pictures = []
    offset = 0
    while offset + BURST_HEADER_SIZE <= len(data) and data[offset:offset + len(BURST_MAGIC)] == BURST_MAGIC:
        _, sequence, elapsed_ms, size = struct.unpack_from("<IIII", data, offset + len(BURST_MAGIC))
        width = size & 0xffff
        height = size >> 16
        pixel_bytes = width * height * BURST_BYTES_PER_PIXEL
        if offset + BURST_HEADER_SIZE + pixel_bytes > len(data):
            break
        pictures.append({"sequence": sequence, "elapsed_ms": elapsed_ms, "width": width, "height": height, "pixels": data[offset + BURST_HEADER_SIZE:offset + BURST_HEADER_SIZE + pixel_bytes]})
        offset = offset + BURST_HEADER_SIZE + pixel_bytes
    return pictures


def save_burst_picture(picture, path, rgb_of_color):
    colors = array.array("H")
    colors.frombytes(picture["pixels"])
    rgb = b"".join(map(rgb_of_color.__getitem__, colors))
    subprocess.run(["magick", "-size", "%dx%d" % (picture["width"], picture["height"]), "-depth", "8", "rgb:-", "-quality", JPEG_QUALITY, path], input=rgb, check=True)


def note_picture_time(output_folder, text):
    with open(os.path.join(output_folder, "times.txt"), "a", encoding="ascii") as times:
        times.write(time.strftime("%H:%M:%S") + " " + text + "\n")


def pull_burst_window():
    connection = open_ftp()
    try:
        return download_bytes(connection, BURST_WINDOW_PATH)
    finally:
        connection.close()


def request_burst(seconds, rounds, output_folder):
    os.makedirs(output_folder, exist_ok=True)
    rgb_of_color = every_565_color_as_rgb()
    next_slot = 0
    for round_number in range(1, rounds + 1):
        write_app_commands("burst %d\nresume\n" % seconds)
        time.sleep(seconds + SECONDS_FOR_BURST_TO_FINISH_WRITING)
        started = time.time()
        data = pull_burst_window()
        pulled_seconds = time.time() - started
        pictures = split_burst_window(data)
        for picture in pictures:
            slot_name = BURST_PICTURE_NAME % (next_slot % BURST_SLOTS)
            save_burst_picture(picture, os.path.join(output_folder, slot_name + ".jpg"), rgb_of_color)
            note_picture_time(output_folder, "%s round=%d sequence=%d device_ms=%d" % (slot_name, round_number, picture["sequence"], picture["elapsed_ms"]))
            next_slot = next_slot + 1
        print("burst round", round_number, "pictures", len(pictures), "bytes", len(data), "pulled in %.1f s," % pulled_seconds, "saved by %.1f s" % (time.time() - started), flush=True)
    write_app_commands("resume\n")
    return 0


def build_parser():
    captures_folder = os.path.join(BUILD_FOLDER, "captures")
    parser = argparse.ArgumentParser(description="Installs, launches and reads back from the app on the Vita.")
    actions = parser.add_subparsers(dest="action", required=True)
    actions.add_parser("first-install", help="stage the package and install the app; only needed once")
    actions.add_parser("upload-eboot", help="quit the app and replace its eboot.bin with the local build")
    launch_action = actions.add_parser("launch", help="start the app and wait until its log shows a new run")
    launch_action.add_argument("--seconds", type=int, default=30, help="how long to wait for the new run")
    actions.add_parser("quit", help="quit the app and wait for it to tear down")
    actions.add_parser("log", help="print the log of the latest run")
    capture_action = actions.add_parser("capture", help="ask the app to save the screen, then pull the BMP back")
    capture_action.add_argument("name", help="name of the capture, without .bmp")
    capture_action.add_argument("--seconds", type=int, default=15, help="how long to wait for the app to save it")
    capture_action.add_argument("--output", default=captures_folder, help="local folder for the BMP")
    burst_action = actions.add_parser("burst", help="record half-size pictures five times a second on the Vita, then pause the game while they are pulled")
    burst_action.add_argument("--seconds", type=int, default=60, help="seconds in each recording window; the game pauses after each one while the pictures are pulled")
    burst_action.add_argument("--rounds", type=int, default=1, help="how many recording windows")
    burst_action.add_argument("--output", required=True, help="local folder for the pictures; a scratch folder, because it fills with up to 1000 of each kind")
    pull_action = actions.add_parser("pull", help="pull a capture the app saved earlier")
    pull_action.add_argument("name", help="name of the capture, without .bmp")
    pull_action.add_argument("--output", default=captures_folder, help="local folder for the BMP")
    device_command_action = actions.add_parser("device-command", help="send a line to the Vita Companion command port")
    device_command_action.add_argument("text", help="the line to send")
    app_command_action = actions.add_parser("app-command", help="write a line to the commands file the running app reads")
    app_command_action.add_argument("text", help="the line to write, such as play_scene opening")
    wait_action = actions.add_parser("wait", help="wait until the log of the latest run contains some text")
    wait_action.add_argument("text", help="the text to wait for")
    wait_action.add_argument("--seconds", type=int, default=30, help="how long to wait")
    upload_assets_action = actions.add_parser("upload-assets", help="upload a converted asset set, skipping files the device already has")
    upload_assets_action.add_argument("folder", help="local folder holding manifest.txt")
    return parser


def main():
    arguments = build_parser().parse_args()

    if arguments.action == "first-install":
        return first_install()
    if arguments.action == "upload-eboot":
        return upload_eboot()
    if arguments.action == "launch":
        return launch(arguments.seconds)
    if arguments.action == "quit":
        quit_and_wait()
        return 0
    if arguments.action == "log":
        for line in latest_run_lines():
            print(line)
        return 0
    if arguments.action == "capture":
        return request_capture(arguments.name, arguments.output, arguments.seconds)
    if arguments.action == "burst":
        return request_burst(arguments.seconds, arguments.rounds, arguments.output)
    if arguments.action == "pull":
        return pull_capture(arguments.name, arguments.output)
    if arguments.action == "device-command":
        print(send_device_command(arguments.text))
        return 0
    if arguments.action == "upload-assets":
        return upload_assets(arguments.folder)
    if arguments.action == "app-command":
        write_app_commands(arguments.text + "\n")
        return 0
    if arguments.action == "wait":
        line = wait_for_text(arguments.text, arguments.seconds)
        print(line if line else "timed out waiting for: " + arguments.text)
        return 0 if line else 1
    return 1


if __name__ == "__main__":
    sys.exit(main())
