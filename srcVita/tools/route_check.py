import argparse
import sys

OPENING_SCRIPT = "st01-st01_game000_Expand"
PUSHES_STILL_EXPECTED = 0
EXPECTED_OPENING = [
    ("core", "shadanki"),
    ("trick", "shadanki"),
    ("core", "guitar"),
    ("trick", "guitar"),
    ("core", "cicel"),
    ("trick", "cicel"),
    ("act_begun", "gameAct070"),
]
FAILURE_MARKS = ("route_step_failed", "route_hop_beyond_reach")
ENDED_LINE_PREFIX = "ended "
PUSHES_WORD_PREFIX = "pushes="


def read_marks(path):
    marks = []
    with open(path, encoding="utf-8", errors="replace") as source:
        for line in source:
            words = line.split()
            if len(words) >= 3 and words[0] == "mark":
                marks.append((words[1], words[2]))
    return marks


def marks_in_script(marks, script):
    found = []
    is_inside = False
    for what, name in marks:
        if what == "script_begun":
            is_inside = name == script
        elif is_inside:
            found.append((what, name))
    return found


def opening_problems(marks):
    inside = marks_in_script(marks, OPENING_SCRIPT)
    position = 0
    for mark in EXPECTED_OPENING:
        try:
            position = inside.index(mark, position) + 1
        except ValueError:
            return ["the opening is missing or misordered at %s %s" % mark]
    return []


def read_route_steps(route_path):
    with open(route_path, encoding="ascii") as source:
        return [line.split() for line in source if line.startswith("step ")]


def step_count_of(route_path):
    return len(read_route_steps(route_path))


def order_problems(marks, route_path):
    wanted = [word[3] for word in read_route_steps(route_path) if word[2] == "move"]
    moved = [name for what, name in marks if what == "core"]
    if moved == wanted:
        return []
    for index, (got, expected) in enumerate(zip(moved, wanted)):
        if got != expected:
            return ["soul move %d went to %s, the route says %s" % (index, got, expected)]
    return ["the replay made %d soul moves, the route has %d" % (len(moved), len(wanted))]


def route_problems(marks, route_path):
    problems = ["%s %s" % mark for mark in marks if mark[0] in FAILURE_MARKS]
    done = len([mark for mark in marks if mark[0] == "route_step_done"])
    expected = step_count_of(route_path)
    if done < expected:
        problems.append("only %d of %d route steps finished" % (done, expected))
    return problems


def forced_pushes(marks_path):
    with open(marks_path, encoding="utf-8", errors="replace") as source:
        for line in source:
            if line.startswith(ENDED_LINE_PREFIX):
                for word in line.split():
                    if word.startswith(PUSHES_WORD_PREFIX):
                        return int(word[len(PUSHES_WORD_PREFIX):])
    return None


def push_problems(marks_path):
    pushes = forced_pushes(marks_path)
    if pushes is None:
        return ["the replay never reported how it ended"]
    if pushes > PUSHES_STILL_EXPECTED:
        return ["the replay needed %d forced pushes" % pushes]
    return []


def main():
    parser = argparse.ArgumentParser(description="fail if a replay stopped following the Chapter 1 route")
    parser.add_argument("marks", help="the output of script_replay --marks")
    parser.add_argument("route", help="the baked route_steps.txt")
    arguments = parser.parse_args()
    marks = read_marks(arguments.marks)
    problems = opening_problems(marks) + order_problems(marks, arguments.route) + route_problems(marks, arguments.route) + push_problems(arguments.marks)
    for problem in problems:
        print(problem)
    print("route check", "failed" if problems else "passed", len(problems), "problems")
    return 1 if problems else 0


sys.exit(main())
