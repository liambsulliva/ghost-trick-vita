import argparse
import sys

OPENING_SCRIPT = "st01-st01_game000_Expand"
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


def order_problems(marks):
    last = -1
    for what, name in marks:
        if what == "route_step_done":
            index = int(name.split(":", 1)[0])
            if index <= last:
                return ["route step %d finished after step %d" % (index, last)]
            last = index
    return []


def route_problems(marks, route_path, most_passed_over):
    problems = ["%s %s" % mark for mark in marks if mark[0] in FAILURE_MARKS]
    done = len([mark for mark in marks if mark[0] == "route_step_done"])
    expected = step_count_of(route_path)
    if done < expected - most_passed_over:
        problems.append("only %d of %d route steps finished" % (done, expected))
    return problems


def main():
    parser = argparse.ArgumentParser(description="fail if a replay stopped following the route")
    parser.add_argument("marks", help="the output of script_replay --marks")
    parser.add_argument("route", help="the baked route_steps.txt")
    parser.add_argument("--most-passed-over", type=int, default=0, help="route steps the story may pass over without playing")
    parser.add_argument("--must-reach", help="a mark name the replay must end on, such as the last script change")
    arguments = parser.parse_args()
    marks = read_marks(arguments.marks)
    problems = opening_problems(marks) + order_problems(marks) + route_problems(marks, arguments.route, arguments.most_passed_over)
    if arguments.must_reach is not None and not any(name == arguments.must_reach.split()[0] for what, name in marks):
        problems.append("the replay never reached %s" % arguments.must_reach)
    for problem in problems:
        print(problem)
    print("route check", "failed" if problems else "passed", len(problems), "problems")
    return 1 if problems else 0


sys.exit(main())
