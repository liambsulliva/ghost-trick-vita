import argparse
import concurrent.futures
import fnmatch
import json
import os
import re
import shutil
import subprocess
import sys
import zipfile

TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(TOOLS_FOLDER)
sys.path.insert(0, os.path.join(TOOLS_FOLDER, "assets"))

import output_folders

SMOKE_FOLDER = os.path.join(SRC_VITA_FOLDER, "tests", "smoke")
FLOWS_PATH = os.path.join(SMOKE_FOLDER, "flows.txt")
LOG_KINDS_PATH = os.path.join(SMOKE_FOLDER, "log_kinds.txt")
KNOWN_GAPS_PATH = os.path.join(SMOKE_FOLDER, "known_gaps.txt")
LANGUAGE_SOURCE = os.path.join(SRC_VITA_FOLDER, "src", "app", "asset_language.c")
SET_DESCRIPTIONS = {"remastered": "game.json", "ds": "game.json"}
SET_FOLDERS = {"remastered": "game", "ds": "game_ds"}
SET_ART_LINES = {"remastered": "art remastered", "ds": "art ds"}
HOST_ARGUMENTS = {"remastered": "--remastered", "ds": "--ds"}
INSTALLS = {"remastered": ("remastered",), "ds": ("ds",), "both": ("remastered", "ds")}
REPLAY_STOP_TICK = 4200000
STEPS_THE_STORY_PASSES_OVER = 1
EXIT_FLOW_OVER = 0
EXIT_OUT_OF_FRAMES = 3
EXIT_RELAUNCH = 10
DEFAULT_FRAMES_MOST = 400000
SCREEN_WIDTH = 960
SCREEN_HEIGHT = 544
BMP_HEADER_BYTES = 54
LEAST_LIT_SHARE = 0.003
LEAST_DIFFERENT_PIXELS = 200
LOG_LINE_PATTERN = re.compile(r"^\[\s*\d+\.\d+\] (.*)$")
STEP_DONE_PATTERN = re.compile(r"route_step_done name=(\d+):")
PACKAGE_FILES = ("eboot.bin", "sce_sys/param.sfo", "sce_sys/icon0.png", "sce_sys/livearea/contents/bg.png", "sce_sys/livearea/contents/startup.png", "sce_sys/livearea/contents/template.xml")
USUAL_SDK_FOLDER = "/usr/local/vitasdk"
PACKAGE_TEXTS = (b"smoke begin name=", b"GHTR00001")
FAILURE_LINES_KEPT = 40
BUTTONS = {"up": 0x1, "down": 0x2, "left": 0x4, "right": 0x8, "cross": 0x10, "circle": 0x20, "square": 0x40, "triangle": 0x80, "l": 0x100, "r": 0x200, "start": 0x400, "select": 0x800}


class Flow:
    def __init__(self, name):
        self.name = name
        self.about = ""
        self.guards = ""
        self.installs = ["remastered"]
        self.needs_full = False
        self.start_state = None
        self.chapters = 0
        self.shows = []
        self.settings = {}
        self.frames_most = DEFAULT_FRAMES_MOST
        self.launches = []
        self.log_has = []
        self.log_lacks = []
        self.known_lines = []
        self.route_steps = None
        self.differs_from = None
        self.loop = None


class Result:
    def __init__(self, name, guards=""):
        self.name = name
        self.guards = guards
        self.problems = []
        self.detail = ""
        self.is_skipped = False
        self.evidence = []
        self.known = []

    def fail(self, problem):
        self.problems.append(problem)

    @property
    def passed(self):
        return not self.problems


def language_codes():
    with open(LANGUAGE_SOURCE, encoding="utf-8") as source:
        row = re.search(r"codes\[ASSET_LANGUAGE_COUNT\] = \{([^}]*)\}", source.read()).group(1)
    return re.findall(r'"([a-z]+)"', row)


def read_lines(path):
    with open(path, encoding="utf-8", errors="replace") as source:
        return source.read().splitlines()


def parse_flows(text):
    flows = []
    blocks = {}
    block = None
    flow = None
    launch = None
    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line:
            continue
        word, _, rest = line.partition(" ")
        if word == "block":
            block = blocks.setdefault(rest, [])
            flow = None
            launch = None
        elif block is not None and word == "use":
            block += blocks[rest]
        elif block is not None and word != "flow":
            block.append(line)
        elif launch is not None and word == "use":
            if rest not in blocks:
                raise ValueError("flow %s uses the block %s, which is not written above it" % (flow.name, rest))
            launch["lines"] += blocks[rest]
        elif word == "flow":
            block = None
            flow = Flow(rest)
            launch = None
            flows.append(flow)
        elif flow is None:
            raise ValueError("line before the first flow: %s" % line)
        elif word == "launch":
            launch = {"lines": [], "relaunches": rest == "relaunches"}
            flow.launches.append(launch)
        elif launch is not None:
            launch["lines"].append(line)
        elif word == "about":
            flow.about = rest
        elif word == "guards":
            flow.guards = rest
        elif word == "sets":
            flow.installs = rest.split()
        elif word == "needs" and rest == "full":
            flow.needs_full = True
        elif word == "start":
            flow.start_state = rest
        elif word == "chapters":
            flow.chapters = int(rest)
        elif word == "shows":
            flow.shows += rest.split()
        elif word == "settings":
            flow.settings = dict(pair.split("=") for pair in rest.split())
        elif word == "frames":
            flow.frames_most = int(rest)
        elif word == "log_has":
            flow.log_has.append(rest)
        elif word == "log_lacks":
            flow.log_lacks.append(rest)
        elif word == "known":
            pattern, _, reason = rest.partition(" | ")
            flow.known_lines.append((pattern.strip(), reason.strip()))
        elif word == "route_steps":
            flow.route_steps = rest
        elif word == "differs_from":
            flow.differs_from = rest
        elif word == "for":
            flow.loop = rest
        else:
            raise ValueError("unknown flow line: %s" % line)
    return flows


def filled(text, values):
    def value_of(match):
        if match.group(1) not in values:
            raise KeyError("flow names {%s}, which this set does not have" % match.group(1))
        return str(values[match.group(1)])
    return re.sub(r"\{([a-z_0-9]+)\}", value_of, text)


def lines_for_install(lines, install, values):
    kept = []
    for line in lines:
        prefix, separator, rest = line.partition(": ")
        if separator and prefix in INSTALLS:
            if prefix != install:
                continue
            line = rest
        line = filled(line, values)
        if line.startswith("press "):
            line = "press %x" % sum(BUTTONS[name] for name in line.split()[1:])
        if line.strip():
            kept.append(line)
    return kept


def instance_of(template, install, values):
    flow = Flow(filled(template.name, values))
    flow.about = filled(template.about, values)
    flow.guards = template.guards
    flow.installs = [install]
    flow.needs_full = template.needs_full
    flow.start_state = filled(template.start_state, values) if template.start_state else None
    flow.chapters = template.chapters
    flow.shows = list(template.shows)
    flow.settings = {key: filled(value, values) for key, value in template.settings.items()}
    flow.frames_most = template.frames_most
    flow.launches = [{"lines": lines_for_install(launch["lines"], install, values), "relaunches": launch["relaunches"]} for launch in template.launches]
    flow.log_has = [filled(text, values) for text in template.log_has]
    flow.log_lacks = [filled(text, values) for text in template.log_lacks]
    flow.known_lines = list(template.known_lines)
    flow.route_steps = filled(template.route_steps, values) if template.route_steps else None
    flow.differs_from = filled(template.differs_from, values) if template.differs_from else None
    return flow


class Story:
    def __init__(self, marks, route_steps):
        self.steps = route_steps
        self.events = []
        visits = {}
        script = None
        for what, name in marks:
            if what == "script_begun":
                visits[name] = visits.get(name, 0) + 1
                script = name
                self.events.append(("scene", name, visits[name] == 1))
            elif what == "route_step_done":
                self.events.append(("step", int(name.split(":", 1)[0]), script))
        self.parts = []
        self.scene_marks = {}

    def use_states(self, state_rows):
        self.scene_marks = {script: mark for script, mark, _ in state_rows}
        seen = set()
        self.parts = []
        for script, mark, _ in state_rows:
            if mark not in seen:
                seen.add(mark)
                self.parts.append((mark, script))

    def first_visit_position(self, scene):
        for position, event in enumerate(self.events):
            if event[0] == "scene" and event[1] == scene and event[2]:
                return position
        raise KeyError("the replay never entered %s" % scene)

    def steps_between(self, first_scene, last_scene=None, last_step=None):
        done = []
        for event in self.events[self.first_visit_position(first_scene):]:
            if event[0] == "scene" and last_scene is not None and event[1] == last_scene and event[2]:
                break
            if event[0] == "step":
                done.append(event[1])
                if last_step is not None and event[1] == last_step:
                    break
        return done

    def scene_before_step(self, step):
        scene = None
        for event in self.events:
            if event[0] == "scene" and event[2]:
                scene = event[1]
            if event[0] == "step" and event[1] == step:
                return scene
        raise KeyError("the replay never finished step %d" % step)

    def first_step_of_each_action(self):
        firsts = {}
        for index, step in enumerate(self.steps):
            action = step[2]
            if action not in firsts and any(event[0] == "step" and event[1] == index for event in self.events):
                firsts[action] = index
        return firsts

    def step_names(self, indexes):
        return ",".join(str(index) for index in indexes)


def read_marks(lines):
    marks = []
    for line in lines:
        words = line.split()
        if len(words) >= 3 and words[0] == "mark":
            marks.append((words[1], words[2]))
    return marks


def read_route_steps(path):
    return [line.split() for line in read_lines(path) if line.startswith("step ")]


def read_state_rows(path):
    rows = []
    for line in read_lines(path):
        script, mark, tick = line.split()
        rows.append((script, int(mark), int(tick)))
    return rows


def loop_values(loop, story):
    if loop is None:
        return [{}]
    variable, _, source = loop.partition(" in ")
    rows = []
    if source == "route_actions":
        for action, step in sorted(story.first_step_of_each_action().items(), key=lambda item: item[1]):
            scene = story.scene_before_step(step)
            rows.append({"action": action, "scene": scene, "step": step, "steps": story.step_names(story.steps_between(scene, last_step=step))})
    elif source == "story_parts":
        for position, (mark, scene) in enumerate(story.parts):
            next_scene = story.parts[position + 1][1] if position + 1 < len(story.parts) else None
            rows.append({
                "part": "%02d" % mark,
                "scene": scene,
                "stop_line": "ff_until script %s" % next_scene if next_scene else "",
                "end_log": "script reached" if next_scene else "title open",
                "steps": story.step_names(story.steps_between(scene, last_scene=next_scene)),
            })
    elif source == "languages":
        for index, code in enumerate(language_codes()):
            rows.append({"language": code, "language_index": index})
    elif source == "other_languages":
        for index, code in enumerate(language_codes()):
            if index > 0:
                rows.append({"language": code, "language_index": index})
    else:
        raise ValueError("unknown loop %s" % loop)
    if variable.strip() == "":
        raise ValueError("loop without a name: %s" % loop)
    return rows


def story_values(story):
    values = {"all_steps": story.step_names(story.steps_between(story.events[0][1]) if story.events else [])}
    for mark, scene in story.parts:
        values["part_%02d_scene" % mark] = scene
    for action, step in story.first_step_of_each_action().items():
        scene = story.scene_before_step(step)
        values[action + "_step"] = step
        values[action + "_scene"] = scene
        values[action + "_steps"] = story.step_names(story.steps_between(scene, last_step=step))
        values[action + "_next_step"] = step + 1
    retry = next((index for index, step in enumerate(story.steps) if "retried" in step[-1]), None)
    if retry is not None:
        try:
            scene = story.scene_before_step(retry)
            values["retry_scene"] = scene
            values["retry_step"] = retry
            values["retry_steps"] = story.step_names(story.steps_between(scene, last_step=retry))
        except KeyError:
            pass
    return values


def expand_flows(templates, stories, is_quick, only):
    flows = []
    for template in templates:
        if is_quick and template.needs_full:
            continue
        for install in template.installs:
            story = stories.get(INSTALLS[install][0])
            if story is None:
                continue
            for values in loop_values(template.loop, story):
                merged = dict(story_values(story))
                merged.update(values)
                merged["set"] = install
                flow = instance_of(template, install, merged)
                if only and not any(fnmatch.fnmatch(flow.name, pattern) for pattern in only):
                    continue
                flows.append(flow)
    return flows


def read_log_kinds(path=LOG_KINDS_PATH):
    rules = []
    for line in read_lines(path):
        words = line.partition(" | ")[0].split()
        if len(words) >= 3:
            rules.append((words[0], " ".join(words[1:3]), " ".join(words[3:])))
    return rules


def kind_of(message):
    words = message.split()
    if not words:
        return ""
    first = words[0].split("=")[0]
    second = words[1].split("=")[0] if len(words) > 1 else ""
    return (first + " " + second).strip()


def rules_for_kind(kind, rules):
    return [(kind_class, rest) for kind_class, pattern, rest in rules if fnmatch.fnmatchcase(kind, pattern)]


def class_of_message(message, kind_rules):
    for kind_class, rest in kind_rules:
        if not rest or fnmatch.fnmatchcase(message, rest):
            return kind_class
    return "new"


def log_messages(lines):
    messages = []
    for line in lines:
        match = LOG_LINE_PATTERN.match(line)
        if match:
            messages.append(match.group(1))
    return messages


def log_kind_problems(messages, rules):
    counts = {}
    examples = {}
    rules_of = {}
    for message in messages:
        kind = kind_of(message)
        if kind not in rules_of:
            rules_of[kind] = rules_for_kind(kind, rules)
        kind_class = class_of_message(message, rules_of[kind])
        if kind_class in ("ok", "known"):
            continue
        key = (kind_class, kind)
        if key not in counts:
            counts[key] = 0
            examples[key] = message
        counts[key] += 1
    problems = []
    for key in sorted(counts):
        if key[0] == "fail":
            problems.append("log has %d of '%s', e.g. %s" % (counts[key], key[1], examples[key]))
        else:
            problems.append("log has a kind tests/smoke/log_kinds.txt does not list: '%s', e.g. %s" % (key[1], examples[key]))
    return problems


def take_known_lines(messages, known):
    kept = messages
    found = []
    for pattern, reason in known:
        matching = [message for message in kept if fnmatch.fnmatchcase(message, pattern)]
        kept = [message for message in kept if not fnmatch.fnmatchcase(message, pattern)]
        found.append((pattern, reason, len(matching)))
    return kept, found


def steps_done_in(messages):
    return [int(match.group(1)) for match in (STEP_DONE_PATTERN.search(message) for message in messages) if match]


def route_problems(messages, wanted_text):
    wanted = [int(word) for word in wanted_text.split(",") if word]
    done = steps_done_in(messages)
    if done == wanted:
        return []
    missing = [step for step in wanted if step not in done]
    extra = [step for step in done if step not in wanted]
    if missing:
        return ["route steps not finished: %s (the replay finishes %d here, the app finished %d)" % (",".join(str(step) for step in missing[:12]), len(wanted), len(done))]
    if extra:
        return ["route steps finished that the replay does not finish here: %s" % ",".join(str(step) for step in extra[:12])]
    return ["route steps finished in another order than the replay's"]


def read_bmp(path):
    with open(path, "rb") as source:
        data = source.read()
    if data[:2] != b"BM" or len(data) < BMP_HEADER_BYTES:
        return None
    width = int.from_bytes(data[18:22], "little")
    height = int.from_bytes(data[22:26], "little", signed=True)
    offset = int.from_bytes(data[10:14], "little")
    bits = int.from_bytes(data[28:30], "little")
    if bits != 24 or width <= 0 or height == 0:
        return None
    return width, abs(height), data[offset:offset + width * abs(height) * 3]


def picture_problems(path, must_show):
    name = os.path.basename(path)
    if not os.path.isfile(path):
        return ["picture %s was not written" % name]
    picture = read_bmp(path)
    if picture is None:
        return ["picture %s is not a 24-bit BMP" % name]
    width, height, pixels = picture
    if (width, height) != (SCREEN_WIDTH, SCREEN_HEIGHT):
        return ["picture %s is %dx%d, not the screen's size" % (name, width, height)]
    if not must_show:
        return []
    lit = 0
    for offset in range(0, len(pixels), 3 * 7):
        if pixels[offset] > 24 or pixels[offset + 1] > 24 or pixels[offset + 2] > 24:
            lit += 1
    if lit < LEAST_LIT_SHARE * (len(pixels) // (3 * 7)):
        return ["picture %s is almost all black" % name]
    return []


def pictures_differ(first_path, second_path):
    first = read_bmp(first_path)
    second = read_bmp(second_path)
    if first is None or second is None:
        return False
    different = 0
    for offset in range(0, min(len(first[2]), len(second[2])), 3):
        if first[2][offset:offset + 3] != second[2][offset:offset + 3]:
            different += 1
            if different >= LEAST_DIFFERENT_PIXELS:
                return True
    return False


def capture_names(flow):
    names = []
    for launch in flow.launches:
        for line in launch["lines"]:
            if line.startswith("capture "):
                names.append(line[len("capture "):])
    return names


def settings_text(settings):
    names = ("brightness", "master", "music", "effects", "music_type", "language", "unlock_all", "fps_cap", "vsync", "high_contrast", "visual_filter", "text_speed", "hint_delay")
    defaults = {"brightness": 10, "master": 10, "music": 10, "effects": 10, "music_type": 0, "language": 0, "unlock_all": 0, "fps_cap": 60, "vsync": 1, "high_contrast": 0, "visual_filter": 0, "text_speed": 1, "hint_delay": 2}
    values = dict(defaults)
    for key, value in settings.items():
        if key not in defaults:
            raise ValueError("unknown setting %s" % key)
        values[key] = int(value)
    return "".join("%s %d\n" % (name, values[name]) for name in names)


def flow_key(flow):
    return "%s@%s" % (flow.name, flow.installs[0])


class MacTarget:
    def __init__(self, host_path, set_folders, state_folders, output_folder):
        self.host_path = host_path
        self.set_folders = set_folders
        self.state_folders = state_folders
        self.output_folder = output_folder

    flows_at_once = 64

    def flows_it_can_run(self, flows):
        return flows, []

    def finish(self):
        pass

    def sandbox_of(self, flow):
        return os.path.join(self.output_folder, "run", flow_key(flow).replace("/", "_"))

    def prepare(self, flow):
        sandbox = self.sandbox_of(flow)
        shutil.rmtree(sandbox, ignore_errors=True)
        os.makedirs(os.path.join(sandbox, "captures"))
        if flow.settings:
            with open(os.path.join(sandbox, "settings.txt"), "w", encoding="ascii") as output:
                output.write(settings_text(flow.settings))
        if flow.start_state:
            state_folder = self.state_folders[INSTALLS[flow.installs[0]][0]]
            shutil.copyfile(os.path.join(state_folder, flow.start_state + ".txt"), os.path.join(sandbox, "save_auto.txt"))
        for chapter, scene in self.chapter_scenes(flow):
            shutil.copyfile(os.path.join(self.state_folders[INSTALLS[flow.installs[0]][0]], scene + ".txt"), os.path.join(sandbox, "chapter_%02d.txt" % chapter))
        return sandbox

    def chapter_scenes(self, flow):
        rows = read_state_rows(os.path.join(self.state_folders[INSTALLS[flow.installs[0]][0]], "states.txt")) if flow.chapters else []
        scenes = {}
        for script, mark, _ in rows:
            if 1 <= mark <= flow.chapters and mark not in scenes:
                scenes[mark] = script
        return sorted(scenes.items())

    def run_launch(self, flow, sandbox, lines):
        with open(os.path.join(sandbox, "commands.txt"), "w", encoding="ascii") as output:
            output.write("smoke %s\n%s\n" % (flow.name, "\n".join(lines)))
        command = [self.host_path, "--sandbox", sandbox, "--frames-most", str(flow.frames_most)]
        for set_kind in INSTALLS[flow.installs[0]]:
            command += [HOST_ARGUMENTS[set_kind], self.set_folders[set_kind]]
        finished = subprocess.run(command, capture_output=True, text=True, errors="replace")
        return finished.returncode, finished.stderr

    def log_lines(self, sandbox):
        path = os.path.join(sandbox, "run.log")
        return read_lines(path) if os.path.isfile(path) else []

    def capture_path(self, sandbox, name):
        return os.path.join(sandbox, "captures", name + ".bmp")


def run_flow(flow, target, rules):
    result = Result("flow " + flow_key(flow), flow.guards)
    result.detail = flow.about
    sandbox = target.prepare(flow)
    for number, launch in enumerate(flow.launches, 1):
        code, errors = target.run_launch(flow, sandbox, launch["lines"])
        wanted = EXIT_RELAUNCH if launch["relaunches"] else EXIT_FLOW_OVER
        if code != wanted:
            reason = {EXIT_OUT_OF_FRAMES: "ran out of frames", EXIT_RELAUNCH: "relaunched", EXIT_FLOW_OVER: "ended without relaunching"}.get(code, "stopped with code %d" % code)
            result.fail("launch %d %s" % (number, reason))
            result.evidence += errors.splitlines()[-FAILURE_LINES_KEPT:]
            break
    messages = log_messages(target.log_lines(sandbox))
    failures = [message for message in messages if message.startswith("smoke fail ")]
    for message in failures:
        result.fail(message)
    finished = len([message for message in messages if message.startswith("smoke ok ")])
    wanted_finishes = len([launch for launch in flow.launches if not launch["relaunches"]])
    if not failures and not result.problems and finished < wanted_finishes:
        result.fail("%d of %d launches finished their lines" % (finished, wanted_finishes))
    unknown_messages, known_found = take_known_lines(messages, flow.known_lines)
    for problem in log_kind_problems(unknown_messages, rules):
        result.fail(problem)
    for pattern, reason, count in known_found:
        if count == 0:
            result.fail("the known line '%s' no longer appears, so take it out of the flow" % pattern)
        else:
            result.known.append("%s (%d lines)" % (reason, count))
    for text in flow.log_has:
        if not any(text in message for message in messages):
            result.fail("log lacks '%s'" % text)
    for text in flow.log_lacks:
        if any(text in message for message in messages):
            result.fail("log has '%s'" % text)
    if flow.route_steps is not None:
        for problem in route_problems(messages, flow.route_steps):
            result.fail(problem)
    for name in capture_names(flow):
        for problem in picture_problems(target.capture_path(sandbox, name), name in flow.shows):
            result.fail(problem)
    result.sandbox = sandbox
    if result.problems and not result.evidence:
        result.evidence = messages[-FAILURE_LINES_KEPT:]
    return result


def difference_results(flows, results, target):
    by_key = {flow_key(flow): flow for flow in flows}
    extra = []
    for flow in flows:
        if flow.differs_from is None or flow.differs_from == flow.name:
            continue
        other = by_key.get("%s@%s" % (flow.differs_from, flow.installs[0]))
        result = Result("pictures %s differ from %s" % (flow_key(flow), flow.differs_from), flow.guards)
        if other is None:
            result.fail("flow %s was not run" % flow.differs_from)
        else:
            same = [name for name in capture_names(flow) if not pictures_differ(target.capture_path(target.sandbox_of(flow), name), target.capture_path(target.sandbox_of(other), name))]
            if same:
                result.fail("pictures the same as %s's: %s" % (flow.differs_from, ", ".join(same)))
        extra.append(result)
    return extra


def check_manifest(set_kind, set_folder):
    result = Result("set %s manifest" % set_kind, "240")
    path = os.path.join(set_folder, "manifest.txt")
    if not os.path.isfile(path):
        result.fail("no manifest.txt in %s" % set_folder)
        return result
    lines = read_lines(path)
    if SET_ART_LINES[set_kind] not in lines:
        result.fail("manifest lacks the line '%s'" % SET_ART_LINES[set_kind])
    missing = []
    backgrounds = [line.split(" ", 1)[1] for line in lines if line.startswith("background ")]
    for name in backgrounds:
        if not os.path.isfile(os.path.join(set_folder, output_folders.disk_path_of("textures/%s.gtex" % name))):
            missing.append(name)
    if missing:
        result.fail("%d of %d backgrounds have no texture, e.g. %s" % (len(missing), len(backgrounds), missing[0]))
    if not backgrounds:
        result.fail("manifest lists no backgrounds")
    result.detail = "%d backgrounds" % len(backgrounds)
    return result


def texture_folders_of(set_folder):
    folders = [os.path.join(set_folder, output_folders.TEXTURE_FOLDER)]
    language_root = os.path.join(set_folder, output_folders.LANGUAGE_FOLDER)
    if os.path.isdir(language_root):
        for code in sorted(os.listdir(language_root)):
            folder = os.path.join(language_root, code, output_folders.TEXTURE_FOLDER)
            if os.path.isdir(folder):
                folders.append(folder)
    return folders


def check_texture_buckets(set_kind, set_folder):
    result = Result("set %s texture folders" % set_kind, "224")
    count = 0
    for folder in texture_folders_of(set_folder):
        if not os.path.isdir(folder):
            result.fail("no folder %s" % folder)
            continue
        for entry in sorted(os.listdir(folder)):
            path = os.path.join(folder, entry)
            if output_folders.is_texture_file_name(entry):
                result.fail("%s sits outside a bucket" % os.path.relpath(path, set_folder))
                break
            if not os.path.isdir(path):
                continue
            for file_name in os.listdir(path):
                if not output_folders.is_texture_file_name(file_name):
                    continue
                count += 1
                if output_folders.texture_bucket_of(file_name[:-len(output_folders.TEXTURE_SUFFIX)]) != entry:
                    result.fail("%s is in the wrong bucket" % os.path.relpath(os.path.join(path, file_name), set_folder))
                    break
    if count == 0:
        result.fail("no textures")
    result.detail = "%d textures" % count
    return result


def check_languages(set_kind, set_folder):
    result = Result("set %s languages" % set_kind, "230 234")
    codes = language_codes()[1:]
    total = 0
    for code in codes:
        folder = os.path.join(set_folder, output_folders.LANGUAGE_FOLDER, code)
        names_path = os.path.join(folder, "names.txt")
        if not os.path.isfile(names_path):
            result.fail("language %s has no names.txt" % code)
            continue
        names = [line.strip() for line in read_lines(names_path) if line.strip()]
        textures = [name for name in names if name.startswith(output_folders.TEXTURE_FOLDER + "/")]
        missing = [name for name in names if not os.path.isfile(os.path.join(folder, output_folders.disk_path_of(name)))]
        total += len(names)
        if missing:
            result.fail("language %s lists %d files that are not on disk, e.g. %s" % (code, len(missing), missing[0]))
        if not textures:
            result.fail("language %s lists no textures" % code)
        if len(names) == len(textures):
            result.fail("language %s lists no text files" % code)
    result.detail = "%d languages, %d files" % (len(codes), total)
    return result


def ogg_frame_count(path):
    with open(path, "rb") as source:
        data = source.read()
    page = data.rfind(b"OggS")
    if page < 0 or page + 14 > len(data):
        return None
    return int.from_bytes(data[page + 6:page + 14], "little")


def loop_point_problem(loop_path):
    words = " ".join(read_lines(loop_path)).split()
    frames = ogg_frame_count(loop_path[:-len(".loop")] + ".ogg") if os.path.isfile(loop_path[:-len(".loop")] + ".ogg") else None
    if frames is None:
        return "has no sound beside it"
    if len(words) != 2 or not all(word.isdigit() for word in words):
        return "does not hold two frame numbers"
    if int(words[0]) > int(words[1]) or int(words[1]) >= frames:
        return "loops from %s to %s in a sound of %d frames" % (words[0], words[1], frames)
    return None


def check_loop_points(set_kind, set_folder):
    result = Result("set %s loop points" % set_kind)
    count = 0
    for folder, _, file_names in sorted(os.walk(set_folder)):
        for file_name in sorted(file_names):
            if file_name.endswith(".loop"):
                count += 1
                problem = loop_point_problem(os.path.join(folder, file_name))
                if problem:
                    result.fail("%s %s, so the app plays it without its loop" % (os.path.relpath(os.path.join(folder, file_name), set_folder), problem))
    result.detail = "%d loop files" % count
    return result


def check_route_is_current(set_kind, set_folder, description_path):
    import script_assets

    result = Result("set %s route is the repository's" % set_kind, "225")
    with open(description_path, encoding="utf-8") as source:
        wanted = script_assets.route_steps_text(json.load(source)["route"])
    path = os.path.join(set_folder, "scripts", "route_steps.txt")
    if not os.path.isfile(path):
        result.fail("the set has no scripts/route_steps.txt")
        return result
    with open(path, encoding="utf-8") as source:
        baked = source.read()
    if baked != wanted:
        baked_lines = baked.splitlines()
        wanted_lines = wanted.splitlines()
        first = next((index for index, pair in enumerate(zip(baked_lines, wanted_lines)) if pair[0] != pair[1]), min(len(baked_lines), len(wanted_lines)))
        result.fail("the set's route differs from %s at step %d (%d steps against %d); convert the set again" % (os.path.basename(description_path), first, len(baked_lines), len(wanted_lines)))
    result.detail = "%d steps" % len(baked.splitlines())
    return result


def story_from(marks_text, scripts, state_folder, result):
    story = Story(read_marks(marks_text.splitlines()), read_route_steps(os.path.join(scripts, "route_steps.txt")))
    states_path = os.path.join(state_folder, "states.txt")
    if os.path.isfile(states_path):
        story.use_states(read_state_rows(states_path))
    else:
        result.fail("the replay saved no states")
    result.detail = "%d steps finished, %d scenes" % (len([event for event in story.events if event[0] == "step"]), len(story.scene_marks))
    return story


def earlier_replay(set_kind, set_folder, state_folder):
    result = Result("replay %s whole game" % set_kind, "225")
    marks_path = os.path.join(state_folder, "marks.txt")
    if not os.path.isfile(marks_path):
        result.fail("no earlier replay in %s" % state_folder)
        return result, None
    with open(marks_path, encoding="utf-8") as source:
        story = story_from(source.read(), os.path.join(set_folder, "scripts"), state_folder, result)
    result.detail += ", from the earlier replay"
    return result, story


def run_replay(set_kind, set_folder, state_folder, replay_path):
    result = Result("replay %s whole game" % set_kind, "225")
    shutil.rmtree(state_folder, ignore_errors=True)
    os.makedirs(state_folder)
    scripts = os.path.join(set_folder, "scripts")
    finished = subprocess.run([replay_path, "--scripts", scripts, "--timing", "remaster", "--auto-paced", "--marks", "--stop-tick", str(REPLAY_STOP_TICK), "--save-states-into", state_folder], capture_output=True, text=True, errors="replace")
    marks_path = os.path.join(state_folder, "marks.txt")
    with open(marks_path, "w", encoding="utf-8") as output:
        output.write(finished.stdout)
    if finished.returncode != 0:
        result.fail("the replay stopped with code %d" % finished.returncode)
        result.evidence = (finished.stdout + finished.stderr).splitlines()[-FAILURE_LINES_KEPT:]
        return result, None
    checked = subprocess.run([sys.executable, "-I", os.path.join(TOOLS_FOLDER, "route_check.py"), marks_path, os.path.join(scripts, "route_steps.txt"), "--most-passed-over", str(STEPS_THE_STORY_PASSES_OVER), "--must-reach", "GOTO"], capture_output=True, text=True)
    if checked.returncode != 0:
        result.fail("the route check failed")
        result.evidence = checked.stdout.splitlines()[-FAILURE_LINES_KEPT:]
    return result, story_from(finished.stdout, scripts, state_folder, result)


def check_package(output_folder):
    result = Result("vita debug package", "233")
    sdk = os.environ.get("VITASDK", "") or USUAL_SDK_FOLDER
    if not os.path.isdir(sdk):
        if os.environ.get("SMOKE_NO_SDK") == "1":
            result.is_skipped = True
            result.detail = "VitaSDK not found and SMOKE_NO_SDK=1"
        else:
            result.fail("VitaSDK not found; set VITASDK, or SMOKE_NO_SDK=1 to leave the Vita link out")
        return result
    environment = dict(os.environ, VITASDK=sdk, PATH=os.path.join(sdk, "bin") + os.pathsep + os.environ.get("PATH", ""))
    built = subprocess.run(["make", "-C", SRC_VITA_FOLDER, "-j8", "DEBUG=1", "smoke-package"], capture_output=True, text=True, errors="replace", env=environment)
    if built.returncode != 0:
        result.fail("the debug build did not link or package")
        result.evidence = (built.stdout + built.stderr).splitlines()[-FAILURE_LINES_KEPT:]
        return result
    package_path = os.path.join(SRC_VITA_FOLDER, "build", "smoke", "package", "ghosttrick-debug.vpk")
    for problem in package_problems(package_path):
        result.fail(problem)
    result.detail = "%d bytes" % os.path.getsize(package_path)
    return result


def package_problems(package_path):
    if not os.path.isfile(package_path):
        return ["no package at %s" % package_path]
    problems = []
    with zipfile.ZipFile(package_path) as package:
        names = set(package.namelist())
        for name in PACKAGE_FILES:
            if name not in names:
                problems.append("the package lacks %s" % name)
            elif package.getinfo(name).file_size == 0:
                problems.append("the package's %s is empty" % name)
        if "eboot.bin" in names and not problems:
            listing = package.read("sce_sys/param.sfo")
            if PACKAGE_TEXTS[1] not in listing:
                problems.append("param.sfo lacks the title id")
    elf_path = os.path.join(os.path.dirname(package_path), "ghosttrick-debug.elf")
    if not os.path.isfile(elf_path):
        problems.append("no linked app beside the package")
    else:
        with open(elf_path, "rb") as source:
            if PACKAGE_TEXTS[0] not in source.read():
                problems.append("the debug app does not hold the smoke runner")
    return problems


def read_known_gaps(path=KNOWN_GAPS_PATH):
    gaps = {}
    if not os.path.isfile(path):
        return gaps
    for line in read_lines(path):
        pattern, _, reason = line.partition(" | ")
        if pattern.strip():
            gaps[pattern.strip()] = reason.strip()
    return gaps


def gap_of(name, gaps):
    for pattern, reason in gaps.items():
        if fnmatch.fnmatchcase(name, pattern):
            return pattern, reason
    return None, None


def write_evidence(result, output_folder):
    folder = os.path.join(output_folder, "failures")
    os.makedirs(folder, exist_ok=True)
    path = os.path.join(folder, re.sub(r"[^A-Za-z0-9_.@-]+", "_", result.name) + ".txt")
    with open(path, "w", encoding="utf-8") as output:
        output.write("%s\nguards: %s\n\n" % (result.name, result.guards))
        for problem in result.problems:
            output.write("problem: %s\n" % problem)
        if getattr(result, "sandbox", None):
            output.write("\nsandbox: %s\n" % result.sandbox)
        output.write("\nlast lines:\n%s\n" % "\n".join(result.evidence))
    return path


def report(results, gaps, output_folder, write=print):
    failed = 0
    used_gaps = set()
    lines = []
    for result in results:
        pattern, reason = gap_of(result.name, gaps)
        if result.is_skipped:
            lines.append("skip  %s: %s" % (result.name, result.detail))
        elif result.passed and pattern is not None:
            failed += 1
            used_gaps.add(pattern)
            lines.append("FAIL  %s: passes now, so take '%s' out of tests/smoke/known_gaps.txt" % (result.name, pattern))
        elif result.passed:
            lines.append("ok    %s" % result.name)
            for known in result.known:
                lines.append("      known: %s" % known)
        elif pattern is not None:
            used_gaps.add(pattern)
            lines.append("known %s: %s [%s]" % (result.name, reason, result.problems[0]))
        else:
            failed += 1
            path = write_evidence(result, output_folder)
            guards = " [root causes %s]" % result.guards if result.guards else ""
            lines.append("FAIL  %s: %s%s" % (result.name, result.problems[0], guards))
            for problem in result.problems[1:4]:
                lines.append("      %s" % problem)
            lines.append("      details in %s" % os.path.relpath(path, SRC_VITA_FOLDER))
    for line in lines:
        write(line)
    counts = (len([r for r in results if r.passed and not r.is_skipped]), failed, len([r for r in results if not r.passed and gap_of(r.name, gaps)[0] is not None]), len([r for r in results if r.is_skipped]))
    summary = "smoke test: %d ok, %d failed, %d known gaps, %d skipped" % counts
    write(summary)
    os.makedirs(output_folder, exist_ok=True)
    with open(os.path.join(output_folder, "report.txt"), "w", encoding="utf-8") as output:
        output.write("\n".join(lines + [summary]) + "\n")
    return failed


def write_sheets(flows, target, output_folder):
    folder = os.path.join(output_folder, "pictures")
    shutil.rmtree(folder, ignore_errors=True)
    os.makedirs(folder)
    if shutil.which("magick") is None:
        return 0
    written = 0
    for flow in flows:
        paths = [target.capture_path(target.sandbox_of(flow), name) for name in capture_names(flow)]
        paths = [path for path in paths if os.path.isfile(path)]
        if not paths:
            continue
        sheet = os.path.join(folder, flow_key(flow).replace("/", "_") + ".png")
        command = ["magick", "montage"] + paths + ["-tile", "3x", "-geometry", "480x272+4+4", "-background", "#202020", sheet]
        if subprocess.run(command, capture_output=True).returncode == 0:
            written += 1
    return written


def build_tools():
    built = subprocess.run(["make", "-C", SRC_VITA_FOLDER, "-j8", "build/smoke_host/smoke_host", "build/tests/golden/script_replay"], capture_output=True, text=True, errors="replace")
    result = Result("build the Mac app and the replay")
    if built.returncode != 0:
        result.fail("the Mac build failed")
        result.evidence = (built.stdout + built.stderr).splitlines()[-FAILURE_LINES_KEPT:]
    return result


def make_mac_target(set_folders, state_folders, output_folder, host_path=None):
    return MacTarget(host_path or os.path.join(SRC_VITA_FOLDER, "build", "smoke_host", "smoke_host"), set_folders, state_folders, output_folder)


def run(arguments, write=print, make_target=make_mac_target):
    output_folder = os.path.abspath(arguments.output)
    sets_folder = os.path.abspath(arguments.sets)
    states_root = os.path.abspath(arguments.states)
    rules = read_log_kinds()
    gaps = read_known_gaps()
    results = [build_tools()]
    if not results[0].passed:
        return report(results, gaps, output_folder, write)
    set_folders = {}
    stories = {}
    state_folders = {}
    for set_kind in arguments.set:
        folder = os.path.join(sets_folder, SET_FOLDERS[set_kind])
        found = Result("set %s is converted" % set_kind)
        if not os.path.isfile(os.path.join(folder, "manifest.txt")):
            found.fail("no converted set in %s; run tools/set_up_game.py" % folder)
            results.append(found)
            continue
        results.append(found)
        set_folders[set_kind] = folder
        state_folders[set_kind] = os.path.join(states_root, set_kind)
        results.append(check_manifest(set_kind, folder))
        results.append(check_texture_buckets(set_kind, folder))
        if set_kind == "remastered":
            results.append(check_languages(set_kind, folder))
        results.append(check_loop_points(set_kind, folder))
        results.append(check_route_is_current(set_kind, folder, os.path.join(TOOLS_FOLDER, "assets", SET_DESCRIPTIONS[set_kind])))
        if arguments.reuse_replay:
            replayed, story = earlier_replay(set_kind, folder, state_folders[set_kind])
        else:
            replayed, story = run_replay(set_kind, folder, state_folders[set_kind], os.path.join(SRC_VITA_FOLDER, "build", "tests", "golden", "script_replay"))
        results.append(replayed)
        if story is not None:
            stories[set_kind] = story
    if not arguments.no_package:
        results.append(check_package(output_folder))
    templates = parse_flows(open(FLOWS_PATH, encoding="utf-8").read())
    templates = [template for template in templates if all(set_kind in set_folders for install in template.installs for set_kind in INSTALLS[install]) or any(all(set_kind in set_folders for set_kind in INSTALLS[install]) for install in template.installs)]
    for template in templates:
        template.installs = [install for install in template.installs if all(set_kind in set_folders for set_kind in INSTALLS[install])]
    flows = expand_flows(templates, stories, arguments.quick, arguments.only)
    target = make_target(set_folders, state_folders, output_folder, arguments.host)
    flows, left_out = target.flows_it_can_run(flows)
    results += left_out
    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=min(arguments.jobs, target.flows_at_once)) as pool:
            flow_results = list(pool.map(lambda flow: run_flow(flow, target, rules), flows))
    finally:
        target.finish()
    results += flow_results
    results += difference_results(flows, flow_results, target)
    sheets = write_sheets(flows, target, output_folder)
    failed = report(results, gaps, output_folder, write)
    write("pictures: %d sheets in %s" % (sheets, os.path.relpath(os.path.join(output_folder, "pictures"), SRC_VITA_FOLDER)))
    return failed


def build_parser():
    parser = argparse.ArgumentParser(description="The smoke test: boots the app on this computer from the converted sets, plays the flows in tests/smoke/flows.txt through the pad and checks the sets, the log and the pictures.")
    parser.add_argument("--sets", default=os.path.join(SRC_VITA_FOLDER, "..", "extracted", "vita_assets"), help="folder holding the converted sets, game and game_ds")
    parser.add_argument("--states", default=os.path.join(SRC_VITA_FOLDER, "..", "extracted", "smoke", "states"), help="folder the replay writes each scene's start state into")
    parser.add_argument("--output", default=os.path.join(SRC_VITA_FOLDER, "build", "smoke"), help="folder for the sandboxes, pictures and report")
    parser.add_argument("--set", action="append", choices=sorted(SET_FOLDERS), help="check only this set; repeat for both (the default)")
    parser.add_argument("--quick", action="store_true", help="leave out the flows marked 'needs full', which play every chapter")
    parser.add_argument("--only", action="append", default=[], help="run only the flows whose name matches this pattern; may repeat")
    parser.add_argument("--no-package", action="store_true", help="leave out the Vita link and package check")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 4, help="flows run at once")
    parser.add_argument("--host", help="run this build of the Mac app in place of the one this tree builds; the mutation check passes its broken builds here")
    parser.add_argument("--reuse-replay", action="store_true", help="take the story and the start states from the last run's replay")
    parser.add_argument("--list", action="store_true", help="print the flows in the flow file and stop")
    return parser


def main():
    arguments = build_parser().parse_args()
    arguments.set = arguments.set or ["remastered", "ds"]
    if arguments.list:
        for template in parse_flows(open(FLOWS_PATH, encoding="utf-8").read()):
            print("%-28s %s" % (template.name, template.about))
        return 0
    return 1 if run(arguments) else 0


if __name__ == "__main__":
    sys.exit(main())
