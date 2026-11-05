import argparse
import ftplib
import hashlib
import io
import json
import os
import socket
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
COMMANDS_PATH = DATA_FOLDER + "/commands.txt"
CAPTURE_FOLDER = DATA_FOLDER + "/captures"
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD_FOLDER = os.path.join(SRC_VITA_FOLDER, "build")
ASSETS_FOLDER = DATA_FOLDER + "/assets"
UPLOAD_RECORD_PATH = os.path.join(BUILD_FOLDER, "uploaded_assets.json")
SECONDS_FOR_APP_TEARDOWN = 3
PACKAGE_FILES = [
    ("eboot.bin", "eboot.bin"),
    ("param.sfo", "sce_sys/param.sfo"),
    ("sce_sys/icon0.png", "sce_sys/icon0.png"),
    ("sce_sys/livearea/contents/bg.png", "sce_sys/livearea/contents/bg.png"),
    ("sce_sys/livearea/contents/startup.png", "sce_sys/livearea/contents/startup.png"),
    ("../sce_sys/livearea/contents/template.xml", "sce_sys/livearea/contents/template.xml"),
]


def send_command(text):
    with socket.create_connection((VITA_HOST, COMMAND_PORT), timeout=10) as connection:
        connection.sendall(text.encode("ascii") + b"\n")
        connection.settimeout(3)
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
    temporary_path = remote_path + ".new"
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


def install():
    connection = open_ftp()
    try:
        if app_is_installed(connection):
            print("already installed")
            return 0
        make_folders(connection, DATA_FOLDER)
        stage_package(connection)
    finally:
        connection.close()
    reply = send_command("promote " + INSTALL_FOLDER.replace("/ux0:/", "ux0:", 1))
    print("promote:", reply)
    return 0 if "Promoted" in reply else 1


def quit_and_wait():
    print("quit:", send_command("quit " + TITLE_ID))
    time.sleep(SECONDS_FOR_APP_TEARDOWN)


def deploy():
    quit_and_wait()
    connection = open_ftp()
    try:
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
    with open(UPLOAD_RECORD_PATH, "w", encoding="utf-8") as output:
        json.dump(record, output, indent=0, sort_keys=True)


def list_local_files(folder):
    files = []
    for root, _, names in os.walk(folder):
        for name in sorted(names):
            local_path = os.path.join(root, name)
            files.append((local_path, os.path.relpath(local_path, folder).replace(os.sep, "/")))
    return files


def upload_assets(folder):
    record = load_upload_record()
    uploaded = 0
    skipped = 0
    uploaded_bytes = 0
    started = time.time()
    connection = open_ftp()
    try:
        made_folders = set()
        for local_path, relative_path in list_local_files(folder):
            digest = sha256_of_file(local_path)
            if record.get(relative_path) == digest:
                skipped = skipped + 1
                continue
            remote_path = ASSETS_FOLDER + "/" + relative_path
            remote_folder = os.path.dirname(remote_path)
            if remote_folder not in made_folders:
                make_folders(connection, remote_folder)
                made_folders.add(remote_folder)
            upload_verified(connection, local_path, remote_path)
            record[relative_path] = digest
            save_upload_record(record)
            uploaded = uploaded + 1
            uploaded_bytes = uploaded_bytes + os.path.getsize(local_path)
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
        if "--- launch" in line:
            start = index
    return lines[start:]


def count_launch_markers():
    count = 0
    for line in log_lines():
        if "--- launch" in line:
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


def launch(seconds=30):
    clear_pending_commands()
    markers_before = count_launch_markers()
    print("launch:", send_command("launch " + TITLE_ID))
    deadline = time.time() + seconds
    while time.time() < deadline:
        if count_launch_markers() > markers_before:
            print("new run started")
            return 0
        time.sleep(1)
    print("no new run appeared within", seconds, "seconds")
    return 1


def wait_for_text(wanted, seconds):
    deadline = time.time() + seconds
    while time.time() < deadline:
        for line in latest_run_lines():
            if wanted in line:
                return line
        time.sleep(1)
    return None


def write_commands(text):
    connection = open_ftp()
    try:
        temporary_path = COMMANDS_PATH + ".new"
        connection.storbinary("STOR " + temporary_path, io.BytesIO(text.encode("ascii")))
        delete_if_present(connection, COMMANDS_PATH)
        connection.rename(temporary_path, COMMANDS_PATH)
    finally:
        connection.close()


def request_capture(name, output_folder, seconds=15):
    write_commands("capture " + name + "\n")
    line = wait_for_text("capture name=" + name + " ", seconds)
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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("action", choices=["install", "deploy", "launch", "quit", "log", "capture", "pull", "command", "wait", "tell-app", "upload-assets"])
    parser.add_argument("argument", nargs="?", default="")
    parser.add_argument("--output", default=os.path.join(BUILD_FOLDER, "captures"))
    parser.add_argument("--seconds", type=int, default=30)
    arguments = parser.parse_args()

    if arguments.action == "install":
        return install()
    if arguments.action == "deploy":
        return deploy()
    if arguments.action == "launch":
        return launch()
    if arguments.action == "quit":
        print(send_command("quit " + TITLE_ID))
        return 0
    if arguments.action == "log":
        for line in latest_run_lines():
            print(line)
        return 0
    if arguments.action == "capture":
        return request_capture(arguments.argument, arguments.output)
    if arguments.action == "pull":
        return pull_capture(arguments.argument, arguments.output)
    if arguments.action == "command":
        print(send_command(arguments.argument))
        return 0
    if arguments.action == "upload-assets":
        return upload_assets(arguments.argument)
    if arguments.action == "tell-app":
        write_commands(arguments.argument + "\n")
        return 0
    if arguments.action == "wait":
        line = wait_for_text(arguments.argument, arguments.seconds)
        print(line if line else "timed out waiting for: " + arguments.argument)
        return 0 if line else 1
    return 1


if __name__ == "__main__":
    sys.exit(main())
