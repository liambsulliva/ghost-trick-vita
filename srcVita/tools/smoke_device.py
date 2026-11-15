import os
import shutil
import time

import smoke_test
import vita_loop

PLAYER_FILES = ["save_auto.txt", "save_manual.txt", "settings.txt", "record_reveal.txt"] + ["chapter_%02d.txt" % chapter for chapter in range(1, 19)]
SET_MANIFESTS = {"remastered": vita_loop.ASSETS_FOLDER + "/manifest.txt", "ds": vita_loop.DS_ASSETS_FOLDER + "/manifest.txt"}
FRAMES_PER_SECOND = 60
SECONDS_OVER_THE_FRAME_BUDGET = 90
SECONDS_FOR_RELAUNCH_TO_SETTLE = 5
LOG_POLL_SECONDS_AT_FIRST = 2
LOG_POLL_SECONDS_MOST = 15


class VitaTarget(smoke_test.MacTarget):
    flows_at_once = 1

    def __init__(self, set_folders, state_folders, output_folder, host_path=None):
        super().__init__(None, set_folders, state_folders, output_folder)
        self.backup_folder = os.path.join(output_folder, "device_backup")
        self.backed_up = None
        self.app_is_up = False
        self.install = None
        self.lines_pulled = 0

    def device_install(self):
        connection = vita_loop.open_ftp()
        try:
            present = [kind for kind in ("remastered", "ds") if vita_loop.remote_size_or_none(connection, SET_MANIFESTS[kind]) is not None]
        finally:
            connection.close()
        for install, kinds in smoke_test.INSTALLS.items():
            if sorted(kinds) == sorted(present):
                return install
        return None

    def flows_it_can_run(self, flows):
        self.install = self.device_install()
        kept = [flow for flow in flows if flow.installs[0] == self.install]
        left_out = []
        for flow in flows:
            if flow.installs[0] != self.install:
                result = smoke_test.Result("flow " + smoke_test.flow_key(flow), flow.guards)
                result.is_skipped = True
                result.detail = "the Vita holds the %s install" % (self.install or "no")
                left_out.append(result)
        if kept:
            self.back_up_player_files()
        return kept, left_out

    def back_up_player_files(self):
        vita_loop.quit_and_wait()
        shutil.rmtree(self.backup_folder, ignore_errors=True)
        os.makedirs(self.backup_folder)
        self.backed_up = []
        for name in PLAYER_FILES:
            data = vita_loop.download_or_none(vita_loop.DATA_FOLDER + "/" + name)
            if data is not None:
                with open(os.path.join(self.backup_folder, name), "wb") as output:
                    output.write(data)
                self.backed_up.append(name)

    def put_player_files(self, folder, names):
        connection = vita_loop.open_ftp()
        try:
            for name in PLAYER_FILES:
                vita_loop.delete_if_present(connection, vita_loop.DATA_FOLDER + "/" + name)
            for name in names:
                with open(os.path.join(folder, name), "rb") as source:
                    connection.storbinary("STOR " + vita_loop.DATA_FOLDER + "/" + name, source)
        finally:
            connection.close()

    def finish(self):
        if self.backed_up is None:
            return
        vita_loop.quit_and_wait()
        self.put_player_files(self.backup_folder, self.backed_up)
        self.backed_up = None

    def prepare(self, flow):
        sandbox = super().prepare(flow)
        vita_loop.quit_and_wait()
        self.app_is_up = False
        self.put_player_files(sandbox, [name for name in PLAYER_FILES if os.path.isfile(os.path.join(sandbox, name))])
        return sandbox

    def run_launch(self, flow, sandbox, lines):
        if not self.app_is_up:
            vita_loop.quit_and_wait()
            if vita_loop.launch() != 0:
                return smoke_test.EXIT_OUT_OF_FRAMES, "the app did not start"
            self.lines_pulled = 0
        self.app_is_up = False
        markers_before = vita_loop.count_launch_markers()
        ends_before = self.ends_in(vita_loop.latest_run_lines(), flow)
        try:
            vita_loop.write_app_commands("smoke %s\n%s\n" % (flow.name, "\n".join(lines)))
        except vita_loop.CommandNotRead as error:
            return smoke_test.EXIT_OUT_OF_FRAMES, str(error)
        code = self.wait_for_end(flow, markers_before, ends_before)
        self.pull(flow, sandbox)
        return code, ""

    def ends_in(self, lines, flow):
        return len([line for line in lines if "smoke ok name=%s " % flow.name in line or "smoke fail name=%s " % flow.name in line])

    def wait_for_end(self, flow, markers_before, ends_before):
        deadline = time.time() + flow.frames_most / FRAMES_PER_SECOND + SECONDS_OVER_THE_FRAME_BUDGET
        pause = LOG_POLL_SECONDS_AT_FIRST
        while time.time() < deadline:
            if vita_loop.count_launch_markers() > markers_before:
                time.sleep(SECONDS_FOR_RELAUNCH_TO_SETTLE)
                self.app_is_up = True
                return smoke_test.EXIT_RELAUNCH
            if self.ends_in(vita_loop.latest_run_lines(), flow) > ends_before:
                return smoke_test.EXIT_FLOW_OVER
            time.sleep(pause)
            pause = min(pause * 2, LOG_POLL_SECONDS_MOST)
        return smoke_test.EXIT_OUT_OF_FRAMES

    def pull(self, flow, sandbox):
        lines = vita_loop.log_lines()
        with open(os.path.join(sandbox, "run.log"), "a", encoding="utf-8") as output:
            output.write("".join(line + "\n" for line in lines[self.lines_pulled:]))
        self.lines_pulled = len(lines)
        for name in smoke_test.capture_names(flow):
            data = vita_loop.download_or_none(vita_loop.CAPTURE_FOLDER + "/" + name + ".bmp")
            if data is not None:
                with open(self.capture_path(sandbox, name), "wb") as output:
                    output.write(data)


def run(arguments, write=print):
    arguments.no_package = True
    return smoke_test.run(arguments, write, VitaTarget)
