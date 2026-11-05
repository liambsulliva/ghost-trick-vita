import argparse
import os
import re
import subprocess
import sys

SCREEN_SIZE = "960x540"
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_OUTPUT_FOLDER = os.path.join(SRC_VITA_FOLDER, "build", "compare")


def run_magick(arguments):
    return subprocess.run(["magick"] + arguments, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)


def normalize_image(source_path, output_path):
    result = run_magick([source_path, "-resize", SCREEN_SIZE + "!", "-alpha", "off", output_path])
    if result.returncode != 0:
        raise RuntimeError("could not read " + source_path + ": " + result.stderr.strip())


def difference_score(capture_path, reference_path, difference_path):
    result = run_magick(["compare", "-metric", "RMSE", capture_path, reference_path, difference_path])
    match = re.search(r"\(([0-9.e-]+)\)", result.stderr)
    if match is None:
        raise RuntimeError("could not read the compare score: " + result.stderr.strip())
    return float(match.group(1))


def make_side_by_side(capture_path, reference_path, difference_path, output_path):
    result = run_magick([capture_path, reference_path, difference_path, "+append", output_path])
    if result.returncode != 0:
        raise RuntimeError("could not build the side by side image: " + result.stderr.strip())


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("capture")
    parser.add_argument("reference")
    parser.add_argument("--name", default="comparison")
    parser.add_argument("--output", default=DEFAULT_OUTPUT_FOLDER)
    parser.add_argument("--max-score", type=float, default=None)
    arguments = parser.parse_args()

    os.makedirs(arguments.output, exist_ok=True)
    capture_path = os.path.join(arguments.output, arguments.name + "_capture.png")
    reference_path = os.path.join(arguments.output, arguments.name + "_reference.png")
    difference_path = os.path.join(arguments.output, arguments.name + "_difference.png")
    side_by_side_path = os.path.join(arguments.output, arguments.name + "_side_by_side.png")

    normalize_image(arguments.capture, capture_path)
    normalize_image(arguments.reference, reference_path)
    score = difference_score(capture_path, reference_path, difference_path)
    make_side_by_side(capture_path, reference_path, difference_path, side_by_side_path)

    print("score %.4f (0 is identical, 1 is completely different)" % score)
    print("side by side:", side_by_side_path)
    if arguments.max_score is not None and score > arguments.max_score:
        print("score is over the limit of %.4f" % arguments.max_score)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
