import os
import subprocess
import time

import gdb_remote

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MELONDS = os.path.join(REPO_DIR, "extracted", "melonds", "melonDS.app", "Contents", "MacOS", "melonDS")
CONFIG = os.path.join(REPO_DIR, "extracted", "melonds", "portable", "melonDS.toml")


def make_default_config(rom_path):
    os.makedirs(os.path.dirname(CONFIG), exist_ok=True)
    log = open(os.path.join(REPO_DIR, "extracted", "melonds", "default_run.log"), "w")
    process = subprocess.Popen([MELONDS, rom_path], stdout=log, stderr=log)
    time.sleep(6)
    process.send_signal(2)
    process.wait(15)
    log.close()


def set_gdb_options():
    f = open(CONFIG, "r")
    lines = f.read().split("\n")
    f.close()
    section = ""
    new_lines = []
    for line in lines:
        text = line.strip()
        if text.startswith("["):
            section = text
        if section == "[Instance0.Gdb]" and text.startswith("Enabled"):
            line = "Enabled = true"
        if section == "[Instance0.Gdb.ARM9]" and text.startswith("BreakOnStartup"):
            line = "BreakOnStartup = true"
        if section == "[Instance0.Gdb.ARM7]" and text.startswith("BreakOnStartup"):
            line = "BreakOnStartup = false"
        if section == "[JIT]" and text.startswith("Enable "):
            line = "Enable = false"
        new_lines.append(line)
    f = open(CONFIG, "w")
    f.write("\n".join(new_lines))
    f.close()


def start(rom_path, log_name):
    if not os.path.exists(CONFIG) or os.path.getsize(CONFIG) == 0:
        make_default_config(rom_path)
    set_gdb_options()
    log = open(os.path.join(REPO_DIR, "extracted", "melonds", log_name), "w")
    process = subprocess.Popen([MELONDS, rom_path], stdout=log, stderr=log)
    sock = gdb_remote.connect(3333, 30)
    if sock is None:
        process.terminate()
        try:
            process.wait(10)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait(10)
        log.close()
        return None, None, None
    return process, sock, log


def stop(process, sock, log):
    try:
        gdb_remote.send_packet(sock, "k")
        sock.close()
    except OSError:
        pass
    time.sleep(1)
    if process.poll() is None:
        process.send_signal(2)
    try:
        process.wait(10)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait(10)
    log.close()
